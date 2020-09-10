/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2015 - 2020 Intel Corporation. <BR>

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

#include <PiPei.h>
#include <Library/HobLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Guid/FspInfo.h>
#include <Guid/PlatformInfo.h>
#include <Guid/SetupVariable.h>
#include <SysConfigSetup.h>
#include <FspUpd.h>
#include <FspmUpd.h>
#include <FspsUpd.h>
#include <Library/FspCommonLib.h>

#include <Guid/SocketVariable.h>
#include <Guid/SocketPciResourceData.h>
#if ME_SUPPORT_FLAG
#include <Guid/MeRcVariable.h>
#endif // ME_SUPPORT_FLAG

#ifdef IE_SUPPORT
#include <Guid/IeRcVariable.h>
#endif //IE_SUPPORT
#include <Platform.h>
#include <IioRegs.h>
#include <Guid/MemBootHealthGuid.h>

STATIC EFI_PEI_PPI_DESCRIPTOR       mPlatformInfoPpi = {
    EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
    &gEfiPlatformInfoGuid,
    NULL
  };

EFI_STATUS
VerifyFspMemoryInitUpdDataPointer (
  IN EFI_PEI_SERVICES  **PeiServices,
  IN FSPM_UPD          *MemoryInitUpdPtr
  )
/*++

Routine Description:

  Validate FSP Memory Init UPD Data Pointer.

Arguments:

  PeiServices:         PEI Services Table.
  MemoryInitUpdPtr: Pointer to structure to be validated.

Returns:

  !EFI_SUCCESS      - if any failure
  EFI_SUCCESS       - Successful

--*/
{
  EFI_STATUS   Status;
  Status = EFI_SUCCESS;

  if (MemoryInitUpdPtr == NULL) {
    Status = EFI_COMPROMISED_DATA;
  } else if (MemoryInitUpdPtr->FspUpdHeader.Signature != FSPM_UPD_SIGNATURE) {
    Status = EFI_COMPROMISED_DATA;
  }

  return Status;
};


EFI_STATUS
GetValidFspMemoryInitUpdDataPointer (
  IN     EFI_PEI_SERVICES    **PeiServices,
  IN OUT FSPM_UPD            **MemoryInitUpdPtr
  )
/*++

Routine Description:

  Get Valid FSP Memory Init UPD Data Pointer.

Arguments:

  PeiServices:         PEI Services Table.
  MemoryInitUpdPtr: Pointer to structure to be updated.

Returns:

  !EFI_SUCCESS      - if any failure
  EFI_SUCCESS       - Successful

--*/
{
  EFI_STATUS   Status;
  Status = EFI_SUCCESS;

  //
  // Get and verify the UPD pointer.
  //
  *MemoryInitUpdPtr = (FSPM_UPD *)GetFspMemoryInitUpdDataPointer();
  ASSERT(*MemoryInitUpdPtr != NULL);
  Status = VerifyFspMemoryInitUpdDataPointer(PeiServices,*MemoryInitUpdPtr);

  // In case of invalid UPD data pointer get it from FSP Config Region and verify.
  //
  if(Status != EFI_SUCCESS){
    *MemoryInitUpdPtr = (FSPM_UPD *) GetFspCfgRegionDataPointer();
    ASSERT(*MemoryInitUpdPtr != NULL);

    Status = VerifyFspMemoryInitUpdDataPointer(PeiServices,*MemoryInitUpdPtr);
    if(Status != EFI_SUCCESS){
       DEBUG ((EFI_D_ERROR, "Invalid Fsp Memory Init UPD data pointer!\n"));
       ASSERT_EFI_ERROR(Status);
    }else{
       DEBUG ((EFI_D_WARN, "Warning: Memory Init UPD PCDs from FSP-M, not bootloader!\n"));
    }
  }

  return Status;
};

EFI_STATUS
EFIAPI
UpdateMemBootHealthConfig (
  IN      EFI_PEI_SERVICES             **PeiServices,
  IN      MEM_BOOT_HEALTH_CONFIG       *MemBootHealthValues
  )
{
  EFI_STATUS Status = EFI_SUCCESS;

  if (FeaturePcdGet (PcdMemBootHealthFeatureSupported)) {
    MemBootHealthValues->MemBootHealthVisible = MEM_BOOT_HEALTH_ENABLE;
  } else {
    MemBootHealthValues->MemBootHealthVisible = MEM_BOOT_HEALTH_DISABLE;
  }

  MemBootHealthValues->MemBootHealthCheck = FixedPcdGet8 (PcdMemBootHealthCheck);       // 0=>Auto, 1=>Manual and 2=>Disabled Memory Boot Health Check

  MemBootHealthValues->ResetOnCriticalError = FixedPcdGet8 (PcdResetOnCriticalError);   // 0 => Dont reboot on critical error, 1 = Reboot on critical error
  //
  // Memory Boot Health check parameters
  //
  MemBootHealthValues->WarningTxDqDelayLeftEdge = FixedPcdGet8 (PcdWarningTxDqDelayLeftEdge);
  MemBootHealthValues->WarningTxDqDelayRightEdge = FixedPcdGet8 (PcdWarningTxDqDelayRightEdge);
  MemBootHealthValues->WarningTxVrefLeftEdge = FixedPcdGet8 (PcdWarningTxVrefLeftEdge);
  MemBootHealthValues->WarningTxVrefRightEdge = FixedPcdGet8 (PcdWarningTxVrefRightEdge);
  MemBootHealthValues->WarningRxDqsDelayLeftEdge = FixedPcdGet8 (PcdWarningRxDqsDelayLeftEdge);
  MemBootHealthValues->WarningRxDqsDelayRightEdge = FixedPcdGet8 (PcdWarningRxDqsDelayRightEdge);
  MemBootHealthValues->WarningRxVrefLeftEdge = FixedPcdGet8 (PcdWarningRxVrefLeftEdge);
  MemBootHealthValues->WarningRxVrefRightEdge = FixedPcdGet8 (PcdWarningRxVrefRightEdge);

  MemBootHealthValues->CriticalTxDqDelayLeftEdge = FixedPcdGet8 (PcdCriticalTxDqDelayLeftEdge);
  MemBootHealthValues->CriticalTxDqDelayRightEdge = FixedPcdGet8 (PcdCriticalTxDqDelayRightEdge);
  MemBootHealthValues->CriticalTxVrefLeftEdge = FixedPcdGet8 (PcdCriticalTxVrefLeftEdge);
  MemBootHealthValues->CriticalTxVrefRightEdge = FixedPcdGet8 (PcdCriticalTxVrefRightEdge);
  MemBootHealthValues->CriticalRxDqsDelayLeftEdge = FixedPcdGet8 (PcdCriticalRxDqsDelayLeftEdge);
  MemBootHealthValues->CriticalRxDqsDelayRightEdge = FixedPcdGet8 (PcdCriticalRxDqsDelayRightEdge);
  MemBootHealthValues->CriticalRxVrefLeftEdge = FixedPcdGet8 (PcdCriticalRxVrefLeftEdge);
  MemBootHealthValues->CriticalRxVrefRightEdge = FixedPcdGet8 (PcdCriticalRxVrefRightEdge);

  return Status;
}


/**

  Updates SystemConfiguration structure with PCDs of MEM_INIT_UPD and VPD.

  @param [in]     PeiServices:         PEI Services Table.
  @param [in,out] SystemConfiguration: Pointer to structure to be updated.
  @param [in,out] PchSetup:            Pointer to PCH configuration structure
  @param [in,out] MemoryConfig:        Pointer to memory configuration structure

  @retval   !EFI_SUCCESS      - if any failure
  @retval   EFI_SUCCESS       - Successful

**/
EFI_STATUS
EFIAPI
UpdateSysConfig (
  IN      EFI_PEI_SERVICES             **PeiServices,
  IN OUT  SYSTEM_CONFIGURATION         *SystemConfiguration,
  IN OUT  PCH_SETUP                    *PchSetup,
  IN OUT  SOCKET_MEMORY_CONFIGURATION  *MemoryConfig
  )
{
  EFI_STATUS   Status;
  FSPM_UPD       *MemoryInitUpdPtr = NULL;

  Status = EFI_SUCCESS;

  // >>> dgucks: TODO: check if overwriting is still required <<< dgucks
  SystemConfiguration->serialDebugMsgLvl = 3;
  //
  // Get the UPD pointer.
  //
  Status = GetValidFspMemoryInitUpdDataPointer(PeiServices,&MemoryInitUpdPtr);
  ASSERT(MemoryInitUpdPtr != NULL);

  if(Status == EFI_SUCCESS){
    //
    // Print PcdCustomerRevision in FSP
    //
    DEBUG((EFI_D_INFO, "CustomerRevision: %a\n", MemoryInitUpdPtr->FspmConfig.CustomerRevision));

    DEBUG((EFI_D_INFO, "Updating Policies with Memory Init UPD PCDs...\n"));

    //
    // Configure MRC and early silicon init
    //

    //SOR TODO: update SystemConfiguration for SNR
    /*
    SystemConfiguration->SmmSize = MemoryInitUpdPtr->PcdSmmTsegSize;

    SystemConfiguration->MrcDebugMsg = MemoryInitUpdPtr->PcdMrcDebugMsg;
    SystemConfiguration->FastBoot = MemoryInitUpdPtr->PcdFastBoot;

    SystemConfiguration->MrcRmtSupport = MemoryInitUpdPtr->PcdMrcRmtSupport;
    SystemConfiguration->MrcRmtCpgcExpLoopCntValue = MemoryInitUpdPtr->PcdMrcRmtCpgcExpLoopCntValue;
    SystemConfiguration->MrcRmtCpgcNumBursts = MemoryInitUpdPtr->PcdMrcRmtCpgcNumBursts;
    */
    /*if (MemoryInitUpdPtr->PcdEccSupport == 0) {
      SystemConfiguration->ECCSupport        = 0;
      SystemConfiguration->EnableParityCheck = 0;
    } else {
      SystemConfiguration->ECCSupport        = 1;
      SystemConfiguration->EnableParityCheck = 1;
    }*/
  }

  //
  // Configure southcluster
  //
  //SOR: update Iqat for SNR SystemConfiguration
  //SystemConfiguration->Iqat = MemoryInitUpdPtr->PcdEnableIQAT;

  //SOR: update SATA0/1 enable for SNR SystemConfiguration
  //PchSetup->PchSata[0] = MemoryInitUpdPtr->PcdEnableSATA0;
  //PchSetup->PchSata[1] = MemoryInitUpdPtr->PcdEnableSATA1;
  // PchSetup->PchSata[0] = 0x01; amos
  // PchSetup->PchSata[1] = 0x01;
  // PchSetup->PchSata[2] = 0x01;

  if (FixedPcdGetBool (PcdRmtAdvancedTraining) == RMT_ON_ADVANCED_TRAINING_DISABLE ) {
    MemoryConfig->RmtOnAdvancedTraining = RMT_ON_ADVANCED_TRAINING_DISABLE;
  } else {
    MemoryConfig->RmtOnAdvancedTraining = RMT_ON_ADVANCED_TRAINING_ENABLE;
  }

  if (FixedPcdGet8 (PcdEnforcePorDefault) == ENFORCE_POR_DIS) {
    MemoryConfig->EnforcePOR = ENFORCE_POR_DIS;
  } else {
    MemoryConfig->EnforcePOR = ENFORCE_POR_EN;
  }

  MemoryConfig->EnforcePopulationPor = FixedPcdGet8 (PcdEnforcePopulationPorDefault);

  //
  // Multi-Threaded MRC
  //
  if (FixedPcdGetBool (PcdMrcMultiThreadedDefault) == TRUE) {
    MemoryConfig->MultiThreaded = MULTI_THREADED_ENABLE;
  } else {
    MemoryConfig->MultiThreaded = MULTI_THREADED_DISABLE;
  }

  //
  // Setup option for fast boot
  //
  if (FixedPcdGet8 (PcdMrcFastBootDefault) == FAST_BOOT_ENABLE) {
    MemoryConfig->AttemptFastBoot = FAST_BOOT_ENABLE;
  } else {
    MemoryConfig->AttemptFastBoot = FAST_BOOT_DISABLE;
  }

  //
  // Setup option for fast cold boot
  //
  if (FixedPcdGetBool (PcdMrcFastColdBootDefault) == TRUE) {
    MemoryConfig->AttemptFastBootCold = FAST_BOOT_COLD_ENABLE;
  } else {
    MemoryConfig->AttemptFastBootCold = FAST_BOOT_COLD_DISABLE;
  }

  //
  // Setup option for Built in RMT
  //
  MemoryConfig->EnableBiosSsaRMT = FixedPcdGet8 (PcdSsaBuiltInRmtEnable);

  //
  // Setup option for RMT Loader
  //
  MemoryConfig->EnableBiosSsaLoader = FixedPcdGet8 (PcdSsaLoaderEnable);

  //
  // Setup option for Legacy RMT
  //
  MemoryConfig->LegacyRmt = FixedPcdGet8 (PcdLegacyRmtEnable);

  //
  // Read / Write 2TCLK preamble
  //

  MemoryConfig->WritePreamble = FixedPcdGet8 (PcdMrcWritePreambleTclkDefault);
  MemoryConfig->ReadPreamble = FixedPcdGet8 (PcdMrcReadPreambleTclkDefault);

  //
  // Rx DFE options
  //

  MemoryConfig->RxDfe = FixedPcdGet8 (PcdMrcRxDfeDefault);

  //
  // Rx OdtEn options
  //
  MemoryConfig->RxOdtEn = FixedPcdGet8 (PcdMrcMcOdtDefault);

  //
  // TX Rise/Fall Slew Rate options
  //

  MemoryConfig->TxRiseFallSlewRate = FixedPcdGet8 (PcdMrcTxRfSlewRateDefault);

  if (FixedPcdGetBool (PcdMrcPmemMemHoleDefault) == TRUE) {
    MemoryConfig->AppDirectMemoryHole = PMEM_MEM_HOLE_ENABLE;
  } else {
    MemoryConfig->AppDirectMemoryHole = PMEM_MEM_HOLE_DISABLE;
  }

  MemoryConfig->FastGoConfig = FixedPcdGet8 (PcdMrcCrQosConfigDefault);

  //
  // Command Normalization
  //
  MemoryConfig->CmdNormalization = FixedPcdGet8 (PcdMrcCmdNormalizationTrainingEnable);
  //
  // Roundtrip Optimization
  //
  MemoryConfig->RoundTripLatency = FixedPcdGet8 (PcdMrcRoundTripOptimizationTrainingEnable);


  return Status;

}

/**

  Initialize POC register by Variable.

  @param TODO: update params

  @retval EFI_SUCCESS  -  Success.

**/
EFI_STATUS
UpdatePlatformInfo (
  IN OUT  EFI_FSP_INFO                       *FspInfo,
  IN OUT  EFI_PLATFORM_INFO                  *PlatformInfo,
  IN OUT  SOCKET_PROCESSORCORE_CONFIGURATION *SocketProcessorCoreConfig,
  IN OUT  SOCKET_IIO_CONFIGURATION           *SocketIioConfig
  )
{
  FSPM_UPD       *FspmUpdDataPtr;

  FspInfo->FspReservedMemoryLength = PcdGet32 (PcdFspReservedMemoryLength);
  FspmUpdDataPtr = (FSPM_UPD *) GetFspMemoryInitUpdDataPointer ();
  FspInfo->FspBootLoaderTolumSize = FspmUpdDataPtr->FspmArchUpd.BootLoaderTolumSize;
  PlatformInfo->BoardId = 0x14; // TypeSnrSvp as described in PlatformInfoTypes.h

  PlatformInfo->SysData.SysIoApicEnable       = PCH_IOAPIC;
#if MAX_SOCKET <= 4
  if (SocketIioConfig->DevPresIoApicIio[0]) {
    PlatformInfo->SysData.SysIoApicEnable |= (PC00_IOAPIC);
  }
  if (SocketIioConfig->DevPresIoApicIio[1]) {
    PlatformInfo->SysData.SysIoApicEnable |= (PC01_IOAPIC);
  }
  if (SocketIioConfig->DevPresIoApicIio[2]) {
    PlatformInfo->SysData.SysIoApicEnable |= (PC02_IOAPIC);
  }
  if (SocketIioConfig->DevPresIoApicIio[3]) {
    PlatformInfo->SysData.SysIoApicEnable |= (PC03_IOAPIC);
  }
  if (SocketIioConfig->DevPresIoApicIio[4]) {
    PlatformInfo->SysData.SysIoApicEnable |= (PC04_IOAPIC);
  }
  if (SocketIioConfig->DevPresIoApicIio[5]) {
    PlatformInfo->SysData.SysIoApicEnable |= (PC05_IOAPIC);
  }
  if (SocketIioConfig->DevPresIoApicIio[6]) {
    PlatformInfo->SysData.SysIoApicEnable |= (PC06_IOAPIC);
  }
  if (SocketIioConfig->DevPresIoApicIio[7]) {
    PlatformInfo->SysData.SysIoApicEnable |= (PC07_IOAPIC);
  }
  if (SocketIioConfig->DevPresIoApicIio[8]) {
    PlatformInfo->SysData.SysIoApicEnable |= (PC08_IOAPIC);
  }
  if (SocketIioConfig->DevPresIoApicIio[9]) {
    PlatformInfo->SysData.SysIoApicEnable |= (PC09_IOAPIC);
  }
  if (SocketIioConfig->DevPresIoApicIio[10]) {
    PlatformInfo->SysData.SysIoApicEnable |= (PC10_IOAPIC);
  }
  if (SocketIioConfig->DevPresIoApicIio[11]) {
    PlatformInfo->SysData.SysIoApicEnable |= (PC11_IOAPIC);
  }
  if (SocketIioConfig->DevPresIoApicIio[12]) {
    PlatformInfo->SysData.SysIoApicEnable |= (PC12_IOAPIC);
  }
  if (SocketIioConfig->DevPresIoApicIio[13]) {
    PlatformInfo->SysData.SysIoApicEnable |= (PC13_IOAPIC);
  }
  if (SocketIioConfig->DevPresIoApicIio[14]) {
    PlatformInfo->SysData.SysIoApicEnable |= (PC14_IOAPIC);
  }
  if (SocketIioConfig->DevPresIoApicIio[15]) {
    PlatformInfo->SysData.SysIoApicEnable |= (PC15_IOAPIC);
  }
  if (SocketIioConfig->DevPresIoApicIio[16]) {
    PlatformInfo->SysData.SysIoApicEnable |= (PC16_IOAPIC);
  }
  if (SocketIioConfig->DevPresIoApicIio[17]) {
    PlatformInfo->SysData.SysIoApicEnable |= (PC17_IOAPIC);
  }
  if (SocketIioConfig->DevPresIoApicIio[18]) {
    PlatformInfo->SysData.SysIoApicEnable |= (PC18_IOAPIC);
  }
  if (SocketIioConfig->DevPresIoApicIio[19]) {
    PlatformInfo->SysData.SysIoApicEnable |= (PC19_IOAPIC);
  }
  if (SocketIioConfig->DevPresIoApicIio[20]) {
    PlatformInfo->SysData.SysIoApicEnable |= (PC20_IOAPIC);
  }
  if (SocketIioConfig->DevPresIoApicIio[21]) {
    PlatformInfo->SysData.SysIoApicEnable |= (PC21_IOAPIC);
  }
  if (SocketIioConfig->DevPresIoApicIio[22]) {
    PlatformInfo->SysData.SysIoApicEnable |= (PC22_IOAPIC);
  }
  if (SocketIioConfig->DevPresIoApicIio[23]) {
    PlatformInfo->SysData.SysIoApicEnable |= (PC23_IOAPIC);
  }
#else
  // Enable all 32 IOxAPIC
  PlatformInfo->SysData.SysIoApicEnable = 0xFFFFFFFF;
#endif
  //
  // Check to make sure TsegSize is in range, if not use default.
  //
  if (SocketProcessorCoreConfig->TsegSize > MAX_PROCESSOR_TSEG) {
    SocketProcessorCoreConfig->TsegSize = MAX_PROCESSOR_TSEG; // if out of range make default 64M
  }
  PlatformInfo->MemData.MemTsegSize  = (0x400000 << SocketProcessorCoreConfig->TsegSize);
  PlatformInfo->MemData.MemIedSize = PcdGet32 (PcdCpuIEDRamSize);

  //
  // Minimum SMM range in TSEG should be larger than 3M
  //
  ASSERT (PlatformInfo->MemData.MemTsegSize - PlatformInfo->MemData.MemIedSize >= 0x300000);

#ifdef LT_FLAG
  PlatformInfo->MemData.MemLtMemSize  = LT_PLATFORM_DPR_SIZE + LT_DEV_MEM_SIZE;
  PlatformInfo->MemData.MemDprMemSize = LT_PLATFORM_DPR_SIZE;
#endif
  return EFI_SUCCESS;
}

/**
  Performs platform specific initialization required for the CPU to access
  the hardware associated with a SerialPortLib instance.  This function does
  not intiailzie the serial port hardware itself.  Instead, it initializes
  hardware devices that are required for the CPU to access the serial port
  hardware.  This function may be called more than once.

  @retval RETURN_SUCCESS       The platform specific initialization succeeded.
  @retval RETURN_DEVICE_ERROR  The platform specific initialization could not be completed.

**/
EFI_STATUS
EFIAPI
PlatformHobBuildLib (
  IN CONST EFI_PEI_SERVICES          **PeiServices
  )
{
  EFI_STATUS   Status;
  VOID         *HobPtr;
  SYSTEM_CONFIGURATION    *SystemConfiguration;
  PCH_SETUP               *PchSetup;
  EFI_FSP_INFO            *FspInfoHob;
  EFI_PLATFORM_INFO       *PlatformInfoHob;

  SOCKET_IIO_CONFIGURATION       *IioConfig;
  SOCKET_COMMONRC_CONFIGURATION  *CommonRcConfig;
  SOCKET_MP_LINK_CONFIGURATION   *UpiConfig;
  SOCKET_MEMORY_CONFIGURATION    *MemoryConfig;
  SOCKET_POWERMANAGEMENT_CONFIGURATION *PowerManagementConfig;
  SOCKET_PROCESSORCORE_CONFIGURATION   *SocketProcessorCoreConfiguration;
#if ME_SUPPORT_FLAG
  ME_RC_CONFIGURATION        *MeRcConfig;
#endif // ME_SUPPORT_FLAG
#ifdef IE_SUPPORT
  IE_RC_CONFIGURATION        *IeRcConfig;
#endif //IE_SUPPORT

  MEM_BOOT_HEALTH_CONFIG     *MemBootHealthConfig;
  SYSTEM_PCI_BASE_LIMITS     *SocketPciResourceConfiguration;

  Status = EFI_SUCCESS;

  IioConfig = (SOCKET_IIO_CONFIGURATION *)AllocateZeroPool(sizeof (SOCKET_IIO_CONFIGURATION));
  ASSERT(IioConfig != NULL);
  CommonRcConfig = (SOCKET_COMMONRC_CONFIGURATION *)AllocateZeroPool(sizeof (SOCKET_COMMONRC_CONFIGURATION));
  ASSERT(CommonRcConfig != NULL);
  UpiConfig = (SOCKET_MP_LINK_CONFIGURATION *)AllocateZeroPool(sizeof (SOCKET_MP_LINK_CONFIGURATION));
  ASSERT(UpiConfig != NULL);
  MemoryConfig = (SOCKET_MEMORY_CONFIGURATION *)AllocateZeroPool(sizeof (SOCKET_MEMORY_CONFIGURATION));
  ASSERT(MemoryConfig != NULL);
  PowerManagementConfig = (SOCKET_POWERMANAGEMENT_CONFIGURATION *)AllocateZeroPool(sizeof (SOCKET_POWERMANAGEMENT_CONFIGURATION));
  ASSERT(PowerManagementConfig != NULL);
  SocketProcessorCoreConfiguration = (SOCKET_PROCESSORCORE_CONFIGURATION *)AllocateZeroPool(sizeof (SOCKET_PROCESSORCORE_CONFIGURATION));
  ASSERT(SocketProcessorCoreConfiguration != NULL);
#if defined(ME_SUPPORT_FLAG) && ME_SUPPORT_FLAG
  MeRcConfig = (ME_RC_CONFIGURATION *)AllocateZeroPool(sizeof (ME_RC_CONFIGURATION));
  ASSERT(MeRcConfig != NULL);
#endif // ME_SUPPORT_FLAG
#ifdef IE_SUPPORT
  IeRcConfig = (IE_RC_CONFIGURATION *)AllocateZeroPool(sizeof (IE_RC_CONFIGURATION));
  ASSERT(IeRcConfig != NULL);
#endif //IE_SUPPORT

  MemBootHealthConfig = (MEM_BOOT_HEALTH_CONFIG *)AllocateZeroPool(sizeof (MEM_BOOT_HEALTH_CONFIG));
  ASSERT(MemBootHealthConfig != NULL);

  SocketPciResourceConfiguration = AllocateZeroPool(sizeof (*SocketPciResourceConfiguration));
  ASSERT(SocketPciResourceConfiguration != NULL);

  //
  // Allocate PCH_SETUP, SYSTEM_CONFIGURATION & PLATFORM_INFO_HOB structure.
  //
  PchSetup = (PCH_SETUP *)AllocateZeroPool(sizeof (PCH_SETUP));
  ASSERT(PchSetup != NULL);

  SystemConfiguration = (SYSTEM_CONFIGURATION *) AllocateZeroPool (sizeof (SYSTEM_CONFIGURATION));
  ASSERT (SystemConfiguration != NULL);

  FspInfoHob = (EFI_FSP_INFO *) AllocateZeroPool (sizeof (EFI_FSP_INFO));
  ASSERT (FspInfoHob != NULL);

  PlatformInfoHob = (EFI_PLATFORM_INFO *) AllocateZeroPool (sizeof (EFI_PLATFORM_INFO));
  ASSERT (PlatformInfoHob != NULL);

  //
  // Initialize PCH_SETUP/SYSTEM_CONFIGURATION structure.
  //
  SetupSysConfig (
      (EFI_PEI_SERVICES**)PeiServices,
#if defined(ME_SUPPORT_FLAG) && ME_SUPPORT_FLAG
      MeRcConfig,
#endif // ME_SUPPORT_FLAG
#ifdef IE_SUPPORT
      IeRcConfig,
#endif // IE_SUPPORT
      SystemConfiguration,
      PchSetup,
      IioConfig,
      CommonRcConfig,
      UpiConfig,
      MemoryConfig,
      PowerManagementConfig,
      SocketProcessorCoreConfiguration
      );
  //
  // Update PCH_SETUP/SYSTEM_CONFIGURATION structure.
  //
  UpdateSysConfig ((EFI_PEI_SERVICES**)PeiServices, SystemConfiguration, PchSetup, MemoryConfig);

  //
  // Update MemBootHealth Config strucutre
  //
  UpdateMemBootHealthConfig ((EFI_PEI_SERVICES**)PeiServices, MemBootHealthConfig);
  //
  // Initialize PLATFORM_INFO_HOB structure.
  //
  // TODO
  UpdatePlatformInfo (FspInfoHob, PlatformInfoHob, SocketProcessorCoreConfiguration, IioConfig);

  //
  // Build PCH_SETUP, SYSTEM_CONFIGURATION & PLATFORM_INFO_HOB HOB.
  //
  HobPtr = BuildGuidDataHob(&gPchSetupVariableGuid, PchSetup, sizeof (PCH_SETUP));
  ASSERT(HobPtr != NULL);
  DEBUG((EFI_D_ERROR, "Build PCH_SETUP HOB at 0x%08X(0x%x bytes)\n", (UINT32)HobPtr, sizeof (PCH_SETUP)));

  HobPtr = BuildGuidDataHob(&gMemBootHealthGuid, MemBootHealthConfig, sizeof (MEM_BOOT_HEALTH_CONFIG));
  ASSERT(HobPtr != NULL);
  DEBUG((EFI_D_ERROR, "Build MEM_BOOT_HEALTH_CONFIG HOB at 0x%08X(0x%x bytes)\n", (UINT32)HobPtr, sizeof (MEM_BOOT_HEALTH_CONFIG)));


  HobPtr = BuildGuidDataHob (&gEfiSetupVariableGuid, SystemConfiguration, sizeof (SYSTEM_CONFIGURATION));
  ASSERT (HobPtr != NULL);
  DEBUG ((EFI_D_ERROR, "Build SYSTEM_CONFIGURATION HOB at 0x%08X(0x%x bytes)\n", (UINT32)HobPtr, sizeof (SYSTEM_CONFIGURATION)));

  HobPtr = BuildGuidDataHob (&gFspInfoHobGuid, FspInfoHob, sizeof (EFI_FSP_INFO));
  ASSERT (HobPtr != NULL);
  DEBUG ((EFI_D_ERROR, "Build FSP_INFO_HOB HOB at 0x%08X(0x%x bytes)\n", (UINT32)HobPtr, sizeof (EFI_FSP_INFO)));

  HobPtr = BuildGuidDataHob (&gEfiPlatformInfoGuid, PlatformInfoHob, sizeof (EFI_PLATFORM_INFO));
  ASSERT (HobPtr != NULL);
  DEBUG ((EFI_D_ERROR, "Build PLATFORM_INFO_HOB HOB at 0x%08X(0x%x bytes)\n", (UINT32)HobPtr, sizeof (EFI_PLATFORM_INFO)));

  Status = (**PeiServices).InstallPpi (PeiServices, &mPlatformInfoPpi);
  ASSERT_EFI_ERROR (Status);

  // >>> dgucks TODO: check if needed here
  UpiConfig->DebugPrintLevel = 0xf;
  UpiConfig->BusRatio[0] = 0x01;
  // <<< dgucks

  HobPtr = BuildGuidDataHob(&gEfiSocketIioVariableGuid, IioConfig, sizeof (SOCKET_IIO_CONFIGURATION));
  ASSERT(HobPtr != NULL);
  DEBUG((EFI_D_ERROR, "Build SOCKET_IIO_CONFIGURATION HOB at 0x%08X(0x%x bytes)\n", (UINT32)HobPtr, sizeof (SOCKET_IIO_CONFIGURATION)));

  HobPtr = BuildGuidDataHob(&gEfiSocketCommonRcVariableGuid, CommonRcConfig, sizeof (SOCKET_COMMONRC_CONFIGURATION));
  ASSERT(HobPtr != NULL);
  DEBUG((EFI_D_ERROR, "Build SOCKET_COMMONRC_CONFIGURATION HOB at 0x%08X(0x%x bytes)\n", (UINT32)HobPtr, sizeof (SOCKET_COMMONRC_CONFIGURATION)));

  HobPtr = BuildGuidDataHob(&gEfiSocketMpLinkVariableGuid, UpiConfig, sizeof (SOCKET_MP_LINK_CONFIGURATION));
  ASSERT(HobPtr != NULL);
  DEBUG((EFI_D_ERROR, "Build SOCKET_MP_LINK_CONFIGURATION HOB at 0x%08X(0x%x bytes)\n", (UINT32)HobPtr, sizeof (SOCKET_MP_LINK_CONFIGURATION)));

  HobPtr = BuildGuidDataHob(&gEfiSocketMemoryVariableGuid, MemoryConfig, sizeof (SOCKET_MEMORY_CONFIGURATION));
  ASSERT(HobPtr != NULL);
  DEBUG((EFI_D_ERROR, "Build SOCKET_MEMORY_CONFIGURATION HOB at 0x%08X(0x%x bytes)\n", (UINT32)HobPtr, sizeof (SOCKET_MEMORY_CONFIGURATION)));

  HobPtr = BuildGuidDataHob(&gEfiSocketPowermanagementVarGuid, PowerManagementConfig, sizeof (SOCKET_POWERMANAGEMENT_CONFIGURATION));
  ASSERT(HobPtr != NULL);
  DEBUG((EFI_D_ERROR, "Build SOCKET_POWERMANAGEMENT_CONFIGURATION HOB at 0x%08X(0x%x bytes)\n", (UINT32)HobPtr, sizeof (SOCKET_POWERMANAGEMENT_CONFIGURATION)));

  HobPtr = BuildGuidDataHob(&gEfiSocketProcessorCoreVarGuid, SocketProcessorCoreConfiguration, sizeof (SOCKET_PROCESSORCORE_CONFIGURATION));
  ASSERT(HobPtr != NULL);
  DEBUG((EFI_D_ERROR, "Build SOCKET_PROCESSORCORE_CONFIGURATION HOB at 0x%08X(0x%x bytes)\n", (UINT32)HobPtr, sizeof (SOCKET_PROCESSORCORE_CONFIGURATION)));

#if ME_SUPPORT_FLAG
  HobPtr = BuildGuidDataHob(&gEfiMeRcVariableGuid, MeRcConfig, sizeof (ME_RC_CONFIGURATION));
  ASSERT(HobPtr != NULL);
  DEBUG((EFI_D_ERROR, "Build ME_RC_CONFIGURATION HOB at 0x%08X(0x%x bytes)\n", (UINT32)HobPtr, sizeof (ME_RC_CONFIGURATION)));
#endif

#ifdef IE_SUPPORT
  HobPtr = BuildGuidDataHob(&gEfiIeRcVariableGuid, IeRcConfig, sizeof (IE_RC_CONFIGURATION));
  ASSERT(HobPtr != NULL);
  DEBUG((EFI_D_ERROR, "Build IE_RC_CONFIGURATION HOB at 0x%08X(0x%x bytes)\n", (UINT32)HobPtr, sizeof (IE_RC_CONFIGURATION)));
#endif


  HobPtr = BuildGuidDataHob(&gEfiSocketPciResourceDataGuid, SocketPciResourceConfiguration,
    sizeof (*SocketPciResourceConfiguration));
  ASSERT(HobPtr != NULL);
  DEBUG ((EFI_D_ERROR, "Build SYSTEM_PCI_BASE_LIMITS HOB at 0x%08X(0x%x bytes)\n", (UINT32)HobPtr,
    sizeof (*SocketPciResourceConfiguration)));

  return Status;
}
