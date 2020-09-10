/** @file
  This file is PEI Memory Policy update library class implementation.

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

#include <KtiSetupDefinitions.h>
#include <Guid/PprVariable.h>
#include <Guid/PartialMirrorGuid.h>
#include <Guid/MemoryOverwriteControl.h>
#include <Ppi/MemoryPolicyPpi.h>
#include <Library/LegacyRmtLib.h>
#include <Library/SetupLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/CrcLib.h>
#include <Library/PeiServicesLib.h>
#include <Ppi/ReadOnlyVariable2.h>
#include <Guid/SetupVariable.h>
#include <PlatformInfoTypes.h>
#include <Library/HobLib.h>
#include <Guid/PlatformInfo.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Library/CpuAndRevisionLib.h>
#include <Library/KtiApi.h>
// APTIOV_SERVER_OVERRIDE_RC_START : Fixed S3 failed since options are changed.
#include <Library/SysHostPointerLib.h>
#include <IioUniversalData.h> 
// APTIOV_SERVER_OVERRIDE_RC_END : Fixed S3 failed since options are changed.
#include <Library/EmulationConfigurationLib.h>
#include <Library/SiliconWorkaroundLib.h>
#include <Library/SysHostPointerLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/UbaGpioPlatformConfig.h>
#include <Library/PeiMemoryPolicyLib.h>
#include <Library/PeiMemoryPolicyUpdateLib.h>

#include <Memory/SetupValues.h>

//
// This may be obsolete, check.
//
#pragma pack(1)
typedef struct {
  UINT8   TpmPresent;
  BOOLEAN HideTpm;
  UINT8   TpmCurrentState;
  BOOLEAN MorState;
} TCG_SETUP;
#pragma pack()

/**
  Update the memory policy default values.

  Updates the memory policy based on setup variable.

  @param[in]  SetupData   Ptr to the data from Setup.
  @param[out] Policy      Ptr to the policy to set memFlows in.

  @retval None

**/

VOID
UpdateMemFlowsFromSetup (
// APTIOV_SERVER_OVERRIDE_RC_START : To solve redefinition error
  IN INTEL_SETUP_DATA *SetupData,
  // APTIOV_SERVER_OVERRIDE_RC_END : To solve redefinition error
  OUT SYS_SETUP *Policy
  )
{
  BOOLEAN Is14nmCpu;

  Is14nmCpu = IsCpuAndRevision (CPU_SKX, REV_ALL) || IsCpuAndRevision (CPU_CLX, REV_ALL) || IsCpuAndRevision (CPU_CPX, REV_ALL);

  //
  // First get the raw memFlows and memFlowsExt values from setup.
  //

  Policy->mem.memFlows     = SetupData->SocketConfig.MemoryConfig.memFlows;
  Policy->mem.memFlowsExt  = SetupData->SocketConfig.MemoryConfig.memFlowsExt;
  Policy->mem.memFlowsExt2 = SetupData->SocketConfig.MemoryConfig.memFlowsExt2;
  Policy->mem.memFlowsExt3 = SetupData->SocketConfig.MemoryConfig.memFlowsExt3;

  //
  // Next handle individual setup options that go in memFlows.
  //

  SetPolicyMemFlow (EarlyReadVrefCenteringTraining,
                    SetupData->SocketConfig.MemoryConfig.readVrefCenter,
                    Policy
                    );

  SetPolicyMemFlow (EarlyWriteVrefCenteringTraining,
                    SetupData->SocketConfig.MemoryConfig.wrVrefCenter,
                    Policy
                    );

  SetPolicyMemFlow (RoundTripOptimizeTraining,
                    SetupData->SocketConfig.MemoryConfig.RoundTripLatency,
                    Policy
                    );

  SetPolicyMemFlow (TxEqTraining,
                    SetupData->SocketConfig.MemoryConfig.txEqCalibration,
                    Policy
                    );

  SetPolicyMemFlow (ImodeTraining,
                    SetupData->SocketConfig.MemoryConfig.iModeTraining,
                    Policy
                    );

  SetPolicyMemFlow (CmdVrefCenteringTraining,
                    SetupData->SocketConfig.MemoryConfig.CmdVrefEnable,
                    Policy
                    );

  SetPolicyMemFlow (LrdimmWriteVrefCenteringTraining,
                    SetupData->SocketConfig.MemoryConfig.LrDimmWrVrefEn,
                    Policy
                    );

  SetPolicyMemFlow (LrdimmReadVrefCenteringTraining,
                    SetupData->SocketConfig.MemoryConfig.LrDimmRdVrefEn,
                    Policy
                    );

  SetPolicyMemFlow (CmdNormalization,
                    SetupData->SocketConfig.MemoryConfig.CmdNormalization,
                    Policy
                    );

  SetPolicyMemFlow (McROnTraining,
                    SetupData->SocketConfig.MemoryConfig.McRonEn,
                    Policy
                    );

  SetPolicyMemFlow (DramROnTraining,
                    SetupData->SocketConfig.MemoryConfig.DramRonEn,
                    Policy
                    );

  SetPolicyMemFlow (McOdtTraining,
                    SetupData->SocketConfig.MemoryConfig.RxOdtEn,
                    Policy
                    );

  SetPolicyMemFlow (RxCtleTraining,
                    SetupData->SocketConfig.MemoryConfig.RxCtleTrnEn,
                    Policy
                    );

  //
  // LrDimm tx and rx have separate questions, but same memflow
  //

  SetPolicyMemFlow (LrdimmBacksideDqTraining,
                    SetupData->SocketConfig.MemoryConfig.LrDimmTxDqCentering,
                    Policy
                    );

  SetPolicyMemFlow (LrdimmBacksideDqTraining,
                    SetupData->SocketConfig.MemoryConfig.LrDimmRxDqCentering,
                    Policy
                    );

  if (!Is14nmCpu) {

    SetPolicyMemFlow (CmdTxEqTraining,
                      SetupData->SocketConfig.MemoryConfig.CmdTxEqCalibration,
                      Policy
                      );

  }

  SetPolicyMemFlow (PhaseCrosstalkTraining,
                    SetupData->SocketConfig.MemoryConfig.PxcTraining,
                    Policy
                    );

  SetPolicyMemFlow (TxDdjcTraining,
                    SetupData->SocketConfig.MemoryConfig.DdjcTraining,
                    Policy
                    );
  if (FixedPcdGetBool (PcdBiosDfxKnobEnabled)) {
    SetPolicyMemFlow (DramRxEqTraining,
                      SetupData->SocketConfig.MemoryConfig.DfxDramRxEqTraining,
                      Policy
                      );
  } //if (FixedPcdGetBool (PcdBiosDfxKnobEnabled))

  SetPolicyMemFlow (
    RxDfeDdr5Training,
    (SetupData->SocketConfig.MemoryConfig.RxDfeDdr5Training == RX_DFE_DDR5_TRAINING_ENABLE),
    Policy
    );

  SetPolicyMemFlow (
    TxDfeDdr5Training,
    (SetupData->SocketConfig.MemoryConfig.TxDfeDdr5Training == TX_DFE_DDR5_TRAINING_ENABLE),
    Policy
    );

  if (SetupData->SocketConfig.MemoryConfig.LrdimmDbDfeTraining != 0) {
    SetPolicyMemFlow (LrdimmDbDfe, TRUE, Policy);
  } else {
    SetPolicyMemFlow (LrdimmDbDfe, FALSE, Policy);
  }

  SetPolicyMemFlow (DramDutyCycleAdjusterTraining,
                    SetupData->SocketConfig.MemoryConfig.DramDutyCycleAdjusterTrainingKnob,
                    Policy
                    );

  return;

} // UpdateMemFlowsFromSetup

/**
  This function returns the memory topology for a specific socket
  and channel

  @param[in] Socket - Socket
  @param[in] Ch     - Channel

  @retval Memory Topology present in the channel:
          DaisyChainTopology
          InvSlotsDaisyChainTopology
          TTopology
**/
EFI_MEMORY_TOPOLOGY_TYPE
GetMemTopology (
  IN UINT8    Socket,
  IN UINT8    Ch
  )
{
  EFI_PLATFORM_INFO *PlatformInfo;
  EFI_HOB_GUID_TYPE *GuidHob;

  GuidHob      = GetFirstGuidHob (&gEfiPlatformInfoGuid);
  PlatformInfo = GET_GUID_HOB_DATA (GuidHob);
  ASSERT (PlatformInfo);

  return PlatformInfo->MemoryTopology[Socket][Ch];

} // GetMemTopology

/**
  This function returns the memory channel connector type
  for the channel specified in the parameters list

  @param[in] Socket - Socket
  @param[in] Ch     - Channel

  @retval Memory connectory type present in the channel:
          DimmConnectorPth        - Through hole connector
          DimmConnectorSmt        - Surface mount connector
          DimmConnectorMemoryDown - Platform soldered DRAMs
**/
EFI_MEMORY_DIMM_CONNECTOR_TYPE
GetMemConnectorType (
  IN UINT8    Socket,
  IN UINT8    Ch
  )
{
  EFI_PLATFORM_INFO *PlatformInfo;
  EFI_HOB_GUID_TYPE *GuidHob;

  GuidHob      = GetFirstGuidHob (&gEfiPlatformInfoGuid);
  PlatformInfo = GET_GUID_HOB_DATA (GuidHob);
  ASSERT (PlatformInfo);

  return PlatformInfo->MemoryConnectorType[Socket][Ch];

} // GetMemConnectorType

/**
  Get the platform type.

  @param[out]  PlatformType       A pointer to the buffer to hold the platform type.

  @retval EFI_SUCCESS             Success
  @retval EFI_INVALID_PARAMETER   Failure
**/
EFI_STATUS
GetPlatformTypeLocal (
  OUT UINT8 *PlatformType
  )
{

  *PlatformType = TypePlatformDefault;


  EFI_PLATFORM_INFO                     *PlatformInfo;
  EFI_HOB_GUID_TYPE                     *GuidHob;

  GuidHob       = GetFirstGuidHob (&gEfiPlatformInfoGuid);
  PlatformInfo  = GET_GUID_HOB_DATA (GuidHob);

  ASSERT (PlatformInfo);

  *PlatformType = PlatformInfo->BoardId;


  return EFI_SUCCESS;

} // GetPlatformTypeLocal

/**
  Update the memory storage policy values.

  Enable / disable ADR related specific features.
  This routine enables batterybacked feature on all the supported channels.
  This routine also sets up CPLD into one of two mode (battery backed mode or C2F mode)
  Note that CPLD setup code is specific to Emerald Point platform.
  This routine needs to be updated with any other board specific changes related to ADR.

  @param[in]   SetupData          A pointer to the setup data.
  @param[out]  PolicyPpi          A pointer to PPI to fill in.

  @retval EFI_SUCCESS             Success
  @retval EFI_INVALID_PARAMETER   Failure

**/

VOID
UpdateMemoryStoragePolicy (
// APTIOV_SERVER_OVERRIDE_RC_START : To solve structure redefinition error in AMI.
  IN INTEL_SETUP_DATA  *SetupData,
// APTIOV_SERVER_OVERRIDE_RC_END : To solve structure redefinition error in AMI.
  OUT MEMORY_POLICY_PPI *PolicyPpi
  )
{
  UINT8      Socket;
  UINT8      Channel;
  SYS_SETUP  *Setup;

  if (PcdGetBool (PcdNvDimmEn) != TRUE) {
    return;
  }

  Setup = PolicyPpi->SysSetup;

  Setup->mem.ADRDataSaveMode = SetupData->SocketConfig.MemoryConfig.ADRDataSaveMode;

  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
    for (Channel = 0; Channel < MAX_CH; Channel++) {
      if (SetupData->SocketConfig.MemoryConfig.ADREn && SetupData->SocketConfig.MemoryConfig.ADRDataSaveMode == ADR_BBU) {
        Setup->mem.socket[Socket].ddrCh[Channel].batterybacked = 1;
      } else {
        Setup->mem.socket[Socket].ddrCh[Channel].batterybacked = 0;
      }
    }
  }

  if (SetupData->SocketConfig.MemoryConfig.ADRDataSaveMode == ADR_NVDIMM) {
    Setup->mem.check_pm_sts = 0;
    Setup->mem.check_platform_detect = 0;
    Setup->mem.eraseArmNVDIMMS = SetupData->SocketConfig.MemoryConfig.eraseArmNVDIMMS;
    Setup->mem.restoreNVDIMMS = SetupData->SocketConfig.MemoryConfig.restoreNVDIMMS;
  } else {
    Setup->mem.eraseArmNVDIMMS = 0;
    Setup->mem.restoreNVDIMMS = 0;
  }

  Setup->mem.interNVDIMMS = SetupData->SocketConfig.MemoryConfig.interNVDIMMS;
  Setup->mem.NvDimmEnergyPolicy = SetupData->SocketConfig.MemoryConfig.NvDimmEnergyPolicy;

  return;

} // UpdateMemoryStoragePolicy
//
// Current EDK2 framework does not provide negative setup options values
// so for DFE options that need to be negative user needs to add +100
// for example to represent -10 user needs to specify it as 110
//
#define ADJ_NEG_VALUES(a) ((a > 100) ?  0 - (a - 100) : a)

/**

  Secondary ADR dection method other than PCH_PM_STS

  @param[in] None

  @retval 0  - No ADR recovery
  @retval 1  - Platform should recover from ADR

**/
UINT32
DetectADR (
  VOID
  )
{
  UINT32 ADRevent;
  EFI_STATUS Status;

  Status = GpioGetFmAdrTriggerPadVal (&ADRevent);
  if (EFI_ERROR(Status)) {
    return (UINT32)Status;
  }

  return ADRevent;

} // DetectADR

/**
  Set the ADR Gpios.

  @param[in]   AdrEnable  Whether ADR is enabled or not.

  @retval - None

**/

VOID
SetAdrEnablePadOutVal (
  IN BOOLEAN AdrEnable
  )
{
  if (PcdGetBool (PcdNvDimmEn)) {
    if (AdrEnable) {
      GpioSetAdrEnablePadOutVal (TRUE);
      DEBUG ((EFI_D_INFO, "Adr Enabled in setup\n"));
    } else {
      GpioSetAdrEnablePadOutVal (FALSE);
      DEBUG ((EFI_D_INFO, "Adr Disabled in setup\n"));
    }
  }

  return;

} // SetAdrEnablePadOutVal

/**
  Update the memory policy default values.

  Updates the memory policy based on setup variable.

  @param[out]  PolicyPpi          A pointer to PPI to fill in.
  @param[in, out] SetupData       Pointer to SETUP_DATA structure.

  @retval EFI_SUCCESS             Success
  @retval EFI_INVALID_PARAMETER   Failure

**/

EFI_STATUS
EFIAPI
UpdatePeiMemoryPolicy (
  OUT MEMORY_POLICY_PPI     *PolicyPpi,
  // APTIOV_SERVER_OVERRIDE_RC_START : To solve redefinition error
  IN OUT    INTEL_SETUP_DATA      *SetupData
  // APTIOV_SERVER_OVERRIDE_RC_END : To solve redefinition error
  )
{
  EFI_STATUS                         Status;
  SYS_SETUP                          *Setup;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI    *PeiVariable;
  UINT8                              socket;
  UINT8                              Imc;
  UINT8                              ch;
  UINT8                              dimm;
  UINT8                              pagePolicy;
  UINT8                              i;
  UINT8                              j;
  UINT16                             pprCrc16 = 0;
  UINT16                             curCrc16 = 0;
  PPR_ADDR_VARIABLE                  pprAddrVariable;
  PPR_ADDR_VARIABLE                  *pprAddrVariablePtr;
  UINTN                              VariableSize;
  UINT8                              PlatformType;
  BOOLEAN                            Is14nmCpu;
  EFI_PLATFORM_INFO                  *PlatformInfo;
  EFI_HOB_GUID_TYPE                  *GuidHob;

  Is14nmCpu = IsCpuAndRevision (CPU_SKX, REV_ALL) || IsCpuAndRevision (CPU_CLX, REV_ALL) || IsCpuAndRevision (CPU_CPX, REV_ALL);

  Setup = PolicyPpi->SysSetup;

  Status = PeiServicesLocatePpi (
             &gEfiPeiReadOnlyVariable2PpiGuid,
             0,
             NULL,
             &PeiVariable
           );
  if (EFI_ERROR (Status)) {
    ASSERT (FALSE);
    return Status;
  }

  VariableSize = sizeof (PPR_ADDR_VARIABLE);

  Status = PeiVariable->GetVariable (
             PeiVariable,
             PPR_VARIABLE_NAME,
             &gEfiPprVariableGuid,
             NULL,
             &VariableSize,
             &pprAddrVariable
           );
  if (!EFI_ERROR (Status)) {

    pprCrc16 =  pprAddrVariable.crc16;
    pprAddrVariable.crc16 = 0;

    Status = CalculateCrc16 (
               (VOID *) &pprAddrVariable,
               sizeof (PPR_ADDR_VARIABLE),
               &curCrc16
             );
  }

  if (EFI_ERROR(Status) || (pprCrc16 != curCrc16)) {
    DEBUG ((EFI_D_ERROR, "PPR Variable not found or CRC mismatch - Status: 0x%0x, Crc: 0x%0x, calc. Crc: 0x%0x!\n", Status, pprCrc16, curCrc16));
    pprAddrVariablePtr = NULL;
  } else {
    pprAddrVariablePtr = &pprAddrVariable;
  }

  Setup->mem.enforcePOR = SetupData->SocketConfig.MemoryConfig.EnforcePOR;

  Setup->mem.EnforcePopulationPor = SetupData->SocketConfig.MemoryConfig.EnforcePopulationPor;

  Setup->mem.PanicWm = SetupData->SocketConfig.MemoryConfig.PanicWm;

  Setup->mem.DataBufferDfe = SetupData->SocketConfig.MemoryConfig.LrdimmDbDfeTraining;

  //
  // Program Temperature refresh value
  //
  if (!Is14nmCpu) {
    Setup->mem.TempRefreshOption = SetupData->SocketConfig.MemoryConfig.TempRefreshOption;
    if (Setup->mem.TempRefreshOption) {
      Setup->mem.HalfxRefreshValue = SetupData->SocketConfig.MemoryConfig.HalfxRefreshValue;
      Setup->mem.TwoxRefreshValue = SetupData->SocketConfig.MemoryConfig.TwoxRefreshValue;
      Setup->mem.FourxRefreshValue = SetupData->SocketConfig.MemoryConfig.FourxRefreshValue;
    }
  }
  //
  // Post Package Repair options
  //
  Setup->mem.pprType = SetupData->SocketConfig.MemoryConfig.pprType;


  Setup->mem.EnableTapSweep = SetupData->SocketConfig.MemoryConfig.EnableTapSweep;

  Setup->mem.DfeGainBias = ADJ_NEG_VALUES (SetupData->SocketConfig.MemoryConfig.DfeGainBias);

  if (FixedPcdGetBool (PcdBiosDfxKnobEnabled)) {
    Setup->mem.Tap1Start  = ADJ_NEG_VALUES (SetupData->SocketConfig.MemoryConfig.Tap1Start);
    Setup->mem.Tap1End    = ADJ_NEG_VALUES (SetupData->SocketConfig.MemoryConfig.Tap1End);
    Setup->mem.Tap1Size   = ADJ_NEG_VALUES (SetupData->SocketConfig.MemoryConfig.Tap1Size);

    Setup->mem.Tap2Start  = ADJ_NEG_VALUES (SetupData->SocketConfig.MemoryConfig.Tap2Start);
    Setup->mem.Tap2End    = ADJ_NEG_VALUES (SetupData->SocketConfig.MemoryConfig.Tap2End);
    Setup->mem.Tap2Size   = ADJ_NEG_VALUES (SetupData->SocketConfig.MemoryConfig.Tap2Size);

    Setup->mem.Tap3Start  = ADJ_NEG_VALUES (SetupData->SocketConfig.MemoryConfig.Tap3Start);
    Setup->mem.Tap3End    = ADJ_NEG_VALUES (SetupData->SocketConfig.MemoryConfig.Tap3End);
    Setup->mem.Tap3Size   = ADJ_NEG_VALUES (SetupData->SocketConfig.MemoryConfig.Tap3Size);

    Setup->mem.Tap4Start  = ADJ_NEG_VALUES (SetupData->SocketConfig.MemoryConfig.Tap4Start);
    Setup->mem.Tap4End    = ADJ_NEG_VALUES (SetupData->SocketConfig.MemoryConfig.Tap4End);
    Setup->mem.Tap4Size   = ADJ_NEG_VALUES (SetupData->SocketConfig.MemoryConfig.Tap4Size);
  } //if (FixedPcdGetBool (PcdBiosDfxKnobEnabled))

  //  ddrFreqLimit
  //    0:Auto
  //    1:Force DDR-800
  //    3:Force DDR-1066
  //    5:Force DDR-1333
  //    7:Force DDR-1600
  //    9:Force DDR-1867
  //    11:Force DDR-2133
  //    13:Force DDR-2400
  //
  if (SetupData->SocketConfig.MemoryConfig.DdrFreqLimit == DDR_FREQ_AUTO) {
    Setup->mem.ddrFreqLimit = DDR_FREQ_AUTO;
  } else {
    Setup->mem.ddrFreqLimit = SetupData->SocketConfig.MemoryConfig.DdrFreqLimit;
  }


  Setup->mem.imcBclk = SetupData->SocketConfig.MemoryConfig.imcBclk;

  //
  // common options
  //
  Setup->common.options &= ~(PROMOTE_MRC_WARN_EN | HALT_ON_ERROR_EN | PROMOTE_WARN_EN | HALT_ON_ERROR_AUTO);

  if (SetupData->SocketConfig.MemoryConfig.promoteMrcWarnings) {
    Setup->common.options |= PROMOTE_MRC_WARN_EN;
  }

  if (SetupData->SocketConfig.MemoryConfig.promoteWarnings) {
    Setup->common.options |= PROMOTE_WARN_EN;
  }

  if (SetupData->SocketConfig.MemoryConfig.haltOnMemErr == HALT_ON_ERROR_ENABLE) {
    Setup->common.options |= HALT_ON_ERROR_EN;
  }

#ifdef DEBUG_CODE_BLOCK
  if (SetupData->SocketConfig.MemoryConfig.haltOnMemErr == HALT_ON_ERROR_AUTO_RESET) {
    Setup->common.options |= HALT_ON_ERROR_AUTO;
  }
#endif //DEBUG_CODE_BLOCK

  if (SetupData->SocketConfig.MemoryConfig.MultiThreaded == MULTI_THREADED_ENABLE) {
    Setup->mem.options |= MULTI_THREAD_MRC_EN;
  } else {
    Setup->mem.options &= ~MULTI_THREAD_MRC_EN;
  }

  // Setup option for memory test
  if (SetupData->SocketConfig.MemoryConfig.HwMemTest == MEM_TEST_ENABLE) {
    Setup->mem.options |= MEMORY_TEST_EN;
  } else {
    Setup->mem.options &= ~MEMORY_TEST_EN;
  }

  // Setup option for advanced memory test
  Setup->mem.AdvMemTestOptions = SetupData->SocketConfig.MemoryConfig.AdvMemTestOptions;
  Setup->mem.AdvMemTestResetList = SetupData->SocketConfig.MemoryConfig.AdvMemTestResetList;

  Setup->mem.AdvMemTestCondition = SetupData->SocketConfig.MemoryConfig.AdvMemTestCondition;
  Setup->mem.AdvMemTestCondVdd = SetupData->SocketConfig.MemoryConfig.AdvMemTestCondVdd;
  Setup->mem.AdvMemTestCondTwr = SetupData->SocketConfig.MemoryConfig.AdvMemTestCondTwr;
  Setup->mem.AdvMemTestCondTrefi = SetupData->SocketConfig.MemoryConfig.AdvMemTestCondTrefi;
  Setup->mem.AdvMemTestCondPause = SetupData->SocketConfig.MemoryConfig.AdvMemTestCondPause;

  //
  // Setup option for memory test loops
  //
  Setup->mem.memTestLoops = SetupData->SocketConfig.MemoryConfig.MemTestLoops;


  //
  // Setup option to offset the final memory training results
  //
  Setup->mem.trainingResultOffsetFunctionEnable = SetupData->SocketConfig.MemoryConfig.TrainingResultOffsetFunctionEnable;
  Setup->mem.offsetTxDq = SetupData->SocketConfig.MemoryConfig.OffsetTxDq;
  Setup->mem.offsetRxDq = SetupData->SocketConfig.MemoryConfig.OffsetRxDq;
  Setup->mem.offsetTxVref = SetupData->SocketConfig.MemoryConfig.OffsetTxVref;
  Setup->mem.offsetRxVref = SetupData->SocketConfig.MemoryConfig.OffsetRxVref;
  Setup->mem.offsetCmdAll = SetupData->SocketConfig.MemoryConfig.OffsetCmdAll;
  Setup->mem.offsetCmdVref = SetupData->SocketConfig.MemoryConfig.OffsetCmdVref;
  Setup->mem.offsetCtlAll = SetupData->SocketConfig.MemoryConfig.OffsetCtlAll;
  Setup->mem.OffsetRecEn = SetupData->SocketConfig.MemoryConfig.OffsetRecEn;


  //
  // Receive Enable Average Feature
  //
  Setup->mem.RecEnDelayAverage = SetupData->SocketConfig.MemoryConfig.RecEnDelayAverage;

  //
  // Memory type
  //
  if (SetupData->SocketConfig.MemoryConfig.DdrMemoryType == MEM_TYPE_AUTO) {
    Setup->mem.dimmTypeSupport = MEM_TYPE_DEFAULT;
  } else {
    Setup->mem.dimmTypeSupport = SetupData->SocketConfig.MemoryConfig.DdrMemoryType;
  }

  if (SetupData->SocketConfig.MemoryConfig.LegacyRmt == LEGACY_RMT_ENABLE) {
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

  if (SetupData->SocketConfig.MemoryConfig.EnableNgnBcomMargining == NGN_BCOM_MARGINING_ENABLE) {
    Setup->mem.enableNgnBcomMargining = NGN_BCOM_MARGINING_ENABLE;
  } else {
    Setup->mem.enableNgnBcomMargining = NGN_BCOM_MARGINING_DISABLE;
  }

  // Fast boot
  if (SetupData->SocketConfig.MemoryConfig.AttemptFastBoot == FAST_BOOT_ENABLE) {
    Setup->mem.options |= ATTEMPT_FAST_BOOT;
  } else {
    Setup->mem.options &= ~ATTEMPT_FAST_BOOT;
  }

  // Fast cold boot
  if (SetupData->SocketConfig.MemoryConfig.AttemptFastBootCold == FAST_BOOT_COLD_ENABLE) {
    Setup->mem.options |= ATTEMPT_FAST_BOOT_COLD;
  } else {
    Setup->mem.options &= ~ATTEMPT_FAST_BOOT_COLD;
  }

  // Setup option for memory test on fast boot
  if (SetupData->SocketConfig.MemoryConfig.MemTestOnColdFastBoot == MEM_TEST_COLD_FAST_BOOT_ENABLE) {
    Setup->mem.options |= MEMORY_TEST_COLD_FAST_BOOT_EN;
  } else {
    Setup->mem.options &= ~MEMORY_TEST_COLD_FAST_BOOT_EN;
  }

  // RMT on Fast cold boot
  if (SetupData->SocketConfig.MemoryConfig.RmtOnColdFastBoot == RMT_COLD_FAST_BOOT_ENABLE) {
    Setup->mem.options |= RMT_COLD_FAST_BOOT;
  } else {
    Setup->mem.options &= ~RMT_COLD_FAST_BOOT;
  }

  // Scrambling DDRT
  if (SetupData->SocketConfig.MemoryConfig.ScrambleEnDDRT == SCRAMBLE_DDRT_ENABLE) {
    Setup->mem.options |= SCRAMBLE_EN_DDRT;
  } else {
    Setup->mem.options &= ~SCRAMBLE_EN_DDRT;
  }

  // Scrambling DDR4
  if (SetupData->SocketConfig.MemoryConfig.ScrambleEn == SCRAMBLE_ENABLE) {
    Setup->mem.options |= SCRAMBLE_EN;
  } else {
    Setup->mem.options &= ~SCRAMBLE_EN;
  }

  // Allow SBE during Memory training

  if ((SetupData->SocketConfig.MemoryConfig.allowCorrectableError == ALLOW_CORRECTABLE_ERROR_AUTO) && (ALLOW_CORRECTABLE_ERROR_DEFAULT == ALLOW_CORRECTABLE_ERROR_ENABLE)) {
    Setup->mem.optionsExt |= ALLOW_CORRECTABLE_ERROR;
  } else if ( SetupData->SocketConfig.MemoryConfig.allowCorrectableError == ALLOW_CORRECTABLE_ERROR_ENABLE) {
    Setup->mem.optionsExt |= ALLOW_CORRECTABLE_ERROR;
  } else {
    Setup->mem.optionsExt &= ~ALLOW_CORRECTABLE_ERROR;
  }

  if (SetupData->SocketConfig.MemoryConfig.allowCorrectableMemTestError) {
    Setup->mem.optionsExt |= ALLOW_CORRECTABLE_MEM_TEST_ERROR;
  } else {
    Setup->mem.optionsExt &= ~ALLOW_CORRECTABLE_MEM_TEST_ERROR;
  }

  // SPD_CRC_CHECK
  if ((SetupData->SocketConfig.MemoryConfig.spdCrcCheck == SPD_CRC_CHECK_AUTO) && (SPD_CRC_CHECK_DEFAULT == SPD_CRC_CHECK_EN)) {
    Setup->mem.optionsExt |= SPD_CRC_CHECK;
  } else if (SetupData->SocketConfig.MemoryConfig.spdCrcCheck == SPD_CRC_CHECK_EN) {
    Setup->mem.optionsExt |= SPD_CRC_CHECK;
  } else {
    Setup->mem.optionsExt &= ~SPD_CRC_CHECK;
  }

  // Early Write CRC for the DDRIO
  if (SetupData->SocketConfig.MemoryConfig.EarlyWrCRCDdrio == WR_CRC_ENABLE) {
    Setup->mem.optionsExt |= EARLY_WR_CRC_DDRIO;
  } else {
    Setup->mem.optionsExt &= ~EARLY_WR_CRC_DDRIO;
  }

  // Write CRC
  if (SetupData->SocketConfig.MemoryConfig.ADDDCEn == 0) {
    // Late Write CRC for the DDRIO, MC and DRAM
    if (SetupData->SocketConfig.MemoryConfig.WrCRC == WR_CRC_ENABLE) {
      Setup->mem.optionsExt |= WR_CRC;
    } else {
      Setup->mem.optionsExt &= ~WR_CRC;
    }
  } else {
    Setup->mem.optionsExt &= ~WR_CRC;
  }

  // DIMM Isolation
  if (SetupData->SocketConfig.MemoryConfig.dimmIsolation == DIMM_ISOLATION_ENABLE) {
    Setup->mem.optionsExt |= DIMM_ISOLATION_EN;
  } else {
    Setup->mem.optionsExt &= ~DIMM_ISOLATION_EN;
  }

  Setup->mem.writePreamble = SetupData->SocketConfig.MemoryConfig.WritePreamble;
  Setup->mem.readPreamble  = SetupData->SocketConfig.MemoryConfig.ReadPreamble;
  //
  // SPR_todo. Test 2TCK in SPR SOC emulation. HSD:1608865524
  //
  if (UbiosGenerationOrHsleEnabled () && SetupData->SocketConfig.MemoryConfig.ReadPreamble == PREAMBLE_AUTO) {
    Setup->mem.readPreamble = PREAMBLE_1TCLK;
  }

  //
  // Verify that the values being set are within a valid range. They are UINTx so the
  // so the minimum value does not need to be checked.
  //
  if (SetupData->SocketConfig.MemoryConfig.ScrambleSeedLow > MAX_SCRAMBLE_SEED_LOW) {
    SetupData->SocketConfig.MemoryConfig.ScrambleSeedLow = SCRAMBLE_SEED_LOW;   // If out of range set the default
  }
  Setup->mem.scrambleSeedLow = SetupData->SocketConfig.MemoryConfig.ScrambleSeedLow;

  if (SetupData->SocketConfig.MemoryConfig.ScrambleSeedHigh > MAX_SCRAMBLE_SEED_HIGH) {
    SetupData->SocketConfig.MemoryConfig.ScrambleSeedHigh = SCRAMBLE_SEED_HIGH; // If out of range set the default
  }
  Setup->mem.scrambleSeedHigh = SetupData->SocketConfig.MemoryConfig.ScrambleSeedHigh;

  Setup->mem.ADREn = SetupData->SocketConfig.MemoryConfig.ADREn;
  Setup->mem.check_pm_sts = SetupData->SocketConfig.MemoryConfig.check_pm_sts;
  Setup->mem.check_platform_detect = SetupData->SocketConfig.MemoryConfig.check_platform_detect;

  // Custom Refresh Rate
  if (SetupData->SocketConfig.MemoryConfig.CustomRefreshRateEn) {
    Setup->mem.customRefreshRate = SetupData->SocketConfig.MemoryConfig.CustomRefreshRate;
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

  if (IsSiliconWorkaroundEnabled ("S22010462897")) {
    Setup->mem.options &= ~ALLOW2XREF_EN;
  } else {
    // Allow 2x Refresh Rate
    if ((SetupData->SocketConfig.MemoryConfig.refreshMode == A2R_AUTO) && (A2R_DEFAULT == A2R_ENABLE)) {
      Setup->mem.options |= ALLOW2XREF_EN;
    } else if (SetupData->SocketConfig.MemoryConfig.refreshMode == A2R_ENABLE) {
      Setup->mem.options |= ALLOW2XREF_EN;
    } else {
      Setup->mem.options &= ~ALLOW2XREF_EN;
    }
  }

  if (FixedPcdGetBool (PcdBiosDfxKnobEnabled)) {
    // Force ODT ON
    Setup->mem.dfxMemSetup.DfxForceOdtOn = SetupData->SocketConfig.MemoryConfig.DfxForceOdtOn;

    //
    // Skip MA14, MA15, MA16 during BSSA RMT
    //
    if (SetupData->SocketConfig.MemoryConfig.dfxCmdFubMarginging == CMD_FUB_ENABLE) {
      Setup->mem.dfxMemSetup.dfxOptions |= CR_CMD_FUB_MARGIN;
    } else {
      Setup->mem.dfxMemSetup.dfxOptions &= ~CR_CMD_FUB_MARGIN;
    }

    // ECC Support
    if (SetupData->SocketConfig.MemoryConfig.EccSupport == ECC_ENABLE) {
      Setup->mem.dfxMemSetup.dfxOptions |= ECC_CHECK_EN;
    } else {
      Setup->mem.dfxMemSetup.dfxOptions &= ~ECC_CHECK_EN;
    }

    //
    // DDR5 Refresh Mode
    //
    if (SetupData->SocketConfig.MemoryConfig.DfxDdr5RefreshMode == DDR5_REFRESH_MODE_AUTO) {
      Setup->mem.dfxMemSetup.DfxDdr5RefreshMode = FixedPcdGet8 (PcdDdr5RefreshModeDefault);
    } else if (SetupData->SocketConfig.MemoryConfig.DfxDdr5RefreshMode == DDR5_REFRESH_MODE_AB_NORMAL) {
      Setup->mem.dfxMemSetup.DfxDdr5RefreshMode = DDR5_REFRESH_MODE_AB_NORMAL;
    } else if (SetupData->SocketConfig.MemoryConfig.DfxDdr5RefreshMode == DDR5_REFRESH_MODE_AB_FINE) {
      Setup->mem.dfxMemSetup.DfxDdr5RefreshMode = DDR5_REFRESH_MODE_AB_FINE;
    } else {
      Setup->mem.dfxMemSetup.DfxDdr5RefreshMode = DDR5_REFRESH_MODE_SB_FINE;
    }

    //
    // DDR5 SSA RxSampler Margin Parameter Selection
    //
    if (SetupData->SocketConfig.MemoryConfig.DfxDdr5SsaRxSamplerSelection == DDR5_SSA_RXSAMPLER_AUTO) {
      Setup->mem.dfxMemSetup.DfxDdr5SsaRxSamplerSelection = FixedPcdGet8 (PcdDdr5SsaRxSamplerSelectionDefault);
    } else if (SetupData->SocketConfig.MemoryConfig.DfxDdr5SsaRxSamplerSelection == DDR5_SSA_RXSAMPLER_EVEN_ONLY) {
      Setup->mem.dfxMemSetup.DfxDdr5SsaRxSamplerSelection = DDR5_SSA_RXSAMPLER_EVEN_ONLY;
    } else if (SetupData->SocketConfig.MemoryConfig.DfxDdr5SsaRxSamplerSelection == DDR5_SSA_RXSAMPLER_ODD_ONLY) {
      Setup->mem.dfxMemSetup.DfxDdr5SsaRxSamplerSelection = DDR5_SSA_RXSAMPLER_ODD_ONLY;
    } else if (SetupData->SocketConfig.MemoryConfig.DfxDdr5SsaRxSamplerSelection == DDR5_SSA_RXVREF) {
      Setup->mem.dfxMemSetup.DfxDdr5SsaRxSamplerSelection = DDR5_SSA_RXVREF;
    } else {
      Setup->mem.dfxMemSetup.DfxDdr5SsaRxSamplerSelection = DDR5_SSA_RXSAMPLER_EVEN_ODD;
    }

    Setup->mem.dllResetTestLoops = SetupData->SocketConfig.MemoryConfig.dllResetTestLoops;

    // WMM Opportunistic read disable
    if (SetupData->SocketConfig.MemoryConfig.oppReadInWmm == OPP_RD_WMM_ENABLE) {
      Setup->mem.options &= ~DISABLE_WMM_OPP_READ;
    } else {
      Setup->mem.options |= DISABLE_WMM_OPP_READ;
    }

    Setup->mem.normOppIntvl = SetupData->SocketConfig.MemoryConfig.normOppInterval;

    // C/A Parity
    if (SetupData->SocketConfig.MemoryConfig.dfxCAparity == CA_PARITY_ENABLE) {
      Setup->mem.dfxMemSetup.dfxOptions |= CA_PARITY_EN;
    } else {
      Setup->mem.dfxMemSetup.dfxOptions &= ~CA_PARITY_EN;
    }
  } // FixedPcdGetBool (PcdBiosDfxKnobEnabled)

  //
  // DDR4 SMB CLK
  //
  Setup->mem.SpdSmbSpeed = SetupData->SocketConfig.MemoryConfig.smbSpeed;

  //
  // SPD Print
  //
  Setup->mem.SpdPrintEn = SetupData->SocketConfig.MemoryConfig.SpdPrintEn;
  Setup->mem.SpdPrintLength = SetupData->SocketConfig.MemoryConfig.SpdPrintLength;
  if (Setup->mem.SpdPrintLength == AUTO || Setup->mem.SpdPrintLength > MAX_SPD_BYTE_DDR) {
    Setup->mem.SpdPrintLength = MAX_SPD_BYTE_DDR;
  }

  // DDR Reset loop Enable
  if (SetupData->SocketConfig.MemoryConfig.mrcRepeatTest) {
    Setup->mem.options |= DDR_RESET_LOOP;
  }

  //
  // Update flows from setup.
  //

  UpdateMemFlowsFromSetup (SetupData, Setup);

  //
  // Init input parameters here according to setup options
  //
  switch (SetupData->SystemConfig.serialDebugMsgLvl) {
    case 0:
      // Disabled Setup Knob
      Setup->common.serialDebugMsgLvl = 0;
      break;
    case 1:
      // Minimum Setup Knob
      Setup->common.serialDebugMsgLvl = (SDBG_DIMM_INFO | SDBG_ERROR | SDBG_LEGACY | SDBG_CRITICAL_INFO);
      break;
    case 2:
      // Normal Setup Knob
      Setup->common.serialDebugMsgLvl = (SDBG_DIMM_INFO | SDBG_ERROR | SDBG_LEGACY | SDBG_INFO | SDBG_WARN | SDBG_MEM_TRAIN);
      break;
    case 3:
      // Maximum Setup Knob
      Setup->common.serialDebugMsgLvl = (SDBG_DIMM_INFO | SDBG_ERROR | SDBG_LEGACY | SDBG_INFO | SDBG_WARN | SDBG_MEM_TRAIN | SDBG_RCWRITETAG);
      break;
    case 4:
      // Auto Setup Knob
      if (PcdGetBool (PcdDfxAdvDebugJumper)) {
        // Normal Setup Knob
        Setup->common.serialDebugMsgLvl = (SDBG_DIMM_INFO | SDBG_ERROR | SDBG_LEGACY | SDBG_INFO | SDBG_WARN | SDBG_MEM_TRAIN);
      } else {
        // Minimum Setup Knob
        Setup->common.serialDebugMsgLvl = (SDBG_DIMM_INFO | SDBG_ERROR | SDBG_LEGACY | SDBG_CRITICAL_INFO);

        SetPolicyMemFlow (LegacyRankMarginTool, FALSE, Setup);
      }
      break;
    default:
      //
      // Use default debug level at build time configuration
      //
      Setup->common.serialDebugMsgLvl = PcdGet32 (PcdRcDebugBuildTimeDefault);
      break;
  }

  if (PcdGetBool (PcdDfxAdvDebugJumper)) {
    Setup->common.debugJumper = 1;
  }

  Setup->common.serialDebugMsgLvl |= (UINT32)SetupData->SystemConfig.serialDebugTrace;
  Setup->common.serialDebugMsgLvl |= (UINT32)SetupData->SystemConfig.serialDebugMsgLvlTrainResults;

  Setup->mem.Blockgnt2cmd1cyc = SetupData->SocketConfig.MemoryConfig.Blockgnt2cmd1cyc;
  Setup->mem.Disddrtopprd = SetupData->SocketConfig.MemoryConfig.Disddrtopprd;
  // Cmd Setup hold offset
  Setup->mem.cmdSetupPercentOffset = SetupData->SocketConfig.MemoryConfig.cmdSetupPercentOffset;

  //
  // Rank Disable Mask
  //
  for (socket = 0; socket < MAX_SOCKET; socket++) {
    for (Imc = 0; Imc < MAX_IMC; Imc++) {
      for (ch = 0; ch < MAX_MC_CH; ch++) {
        if (SetupData->SocketConfig.MemoryConfig.dfxRankMaskEn && (FixedPcdGetBool (PcdBiosDfxKnobEnabled))) {
          Setup->mem.socket[socket].ddrCh[Imc*MAX_MC_CH + ch].rankmask = SetupData->SocketConfig.MemoryConfig.dfxRankMask[socket*MAX_SETUP_IMC*MAX_SETUP_MC_CH + Imc*MAX_SETUP_MC_CH + ch];
        } else {
          Setup->mem.socket[socket].ddrCh[Imc*MAX_MC_CH + ch].rankmask = MAX_RANK_ENABLE;
        }
      }
    }
  }

  // Page mode
  pagePolicy = SetupData->SocketConfig.MemoryConfig.PagePolicy;

  if (pagePolicy == CLOSED_PAGE_EN) {
    // Closed
    Setup->mem.options |= PAGE_POLICY;
    Setup->mem.options &= ~ADAPTIVE_PAGE_EN;
  } else {
    // Open Adaptive
    Setup->mem.options &= ~PAGE_POLICY;
    Setup->mem.options |= ADAPTIVE_PAGE_EN;
  }

  if (SetupData->SocketConfig.MemoryConfig.LrDimmBacksideVrefEn == LRDIMM_BACKSIDE_VREF_ENABLE) {
    Setup->mem.optionsExt |= LRDIMM_BACKSIDE_VREF_EN;
  } else {
    Setup->mem.optionsExt &= ~LRDIMM_BACKSIDE_VREF_EN;
  }

  //
  // RX DFE Training
  //
  Setup->mem.RxDfeEn = SetupData->SocketConfig.MemoryConfig.RxDfe;

  //
  // Tx Rise Fall Slew Rate Training
  //
  Setup->mem.TxRiseFallSlewRate = SetupData->SocketConfig.MemoryConfig.TxRiseFallSlewRate;

  //
  //Cmi Inialization option
  //
  Setup->mem.CmiInitOption = SetupData->SocketConfig.MemoryConfig.CmiInitOption;

  //
  // Tco Comp training
  //
  if (SetupData->SocketConfig.MemoryConfig.TcoCompTraining == TCO_COMP_ENABLE) {
    Setup->mem.optionsExt |= TCO_COMP_EN;
  } else {
    Setup->mem.optionsExt &= ~TCO_COMP_EN;
  }

  if ((SetupData->SocketConfig.MemoryConfig.DutyCycleTraining == DUTY_CYCLE_AUTO) && (DUTY_CYCLE_DEFAULT == DUTY_CYCLE_ENABLE)) {
    Setup->mem.optionsExt |= DUTY_CYCLE_EN;
  } else if (SetupData->SocketConfig.MemoryConfig.DutyCycleTraining == DUTY_CYCLE_ENABLE) {
    Setup->mem.optionsExt |= DUTY_CYCLE_EN;
  } else {
    Setup->mem.optionsExt &= ~DUTY_CYCLE_EN;
  }

  if ((SetupData->SocketConfig.MemoryConfig.eyeDiagram == EYE_DIAGRAM_AUTO) && (EYE_DIAGRAM_DEFAULT == EYE_DIAGRAM_ENABLE)) {
    Setup->mem.options |= DISPLAY_EYE_EN;
  } else if (SetupData->SocketConfig.MemoryConfig.eyeDiagram == EYE_DIAGRAM_ENABLE) {
    Setup->mem.options |= DISPLAY_EYE_EN;
  } else {
    Setup->mem.options &= ~DISPLAY_EYE_EN;
  }

  //
  // Turnaround Time Optimization
  //
  if (SetupData->SocketConfig.MemoryConfig.turnaroundOpt == TURNAROUND_ENABLE) {
    Setup->mem.optionsExt |= TURNAROUND_OPT_EN;
  } else {
    Setup->mem.optionsExt &= ~TURNAROUND_OPT_EN;
  }

  //
  // Turnaround Time Optimization
  //
  if (SetupData->SocketConfig.MemoryConfig.turnaroundOptDdrt == TURNAROUND_DDRT_ENABLE) {
    Setup->mem.optionsExt |= TURNAROUND_OPT_EN_DDRT;
  } else {
    Setup->mem.optionsExt &= ~TURNAROUND_OPT_EN_DDRT;
  }

  //
  // One Rank Timing Mode Enable
  //
  Setup->mem.oneRankTimingModeEn = SetupData->SocketConfig.MemoryConfig.oneRankTimingMode;

  //
  // PDA
  //
  if (SetupData->SocketConfig.MemoryConfig.pda == PDA_ENABLE) {
    Setup->mem.optionsExt |= PDA_EN;
  } else {
    Setup->mem.optionsExt &= ~PDA_EN;
  }

  if (FixedPcdGetBool (PcdBiosDfxKnobEnabled)) {
    //
    // DRAM Tx Echo Cancellation
    //
    if (SetupData->SocketConfig.MemoryConfig.DfxDramTxEcho == DRAM_TX_ECHO_CANCEL_ENABLE) {
      Setup->mem.optionsExt |= TX_ECHO_CANCEL_EN;
    } else {
      Setup->mem.optionsExt &= ~TX_ECHO_CANCEL_EN;
    }

    //
    // DRAM Legacy Rx Dfe Training
    //
    if (SetupData->SocketConfig.MemoryConfig.DfxDramLegacyRxDfe == DRAM_LEGACY_RXDFE_ENABLE) {
      Setup->mem.dfxMemSetup.DfxLegacyRxDfe = DRAM_LEGACY_RXDFE_ENABLE;
    } else {
      Setup->mem.dfxMemSetup.DfxLegacyRxDfe = DRAM_LEGACY_RXDFE_DISABLE;
    }

  } //if (FixedPcdGetBool (PcdBiosDfxKnobEnabled))

  //
  // thermal throttling options
  //
  Setup->mem.thermalThrottlingOptions &= ~(OLTT_EN | CLTT_EN | MH_OUTPUT_EN | MH_SENSE_EN | CLTT_PECI_EN);

  if (SetupData->SocketConfig.MemoryConfig.thermalthrottlingsupport == CLTT_ENABLE) {
    Setup->mem.thermalThrottlingOptions |= CLTT_EN;
  } else if (SetupData->SocketConfig.MemoryConfig.thermalthrottlingsupport == OLTT_ENABLE) {
    Setup->mem.thermalThrottlingOptions |= OLTT_EN;
  } else if (SetupData->SocketConfig.MemoryConfig.thermalthrottlingsupport == CLTT_PECI_ENABLE) {
    Setup->mem.thermalThrottlingOptions |= CLTT_PECI_EN;
  }

  //
  // memtrip options
  //
  Setup->mem.thermalThrottlingOptions &= ~(OFFPKG_MEM_TO_MEMTRIP_EN | OFFPKG_MEM_TO_THERMTRIP_EN | INPKG_MEM_TO_MEMTRIP_EN | INPKG_MEM_TO_THERMTRIP_EN);

  if (IsCpuAndRevision (CPU_SPRSP, REV_ALL)) {
    //
    // Memtrip reporting options
    //
    if (SetupData->SocketConfig.MemoryConfig.MemTripReporting) {
      if (IsHbmSku ()) {
        Setup->mem.thermalThrottlingOptions |= (INPKG_MEM_TO_MEMTRIP_EN | INPKG_MEM_TO_THERMTRIP_EN);
      } else {
        Setup->mem.thermalThrottlingOptions |= (OFFPKG_MEM_TO_MEMTRIP_EN | OFFPKG_MEM_TO_THERMTRIP_EN | INPKG_MEM_TO_MEMTRIP_EN | INPKG_MEM_TO_THERMTRIP_EN);
      }
    }
  } else {
    //
    // thermal memtrip options
    //
    if (SetupData->SocketConfig.MemoryConfig.OffPkgMemToMemTrip) {
      Setup->mem.thermalThrottlingOptions |= OFFPKG_MEM_TO_MEMTRIP_EN;
    }

    if (SetupData->SocketConfig.MemoryConfig.OffPkgMemToThermTrip) {
      Setup->mem.thermalThrottlingOptions |= OFFPKG_MEM_TO_THERMTRIP_EN;
    }

    //
    // thermal in pkg memtrip options
    //
    if (SetupData->SocketConfig.MemoryConfig.InPkgMemToMemTrip) {
      Setup->mem.thermalThrottlingOptions |= INPKG_MEM_TO_MEMTRIP_EN;
    }

    if (SetupData->SocketConfig.MemoryConfig.InPkgMemToThermTrip) {
      Setup->mem.thermalThrottlingOptions |= INPKG_MEM_TO_THERMTRIP_EN;
    }
  }

  if (Is14nmCpu) {
    //
    // mem hot options
    //
    if (SetupData->SocketConfig.MemoryConfig.memhotSupport == MEMHOT_OUTPUT_ONLY) {
      Setup->mem.thermalThrottlingOptions |= MH_OUTPUT_EN;
    } else if (SetupData->SocketConfig.MemoryConfig.memhotSupport == MEMHOT_INPUT_ONLY) {
      Setup->mem.thermalThrottlingOptions |= MH_SENSE_EN;
    } else if (SetupData->SocketConfig.MemoryConfig.memhotSupport == MEMHOT_INPUT_OUTPUT) {
      Setup->mem.thermalThrottlingOptions |= (MH_OUTPUT_EN + MH_SENSE_EN);
    }

    //
    // MEMHOT Output Only options
    //
    if (SetupData->SocketConfig.MemoryConfig.MemhotOutputOnlyOpt == MEMHOT_OUTPUT_ONLY_DIS) {
      Setup->mem.memhotOutputOnlyOpt = MEMHOT_OUTPUT_ONLY_DIS;
    } else if(SetupData->SocketConfig.MemoryConfig.MemhotOutputOnlyOpt == MEMHOT_OUTPUT_ONLY_HIGH) {
      Setup->mem.memhotOutputOnlyOpt = MEMHOT_OUTPUT_ONLY_HIGH;
    } else if(SetupData->SocketConfig.MemoryConfig.MemhotOutputOnlyOpt == MEMHOT_OUTPUT_ONLY_HIGH_MED) {
      Setup->mem.memhotOutputOnlyOpt = MEMHOT_OUTPUT_ONLY_HIGH_MED;
    } else if(SetupData->SocketConfig.MemoryConfig.MemhotOutputOnlyOpt == MEMHOT_OUTPUT_ONLY_HIGH_MED_LOW) {
      Setup->mem.memhotOutputOnlyOpt = MEMHOT_OUTPUT_ONLY_HIGH_MED_LOW;
    } else {
      Setup->mem.memhotOutputOnlyOpt = MEMHOT_OUTPUT_ONLY_HIGH; //Default = high
    }
  } else {
    // 10nm memhot support
    if (SetupData->SocketConfig.MemoryConfig.memhotSupport == MEMHOT_ENABLE ) {
      if (SetupData->SocketConfig.MemoryConfig.MemHotIn == MEMHOT_IN_ENABLE)  {
        Setup->mem.thermalThrottlingOptions |= MH_INPUT_EN;
      }
      if (SetupData->SocketConfig.MemoryConfig.MemHotOut == MEMHOT_OUT_ENABLE) {
        Setup->mem.thermalThrottlingOptions |= MH_OUTPUT_EN;
        if (SetupData->SocketConfig.MemoryConfig.MemhotOutputOnlyOpt == MEMHOT_OUTPUT_ONLY_DIS) {
          Setup->mem.memhotOutputOnlyOpt = MEMHOT_OUTPUT_ONLY_DIS;
        } else if (SetupData->SocketConfig.MemoryConfig.MemhotOutputOnlyOpt == MEMHOT_OUTPUT_ONLY_HIGH) {
          Setup->mem.memhotOutputOnlyOpt = MEMHOT_OUTPUT_ONLY_HIGH;
        } else if (SetupData->SocketConfig.MemoryConfig.MemhotOutputOnlyOpt == MEMHOT_OUTPUT_ONLY_HIGH_MED) {
          Setup->mem.memhotOutputOnlyOpt = MEMHOT_OUTPUT_ONLY_HIGH_MED;
        } else if (SetupData->SocketConfig.MemoryConfig.MemhotOutputOnlyOpt == MEMHOT_OUTPUT_ONLY_HIGH_MED_LOW) {
          Setup->mem.memhotOutputOnlyOpt = MEMHOT_OUTPUT_ONLY_HIGH_MED_LOW;
        } else {
          Setup->mem.memhotOutputOnlyOpt = MEMHOT_OUTPUT_ONLY_HIGH; //Default = high
        }
      }
    }
  }

  Setup->mem.CkeProgramming = SetupData->SocketConfig.MemoryConfig.CkeProgramming;
  Setup->mem.SrefProgramming = SetupData->SocketConfig.MemoryConfig.SrefProgramming;
  Setup->mem.PkgcSrefEn = SetupData->SocketConfig.MemoryConfig.PkgcSrefEn;
  Setup->mem.CkeIdleTimer = SetupData->SocketConfig.MemoryConfig.CkeIdleTimer;
  Setup->mem.ApdEn = SetupData->SocketConfig.MemoryConfig.ApdEn;
  Setup->mem.PpdEn = SetupData->SocketConfig.MemoryConfig.PpdEn;
  Setup->mem.DdrtCkeEn = SetupData->SocketConfig.MemoryConfig.DdrtCkeEn;
  Setup->mem.OppSrefEn = SetupData->SocketConfig.MemoryConfig.OppSrefEn;
  Setup->mem.DataDllOff = SetupData->SocketConfig.MemoryConfig.DataDllOff;
  Setup->mem.MdllOffEn = SetupData->SocketConfig.MemoryConfig.MdllOffEn;
  Setup->mem.CkMode = SetupData->SocketConfig.MemoryConfig.CkMode;

  if (PcdGetBool (PcdMrcXmpSupport)) {
    Setup->mem.options &= ~(MEM_OVERRIDE_EN);
    if (SetupData->SocketConfig.MemoryConfig.XMPMode != XMP_AUTO) {
      Setup->mem.options |= MEM_OVERRIDE_EN;
      Setup->mem.enforcePOR = ENFORCE_POR_DIS;
    }

    //
    // Verify that the values being set are within a valid range. They are UINTs so the
    // so the minimum value does not need to be checked.
    //
    //Setup->mem.ddrFreqLimit = SetupData.SocketConfig.MemoryConfig.DdrFreqLimit;
    Setup->mem.inputMemTime.vdd = SetupData->SocketConfig.MemoryConfig.Vdd;

    Setup->mem.inputMemTime.nCMDRate = SetupData->SocketConfig.MemoryConfig.commandTiming;

    if (SetupData->SocketConfig.MemoryConfig.tREFI > MAX_REFRESH_RATE) {
      SetupData->SocketConfig.MemoryConfig.tREFI = 0;  // If out of range set the default
    }
    Setup->mem.inputMemTime.tREFI = SetupData->SocketConfig.MemoryConfig.tREFI;

    if (SetupData->SocketConfig.MemoryConfig.tCAS > MAX_CAS_LATENCY) {
      SetupData->SocketConfig.MemoryConfig.tCAS = 0;  // If out of range set the default.
    }
    Setup->mem.inputMemTime.nCL = SetupData->SocketConfig.MemoryConfig.tCAS;

    if (SetupData->SocketConfig.MemoryConfig.tRP > MAX_TRP_LATENCY) {
      SetupData->SocketConfig.MemoryConfig.tRP = 0;   // If out of range set the default.
    }
    Setup->mem.inputMemTime.nRP = SetupData->SocketConfig.MemoryConfig.tRP;

    if (SetupData->SocketConfig.MemoryConfig.tRCD > MAX_TRCD_LATENCY) {
      SetupData->SocketConfig.MemoryConfig.tRCD = 0;  // If out of range set the default
    }
    Setup->mem.inputMemTime.nRCD = SetupData->SocketConfig.MemoryConfig.tRCD;

    if (SetupData->SocketConfig.MemoryConfig.tRAS > MAX_TRAS_LATENCY) {
      SetupData->SocketConfig.MemoryConfig.tRAS = 0;   // If out of range set the default
    }
    Setup->mem.inputMemTime.nRAS = SetupData->SocketConfig.MemoryConfig.tRAS;

    if (SetupData->SocketConfig.MemoryConfig.tWR > MAX_TWR_LATENCY) {
      SetupData->SocketConfig.MemoryConfig.tWR = 0;     // If out of range set the default
    }
    Setup->mem.inputMemTime.nWR = SetupData->SocketConfig.MemoryConfig.tWR;

    if (SetupData->SocketConfig.MemoryConfig.tRFC > MAX_TRFC_LATENCY) {
      SetupData->SocketConfig.MemoryConfig.tRFC = 0;    // If out of range set the default
    }
    Setup->mem.inputMemTime.nRFC = SetupData->SocketConfig.MemoryConfig.tRFC;

    if (SetupData->SocketConfig.MemoryConfig.tRRD_L > MAX_TRRDL_LATENCY) {
      SetupData->SocketConfig.MemoryConfig.tRRD_L = TRRDL_LATENCY_DEFAULT;   // If out of range set the default
    }
    Setup->mem.inputMemTime.nRRD_L = SetupData->SocketConfig.MemoryConfig.tRRD_L;

    if (SetupData->SocketConfig.MemoryConfig.tRRD > MAX_TRRD_LATENCY) {
      SetupData->SocketConfig.MemoryConfig.tRRD = 0;   // If out of range set the default
    }
    Setup->mem.inputMemTime.nRRD = SetupData->SocketConfig.MemoryConfig.tRRD;

    if (SetupData->SocketConfig.MemoryConfig.tRTP > MAX_TRTP_LATENCY) {
      SetupData->SocketConfig.MemoryConfig.tRTP = 0;   // If out of range set the default
    }
    Setup->mem.inputMemTime.nRTP = SetupData->SocketConfig.MemoryConfig.tRTP;

    if (SetupData->SocketConfig.MemoryConfig.tWTR > MAX_TWTR_LATENCY) {
      SetupData->SocketConfig.MemoryConfig.tWTR = 0;    // If out of range set the default
    }
    Setup->mem.inputMemTime.nWTR = SetupData->SocketConfig.MemoryConfig.tWTR;

    if (SetupData->SocketConfig.MemoryConfig.tFAW > MAX_TFAW_LATENCY) {
      SetupData->SocketConfig.MemoryConfig.tFAW = 0;   // If out of range set the default
    }
    Setup->mem.inputMemTime.nFAW = SetupData->SocketConfig.MemoryConfig.tFAW;

    if (SetupData->SocketConfig.MemoryConfig.tRC > MAX_TRC_LATENCY) {
      SetupData->SocketConfig.MemoryConfig.tRC = 0;     // If out of range set the default
    }
    Setup->mem.inputMemTime.nRC = SetupData->SocketConfig.MemoryConfig.tRC;

    if (SetupData->SocketConfig.MemoryConfig.tCWL > MAX_TCWL_LATENCY) {
      SetupData->SocketConfig.MemoryConfig.tCWL = 0;    // If out of range set the default
    }
    Setup->mem.inputMemTime.nCWL = SetupData->SocketConfig.MemoryConfig.tCWL;
  }


  if (!(IsCpuAndRevision (CPU_SNR, REV_ALL) || IsCpuAndRevision (CPU_TNR, REV_ALL))) {
    Setup->mem.volMemMode = SetupData->SocketConfig.MemoryConfig.volMemMode;
    Setup->mem.CacheMemType = SetupData->SocketConfig.MemoryConfig.CacheMemType;
    Setup->mem.PmemCaching = SetupData->SocketConfig.MemoryConfig.PmemCaching;
    Setup->mem.EadrSupport = SetupData->SocketConfig.MemoryConfig.EadrSupport;
    Setup->mem.EadrCacheFlushMode = SetupData->SocketConfig.MemoryConfig.EadrCacheFlushMode;
  }

  if (!IsCpuAndRevision (CPU_CPX, REV_ALL) && !IsCpuAndRevision (CPU_ICXSP, REV_ALL) && !IsCpuAndRevision (CPU_SPRSP, REV_ALL) &&
      !IsCpuAndRevision (CPU_ICXD, REV_ALL) && !IsCpuAndRevision (CPU_SNR, REV_ALL) && !IsCpuAndRevision (CPU_TNR, REV_ALL)) {
    Setup->mem.DdrCacheSize = SetupData->SocketConfig.MemoryConfig.DdrCacheSize;
  } else {
    Setup->mem.DdrCacheSize = DDR_ENCODED_CACHE_SIZE_DEFAULT;
  }

  //
  // Update fADR setup policy.
  //
  Setup->mem.FadrSupport = SetupData->SocketConfig.MemoryConfig.FadrSupport;

  // Interleave and NUMA
  if (SetupData->SocketConfig.CommonRcConfig.NumaEn == NUMA_EN) {
    Setup->mem.options |= NUMA_AWARE;
  } else {
    Setup->mem.options &= ~NUMA_AWARE;
    // Force Volatile memory mode to 1LM if system configured as UMA
    Setup->mem.volMemMode = VOL_MEM_MODE_1LM;
  }

  if (FixedPcdGetBool (PcdBiosDfxKnobEnabled)) {
    Setup->mem.dfxMemSetup.DfxBankGroupInterleavingMode = SetupData->SocketConfig.MemoryConfig.DfxBankGroupInterleavingMode;

    Setup->mem.dfxMemSetup.dfxPerMemMode = SetupData->SocketConfig.MemoryConfig.dfxPerMemMode;

    Setup->mem.dfxMemSetup.DfxDdrtChInterleaveGran = SetupData->SocketConfig.MemoryConfig.DfxDdrtChInterleaveGran;

    Setup->mem.dfxMemSetup.dfxDimmManagement = SetupData->SocketConfig.MemoryConfig.dfxDimmManagement;

    Setup->mem.dfxMemSetup.DfxTmeCmiCplFabCreditWa = SetupData->SocketConfig.MemoryConfig.DfxTmeCmiCplFabCreditWa;

    Setup->mem.dfxMemSetup.DfxMcDataParityCheck = SetupData->SocketConfig.MemoryConfig.DfxMcDataParityCheck;

    Setup->mem.dfxMemSetup.dfxLoadDimmMgmtDriver = SetupData->SocketConfig.MemoryConfig.dfxLoadDimmMgmtDriver;

    Setup->mem.dfxMemSetup.dfxPartitionDDRTDimm = SetupData->SocketConfig.MemoryConfig.dfxPartitionDDRTDimm;

    Setup->mem.dfxMemSetup.DfxDdrtSchedulerDebug = SetupData->SocketConfig.MemoryConfig.DfxDdrtSchedulerDebug;

    for(i = 0; i < (MAX_SOCKET * MAX_IMC); i++) {
      Setup->mem.dfxMemSetup.dfxPartitionRatio[i] = SetupData->SocketConfig.MemoryConfig.dfxPartitionRatio[i];
    }
    Setup->mem.dfxMemSetup.dfxCfgMask2LM = SetupData->SocketConfig.MemoryConfig.dfxCfgMask2LM;

    Setup->mem.dfxMemSetup.DfxDdrEarlyCmpEn = SetupData->SocketConfig.MemoryConfig.DfxDdrEarlyCmpEn;
    Setup->mem.dfxMemSetup.DfxHbmEarlyCmpEn = SetupData->SocketConfig.MemoryConfig.DfxHbmEarlyCmpEn;

    //
    // Update DFX setup policy of DDR XOR decoder for cluster, channel, rank, SID and bank for SPR CPU.
    //
    if (IsCpuAndRevision (CPU_SPRSP, REV_ALL)) {
      Setup->mem.dfxMemSetup.DfxClusterXorEn = SetupData->SocketConfig.MemoryConfig.DfxClusterXorEn;
      Setup->mem.dfxMemSetup.DfxChannelXorEn = SetupData->SocketConfig.MemoryConfig.DfxChannelXorEn;
      Setup->mem.dfxMemSetup.DfxPseudoChannelXorEn = SetupData->SocketConfig.MemoryConfig.DfxPseudoChannelXorEn;
      Setup->mem.dfxMemSetup.DfxDimmXorEn = SetupData->SocketConfig.MemoryConfig.DfxDimmXorEn;
      Setup->mem.dfxMemSetup.DfxRankXorEn = SetupData->SocketConfig.MemoryConfig.DfxRankXorEn;
      Setup->mem.dfxMemSetup.DfxSidXorEn = SetupData->SocketConfig.MemoryConfig.DfxSidXorEn;
      Setup->mem.dfxMemSetup.DfxBankXorEn = SetupData->SocketConfig.MemoryConfig.DfxBankXorEn;
    }
    if (IsCpuAndRevision (CPU_ICXSP, REV_ALL)) {
      Setup->mem.dfxMemSetup.DfxBankXorEn = SetupData->SocketConfig.MemoryConfig.DfxBankXorEn;
    }
    //
    // Below DFx Setup knobs have been created for FC Emulation usage only
    //
    Setup->mem.dfxMemSetup.DfxTimingOverrideCtrl = SetupData->SocketConfig.MemoryConfig.DfxTimingOverrideCtrl;
    Setup->mem.dfxMemSetup.DfxInputRt = SetupData->SocketConfig.MemoryConfig.DfxInputRt;
    Setup->mem.dfxMemSetup.DfxInputIoLatency = SetupData->SocketConfig.MemoryConfig.DfxInputIoLatency;
    Setup->mem.dfxMemSetup.DfxInputIoLatencyIoComp = SetupData->SocketConfig.MemoryConfig.DfxInputIoLatencyIoComp;
    Setup->mem.dfxMemSetup.DfxInputTxDqDelay = SetupData->SocketConfig.MemoryConfig.DfxInputTxDqDelay;
    Setup->mem.dfxMemSetup.DfxInputTxDqsDelay = SetupData->SocketConfig.MemoryConfig.DfxInputTxDqsDelay;
    Setup->mem.dfxMemSetup.DfxInputTxDqBitDelay = SetupData->SocketConfig.MemoryConfig.DfxInputTxDqBitDelay;
    Setup->mem.dfxMemSetup.DfxInputDdrtTxDqsDelay = SetupData->SocketConfig.MemoryConfig.DfxInputDdrtTxDqsDelay;
    Setup->mem.dfxMemSetup.DfxInputDdrtTxDqBitDelay = SetupData->SocketConfig.MemoryConfig.DfxInputDdrtTxDqBitDelay;
    Setup->mem.dfxMemSetup.DfxInputRxDqDelay = SetupData->SocketConfig.MemoryConfig.DfxInputRxDqDelay;
    Setup->mem.dfxMemSetup.DfxInputRxDqsDelay = SetupData->SocketConfig.MemoryConfig.DfxInputRxDqsDelay;
    Setup->mem.dfxMemSetup.DfxInputRxDqBitDelay = SetupData->SocketConfig.MemoryConfig.DfxInputRxDqBitDelay;
    Setup->mem.dfxMemSetup.DfxInputGnt2Erid = SetupData->SocketConfig.MemoryConfig.DfxInputGnt2Erid;
    Setup->mem.dfxMemSetup.DfxCwlAdjPos = SetupData->SocketConfig.MemoryConfig.DfxCwlAdjPos;
    Setup->mem.dfxMemSetup.DfxCwlAdjNeg = SetupData->SocketConfig.MemoryConfig.DfxCwlAdjNeg;
    Setup->mem.dfxMemSetup.DfxRcvEnDelay = SetupData->SocketConfig.MemoryConfig.DfxRcvEnDelay;
    Setup->mem.dfxMemSetup.DfxDdrtRcvEnDelay = SetupData->SocketConfig.MemoryConfig.DfxDdrtRcvEnDelay;
    Setup->mem.dfxMemSetup.DfxDdrtRt = SetupData->SocketConfig.MemoryConfig.DfxDdrtRt;
    Setup->mem.dfxMemSetup.DfxRxCtleExtendedRange = SetupData->SocketConfig.MemoryConfig.DfxRxCtleExtendedRangeEn;
    Setup->mem.dfxMemSetup.DfxInputCmdDelay= SetupData->SocketConfig.MemoryConfig.DfxInputCmdDelay;

    if ((SetupData->SocketConfig.MemoryConfig.DfxUbiosSmbusTrace == UBIOS_SMBUS_TRACE_AUTO) && (UBIOS_SMBUS_TRACE_DEFAULT == UBIOS_SMBUS_TRACE_ENABLE)) {
      Setup->mem.dfxMemSetup.DfxUbiosSmbusTraceEnable = TRUE;
    } else if (SetupData->SocketConfig.MemoryConfig.DfxUbiosSmbusTrace == UBIOS_SMBUS_TRACE_ENABLE) {
      Setup->mem.dfxMemSetup.DfxUbiosSmbusTraceEnable = TRUE;
    } else {
      Setup->mem.dfxMemSetup.DfxUbiosSmbusTraceEnable = FALSE;
    }

    if ((SetupData->SocketConfig.MemoryConfig.DfxSmbusI3cMode == SMBUS_I3C_MODE_AUTO) && (SMBUS_I3C_MODE_DEFAULT == SMBUS_I3C_MODE_ENABLE)) {
      Setup->mem.dfxMemSetup.DfxSmbusI3cModeEnable = TRUE;
    } else if (SetupData->SocketConfig.MemoryConfig.DfxSmbusI3cMode == SMBUS_I3C_MODE_ENABLE) {
      Setup->mem.dfxMemSetup.DfxSmbusI3cModeEnable = TRUE;
    } else {
      Setup->mem.dfxMemSetup.DfxSmbusI3cModeEnable = FALSE;
    }
  } //if (FixedPcdGetBool (PcdBiosDfxKnobEnabled))
  Setup->mem.TrainingCompOptions = SetupData->SocketConfig.MemoryConfig.TrainingCompOptions;

  Setup->mem.PeriodicRcomp = SetupData->SocketConfig.MemoryConfig.PeriodicRcomp;
  Setup->mem.PeriodicRcompInterval = SetupData->SocketConfig.MemoryConfig.PeriodicRcompInterval;

  Setup->mem.memInterleaveGran1LM = SetupData->SocketConfig.MemoryConfig.memInterleaveGran1LM;

  //
  // Commenting this out for now in order to reconcile SI policy default and platform override abd DfxKnob override in platform
  // Current code ends up setting this to 3way regardless of the platform.  How chInter is set in policy code needs to be revisited
  // Setup->mem.chInter = MAX_MC_CH;
  //
  Setup->mem.imcInter = MAX_IMC;

  if (FixedPcdGetBool (PcdBiosDfxKnobEnabled)) {
    if (SetupData->SocketConfig.MemoryConfig.DfxDimmInterleaving == DIMM_INTER_DISABLE) {
      Setup->mem.imcInter = IMC_1WAY;
      Setup->mem.chInter = CH_1WAY;
    } else {
      if (SetupData->SocketConfig.MemoryConfig.DfxMod3Support == MOD3_SUPPORT_ENABLE) {
        Setup->mem.chInter = CH_3WAY;
      } else {
        Setup->mem.chInter = MAX_MC_CH;
      }
      Setup->mem.imcInter = MAX_IMC;
    }
  } //if (FixedPcdGetBool (PcdBiosDfxKnobEnabled))

  //
  // Initial Stagger Host Refresh Per Channel/Rank and additional delay options
  //
  Setup->mem.TrefiPerChannel = SetupData->SocketConfig.MemoryConfig.TrefiPerChannel;
  Setup->mem.TrefiNumofRank = SetupData->SocketConfig.MemoryConfig.TrefiNumofRank;
  Setup->mem.TrefiDelay = SetupData->SocketConfig.MemoryConfig.TrefiDelay;

  if (FixedPcdGetBool (PcdBiosDfxKnobEnabled)) {
    Setup->mem.dfxMemSetup.dfxRankInter = SetupData->SocketConfig.MemoryConfig.DfxRankInterleaving;

    Setup->mem.dfxMemSetup.dfxOptions &= ~LOCK_NGN_CSR_DIS;
  } //if (FixedPcdGetBool (PcdBiosDfxKnobEnabled))
  Setup->mem.optionsNgn &= ~NGN_DEBUG_LOCK;
  if ((SetupData->SocketConfig.MemoryConfig.NgnCmdTime == NGN_CMD_TIME_AUTO) && (NGN_CMD_TIME_DEFAULT == NGN_CMD_TIME_AUTO)) {
    Setup->mem.optionsNgn &= ~NGN_CMD_TIME;
  } else if (SetupData->SocketConfig.MemoryConfig.NgnCmdTime == NGN_CMD_2N) {
    Setup->mem.optionsNgn |= NGN_CMD_TIME;
  } else {
    Setup->mem.optionsNgn &= ~NGN_CMD_TIME;
  }

  if (FixedPcdGetBool (PcdBiosDfxKnobEnabled)) {
    if ((SetupData->SocketConfig.MemoryConfig.NgnEccCorr == NGN_ECC_CORR_AUTO) && (NGN_ECC_CORR_DEFAULT == NGN_ECC_CORR_ENABLE)) {
      Setup->mem.dfxMemSetup.dfxOptions |= NGN_ECC_CORR;
    } else if (SetupData->SocketConfig.MemoryConfig.NgnEccCorr == NGN_ECC_CORR_ENABLE) {
      Setup->mem.dfxMemSetup.dfxOptions |= NGN_ECC_CORR;
    } else {
      Setup->mem.dfxMemSetup.dfxOptions &= ~NGN_ECC_CORR;
    }

    if ((SetupData->SocketConfig.MemoryConfig.NgnEccWrChk == NGN_ECC_WR_CHK_AUTO) && (NGN_ECC_WR_CHK_DEFAULT == NGN_ECC_WR_CHK_ENABLE)) {
      Setup->mem.dfxMemSetup.dfxOptions |= NGN_ECC_WR_CHK;
    } else if (SetupData->SocketConfig.MemoryConfig.NgnEccWrChk == NGN_ECC_WR_CHK_ENABLE) {
      Setup->mem.dfxMemSetup.dfxOptions |= NGN_ECC_WR_CHK;
    } else {
      Setup->mem.dfxMemSetup.dfxOptions &= ~NGN_ECC_WR_CHK;
    }

    if ((SetupData->SocketConfig.MemoryConfig.NgnEccExitCorr == NGN_ECC_EXIT_CORR_AUTO) && (NGN_ECC_EXIT_CORR_DEFAULT == NGN_ECC_EXIT_CORR_ENABLE)) {
      Setup->mem.dfxMemSetup.dfxOptions |= NGN_ECC_EXIT_CORR;
    } else if (SetupData->SocketConfig.MemoryConfig.NgnEccExitCorr == NGN_ECC_EXIT_CORR_ENABLE) {
      Setup->mem.dfxMemSetup.dfxOptions |= NGN_ECC_EXIT_CORR;
    } else {
      Setup->mem.dfxMemSetup.dfxOptions &= ~NGN_ECC_EXIT_CORR;
    }
  } //if (FixedPcdGetBool (PcdBiosDfxKnobEnabled))

  if ((SetupData->SocketConfig.MemoryConfig.NgnEccRdChk == NGN_ECC_RD_CHK_AUTO) && (NGN_ECC_RD_CHK_DEFAULT == NGN_ECC_RD_CHK_ENABLE)) {
    Setup->mem.optionsNgn |= NGN_ECC_RD_CHK;
  } else if (SetupData->SocketConfig.MemoryConfig.NgnEccRdChk == NGN_ECC_RD_CHK_ENABLE) {
    Setup->mem.optionsNgn |= NGN_ECC_RD_CHK;
  } else {
    Setup->mem.optionsNgn &= ~NGN_ECC_RD_CHK;
  }

  if ((SetupData->SocketConfig.MemoryConfig.NgnArsPublish == NGN_ARS_PUBLISH_AUTO) && (NGN_ARS_PUBLISH_DEFAULT == NGN_ARS_PUBLISH_EN)) {
    Setup->mem.optionsNgn |= NGN_ARS_PUBLISH;
  } else if (SetupData->SocketConfig.MemoryConfig.NgnArsPublish == NGN_ARS_PUBLISH_EN) {
    Setup->mem.optionsNgn |= NGN_ARS_PUBLISH;
  } else {
    Setup->mem.optionsNgn &= ~NGN_ARS_PUBLISH;
  }
  if (FixedPcdGetBool (PcdBiosDfxKnobEnabled)) {
    if (SetupData->SocketConfig.MemoryConfig.ShortStroke2GB == SHORT_STROKE_2GB_INT) {
      Setup->mem.dfxMemSetup.dfxOptions |= SHORT_STROKE_INT;
    } else if (SetupData->SocketConfig.MemoryConfig.ShortStroke2GB == SHORT_STROKE_2GB_NON_INT) {
      Setup->mem.dfxMemSetup.dfxOptions |= SHORT_STROKE_NO_INT;
    } else {
      Setup->mem.dfxMemSetup.dfxOptions &= ~(SHORT_STROKE_INT | SHORT_STROKE_NO_INT);
    }

  } //if (FixedPcdGetBool (PcdBiosDfxKnobEnabled))

  Setup->mem.NgnAveragePower = SetupData->SocketConfig.MemoryConfig.NgnAveragePower;

  Setup->mem.setSecureEraseAllDIMMs = SetupData->SocketConfig.MemoryConfig.setSecureEraseAllDIMMs;

  Setup->mem.EnableTwoWayNmCache = SetupData->SocketConfig.MemoryConfig.EnableTwoWayNmCache;

  if (SetupData->SocketConfig.MemoryConfig.NonPreferredWayMask <= NON_PREFERRED_WAY_MASK_MAX) {
    Setup->mem.NonPreferredWayMask = SetupData->SocketConfig.MemoryConfig.NonPreferredWayMask;
  } else {
    DEBUG ((EFI_D_ERROR, "Illegal non-preferred way mask 0x%03x for biased 2-way near memory cache, set to default\n",
      SetupData->SocketConfig.MemoryConfig.NonPreferredWayMask));
    Setup->mem.NonPreferredWayMask = NON_PREFERRED_WAY_MASK_DEFAULT;
  }

  Setup->mem.PreferredReadFirst = SetupData->SocketConfig.MemoryConfig.PreferredReadFirst;

  //
  // Boot-time fast zero memory setup policy update
  //
  Setup->mem.FastZeroMemSupport = SetupData->SocketConfig.MemoryConfig.FastZeroMemSupport;

  //
  // FIS 2.x
  //
  Setup->mem.DcpmmAveragePowerLimit = SetupData->SocketConfig.MemoryConfig.DcpmmAveragePowerLimit;
  Setup->mem.DcpmmAveragePowerTimeConstant = SetupData->SocketConfig.MemoryConfig.DcpmmAveragePowerTimeConstant; // Turbo Mode Support
  Setup->mem.DcpmmMbbAveragePowerTimeConstant = SetupData->SocketConfig.MemoryConfig.DcpmmMbbAveragePowerTimeConstant; // Memory Bandwidth Boost Feature support
  Setup->mem.DcpmmMbbFeature = SetupData->SocketConfig.MemoryConfig.DcpmmMbbFeature;
  if (SetupData->SocketConfig.MemoryConfig.DcpmmMbbFeature == TURBO_MODE_STATE_ENABLED) {
    Setup->mem.DcpmmMbbMaxPowerLimit = SetupData->SocketConfig.MemoryConfig.DcpmmMbbMaxPowerLimit;
  }

  socket = 0;
  ch = 0;
  for(dimm = 0; dimm < MAX_DCPMMS; dimm++) {
    if (ch == MAX_CH) {
      ch = 0;
      socket += 1;
    }

    Setup->mem.setSecureEraseSktCh[socket][ch] = SetupData->SocketConfig.MemoryConfig.setSecureEraseSktCh[dimm];

    ch++;
  }//dimm

  //
  //  Get BIOS Setup option for RMT on Advanced Memory Training
  //
  Setup->mem.RmtOnAdvancedTraining = SetupData->SocketConfig.MemoryConfig.RmtOnAdvancedTraining;

  //
  // Enabling the BIOS SSA loader
  //
  Setup->mem.enableBiosSsaLoader = BIOS_SSA_LOADER_DISABLE;

  //
  // Enabling the BIOS SSA Stitched Mode (RMT)
  //
  if (SetupData->SocketConfig.MemoryConfig.EnableBiosSsaRMT == BIOS_SSA_RMT_ENABLE) {
    Setup->mem.enableBiosSsaRMT = BIOS_SSA_RMT_ENABLE;
  } else {
    Setup->mem.enableBiosSsaRMT = BIOS_SSA_RMT_DISABLE;
  }

  //
  // Enabling the BIOS SSA Stitched Mode (RMT) on FCB
  //
  if (SetupData->SocketConfig.MemoryConfig.EnableBiosSsaRMTonFCB == BIOS_SSA_RMT_FCB_ENABLE) {
    Setup->mem.enableBiosSsaRMTonFCB = BIOS_SSA_RMT_FCB_ENABLE;
  } else {
    Setup->mem.enableBiosSsaRMTonFCB = BIOS_SSA_RMT_FCB_DISABLE;
  }

  //
  // Enabling BiosSsaPerBitMargining
  //
  if (SetupData->SocketConfig.MemoryConfig.BiosSsaPerBitMargining == BIOS_SSA_PER_BIT_MARGINING_ENABLE) {
    Setup->mem.biosSsaPerBitMargining = BIOS_SSA_PER_BIT_MARGINING_ENABLE;
  } else {
    Setup->mem.biosSsaPerBitMargining = BIOS_SSA_PER_BIT_MARGINING_DISABLE;
  }

  //
  // Enabling BiosSsaDisplayTables
  //
  if (SetupData->SocketConfig.MemoryConfig.BiosSsaDisplayTables == BIOS_SSA_DISPLAY_TABLE_ENABLE) {
    Setup->mem.biosSsaDisplayTables = BIOS_SSA_DISPLAY_TABLE_ENABLE;
  } else {
    Setup->mem.biosSsaDisplayTables = BIOS_SSA_DISPLAY_TABLE_DISABLE;
  }

  //
  // Enabling BiosSsaPerDisplayPlots
  //
  if (SetupData->SocketConfig.MemoryConfig.BiosSsaPerDisplayPlots == BIOS_SSA_DISPLAY_PLOTS_ENABLE) {
    Setup->mem.biosSsaPerDisplayPlots = BIOS_SSA_DISPLAY_PLOTS_ENABLE;
  } else {
    Setup->mem.biosSsaPerDisplayPlots = BIOS_SSA_DISPLAY_PLOTS_DISABLE;
  }

  //
  //Exponential loop count for single rank test
  //
  Setup->mem.biosSsaLoopCount = SetupData->SocketConfig.MemoryConfig.BiosSsaLoopCount;

  //
  // Enabling BiosSsaBacksideMargining;
  //
  if (SetupData->SocketConfig.MemoryConfig.BiosSsaBacksideMargining == BIOS_SSA_BACKSIDE_MARGINING_ENABLE) {
    Setup->mem.biosSsaBacksideMargining = BIOS_SSA_BACKSIDE_MARGINING_ENABLE;
  } else {
    Setup->mem.biosSsaBacksideMargining = BIOS_SSA_BACKSIDE_MARGINING_DISABLE;
  }

  //
  // Enabling BiosSsaEarlyReadIdMargining
  //
  if (SetupData->SocketConfig.MemoryConfig.BiosSsaEarlyReadIdMargining == BIOS_SSA_EARLY_READ_ID_ENABLE) {
    Setup->mem.biosSsaEarlyReadIdMargining = BIOS_SSA_EARLY_READ_ID_ENABLE;
  } else {
    Setup->mem.biosSsaEarlyReadIdMargining = BIOS_SSA_EARLY_READ_ID_DISABLE;
  }

  //
  // Enabling BiosSsaStepSizeOverride;
  //
  if (SetupData->SocketConfig.MemoryConfig.BiosSsaStepSizeOverride == BIOS_SSA_STEP_SIZE_OVERRIDE_ENABLE) {
    Setup->mem.biosSsaStepSizeOverride = BIOS_SSA_STEP_SIZE_OVERRIDE_ENABLE;
  } else {
    Setup->mem.biosSsaStepSizeOverride = BIOS_SSA_STEP_SIZE_OVERRIDE_DISABLE;
  }

  //
  //  Step size of RxDqs
  //
  Setup->mem.biosSsaRxDqs = SetupData->SocketConfig.MemoryConfig.BiosSsaRxDqs;

  //
  //  Step size of RxVref
  //
  Setup->mem.biosSsaRxVref = SetupData->SocketConfig.MemoryConfig.BiosSsaRxVref;
  //
  //  Step size of TxDq
  //
  Setup->mem.biosSsaTxDq = SetupData->SocketConfig.MemoryConfig.BiosSsaTxDq;
  //
  //  Step size of TxVref
  //
  Setup->mem.biosSsaTxVref = SetupData->SocketConfig.MemoryConfig.BiosSsaTxVref;
  //
  //  Step size of CmdAll
  //
  Setup->mem.biosSsaCmdAll = SetupData->SocketConfig.MemoryConfig.BiosSsaCmdAll;
  //
  //  Step size of CmdVref
  //
  Setup->mem.biosSsaCmdVref = SetupData->SocketConfig.MemoryConfig.BiosSsaCmdVref;
  //
  //  Step size of CtlVref
  //
  Setup->mem.biosSsaCtlAll = SetupData->SocketConfig.MemoryConfig.BiosSsaCtlAll;
  //
  //  Step size of EridDelay
  //
  Setup->mem.biosSsaEridDelay = SetupData->SocketConfig.MemoryConfig.BiosSsaEridDelay;
  //
  //  Step size of EridVref
  //
  Setup->mem.biosSsaEridVref = SetupData->SocketConfig.MemoryConfig.BiosSsaEridVref;

  //
  // Enabling BiosSsaDebugMessages
  //
  if (SetupData->SocketConfig.MemoryConfig.BiosSsaDebugMessages == BIOS_SSA_RMT_DEBUG_MSGS_ENABLE) {
    Setup->mem.biosSsaDebugMessages = BIOS_SSA_RMT_DEBUG_MSGS_ENABLE;
  } else {
    Setup->mem.biosSsaDebugMessages = BIOS_SSA_RMT_DEBUG_MSGS_DISABLE;
  }

  // MemorySetup.hfr finishes

  if (pprAddrVariablePtr != NULL) {
    for (j = 0; j < MAX_PPR_ADDR_ENTRIES; j++) {
      Setup->mem.pprAddrSetup[j].pprAddrStatus    = pprAddrVariable.pprAddrSetup[j].pprAddrStatus;
      Setup->mem.pprAddrSetup[j].socket           = pprAddrVariable.pprAddrSetup[j].socket;
      Setup->mem.pprAddrSetup[j].mc               = pprAddrVariable.pprAddrSetup[j].mc;
      Setup->mem.pprAddrSetup[j].ch               = pprAddrVariable.pprAddrSetup[j].ch;
      Setup->mem.pprAddrSetup[j].pprAddr.dimm     = pprAddrVariable.pprAddrSetup[j].dimm;
      Setup->mem.pprAddrSetup[j].pprAddr.rank     = pprAddrVariable.pprAddrSetup[j].rank;
      Setup->mem.pprAddrSetup[j].pprAddr.subRank  = pprAddrVariable.pprAddrSetup[j].subRank;
      Setup->mem.pprAddrSetup[j].pprAddr.bank     = pprAddrVariable.pprAddrSetup[j].bank;
      Setup->mem.pprAddrSetup[j].pprAddr.row      = pprAddrVariable.pprAddrSetup[j].row;
      Setup->mem.pprAddrSetup[j].pprAddr.nibbleMask = pprAddrVariable.pprAddrSetup[j].nibbleMask;
    }
  }

#ifndef CRB_FLAG
  if (SetupData->SocketConfig.MemoryConfig.staggerref) {
  }
#endif

  GetPlatformTypeLocal (&PlatformType);

  // Forcing ECC off for TypeHedtxxx

  if (PlatformType == TypeHedtCRB || (SetupData->SocketConfig.MemoryConfig.EccEnable == ECC_DISABLE) && (FixedPcdGetBool (PcdBiosDfxKnobEnabled))) {
    Setup->mem.options |= DISABLE_ECC_SUPPORT;
  }

  if (FixedPcdGetBool (PcdBiosDfxKnobEnabled)) {
    // Forcing C/A Parity off for TypeHedtxxx
    if (PlatformType == TypeHedtCRB) {
      Setup->mem.dfxMemSetup.dfxOptions &= ~CA_PARITY_EN;
    }
  } //if (FixedPcdGetBool (PcdBiosDfxKnobEnabled))
  Setup->mem.DramRaplEnable = SetupData->SocketConfig.MemoryConfig.DramRaplEnable;
  Setup->mem.dramraplbwlimittf = SetupData->SocketConfig.MemoryConfig.BwLimitTfOvrd;
  Setup->mem.DramRaplExtendedRange = SetupData->SocketConfig.MemoryConfig.DramRaplExtendedRange;
  Setup->mem.CmsEnableDramPm = SetupData->SocketConfig.MemoryConfig.CmsEnableDramPm;

  if (SetupData->SocketConfig.MemoryConfig.mdllSden == MDLL_SDEN_EN) {
    Setup->mem.options |= MDLL_SHUT_DOWN_EN;
  } else if((SetupData->SocketConfig.MemoryConfig.mdllSden == MDLL_SDEN_AUTO) && (MDLL_SDEN_DEFAULT == MDLL_SDEN_EN)) {
    Setup->mem.options |= MDLL_SHUT_DOWN_EN;
  } else {
    Setup->mem.options &= ~MDLL_SHUT_DOWN_EN;
  }

  Setup->mem.spareErrTh = SetupData->SocketConfig.MemoryConfig.spareErrTh;
  Setup->mem.spareRanks = SetupData->SocketConfig.MemoryConfig.multiSparingRanks;
  Setup->mem.patrolScrubDuration = SetupData->SocketConfig.MemoryConfig.PatrolScrubDuration;
  Setup->mem.patrolScrubAddrMode = SetupData->SocketConfig.MemoryConfig.PatrolScrubAddrMode;

  Setup->mem.DieSparing = SetupData->SocketConfig.MemoryConfig.DieSparing;


  Setup->mem.NsddcEn = SetupData->SocketConfig.MemoryConfig.NsddcEn;
  Setup->mem.EsddcEn = SetupData->SocketConfig.MemoryConfig.EsddcEn;

  if (IsCpuAndRevisionOrLater (CPU_ICXSP, REV_D0)  && (Setup->mem.NsddcEn)) {
    Setup->mem.EsddcEn = 0;
  }

  //
  // Leaky bucket configuration options
  //
  Setup->mem.leakyBktTimeWindow = SetupData->SocketConfig.MemoryConfig.leakyBktTimeWindow;
  Setup->mem.leakyBktHi = SetupData->SocketConfig.MemoryConfig.leakyBktHi;
  Setup->mem.leakyBktLo = SetupData->SocketConfig.MemoryConfig.leakyBktLo;
  Setup->mem.leakyBktHour = SetupData->SocketConfig.MemoryConfig.leakyBktHour;
  Setup->mem.leakyBktMinute = SetupData->SocketConfig.MemoryConfig.leakyBktMinute;


  {
    ADDRESS_RANGE_MIRROR_VARIABLE_DATA AddressBasedMirrorData;

    DEBUG ((DEBUG_INFO, "Looking for MirrorRequest\n"));

    VariableSize = sizeof(ADDRESS_RANGE_MIRROR_VARIABLE_DATA);

    Status = PeiVariable->GetVariable (
                            PeiVariable,
                            ADDRESS_RANGE_MIRROR_VARIABLE_REQUEST,
                            &gAddressBasedMirrorGuid,
                            NULL,
                            &VariableSize,
                            &AddressBasedMirrorData
                            );
    if (Status == EFI_SUCCESS) {

      DEBUG ((DEBUG_INFO, "MirrorRequest found setting up mirror regions\n"));

      if(AddressBasedMirrorData.MirrorVersion == 1) {
        //Enable partial mirror both mdoes if requested from UEFI method in previous boot
        Setup->mem.partialMirrorUEFI = 1;
        Setup->mem.partialmirrorsad0 = AddressBasedMirrorData.MirrorMemoryBelow4GB;
        Setup->mem.partialmirrorpercent = AddressBasedMirrorData.MirroredAmountAbove4GB;
        Setup->mem.partialmirrorsts = MIRROR_STATUS_SUCCESS;

      } else {

        DEBUG ((EFI_D_ERROR, "Incorrect Mirror Request Version\n"));
        Setup->mem.partialmirrorsts = MIRROR_STATUS_VERSION_MISMATCH;
        Setup->mem.partialMirrorUEFI = 1;
        Setup->mem.partialmirrorsad0 = 0;
        Setup->mem.partialmirrorpercent = 0;
      }
    } else {

      UINT8 count;
      //Use Setup for configuration
      Setup->mem.partialmirrorsts = MIRROR_STATUS_SUCCESS;
      Setup->mem.partialMirrorUEFI = 0;
      Setup->mem.partialmirrorpercent = 0;
      if (SetupData->SocketConfig.CommonRcConfig.MirrorMode == PMIRROR_1LM_ENABLED) {
        Setup->mem.partialmirrorsts = MIRROR_STATUS_OEM_SPECIFIC_CONFIGURATION;
      }

      if (SetupData->SocketConfig.CommonRcConfig.MirrorMode == PMIRROR_2LM_ENABLED) {
        Setup->mem.partialmirrorsts = MIRROR_STATUS_OEM_SPECIFIC_CONFIGURATION;
      }

      if (SetupData->SocketConfig.MemoryConfig.partialmirrorsad0 == PMIRROR_ENABLED) {
        Setup->mem.partialmirrorsad0 = 1;
        Setup->mem.partialmirrorsts = MIRROR_STATUS_OEM_SPECIFIC_CONFIGURATION;
      }

      if (SetupData->SocketConfig.MemoryConfig.PartialMirrorUefi == PMIRROR_ENABLED) {
        Setup->mem.partialMirrorUEFI = 1;
        Setup->mem.partialmirrorpercent = SetupData->SocketConfig.MemoryConfig.PartialMirrorUefiPercent;
        Setup->mem.partialmirrorsts = MIRROR_STATUS_OEM_SPECIFIC_CONFIGURATION;
      }

      for (count=0; count<MAX_PARTIAL_MIRROR; count++) {
        //Convert Value entered in 1 GB multiples into 64MB Chunks (1064/64 = 16)
        Setup->mem.partialmirrorsize[count] = (SetupData->SocketConfig.MemoryConfig.partialmirrorsize[count])*16;
      }
    }

  }


  if (FixedPcdGetBool (PcdBiosDfxKnobEnabled)) {
    Setup->mem.dfxMemSetup.dfxLowMemChannel = SetupData->SocketConfig.MemoryConfig.dfxLowMemChannel;
    // Force channel interleave to 1
    if (Setup->mem.dfxMemSetup.dfxLowMemChannel) {
      Setup->mem.chInter = CH_1WAY;
    }

    if (SetupData->SocketConfig.MemoryConfig.dfxHighAddressStartBitPosition) {
      Setup->mem.dfxMemSetup.dfxOptions |= HIGH_ADDR_EN;
      Setup->mem.dfxMemSetup.dfxHighAddrBitStart = SetupData->SocketConfig.MemoryConfig.dfxHighAddressStartBitPosition;
    } else {
      Setup->mem.dfxMemSetup.dfxHighAddrBitStart = 0;
    }

    Setup->mem.dfxMemSetup.dfxStraddleHighAddressBit = SetupData->SocketConfig.MemoryConfig.dfxStraddleHighAddressBit;

    Setup->mem.dfxMemSetup.dfxLowMemChannel = SetupData->SocketConfig.MemoryConfig.dfxLowMemChannel;

    if (!Is14nmCpu) {
      Setup->common.ddrtXactor = SetupData->SocketConfig.MemoryConfig.DfxDdrtTransactor;
    }
    if (SetupData->SocketConfig.MemoryConfig.DfxCrSkipArsOnBoot == CR_SKIP_ARS_ON_BOOT_EN) {
      Setup->mem.dfxMemSetup.dfxOptions |= CR_SKIP_ARS_ON_BOOT;
    } else if (SetupData->SocketConfig.MemoryConfig.DfxCrSkipArsOnBoot == CR_SKIP_ARS_ON_BOOT_DIS) {
      Setup->mem.dfxMemSetup.dfxOptions &= ~CR_SKIP_ARS_ON_BOOT;
    }

    if (SetupData->SocketConfig.MemoryConfig.DfxCrBackgroundArs == CR_BACKGROUND_ARS_AUTO) {
      Setup->mem.dfxMemSetup.dfxOptions &= ~CR_NO_BACKGROUND_ARS;
    } else if (SetupData->SocketConfig.MemoryConfig.DfxCrBackgroundArs == CR_BACKGROUND_ARS_DIS) {
      Setup->mem.dfxMemSetup.dfxOptions |= CR_NO_BACKGROUND_ARS;
    }

    if (SetupData->SocketConfig.MemoryConfig.DfxCrSeamlessOptIn == CR_SEAMLESS_OPT_IN_EN) {
      Setup->mem.dfxMemSetup.dfxOptions |= CR_SEAMLESS_OPT_IN;
    } else if (SetupData->SocketConfig.MemoryConfig.DfxCrSeamlessOptIn == CR_SEAMLESS_OPT_IN_DIS) {
      Setup->mem.dfxMemSetup.dfxOptions &= ~CR_SEAMLESS_OPT_IN;
    }


    if (PcdGetBool (PcdPostedCsrAccessSupported) && !IsSiliconWorkaroundEnabled ("S2207989002")) {
      if (SetupData->SocketConfig.MemoryConfig.DfxPostedCsrAccessAllowed == POSTED_CSR_ACCESS_AUTO) {
        Setup->mem.dfxMemSetup.DfxPostedCsrAccessAllowed = PcdGetBool (PcdPostedCsrAccessAllowedDefault);
      } else {
        if (SetupData->SocketConfig.MemoryConfig.DfxPostedCsrAccessAllowed == POSTED_CSR_ACCESS_EN) {
          Setup->mem.dfxMemSetup.DfxPostedCsrAccessAllowed = TRUE;
        } else {
          Setup->mem.dfxMemSetup.DfxPostedCsrAccessAllowed = FALSE;
        }
      }
    }

    if (PcdGetBool (PcdCpgcRegCacheAccessSupported)) {
      if (SetupData->SocketConfig.MemoryConfig.DfxCpgcRegCacheAccessAllowed == CPGC_REG_CACHE_ACCESS_AUTO) {
        Setup->mem.dfxMemSetup.DfxCpgcRegCacheAccessAllowed = PcdGetBool (PcdCpgcRegCacheAccessAllowedDefault);
      } else {
        if (SetupData->SocketConfig.MemoryConfig.DfxCpgcRegCacheAccessAllowed == CPGC_REG_CACHE_ACCESS_EN) {
          Setup->mem.dfxMemSetup.DfxCpgcRegCacheAccessAllowed = TRUE;
        } else {
          Setup->mem.dfxMemSetup.DfxCpgcRegCacheAccessAllowed = FALSE;
        }
      }
    }

    //
    // Synchronized CPGC test start
    //
    if (PcdGetBool (PcdCpgcGlobalSyncCtrlSupported)) {
      if (SetupData->SocketConfig.MemoryConfig.DfxCpgcGlobalSyncCtrl == CPGC_SYNC_START_AUTO) {
        Setup->mem.dfxMemSetup.DfxCpgcGlobalSyncCtrlEnabled = PcdGetBool (PcdCpgcGlobalSyncCtrlEnableDefault);
      } else {
        if (SetupData->SocketConfig.MemoryConfig.DfxCpgcGlobalSyncCtrl == CPGC_SYNC_START_EN) {
          Setup->mem.dfxMemSetup.DfxCpgcGlobalSyncCtrlEnabled = TRUE;
        } else {
          Setup->mem.dfxMemSetup.DfxCpgcGlobalSyncCtrlEnabled = FALSE;
        }
      }
    }

    if (SetupData->SocketConfig.MemoryConfig.DfxSsaRmtRecvEnableDenormalize == DFX_SSA_RMT_RECV_ENABLE_DENORMALIZE_ENABLE) {
      Setup->mem.dfxMemSetup.DfxSsaRmtRecvEnableDenormalize = TRUE;
    } else {
      Setup->mem.dfxMemSetup.DfxSsaRmtRecvEnableDenormalize = FALSE;
    }

    //
    // DFX Periodic ZQCal
    //
    if (SetupData->SocketConfig.MemoryConfig.DfxPeriodicZqCal == DFX_PERIODIC_ZQCAL_AUTO) {
      Setup->mem.dfxMemSetup.DfxPeriodicZqCal = PcdGetBool (PcdDfxPeriodicZqCalDefault);
    } else if (SetupData->SocketConfig.MemoryConfig.DfxPeriodicZqCal == DFX_PERIODIC_ZQCAL_ENABLE) {
      Setup->mem.dfxMemSetup.DfxPeriodicZqCal = TRUE;
    } else {
      ASSERT (SetupData->SocketConfig.MemoryConfig.DfxPeriodicZqCal == DFX_PERIODIC_ZQCAL_DISABLE);
      Setup->mem.dfxMemSetup.DfxPeriodicZqCal = FALSE;
    }

    //
    // DFX Sense Amp Calibration Algorithm selection
    //
    if (PcdGetBool (PcdSenseAmpCalibHwFsmSupported)) {
      if (SetupData->SocketConfig.MemoryConfig.DfxSenseAmpCalibHwSwOption == DFX_SENSE_AMP_CALIB_AUTO) {
        Setup->mem.dfxMemSetup.SenseAmpCalibHwSwOption = FixedPcdGet8 (PcdDfxSenseAmpCalibHwSwDefault);
      } else if (SetupData->SocketConfig.MemoryConfig.DfxSenseAmpCalibHwSwOption == DFX_SENSE_AMP_CALIB_HW_FSM) {
        Setup->mem.dfxMemSetup.SenseAmpCalibHwSwOption = DFX_SENSE_AMP_CALIB_HW_FSM;
      } else {
        ASSERT (SetupData->SocketConfig.MemoryConfig.DfxSenseAmpCalibHwSwOption == DFX_SENSE_AMP_CALIB_SW);
        Setup->mem.dfxMemSetup.SenseAmpCalibHwSwOption = DFX_SENSE_AMP_CALIB_SW;
      }
    } // PcdSenseAmpCalibHwFsmSupported

    //
    // DFE SW Xover selection
    //
    Setup->mem.dfxMemSetup.DfxSwXoverOption = SetupData->SocketConfig.MemoryConfig.DfxSwXoverOption;
  } //if (FixedPcdGetBool (PcdBiosDfxKnobEnabled))

  //
  // Fetch MemoryConfig options
  //
  Setup->mem.EliminateDirectoryInFarMemory = SetupData->SocketConfig.MemoryConfig.EliminateDirectoryInFarMemory;
  Setup->mem.FastGoConfig                  = SetupData->SocketConfig.MemoryConfig.FastGoConfig;
  Setup->mem.NvmdimmPerfConfig             = SetupData->SocketConfig.MemoryConfig.NvmdimmPerfConfig;
  Setup->mem.NvmdimmPowerCyclePolicy       = SetupData->SocketConfig.MemoryConfig.NvmdimmPowerCyclePolicy;
  Setup->mem.LatchSystemShutdownState      = SetupData->SocketConfig.MemoryConfig.LatchSystemShutdownState;
  Setup->mem.DisableDirForAppDirect        = SetupData->SocketConfig.MemoryConfig.DisableDirForAppDirect;
  Setup->mem.AppDirectMemoryHole           = SetupData->SocketConfig.MemoryConfig.AppDirectMemoryHole;
  Setup->mem.ExtendedType17                = SetupData->SocketConfig.MemoryConfig.ExtendedType17;

  // Factory Reset Clear
  Setup->mem.FactoryResetClear = SetupData->SocketConfig.MemoryConfig.FactoryResetClear;

  //
  // CR QoS feature
  //
  Setup->mem.NvmQos = SetupData->SocketConfig.MemoryConfig.NvmQos;
  if (FixedPcdGetBool (PcdBiosDfxKnobEnabled)) {
    //
    // Fetch NFIT implementation options
    //
    Setup->mem.dfxMemSetup.DfxNfitDebugLogsEnable = SetupData->SocketConfig.MemoryConfig.DfxNfitDebugLogs;
    Setup->mem.dfxMemSetup.DfxNfitNvdimmSkuBasedDisable = !SetupData->SocketConfig.MemoryConfig.DfxNfitNvdimmSkuBased;
    Setup->mem.dfxMemSetup.DfxNfitMailboxWithPropGuid = SetupData->SocketConfig.MemoryConfig.DfxNfitMailboxWithPropGuid;

    Setup->mem.dfxMemSetup.DfxCrAcpiDebugInterface = SetupData->SocketConfig.MemoryConfig.DfxCrAcpiDebugInterface;
  } //if (FixedPcdGetBool (PcdBiosDfxKnobEnabled))

  Setup->mem.LsxImplementation                   = SetupData->SocketConfig.MemoryConfig.LsxImplementation;
  Setup->mem.NvdimmSmbusMaxAccessTime            = SetupData->SocketConfig.MemoryConfig.NvdimmSmbusMaxAccessTime;
  Setup->mem.NvdimmSmbusReleaseDelay             = SetupData->SocketConfig.MemoryConfig.NvdimmSmbusReleaseDelay;
  Setup->mem.NfitPublishMailboxStructsDisable    = !SetupData->SocketConfig.MemoryConfig.NfitPublishMailboxStructs;
  if (FixedPcdGetBool (PcdBiosDfxKnobEnabled)) {
    //
    // Enabled/Disable eADR additional logs
    //
    Setup->mem.dfxMemSetup.DfxEadrDebugLogsEnable = SetupData->SocketConfig.MemoryConfig.DfxEadrDebugLogs;
    Setup->mem.dfxMemSetup.DfxEadrFlowTimeLogsEnable = SetupData->SocketConfig.MemoryConfig.DfxEadrFlowTimeLogs;
    Setup->mem.dfxMemSetup.DfxEadrProtocolLogsEnable = SetupData->SocketConfig.MemoryConfig.DfxEadrProtocolLogs;
  } //if (FixedPcdGetBool (PcdBiosDfxKnobEnabled))

  //
  // Enabled/Disable fADR additional logs
  //
  Setup->mem.dfxMemSetup.DfxFadrDebugLogsEnable = SetupData->SocketConfig.MemoryConfig.DfxFadrDebugLogs;
  Setup->mem.dfxMemSetup.DfxFadrFlowTimeLogsEnable = SetupData->SocketConfig.MemoryConfig.DfxFadrFlowTimeLogs;

  //
  // Enable/Disable NVMCTRL Media status as exception
  //
  Setup->mem.NvmMediaStatusException = SetupData->SocketConfig.MemoryConfig.NvmMediaStatusException;

  //
  // Enable/Disable not supported DCPMM as exception
  //
  Setup->mem.AepNotSupportedException = SetupData->SocketConfig.MemoryConfig.AepNotSupportedException;

  //
  // Check MOR to see if we need clear memory
  //
  {
    TCG_SETUP TcgSetup;
    UINT8     MorControl;
// APTIOV_SERVER_OVERRIDE_RC_START : Using AMI TCG Module ,  Fixed S3 failed since options are changed.
    BootMode                              BootMode;
    BootMode = GetSysBootMode ();
// APTIOV_SERVER_OVERRIDE_RC_END : Using AMI TCG Module ,  Fixed S3 failed since options are changed.

    TcgSetup.MorState = SetupData->SystemConfig.MorState;
// APTIOV_SERVER_OVERRIDE_RC_START
//  if (TcgSetup.MorState){ // Using AMI TCG Module
    if (BootMode != S3Resume) {  //  Fixed S3 failed since options are changed.
// APTIOV_SERVER_OVERRIDE_RC_END
      DEBUG((DEBUG_INFO, "MEMORY_INIT: MOR Control Setup Enabled!\n"));
      VariableSize   = sizeof (UINT8);
      Status = PeiVariable->GetVariable (
                              PeiVariable,
                              MEMORY_OVERWRITE_REQUEST_VARIABLE_NAME,
                              &gEfiMemoryOverwriteControlDataGuid,
                              NULL,
                              &VariableSize,
                              &MorControl
                            );

      if (MOR_CLEAR_MEMORY_VALUE (MorControl) == 0x1) {
        SetWipeMemory ();
        DEBUG ((DEBUG_INFO, "MEMORY_INIT: MOR Control Clear Action requested!\n"));
        DEBUG ((DEBUG_INFO, "MEMORY_INIT: Memory will be Wiped\n"));
      }
    }
  }

  for (socket = 0; socket < MAX_SOCKET; socket++) {
    for (ch = 0; ch < MAX_CH; ch++) {
      Setup->mem.socket[socket].ddrCh[ch].numDimmSlots = 2;
    }
  }

  for (socket = 0; socket < MAX_SOCKET; socket++) {
    for (ch = 0; ch < MAX_CH; ch++) {
      Setup->mem.MemoryTopology[socket][ch] = GetMemTopology (socket, ch);
      Setup->mem.MemoryConnectorType[socket][ch] = GetMemConnectorType (socket, ch);
    }
  }

  UpdateMemoryStoragePolicy (SetupData, PolicyPpi);

  //
  // Set in the policy whether an ADR event has been set or not.
  //

  Setup->AdrEvent = DetectADR ();

  //
  // Set the ADR GPIO's. TODO - This is not the correct place for this. Temporarily
  // putting this here because platform dependencies block us from putting this in
  // the MRC.
  //

  SetAdrEnablePadOutVal (SetupData->SocketConfig.MemoryConfig.ADREn);

  Setup->mem.LegacyADRModeEn = SetupData->SocketConfig.MemoryConfig.LegacyADRModeEn;
  Setup->mem.MinNormalMemSize = SetupData->SocketConfig.MemoryConfig.MinNormalMemSize;

  //
  //Set margin check data
  //
  Setup->mem.MemBootHealthConfig.MemBootHealthCheck   = SetupData->MemBootHealthConfig.MemBootHealthCheck;
  Setup->mem.MemBootHealthConfig.ResetOnCriticalError = SetupData->MemBootHealthConfig.ResetOnCriticalError;

  Setup->mem.MemBootHealthConfig.WarningTxDqDelayLeftEdge   = SetupData->MemBootHealthConfig.WarningTxDqDelayLeftEdge;
  Setup->mem.MemBootHealthConfig.WarningTxDqDelayRightEdge  = SetupData->MemBootHealthConfig.WarningTxDqDelayRightEdge;
  Setup->mem.MemBootHealthConfig.WarningTxVrefLeftEdge      = SetupData->MemBootHealthConfig.WarningTxVrefLeftEdge;
  Setup->mem.MemBootHealthConfig.WarningTxVrefRightEdge     = SetupData->MemBootHealthConfig.WarningTxVrefRightEdge;
  Setup->mem.MemBootHealthConfig.WarningRxDqsDelayLeftEdge  = SetupData->MemBootHealthConfig.WarningRxDqsDelayLeftEdge;
  Setup->mem.MemBootHealthConfig.WarningRxDqsDelayRightEdge = SetupData->MemBootHealthConfig.WarningRxDqsDelayRightEdge;
  Setup->mem.MemBootHealthConfig.WarningRxVrefLeftEdge      = SetupData->MemBootHealthConfig.WarningRxVrefLeftEdge;
  Setup->mem.MemBootHealthConfig.WarningRxVrefRightEdge     = SetupData->MemBootHealthConfig.WarningRxVrefRightEdge;

  Setup->mem.MemBootHealthConfig.CriticalTxDqDelayLeftEdge   = SetupData->MemBootHealthConfig.CriticalTxDqDelayLeftEdge;
  Setup->mem.MemBootHealthConfig.CriticalTxDqDelayRightEdge  = SetupData->MemBootHealthConfig.CriticalTxDqDelayRightEdge;
  Setup->mem.MemBootHealthConfig.CriticalTxVrefLeftEdge      = SetupData->MemBootHealthConfig.CriticalTxVrefLeftEdge;
  Setup->mem.MemBootHealthConfig.CriticalTxVrefRightEdge     = SetupData->MemBootHealthConfig.CriticalTxVrefRightEdge;
  Setup->mem.MemBootHealthConfig.CriticalRxDqsDelayLeftEdge  = SetupData->MemBootHealthConfig.CriticalRxDqsDelayLeftEdge;
  Setup->mem.MemBootHealthConfig.CriticalRxDqsDelayRightEdge = SetupData->MemBootHealthConfig.CriticalRxDqsDelayRightEdge;
  Setup->mem.MemBootHealthConfig.CriticalRxVrefLeftEdge      = SetupData->MemBootHealthConfig.CriticalRxVrefLeftEdge;
  Setup->mem.MemBootHealthConfig.CriticalRxVrefRightEdge     = SetupData->MemBootHealthConfig.CriticalRxVrefRightEdge;

  //
  // Set socket config
  //
  GuidHob = GetFirstGuidHob (&gEfiPlatformInfoGuid);
  PlatformInfo = GET_GUID_HOB_DATA (GuidHob);
  Setup->common.SocketConfig = PlatformInfo->SysData.SocketConfig;

  return EFI_SUCCESS;

} // UpdatePeiMemoryPolicy
