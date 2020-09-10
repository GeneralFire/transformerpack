/** @file
  ACPI Platform Driver Hooks

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1996 - 2020 Intel Corporation. <BR>

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

//
// Statements that include other files
//
#include "AcpiPlatform.h"
#include "AcpiPlatformUtils.h"
#include "AcpiPlatformHooks.h"
#include <RcRegs.h>
#include <Library/CpuAndRevisionLib.h>
#include <Library/KtiApi.h>
// APTIOV_SERVER_OVERRIDE_RC_START : Use OEM functions from AmiPlatformHooksLib.h 
#include <Library/AmiPlatformHooksLib.h>
// APTIOV_SERVER_OVERRIDE_RC_END : Use OEM functions from AmiPlatformHooksLib.h 

#ifndef __GNUC__
#pragma optimize("",off)
#endif  //__GNUC__

extern BOOLEAN                        mX2ApicEnabled;
extern UINT32                         mNumOfBitShift;
extern EFI_PLATFORM_INFO              *mPlatformInfo;
extern BIOS_ACPI_PARAM                *mAcpiParameter;
extern EFI_IIO_UDS_PROTOCOL           *mIioUds2;
extern CPU_CSR_ACCESS_VAR             *mCpuCsrAccessVarPtr;
extern EFI_CPU_CSR_ACCESS_PROTOCOL    *mCpuCsrAccess;

extern SOCKET_MEMORY_CONFIGURATION    mSocketMemoryConfiguration;
extern SOCKET_MP_LINK_CONFIGURATION   mSocketMpLinkConfiguration;
extern SOCKET_COMMONRC_CONFIGURATION  mSocketCommonRcConfiguration;
extern SOCKET_IIO_CONFIGURATION       mSocketIioConfiguration;
extern SOCKET_POWERMANAGEMENT_CONFIGURATION mSocketPowermanagementConfiguration;
extern SOCKET_PROCESSORCORE_CONFIGURATION   mSocketProcessorCoreConfiguration;
extern SYSTEM_CONFIGURATION           mSystemConfiguration;
extern PCH_SETUP                      mPchSetup;
extern BOOLEAN                        Is14nmCpu;

EFI_GLOBAL_NVS_AREA_PROTOCOL        mGlobalNvsArea;


/**

    GC_TODO: add routine description

    @param None

    @retval EFI_SUCCESS - GC_TODO: add retval description

**/
EFI_STATUS
PlatformHookInit (
  VOID
  )
{
  EFI_STATUS                    Status;
  EFI_HANDLE                    Handle;
  EFI_PHYSICAL_ADDRESS          AcpiParameterAddr;
  EFI_CPUID_REGISTER            CpuidRegisters;
  // APTIOV_SERVER_OVERRIDE_RC_START: To solve structure redefinition error in AMI.
  INTEL_SETUP_DATA              SetupData;
  // APTIOV_SERVER_OVERRIDE_RC_END: To solve structure redefinition error in AMI.
  ASSERT (mIioUds2);

  Status = EFI_SUCCESS;

  Status = GetEntireConfig (&SetupData);

  CopyMem (&mSocketMemoryConfiguration, &(SetupData.SocketConfig.MemoryConfig), sizeof(SOCKET_MEMORY_CONFIGURATION));
  CopyMem (&mSocketMpLinkConfiguration, &(SetupData.SocketConfig.UpiConfig), sizeof(SOCKET_MP_LINK_CONFIGURATION));
  CopyMem (&mSocketCommonRcConfiguration, &(SetupData.SocketConfig.CommonRcConfig), sizeof(SOCKET_COMMONRC_CONFIGURATION));
  CopyMem (&mSocketPowermanagementConfiguration, &(SetupData.SocketConfig.PowerManagementConfig), sizeof(SOCKET_POWERMANAGEMENT_CONFIGURATION));
  CopyMem (&mSocketProcessorCoreConfiguration, &(SetupData.SocketConfig.SocketProcessorCoreConfiguration), sizeof(SOCKET_PROCESSORCORE_CONFIGURATION));
  CopyMem (&mSystemConfiguration, &(SetupData.SystemConfig), sizeof(SYSTEM_CONFIGURATION));
  CopyMem (&mSocketIioConfiguration, &(SetupData.SocketConfig.IioConfig), sizeof(SOCKET_IIO_CONFIGURATION));
  CopyMem (&mPchSetup, &(SetupData.PchSetup), sizeof(PCH_SETUP));

  if (EFI_ERROR (Status)) {
    mSocketPowermanagementConfiguration.ProcessorEistEnable = 0;
    mSocketPowermanagementConfiguration.TurboMode           = 0;
    mSocketPowermanagementConfiguration.PackageCState       = 0;
    mSocketPowermanagementConfiguration.PwrPerfTuning       = PWR_PERF_TUNING_BIOS_CONTROL;
    mSocketProcessorCoreConfiguration.ProcessorX2apic         = 0;
    mSocketProcessorCoreConfiguration.ForcePhysicalModeEnable = 0;
  }
  //
  //Force disable S3 if PCLS is enabled on platform. PCLS overrides S3 enable
  //
  if (!Is14nmCpu) {
    if (IsCpuAndRevision (CPU_ICXSP, REV_ALL) || IsCpuAndRevision (CPU_ICXD, REV_ALL) ||
      IsCpuAndRevision (CPU_SNR, REV_ALL) || IsHbmSku ()) {
      if (SetupData.SocketConfig.MemoryConfig.PclsEn) {
        mSocketPowermanagementConfiguration.AcpiS3Enable = 0;
      }
    }
  }
  //
  // If Emulation flag set by InitializeDefaultData in ProcMemInit.c
  //  force X2APIC
  // Else read setup data
  //
  mX2ApicEnabled = mSocketProcessorCoreConfiguration.ProcessorX2apic;

  //
  // Force x2APIC if the system is configured as X2APIC; or CPU hotplug is enabled.
  //
  if ((GetApicMode () == LOCAL_APIC_MODE_X2APIC) || (mIioUds2->IioUdsPtr->SystemStatus.OutKtiCpuSktHotPlugEn)) {
    mX2ApicEnabled = TRUE;
  }

  //
  // Allocate 256 runtime memory to pass ACPI parameter
  // This Address must be < 4G because we only have 32bit in the dsdt
  //
  AcpiParameterAddr = 0xffffffff;
  Status = gBS->AllocatePages (
                  AllocateMaxAddress,
                  EfiACPIMemoryNVS,
                  EFI_SIZE_TO_PAGES (sizeof(BIOS_ACPI_PARAM)),
                  &AcpiParameterAddr
                  );
  ASSERT_EFI_ERROR (Status);
  mAcpiParameter = (BIOS_ACPI_PARAM *)AcpiParameterAddr;

  DEBUG ((EFI_D_INFO, "ACPI Parameter Block Address: 0x%X\n", mAcpiParameter));

  ZeroMem (mAcpiParameter, sizeof (BIOS_ACPI_PARAM));
  mAcpiParameter->PlatformId    = (UINT32)mPlatformInfo->BoardId;
  mAcpiParameter->IoApicEnable  = mPlatformInfo->SysData.SysIoApicEnable;
  mAcpiParameter->PchIoApic_24_119 = mPchSetup.PchIoApic24119Entries;

  Handle = NULL;
  mGlobalNvsArea.Area = mAcpiParameter;
  // APTIOV_SERVER_OVERRIDE_RC_START : Assign result to Status
  Status = gBS->InstallProtocolInterface ( 
  // APTIOV_SERVER_OVERRIDE_RC_END : Assign result to Status
         &Handle,
         &gEfiGlobalNvsAreaProtocolGuid,
         EFI_NATIVE_INTERFACE,
         &mGlobalNvsArea
         );
  ASSERT_EFI_ERROR (Status);

  AsmCpuid (CPUID_VERSION_INFO,  &CpuidRegisters.RegEax, &CpuidRegisters.RegEbx, &CpuidRegisters.RegEcx, &CpuidRegisters.RegEdx);
  mAcpiParameter->ProcessorId = (CpuidRegisters.RegEax & 0xFFFF0);

  // support up to 64 threads/socket
  AsmCpuidEx(CPUID_EXTENDED_TOPOLOGY, 1, &mNumOfBitShift, NULL, NULL, NULL);
  mNumOfBitShift &= 0x1F;

  // Set the bit shift value for CPU SKU
  mAcpiParameter->CpuSkuNumOfBitShift = (UINT8) mNumOfBitShift;

  mAcpiParameter->ProcessorApicIdBase[0] = (UINT32) (0 << mNumOfBitShift);
  mAcpiParameter->ProcessorApicIdBase[1] = (UINT32) (1 << mNumOfBitShift);
  mAcpiParameter->ProcessorApicIdBase[2] = (UINT32) (2 << mNumOfBitShift);
  mAcpiParameter->ProcessorApicIdBase[3] = (UINT32) (3 << mNumOfBitShift);
  mAcpiParameter->ProcessorApicIdBase[4] = (UINT32) (4 << mNumOfBitShift);
  mAcpiParameter->ProcessorApicIdBase[5] = (UINT32) (5 << mNumOfBitShift);
  mAcpiParameter->ProcessorApicIdBase[6] = (UINT32) (6 << mNumOfBitShift);
  mAcpiParameter->ProcessorApicIdBase[7] = (UINT32) (7 << mNumOfBitShift);

  //
  // If SNC is enabled, and NumOfCluster is 2, set the ACPI variable for PXM value
  //
  if (mIioUds2->IioUdsPtr->SystemStatus.OutSncEn) {
    mAcpiParameter->SncAnd2Cluster = mIioUds2->IioUdsPtr->SystemStatus.OutNumOfCluster;
  }

   mAcpiParameter->MmCfg = (UINT32)mIioUds2->IioUdsPtr->PlatformData.PciExpressBase;
   mAcpiParameter->TsegSize = (UINT32)(mIioUds2->IioUdsPtr->PlatformData.MemTsegSize >> 20);

  Status = PlatformHookAfterAcpiParamInit ();
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "PlatformHookAfterAcpiParamInit() failed with Status: %r\n", Status));
  }

  return Status;
}


/**
  Install and patch specific ACPI Table

  @param AcpiTableSignature

  @retval None
**/
VOID
InstallAndPatchAcpiTable (
  UINT32 AcpiTableSignature
  )
{
  EFI_STATUS                    Status;
  EFI_ACPI_TABLE_PROTOCOL       *AcpiTable;
  EFI_ACPI_TABLE_VERSION        TableVersion;
  EFI_FIRMWARE_VOLUME2_PROTOCOL *FwVol;
  EFI_ACPI_COMMON_HEADER        *CurrentTable = NULL;
  UINT32                        FvStatus;
  UINTN                         Size;
  UINTN                         TableHandle = 0;
  INTN                          Instance = 0;

  DEBUG ((DEBUG_INFO, "InstallAndPatchAcpiTable '%c%c%c%c'\n",
          ((CHAR8*)&AcpiTableSignature)[0], ((CHAR8*)&AcpiTableSignature)[1],
          ((CHAR8*)&AcpiTableSignature)[2], ((CHAR8*)&AcpiTableSignature)[3]));

  Status = LocateSupportProtocol (
            &gEfiAcpiTableProtocolGuid,
            gEfiAcpiTableStorageGuid,
            &AcpiTable,
            FALSE
            );
  ASSERT_EFI_ERROR (Status);

  Status = LocateSupportProtocol (
            &gEfiFirmwareVolume2ProtocolGuid,
            gEfiAcpiTableStorageGuid,
            &FwVol,
            TRUE
            );
  ASSERT_EFI_ERROR (Status);

  while (!EFI_ERROR (Status)) {

    Status = FwVol->ReadSection (
                      FwVol,
                      &gEfiAcpiTableStorageGuid,
                      EFI_SECTION_RAW,
                      Instance,
                      &CurrentTable,
                      (UINTN *) &Size,
                      &FvStatus
                      );

    if (!EFI_ERROR (Status)) {
      if (CurrentTable->Signature == AcpiTableSignature) {

        Status = AcpiPlatformHooksIsActiveTable (CurrentTable);
        if (!EFI_ERROR (Status)) {

          Status = PlatformUpdateTables (CurrentTable, &TableVersion);
          if (!EFI_ERROR (Status)) {

            if (TableVersion != EFI_ACPI_TABLE_VERSION_NONE) {
              Status = AcpiTable->InstallAcpiTable (
                                        AcpiTable,
                                        CurrentTable,
                                        CurrentTable->Length,
                                        &TableHandle
                                        );
            }
          }
          ASSERT_EFI_ERROR (Status);
        } else {
          DEBUG ((DEBUG_ERROR, "No Table for current platform\n"));
        }
      }

      CurrentTable = NULL;
      Instance++;
    }
  }
}


/**
  Install Xhci ACPI Table

**/
VOID
InstallXhciAcpiTable (
  VOID
  )
{
  EFI_HANDLE                    *HandleBuffer;
  EFI_STATUS                    Status;
  EFI_FIRMWARE_VOLUME2_PROTOCOL *FwVol;
  EFI_ACPI_COMMON_HEADER        *CurrentTable;
  UINT32                        FvStatus;
  UINTN                         Size;
  UINTN                         TableHandle;
  INTN                          Instance;
  EFI_ACPI_TABLE_PROTOCOL       *AcpiTable;
  EFI_ACPI_DESCRIPTION_HEADER   *TableHeader;
  UINT64                        XhciAcpiTable;
  UINT64                        TempOemTableId;

  HandleBuffer  = 0;
  Instance      = 0;
  TableHandle   = 0;
  CurrentTable  = NULL;
  FwVol         = NULL;
  XhciAcpiTable = 0;

  DEBUG ((DEBUG_INFO, "InstallXhciAcpiTable\n"));


  XhciAcpiTable = *(UINT64*)PcdGetPtr (PcdOemTableIdXhci);
  // APTIOV_SERVER_OVERRIDE_RC_START : Oem Hook to Override XhciAcpiTable
  OemInstallXhciAcpiTableOverride((VOID *) mPlatformInfo, &XhciAcpiTable);
  // APTIOV_SERVER_OVERRIDE_RC_END : Oem Hook to Override XhciAcpiTable

  //
  // Find the AcpiSupport protocol
  //
  Status = LocateSupportProtocol (
            &gEfiAcpiTableProtocolGuid,
            gEfiAcpiTableStorageGuid,
            &AcpiTable,
            FALSE
            );

  ASSERT_EFI_ERROR (Status);

  //
  // Locate the firmware volume protocol
  //
  Status = LocateSupportProtocol (
            &gEfiFirmwareVolume2ProtocolGuid,
            gEfiAcpiTableStorageGuid,
            &FwVol,
            TRUE
            );

  ASSERT_EFI_ERROR (Status);

  //
  // Read tables from the storage file.
  //
  while (Status == EFI_SUCCESS) {
    Status = FwVol->ReadSection (
                      FwVol,
                      &gEfiAcpiTableStorageGuid,
                      EFI_SECTION_RAW,
                      Instance,
                      &CurrentTable,
                      &Size,
                      &FvStatus
                      );

    if (!EFI_ERROR (Status)) {

      TableHeader = (EFI_ACPI_DESCRIPTION_HEADER *) CurrentTable;

      if (TableHeader->OemTableId == XhciAcpiTable) {
        DEBUG ((DEBUG_INFO, "Install xhci table: %x\n", TableHeader->OemTableId));

        TempOemTableId  = PcdGet64(PcdAcpiDefaultOemTableId);

        CopyMem (TableHeader->OemId, PcdGetPtr(PcdAcpiDefaultOemId), sizeof(TableHeader->OemId));
        CopyMem (&TableHeader->OemTableId, &TempOemTableId, sizeof(TableHeader->OemTableId));

        TableHeader->CreatorId = EFI_ACPI_CREATOR_ID;
        TableHeader->CreatorRevision = EFI_ACPI_CREATOR_REVISION;

        //
        // Add the table
        //
        TableHandle = 0;

        Status = AcpiTable->InstallAcpiTable (
                                    AcpiTable,
                                    CurrentTable,
                                    CurrentTable->Length,
                                    &TableHandle
                                    );
        break;
      }

      //
      // Increment the instance
      //

      Instance++;
      CurrentTable = NULL;
    }
  }
}

UINT8
EFIAPI
DetectHwpFeature (
   VOID
  )
{
  UINT8  HwpSupport = 1;
  UINT8  socket;
  CAPID5_PCU_FUN3_STRUCT        Capid5;
  MSR_MISC_PWR_MGMT_REGISTER    MiscPwrMsr;

  ASSERT (mIioUds2);

  for (socket = 0; socket < MAX_SOCKET; socket++) {
    if (mCpuCsrAccessVarPtr->socketPresentBitMap & (1 << socket)) {
      Capid5.Data = mCpuCsrAccess->ReadCpuCsr(socket, 0, CAPID5_PCU_FUN3_REG);
      HwpSupport &= Capid5.Bits.hwp_en;
    }
  }

  if (!HwpSupport) {
    return HwpSupport;
  }

  MiscPwrMsr.Uint64 = AsmReadMsr64 (MSR_MISC_PWR_MGMT);
  HwpSupport = 0;

  if (MiscPwrMsr.Bits.EnableHwp == 1) {
    HwpSupport = 1;
  } else if (MiscPwrMsr.Bits.EnableOutOfBandAutonomous == 1) {
    HwpSupport = 2;
  }

  if (HwpSupport > 0) {
    return (mSocketPowermanagementConfiguration.ProcessorHWPMEnable);
  }
  return HwpSupport;
}
