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
#include "SysRegs.h"
#include <Library/CrcLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/SystemInfoLib.h>
#include <Library/CheckpointLib.h>
#include <Library/KtiApi.h>
#include <Library/ReferenceCodePolicyLib.h>
#include <Library/EmulationConfigurationLib.h>
#include <Library/IoAccessLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/TimerLib.h>
#include <Library/MemCallTableLib.h>
#include <Library/MemFmcIpLib.h>
#include <Library/SysHostPointerLib.h>
#include "Include/BaseMemoryCoreInternal.h"
#include <Library/SpdAccessLib.h>
#include <Library/SpdDecodeLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/MemRcLib.h>
#include <Memory/MemoryCheckpointCodes.h>
#include <Library/PchAdrLib.h>
#include <Library/MemBootHealthLib.h>
#include <Library/BasicIpBlockServicesLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/PprCmosLib.h>
#include <Ppi/MemoryPolicyPpi.h>
#include <Library/PcuIpLib.h>
#include <Guid/MrcPlatformHooksGuid.h>
#include <Library/PipeSyncLib.h>
#include <Register/Cpuid.h>


//
// Function declarations
//
UINT32
InitStructures (
  PSYSHOST Host,
  UINT8    socket
  );

UINT32
DetectDIMMEnabledChange (
  PSYSHOST Host,
  UINT8 socket
  );

VOID
InitSpdPageSelect (
  PSYSHOST Host,
  UINT8 socket
  );


/**
  Pass MMIO BAR and Mask of each iMC to PythonSV or CTE server.

  Each iMC has one MemBar##### string. Since the ChMask is 32-bit, so it could only marks 32 channels maximally.

  @param[in]   Host             - Pointer to sysHost structure
  @param[in]   Socket           - Socket number

  @retval   EFI_SUCCESS

**/
EFI_STATUS
PassMemBarAndMask (
  IN    PSYSHOST        Host,
  IN    UINT8           Socket
  )
{

  return EFI_SUCCESS;
}


#ifdef DEBUG_CODE_BLOCK
/**

    Print Memory Setup Options

    @param Host   - Pointer to SysHost
    @param socket - Processor Socket

    @retval None

**/
VOID
PrintMemSetupOptions (
  PSYSHOST Host,
  UINT8    socket
  )
{
  struct dimmNvram  (*dimmNvList)[MAX_DIMM];
  UINT8           j, k, l;
  UINT32          build_flag;
  SYS_SETUP       *Setup;
  UINT8           MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  Setup = GetSysSetupPointer ();

  AcquirePrintControl ();

  if (GetDebugLevel () & SDBG_MAX) {

    //
    // Output Build Option
    //

    RcDebugPrint (SDBG_DEFAULT, "\nBuild Option Summary\n");
    build_flag = 0;
    if (PcdGetBool (SaveBuiltInRmtResultToBdat)) {
      RcDebugPrint (SDBG_DEFAULT, "\nBDAT_SUPPORT BIT0 is defined\n");
      build_flag |= BIT0;
    }
    if (PcdGetBool (PcdMrcXmpSupport)) {
      RcDebugPrint (SDBG_DEFAULT, "\nXMP_SUPPORT BIT1 is defined\n");
      build_flag |= BIT1;
    }

    // PPR_SUPPORT
    RcDebugPrint (SDBG_DEFAULT, "\nPPR_SUPPORT BIT2 is defined\n");
    build_flag |= BIT2;
    // End PPR_SUPPORT
    RcDebugPrint (SDBG_DEFAULT, "\nSSA_FLAG BIT3 is defined\n");
    build_flag |= BIT3;

    RcDebugPrint (SDBG_DEFAULT, "\nMARGIN_CHECK BIT4 is defined\n");
    build_flag |= BIT4;

    if (PcdGetBool (PcdNvDimmEn)) {
      RcDebugPrint (SDBG_DEFAULT, "\nMEM_NVDIMM_EN BIT5 is defined\n");
      build_flag |= BIT5;
    }
#ifdef  LRDIMM_SUPPORT
    RcDebugPrint (SDBG_DEFAULT, "\nLRDIMM_SUPPORT BIT6 is defined\n");
    build_flag |= BIT6;
#endif

    if (PcdGetBool (PcdMrcQrDimmSupport) == TRUE) {
    RcDebugPrint (SDBG_DEFAULT, "\nQR_DIMM_SUPPORT BIT7 is defined\n");
      build_flag |= BIT7;
    }

    if (PcdGetBool (PcdMrcSoDimmSupport) == TRUE) {
    RcDebugPrint (SDBG_DEFAULT, "\nSODIMM_SUPPORT BIT8 is defined\n");
      build_flag |= BIT8;
    }

    RcDebugPrint (SDBG_DEFAULT, "\nADV_MEMTEST_SUPPORT BIT9 is defined\n");
    build_flag |= BIT9;
    RcDebugPrint (SDBG_DEFAULT, "\nbuild_flag:%x\n", build_flag);

  }

  RcDebugPrint (SDBG_DEFAULT, "\nsizeof memSetup     = %d\n", sizeof (struct memSetup));
  RcDebugPrint (SDBG_DEFAULT, "\nsizeof memNvram     = %d\n", sizeof (struct memNvram));
  RcDebugPrint (SDBG_DEFAULT, "\nsizeof socketNvram  = %d\n", sizeof (struct socketNvram));
  RcDebugPrint (SDBG_DEFAULT, "\nsizeof memVar       = %d\n", sizeof (struct memVar));
  RcDebugPrint (SDBG_DEFAULT, "\nsizeof Socket       = %d\n", sizeof (struct Socket));
  RcDebugPrint (SDBG_DEFAULT, "\nsizeof ddrChannel   = %d\n", sizeof (struct ddrChannel));
  RcDebugPrint (SDBG_DEFAULT, "\nsizeof dimmDevice   = %d\n", sizeof (struct dimmDevice));
  RcDebugPrint (SDBG_DEFAULT, "\nsizeof SADTable     = %d\n", sizeof (struct SADTable));
  RcDebugPrint (SDBG_DEFAULT, "\nsizeof TADTable     = %d\n", sizeof (struct TADTable));

  RcDebugPrint (SDBG_DEFAULT, "\nMAX_SOCKET = %d\n", MAX_SOCKET);

  //
  // Output sysHostSetup structure via serial debug messages
  //
  RcDebugPrint (SDBG_DEFAULT, "\nsizeof sysHostSetup = %d\n", sizeof (struct sysSetup));

  //
  // *** struct sysHostSetup ***
  //
  RcDebugPrint (SDBG_DEFAULT, "\nstruct sysHost.common {\n");
  RcDebugPrint (SDBG_DEFAULT, "  options:         %08X\n", Setup->common.options);
  RcDebugPrint (SDBG_DEFAULT, "      PROMOTE_WARN_EN     ");
  if (Setup->common.options & PROMOTE_WARN_EN) {
    RcDebugPrint (SDBG_DEFAULT, "1\n");
  } else  {
    RcDebugPrint (SDBG_DEFAULT, "0\n");
  }
  RcDebugPrint (SDBG_DEFAULT, "      HALT_ON_ERROR_EN    ");
  if (Setup->common.options & HALT_ON_ERROR_EN) {
    RcDebugPrint (SDBG_DEFAULT, "1\n");
  } else  {
    RcDebugPrint (SDBG_DEFAULT, "0\n");
  }
  RcDebugPrint (SDBG_DEFAULT, "  serialDebugMsgLvl:%02X\n", Setup->common.serialDebugMsgLvl);
  RcDebugPrint (SDBG_DEFAULT, "  maxAddrMem:      %02X\n", Setup->common.maxAddrMem);
  RcDebugPrint (SDBG_DEFAULT, "  debugPort:       %02X\n", Setup->common.debugPort);
  RcDebugPrint (SDBG_DEFAULT, "  nvramPtr:        %08X\n", Setup->common.nvramPtr);
  RcDebugPrint (SDBG_DEFAULT, "  sysHostBufferPtr:%08X\n", Setup->common.sysHostBufferPtr);
  RcDebugPrint (SDBG_DEFAULT, "  mmCfgBase:       %08X\n", GetmmCfgBase ());
  RcDebugPrint (SDBG_DEFAULT, "  mmCfgSize:       %08X\n", GetmmCfgSize ());
  RcDebugPrint (SDBG_DEFAULT, "  numaEn:          %02X\n", GetNumaEn ());
  RcDebugPrint (SDBG_DEFAULT, "  UmaBasedClustering:%02X\n", GetUmaBasedClustering ());
  RcDebugPrint (SDBG_DEFAULT, "\nstruct sysHost.setup.mem {\n");
  RcDebugPrint (SDBG_DEFAULT, "  options:         %08X\n", Setup->mem.options);
  RcDebugPrint (SDBG_DEFAULT, "      TEMPHIGH_EN         ");
  if (Setup->mem.options & TEMPHIGH_EN) {
    RcDebugPrint (SDBG_DEFAULT, "1\n");
  } else  {
    RcDebugPrint (SDBG_DEFAULT, "0\n");
  }
  RcDebugPrint (SDBG_DEFAULT, "      PDWN_SR_CKE_MODE    ");
  if (Setup->mem.options & PDWN_SR_CKE_MODE) {
    RcDebugPrint (SDBG_DEFAULT, "1\n");
  } else  {
    RcDebugPrint (SDBG_DEFAULT, "0\n");
  }
  RcDebugPrint (SDBG_DEFAULT, "      OPP_SELF_REF_EN     ");
  if (Setup->mem.options & OPP_SELF_REF_EN) {
    RcDebugPrint (SDBG_DEFAULT, "1\n");
  } else  {
    RcDebugPrint (SDBG_DEFAULT, "0\n");
  }
  RcDebugPrint (SDBG_DEFAULT, "      MDLL_SHUT_DOWN_EN   ");
  if (Setup->mem.options & MDLL_SHUT_DOWN_EN) {
    RcDebugPrint (SDBG_DEFAULT, "1\n");
  } else  {
    RcDebugPrint (SDBG_DEFAULT, "0\n");
  }
  RcDebugPrint (SDBG_DEFAULT, "      PAGE_POLICY         ");
  if (Setup->mem.options & PAGE_POLICY) {
    RcDebugPrint (SDBG_DEFAULT, "1\n");
  } else  {
    RcDebugPrint (SDBG_DEFAULT, "0\n");
  }
  RcDebugPrint (SDBG_DEFAULT, "      MULTI_THREAD_MRC_EN ");
  if (Setup->mem.options & MULTI_THREAD_MRC_EN) {
    RcDebugPrint (SDBG_DEFAULT, "1\n");
  } else  {
    RcDebugPrint (SDBG_DEFAULT, "0\n");
  }
  RcDebugPrint (SDBG_DEFAULT, "      ADAPTIVE_PAGE_EN    ");
  if (Setup->mem.options & ADAPTIVE_PAGE_EN) {
    RcDebugPrint (SDBG_DEFAULT, "1\n");
  } else  {
    RcDebugPrint (SDBG_DEFAULT, "0\n");
  }
  RcDebugPrint (SDBG_DEFAULT, "      SCRAMBLE_EN ");
  if (Setup->mem.options & SCRAMBLE_EN) {
    RcDebugPrint (SDBG_DEFAULT, "1\n");
  } else  {
    RcDebugPrint (SDBG_DEFAULT, "0\n");
  }
  //RcDebugPrint (SDBG_DEFAULT, "      RX_SENSE_AMP_EN     ");
  //if (Setup->mem.options & RX_SENSE_AMP_EN) RcDebugPrint (SDBG_DEFAULT, "1\n");
  //else RcDebugPrint (SDBG_DEFAULT, "0\n");
  RcDebugPrint (SDBG_DEFAULT, "      DDR_RESET_LOOP      ");
  if (Setup->mem.options & DDR_RESET_LOOP) {
    RcDebugPrint (SDBG_DEFAULT, "1\n");
  } else  {
    RcDebugPrint (SDBG_DEFAULT, "0\n");
  }
  RcDebugPrint (SDBG_DEFAULT, "      NUMA_AWARE          ");
  if (Setup->mem.options & NUMA_AWARE) {
    RcDebugPrint (SDBG_DEFAULT, "1\n");
  } else  {
    RcDebugPrint (SDBG_DEFAULT, "0\n");
  }
  RcDebugPrint (SDBG_DEFAULT, "      DISABLE_WMM_OPP_READ ");
  if (Setup->mem.options & DISABLE_WMM_OPP_READ) {
    RcDebugPrint (SDBG_DEFAULT, "1\n");
  } else  {
    RcDebugPrint (SDBG_DEFAULT, "0\n");
  }
  RcDebugPrint (SDBG_DEFAULT, "      ECC_CHECK_EN        ");
  if (Setup->mem.dfxMemSetup.dfxOptions & ECC_CHECK_EN) {
    RcDebugPrint (SDBG_DEFAULT, "1\n");
  } else  {
    RcDebugPrint (SDBG_DEFAULT, "0\n");
  }
  RcDebugPrint (SDBG_DEFAULT, "      ECC_MIX_EN ");
  if (Setup->mem.options & ECC_MIX_EN) {
    RcDebugPrint (SDBG_DEFAULT, "1\n");
  } else  {
    RcDebugPrint (SDBG_DEFAULT, "0\n");
  }
  RcDebugPrint (SDBG_DEFAULT, "      DISABLE_ECC_SUPPORT    ");
  if (Setup->mem.options & DISABLE_ECC_SUPPORT) {
    RcDebugPrint (SDBG_DEFAULT, "1\n");
  } else  {
    RcDebugPrint (SDBG_DEFAULT, "0\n");
  }
  RcDebugPrint (SDBG_DEFAULT, "      CA_PARITY_EN ");
  if (Setup->mem.dfxMemSetup.dfxOptions & CA_PARITY_EN) {
    RcDebugPrint (SDBG_DEFAULT, "1\n");
  } else  {
    RcDebugPrint (SDBG_DEFAULT, "0\n");
  }
  RcDebugPrint (SDBG_DEFAULT, "      MARGIN_RANKS_EN     ");
  if (Setup->mem.options & MARGIN_RANKS_EN) {
    RcDebugPrint (SDBG_DEFAULT, "1\n");
  } else {
    RcDebugPrint (SDBG_DEFAULT, "0\n");
  }

  if (PcdGetBool (PcdMrcXmpSupport)) {
    RcDebugPrint (SDBG_DEFAULT, "      MEM_OVERRIDE_EN     ");
    if (Setup->mem.options & MEM_OVERRIDE_EN) {
      RcDebugPrint (SDBG_DEFAULT, "1\n");
    } else {
      RcDebugPrint (SDBG_DEFAULT, "0\n");
    }
  }


  RcDebugPrint (SDBG_DEFAULT, "      DRAMDLL_OFF_PD_EN   ");
  if (Setup->mem.options & DRAMDLL_OFF_PD_EN) {
    RcDebugPrint (SDBG_DEFAULT, "1\n");
  } else  {
    RcDebugPrint (SDBG_DEFAULT, "0\n");
  }
  RcDebugPrint (SDBG_DEFAULT, "      MEMORY_TEST_EN      ");
  if (Setup->mem.options & MEMORY_TEST_EN) {
    RcDebugPrint (SDBG_DEFAULT, "1\n");
  } else  {
    RcDebugPrint (SDBG_DEFAULT, "0\n");
  }
  RcDebugPrint (SDBG_DEFAULT, "      MEMORY_TEST_COLD_FAST_BOOT_EN ");
  if (Setup->mem.options & MEMORY_TEST_COLD_FAST_BOOT_EN) {
    RcDebugPrint (SDBG_DEFAULT, "1\n");
  } else  {
    RcDebugPrint (SDBG_DEFAULT, "0\n");
  }
  RcDebugPrint (SDBG_DEFAULT, "      ATTEMPT_FAST_BOOT   ");
  if (Setup->mem.options & ATTEMPT_FAST_BOOT) {
    RcDebugPrint (SDBG_DEFAULT, "1\n");
  } else  {
    RcDebugPrint (SDBG_DEFAULT, "0\n");
  }
  RcDebugPrint (SDBG_DEFAULT, "      ATTEMPT_FAST_BOOT_COLD ");
  if (Setup->mem.options & ATTEMPT_FAST_BOOT_COLD) {
    RcDebugPrint (SDBG_DEFAULT, "1\n");
  } else  {
    RcDebugPrint (SDBG_DEFAULT, "0\n");
  }
  RcDebugPrint (SDBG_DEFAULT, "      SW_MEMORY_TEST_EN   ");
  if (Setup->mem.options & SW_MEMORY_TEST_EN) {
    RcDebugPrint (SDBG_DEFAULT, "1\n");
  } else  {
    RcDebugPrint (SDBG_DEFAULT, "0\n");
  }
  RcDebugPrint (SDBG_DEFAULT, "      RMT_COLD_FAST_BOOT   ");
  if (Setup->mem.options & RMT_COLD_FAST_BOOT) {
    RcDebugPrint (SDBG_DEFAULT, "1\n");
  } else  {
    RcDebugPrint (SDBG_DEFAULT, "0\n");
  }
  RcDebugPrint (SDBG_DEFAULT, "      DISPLAY_EYE_EN ");
  if (Setup->mem.options & DISPLAY_EYE_EN) {
    RcDebugPrint (SDBG_DEFAULT, "1\n");
  } else  {
    RcDebugPrint (SDBG_DEFAULT, "0\n");
  }
  RcDebugPrint (SDBG_DEFAULT, "      PER_NIBBLE_EYE_EN ");
  if (Setup->mem.options & PER_NIBBLE_EYE_EN) {
    RcDebugPrint (SDBG_DEFAULT, "1\n");
  } else  {
    RcDebugPrint (SDBG_DEFAULT, "0\n");
  }
  RcDebugPrint (SDBG_DEFAULT, "      ALLOW2XREF_EN ");
  if (Setup->mem.options & ALLOW2XREF_EN) {
    RcDebugPrint (SDBG_DEFAULT, "1\n");
  } else {
    RcDebugPrint(SDBG_DEFAULT, "0\n");
  }
  RcDebugPrint (SDBG_DEFAULT, " optionsExt: %08X\n", Setup->mem.optionsExt);
  RcDebugPrint (SDBG_DEFAULT, "  SPD_CRC_CHECK ");
  if (Setup->mem.optionsExt & SPD_CRC_CHECK) {
    RcDebugPrint (SDBG_DEFAULT, "1\n");
  } else  {
    RcDebugPrint (SDBG_DEFAULT, "0\n");
  }
  RcDebugPrint (SDBG_DEFAULT, "  PDA_EN ");
  if (Setup->mem.optionsExt & PDA_EN) {
    RcDebugPrint (SDBG_DEFAULT, "1\n");
  } else  {
    RcDebugPrint (SDBG_DEFAULT, "0\n");
  }
  RcDebugPrint (SDBG_DEFAULT, "  TURNAROUND_OPT_EN ");
  if (Setup->mem.optionsExt & TURNAROUND_OPT_EN) {
    RcDebugPrint (SDBG_DEFAULT, "1\n");
  } else  {
    RcDebugPrint (SDBG_DEFAULT, "0\n");
  }
  RcDebugPrint (SDBG_DEFAULT, "  TURNAROUND_OPT_EN_DDRT ");
  if (Setup->mem.optionsExt & TURNAROUND_OPT_EN_DDRT) {
    RcDebugPrint (SDBG_DEFAULT, "1\n");
  } else  {
    RcDebugPrint (SDBG_DEFAULT, "0\n");
  }
  RcDebugPrint (SDBG_DEFAULT, "  DRAM_RX_EQ_EN ");
  if ((Setup->mem.optionsExt & DRAM_RX_EQ_EN) != 0) {
    RcDebugPrint (SDBG_DEFAULT, "1\n");
  } else {
    RcDebugPrint (SDBG_DEFAULT, "0\n");
  }
  RcDebugPrint (SDBG_DEFAULT, "  TX_ECHO_CANCEL_EN ");
  if ((Setup->mem.optionsExt & TX_ECHO_CANCEL_EN) != 0) {
    RcDebugPrint (SDBG_DEFAULT, "1\n");
  } else {
    RcDebugPrint (SDBG_DEFAULT, "0\n");
  }
  RcDebugPrint (SDBG_DEFAULT, "  PER_BIT_MARGINS  ");
  if (Setup->mem.optionsExt & PER_BIT_MARGINS) {
    RcDebugPrint (SDBG_DEFAULT, "1\n");
  } else  {
    RcDebugPrint (SDBG_DEFAULT, "0\n");
  }
  RcDebugPrint (SDBG_DEFAULT, "  ALLOW_CORRECTABLE_ERROR  ");
  if (Setup->mem.optionsExt & ALLOW_CORRECTABLE_ERROR) {
    RcDebugPrint (SDBG_DEFAULT, "1\n");
  } else {
    RcDebugPrint (SDBG_DEFAULT, "0\n");
  }
  RcDebugPrint (SDBG_DEFAULT, "  ALLOW_CORRECTABLE_MEM_TEST_ERROR  ");
  if (Setup->mem.optionsExt & ALLOW_CORRECTABLE_MEM_TEST_ERROR) {
    RcDebugPrint (SDBG_DEFAULT, "1\n");
  } else {
    RcDebugPrint (SDBG_DEFAULT, "0\n");
  }

  RcDebugPrint (SDBG_DEFAULT, "   MEM_FLOWS - X_OVER_EN   %d\n", IsMemFlowEnabled (CrossoverCalibration));
  RcDebugPrint (SDBG_DEFAULT, "   MEM_FLOWS - SENSE_AMP_EN   %d\n", IsMemFlowEnabled (SenseAmpOffsetTraining));
  RcDebugPrint (SDBG_DEFAULT, "   MEM_FLOWS - E_CMDCLK_EN   %d\n", IsMemFlowEnabled (EarlyCmdClkTraining));
  RcDebugPrint (SDBG_DEFAULT, "   MEM_FLOWS - REC_EN_EN   %d\n", IsMemFlowEnabled (ReceiveEnableTraining));
  RcDebugPrint (SDBG_DEFAULT, "   MEM_FLOWS - RD_DQS_EN   %d\n", IsMemFlowEnabled (ReadDqDqsTraining));
  RcDebugPrint (SDBG_DEFAULT, "   MEM_FLOWS - WR_LVL_EN   %d\n", IsMemFlowEnabled (WriteLevelingTraining));
  RcDebugPrint (SDBG_DEFAULT, "   MEM_FLOWS - WR_FLYBY_EN   %d\n", IsMemFlowEnabled (WriteLevelingFlybyCleanup));
  RcDebugPrint (SDBG_DEFAULT, "   MEM_FLOWS - WR_DQ_EN   %d\n", IsMemFlowEnabled (WriteDqDqsTraining));
  RcDebugPrint (SDBG_DEFAULT, "   MEM_FLOWS - CMDCLK_EN   %d\n", IsMemFlowEnabled (LateCmdClkTraining));
  RcDebugPrint (SDBG_DEFAULT, "   MEM_FLOWS - RD_ADV_EN   %d\n", IsMemFlowEnabled (ReadAdvancedCenteringTraining));
  RcDebugPrint (SDBG_DEFAULT, "   MEM_FLOWS - WR_ADV_EN   %d\n", IsMemFlowEnabled (WriteAdvancedCenteringTraining));
  RcDebugPrint (SDBG_DEFAULT, "   MEM_FLOWS - RD_VREF_EN   %d\n", IsMemFlowEnabled (ReadVrefTraining));
  RcDebugPrint (SDBG_DEFAULT, "   MEM_FLOWS - WR_VREF_EN   %d\n", IsMemFlowEnabled (WriteVrefTraining));
  RcDebugPrint (SDBG_DEFAULT, "   MEM_FLOWS - RT_OPT_EN   %d\n", IsMemFlowEnabled (RoundTripOptimizeTraining));
  RcDebugPrint (SDBG_DEFAULT, "   MEM_FLOWS - RX_DESKEW_EN   %d\n", IsMemFlowEnabled (RxTxPerBitDeskewTraining));
  RcDebugPrint (SDBG_DEFAULT, "   MEM_FLOWS - TX_DESKEW_EN   %d\n", IsMemFlowEnabled (RxTxPerBitDeskewTraining));
  RcDebugPrint (SDBG_DEFAULT, "   MEM_FLOWS - TX_EQ_EN   %d\n", IsMemFlowEnabled (TxEqTraining));
  RcDebugPrint (SDBG_DEFAULT, "   MEM_FLOWS - IMODE_EN   %d\n", IsMemFlowEnabled (ImodeTraining));
  RcDebugPrint (SDBG_DEFAULT, "   MEM_FLOWS - EARLY_RID_EN   %d\n", IsMemFlowEnabled (EarlyReadIdTraining));
  RcDebugPrint (SDBG_DEFAULT, "   MEM_FLOWS - DQ_SWIZ_EN   %d\n", IsMemFlowEnabled (DqSwizzlingDiscovery));
  RcDebugPrint (SDBG_DEFAULT, "   MEM_FLOWS - LRBUF_RD_EN   %d\n", IsMemFlowEnabled (LrdimmBacksideRxTraining));
  RcDebugPrint (SDBG_DEFAULT, "   MEM_FLOWS - LRBUF_WR_EN   %d\n", IsMemFlowEnabled (LrdimmBacksideTxTraining));
  RcDebugPrint (SDBG_DEFAULT, "   MEM_FLOWS - RANK_MARGIN_EN   %d\n", IsMemFlowEnabled (LegacyRankMarginTool));

  RcDebugPrint (SDBG_DEFAULT, "   MEM_FLOWS - MEMINIT_EN   %d\n", IsMemFlowEnabled (MemInitialization));
  RcDebugPrint (SDBG_DEFAULT, "   MEM_FLOWS - MEMTEST_EN   %d\n", IsMemFlowEnabled (AdvancedMemoryTest));
  RcDebugPrint (SDBG_DEFAULT, "   MEM_FLOWS - NORMAL_MODE_EN   %d\n", IsMemFlowEnabled (NormalModeSwitch));
  RcDebugPrint (SDBG_DEFAULT, "   MEM_FLOWS - E_CTLCLK_EN   %d\n", IsMemFlowEnabled (EarlyCtlClkTraining));

  RcDebugPrint (SDBG_DEFAULT, "   MEM_FLOWS_EXT - MF_EXT_CSCLK_EN   %d\n", IsMemFlowEnabled (EarlyCsClkTraining));
  RcDebugPrint (SDBG_DEFAULT, "   MEM_FLOWS_EXT - RX_CTLE_EN   %d\n", IsMemFlowEnabled (RxCtleTraining));
  RcDebugPrint (SDBG_DEFAULT, "   MEM_FLOWS_EXT - MF_EXT_QxCA_CLK_EN   %d\n", IsMemFlowEnabled (BacksideCmdCtlTraining));
  RcDebugPrint (SDBG_DEFAULT, "   MEM_FLOWS_EXT - MF_EXT_PXC_EN   %d\n", IsMemFlowEnabled (PhaseCrosstalkTraining));
  RcDebugPrint (SDBG_DEFAULT, "   MEM_FLOWS_EXT - MF_EXT_BIOS_SSA_RMT_EN   %d\n", IsMemFlowEnabled (SsaRankMarginTool));
  RcDebugPrint (SDBG_DEFAULT, "   MEM_FLOWS_EXT - MF_EXT_LRDIMM_BKSIDE_EN   %d\n", IsMemFlowEnabled (LrdimmBacksideDqTraining));
  RcDebugPrint (SDBG_DEFAULT, "   MEM_FLOWS_EXT - CMD_TX_EQ_EN   %d\n", IsMemFlowEnabled (CmdTxEqTraining));
  RcDebugPrint (SDBG_DEFAULT, "   MEM_FLOWS_EXT - MF_EXT_RCOMP_STAT_LEG   %d\n", IsMemFlowEnabled (RcompStaticLeg));
  RcDebugPrint (SDBG_DEFAULT, "   MEM_FLOWS_EXT - MF_EXT_RX_DFE_EN   %d\n", IsMemFlowEnabled (RxDfeTraining));
  RcDebugPrint (SDBG_DEFAULT, "   MEM_FLOWS_EXT - MF_DQ_SWIZ_X16_EN   %d\n", IsMemFlowEnabled (DqSwizzlingDiscoveryX16));

  RcDebugPrint (SDBG_DEFAULT, "\nstruct sysHost.setup.mem {\n");
  RcDebugPrint (SDBG_DEFAULT, "  imcBclk:         %02X\n", Setup->mem.imcBclk);
  RcDebugPrint (SDBG_DEFAULT, "  enforcePOR:      %02X\n", Setup->mem.enforcePOR);
  RcDebugPrint (SDBG_DEFAULT, "  EnforcePopulationPor: %02X\n", Setup->mem.EnforcePopulationPor);
  RcDebugPrint (SDBG_DEFAULT, "  ddrFreqLimit:    %02X\n", Setup->mem.ddrFreqLimit);
  RcDebugPrint (SDBG_DEFAULT, "  PanicWm:         %02X\n", Setup->mem.PanicWm);
  RcDebugPrint (SDBG_DEFAULT, "  chInter:         %02X\n", Setup->mem.chInter);
  RcDebugPrint (SDBG_DEFAULT, "  dimmTypeSupport: %02X\n", Setup->mem.dimmTypeSupport);
  RcDebugPrint (SDBG_DEFAULT, "  pprType:         %02X\n", Setup->mem.pprType);
  for (j = 0; j < MAX_PPR_ADDR_ENTRIES; j++) {
     RcDebugPrint (SDBG_DEFAULT, "  pprAddrSetup[%d].pprAddrStatus    %d\n", j, Setup->mem.pprAddrSetup[j].pprAddrStatus);
     RcDebugPrint (SDBG_DEFAULT, "  pprAddrSetup[%d].socket    %02d\n", j, Setup->mem.pprAddrSetup[j].socket);
     RcDebugPrint (SDBG_DEFAULT, "  pprAddrSetup[%d].mc    %02d\n", j, Setup->mem.pprAddrSetup[j].mc);
     RcDebugPrint (SDBG_DEFAULT, "  pprAddrSetup[%d].pprAddr.ch    %02d\n", j, Setup->mem.pprAddrSetup[j].ch);
     RcDebugPrint (SDBG_DEFAULT, "  pprAddrSetup[%d].pprAddr.dimm    %02d\n", j, Setup->mem.pprAddrSetup[j].pprAddr.dimm);
     RcDebugPrint (SDBG_DEFAULT, "  pprAddrSetup[%d].pprAddr.rank    %02d\n", j, Setup->mem.pprAddrSetup[j].pprAddr.rank);
     RcDebugPrint (SDBG_DEFAULT, "  pprAddrSetup[%d].pprAddr.subRank    %02d\n", j, Setup->mem.pprAddrSetup[j].pprAddr.subRank);
     RcDebugPrint (SDBG_DEFAULT, "  pprAddrSetup[%d].pprAddr.bank    %02d\n", j, Setup->mem.pprAddrSetup[j].pprAddr.bank);
     RcDebugPrint (SDBG_DEFAULT, "  pprAddrSetup[%d].pprAddr.row    %x\n", j, Setup->mem.pprAddrSetup[j].pprAddr.row);
     RcDebugPrint (SDBG_DEFAULT, "  pprAddrSetup[%d].pprAddr.nibbleMask    %x\n", j, Setup->mem.pprAddrSetup[j].pprAddr.nibbleMask);
     RcDebugPrint (SDBG_DEFAULT, "  *********************************************\n");
  }
  RcDebugPrint (SDBG_DEFAULT, "  EnableTapSweep = %d\n", Setup->mem.EnableTapSweep);
  RcDebugPrint (SDBG_DEFAULT, "  DfeGainBias = %d\n", Setup->mem.DfeGainBias);

  RcDebugPrint (SDBG_DEFAULT, "  Tap1Start  = %d\n", Setup->mem.Tap1Start);
  RcDebugPrint (SDBG_DEFAULT, "  Tap1End    = %d\n", Setup->mem.Tap1End  );
  RcDebugPrint (SDBG_DEFAULT, "  Tap1Size   = %d\n", Setup->mem.Tap1Size );

  RcDebugPrint (SDBG_DEFAULT, "  Tap2Start  = %d\n", Setup->mem.Tap2Start);
  RcDebugPrint (SDBG_DEFAULT, "  Tap2End    = %d\n", Setup->mem.Tap2End  );
  RcDebugPrint (SDBG_DEFAULT, "  Tap2Size   = %d\n", Setup->mem.Tap2Size );

  RcDebugPrint (SDBG_DEFAULT, "  Tap3Start  = %d\n", Setup->mem.Tap3Start);
  RcDebugPrint (SDBG_DEFAULT, "  Tap3End    = %d\n", Setup->mem.Tap3End  );
  RcDebugPrint (SDBG_DEFAULT, "  Tap3Size   = %d\n", Setup->mem.Tap3Size );

  RcDebugPrint (SDBG_DEFAULT, "  Tap4Start  = %d\n", Setup->mem.Tap4Start);
  RcDebugPrint (SDBG_DEFAULT, "  Tap4End    = %d\n", Setup->mem.Tap4End  );
  RcDebugPrint (SDBG_DEFAULT, "  Tap4Size   = %d\n", Setup->mem.Tap4Size );



  RcDebugPrint (SDBG_DEFAULT, "  ckeThrottling:   %02X\n", Setup->mem.ckeThrottling);
  RcDebugPrint (SDBG_DEFAULT, "  olttPeakBWLIMITPercent: %02X\n", Setup->mem.olttPeakBWLIMITPercent);
  RcDebugPrint (SDBG_DEFAULT, "  thermalThrottlingOptions:  %04X\n", Setup->mem.thermalThrottlingOptions);
  RcDebugPrint (SDBG_DEFAULT, "      CLTT_EN             ");
  if (Setup->mem.thermalThrottlingOptions & CLTT_EN) {
    RcDebugPrint (SDBG_DEFAULT, "1\n");
  } else  {
    RcDebugPrint (SDBG_DEFAULT, "0\n");
  }
  RcDebugPrint (SDBG_DEFAULT, "      CLTT_EN with PECI   ");
  if (Setup->mem.thermalThrottlingOptions & CLTT_PECI_EN) {
    RcDebugPrint (SDBG_DEFAULT, "1\n");
  } else {
    RcDebugPrint (SDBG_DEFAULT, "0\n");
  }
  RcDebugPrint (SDBG_DEFAULT, "      OLTT_EN             ");
  if (Setup->mem.thermalThrottlingOptions & OLTT_EN) {
    RcDebugPrint (SDBG_DEFAULT, "1\n");
  } else  {
    RcDebugPrint (SDBG_DEFAULT, "0\n");
  }
  RcDebugPrint (SDBG_DEFAULT, "      MH_OUTPUT_EN        ");
  if (Setup->mem.thermalThrottlingOptions & MH_OUTPUT_EN) {
    RcDebugPrint (SDBG_DEFAULT, "1\n");
  } else  {
    RcDebugPrint (SDBG_DEFAULT, "0\n");
  }
  RcDebugPrint (SDBG_DEFAULT, "      MH_SENSE_EN         ");
  if (Setup->mem.thermalThrottlingOptions & MH_SENSE_EN) {
    RcDebugPrint (SDBG_DEFAULT, "1\n");
  } else  {
    RcDebugPrint (SDBG_DEFAULT, "0\n");
  }

  RcDebugPrint (SDBG_DEFAULT, "  DramRaplEnable:      %02X\n", Setup->mem.DramRaplEnable);
  RcDebugPrint (SDBG_DEFAULT, "  dramraplbwlimittf: %02X\n", Setup->mem.dramraplbwlimittf);
  RcDebugPrint (SDBG_DEFAULT, "  customRefreshRate: %02X\n", Setup->mem.customRefreshRate);
  RcDebugPrint (SDBG_DEFAULT, "  enableBiosSsaLoader: %02X\n", Setup->mem.enableBiosSsaLoader);


  RcDebugPrint (SDBG_DEFAULT, "  rmtPatternLength:%02X\n", Setup->mem.rmtPatternLength);
  RcDebugPrint (SDBG_DEFAULT, "  rmtPatternLengthExt(CMD/CTL):%02X\n", Setup->mem.rmtPatternLengthExt);

  RcDebugPrint (SDBG_DEFAULT, "  enableNgnBcomMargining: %02X\n", Setup->mem.enableNgnBcomMargining);
  RcDebugPrint (SDBG_DEFAULT, "  patrolScrubDuration:%02X\n", Setup->mem.patrolScrubDuration);
  RcDebugPrint (SDBG_DEFAULT, "  AdvMemTestOptions:    %02X\n", Setup->mem.AdvMemTestOptions);
  RcDebugPrint (SDBG_DEFAULT, "  AdvMemTestResetList:    %02X\n", Setup->mem.AdvMemTestResetList);

  RcDebugPrint (SDBG_DEFAULT, "  AdvMemTestCondition:    %02X\n", Setup->mem.AdvMemTestCondition);
  RcDebugPrint (SDBG_DEFAULT, "  AdvMemTestCondVdd:    %02X\n", Setup->mem.AdvMemTestCondVdd);
  RcDebugPrint (SDBG_DEFAULT, "  AdvMemTestCondTwr:    %02X\n", Setup->mem.AdvMemTestCondTwr);
  RcDebugPrint (SDBG_DEFAULT, "  AdvMemTestCondTrefi:    %02X\n", Setup->mem.AdvMemTestCondTrefi);
  RcDebugPrint (SDBG_DEFAULT, "  AdvMemTestCondPause:    %02X\n", Setup->mem.AdvMemTestCondPause);

  RcDebugPrint (SDBG_DEFAULT, "  memTestLoops:    %02X\n", Setup->mem.memTestLoops);
  RcDebugPrint (SDBG_DEFAULT, "  trainingResultOffsetFunctionEnable:    %02X\n", Setup->mem.trainingResultOffsetFunctionEnable);
  //RcDebugPrint (SDBG_DEFAULT, "  errLedBitOverride: %02X\n", Setup->mem.errLedBitOverride);
  RcDebugPrint (SDBG_DEFAULT, "  scrambleSeedLow: %08X\n", Setup->mem.scrambleSeedLow);
  RcDebugPrint (SDBG_DEFAULT, "  scrambleSeedHigh:%08X\n", Setup->mem.scrambleSeedHigh);
  RcDebugPrint (SDBG_DEFAULT, "  ADREn:           %02X\n", Setup->mem.ADREn);
  RcDebugPrint (SDBG_DEFAULT, "  LegacyADRModeEn: %02X\n", Setup->mem.LegacyADRModeEn);
  RcDebugPrint (SDBG_DEFAULT, "  MinNormalMemSize:%02X\n", Setup->mem.MinNormalMemSize);

  if (PcdGetBool (PcdNvDimmEn)) {
    RcDebugPrint (SDBG_DEFAULT, "  eraseArmNVDIMMS: %02X\n", Setup->mem.eraseArmNVDIMMS);
    RcDebugPrint (SDBG_DEFAULT, "  restoreNVDIMMS: %02X\n", Setup->mem.restoreNVDIMMS);
    if (Setup->mem.ADRDataSaveMode == ADR_NVDIMM) {
      RcDebugPrint (SDBG_DEFAULT, "  ADRDataSaveMode: ADR_NVDIMM\n");
    } else if (Setup->mem.ADRDataSaveMode == ADR_BBU) {
      RcDebugPrint (SDBG_DEFAULT, "  ADRDataSaveMode: ADR_BBU\n");
    } else {
      RcDebugPrint (SDBG_DEFAULT, "  ADRDataSaveMode: ADR_DIS\n");
    }
  }
  RcDebugPrint (SDBG_DEFAULT, "  check_pm_sts:    %02X\n", Setup->mem.check_pm_sts);
  RcDebugPrint (SDBG_DEFAULT, "  check_platform_detect: %02X\n", Setup->mem.check_platform_detect);
  RcDebugPrint (SDBG_DEFAULT, "  normOppIntvl:  %04x\n", Setup->mem.normOppIntvl);
  RcDebugPrint (SDBG_DEFAULT, "  SpdSmbSpeed: %02X\n", Setup->mem.SpdSmbSpeed);
  RcDebugPrint (SDBG_DEFAULT, "  SpdPrintEn: %02X\n", Setup->mem.SpdPrintEn);
  RcDebugPrint (SDBG_DEFAULT, "  SpdPrintLength: %04X\n", Setup->mem.SpdPrintLength);
  RcDebugPrint (SDBG_DEFAULT, "  Cmd setup/hold: %d\n", Setup->mem.cmdSetupPercentOffset);
  //
  // * struct ddrChannelSetup *
  //
  for (j = 0; j < MaxChDdr; j++) {
    dimmNvList = GetDimmNvList (Host, socket, j);
    RcDebugPrint (SDBG_DEFAULT, "    struct ddrChannelSetup[%02X] {\n", j);
    RcDebugPrint (SDBG_DEFAULT, "      enabled:       %02X\n", Setup->mem.socket[socket].ddrCh[j].enabled);
    RcDebugPrint (SDBG_DEFAULT, "      numDimmSlots:  %02X\n", Setup->mem.socket[socket].ddrCh[j].numDimmSlots);
    RcDebugPrint (SDBG_DEFAULT, "      batterybacked: %02X\n", Setup->mem.socket[socket].ddrCh[j].batterybacked);
    for (k = 0; k < MAX_DIMM; k++) {
      RcDebugPrint (SDBG_DEFAULT, "      struct ddrDimmSetup[%02X] {\n", k);
      for (l = 0; l < MAX_RANK_DIMM; l++) {
        RcDebugPrint (SDBG_DEFAULT, "        mapOut[%02X]: %02X\n", l, Setup->mem.socket[socket].ddrCh[j].dimmList[k].mapOut[l]);
      }
      RcDebugPrint (SDBG_DEFAULT, "      };\n");
    }
    RcDebugPrint (SDBG_DEFAULT, "    };\n");
  }

  if (PcdGetBool (PcdMrcXmpSupport)) {
    RcDebugPrint (SDBG_DEFAULT, "  struct memTiming {\n");
    RcDebugPrint (SDBG_DEFAULT, "    nCL:            %02X\n", Setup->mem.inputMemTime.nCL);
    RcDebugPrint (SDBG_DEFAULT, "    nRP:            %02X\n", Setup->mem.inputMemTime.nRP);
    RcDebugPrint (SDBG_DEFAULT, "    nRCD:           %02X\n", Setup->mem.inputMemTime.nRCD);
    RcDebugPrint (SDBG_DEFAULT, "    nRRD:           %02X\n", Setup->mem.inputMemTime.nRRD);
    RcDebugPrint (SDBG_DEFAULT, "    nRRD_L          %02X\n", Setup->mem.inputMemTime.nRRD_L);
    RcDebugPrint (SDBG_DEFAULT, "    nWTR:           %02X\n", Setup->mem.inputMemTime.nWTR);
    RcDebugPrint (SDBG_DEFAULT, "    nRAS:           %02X\n", Setup->mem.inputMemTime.nRAS);
    RcDebugPrint (SDBG_DEFAULT, "    nRTP:           %02X\n", Setup->mem.inputMemTime.nRTP);
    RcDebugPrint (SDBG_DEFAULT, "    nWR:            %02X\n", Setup->mem.inputMemTime.nWR);
    RcDebugPrint (SDBG_DEFAULT, "    nFAW:           %02X\n", Setup->mem.inputMemTime.nFAW);
    RcDebugPrint (SDBG_DEFAULT, "    nRC:            %02X\n", Setup->mem.inputMemTime.nRC);
    RcDebugPrint (SDBG_DEFAULT, "    nCWL:           %02X\n", Setup->mem.inputMemTime.nCWL);
    RcDebugPrint (SDBG_DEFAULT, "    nCMDRate:       %02X\n", Setup->mem.inputMemTime.nCMDRate);
    RcDebugPrint (SDBG_DEFAULT, "    ddrFreqLimit:   %02X\n", Setup->mem.inputMemTime.ddrFreqLimit);
    RcDebugPrint (SDBG_DEFAULT, "    vdd:            %02X\n", Setup->mem.inputMemTime.vdd);
    RcDebugPrint (SDBG_DEFAULT, "    ucVolt:         %02X\n", Setup->mem.inputMemTime.ucVolt);
    RcDebugPrint (SDBG_DEFAULT, "    casSup:         %02X\n", Setup->mem.inputMemTime.casSup);
    RcDebugPrint (SDBG_DEFAULT, "    tREFI:          %02X\n", Setup->mem.inputMemTime.tREFI);
    RcDebugPrint (SDBG_DEFAULT, "    nRFC:           %02X\n", Setup->mem.inputMemTime.nRFC);
    RcDebugPrint (SDBG_DEFAULT, "    ddrFreq:        %02X\n", Setup->mem.inputMemTime.ddrFreq);
    RcDebugPrint (SDBG_DEFAULT, "  };\n");
  }

#if defined(ME_SUPPORT_FLAG) && ME_SUPPORT_FLAG
  RcDebugPrint (SDBG_DEFAULT, "  meRequestedSize:  %08X\n", Setup->mem.meRequestedSize);
#endif // ME_SUPPORT_FLAG
#if defined(IE_SUPPORT) && IE_SUPPORT
  RcDebugPrint (SDBG_DEFAULT, "  ieRequestedSize:  %08X\n", Setup->mem.ieRequestedSize);
#endif // IE_SUPPORT
  RcDebugPrint (SDBG_DEFAULT, "    dfxOptions(BIT0->HIGH_ADDR_EN): ");
  if (Setup->mem.dfxMemSetup.dfxOptions & HIGH_ADDR_EN) {
    RcDebugPrint (SDBG_DEFAULT, "1\n");
  } else  {
    RcDebugPrint (SDBG_DEFAULT, "0\n");
  }
  RcDebugPrint (SDBG_DEFAULT, "    DfxBankGroupInterleavingMode: 0x%02X\n", Setup->mem.dfxMemSetup.DfxBankGroupInterleavingMode);
  RcDebugPrint (SDBG_DEFAULT, "    dfxHighAddrBitStart:  0x%02X\n", Setup->mem.dfxMemSetup.dfxHighAddrBitStart);
  RcDebugPrint (SDBG_DEFAULT, "    dfxStraddleHighAddressBit:  0x%02X\n", Setup->mem.dfxMemSetup.dfxStraddleHighAddressBit);
  RcDebugPrint (SDBG_DEFAULT, "    dfxLowMemChannel:     0x%02X\n", Setup->mem.dfxMemSetup.dfxLowMemChannel);
  RcDebugPrint (SDBG_DEFAULT, "    DfxDdrtRt:     0x%02X\n", Setup->mem.dfxMemSetup.DfxDdrtRt);
  RcDebugPrint (SDBG_DEFAULT, "    DfxInputRt:     0x%02X\n", Setup->mem.dfxMemSetup.DfxInputRt);
  RcDebugPrint (SDBG_DEFAULT, "    DfxInputIoLatency:     0x%02X\n", Setup->mem.dfxMemSetup.DfxInputIoLatency);
  RcDebugPrint (SDBG_DEFAULT, "    DfxInputTxDqDelay:     0x%04X\n", Setup->mem.dfxMemSetup.DfxInputTxDqDelay);
  RcDebugPrint (SDBG_DEFAULT, "    DfxInputTxDqsDelay:     0x%04X\n", Setup->mem.dfxMemSetup.DfxInputTxDqsDelay);
  RcDebugPrint (SDBG_DEFAULT, "    DfxInputTxDqBitDelay:     0x%04X\n", Setup->mem.dfxMemSetup.DfxInputTxDqBitDelay);
  RcDebugPrint (SDBG_DEFAULT, "    DfxInputRxDqDelay:     0x%02X\n", Setup->mem.dfxMemSetup.DfxInputRxDqDelay);
  RcDebugPrint (SDBG_DEFAULT, "    DfxInputRxDqsDelay:     0x%04X\n", Setup->mem.dfxMemSetup.DfxInputRxDqsDelay);
  RcDebugPrint (SDBG_DEFAULT, "    DfxInputRxDqBitDelay:     0x%02X\n", Setup->mem.dfxMemSetup.DfxInputRxDqBitDelay);
  RcDebugPrint (SDBG_DEFAULT, "    DfxRcvEnDelay:     0x%04X\n", Setup->mem.dfxMemSetup.DfxRcvEnDelay);
  RcDebugPrint (SDBG_DEFAULT, "    DfxDdrtRcvEnDelay:     0x%04X\n", Setup->mem.dfxMemSetup.DfxDdrtRcvEnDelay);
  RcDebugPrint (SDBG_DEFAULT, "    DfxInputGnt2Erid:     0x%04X\n", Setup->mem.dfxMemSetup.DfxInputGnt2Erid);
  RcDebugPrint (SDBG_DEFAULT, "    DfxInputIoLatencyIoComp:     0x%02X\n", Setup->mem.dfxMemSetup.DfxInputIoLatencyIoComp);
  RcDebugPrint (SDBG_DEFAULT, "    DfxCwlAdjPos:     0x%02X\n", Setup->mem.dfxMemSetup.DfxCwlAdjPos);
  RcDebugPrint (SDBG_DEFAULT, "    DfxCwlAdjNeg:     0x%02X\n", Setup->mem.dfxMemSetup.DfxCwlAdjNeg);
  RcDebugPrint (SDBG_DEFAULT, "    DfxUbiosSmbusTrace:     0x%02X\n", Setup->mem.dfxMemSetup.DfxUbiosSmbusTraceEnable);
  RcDebugPrint (SDBG_DEFAULT, "    DfxSmbusI3cMode:     0x%02X\n", Setup->mem.dfxMemSetup.DfxSmbusI3cModeEnable);
  RcDebugPrint (SDBG_DEFAULT, "    DfxPeriodicZqCal: 0x%02X\n", Setup->mem.dfxMemSetup.DfxPeriodicZqCal);
  RcDebugPrint (SDBG_DEFAULT, "  };\n");
  RcDebugPrint (SDBG_DEFAULT, "  serialDebugMsgLvl:   %02X\n", Setup->common.serialDebugMsgLvl);
  PrintMemSetupOptionsChip (Host, socket);

  RcDebugPrint (SDBG_DEFAULT, "}; //struct sysHost.setup\n\n");
  ReleasePrintControl ();
}
#endif // DEBUG_CODE_BLOCK

/**
  Function forces slow boot on PPR Error
  If PPR data has already been consumed, then does nothing

  @param[in] Setup - Setup Policy structure

  @retval N/A
**/
VOID
ForceSlowBootOnPprError (
  SYS_SETUP           *Setup
  )
{
  UINT8  i;
  UINT8  PprSocketBitMap = 0;
  UINT8  CmosByteVal = 0;

  for (i = 0; i < MAX_PPR_ADDR_ENTRIES; i++) {
    if (Setup->mem.pprAddrSetup[i].pprAddrStatus == PPR_STS_ADDR_VALID) {
      PprSocketBitMap |= (1 << Setup->mem.pprAddrSetup[i].socket);
    }
  }
  CmosByteVal = GetPprCmosStatus ();
  if (PprSocketBitMap != 0) { //Atleast one valid error in PPR Structure
    if ((PprSocketBitMap & CmosByteVal) != PprSocketBitMap) { //Cmos bit map and PprSocketBitMap are not same, so we disable fast boot
      RcDebugPrint (SDBG_ERROR, "PPR Error on previous boot\nDisabling Fast boot");
      SetPreviousBootError (ERROR_ON_PREVIOUS_BOOT);
    }
  }
}

/**

  Memory Controller initialization entry point

  @param Host          - Pointer to the system Host (root) structure

  @retval EFI_SUCCESS  - Memory Initialization Complete
  @retval Other        - Did not complete memory initialization

**/
EFI_STATUS
EFIAPI
MemStart (
  PSYSHOST Host
  )
{
  UINT32              status;
  struct memSetup     tempSetupData;
  UINT8               socket;
  UINT8               maxNode;
  UINT8               sbspSktId = GetSysSbspSocketIdNv ();
  CallTableEntryType  *CallTableMain;
  UINT8               CallTableMainSize;
  UINT8               AdrResumeBoot = 0;
  UINT8               FrbEventLastBoot;
  BootMode            SysBootMode;
  SYS_SETUP           *Setup;

  Setup = GetSysSetupPointer ();

  PUSH_EXIT_FRAME (Host);


  socket = GetSysSocketId ();

  PassMemBarAndMask (Host, socket);


  SetSetupOptionsChip (Host);

#ifdef DEBUG_CODE_BLOCK
  if (GetDebugLevel () & SDBG_MAX) {
    PrintMemSetupOptions (Host, socket);
  }
#endif // DEBUG_CODE_BLOCK

  //
  // Save input structure into a temporary buffer
  //
  CopyMem ( &tempSetupData, &(Setup->mem), sizeof (Setup->mem));
  //
  // Set firstPass variable for InitStructures().
  //
  Host->var.mem.firstPass = 1;

  if (Setup->mem.options & MULTI_THREAD_MRC_EN) {
    maxNode = socket + 1;
  } else {
    maxNode = MAX_SOCKET;
    socket = 0;
  }


  if (sbspSktId == socket) {
    //Only need to be processed for SBSP
    ForceSlowBootOnPprError (Setup);
  }

  for (; socket < maxNode; socket++) {
    //
    // Continue if this socket is not present
    //
    if (!SocketPresent (socket)) {
      continue;
    }

    //
    // Initialize DDR and platform control structures
    //
    InitStructures (Host, socket);
  } // socket loop


  RcDebugPrint (SDBG_MAX, "\nMAX_SOCKET = %d, sizeof sysHost = %d\n", MAX_SOCKET, sizeof (SYSHOST));
  RcDebugPrint (SDBG_MAX, "sizeof sysNvram = %d\n\n", sizeof (struct sysNvram));

  // Initialize the boot mode after InitStructures, as it can be changed in that function.
  SysBootMode = GetSysBootMode ();

  if (SysBootMode == NormalBoot) {
    RcDebugPrint (SDBG_DEFAULT, "bootMode = NormalBoot\n");
    if (Host->var.mem.subBootMode == WarmBootFast) {
      RcDebugPrint (SDBG_DEFAULT, "subBootMode = WarmBootFast\n");
    } else if (Host->var.mem.subBootMode == ColdBoot) {
      RcDebugPrint (SDBG_DEFAULT, "subBootMode = ColdBoot\n");
    } else if (Host->var.mem.subBootMode == ColdBootFast) {
      RcDebugPrint (SDBG_DEFAULT, "subBootMode = ColdBootFast\n");
    } else {
      RcDebugPrint (SDBG_DEFAULT, "subBootMode = unknown.\n");
      FatalError (ERR_MRC_STRUCT, ERR_INVALID_SUB_BOOT_MODE);
    }
  } else if (SysBootMode == S3Resume) {
    RcDebugPrint (SDBG_DEFAULT, "bootMode = S3Resume\n");
    if (Host->var.mem.subBootMode == AdrResume) {
      RcDebugPrint (SDBG_DEFAULT, "subBootMode = AdrResume.\n");
    }
  } else if (Host->var.mem.subBootMode == NvDimmResume) {
    RcDebugPrint (SDBG_DEFAULT, "subBootMode = NvDimmResume\n");
  } else {
    RcDebugPrint (SDBG_DEFAULT, "bootMode is unknown.\n");
    FatalError (ERR_MRC_STRUCT, ERR_INVALID_BOOT_MODE);
  }

  if (PcdGetBool (PcdNvDimmEn)) {
    if (Host->var.mem.subBootMode == AdrResume) {
      AdrResumeBoot = 1;
      // Host->var.mem.subBootMode = 0;
    } else {
      AdrResumeBoot = 0;
    }
  }
  //
  // Call into memory reference code
  //
  //
  // Force serial during until certain KTI routes are complete if > 2S
  //
  if (!(GetSysResetRequired () & POST_RESET_POWERGOOD)) {


    GetCallTablePtrs (&CallTableMain, &CallTableMainSize);
    status = MemMain (Host, CallTableMain, CallTableMainSize, GetSysSocketId ());

  } else {
    status = FAILURE;
  }

  if (PcdGetBool (PcdNvDimmEn)) {
    // Restore the subboot mode
    if (AdrResumeBoot) {
      Host->var.mem.subBootMode = AdrResume;
    }
  }
  //
  // Only execute this for the SBSP
  //
  if (AcquireCommandPipe (Host, GetSysSocketId ())) {
    //
    // check for FRB errors
    //
    if (CheckFrb (Host, sbspSktId, 0, &FrbEventLastBoot)) {
      // Error returned
      RcDebugPrint (SDBG_DEFAULT, "Error returned from CheckFrb, this is not fatal.\n  Attempted to set the bit to 0\n");
    }

    //
    // on cold boot, save MRC input structure into NVRAM
    //
    if (SysBootMode == NormalBoot &&
        (Host->var.mem.subBootMode == ColdBoot ||
         Host->var.mem.subBootMode == NvDimmResume)) {
      CopyMem (&(Host->nvram.mem.savedSetupData), &tempSetupData,  sizeof (tempSetupData));
    }

    //
    // Output data on normal boot path
    //
    if (SysBootMode == NormalBoot) {
      //
      // Call OEM hook to write NVRAM data
      // OemWriteNvram(Host);
      //
#ifdef DEBUG_CODE_BLOCK
#if 0
      RcDebugPrint (SDBG_DEFAULT, "Normal boot path Host->nvram contents:");
      for (i = 0; i < sizeof (struct sysNvram); i += 4) {
        if (i % 16 == 0) {
          //        rcPrintf (Host, "\nord1 0%Xp = ", (UINT8 *) &Host->nvram + i);
          RcDebugPrint (SDBG_DEFAULT, "\n");
        }

        RcDebugPrint (SDBG_DEFAULT, "0x%08X", *((UINT32 *) &Host->nvram + i) );
        if (i % 16 < 15) {
          RcDebugPrint (SDBG_DEFAULT, ", ");
        }
      }

      RcDebugPrint (SDBG_DEFAULT, "\n");
#endif // 0
#endif // DEBUG_CODE_BLOCK
    }

#ifdef DEBUG_CODE_BLOCK
    if (Setup->common.options & HALT_ON_ERROR_AUTO) {
      FatalErrSetMaxSDBGSelect (Host, MAX_SDBG_FATAL_ERR_CLEAR);
    }
#endif //DEBUG_CODE_BLOCK

    //
    // Indicate that MRC is complete
    //
    RcDebugPrint (SDBG_DEFAULT, "STOP_MRC_RUN\n");


    if (!GetSysResetRequired ()) {
      OutputCheckpoint (CHECKPOINT_MAJOR_MRC_DONE, 0, 0);
      SetMemDone (Host);
    }
    //
    // Hard-coded memory config for Virtual Platform single socket
    //
  }
  //
  // if SBSP
  //

  //
  // Return MRC status
  //
  POP_EXIT_FRAME (Host, status);
  return status;
}

VOID
HandleAdrEvent (
  IN PSYSHOST Host
  )
{

#ifdef IA32

  SYS_SETUP  *Setup;
  MEMORY_POLICY_PPI *MemSetupPpi;
  MRC_PLATFORM_HOOKS_INTERFACE *MrcPlatformHooksInterface = NULL;
  EFI_STATUS Status;
  MRC_STATUS HookStatus;
  UINT32 AdrEvent = 0;

  //
  // First try the OEM hook to check for ADR.
  //

  HookStatus = MRC_FUNCTION_NOT_IMPLEMENTED;
  Status = LocateInterface (
                &gMrcPlatformHooksInterfaceGuid,
                0,
                (VOID **)&MrcPlatformHooksInterface
                );
  if (!EFI_ERROR (Status)) {
    HookStatus = MrcPlatformHooksInterface->PlatformDetectADR (&AdrEvent);

    if (HookStatus != MRC_FUNCTION_NOT_IMPLEMENTED) {
      if (AdrEvent != 0) {
        RcDebugPrint (SDBG_MAX,
                        "ADR recovery triggered via PlatformDetectADR\n");
        SetSysBootMode (S3Resume);
        Host->var.mem.subBootMode = AdrResume;

        return;
      }
    }

  }

  //
  // Hook not present, check to see if the policy has an ADR event.
  //

  Status = PeiServicesLocatePpi (
              &gMemoryPolicyPpiGuid,
              0,
              NULL,
              (VOID **) &MemSetupPpi
              );
  if (EFI_ERROR (Status)) {
    ASSERT (FALSE);
    return;
  }

  Setup = MemSetupPpi->SysSetup;

  if (Setup->AdrEvent != 0) {
    RcDebugPrint (SDBG_MAX,
                    "ADR recovery triggered via policy\n");
    SetSysBootMode (S3Resume);
    Host->var.mem.subBootMode = AdrResume;
  }

#endif // #ifdef IA32

  return;

} // HandleAdrEvent

/**
  Indicate whether handling for an ADR event is needed.

  @param[in]  Setup   A pointer to the setup structure

  @retval FALSE   No ADR event needs to be handled.
  @retval TRUE    An ADR event needs to be handled.
**/
BOOLEAN
IsAdrHandlingNeeded (
  IN  SYS_SETUP *Setup
  )
{
  if (Setup->mem.ADREn && Setup->mem.check_pm_sts) {
    return PchAdrGetAdrResetStatus ();
  }
  return FALSE;
}

/**

  Initialize internal data structures

  @param Host  - Pointer to the system Host (root) structure
  @param socket  - processor socket ID

  @retval 0 - successful
  @retval Other - failure

**/
UINT32
InitStructures (
  PSYSHOST Host,
  UINT8    socket
  )
{
  struct channelNvram  (*channelNvList)[MAX_CH];
  struct imcNvram  (*imcNvList)[MAX_IMC];
  UINT8                     ch;
  UINT8                     dimm;
  UINT8                     rank;
  UINT8                     setupChanged;
  UINT8                     SocketId;
  UINT16                    Crc16;
  EFI_STATUS                Status;
  BOOLEAN                   forceColdBoot;
  INT32                     i;
  UINT8                     FrbEventLastBoot;
  UINT8                     WarmBoot;
  UINT8                     SbspSocketIdNv;
  SYS_SETUP                 *Setup;
  DDR_TECHNOLOGY_TYPE       DdrType;
  UINT8                     MaxChDdr;
  UINT8                     NumChPerMc;
  CPUID_REGISTER_INFO       *CpuIdPtr;

  MaxChDdr   = GetMaxChDdr ();
  NumChPerMc = GetNumChannelPerMc ();
  Setup      = GetSysSetupPointer ();

  InitDdrFreqChip (Host, socket);

  channelNvList = GetChannelNvList (Host, socket);
  imcNvList = GetImcNvList (Host, socket);

  setupChanged = 0;
  SbspSocketIdNv = GetSysSbspSocketIdNv ();
  //
  // Initialize to ColdBoot at first.  Can change to a faster boot method later
  // if certain conditions are met and the caller requests a faster boot mode attempt.
  //
  Host->var.mem.subBootMode = ColdBoot;
  Host->var.mem.socket[socket].socketSubBootMode = ColdBoot;
  WarmBoot = 0;

  //
  // MRC only needs to worry about 2 main boot modes - NormalBoot and S3Resume.
  // In case the caller passes in some other value, make sure it gets reset
  // to NormalBoot to take the main flow through MRC.
  //
  if (GetSysBootMode () != S3Resume) {
    SetSysBootMode (NormalBoot);
  }

  for (i = 0; i < MaxChDdr; i++) {
    Host->var.mem.socket[socket].channelList[i].mcId = (UINT8) (i / NumChPerMc);
  }

  // Initialize Socket Die count
  Host->var.mem.socket[socket].SocketDieCount = PcuGetDieCount (socket);

  //
  // Initialize processor common information of non-SBSP for single-thread mode.
  //
  if ((Setup->mem.options & MULTI_THREAD_MRC_EN) == 0) {
    if (socket != SbspSocketIdNv) {
      LoadCapId (socket);
      CpuIdPtr = &Host->var.common.procCom[socket].ExtCpuid7;
      ExecuteCpuidPipe (socket, CPUID_STRUCTURED_EXTENDED_FEATURE_FLAGS, 0, &CpuIdPtr->Eax, &CpuIdPtr->Ebx, &CpuIdPtr->Ecx, &CpuIdPtr->Edx);
    }
  }

#if defined(ME_SUPPORT_FLAG) && ME_SUPPORT_FLAG
  //
  // For S3 resume patch meRequestedSize is not read from ME
  // Saved value has to be used instead.
  //
  if (GetSysBootMode () == S3Resume) {
    Setup->mem.meRequestedSize = Host->nvram.mem.savedSetupData.meRequestedSize;
  }
#endif //ME_SUPPORT_FLAG

#if defined(IE_SUPPORT) && IE_SUPPORT
  //
  // For S3 resume patch meRequestedSize is not read from ME
  // Saved value has to be used instead.
  //
  if (GetSysBootMode () == S3Resume) {
    Setup->mem.ieRequestedSize = Host->nvram.mem.savedSetupData.ieRequestedSize;
  }
#endif //IE_SUPPORT

  //
  // Check if the MRC version has changed
  //
  if (CompareMem (&Host->nvram.common.RcVersion, &Host->var.common.RcVersion, sizeof (RC_VERSION)) != 0) {
    setupChanged = 1;
  }

  //
  // Check if the MRC input has changed since the previously saved cold boot data
  //
  if (
       (Host->nvram.mem.savedSetupData.options & ~FASTBOOT_MASK) != (Setup->mem.options & ~FASTBOOT_MASK) ||
       Host->nvram.mem.savedSetupData.optionsExt != Setup->mem.optionsExt ||
       Host->nvram.mem.savedSetupData.enforcePOR != Setup->mem.enforcePOR ||
       Host->nvram.mem.savedSetupData.PanicWm != Setup->mem.PanicWm ||
       Host->nvram.mem.savedSetupData.ddrFreqLimit != Setup->mem.ddrFreqLimit ||
       Host->nvram.mem.savedSetupData.chInter != Setup->mem.chInter ||
       Host->nvram.mem.savedSetupData.dimmTypeSupport != Setup->mem.dimmTypeSupport ||
       Host->nvram.mem.savedSetupData.ckeThrottling != Setup->mem.ckeThrottling ||
       Host->nvram.mem.savedSetupData.olttPeakBWLIMITPercent != Setup->mem.olttPeakBWLIMITPercent ||
       Host->nvram.mem.savedSetupData.thermalThrottlingOptions != Setup->mem.thermalThrottlingOptions ||
       Host->nvram.mem.savedSetupData.DramRaplEnable != Setup->mem.DramRaplEnable ||
       Host->nvram.mem.savedSetupData.dramraplbwlimittf != Setup->mem.dramraplbwlimittf ||
       Host->nvram.mem.savedSetupData.enableBiosSsaLoader != Setup->mem.enableBiosSsaLoader ||
       Host->nvram.mem.savedSetupData.RecEnDelayAverage != Setup->mem.RecEnDelayAverage ||

       Host->nvram.mem.savedSetupData.rmtPatternLength != Setup->mem.rmtPatternLength ||
       Host->nvram.mem.savedSetupData.rmtPatternLengthExt != Setup->mem.rmtPatternLengthExt ||

       Host->nvram.mem.savedSetupData.patrolScrubDuration != Setup->mem.patrolScrubDuration ||
       Host->nvram.mem.savedSetupData.memTestLoops != Setup->mem.memTestLoops ||
       Host->nvram.mem.savedSetupData.trainingResultOffsetFunctionEnable != Setup->mem.trainingResultOffsetFunctionEnable ||
       Host->nvram.mem.savedSetupData.scrambleSeedLow != Setup->mem.scrambleSeedLow ||
       Host->nvram.mem.savedSetupData.scrambleSeedHigh != Setup->mem.scrambleSeedHigh ||
       Host->nvram.mem.savedSetupData.ADREn != Setup->mem.ADREn ||
       Host->nvram.mem.savedSetupData.LegacyADRModeEn != Setup->mem.LegacyADRModeEn ||
       Host->nvram.mem.savedSetupData.MinNormalMemSize != Setup->mem.MinNormalMemSize ||
       Host->nvram.mem.savedSetupData.check_pm_sts != Setup->mem.check_pm_sts ||
       Host->nvram.mem.savedSetupData.check_platform_detect != Setup->mem.check_platform_detect ||
       Host->nvram.mem.savedSetupData.dfxMemSetup.DfxBankGroupInterleavingMode != Setup->mem.dfxMemSetup.DfxBankGroupInterleavingMode ||
       Host->nvram.mem.savedSetupData.dfxMemSetup.dfxHighAddrBitStart != Setup->mem.dfxMemSetup.dfxHighAddrBitStart ||
       Host->nvram.mem.savedSetupData.dfxMemSetup.dfxStraddleHighAddressBit != Setup->mem.dfxMemSetup.dfxStraddleHighAddressBit ||
       Host->nvram.mem.savedSetupData.dfxMemSetup.dfxLowMemChannel != Setup->mem.dfxMemSetup.dfxLowMemChannel ||
       Host->nvram.mem.savedSetupData.dfxMemSetup.DfxDdrtRt != Setup->mem.dfxMemSetup.DfxDdrtRt ||
       Host->nvram.mem.savedSetupData.dfxMemSetup.DfxInputRt != Setup->mem.dfxMemSetup.DfxInputRt ||
       Host->nvram.mem.savedSetupData.dfxMemSetup.DfxInputIoLatency != Setup->mem.dfxMemSetup.DfxInputIoLatency ||
       Host->nvram.mem.savedSetupData.dfxMemSetup.DfxInputTxDqDelay != Setup->mem.dfxMemSetup.DfxInputTxDqDelay ||
       Host->nvram.mem.savedSetupData.dfxMemSetup.DfxInputTxDqsDelay != Setup->mem.dfxMemSetup.DfxInputTxDqsDelay ||
       Host->nvram.mem.savedSetupData.dfxMemSetup.DfxInputTxDqBitDelay != Setup->mem.dfxMemSetup.DfxInputTxDqBitDelay ||
       Host->nvram.mem.savedSetupData.dfxMemSetup.DfxInputRxDqDelay != Setup->mem.dfxMemSetup.DfxInputRxDqDelay ||
       Host->nvram.mem.savedSetupData.dfxMemSetup.DfxInputRxDqsDelay != Setup->mem.dfxMemSetup.DfxInputRxDqsDelay ||
       Host->nvram.mem.savedSetupData.dfxMemSetup.DfxInputRxDqBitDelay != Setup->mem.dfxMemSetup.DfxInputRxDqBitDelay ||
       Host->nvram.mem.savedSetupData.dfxMemSetup.DfxRcvEnDelay != Setup->mem.dfxMemSetup.DfxRcvEnDelay ||
       Host->nvram.mem.savedSetupData.dfxMemSetup.DfxInputGnt2Erid != Setup->mem.dfxMemSetup.DfxInputGnt2Erid ||
       Host->nvram.mem.savedSetupData.dfxMemSetup.DfxInputIoLatencyIoComp != Setup->mem.dfxMemSetup.DfxInputIoLatencyIoComp ||
       Host->nvram.mem.savedSetupData.dfxMemSetup.DfxCwlAdjPos != Setup->mem.dfxMemSetup.DfxCwlAdjPos ||
       Host->nvram.mem.savedSetupData.dfxMemSetup.DfxCwlAdjNeg != Setup->mem.dfxMemSetup.DfxCwlAdjNeg ||
       Host->nvram.mem.savedSetupData.ADRDataSaveMode != Setup->mem.ADRDataSaveMode ||
       Host->nvram.mem.savedSetupData.interNVDIMMS != Setup->mem.interNVDIMMS ||
       Host->nvram.mem.savedSetupData.restoreNVDIMMS != Setup->mem.restoreNVDIMMS ||
       Host->nvram.mem.savedSetupData.eraseArmNVDIMMS != Setup->mem.eraseArmNVDIMMS ||
       Host->nvram.mem.savedSetupData.dfxMemSetup.DfxDdrtSchedulerDebug != Setup->mem.dfxMemSetup.DfxDdrtSchedulerDebug ||
       Host->nvram.mem.savedSetupData.dfxMemSetup.DfxForceOdtOn != Setup->mem.dfxMemSetup.DfxForceOdtOn ||

       Host->nvram.mem.savedSetupData.MemBootHealthConfig.MemBootHealthCheck != Setup->mem.MemBootHealthConfig.MemBootHealthCheck ||
       Host->nvram.mem.savedSetupData.MemBootHealthConfig.ResetOnCriticalError != Setup->mem.MemBootHealthConfig.ResetOnCriticalError ||
       Host->nvram.mem.savedSetupData.MemBootHealthConfig.WarningTxDqDelayLeftEdge != Setup->mem.MemBootHealthConfig.WarningTxDqDelayLeftEdge ||
       Host->nvram.mem.savedSetupData.MemBootHealthConfig.WarningTxDqDelayRightEdge != Setup->mem.MemBootHealthConfig.WarningTxDqDelayRightEdge ||
       Host->nvram.mem.savedSetupData.MemBootHealthConfig.WarningTxVrefLeftEdge != Setup->mem.MemBootHealthConfig.WarningTxVrefLeftEdge ||
       Host->nvram.mem.savedSetupData.MemBootHealthConfig.WarningTxVrefRightEdge != Setup->mem.MemBootHealthConfig.WarningTxVrefRightEdge ||
       Host->nvram.mem.savedSetupData.MemBootHealthConfig.WarningRxDqsDelayLeftEdge != Setup->mem.MemBootHealthConfig.WarningRxDqsDelayLeftEdge ||
       Host->nvram.mem.savedSetupData.MemBootHealthConfig.WarningRxDqsDelayRightEdge != Setup->mem.MemBootHealthConfig.WarningRxDqsDelayRightEdge ||
       Host->nvram.mem.savedSetupData.MemBootHealthConfig.WarningRxVrefLeftEdge != Setup->mem.MemBootHealthConfig.WarningRxVrefLeftEdge ||
       Host->nvram.mem.savedSetupData.MemBootHealthConfig.WarningRxVrefRightEdge != Setup->mem.MemBootHealthConfig.WarningRxVrefRightEdge ||
       Host->nvram.mem.savedSetupData.MemBootHealthConfig.CriticalTxDqDelayLeftEdge != Setup->mem.MemBootHealthConfig.CriticalTxDqDelayLeftEdge ||
       Host->nvram.mem.savedSetupData.MemBootHealthConfig.CriticalTxDqDelayRightEdge != Setup->mem.MemBootHealthConfig.CriticalTxDqDelayRightEdge ||
       Host->nvram.mem.savedSetupData.MemBootHealthConfig.CriticalTxVrefLeftEdge != Setup->mem.MemBootHealthConfig.CriticalTxVrefLeftEdge ||
       Host->nvram.mem.savedSetupData.MemBootHealthConfig.CriticalTxVrefRightEdge != Setup->mem.MemBootHealthConfig.CriticalTxVrefRightEdge ||
       Host->nvram.mem.savedSetupData.MemBootHealthConfig.CriticalRxDqsDelayLeftEdge != Setup->mem.MemBootHealthConfig.CriticalRxDqsDelayLeftEdge ||
       Host->nvram.mem.savedSetupData.MemBootHealthConfig.CriticalRxDqsDelayRightEdge != Setup->mem.MemBootHealthConfig.CriticalRxDqsDelayRightEdge ||
       Host->nvram.mem.savedSetupData.MemBootHealthConfig.CriticalRxVrefLeftEdge != Setup->mem.MemBootHealthConfig.CriticalRxVrefLeftEdge ||
       Host->nvram.mem.savedSetupData.MemBootHealthConfig.CriticalRxVrefRightEdge != Setup->mem.MemBootHealthConfig.CriticalRxVrefRightEdge ||
       SavedSetupDataChangedChip (Host)

       //
       // check ddrIMCsetup after this
       //

       ) {
    //
    // Indicate a change occured to the input parameters
    //
    setupChanged = 1;

  }

  //
  // Check XMP if enabled
  //

  if (PcdGetBool (PcdMrcXmpSupport)) {

    if (Host->nvram.mem.savedSetupData.inputMemTime.nCL           != Setup->mem.inputMemTime.nCL ||
        Host->nvram.mem.savedSetupData.inputMemTime.nRP           != Setup->mem.inputMemTime.nRP ||
        Host->nvram.mem.savedSetupData.inputMemTime.nRCD          != Setup->mem.inputMemTime.nRCD ||
        Host->nvram.mem.savedSetupData.inputMemTime.nRRD          != Setup->mem.inputMemTime.nRRD ||
        Host->nvram.mem.savedSetupData.inputMemTime.nRRD_L        != Setup->mem.inputMemTime.nRRD_L ||
        Host->nvram.mem.savedSetupData.inputMemTime.nWTR          != Setup->mem.inputMemTime.nWTR ||
        Host->nvram.mem.savedSetupData.inputMemTime.nRAS          != Setup->mem.inputMemTime.nRAS ||
        Host->nvram.mem.savedSetupData.inputMemTime.nRTP          != Setup->mem.inputMemTime.nRTP ||
        Host->nvram.mem.savedSetupData.inputMemTime.nWR           != Setup->mem.inputMemTime.nWR ||
        Host->nvram.mem.savedSetupData.inputMemTime.nFAW          != Setup->mem.inputMemTime.nFAW ||
        Host->nvram.mem.savedSetupData.inputMemTime.nCWL          != Setup->mem.inputMemTime.nCWL ||
        Host->nvram.mem.savedSetupData.inputMemTime.nRC           != Setup->mem.inputMemTime.nRC ||
        Host->nvram.mem.savedSetupData.inputMemTime.nCMDRate      != Setup->mem.inputMemTime.nCMDRate ||
        Host->nvram.mem.savedSetupData.inputMemTime.nRFC          != Setup->mem.inputMemTime.nRFC ||
        Host->nvram.mem.savedSetupData.inputMemTime.ddrFreqLimit  != Setup->mem.inputMemTime.ddrFreqLimit ||
        Host->nvram.mem.savedSetupData.inputMemTime.vdd           != Setup->mem.inputMemTime.vdd ||
        Host->nvram.mem.savedSetupData.inputMemTime.ucVolt        != Setup->mem.inputMemTime.ucVolt ||
        Host->nvram.mem.savedSetupData.inputMemTime.casSup        != Setup->mem.inputMemTime.casSup ||
        Host->nvram.mem.savedSetupData.inputMemTime.tREFI         != Setup->mem.inputMemTime.tREFI ||
        Host->nvram.mem.savedSetupData.inputMemTime.ddrFreq       != Setup->mem.inputMemTime.ddrFreq) {

      setupChanged = 1;

    }


  }
  // APTIOV_SERVER_OVERRIDE_RC_START : Ignore TEST flags in options when S3 resumed.
  if (((Host->nvram.mem.savedSetupData.options & ~(MEMORY_TEST_EN|MEMORY_TEST_COLD_FAST_BOOT_EN)) == (Setup->mem.options & ~(MEMORY_TEST_EN|MEMORY_TEST_COLD_FAST_BOOT_EN))) &&
          GetSysBootMode() == S3Resume) 
    setupChanged = 0;
  // APTIOV_SERVER_OVERRIDE_RC_END : Ignore TEST flags in options when S3 resumed.

  //
  // Check if IMC enabled changed
  //
  if (DetectIMCEnabledChangeChip (Host, socket)) {
    setupChanged = 1;
  }

  //
  // Check if the DIMM map out input has changed
  //
  if (Host->nvram.mem.savedSetupData.socket[socket].enabled != Setup->mem.socket[socket].enabled ||
      SavedSetupDataChangedIMCChip (Host, socket)) {
    setupChanged = 1;
  }

  for (ch = 0; ch < MaxChDdr; ch++) {
    if (Host->nvram.mem.savedSetupData.socket[socket].ddrCh[ch].enabled != Setup->mem.socket[socket].ddrCh[ch].enabled) {
      setupChanged = 1;
      break;
    }

    for (dimm = 0; dimm < MAX_DIMM; dimm++) {
      for (rank = 0; rank < MAX_RANK_DIMM; rank++) {
        if (Host->nvram.mem.savedSetupData.socket[socket].ddrCh[ch].dimmList[dimm].mapOut[rank] !=
            Setup->mem.socket[socket].ddrCh[ch].dimmList[dimm].mapOut[rank]) {
          //
          // Indicate a change occured to the input parameters
          //
          setupChanged = 1;
          break;
        }
      } // rank loop
    } // dimm loop
  } // ch loop

  //
  // Initialize to do not force cold boot
  //
  forceColdBoot = FALSE;

  //
  // Check memory boot health check status. It was stored in the CMOS
  // If there was a boot health check error, disable fast boot.
  //

  if (GetMemBootHealthCheckErrorFlag ()) {
    RcDebugPrint (SDBG_ERROR, "Detect MemBootHealthCheckErrorFlag and will force slow cold boot.\n");
    setupChanged = 1;
  }

  //
  // Check if there was a failure on the previous boot and disable fast boot if yes
  //
  if (Host->var.mem.previousBootError) {
    RcDebugPrint (SDBG_ERROR, "Previous error(s); forcing Cold Boot\n");
    setupChanged = 1;
  }

  //
  // check for FRB errors
  //
  if (CheckFrb (Host, socket, 0, &FrbEventLastBoot)) {
    // Error returned
    RcDebugPrint (SDBG_DEFAULT, "Error returned from CheckFrb, this is not fatal.\n");
  }

  if (FrbEventLastBoot) {
    Host->var.mem.subBootMode = ColdBoot;
    forceColdBoot = TRUE;
    RcDebugPrint (SDBG_DEFAULT,"BIST error found disable fast boot\n");
  }//endif FRB error

  if (Setup->mem.ADREn) {

    if (IsAdrHandlingNeeded (Setup)) {
      RcDebugPrint (SDBG_MAX, "ADR recovery triggered\n");
      SetSysBootMode (S3Resume);
      Host->var.mem.subBootMode = AdrResume;
    }

    if (Setup->mem.check_platform_detect) {
      HandleAdrEvent (Host);
    }

    if (Host->var.mem.subBootMode == AdrResume) {
      Host->var.mem.skipMemoryInit = 0;
    } else {
      RcDebugPrint (SDBG_MAX,
                     "No ADR event detected\n");
      if (socket == SbspSocketIdNv) {

        EFI_STATUS PpiStatus;
        MRC_PLATFORM_HOOKS_INTERFACE *MrcPlatformHooksInterface = NULL;

        RcDebugPrint (SDBG_MAX,
                        "Release ADR clamps\n");

        PpiStatus = LocateInterface (
                      &gMrcPlatformHooksInterfaceGuid,
                      0,
                      (VOID **)&MrcPlatformHooksInterface
                      );
        if (!EFI_ERROR (PpiStatus)) {
          MrcPlatformHooksInterface->PlatformReleaseADRClamps (socket);
        }

      }
    }
    if (PcdGetBool (PcdNvDimmEn)) {
      if (Setup->mem.ADRDataSaveMode == ADR_NVDIMM) {
        // Make sure ADR Complete isn't high
      if (socket == GetSysSbspSocketIdNv()) {
        RcDebugPrint (SDBG_MAX,
                       "Clear ADR status\n");
        PchAdrClearAdrResetStatus ();
      }
      }
    }
  } // ADREn

  if (GetSysBootMode () == NormalBoot) {
    //
    // On normal boot paths (i.e. not S3), do meminit by default.
    //
    Host->var.mem.skipMemoryInit = 0;
    //
    // Check BIT1 for SSPAD0 on each socket.  If this bit is set, it means we are on a warm
    // boot cycle.
    //
    WarmBoot = GetScratchpadWarmBoot (Host, socket);
    //
    // If this CPU does not have this bit set, it means we are not on a warm boot path.
    //
    if (WarmBoot == 0) {
      forceColdBoot = TRUE;
    }

    //
    // Store information about warm boot request, because it is not possible to
    // detect warm boot via subBootMode in case when fast boot is disabled
    // and MRC decides to take the cold boot path
    //
    if (WarmBoot == 0) {
      Host->var.mem.WarmBootRequested = FALSE;
    } else {
      Host->var.mem.WarmBootRequested = TRUE;
    }

    // forced coldboot on a warmreset if FCB is enabled and FWB is not.
    if ((WarmBoot) && (Setup->mem.options & ATTEMPT_FAST_BOOT_COLD) && !(Setup->mem.options & ATTEMPT_FAST_BOOT)) {
      forceColdBoot = TRUE;
    }

    //
    // Clear out this register so that if MRC fails, the next reboot
    // will take the cold boot path with full training.
    //
    ClearScratchpadWarmBoot (Host, socket);

    //
    // Switch to warm boot if all CPUs have the warm reset bit set.
    //
    if (forceColdBoot) {
      Host->var.mem.subBootMode = ColdBoot;
      if (!(Setup->mem.options & ATTEMPT_FAST_BOOT_COLD)) {
        setupChanged = 1;
        RcDebugPrint (SDBG_MINMAX,
                                "forceColdBoot bit set\n");
      }
    } else {
      Host->var.mem.subBootMode = WarmBoot;
    }
  }

  //
  // At this point, we have determined whether or not we are on a NormalBoot path or an
  // S3Resume path.  In addition, we have determined whether we're on a WarmBoot path
  // or ColdBoot path as a subset (sub-boot type) of the NormalBoot path through MRC.
  //
  // Now we need to do some further checks to see if these boot types need to be
  // modified for speed.
  //
  if ((Setup->mem.options & ATTEMPT_FAST_BOOT) || (Setup->mem.options & ATTEMPT_FAST_BOOT_COLD) || (GetSysBootMode () == S3Resume)) {
    //
    // Check the CRC of the memory portion of the NVRAM structure.
    // If it doesn't pass, then something is wrong and we can not
    // attempt a fast boot or S3 boot since they require the NVRAM structure
    // to be healthy.
    //
    Crc16 = Host->nvram.mem.Crc16;
    Host->nvram.mem.Crc16 = 0;
    Status = CalculateCrc16 (
               (VOID *) &Host->nvram.mem,
               sizeof (Host->nvram.mem),
               &Host->nvram.mem.Crc16
               );

    //
    // If NVRAM structure failed its CRC check...
    //
    if (EFI_ERROR (Status) || Crc16 != Host->nvram.mem.Crc16) {
      RcDebugPrint (SDBG_MAX,
                     "NVRAM CRC check failed. Disable fast boot\n");
      setupChanged = 1;
    }

  }

  //
  // Check if DIMM enabled changed
  // InitializeSpd will update nvram structure. If setupChanged == 0,
  // Nvram CRC needs to be updated here accordingly in case CRC check mismatch
  // in next socket loop. Otherwise, Nvram will be clear later
  DdrType = Ddr4Type;
#ifdef DDR5_SUPPORT
  DdrType = Ddr5Type;
#endif
  InitializeSpd (socket, DdrType);
  if ((GetSysBootMode () == NormalBoot) &&
      (Setup->mem.options & ATTEMPT_FAST_BOOT_COLD) &&
      (Host->var.mem.subBootMode != WarmBoot) &&
      (DetectDIMMEnabledChange (Host, socket))) {

    setupChanged = 1;
    RcDebugPrintWithDevice (SDBG_MINMAX, socket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                   "Dimm changed.\n");
  }

  //
  // Detect NVMVIMM is disabled but been recovered by FW update or Format under OS, then need to re-train
  //
  if (setupChanged == 0) {
    if (!(GetEmulation () & SIMICS_FLAG)) {
      if ((GetSysBootMode () == NormalBoot) && (DetectNvmdimmDisableChange (Host, socket))) {
        setupChanged = 1;
      }
    }
  }

  //
  // Check for processor change for fast boot
  //
  if ((GetSysBootMode () == NormalBoot) && (((Host->var.mem.subBootMode == ColdBoot) && (Setup->mem.options & ATTEMPT_FAST_BOOT_COLD)) ||
                                      ((Host->var.mem.subBootMode == WarmBoot) && (Setup->mem.options & ATTEMPT_FAST_BOOT)))) {
    if (socket == SbspSocketIdNv){  // Only have SBSP check processor configuration
      if (DetectProcessorRemovedChip (Host)) {
        setupChanged = 1;
        RcDebugPrint (SDBG_DEFAULT,"Processors changed.\n");
      }
    }
  }

  //
  // Check processors PPIN for cold fast boot
  //
  if (setupChanged) {
    Host->var.mem.socket[socket].procPpin = GetPpinChip (Host, socket);
  } else {
    if ((GetSysBootMode () == NormalBoot) && (Host->var.mem.subBootMode == ColdBoot) && (Setup->mem.options & ATTEMPT_FAST_BOOT_COLD)) {
      if ((Setup->mem.options & MULTI_THREAD_MRC_EN)) {
          RcDebugPrint (SDBG_DEFAULT, "Cold Fast Boot Check\n");
        if(ProcPpinChangedCheck(Host, socket)){  //Per-socket check
          setupChanged = 1;
        }
      } else {
        if (socket == SbspSocketIdNv){  // Only have SPSP check PPIN numbers when not MTMRC
          for (SocketId = 0; SocketId < MAX_SOCKET; SocketId++) {  //loop through sockets
            RcDebugPrint (SDBG_DEFAULT, "Cold Fast Boot Check\n");
            if (ProcPpinChangedCheck (Host, SocketId)){  //Per-socket check
              setupChanged = 1;
            }
          }
        }
      } //
    } //cold-boot check
  }

  RcDebugPrint (SDBG_DEFAULT,"setupChanged: %d\n", setupChanged);

  //
  // Enable WarmBoot if there haven't been any changes to setup
  //
  if (!setupChanged) {
    //
    // Nothing has changed from boot to boot.  Now, see if we can take a fast
    // path through MRC based on the input parameter ATTEMPT_FAST_BOOT.
    //
    if (GetSysBootMode () == NormalBoot) {
      //
      // If input parameters request a fast boot attempt, change the boot type to
      // WarmBootFast or ColdBootFast, depending on the type of boot.
      //
      if ((Host->var.mem.subBootMode == WarmBoot) && (Setup->mem.options & ATTEMPT_FAST_BOOT)) {
        Host->var.mem.subBootMode = WarmBootFast;
        Host->var.mem.socket[socket].socketSubBootMode = WarmBootFast;
        // For fast warm boot, skip memory init.  This is an optimization for boot time
        // and it is also a requirement for ME UMA systems because the ME UMA memory
        // must not be re-initialized on a warm boot.
        //
        Host->var.mem.skipMemoryInit = 1;
      }
      if ((Host->var.mem.subBootMode == ColdBoot) && (Setup->mem.options & ATTEMPT_FAST_BOOT_COLD)) {
        if (AttemptColdBootFastChip (Host)) {
          Host->var.mem.subBootMode = ColdBootFast;
          Host->var.mem.socket[socket].socketSubBootMode = ColdBootFast;
        } else {
          if (Host->var.mem.firstPass != 0) {
            RcDebugPrint (SDBG_DEFAULT, "ColdBootFast disabled, clearing the MRC NVRAM structure for ColdBoot.\n");
            Host->var.mem.firstPass = 0;
            ZeroMem ((UINT8 *) &Host->nvram.mem, sizeof (struct memNvram));
          }
        }
      }  // Attempt Fast Cold Boot
    } // NormalBoot
  } else {
    //
    // If we are on an S3 Resume path and something has changed,
    // do not attempt an S3 boot.  Rather, go back to a normal boot.
    //
    // HSD 3614451: MRC parameter mismatch results in hybrid boot (S3 tag)
    // Make a parameter mismatch in S3 path a Fatal error
    //
    if (GetSysBootMode () == S3Resume) {
      RcDebugPrint (SDBG_DEFAULT, "S3 resume path but something changed\n");
      FatalError (ERR_MRC_STRUCT, ERR_INVALID_BOOT_MODE);
    }
    //
    // If this is our first pass through this routine, then zero out the NVRAM
    // structure.  We don't do this on subsequent passes because they would blow
    // away the NVRAM data for CPUs that have already been initialized in this
    // routine on previous passes.
    //
    if (Host->var.mem.firstPass != 0) {
      RcDebugPrint (SDBG_DEFAULT, "Clearing the MRC NVRAM structure.\n");
      Host->var.mem.firstPass = 0;
      ZeroMem ((UINT8 *) &Host->nvram.mem, sizeof (struct memNvram));
    }
  }

  Host->var.mem.DcpmmWaitMediaReady = FALSE;

  //
  // There are no differences between Warm and Cold boot from an MRC
  // perspective so take the warm boot and make it go through the cold boot path.
  //
  if (Host->var.mem.subBootMode == WarmBoot) {
    RcDebugPrint (SDBG_DEFAULT, "bootMode = NormalBoot\n");
    RcDebugPrint (SDBG_DEFAULT, "subBootMode = WarmBoot --> ColdBoot\n");
    Host->var.mem.subBootMode = ColdBoot;

    //
    // Set this flag when going through a Slow Warm reset to indicate to
    // DCPMM DIMMs that they need to wait for media ready after CKE is assertion.
    // This flag does not need to be set on Fast Warm resets because that path
    // can be identified by checking Host->var.mem.subBootMode.
    //
    Host->var.mem.DcpmmWaitMediaReady = TRUE;
  }
  //
  // Copy Host setup parameters
  //
  Host->var.mem.ddrFreqLimit = Setup->mem.ddrFreqLimit;
  //
  // Check if QR DIMM is disabled in HW
  //

  if (GetDisableQRDimmChip (Host, socket) == TRUE) {
    Host->var.mem.socket[socket].maxRankDimm = 2;
  } else {
    Host->var.mem.socket[socket].maxRankDimm = MAX_RANK_DIMM;
  }
  for (ch = 0; ch < MaxChDdr; ch++) {

    if (GetChnDisableChip (Host, socket, ch)) {
      (*channelNvList)[ch].enabled = 0;
    }

    Host->var.mem.socket[socket].channelList[ch].numDimmSlots = Setup->mem.socket[socket].ddrCh[ch].numDimmSlots;
    //
    // Check if DIMM slots 1 and 2 are disabled in HW
    //
    if ((Setup->mem.socket[socket].ddrCh[ch].numDimmSlots > 1) && GetDisable2dpcChip (Host, socket)) {
      Host->var.mem.socket[socket].channelList[ch].numDimmSlots = 1;
    }

    //
    // Check if DIMM slot 2 is disabled in HW
    //
    if ((Setup->mem.socket[socket].ddrCh[ch].numDimmSlots > 2) && GetDisable3dpcChip (Host, socket)) {
      Host->var.mem.socket[socket].channelList[ch].numDimmSlots = 2;
    }
  } // ch loop

  CheckMCPPresent (Host, socket);


  // At this point, NVRAM has been cleared if necessary.  Save proc PPIN info to NVRAM structure if necessary.
  if ((GetSysBootMode () == NormalBoot) && ((Host->var.mem.subBootMode == ColdBoot) || (Host->var.mem.subBootMode == ColdBootFast))) {
    if ((Host->nvram.mem.socket[socket].procPpin.hi != Host->var.mem.socket[socket].procPpin.hi) ||
        (Host->nvram.mem.socket[socket].procPpin.lo != Host->var.mem.socket[socket].procPpin.lo)) {

      Host->nvram.mem.socket[socket].procPpin.hi = Host->var.mem.socket[socket].procPpin.hi;
      Host->nvram.mem.socket[socket].procPpin.lo = Host->var.mem.socket[socket].procPpin.lo;
    }
  }

#ifdef DEBUG_CODE_BLOCK
  if (Setup->common.options & HALT_ON_ERROR_AUTO) {
    FatalErrSetMaxSDBGSelect (Host, MAX_SDBG_FATAL_ERR_SET);
  }

#endif // DEBUG_CODE_BLOCK
#ifdef LRDIMM_SUPPORT
  Host->var.mem.InPbaWaMode = 0;
  Host->var.mem.InOvrRttPrkMode = 0;
#endif // LRDIMM_SUPPORT

  return SUCCESS;
}


UINT32
ProcPpinChangedCheck (
  PSYSHOST Host,
  UINT8    socket
  )
/*++

Routine Description:

  Check if any processor has been changed across boots

Arguments:

  Host  - Pointer to the system Host (root) structure

Returns:

  None.

--*/
//HSX HSD s5001717
{
  if (SocketPresent (socket)) {

    Host->var.mem.socket[socket].procPpin = GetPpinChip (Host, socket);

    if (Host->var.mem.socket[socket].procPpin.hi == 0 && Host->var.mem.socket[socket].procPpin.lo == 0) {
      // If PPIN is not supported, disable cold fast boot.
        RcDebugPrint (SDBG_DEFAULT, "Cold fast boot not capable\n");
      return TRUE;
    }

#ifdef DEBUG_CODE_BLOCK
    RcDebugPrint (SDBG_DEFAULT, "Checking PPIN\n");
    RcDebugPrint (SDBG_DEFAULT, "nvram[%d].ppin.hi: 0x%x, var[%d].ppin.hi: 0x%x \n", socket, Host->nvram.mem.socket[socket].procPpin.hi, socket, Host->var.mem.socket[socket].procPpin.hi );
    RcDebugPrint (SDBG_DEFAULT, "nvram[%d].ppin.lo: 0x%x, var[%d].ppin.lo: 0x%x \n", socket, Host->nvram.mem.socket[socket].procPpin.lo, socket, Host->var.mem.socket[socket].procPpin.lo );
#endif //DEBUG_CODE_BLOCK

    //
    // Check if processors changed
    //   First check if the NVRAM values are both 0.  If so then we have a possible condition where a DIMM on a non-sbsp socket
    //   has been swapped out and NVRAM is getting cleared for all sockets. Then the sockets take turns detecting a difference
    //   in PPIN and then clearing the NVRAM thus forcing the MRC to constantly detect a changed DIMM due to mismatched PPIN.
    //   This will cause the MRC to always boot in Cold Boot, never taking the fast path.
    //
    if ((Host->nvram.mem.socket[socket].procPpin.hi) == 0 && (Host->nvram.mem.socket[socket].procPpin.lo == 0)) {
      Host->nvram.mem.socket[socket].procPpin.hi = Host->var.mem.socket[socket].procPpin.hi;
      Host->nvram.mem.socket[socket].procPpin.lo = Host->var.mem.socket[socket].procPpin.lo;
    }
    if ((Host->nvram.mem.socket[socket].procPpin.hi != Host->var.mem.socket[socket].procPpin.hi) || (Host->nvram.mem.socket[socket].procPpin.lo != Host->var.mem.socket[socket].procPpin.lo)) {
      RcDebugPrint (SDBG_DEFAULT, "PPIN Changed\n");
      return TRUE;
    }//if-procPpin
  }
  return FALSE;
}

UINT16
MemGetCmosDate (
  PSYSHOST Host
  )
/*++

  Read and return a linear date based on year, month, and day of the month from RTC

  @param Host  - Pointer to the system Host (root) structure

  @retval linear approximate # of day's since Jan 1 2000

--*/
{
  UINT16 retVal = 0;
  UINT8  updateInProgragessBit = 1;
  UINT16 cmosDay, cmosMonth, cmosYear;

  // We do not poll the "update in progress" bit since we are only interested in date/month/year
  // and accuracy is not paramount.
  while (updateInProgragessBit) {
    OutPort8 (RTC_INDEX_REG, RTC_NMI_MASK | RTC_REG_A_INDEX);
    updateInProgragessBit = ((UINT8) ((InPort8 (RTC_DATA_REG)) >> 7) & 1); //Just read BIT7

    //RcDebugPrint (SDBG_MINMAX,
    //              "updateInProgragessBit = %u\n", updateInProgragessBit);
    if (updateInProgragessBit == 0) {
      break;
    }
  }

  OutPort8 (RTC_INDEX_REG, RTC_NMI_MASK | RTC_DAY_OF_MONTH_REG); // Select Day of Month
  cmosDay = (UINT16) BCDtoDecimal (InPort8 (RTC_DATA_REG));


  OutPort8 (RTC_INDEX_REG, RTC_NMI_MASK | RTC_MONTH_REG); // Select Month
  cmosMonth = (UINT16) BCDtoDecimal (InPort8 (RTC_DATA_REG));

  OutPort8 (RTC_INDEX_REG, RTC_NMI_MASK | RTC_YEAR_REG); // Select year (2 digits)
  cmosYear = (UINT16) BCDtoDecimal (InPort8 (RTC_DATA_REG));

//  RcDebugPrint (SDBG_MINMAX,
//                          "Year = %u, Month = %u, Day = %u\n", cmosYear, cmosMonth, cmosDay);

  // Check for valid date range
  if ((cmosYear < 99) && ((cmosMonth > 0) && (cmosMonth <= 12)) && ((cmosDay > 0) && (cmosDay <= 31))) {
    // Subtract one from month make 0 based for multiplication;
    cmosMonth = cmosMonth - 1;

    retVal = (cmosYear * 365) + (cmosMonth * 30) + cmosDay;
  }
  return (retVal);
}

UINT32
DetectDIMMEnabledChange (
  PSYSHOST Host,
  UINT8 socket
  )
/*++

Routine Description:

  Detect DIMM Enabled Change

Arguments:

  Host   - Pointer to the system Host (root) structure
  socket - socket id

Returns:

  TRUE / FALSE

--*/
{
  UINT32              result;
  UINT8               ch;
  UINT8               dimm;
  UINT8               rank;
  UINT8               SPDReg;
  UINT8               i;
  struct dimmNvram    (*dimmNvList)[MAX_DIMM];
  UINT16              SPDMMfgId = 0;
  UINT16              SPDModDate = 0;
  UINT8               SPDModSN[SPD_MODULE_SERIAL];
  SYS_SETUP           *Setup;
  MRC_STATUS          Status;
  BOOLEAN             DimmMapOut;
  UINT8               MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  Setup = GetSysSetupPointer ();

  result = FALSE;

  for (ch = 0; ch < MaxChDdr; ch++) {
    dimmNvList = GetDimmNvList (Host, socket, ch);
    for (dimm = 0; dimm < Setup->mem.socket[socket].ddrCh[ch].numDimmSlots; dimm++) {
      DimmMapOut = TRUE;           // Assume DimmMapOut = TRUE first
      Status = SpdGetModuleType (socket, ch, dimm, &SPDReg);
      if (((*dimmNvList)[dimm].dimmPresent == 0 && Status == MRC_STATUS_SUCCESS) ||
          ((*dimmNvList)[dimm].dimmPresent == 1 && Status != MRC_STATUS_SUCCESS)) {
        RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                "DIMM mismatch found\n");
        result = TRUE;
        break;
      } else if (Status == MRC_STATUS_SUCCESS) {
        for (rank = 0; rank < MAX_RANK_DIMM; rank++) {
          if (Setup->mem.socket[socket].ddrCh[ch].dimmList[dimm].mapOut[rank] == 0) {
            DimmMapOut = FALSE;
          }
        } // rank loop
        if (!DimmMapOut) {
          RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                "DIMM found check SN\n");
          // Check Mfg ID
          SpdGetModuleManufacturerId (socket, ch, dimm, &SPDMMfgId);

          if ((*dimmNvList)[dimm].SPDMMfgId != SPDMMfgId) {
            RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                  "DIMM Mfg ID mismatch found %x %x\n",(*dimmNvList)[dimm].SPDMMfgId,SPDMMfgId);
            result = TRUE;
            break;
          }
          // Check mfg location
          SpdGetModuleManufacturingLocation (socket, ch, dimm, &SPDReg);

          if ((*dimmNvList)[dimm].SPDMMfLoc != SPDReg) {
            RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                  "DIMM Mfg Location mismatch found\n");
            result = TRUE;
            break;
          }
          // Check MSB module date
          SpdGetModuleManufacturingDate (socket, ch, dimm, &SPDModDate);

          if ((*dimmNvList)[dimm].SPDModDate != SPDModDate) {
            RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                  "DIMM Module date mismatch found\n");
            result = TRUE;
            break;
          }
          // Check module Serial Number Bytes 325 - 328
          GetModuleSerialNumber (Host, socket, ch, dimm, &SPDModSN[0]);
          for (i = 0; i < SPD_MODULE_SERIAL; i++) {
            if ((*dimmNvList)[dimm].SPDModSN[i] != SPDModSN[i]) {
              RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                    "DIMM SN mismatch found\n");
              result = TRUE;
              break;
            }
          } // i loop
        } // if (!DimmMapOut)
      } else {
        RcDebugPrintWithDevice (SDBG_MAX, socket, ch, dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                      "No Dimm\n");
      }
    } // dimm loop
  } // ch loop

  return result;
} // DetectDIMMEnabledChange

/**

  Determine which bootmode to use for all sockets

  @param[in] Host - Pointer to sysHost, the system Host (root) structure

  @retval None

**/
UINT32
DetermineBootMode (
  PSYSHOST Host
  )
{
  UINT8    socket;
  UINT32   socketPresentBitMap;
  UINT8    coldFastSupported = 0;
  UINT8    warmFastSupported = 0;
  UINT8    cpuCount = 0;
  BootMode SysBootMode;
  SYS_SETUP *Setup;

  Setup = GetSysSetupPointer ();

  SysBootMode = GetSysBootMode ();

  //
  // Check for differences between sockets
  //
  for (socket = 0; socket < MAX_SOCKET; socket++) {
    socketPresentBitMap = GetSocketPresentBitMap ();
    if ((socketPresentBitMap & (BIT0 << socket)) == 0) {
      continue;
    }
    if ((Setup->mem.socket[socket].enabled == 0)) {
      continue;
    }

    cpuCount++;
    //
    // 1. If any boot mode is cold boot, we must take cold boot
    //
    if (SysBootMode == NormalBoot && Host->var.mem.socket[socket].socketSubBootMode == ColdBoot) {
      Host->var.mem.subBootMode = ColdBoot;
      break;
    }

    //
    // 2. If all socket support Fast Cold, we will take Fast Cold
    //
    if (SysBootMode == NormalBoot && Host->var.mem.socket[socket].socketSubBootMode == ColdBootFast) {
      coldFastSupported++;
    }

    //
    // 3. If all socket support Fast Warm, we will take Fast Warm
    //
    if (SysBootMode == NormalBoot && Host->var.mem.socket[socket].socketSubBootMode == WarmBootFast) {
      warmFastSupported++;
    }
  }

  if (coldFastSupported == cpuCount) {
    Host->var.mem.subBootMode = ColdBootFast;
  }

  if (warmFastSupported == cpuCount) {
    Host->var.mem.subBootMode = WarmBootFast;
  }

  return SUCCESS;
}

/**

  Initialize Memory Structure

  @param[in] Host - Pointer to sysHost, the system Host (root) structure

  @retval None

**/
UINT32
InitStructuresLate (
  PSYSHOST Host
  )
{
  UINT8                     socket;
  UINT8                     McId;
  UINT8                     ch;
  UINT8                     dimm;
  UINT8                     rank;
  BootMode                  SysBootMode;
  struct dimmNvram (*dimmNvList)[MAX_DIMM];
#ifdef LRDIMM_SUPPORT
  UINT8                     strobe;
  struct ddrRank (*rankList)[MAX_RANK_DIMM];
#endif // LRDIMM_SUPPORT
  SYS_SETUP                 *Setup;
  UINT8                     MaxChDdr;

  MaxChDdr = GetMaxChDdr ();
  Setup = GetSysSetupPointer ();

  SysBootMode = GetSysBootMode ();
  socket = Host->var.mem.currentSocket;

  if (SysBootMode == S3Resume) {
    //
    // Initialize to skip memory init.  We want to skip memory init
    // on S3.
    //
    Host->var.mem.skipMemoryInit = 1;
  } else {
    if (Host->var.mem.subBootMode == WarmBootFast) {
      // For fast warm boot, skip memory init.  This is an optimization for boot time
      // and it is also a requirement for ME UMA systems because the ME UMA memory
      // must not be re-initialized on a warm boot.
      //
      Host->var.mem.skipMemoryInit = 1;
    } else {
      //
      // On normal boot paths (i.e. not S3), do meminit by default.
      //
      Host->var.mem.skipMemoryInit = 0;
    }
  }

  if (SysBootMode == NormalBoot && Host->var.mem.subBootMode == ColdBoot) {
    if (Host->var.mem.firstPass != 0) {
#ifdef DEBUG_CODE_BLOCK
      RcDebugPrint (SDBG_DEFAULT, "Clearing the MRC NVRAM structure for ColdBoot.\n");
#endif // #ifdef DEBUG_CODE_BLOCK
      Host->var.mem.firstPass = 0;
      ZeroMem ((UINT8 *)&Host->nvram.mem, sizeof (struct memNvram));
    }

    SetIMCEnabledChip (Host, socket);

    for (ch = 0; ch < MaxChDdr; ch++) {
      McId = GetMCID (Host, socket, ch);
      Host->nvram.mem.socket[socket].imc[McId].enabled = Host->var.mem.socket[socket].imcEnabled[McId];
      if (Host->var.mem.socket[socket].ImcPresent[McId]) {
        Host->nvram.mem.socket[socket].channelList[ch].enabled = Setup->mem.socket[socket].ddrCh[ch].enabled;
      } else {
        Host->nvram.mem.socket[socket].channelList[ch].enabled = 0;
      }

      dimmNvList = GetDimmNvList (Host, socket, ch);

      for (dimm = 0; dimm < Host->var.mem.socket[socket].channelList[ch].numDimmSlots; dimm++) {

        if (PcdGetBool (PcdMrcXmpSupport)) {
          (*dimmNvList)[dimm].XMPChecksum = Setup->mem.XMPChecksum[socket][ch][dimm];
        }

        for (rank = 0; rank < MAX_RANK_DIMM; rank++) {
          (*dimmNvList)[dimm].mapOut[rank] = Setup->mem.socket[socket].ddrCh[ch].dimmList[dimm].mapOut[rank];
#ifdef LRDIMM_SUPPORT
          //
          // For DDR4 LRDIMM, initialize the training data structures back to 0 before JEDEC INIT
          // so that all training will start from the same state. THe NVRAM data is only needed
          // for fast cold boot support
          //
          if (IsLrdimmPresent (socket, ch, dimm)) {
            rankList = GetRankNvList (Host, socket, ch, dimm);

            for (strobe = 0; strobe < MAX_STROBE; strobe++) {
              (*rankList)[rank].lrBuf_FxBC2x3x[strobe] = 0;
              (*rankList)[rank].lrBuf_FxBC4x5x[strobe] = 0;
              (*rankList)[rank].lrBuf_FxBC8x9x[strobe] = 0;
              (*rankList)[rank].lrBuf_FxBCAxBx[strobe] = 0;
              if (strobe < (MAX_STROBE / 2)) {
                (*rankList)[rank].lrBuf_FxBCCxEx[strobe] = 0;
                (*rankList)[rank].lrBuf_FxBCDxFx[strobe] = 0;
              }
            }
          }
#endif // #ifdef LRDIMM_SUPPORT
        } // rank
      } // dimm
    } // ch
  }

  return SUCCESS;
}
