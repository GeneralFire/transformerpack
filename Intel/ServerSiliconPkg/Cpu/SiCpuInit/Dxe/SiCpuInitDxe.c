/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2020 Intel Corporation. <BR>

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

#include "SiCpuInitDxe.h"

CPU_POLICY_CONFIGURATION            *mCpuPolicyConfiguration;
MP_SYSTEM_DATA                      mMPSystemData;
EFI_CPU_CSR_ACCESS_PROTOCOL         *mCpuCsrAccess;
CPU_VAR_DATA                        *mCpuVarDataPtr;
EFI_MP_SERVICES_PROTOCOL            *mMpServices = NULL;

/**
  The post phase MP initialization after SMM initialization.

**/
VOID
PostProcessorConfiguration (
  VOID
  )
{
  //
  // Wakeup APs. Collect data of all processors. BSP polls to
  // wait for APs' completion.
  //
  DataCollectionPhase ();

  //
  // Select least-feature procesosr as BSP
  //
  if (FeaturePcdGet (PcdCpuSelectLfpAsBspFlag)) {
    SelectLfpAsBsp ();
  }

  //
  // Add SMBIOS Processor Type and Cache Type tables for the CPU.
  //
  AddCpuSmbiosTables ();
}

/**
  Event notification for gEfiDxeSmmReadyToLockProtocolGuid event.

  @param  Event                 The Event that is being processed, not used.
  @param  Context               Event Context, not used.

**/
VOID
EFIAPI
DxeSmmReadyToLockEventNotify (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  EFI_STATUS                Status;
  VOID                      *Interface;

  Status = gBS->LocateProtocol (&gEfiDxeSmmReadyToLockProtocolGuid, NULL, &Interface);
  if (EFI_ERROR (Status)) {
    return;
  }

  //
  // Save register table to RegisterTable and PreSmmInitRegisterTable
  // in PcdCpuS3DataAddress before PcdCpuS3DataAddress is consumed by
  // PiSmmCpuDxeSmm for CPU S3.
  //
  SaveRegisterTable ();

  gBS->CloseEvent (Event);
}

/**
  Early MP Initialization.

  This function does early MP initialization, including MTRR sync and first time microcode load.

  @param  ProcessorNumber    Handle number of specified logical processor.

**/
VOID
EFIAPI
EarlyMpInit (
  IN VOID                 *Buffer
  )
{
  EFI_STATUS         Status;
  UINTN              ProcessorNumber;
  Status = mMpServices->WhoAmI(mMpServices, &ProcessorNumber);
  ASSERT_EFI_ERROR (Status);

  CollectBasicProcessorData (ProcessorNumber);

  //
  //Collect the microcode version.
  //
  CollectMicrocodeRevision (ProcessorNumber);
}

/**
  Assign a package scope BSP responsible for package scope programming.

  This functions assigns the processor with the least APIC ID within a processor package as
  the package BSP.

**/
VOID
AssignPackageBsp (
  VOID
  )
{
  UINTN  Index;
  UINT32 PackageId;

  mCpuConfigLibConfigContextBuffer->CollectedDataBuffer[0].PackageBsp = TRUE;
  PackageId = mCpuConfigLibConfigContextBuffer->CollectedDataBuffer[0].ProcessorLocation.Package;
  for (Index = 1; Index < mCpuConfigLibConfigContextBuffer->NumberOfProcessors; Index++) {
    if (PackageId != mCpuConfigLibConfigContextBuffer->CollectedDataBuffer[Index].ProcessorLocation.Package) {
      PackageId = mCpuConfigLibConfigContextBuffer->CollectedDataBuffer[Index].ProcessorLocation.Package;
      mCpuConfigLibConfigContextBuffer->CollectedDataBuffer[Index].PackageBsp = TRUE;
    }
  }
}

/**
  Configures all logical processors with three-phase architecture.

  This function configures all logical processors with three-phase architecture.

**/
EFI_STATUS
ProcessorConfiguration (
  VOID
  )
{
  EFI_STATUS                   Status;
  UINTN                        NumberOfProcessors;
  UINTN                        NumberOfEnabledProcessors;
  UINTN                        ProcessorNum;
  EFI_PROCESSOR_INFORMATION    ProcessorInformation;

  Status = gBS->LocateProtocol (&gEfiMpServiceProtocolGuid, NULL, (VOID **) &mMpServices);
  ASSERT_EFI_ERROR (Status);
  Status = mMpServices->GetNumberOfProcessors (mMpServices, &NumberOfProcessors, &NumberOfEnabledProcessors);
  ASSERT_EFI_ERROR (Status);

  for (ProcessorNum = 0; ProcessorNum < NumberOfProcessors; ProcessorNum++) {
    Status = mMpServices->GetProcessorInfo (mMpServices, ProcessorNum, &ProcessorInformation);
    ASSERT_EFI_ERROR (Status);

    DEBUG ((EFI_D_ERROR, "GetProcessorInfo - Index - %x\n", ProcessorNum));
    DEBUG ((EFI_D_ERROR, "GetProcessorInfo - ProcessorId      - %016lx\n", ProcessorInformation.ProcessorId));
    DEBUG ((EFI_D_ERROR, "GetProcessorInfo - StatusFlag       - %08x\n",   ProcessorInformation.StatusFlag));
    DEBUG ((EFI_D_ERROR, "GetProcessorInfo - Location.Package - %08x\n",   ProcessorInformation.Location.Package));
    DEBUG ((EFI_D_ERROR, "GetProcessorInfo - Location.Core    - %08x\n",   ProcessorInformation.Location.Core));
    DEBUG ((EFI_D_ERROR, "GetProcessorInfo - Location.Thread  - %08x\n",   ProcessorInformation.Location.Thread));
    if ((ProcessorInformation.StatusFlag & PROCESSOR_AS_BSP_BIT) != 0) {
      mCpuConfigLibConfigContextBuffer->BspNumber = ProcessorNum;
    }
  }
  DEBUG ((EFI_D_ERROR, "mCpuConfigLibConfigContextBuffer->NumberOfProcessors = %x\n", mCpuConfigLibConfigContextBuffer->NumberOfProcessors));
  DEBUG ((EFI_D_ERROR, "mCpuConfigLibConfigContextBuffer->BspNumber = %x\n", mCpuConfigLibConfigContextBuffer->BspNumber));

  //
  // Initialize Spin Locks for system
  //
  InitializeSpinLock (&mMPSystemData.APSerializeLock);

  Status = mMpServices->StartupAllAPs (mMpServices, EarlyMpInit, FALSE, NULL, 0, NULL, NULL);
  if (EFI_ERROR(Status)) {
    DEBUG((EFI_D_ERROR, "Error: Not able to StartupAllAPs.  Status: %r\n", Status));
  }

  EarlyMpInit (NULL);

  DEBUG_CODE (
    //
    // Verify that all processors have same APIC ID topology. New APIC IDs
    // were constructed based on this assumption.
    //
    UINTN Index;
    UINT8 PackageIdBitOffset;

    PackageIdBitOffset = mCpuConfigLibConfigContextBuffer->CollectedDataBuffer[0].PackageIdBitOffset;
    for (Index = 1; Index < mCpuConfigLibConfigContextBuffer->NumberOfProcessors; Index++) {
      if (PackageIdBitOffset != mCpuConfigLibConfigContextBuffer->CollectedDataBuffer[Index].PackageIdBitOffset) {
        ASSERT (FALSE);
      }
    }
  );

  //
  // Assign Package BSP for package scope programming later.
  //
  AssignPackageBsp ();

  return EFI_SUCCESS;
}

/**
  Worker function to add CPU feature to user setting.

  @param[in]      Feature                       CPU feature index.
  @param[in, out] CpuFeaturesUserSetting        Pointer to CPU features user setting.
  @param[in]      CpuFeaturesUserSettingSize    CPU features user setting size.

**/
VOID
AddCpuFeaturesUserSetting (
  IN     UINT32        Feature,
  IN OUT UINT8         *CpuFeaturesUserSetting,
  IN     UINTN         CpuFeaturesUserSettingSize
  )
{
  CpuFeaturesUserSetting[Feature >> 3] |= (1 << (Feature & 0x07));
}

/**
  Set PcdCpuFeaturesSetting and install gEdkiiCpuFeaturesSetDoneGuid
  protocol which is depended on by CpuFeaturesDxe driver.

**/
VOID
SetPcdCpuFeaturesSetting (
  VOID
  )
{
  EFI_STATUS                    Status;
  CPU_POLICY_CONFIGURATION      *CpuPolicyConfiguration;
  UINT8                         *CpuFeaturesUserSetting;
  UINTN                         CpuFeaturesUserSettingSize;
  EFI_HANDLE                    Handle;

  Status = gBS->LocateProtocol (
                  &gEfiCpuPolicyProtocolGuid,
                  NULL,
                  &CpuPolicyConfiguration
                  );
  ASSERT_EFI_ERROR (Status);

  if (CpuPolicyConfiguration->Policy.RunCpuPpmInPei) {
    return;
  }

  DEBUG ((DEBUG_INFO, "SetPcdCpuFeaturesSetting in DXE\n"));

  CpuFeaturesUserSetting = (UINT8 *) PcdGetPtr (PcdCpuFeaturesSetting);
  ASSERT (CpuFeaturesUserSetting != NULL);
  if (CpuFeaturesUserSetting == NULL) {
    return;
  }
  CpuFeaturesUserSettingSize = PcdGetSize (PcdCpuFeaturesSetting);

  //
  // There is implementation for the features below in UefiCpuPkg CpuCommonFeaturesLib.
  // Some implementation needs to be overridden by CpuSpecificFeaturesLib.
  //
  if (CpuPolicyConfiguration->Policy.CpuTStateEnable) {
    AddCpuFeaturesUserSetting (CPU_FEATURE_ACPI, CpuFeaturesUserSetting, CpuFeaturesUserSettingSize);
    Status = PcdSet8S (PcdCpuClockModulationDutyCycle, CpuPolicyConfiguration->Policy.CpuClockModulationDutyCycle);
    ASSERT_EFI_ERROR (Status);
  }
  if (CpuPolicyConfiguration->Policy.CpuAesEnable) {
    AddCpuFeaturesUserSetting (CPU_FEATURE_AESNI, CpuFeaturesUserSetting, CpuFeaturesUserSettingSize);
  }
  if (CpuPolicyConfiguration->Policy.CpuFastStringEnable) {
    AddCpuFeaturesUserSetting (CPU_FEATURE_FASTSTRINGS, CpuFeaturesUserSetting, CpuFeaturesUserSettingSize);
  }
  if (CpuPolicyConfiguration->Policy.CpuMaxCpuidValueLimitEnable) {
    AddCpuFeaturesUserSetting (CPU_FEATURE_LIMIT_CPUID_MAX_VAL, CpuFeaturesUserSetting, CpuFeaturesUserSettingSize);
  }
  if (CpuPolicyConfiguration->Policy.CpuMachineCheckEnable) {
    AddCpuFeaturesUserSetting (CPU_FEATURE_MCE, CpuFeaturesUserSetting, CpuFeaturesUserSettingSize);
    AddCpuFeaturesUserSetting (CPU_FEATURE_MCA, CpuFeaturesUserSetting, CpuFeaturesUserSettingSize);
    AddCpuFeaturesUserSetting (CPU_FEATURE_MCG_CTL, CpuFeaturesUserSetting, CpuFeaturesUserSettingSize);
  }
  if (CpuPolicyConfiguration->Policy.CpuMonitorMwaitEnable) {
    AddCpuFeaturesUserSetting (CPU_FEATURE_MWAIT, CpuFeaturesUserSetting, CpuFeaturesUserSettingSize);
  }
  if (CpuPolicyConfiguration->Policy.CpuLtEnable) {
    AddCpuFeaturesUserSetting (CPU_FEATURE_SMX, CpuFeaturesUserSetting, CpuFeaturesUserSettingSize);
  }

  if (CpuPolicyConfiguration->Policy.CpuVtEnable) {
    AddCpuFeaturesUserSetting (CPU_FEATURE_VMX, CpuFeaturesUserSetting, CpuFeaturesUserSettingSize);
  }
  if (CpuPolicyConfiguration->Policy.CpuX2ApicEnable) {
    AddCpuFeaturesUserSetting (CPU_FEATURE_X2APIC, CpuFeaturesUserSetting, CpuFeaturesUserSettingSize);
  }
  if (CpuPolicyConfiguration->Policy.CpuEistEnable) {
    AddCpuFeaturesUserSetting (CPU_FEATURE_EIST, CpuFeaturesUserSetting, CpuFeaturesUserSettingSize);
  }
  if (CpuPolicyConfiguration->Policy.CpuPpinControlEnable) {
    //
    // It is Enable and Unlock, otherwise will be Disable and Lock.
    //
    AddCpuFeaturesUserSetting (CPU_FEATURE_PPIN, CpuFeaturesUserSetting, CpuFeaturesUserSettingSize);
  }

  //
  // There is no implementation for the features below in UefiCpuPkg CpuCommonFeaturesLib.
  //
  if (CpuPolicyConfiguration->Policy.CpuPeciDownstreamWriteEnable) {
    AddCpuFeaturesUserSetting (CPU_FEATURE_PECI_DOWN_STREAM, CpuFeaturesUserSetting, CpuFeaturesUserSettingSize);
  }
  if (!CpuPolicyConfiguration->Policy.CpuL1NextPagePrefetcherDisable) {
    AddCpuFeaturesUserSetting (CPU_FEATURE_L1_NEXT_PAGE_PREFETCHER, CpuFeaturesUserSetting, CpuFeaturesUserSettingSize);
  }
  if (CpuPolicyConfiguration->Policy.CpuDcuPrefetcherEnable) {
    AddCpuFeaturesUserSetting (CPU_FEATURE_DCU_PREFETCHER, CpuFeaturesUserSetting, CpuFeaturesUserSettingSize);
  }
  if (CpuPolicyConfiguration->Policy.CpuIpPrefetcherEnable) {
    AddCpuFeaturesUserSetting (CPU_FEATURE_IP_PREFETCHER, CpuFeaturesUserSetting, CpuFeaturesUserSettingSize);
  }
  if (CpuPolicyConfiguration->Policy.CpuMlcStreamerPrefetecherEnable) {
    AddCpuFeaturesUserSetting (CPU_FEATURE_MLC_STREAMER_PREFETCHER, CpuFeaturesUserSetting, CpuFeaturesUserSettingSize);
  }
  if (CpuPolicyConfiguration->Policy.CpuMlcSpatialPrefetcherEnable) {
    AddCpuFeaturesUserSetting (CPU_FEATURE_MLC_SPATIAL_PREFETCHER, CpuFeaturesUserSetting, CpuFeaturesUserSettingSize);
  }
  if (CpuPolicyConfiguration->Policy.CpuAmpPrefetchEnable) {
    AddCpuFeaturesUserSetting (CPU_FEATURE_AMP_PREFETCH, CpuFeaturesUserSetting, CpuFeaturesUserSettingSize);
  }
  if (CpuPolicyConfiguration->Policy.CpuThreeStrikeCounterEnable) {
    AddCpuFeaturesUserSetting (CPU_FEATURE_THREE_STRIKE_COUNTER, CpuFeaturesUserSetting, CpuFeaturesUserSettingSize);
  }
  if (CpuPolicyConfiguration->Policy.CpuCStateEnable) {
    AddCpuFeaturesUserSetting (CPU_FEATURE_C_STATE, CpuFeaturesUserSetting, CpuFeaturesUserSettingSize);
  }
  if (CpuPolicyConfiguration->Policy.CpuThermalManagementEnable) {
    AddCpuFeaturesUserSetting (CPU_FEATURE_TM, CpuFeaturesUserSetting, CpuFeaturesUserSettingSize);
  }
  if (CpuPolicyConfiguration->Policy.CpuDbpfEnable) {
    AddCpuFeaturesUserSetting (CPU_FEATURE_DBP_F, CpuFeaturesUserSetting, CpuFeaturesUserSettingSize);
  }
  if (CpuPolicyConfiguration->Policy.CpuEnergyPerformanceBiasEnable) {
    AddCpuFeaturesUserSetting (CPU_FEATURE_ENERGY_PERFORMANCE_BIAS, CpuFeaturesUserSetting, CpuFeaturesUserSettingSize);
  }
  if ((CpuPolicyConfiguration->Policy.CpuIioLlcWaysBitMask > 0) ||
      (CpuPolicyConfiguration->Policy.CpuExpandedIioLlcWaysBitMask > 0) ||
      (CpuPolicyConfiguration->Policy.CpuRemoteWaysBitMask > 0) ||
      (CpuPolicyConfiguration->Policy.CpuRrqCountThreshold > 0)) {
    AddCpuFeaturesUserSetting (CPU_FEATURE_IIO_LLC_WAYS, CpuFeaturesUserSetting, CpuFeaturesUserSettingSize);
  }

  if (CpuPolicyConfiguration->Policy.CpuMtoIWa > 0) {
    AddCpuFeaturesUserSetting (CPU_FEATURE_MTOI_WA, CpuFeaturesUserSetting, CpuFeaturesUserSettingSize);
  }
  if (CpuPolicyConfiguration->Policy.AcExceptionOnSplitLockEnable) {
    AddCpuFeaturesUserSetting (CPU_FEATURE_AC_SPLIT_LOCK, CpuFeaturesUserSetting, CpuFeaturesUserSettingSize);
  }

  //
  // No setup option for the features below.
  //
  AddCpuFeaturesUserSetting (CPU_FEATURE_SNC_INIT, CpuFeaturesUserSetting, CpuFeaturesUserSettingSize);
  AddCpuFeaturesUserSetting (CPU_FEATURE_MBM_INIT, CpuFeaturesUserSetting, CpuFeaturesUserSettingSize);

  //
  // Set CPU User Setting PCD
  //
  Status = PcdSetPtrS (
             PcdCpuFeaturesSetting,
             &CpuFeaturesUserSettingSize,
             CpuFeaturesUserSetting
             );
  ASSERT_EFI_ERROR (Status);

  Handle = NULL;
  Status = gBS->InstallProtocolInterface (
                  &Handle,
                  &gEdkiiCpuFeaturesSetDoneGuid,
                  EFI_NATIVE_INTERFACE,
                  NULL
                  );
  ASSERT_EFI_ERROR (Status);

}

/**
  Entry point for the driver.

  @param[in]  ImageHandle  Image Handle.
  @param[in]  SystemTable  EFI System Table.

  @retval     EFI_SUCCESS  Function has completed successfully.
  @retval     Others       All other error conditions encountered result in an ASSERT.
**/
EFI_STATUS
EFIAPI
SiCpuInitDxeEntryPoint (
  IN EFI_HANDLE               ImageHandle,
  IN EFI_SYSTEM_TABLE         *SystemTable
  )
{
  EFI_STATUS  Status;
  VOID        *Registration;
  EFI_HANDLE  Handle;

  Status = gBS->LocateProtocol (
                  &gEfiCpuPolicyProtocolGuid,
                  NULL,
                  &mCpuPolicyConfiguration
                  );
  ASSERT_EFI_ERROR (Status);

  Status = gBS->LocateProtocol (&gEfiCpuCsrAccessGuid, NULL, &mCpuCsrAccess);
  ASSERT_EFI_ERROR (Status);

  mCpuVarDataPtr = GetCpuVarData ();

  Status = GetMicrocodePatchInfo ();
  ASSERT_EFI_ERROR (Status);

  //
  // Configure processors with three-phase architecture
  //
  Status = ProcessorConfiguration ();
  if (EFI_ERROR (Status)) {
    return Status;
  }

  PostProcessorConfiguration ();

  Handle = NULL;
  Status = gBS->InstallProtocolInterface (
                  &Handle,
                  &gCpuConfigContextReadyProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  NULL
                  );
  ASSERT_EFI_ERROR (Status);

  //
  // Install notification callback on DxeSmmReadyToLock Protocol
  //
  EfiCreateProtocolNotifyEvent (
    &gEfiDxeSmmReadyToLockProtocolGuid,
    TPL_NOTIFY,
    DxeSmmReadyToLockEventNotify,
    NULL,
    &Registration
    );

  //
  // Set PcdCpuFeaturesSetting and install gEdkiiCpuFeaturesSetDoneGuid
  // protocol which is depended on by CpuFeaturesDxe driver.
  //
  SetPcdCpuFeaturesSetting ();

  return EFI_SUCCESS;
}
