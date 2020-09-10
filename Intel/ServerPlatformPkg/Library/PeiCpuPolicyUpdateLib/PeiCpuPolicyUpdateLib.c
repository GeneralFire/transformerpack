/** @file
  PEI CPU policy update library implementation.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 - 2020 Intel Corporation. <BR>

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

#include <Register/Cpuid.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/HobLib.h>
#include <Library/CpuPreMemPolicyLib.h>
#include <Library/SetupLib.h>
#include <Library/PmcLib.h>
#include <Library/MeTypeLib.h>
#include <Library/MeUtilsLib.h>
#include <Library/MmPciBaseLib.h>
#include <Library/EmulationConfigurationLib.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <Guid/CpuNvramData.h>
#include <Ppi/ReadOnlyVariable2.h>
#include <Ppi/Stall.h>
#include <Sps.h>
#include <MeAccess.h>
#include <HeciRegs.h>


/**
  Get OSC core disable mask

  @param CpuPolicy      Cpu Pre Mem policy pointer

**/
VOID
GetOscCoreDisMask (
  IN OUT CPU_PRE_MEM_POLICY *CpuPolicy
  )
{
  EFI_STATUS                      Status;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI *PeiVariable;
  UINTN                           VariableSize;
  UINT64                          OscCoreDisableMask[MAX_SOCKET];
  UINT8                           Socket;

  Status = PeiServicesLocatePpi (
             &gEfiPeiReadOnlyVariable2PpiGuid,
             0,
             NULL,
             (VOID **) &PeiVariable
             );
  if (EFI_ERROR (Status)) {
    return;
  }

  Status = PeiVariable->GetVariable (
                          PeiVariable,
                          L"OscCoreDisableMask",
                          &gRasGlobalDataVariableGuid,
                          NULL,
                          &VariableSize,
                          &OscCoreDisableMask
                          );
  if(Status == EFI_SUCCESS) {
    CopyMem (&CpuPolicy->OscCoreDisMask, OscCoreDisableMask, sizeof(UINT64) * MAX_SOCKET);
  } else {
    for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
      CpuPolicy->OscCoreDisMask[Socket] = 0x00;
    }
  }
}

/**
  Update NumberOfCores2Disable value in CPU Pre Mem policy.

  @param SetupData  Pointer to SETUP data
  @param CpuPolicy  Pointer to CPU Pre Mem policy

**/
VOID
UpdateNumberOfCores2Disable (
//APTIOV_SERVER_OVERRIDE_RC_START : To solve structure redefinition error in AMI.         
  IN     INTEL_SETUP_DATA *SetupData,
//APTIOV_SERVER_OVERRIDE_RC_END : To solve structure redefinition error in AMI.
  IN OUT CPU_PRE_MEM_POLICY *CpuPolicy
  )
{

#if ME_SPS_SUPPORT
  EFI_BOOT_MODE         BootMode;
  HECI_FWS_REGISTER     MeFs1;
  SPS_NMFS              NmFs;
  UINT32                Timeout;
  EFI_PEI_STALL_PPI     *StallPpi;
  EFI_PEI_SERVICES      **PeiServices;
  EFI_STATUS            Status;
  UINT16                NumberOfCores2Disable;
  UINT8                 Socket;

  if (PmcIsDwrBootMode ()) {
    return;
  }

  DEBUG ((DEBUG_INFO, "[SPS] %a called.\n", __FUNCTION__));

  //
  // Determine boot mode
  //
  Status = PeiServicesGetBootMode (&BootMode);
  ASSERT_EFI_ERROR (Status);

  //
  // If it is SPS firmware running in ME and Node Manager is enabled
  // apply NM boot time policy, i.e. processor cores disabling.
  // Note that default for Cores2Disable is neutral so
  // for non SPS firmware or ME error BIOS default is not changed.
  //
  if (BootMode != BOOT_ON_S3_RESUME) {
    if (BootMode == BOOT_ON_S4_RESUME) {
      DEBUG ((DEBUG_INFO, "[SPS] Skip cores reconfiguration on S4 resume\n"));
      NumberOfCores2Disable = SetupData->MeRcConfig.NmCores2Disable;
    } else {
      NumberOfCores2Disable = 0;
      if (HybridSystemLevelEmulationEnabled ()) {
        Timeout = 2;
      } else {
        Timeout = 500;
      }
      PeiServices = (EFI_PEI_SERVICES **) GetPeiServicesTablePointer ();
      StallPpi = NULL;
      PeiServicesLocatePpi (&gEfiPeiStallPpiGuid, 0, NULL, &StallPpi);
      while (TRUE) {
        MeFs1.ul = MmPci32 (0, ME_BUS, ME_DEVICE_NUMBER, HECI_FUNCTION_NUMBER, R_ME_HFS);
        NmFs.UInt32 = MmPci32 (0, ME_BUS, ME_DEVICE_NUMBER, HECI2_FUNCTION_NUMBER, SPS_REG_NMFS);

        if (!MeTypeIsSps ()) {
          // do not proceed for non-SPS firmware
          DEBUG ((DEBUG_WARN, "[ME] Non SPS firmware in ME (MEFS1: 0x%08X, NMFS: 0x%08X)\n", MeFs1.ul, NmFs.UInt32));
          break;
        }

        if (MeFs1.r.ErrorCode != 0) {
          DEBUG ((DEBUG_ERROR, "[ME] ERROR: Invalid ME state (MEFS1: 0x%08X)\n", MeFs1.ul));
          break;
        }

        if (MeSpsIsNm ()) {
#if ME_TESTMENU_FLAG
          if (SetupData->MeRcConfig.NmCores2DisableOverride) {
            NumberOfCores2Disable = SetupData->MeRcConfig.NmCores2Disable;
          } else
#endif
          {
            NumberOfCores2Disable = (UINT16) NmFs.Bits.Cores2Disable;
          }
          DEBUG ((DEBUG_INFO, "[SPS] NM firmware detected in ME, "
                              "cores to disable: %d (NMFS: 0x%08X)\n",
                              NumberOfCores2Disable, NmFs.UInt32));
          break;
        }

        if (MeFs1.r.CurrentState >= ME_STATE_RECOVERY) {
          DEBUG ((DEBUG_ERROR, "[SPS] NM not enabled in ME (MEFS1: 0x%08X, NMFS: 0x%08X)\n", MeFs1.ul, NmFs.UInt32));
          break;
        }
        if (Timeout-- == 0) {
         DEBUG ((DEBUG_ERROR, "[SPS] ERROR: Timeout waiting for ME (MEFS1: 0x%08X)\n", MeFs1.ul));
         break;
        }
        if (StallPpi != NULL) {
          if (!(HybridSystemLevelEmulationEnabled ())) {
            StallPpi->Stall (PeiServices, StallPpi, 1000);
          }
        }
      }
    }

    for (Socket = 0; Socket < MAX_SOCKET; Socket++) {
      CpuPolicy->NumberOfCores2Disable[Socket] = NumberOfCores2Disable;
    }
  } // if (BootMode != BOOT_ON_S3_RESUME)
#endif // ME_SPS_SUPPORT
}

/**
  Set CPU Pre Mem Policy based on SETUP data.

  @param[in, out] SetupData       Pointer to SETUP_DATA structure.

**/
VOID
SetCpuPolicyBasedOnSetup (
  //APTIOV_SERVER_OVERRIDE_RC_START : Match Intel Setup Data definition
  IN OUT    INTEL_SETUP_DATA          *SetupData
  //APTIOV_SERVER_OVERRIDE_RC_END : Match Intel Setup Data definition
  )
{
  CPU_PRE_MEM_POLICY              *CpuPolicy;
  UINT8                           socket;

  CpuPolicy = GetCpuPreMemPolicy();
  ASSERT (CpuPolicy != NULL);
  if (CpuPolicy == NULL) {
    return;
  }


  CpuPolicy->debugInterfaceEn = 0;
  CpuPolicy->dcuModeSelect = SetupData->SocketConfig.SocketProcessorCoreConfiguration.DCUModeSelection;    // CPU DCU Mode select
  CpuPolicy->flexRatioEn = SetupData->SocketConfig.SocketProcessorCoreConfiguration.ProcessorFlexibleRatioOverrideEnable; // FlexRatio enable
  //
  // Verify that the values being set are within a valid range, if not use default.
  //
  if(SetupData->SocketConfig.SocketProcessorCoreConfiguration.ProcessorFlexibleRatio > MAX_PROCESSOR_CORE_RATIO){
    SetupData->SocketConfig.SocketProcessorCoreConfiguration.ProcessorFlexibleRatio = DEFAULT_PROCESSOR_CORE_RATIO; // If out of range set the default
  }

  CpuPolicy->flexRatioNext = SetupData->SocketConfig.SocketProcessorCoreConfiguration.ProcessorFlexibleRatio;        // Common NewRatio for all socket
  CpuPolicy->smtEnable = SetupData->SocketConfig.SocketProcessorCoreConfiguration.ProcessorHyperThreadingDisable ? 0 : 1;
  CpuPolicy->vtEnable = SetupData->SocketConfig.SocketProcessorCoreConfiguration.ProcessorVmxEnable;
  CpuPolicy->EnableGv = SetupData->SocketConfig.PowerManagementConfig.ProcessorEistEnable;
  CpuPolicy->WFRWAEnable = SetupData->SocketConfig.PowerManagementConfig.WFRWAEnable;
  CpuPolicy->IssTdpLevel = SetupData->SocketConfig.PowerManagementConfig.IssTdpLevel;
  CpuPolicy->DynamicIss = SetupData->SocketConfig.PowerManagementConfig.DynamicIss;
  CpuPolicy->ActivePbf = SetupData->SocketConfig.PowerManagementConfig.ProcessorActivePbf;
  CpuPolicy->ConfigTdpLevel = SetupData->SocketConfig.PowerManagementConfig.ConfigTdpLevel;
  CpuPolicy->AllowMixedPowerOnCpuRatio = SetupData->SocketConfig.SocketProcessorCoreConfiguration.AllowMixedPowerOnCpuRatio;
  CpuPolicy->CheckCpuBist = SetupData->SocketConfig.SocketProcessorCoreConfiguration.CheckCpuBist;
  CpuPolicy->CoreFailover = SetupData->SocketConfig.SocketProcessorCoreConfiguration.CoreFailover;
  CpuPolicy->RdtCatOpportunisticTuning = (UINT8) SetupData->SocketConfig.SocketProcessorCoreConfiguration.RdtCatOpportunisticTuning;
  CpuPolicy->EarlyC1eEnable = SetupData->SocketConfig.PowerManagementConfig.ProcessorC1eEnable;
  CpuPolicy->CpuPaLimit = SetupData->SocketConfig.SocketProcessorCoreConfiguration.CpuPaLimit;
  CpuPolicy->LlcPrefetchEnable = SetupData->SocketConfig.SocketProcessorCoreConfiguration.LlcPrefetchEnable;
  CpuPolicy->ProcessorMsrLockControl = SetupData->SocketConfig.SocketProcessorCoreConfiguration.ProcessorMsrLockControl;
  CpuPolicy->ProcessorMsrPkgCstConfigControlLock = SetupData->SocketConfig.SocketProcessorCoreConfiguration.ProcessorMsrPkgCstConfigControlLock;
  CpuPolicy->FadrSupport = SetupData->SocketConfig.MemoryConfig.FadrSupport;
  CpuPolicy->TscResetEnable = SetupData->SocketConfig.SocketProcessorCoreConfiguration.TscResetEnable;


  if (FixedPcdGetBool (PcdBiosDfxKnobEnabled)) {
    CpuPolicy->DfxBistFailureEmulation = SetupData->SocketConfig.SocketProcessorCoreConfiguration.DfxBistFailureEmulation;
  }

  //
  // PMAX Disable OEM hook, set to 1 to disable
  //
  CpuPolicy->PmaxDisable = SetupData->SocketConfig.PowerManagementConfig.PmaxDetector ? 0 : 1;

  //
  // 5330679: Requesting BIST Check setup option disabled
  //

  for (socket = 0; socket<MAX_SOCKET; socket++) {
    CpuPolicy->CoreDisableMask[socket] = SetupData->SocketConfig.SocketProcessorCoreConfiguration.CoreDisableMask[socket];
    CpuPolicy->NumberOfCores2Disable[socket] = 0;
    CpuPolicy->IotEn[socket] = (UINT8) SetupData->SocketConfig.SocketProcessorCoreConfiguration.IotEn[socket];
    CpuPolicy->OclaTorEntry[socket] = SetupData->SocketConfig.SocketProcessorCoreConfiguration.OclaMaxTorEntry[socket];
    CpuPolicy->OclaWay[socket] = SetupData->SocketConfig.SocketProcessorCoreConfiguration.OclaMinWay[socket];
  }
  UpdateNumberOfCores2Disable (SetupData, CpuPolicy);

  GetOscCoreDisMask (CpuPolicy);

  SetCpuPreMemPolicy(CpuPolicy, sizeof(CpuPolicy));
}

/**
  Save CPU NVRAM data from Variable to Guided HOB.

**/
VOID
SaveCpuNvram (
  VOID
  )
{
  CPU_NVRAM                         *CpuNvramGuidHob;
  UINTN                             DataSize;
  VOID*                             GuidHob;
  EFI_STATUS                        Status;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI   *PeiVariable;
  UINT8                             *CpuNvramGuidHobHeader;

  Status = PeiServicesLocatePpi (
             &gEfiPeiReadOnlyVariable2PpiGuid,
             0,
             NULL,
             (VOID **) &PeiVariable
             );
  if (EFI_ERROR (Status)) {
    return;
  }

  GuidHob = GetFirstGuidHob (&gEfiCpuNvramDataGuid);
  if (GuidHob != NULL) {
    CpuNvramGuidHob = GET_GUID_HOB_DATA (GuidHob);
  } else {
    CpuNvramGuidHob = BuildGuidHob (&gEfiCpuNvramDataGuid, sizeof (CPU_NVRAM));
    if (CpuNvramGuidHob == NULL) {
      return;
    }

    ZeroMem (CpuNvramGuidHob, sizeof (CPU_NVRAM));
    CpuNvramGuidHobHeader = (UINT8 *) CpuNvramGuidHob;
    GuidHob = CpuNvramGuidHobHeader - sizeof (EFI_HOB_GUID_TYPE);
  }
  DataSize = GET_GUID_HOB_DATA_SIZE (GuidHob);
  Status = PeiVariable->GetVariable (
                          PeiVariable,
                          EFI_CPU_NVRAM_DATA_VARIABLE_NAME,
                          &gEfiCpuNvramDataGuid,
                          NULL,
                          &DataSize,
                          CpuNvramGuidHob
                          );
}

/**
  Update PEI CPU policy.

  @param[in, out] SetupData       Pointer to SETUP_DATA structure.

  @return Update status.

**/
EFI_STATUS
EFIAPI
UpdatePeiCpuPolicy (
  // APTIOV_SERVER_OVERRIDE_RC_START
  IN OUT    INTEL_SETUP_DATA          *SetupData
  // APTIOV_SERVER_OVERRIDE_RC_END
  )
{
  SetCpuPolicyBasedOnSetup (SetupData);
  SaveCpuNvram ();
  return EFI_SUCCESS;
}
