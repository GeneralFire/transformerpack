/** @file
  Implementation of Silicon Init PEI module.
  This PEIM builds CPU HOB.

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

#include "SiCpuInitPei.h"

STATIC EFI_PEI_PPI_DESCRIPTOR           mPeiCpuFeaturesSetDonePpiList = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEdkiiCpuFeaturesSetDoneGuid,
  NULL
};

STATIC EFI_PEI_NOTIFY_DESCRIPTOR        mNotifyList[] = {
  {
    (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK),
    &gEfiEndOfPeiSignalPpiGuid,
    BuildCpuInfoHob
  },
  {
    (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK),
    &gPeiPostScriptTablePpiGuid,
    S3RestoreMsrCallback
  },
  {
    (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK),
    &gPeiCpuLatePolicyPpiGuid,
    PeiCpuLatePolicyReadyCallback
  },
  {
    (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_DISPATCH | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
    &gEfiPeiMpServicesPpiGuid,
    PeiMpServicesReadyCallback
  }
};

typedef struct {
  CPU_PRE_MEM_POLICY            *CpuPolicy;
} PEI_CPU_THREAD_INIT_INFO;

/**
  Programs the MSRs for required Cpu MSRs on every thread

  This function follows EFI_AP_PROCEDURE prototype.

  @param Buffer Pointer to CPU Pre Mem policy.

**/
VOID
EFIAPI
RestoreCpuMsrs (
  IN OUT VOID  *Buffer
  )
{
  CPU_PRE_MEM_POLICY                    *CpuPolicy;
  MSR_CLOCK_CST_CONFIG_CONTROL_REGISTER CstConfig;
  MSR_IA32_FEATURE_CONTROL_REGISTER     FeatureControl;

  CpuPolicy = (CPU_PRE_MEM_POLICY *) Buffer;

  if (CpuPolicy->ProcessorMsrLockControl != 0){
    //
    // Program IA32_FEATURE_CONTROL MSR lock bit for this thread.
    //
    FeatureControl.Uint32 = (UINT32)AsmReadMsr64 (MSR_IA32_FEATURE_CONTROL);
    if (FeatureControl.Bits.Lock == 0) {
      FeatureControl.Bits.Lock = 1;
      AsmWriteMsr64 (MSR_IA32_FEATURE_CONTROL, FeatureControl.Uint32);
    }
  }

  if (CpuPolicy->ProcessorMsrPkgCstConfigControlLock != 0) {
    //
    // Program PKG_CST_CONFIG_CONTROL MSR lock bit for this thread.
    //
    CstConfig.Uint32 = (UINT32)AsmReadMsr32 (MSR_CLOCK_CST_CONFIG_CONTROL);
    if (CstConfig.Bits.Lock == 0) {
      CstConfig.Bits.Lock = 1;
      AsmWriteMsr32 (MSR_CLOCK_CST_CONFIG_CONTROL, CstConfig.Uint32);
    }
  }
}

/**
  Programs MSRs on S3 resume on every package, instead of every thread. Meant
  for package level MSRs that introduce race condition issues in multi-thread
  parallel execution.

  This function follows EFI_AP_PROCEDURE prototype.

  @param Buffer Pointer to CPU Pre Mem policy.

**/
VOID
EFIAPI
RestoreCpuPkgMsrs (
  IN OUT VOID  *Buffer
  )
{
  MSR_MISC_PWR_MGMT_REGISTER        MiscPwrMsr;
  //
  // Program MISC_PWR_MGMT MSR lock bit for this thread
  //
  MiscPwrMsr.Uint64 = AsmReadMsr64 (MSR_MISC_PWR_MGMT);
  if (MiscPwrMsr.Bits.Lock == 0) {
    MiscPwrMsr.Bits.Lock = 1;
    AsmWriteMsr64 (MSR_MISC_PWR_MGMT, MiscPwrMsr.Uint64);
  }
}

/**

  Restores required MSR settings on S3 Resume. This is used for MSRs
  that must be set late in S3 resume such as lock bits or MSRs that cannot
  be handled by WriteRegisterTable restore method.

  @param PeiServices   - Pointer to PEI Services Table.

  @retval EFI_SUCCESS in all cases.

**/
EFI_STATUS
EFIAPI
S3RestoreCpuMsrs (
  IN      EFI_PEI_SERVICES          **PeiServices
  )
{
  EFI_STATUS                 Status;
  EFI_PEI_MP_SERVICES_PPI    *PeiMpServices;
  CPU_PRE_MEM_POLICY         *CpuPolicy;
  EFI_GUID                   gUniversalDataGuid = IIO_UNIVERSAL_DATA_GUID;
  IIO_UDS                    *IioUds;
  EFI_HOB_GUID_TYPE          *GuidHob;
  UINT8                      SocketNumber = 0;
  UINT8                      Index;
  UINTN                      NumberOfProcessors;
  UINTN                      NumberEnabledProcessors;
  UINT16                     PackageDoneBitmap;
  EFI_PROCESSOR_INFORMATION  ProcInfo;
  CPU_CSR_ACCESS_VAR         *CpuCsrAccessVarPtr;

  CpuPolicy = GetCpuPreMemPolicy ();

  GuidHob = GetFirstGuidHob (&gUniversalDataGuid);
  ASSERT (GuidHob != NULL);
  if(GuidHob == NULL) {
    SocketNumber = MAX_SOCKET;
  } else {
    IioUds  = GET_GUID_HOB_DATA(GuidHob);
    CpuCsrAccessVarPtr = GetSysCpuCsrAccessVar ();
    for (Index = 0; Index < MAX_SOCKET; Index++) {
      if(CpuCsrAccessVarPtr->socketPresentBitMap & (1 << Index)){
        SocketNumber++;
      }
    }
  }

  Status = (*PeiServices)->LocatePpi(
                      PeiServices,
                      &gEfiPeiMpServicesPpiGuid,
                      0,
                      NULL,
                      &PeiMpServices
                      );

  if(!EFI_ERROR(Status)){
    //
    // Set lock bits on Cpu MSRs on BSP, then APs
    //
    RestoreCpuMsrs((VOID*)CpuPolicy);
    Status = PeiMpServices->StartupAllAPs (
              PeiServices,
              PeiMpServices,
              RestoreCpuMsrs,
              FALSE,
              0,
              (VOID*)CpuPolicy
              );
    if (EFI_ERROR(Status)) {
      DEBUG((EFI_D_ERROR, "Error: Not able to relock Processor MSRs.  Status: %r\n", Status));
    }

    PackageDoneBitmap = 0;
    PeiMpServices->GetNumberOfProcessors(
                  PeiServices,
                  PeiMpServices,
                  &NumberOfProcessors,
                  &NumberEnabledProcessors
                  );
    for(Index = 0; Index < NumberOfProcessors; Index++) {
      PeiMpServices->GetProcessorInfo(
                      PeiServices,
                      PeiMpServices,
                      Index,
                      &ProcInfo
                      );
      //
      // Only call RestoreCpuPkgMsrs on each socket one time
      //
      if(!(PackageDoneBitmap & (1 << ProcInfo.Location.Package)) && (ProcInfo.StatusFlag & PROCESSOR_ENABLED_BIT)){
        PackageDoneBitmap |= (1 << ProcInfo.Location.Package);
        SocketNumber--;
        if(ProcInfo.StatusFlag & PROCESSOR_AS_BSP_BIT) {
          RestoreCpuPkgMsrs((VOID *)CpuPolicy);
        } else {
          PeiMpServices->StartupThisAP(PeiServices,
                                    PeiMpServices,
                                    RestoreCpuPkgMsrs,
                                    Index,
                                    0,
                                    (VOID *)CpuPolicy
                                    );
        }
        //
        // If we have reached the last socket, break loop
        //
        if(SocketNumber == 0){
          break;
        }
      }
    }
  }

  return EFI_SUCCESS;
}

VOID
ApicTprUpdateMessageInitialize (
  VOID
  )
{
  MSR_PIC_MSG_CONTROL_REGISTER ApicTprMsr;

  //
  // APIC_TPR_UPD_MSG_DISABLE:
  // When set to 1 (which is the default at reset), no APIC messages (TPR updates) will be
  // sent. When set to 0, APIC messages (TPR updates) will be sent.
  // Writes from 1 to 0 are allowed, but writes of 1 are ignored. BIOS must write 0 to this
  // bit for each thread in each package before any Local APIC mode changes occur.
  // Note: If BIOS does not clear this bit, OS initialization of interrupts will fail.
  //
  // So, always set APIC_TPR_UPD_MSG_DISABLE to 0.
  //
  ApicTprMsr.Uint64 = AsmReadMsr64 (MSR_PIC_MSG_CONTROL);
  ApicTprMsr.Bits.TprMsgOff = 0;
  AsmWriteMsr64 (MSR_PIC_MSG_CONTROL, ApicTprMsr.Uint64);
}

/**
  Programs the MSRs for required Cpu MSRs on every thread.

  This function follows EFI_AP_PROCEDURE prototype.

  @param Buffer Pointer to CPU thread init info.

**/
VOID
EFIAPI
LatePeiCpuThreadInit (
  IN OUT VOID  *Buffer
  )
{
  PEI_CPU_THREAD_INIT_INFO          *CpuThreadInitInfo;
  UINT64                            MsrData;
  ICX_MSR_FEATURE_TUNING_1_REGISTER FeatureTuning1;
  UINT32                            LlcPrefetchDis;

  CpuThreadInitInfo = (PEI_CPU_THREAD_INIT_INFO *) Buffer;

  if (IsCpuAndRevision (CPU_SKX, REV_ALL) || IsCpuAndRevision (CPU_CLX, REV_ALL) || IsCpuAndRevision (CPU_CPX, REV_ALL)) {
    //
    // Set LLC Prefetch with requested value. Only supported on SKX H0 or later, on patch 0x05+
    // LLC Prefetch is supported on CLX/CPX without a patch.
    //
    LlcPrefetchDis = (CpuThreadInitInfo->CpuPolicy->LlcPrefetchEnable != 0) ? 0 : B_VIRTUAL_MSR_LLC_PREFETCH_DISABLE;

    if (IsCpuAndRevision (CPU_CPX, REV_ALL)) {
      MsrData = AsmReadMsr64 (MSR_VIRTUAL_MSR_LLC_PREFETCH);
      MsrData &= (UINT64) (~B_VIRTUAL_MSR_LLC_PREFETCH_DISABLE);
      MsrData |= (UINT64) LlcPrefetchDis;
    } else {
     //
     // SKX/CLX doesn't do read-modify-write to MSR 0x972 (to prevent GP fault)
     //
      MsrData = LlcPrefetchDis;
    }
    AsmWriteMsr64 (MSR_VIRTUAL_MSR_LLC_PREFETCH, MsrData);
  } else if ((IsCpuAndRevision (CPU_ICXSP, REV_ALL)) || (IsCpuAndRevision (CPU_SPRSP, REV_ALL))) {
    //
    // Feature Tuning MSR programmed here is only available on patch 0x16 or later
    // for ICX R0.  This MSR is available inherently for ICX L0 and later.
    //
    if (!IsSimicsEnvironment()) {
      FeatureTuning1.Uint64 = AsmReadMsr64 (ICX_MSR_FEATURE_TUNING_1);
      FeatureTuning1.Bits.DisableLlpref = ((CpuThreadInitInfo->CpuPolicy->LlcPrefetchEnable != 0) ? 0 : 1);
      AsmWriteMsr64 (ICX_MSR_FEATURE_TUNING_1, FeatureTuning1.Uint64);
    }
  }

  ApicTprUpdateMessageInitialize ();
}

/**

  Performs any PEI CPU Initialization needed in PEI phase that
  requires multi-threaded execution. This is executed on normal
  boots and S3 resume.

  @param PeiServices   - Pointer to PEI Services Table.

  @retval EFI_SUCCESS in all cases.

**/
EFI_STATUS
EFIAPI
LatePeiCpuInit (
  IN      EFI_PEI_SERVICES          **PeiServices
  )
{
  EFI_STATUS               Status;
  EFI_PEI_MP_SERVICES_PPI  *PeiMpServices;
  PEI_CPU_THREAD_INIT_INFO CpuThreadInitInfo;

  Status = (*PeiServices)->LocatePpi(
                      PeiServices,
                      &gEfiPeiMpServicesPpiGuid,
                      0,
                      NULL,
                      &PeiMpServices
                      );

  if(!EFI_ERROR(Status)){

    CpuThreadInitInfo.CpuPolicy = GetCpuPreMemPolicy ();

    //
    // Perform late Cpu Pei Init on BSP, then APs
    //
    LatePeiCpuThreadInit ((VOID*)&CpuThreadInitInfo);
    Status = PeiMpServices->StartupAllAPs (
                              PeiServices,
                              PeiMpServices,
                              LatePeiCpuThreadInit,
                              FALSE,
                              0,
                              (VOID*)&CpuThreadInitInfo
                              );
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "Error: Not able to execute Late PEI thread init.  Status: %r\n", Status));
    }
  }

  return EFI_SUCCESS;
}

/**
  A callback function to build CPUs information Guided HOB.

  This function is a callback function to build one Guided HOB to pass
  CPUs' information to DXE phase.

  @param PeiServices        Pointer to PEI Services Table
  @param NotifyDescriptor   Address if the notification descriptor data structure
  @param Ppi                Address of the PPI that was installed

**/
EFI_STATUS
EFIAPI
BuildCpuInfoHob(
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
)
{
  EFI_STATUS                     Status;
  EFI_BOOT_MODE                  BootMode;
  UINT32                         RegEax;
  UINT8                          PhysicalAddressBits;

  Status = PeiServicesGetBootMode (&BootMode);
  ASSERT_EFI_ERROR(Status);

  //
  // We do not build CPU Hobs on S3 path, because the data are only needed by DXE drivers.
  //
  if (BootMode != BOOT_ON_S3_RESUME) {
    //
    // Local APIC range
    //
    BuildResourceDescriptorHob (
      EFI_RESOURCE_MEMORY_MAPPED_IO,
      (EFI_RESOURCE_ATTRIBUTE_PRESENT |
       EFI_RESOURCE_ATTRIBUTE_INITIALIZED |
       EFI_RESOURCE_ATTRIBUTE_UNCACHEABLE),
      0xFEC80000,
      SIZE_512KB
      );
    BuildMemoryAllocationHob (
      0xFEC80000,
      SIZE_512KB,
      EfiMemoryMappedIO
      );

    //
    // Build CPU memory space and IO space hob
    //
    AsmCpuid(0x80000000, &RegEax, NULL, NULL, NULL);
    if (RegEax >= 0x80000008) {
      AsmCpuid(0x80000008, &RegEax, NULL, NULL, NULL);
      PhysicalAddressBits = (UINT8)RegEax;
    } else {
      PhysicalAddressBits = 36;
    }

    //
    // Create a CPU hand-off information
    //
    BuildCpuHob (PhysicalAddressBits, 16);
  }
  return EFI_SUCCESS;
}

/**
  Call into Cpu library to restore the necessary MSR settings late in S3 resume.
  This callback is used for MSRs that must be set late in S3 resume such as lock bits
  or features that cannot be handled by WriteRegisterTable restore method.

  @param PeiServices    Pointer to PEI Services Table.
  @param NotifyDesc     Pointer to the descriptor for the Notification
                        event that caused this function to execute.
  @param Ppi            Pointer the the Ppi data associated with this function.

  @retval EFI_SUCCESS   S3 Cpu MSR restore call completed successfully.

**/
EFI_STATUS
EFIAPI
S3RestoreMsrCallback (
  IN      EFI_PEI_SERVICES          **PeiServices,
  IN      EFI_PEI_NOTIFY_DESCRIPTOR *NotifyDesc,
  IN      VOID                      *Ppi
  )
{
  //
  // Restore MSR settings required late in S3 resume process
  //
  return S3RestoreCpuMsrs (PeiServices);
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
  PPI which is depended on by CpuFeaturesPei driver.

**/
VOID
SetPcdCpuFeaturesSetting (
  VOID
  )
{
  EFI_STATUS                    Status;
  CPU_LATE_POLICY_PPI           *CpuLatePolicy;
  UINT8                         *CpuFeaturesUserSetting;
  UINTN                         CpuFeaturesUserSettingSize;

  CpuLatePolicy = NULL;
  Status = PeiServicesLocatePpi (
             &gPeiCpuLatePolicyPpiGuid,
             0,
             NULL,
             (VOID **) &CpuLatePolicy
             );
  ASSERT_EFI_ERROR (Status);
  ASSERT (CpuLatePolicy != NULL);
  if (CpuLatePolicy == NULL) {
    return;
  }

  if (!CpuLatePolicy->Policy.RunCpuPpmInPei) {
    return;
  }

  DEBUG ((DEBUG_INFO, "SetPcdCpuFeaturesSetting in PEI\n"));

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
  if (CpuLatePolicy->Policy.CpuTStateEnable) {
    AddCpuFeaturesUserSetting (CPU_FEATURE_ACPI, CpuFeaturesUserSetting, CpuFeaturesUserSettingSize);
    Status = PcdSet8S (PcdCpuClockModulationDutyCycle, CpuLatePolicy->Policy.CpuClockModulationDutyCycle);
    ASSERT_EFI_ERROR (Status);
  }
  if (CpuLatePolicy->Policy.CpuAesEnable) {
    AddCpuFeaturesUserSetting (CPU_FEATURE_AESNI, CpuFeaturesUserSetting, CpuFeaturesUserSettingSize);
  }
  if (CpuLatePolicy->Policy.CpuFastStringEnable) {
    AddCpuFeaturesUserSetting (CPU_FEATURE_FASTSTRINGS, CpuFeaturesUserSetting, CpuFeaturesUserSettingSize);
  }
  if (CpuLatePolicy->Policy.CpuMaxCpuidValueLimitEnable) {
    AddCpuFeaturesUserSetting (CPU_FEATURE_LIMIT_CPUID_MAX_VAL, CpuFeaturesUserSetting, CpuFeaturesUserSettingSize);
  }
  if (CpuLatePolicy->Policy.CpuMachineCheckEnable) {
    AddCpuFeaturesUserSetting (CPU_FEATURE_MCE, CpuFeaturesUserSetting, CpuFeaturesUserSettingSize);
    AddCpuFeaturesUserSetting (CPU_FEATURE_MCA, CpuFeaturesUserSetting, CpuFeaturesUserSettingSize);
    AddCpuFeaturesUserSetting (CPU_FEATURE_MCG_CTL, CpuFeaturesUserSetting, CpuFeaturesUserSettingSize);
  }
  if (CpuLatePolicy->Policy.CpuMonitorMwaitEnable) {
    AddCpuFeaturesUserSetting (CPU_FEATURE_MWAIT, CpuFeaturesUserSetting, CpuFeaturesUserSettingSize);
  }
  if (CpuLatePolicy->Policy.CpuVtEnable) {
    AddCpuFeaturesUserSetting (CPU_FEATURE_VMX, CpuFeaturesUserSetting, CpuFeaturesUserSettingSize);
  }
  if (CpuLatePolicy->Policy.CpuLtEnable) {
    AddCpuFeaturesUserSetting (CPU_FEATURE_SMX, CpuFeaturesUserSetting, CpuFeaturesUserSettingSize);
  }
  if (CpuLatePolicy->Policy.CpuX2ApicEnable) {
    AddCpuFeaturesUserSetting (CPU_FEATURE_X2APIC, CpuFeaturesUserSetting, CpuFeaturesUserSettingSize);
  }
  if (CpuLatePolicy->Policy.CpuEistEnable) {
    AddCpuFeaturesUserSetting (CPU_FEATURE_EIST, CpuFeaturesUserSetting, CpuFeaturesUserSettingSize);
  }
  if (CpuLatePolicy->Policy.CpuPpinControlEnable) {
    //
    // It is Enable and Unlock, otherwise will be Disable and Lock.
    //
    AddCpuFeaturesUserSetting (CPU_FEATURE_PPIN, CpuFeaturesUserSetting, CpuFeaturesUserSettingSize);
  }

  //
  // There is no implementation for the features below in UefiCpuPkg CpuCommonFeaturesLib.
  //
  if (CpuLatePolicy->Policy.CpuPeciDownstreamWriteEnable) {
    AddCpuFeaturesUserSetting (CPU_FEATURE_PECI_DOWN_STREAM, CpuFeaturesUserSetting, CpuFeaturesUserSettingSize);
  }
  if (!CpuLatePolicy->Policy.CpuL1NextPagePrefetcherDisable) {
    AddCpuFeaturesUserSetting (CPU_FEATURE_L1_NEXT_PAGE_PREFETCHER, CpuFeaturesUserSetting, CpuFeaturesUserSettingSize);
  }
  if (CpuLatePolicy->Policy.CpuDcuPrefetcherEnable) {
    AddCpuFeaturesUserSetting (CPU_FEATURE_DCU_PREFETCHER, CpuFeaturesUserSetting, CpuFeaturesUserSettingSize);
  }
  if (CpuLatePolicy->Policy.CpuIpPrefetcherEnable) {
    AddCpuFeaturesUserSetting (CPU_FEATURE_IP_PREFETCHER, CpuFeaturesUserSetting, CpuFeaturesUserSettingSize);
  }
  if (CpuLatePolicy->Policy.CpuMlcStreamerPrefetecherEnable) {
    AddCpuFeaturesUserSetting (CPU_FEATURE_MLC_STREAMER_PREFETCHER, CpuFeaturesUserSetting, CpuFeaturesUserSettingSize);
  }
  if (CpuLatePolicy->Policy.CpuMlcSpatialPrefetcherEnable) {
    AddCpuFeaturesUserSetting (CPU_FEATURE_MLC_SPATIAL_PREFETCHER, CpuFeaturesUserSetting, CpuFeaturesUserSettingSize);
  }
  if (CpuLatePolicy->Policy.CpuAmpPrefetchEnable) {
    AddCpuFeaturesUserSetting (CPU_FEATURE_AMP_PREFETCH, CpuFeaturesUserSetting, CpuFeaturesUserSettingSize);
  }
  if (CpuLatePolicy->Policy.CpuThreeStrikeCounterEnable) {
    AddCpuFeaturesUserSetting (CPU_FEATURE_THREE_STRIKE_COUNTER, CpuFeaturesUserSetting, CpuFeaturesUserSettingSize);
  }
  if (CpuLatePolicy->Policy.CpuCStateEnable) {
    AddCpuFeaturesUserSetting (CPU_FEATURE_C_STATE, CpuFeaturesUserSetting, CpuFeaturesUserSettingSize);
  }
  if (CpuLatePolicy->Policy.CpuThermalManagementEnable) {
    AddCpuFeaturesUserSetting (CPU_FEATURE_TM, CpuFeaturesUserSetting, CpuFeaturesUserSettingSize);
  }
  if (CpuLatePolicy->Policy.CpuDbpfEnable) {
    AddCpuFeaturesUserSetting (CPU_FEATURE_DBP_F, CpuFeaturesUserSetting, CpuFeaturesUserSettingSize);
  }
  if (CpuLatePolicy->Policy.CpuEnergyPerformanceBiasEnable) {
    AddCpuFeaturesUserSetting (CPU_FEATURE_ENERGY_PERFORMANCE_BIAS, CpuFeaturesUserSetting, CpuFeaturesUserSettingSize);
  }
  if ((CpuLatePolicy->Policy.CpuIioLlcWaysBitMask > 0) ||
      (CpuLatePolicy->Policy.CpuExpandedIioLlcWaysBitMask > 0) ||
      (CpuLatePolicy->Policy.CpuRemoteWaysBitMask > 0) ||
      (CpuLatePolicy->Policy.CpuRrqCountThreshold > 0)) {
    AddCpuFeaturesUserSetting (CPU_FEATURE_IIO_LLC_WAYS, CpuFeaturesUserSetting, CpuFeaturesUserSettingSize);
  }

  if (CpuLatePolicy->Policy.CpuMtoIWa > 0) {
    AddCpuFeaturesUserSetting (CPU_FEATURE_MTOI_WA, CpuFeaturesUserSetting, CpuFeaturesUserSettingSize);
  }
  if (CpuLatePolicy->Policy.AcExceptionOnSplitLockEnable) {
    AddCpuFeaturesUserSetting (CPU_FEATURE_AC_SPLIT_LOCK, CpuFeaturesUserSetting, CpuFeaturesUserSettingSize);
  }

  //
  // No setup option for the features below.
  //
  AddCpuFeaturesUserSetting (CPU_FEATURE_SNC_INIT, CpuFeaturesUserSetting, CpuFeaturesUserSettingSize);
  AddCpuFeaturesUserSetting (CPU_FEATURE_MBM_INIT, CpuFeaturesUserSetting, CpuFeaturesUserSettingSize);

  //
  // Set CPU User Setting PCD.
  //
  Status = PcdSetPtrS (
             PcdCpuFeaturesSetting,
             &CpuFeaturesUserSettingSize,
             CpuFeaturesUserSetting
             );
  ASSERT_EFI_ERROR (Status);

  //
  // CpuFeaturesPei could be dispatched after this PPI is installed.
  //
  Status = PeiServicesInstallPpi (&mPeiCpuFeaturesSetDonePpiList);
  ASSERT_EFI_ERROR (Status);
}

/**
  PEI CPU late policy PPI callback.

  @param PeiServices    Pointer to PEI Services Table.
  @param NotifyDesc     Pointer to the descriptor for the Notification
                        event that caused this function to execute.
  @param Ppi            Pointer to the Ppi data associated with this function.

  @retval EFI_SUCCESS   PEI CPU late policy PPI callback completed successfully.

**/
EFI_STATUS
EFIAPI
PeiCpuLatePolicyReadyCallback (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDesc,
  IN VOID                       *Ppi
  )
{
  //
  // Set PcdCpuFeaturesSetting and install gEdkiiCpuFeaturesSetDoneGuid
  // PPI which is depended on by CpuFeaturesPei driver.
  //
  SetPcdCpuFeaturesSetting ();

  return EFI_SUCCESS;
}

/**
  Call into Cpu library to perform any PEI CPU Initialization needed
  in PEI phase that requires multi-threaded execution. This is executed
  on normal boots and S3 resume.

  @param PeiServices    Pointer to PEI Services Table.
  @param NotifyDesc     Pointer to the descriptor for the Notification
                        event that caused this function to execute.
  @param Ppi            Pointer to the Ppi data associated with this function.

  @retval EFI_SUCCESS   PEI MP Services callback completed successfully.

**/
EFI_STATUS
EFIAPI
PeiMpServicesReadyCallback (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDesc,
  IN VOID                       *Ppi
  )
{
  //
  // Hook for CPU initialization after MpServices PPI is installed
  //
  return LatePeiCpuInit(PeiServices);
}

/**
  PEI Silicon Initialization Code for CPU.

  @param[in] FileHandle         Handle of the file being invoked.
  @param[in] PeiServices        Describes the list of possible PEI Services.

  @retval EFI_SUCCESS           The function completes successfully.

**/
EFI_STATUS
EFIAPI
SiCpuInitPeiEntryPoint (
  IN  EFI_PEI_FILE_HANDLE               FileHandle,
  IN CONST EFI_PEI_SERVICES             **PeiServices
  )
{
  EFI_STATUS                Status;

  //
  // Report Status Code to indicate the start of CPU PEIM
  //
  REPORT_STATUS_CODE (
    EFI_PROGRESS_CODE,
    EFI_COMPUTING_UNIT_HOST_PROCESSOR + EFI_CU_HP_PC_POWER_ON_INIT
    );

  //
  // Register for PPI Notifications
  //
  Status = PeiServicesNotifyPpi (&mNotifyList[0]);
  ASSERT_EFI_ERROR (Status);

  //
  // Report Status Code to indicate the start of CPU PEI initialization
  //
  REPORT_STATUS_CODE (
    EFI_PROGRESS_CODE,
    EFI_COMPUTING_UNIT_HOST_PROCESSOR + EFI_CU_PC_INIT_BEGIN
    );

  return Status;
}
