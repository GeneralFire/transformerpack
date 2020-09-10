/** @file
  ACPI Platform Driver

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1999 - 2020 Intel Corporation. <BR>

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

#include "AcpiPlatform.h"
#include "AcpiPlatformUtils.h"
#include "AcpiPlatformHooks.h"
#include <Library/PcdLib.h>
#include <Protocol/PciEnumerationComplete.h>
#include <IioSetupDefinitions.h>
#include <ProcessorPpmSetup.h>

// APTIOV_SERVER_OVERRIDE_RC_START : To get AmiPlatformAcpiLib functions.
#include <Library/AmiPlatformAcpiLib.h> 
// APTIOV_SERVER_OVERRIDE_RC_END : To get AmiPlatformAcpiLib functions.
// APTIOV_SERVER_OVERRIDE_RC_START :
#include <Library/PciLib.h>
// APTIOV_SERVER_OVERRIDE_RC_END :

#ifndef __GNUC__
#pragma optimize("", off)
#endif  //__GNUC__

// APTIOV_SERVER_OVERRIDE_RC_START : Update "PCIe ASPM Controls" flag of FACP table.
#define MAX_FADT 2
// APTIOV_SERVER_OVERRIDE_RC_END : Update "PCIe ASPM Controls" flag of FACP table.
extern SOCKET_IIO_CONFIGURATION                       mSocketIioConfiguration;
extern SOCKET_POWERMANAGEMENT_CONFIGURATION           mSocketPowermanagementConfiguration;
extern SOCKET_PROCESSORCORE_CONFIGURATION             mSocketProcessorCoreConfiguration;
extern EFI_ACPI_SERIAL_PORT_CONSOLE_REDIRECTION_TABLE *mSpcrTable;
extern EFI_GUID                                       gEfiGlobalVariableGuid;
extern EFI_GUID                                       gEfiPmSsdtTableStorageGuid;

BIOS_ACPI_PARAM       *mAcpiParameter = NULL;
BOOLEAN               mFirstNotify;
SYSTEM_CONFIGURATION  mSystemConfiguration;
PCH_SETUP             mPchSetup;

UINT8                       mKBPresent = 0;
UINT8                       mMousePresent = 0;
EFI_IIO_UDS_PROTOCOL        *mIioUds2 = NULL;
extern CPU_CSR_ACCESS_VAR   *mCpuCsrAccessVarPtr;
UINT8                       mPStateEnable = 0;
BOOLEAN                     Is14nmCpu = FALSE;


VOID
EFIAPI
AcpiOnPciEnumCmplCallback (
  IN EFI_EVENT  Event,
  IN VOID      *Context
  )
{
  EFI_STATUS    Status;

  Status = gBS->LocateProtocol (&gEfiPciEnumerationCompleteProtocolGuid, NULL, &Context);
  if (EFI_ERROR(Status)) {
    //
    // Skip the first dummy event signal.
    //
    return;
  }
  gBS->CloseEvent (Event);

  DEBUG ((DEBUG_INFO, "[ACPI] %a\n", __FUNCTION__));
  AcpiVtdTablesInstall();
}


VOID
EFIAPI
AcpiOnEndOfDxeCallback (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  DEBUG ((DEBUG_INFO, "[ACPI] %a\n", __FUNCTION__));
  //
  // Installing ACPI Tables: NFIT, PCAT
  //
  InstallAndPatchAcpiTable (NVDIMM_FW_INTERFACE_TABLE_SIGNATURE);
  InstallAndPatchAcpiTable (NVDIMM_PLATFORM_CONFIG_ATTRIBUTE_TABLE_SIGNATURE);
}


//Enable SCI for ACPI aware OS at ExitBootServices

VOID
EFIAPI
AcpiOnExitBootServicesCallback (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  UINT16        Pm1Cnt;

  gBS->CloseEvent (Event);

  DEBUG ((DEBUG_INFO, "[ACPI] %a\n", __FUNCTION__));
  //
  // Enable SCI
  //
  Pm1Cnt = IoRead16(mAcpiParameter->PmBase + R_ACPI_IO_PM1_CNT);
  Pm1Cnt |= B_ACPI_IO_PM1_CNT_SCI_EN;
  IoWrite16(mAcpiParameter->PmBase + R_ACPI_IO_PM1_CNT, Pm1Cnt);
}

/**

Disables the SW SMI Timer.
ACPI events are disabled and ACPI event status is cleared.
SCI mode is then enabled.

Disable SW SMI Timer

Clear all ACPI event status and disable all ACPI events
Disable PM sources except power button
Clear status bits

Disable GPE0 sources
Clear status bits

Disable GPE1 sources
Clear status bits

Guarantee day-of-month alarm is invalid (ACPI 1.0 section 4.7.2.4)

Enable SCI

@param Event   - not used
@param Context - not used

@retval None

**/
STATIC
VOID
AcpiEnableAtReadyToBoot(
  VOID
)
{
  UINT32      SmiEn;
  UINT8       Data8;
  UINT16      Pm1En;

  ASSERT(mAcpiParameter->PmBase != 0);
  
// APTIOV_SERVER_OVERRIDE_RC_START : To Fix legacy OS Boot issue.
#if defined EMUL6064_SUPPORT && EMUL6064_SUPPORT == 1
  if ( PciRead8( PCI_LIB_ADDRESS( LPC_BUS, LPC_DEVICE, LPC_FUNC, R_LPC_CFG_ULKMC)) ){
      PciWrite8(PCI_LIB_ADDRESS( LPC_BUS, LPC_DEVICE, LPC_FUNC, R_LPC_CFG_ULKMC ), 0);
  }
#endif
// APTIOV_SERVER_OVERRIDE_RC_END : To Fix legacy OS Boot issue.
  
  SmiEn = IoRead32(mAcpiParameter->PmBase + R_ACPI_IO_SMI_EN);

  //
  // Disable SW SMI Timer and legacy USB
  //
  
// APTIOV_SERVER_OVERRIDE_RC_START : To Fix legacy OS Boot issue.
  //SmiEn &= ~(B_ACPI_IO_SMI_EN_SWSMI_TMR | B_ACPI_IO_SMI_EN_LEGACY_USB | B_ACPI_IO_SMI_EN_LEGACY_USB2);
  SmiEn &= ~(B_ACPI_IO_SMI_EN_SWSMI_TMR);
// APTIOV_SERVER_OVERRIDE_RC_END : To Fix legacy OS Boot issue.
  
  //
  // And enable SMI on write to B_ACPI_IO_PM1_CNT_SLP_EN when SLP_TYP is written
  //
  SmiEn |= B_ACPI_IO_SMI_EN_ON_SLP_EN;
  IoWrite32(mAcpiParameter->PmBase + R_ACPI_IO_SMI_EN, SmiEn);

  //
  // Disable PM sources except power button
  //

  Pm1En = B_ACPI_IO_PM1_EN_PWRBTN;
  IoWrite16(mAcpiParameter->PmBase + R_ACPI_IO_PM1_EN, Pm1En);

  //
  // Guarantee day-of-month alarm is invalid (ACPI 1.0 section 4.7.2.4)
  //
  Data8 = RTC_ADDRESS_REGISTER_D;
  IoWrite8(R_IOPORT_CMOS_STANDARD_INDEX, Data8);
  Data8 = 0x0;
  IoWrite8(R_IOPORT_CMOS_STANDARD_DATA, Data8);
  // APTIOV_SERVER_OVERRIDE_RC_END : Power on override

  //
  // Do platform specific stuff for ACPI enable SMI
  //


}

/**
  Executes ACPI Platform actions related with ready to boot event

  @param Event   - not used
  @param Context - not used

  @retval None

**/
STATIC
VOID
EFIAPI
AcpiOnReadyToBootCallback (
  IN     EFI_EVENT            Event,
  IN     VOID                *Context
  )
{
  EFI_STATUS                  Status;
  // APTIOV_SERVER_OVERRIDE_RC_START
  //EFI_ACPI_DESCRIPTION_HEADER Table = {0};
  //EFI_ACPI_TABLE_VERSION      TableVersion;
  //UINTN                       TableHandle;
  //EFI_ACPI_TABLE_PROTOCOL     *AcpiTable;
  // APTIOV_SERVER_OVERRIDE_RC_END
  EFI_CPUID_REGISTER          CpuidRegisters;
  // APTIOV_SERVER_OVERRIDE_RC_START: To solve structure redefinition error in AMI.
  INTEL_SETUP_DATA            SetupData;
  // APTIOV_SERVER_OVERRIDE_RC_END: To solve structure redefinition error in AMI.
  UINT8                       ARIForward;

  if (mFirstNotify) {
    return;
  }
  mFirstNotify = TRUE;
  DEBUG ((DEBUG_INFO, "[ACPI] %a\n", __FUNCTION__));

  Status = GetEntireConfig (&SetupData);
  ASSERT_EFI_ERROR (Status);
  CopyMem (&mSystemConfiguration, &(SetupData.SystemConfig), sizeof(SYSTEM_CONFIGURATION));
  CopyMem (&mSocketIioConfiguration, &(SetupData.SocketConfig.IioConfig), sizeof(SOCKET_IIO_CONFIGURATION));
  CopyMem (&mSocketPowermanagementConfiguration, &(SetupData.SocketConfig.PowerManagementConfig), sizeof(SOCKET_POWERMANAGEMENT_CONFIGURATION));
  CopyMem (&mSocketProcessorCoreConfiguration, &(SetupData.SocketConfig.SocketProcessorCoreConfiguration), sizeof(SOCKET_PROCESSORCORE_CONFIGURATION));
  CopyMem (&mPchSetup, &(SetupData.PchSetup), sizeof(PCH_SETUP));

  mAcpiParameter->TpmEnable = mSystemConfiguration.TpmEnable;

  // CpuPm.Asl: External (CSEN, FieldUnitObj)
  mAcpiParameter->CStateEnable = TRUE;
  // CpuPm.Asl: External (C3EN, FieldUnitObj)
  mAcpiParameter->C3Enable     = mSocketPowermanagementConfiguration.C3Enable;
  // CpuPm.Asl: External (C6EN, FieldUnitObj)
  AsmCpuid(CPUID_MONITOR_MWAIT, &CpuidRegisters.RegEax, &CpuidRegisters.RegEbx, &CpuidRegisters.RegEcx, &CpuidRegisters.RegEdx);
  // If C6 is not supported by CPU, disregard setup C6 knob value
  if(((CpuidRegisters.RegEdx >> 12) & 0xF) > 0) {
    if (mSocketPowermanagementConfiguration.C6Enable == PPM_AUTO) {
      mAcpiParameter->C6Enable = 1;  //POR Default = Enabled
    }
    else {
      mAcpiParameter->C6Enable = mSocketPowermanagementConfiguration.C6Enable;
    }
  }
  else {
    mAcpiParameter->C6Enable = 0;
    DEBUG((DEBUG_INFO, "Cpu does not support C6 state\n"));
  }

  if(mAcpiParameter->C6Enable && mAcpiParameter->C3Enable) {  //C3 and C6 enable are exclusive
    mAcpiParameter->C6Enable = 1;
    mAcpiParameter->C3Enable = 0;
  }
  // CpuPm.Asl: External (C7EN, FieldUnitObj)
  mAcpiParameter->C7Enable     = 0;
  // CpuPm.Asl: External (OSCX, FieldUnitObj)
  mAcpiParameter->OSCX         = mSocketPowermanagementConfiguration.OSCx;
  // CpuPm.Asl: External (MWOS, FieldUnitObj)
  mAcpiParameter->MonitorMwaitEnable = 1;
  // CpuPm.Asl: External (PSEN, FieldUnitObj)
  mAcpiParameter->PStateEnable = mPStateEnable;
  // CpuPm.Asl: External (HWAL, FieldUnitObj)
  mAcpiParameter->HWAllEnable = mSocketPowermanagementConfiguration.ProcessorEistPsdFunc;

  mAcpiParameter->KBPresent    = mKBPresent;
  mAcpiParameter->MousePresent = mMousePresent;
  mAcpiParameter->TStateEnable = mSocketPowermanagementConfiguration.TStateEnable;
  // Debug mode indicator for Acpi use
  mAcpiParameter->DebugModeIndicator = (UINT8)PcdGet8(PcdDebugModeEnable);
  DEBUG((EFI_D_ERROR, "DebugModeIndicator = %x\n",mAcpiParameter->DebugModeIndicator));

  //Fine grained T state
  AsmCpuid (CPUID_THERMAL_POWER_MANAGEMENT,  &CpuidRegisters.RegEax, &CpuidRegisters.RegEbx, &CpuidRegisters.RegEcx, &CpuidRegisters.RegEdx);
  if ((((CPUID_THERMAL_POWER_MANAGEMENT_EAX*)&CpuidRegisters.RegEax)->Bits.ECMD) && mSocketPowermanagementConfiguration.TStateEnable){
    mAcpiParameter->TStateFineGrained = 1;
  }
  if (((CPUID_THERMAL_POWER_MANAGEMENT_EAX*)&CpuidRegisters.RegEax)->Bits.HWP_Notification != 0) {
    mAcpiParameter->HwpInterrupt = 1;
  }
  // CpuPm.Asl: External (HWEN, FieldUnitObj)
  mAcpiParameter->HWPMEnable = DetectHwpFeature();

  mAcpiParameter->EmcaEn    = mSystemConfiguration.EmcaEn;
  mAcpiParameter->WheaSupportEn  = mSystemConfiguration.WheaSupportEn;

  mAcpiParameter->PcieAcpiHotPlugEnable = (UINT8) (BOOLEAN) (mSocketIioConfiguration.PcieAcpiHotPlugEnable != 0);
  //
  // Initialize USB3 mode from setup data
  //
  // If mode != manual control
  //  just copy mode from setup
    if (mPchSetup.PchUsbManualMode != 1) {
      mAcpiParameter->XhciMode = mPchSetup.PchUsbManualMode;
    }

  //
  // Get ACPI IO Base Address
  //
  mAcpiParameter->PmBase = PmcGetAcpiBase ();
  DEBUG ((DEBUG_INFO, "ACPI IO Base Address = %x\n", mAcpiParameter->PmBase));

  //
  // When X2APIC enabled and VTD support enabled, Enable ApicIdOverrided parameter to update ACPI table.
  //
  if (mSocketIioConfiguration.VTdSupport && mSocketProcessorCoreConfiguration.ProcessorX2apic) {
    mAcpiParameter->ApicIdOverrided = 1;
  }

  //
  // Find the AcpiTable protocol
  //
// APTIOV_SERVER_OVERRIDE_RC_START
  #if 0
  Status = LocateSupportProtocol (
            &gEfiAcpiTableProtocolGuid,
            gEfiAcpiTableStorageGuid,
            &AcpiTable,
            FALSE
            );

  ASSERT_EFI_ERROR (Status);

  TableVersion    = EFI_ACPI_TABLE_VERSION_2_0;
  Table.Signature = EFI_ACPI_6_2_SERIAL_PORT_CONSOLE_REDIRECTION_TABLE_SIGNATURE;
  Status = PlatformUpdateTables ((EFI_ACPI_COMMON_HEADER *)&Table, &TableVersion);
  if(!EFI_ERROR (Status)) {
    //
    // Add SPCR table
    //
    if (mSpcrTable != NULL) {
      DEBUG ((DEBUG_INFO, "mSpcrTable->Header.Length=%d\n", mSpcrTable->Header.Length));
      DEBUG ((DEBUG_INFO, "install=%x\n", &(AcpiTable->InstallAcpiTable)));
      DEBUG ((DEBUG_INFO, "acpit=%x\n",   AcpiTable));
      DEBUG ((DEBUG_INFO, "mSpcr=%x\n",   mSpcrTable));
      DEBUG ((DEBUG_INFO, "len   =%d\n",  mSpcrTable->Header.Length));

      TableHandle = 0;
      Status = AcpiTable->InstallAcpiTable (
        AcpiTable,
        mSpcrTable,
        mSpcrTable->Header.Length,
        &TableHandle
        );
      ASSERT_EFI_ERROR (Status);
    } else {
      DEBUG ((DEBUG_INFO, "Warning: mSpcrTable is NULL\n"));
    }
  }
  if (mSpcrTable != NULL) {
    gBS->FreePool (mSpcrTable);
  }

 #endif
// APTIOV_SERVER_OVERRIDE_RC_END
  AcpiVtdIntRemappingEnable();

  AcpiEnableAtReadyToBoot();

  Status = GetOptionData (&gEfiSetupVariableGuid,  OFFSET_OF(SYSTEM_CONFIGURATION, ARIForward), &ARIForward, sizeof(UINT8));
  ASSERT_EFI_ERROR (Status);
  if (!ARIForward) {
    DisableAriForwarding();
  }

  // APTIOV_SERVER_OVERRIDE_RC_START : Close event so that is isn't called again.
  gBS->CloseEvent(Event);
  // APTIOV_SERVER_OVERRIDE_RC_END : Close event so that is isn't called again.
}


/**
  Installs ACPI Platform tables

  @param None

  @retval EFI_SUCCESS -  Operation completed successfully.

**/
STATIC
EFI_STATUS
EFIAPI
AcpiPlatformEarlyAcpiTablesInstall(
   VOID
   )
{
  EFI_STATUS                    Status;
  EFI_STATUS                    AcpiStatus;
  BOOLEAN                       Installed;
  EFI_ACPI_TABLE_PROTOCOL       *AcpiTable;
  EFI_FIRMWARE_VOLUME2_PROTOCOL  *FwVol;
  INTN                          Instance = 0;
  EFI_ACPI_COMMON_HEADER        *CurrentTable;
  EFI_ACPI_TABLE_VERSION        TableVersion;
  UINTN                         TableHandle;
  UINT32                        FvStatus;
  UINT32                        Size;
//
// APTIOV_SERVER_OVERRIDE_RC_START : Update "PCIe ASPM Controls" flag of FACP table.
//
  UINT32                        Version = EFI_ACPI_TABLE_VERSION_2_0;
  UINT8                         FadtTableCount;
//
// APTIOV_SERVER_OVERRIDE_RC_END : Update "PCIe ASPM Controls" flag of FACP table.
//

  //
  // Find the AcpiTable protocol
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
  while (!EFI_ERROR (Status)) {
    CurrentTable = NULL;
    TableVersion = EFI_ACPI_TABLE_VERSION_NONE;
    TableHandle = 0;
    Installed = FALSE;

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

      DEBUG ((DEBUG_INFO, "[ACPI] Table '%c%c%c%c' found in FwVol\n",
              ((CHAR8*)&CurrentTable->Signature)[0], ((CHAR8*)&CurrentTable->Signature)[1],
              ((CHAR8*)&CurrentTable->Signature)[2], ((CHAR8*)&CurrentTable->Signature)[3]));

      //
      // Check if table should be processed or will be updated later
      //
      if (CurrentTable->Signature != NVDIMM_FW_INTERFACE_TABLE_SIGNATURE
          && CurrentTable->Signature != NVDIMM_PLATFORM_CONFIG_ATTRIBUTE_TABLE_SIGNATURE
          ) {
        //
        // Allow platform specific code to reject the table or update it
        //
        AcpiStatus = AcpiPlatformHooksIsActiveTable (CurrentTable); //SystemBoard);
        if (!EFI_ERROR (AcpiStatus)) {
          //
          // Perform any table specific updates.
          //
          AcpiStatus = PlatformUpdateTables (CurrentTable, &TableVersion);
          if (!EFI_ERROR (AcpiStatus)) {
            //
            // Add the table
            //
            if (TableVersion != EFI_ACPI_TABLE_VERSION_NONE) {
              //
              // Install the table
              //
              AcpiStatus = AcpiTable->InstallAcpiTable (AcpiTable, CurrentTable, CurrentTable->Length, &TableHandle);
              if (!EFI_ERROR (AcpiStatus)) {
                Installed = TRUE;
              }
              ASSERT_EFI_ERROR (AcpiStatus);
            }
          }
        }
      }
      //
      // Increment the instance
      //
      Instance++;
      //APTIOV_SERVER_OVERRIDE_RC_START: To free memory allocated by ReadSection
      FreePool(CurrentTable);
	  //APTIOV_SERVER_OVERRIDE_RC_END: To free memory allocated by ReadSection
    }
  }

  //
    //APTIOV_SERVER_OVERRIDE_RC_START: To read from AMI DSDT FFS
    //
        Status = AcpiLibGetAcpiTable(
                     (EFI_ACPI_TABLE_VERSION_1_0B|EFI_ACPI_TABLE_VERSION_2_0|EFI_ACPI_TABLE_VERSION_3_0|EFI_ACPI_TABLE_VERSION_4_0),
                     EFI_ACPI_3_0_DIFFERENTIATED_SYSTEM_DESCRIPTION_TABLE_SIGNATURE,
                     (EFI_ACPI_SDT_HEADER**) &CurrentTable,
                     &TableHandle
                     );
        if(EFI_ERROR(Status)){
            DEBUG ((EFI_D_ERROR,"AcpiPlatform: Failed to get AMI DSDT, Status: %r\n", Status));
            ASSERT_EFI_ERROR(Status);
        } else{
            //
            // Allow platform specific code to reject the table or update it
            //
            AcpiStatus = AcpiPlatformHooksIsActiveTable (CurrentTable); 
            if (!EFI_ERROR (AcpiStatus)) {
                //
                // Perform any table specific updates.
                //
                AcpiStatus = PlatformUpdateTables (CurrentTable, &TableVersion);
            }
            if (!EFI_ERROR(AcpiStatus)){
              Status = AcpiLibReinstallAcpiTable (
                        (VOID *)CurrentTable,
                        &TableHandle
                       );

                DEBUG ((EFI_D_INFO,"AcpiPlatform: ReinstallAcpiTable for AMI DSDT %r\n", Status));
              } else {
#if ACPI_MODULE_VER < 170 
                  FreePool(CurrentTable);  // Freeing memory allocated by GetAcpiTable call for DSDT.
#endif 
              }           
        }
        
    //
    //APTIOV_SERVER_OVERRIDE_RC_END: To read from AMI DSDT FFS
    //
    //
  //
  // APTIOV_SERVER_OVERRIDE_RC_START : Update "PCIe ASPM Controls" flag of FACP table.
  //
      if (ACPI_SPEC_VERSION >= 30)
      {
          Version = EFI_ACPI_TABLE_VERSION_3_0;  
      }
      if (ACPI_SPEC_VERSION >= 40)
      {
          Version = EFI_ACPI_TABLE_VERSION_4_0;
      }
      for (FadtTableCount = 0; FadtTableCount < MAX_FADT; FadtTableCount++) {
          Status = AcpiLibGetAcpiTable (
                     Version,
                     EFI_ACPI_3_0_FIXED_ACPI_DESCRIPTION_TABLE_SIGNATURE,
                     (EFI_ACPI_SDT_HEADER **) &CurrentTable,
                     &TableHandle
                   );
          DEBUG ((EFI_D_ERROR,"AcpiPlatform: AcpiLibGetAcpiTable Status: %r\n", Status));
          if (!EFI_ERROR(Status)) {
              //
              // Perform any table specific updates.
              //
              AcpiStatus = PlatformUpdateTables (CurrentTable, &TableVersion);
              if (!EFI_ERROR (AcpiStatus)) {
                  Status = AcpiLibReinstallAcpiTable (
                             (VOID *)CurrentTable,
                             &TableHandle
                           );
                  DEBUG ((EFI_D_ERROR,"AcpiPlatform: AcpiLibReinstallAcpiTable for AMI FADT %r\n", Status));
                  Version = EFI_ACPI_TABLE_VERSION_X;
              } else {
#if ACPI_MODULE_VER < 170 
                  FreePool(CurrentTable);  // Freeing memory allocated by GetAcpiTable call for FADT.
#endif 
              }
          } else {
              if (Status != EFI_NOT_FOUND) ASSERT_EFI_ERROR (Status);
              break;
          }
      }
  //
  // APTIOV_SERVER_OVERRIDE_RC_END :  Update "PCIe ASPM Controls" flag of FACP table.
  //
        
  //
  // Build any from-scratch ACPI tables. Halt on errors for debug builds, but
  // for release builds it is safe to continue.
  //
  Status = PlatformBuildTables ();
  ASSERT_EFI_ERROR (Status);
  return EFI_SUCCESS;
}

/**
  Installs ACPI Platform tables that wasn't installed in the early phase

  @param None

  @retval EFI_SUCCESS -  Operation completed successfully.
**/
STATIC
EFI_STATUS
EFIAPI
AcpiPlatformLateAcpiTablesInstall(
   VOID
   )
{
  //
  // Install xHCI ACPI Table
  //
  InstallXhciAcpiTable ();

  return EFI_SUCCESS;
}


/**
  Installs ACPI PmSsdt tables

  @param None

  @retval EFI_SUCCESS -  Operation completed successfully.

**/
STATIC
EFI_STATUS
EFIAPI
PmSsdtEarlyAcpiTablesInstall(
   VOID
   )
{
  EFI_STATUS                    Status;
  EFI_STATUS                    AcpiStatus;
  BOOLEAN                       Installed;
  EFI_ACPI_TABLE_PROTOCOL       *AcpiTable;
  EFI_FIRMWARE_VOLUME2_PROTOCOL  *FwVol;
  INTN                          Instance = 0;
  EFI_ACPI_COMMON_HEADER        *CurrentTable;
  EFI_ACPI_TABLE_VERSION        TableVersion;
  UINTN                         TableHandle;
  UINT32                        FvStatus;
  UINT32                        Size;

  //
  // Find the AcpiTable protocol
  //
  Status = LocateSupportProtocol (
            &gEfiAcpiTableProtocolGuid,
            gEfiPmSsdtTableStorageGuid,
            &AcpiTable,
            FALSE
            );

  ASSERT_EFI_ERROR (Status);

  //
  // Locate the firmware volume protocol
  //
  Status = LocateSupportProtocol (
            &gEfiFirmwareVolume2ProtocolGuid,
            gEfiPmSsdtTableStorageGuid,
            &FwVol,
            TRUE
            );
  ASSERT_EFI_ERROR (Status);

  //
  // Read tables from the storage file.
  //
  while (!EFI_ERROR (Status)) {
    CurrentTable = NULL;
    TableVersion = EFI_ACPI_TABLE_VERSION_NONE;
    TableHandle = 0;
    Installed = FALSE;

    Status = FwVol->ReadSection (
                      FwVol,
                      &gEfiPmSsdtTableStorageGuid,
                      EFI_SECTION_RAW,
                      Instance,
                      &CurrentTable,
                      (UINTN *) &Size,
                      &FvStatus
                      );

    if (!EFI_ERROR (Status)) {
      //
      // Check if table should be processed or will be updated later
      //
      if (CurrentTable->Signature != NVDIMM_FW_INTERFACE_TABLE_SIGNATURE) {
        //
        // Allow platform specific code to reject the table or update it
        //
        AcpiStatus = AcpiPlatformHooksIsActiveTable (CurrentTable); //SystemBoard);
        if (!EFI_ERROR (AcpiStatus)) {
          //
          // Perform any table specific updates.
          //
          AcpiStatus = PlatformUpdateTables (CurrentTable, &TableVersion);
          if (!EFI_ERROR (AcpiStatus)) {
            //
            // Add the table
            //
            if (TableVersion != EFI_ACPI_TABLE_VERSION_NONE) {
              //
              // Install the table
              //
              AcpiStatus = AcpiTable->InstallAcpiTable (AcpiTable, CurrentTable, CurrentTable->Length, &TableHandle);
              if (!EFI_ERROR (AcpiStatus)) {
                Installed = TRUE;
              }
              ASSERT_EFI_ERROR (AcpiStatus);
            }
          }
        }
      }
      //
      // Increment the instance
      //
      Instance++;
    }
  }
  return EFI_SUCCESS;
} // PmSsdtEarlyAcpiTablesInstall()


/**
  Entry point for Acpi platform driver.

  @param ImageHandle  -  A handle for the image that is initializing this driver.
  @param SystemTable  -  A pointer to the EFI system table.

  @retval EFI_SUCCESS           -  Driver initialized successfully.
  @retval EFI_LOAD_ERROR        -  Failed to Initialize or has been loaded.
  @retval EFI_OUT_OF_RESOURCES  -  Could not allocate needed resources.
**/
EFI_STATUS
EFIAPI
AcpiPlatformEntryPoint (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
// APTIOV_SERVER_OVERRIDE_RC_START : Patch to resolve Legacy Boot issue
  EFI_EVENT   LegacyBootEvent;
// APTIOV_SERVER_OVERRIDE_RC_END : Patch to resolve Legacy Boot issue
  EFI_STATUS  Status;
  EFI_EVENT   Event;
  VOID       *HobList;
  VOID       *RgstPtr;

  mFirstNotify = FALSE;
  //
  // Report Status Code to indicate Acpi Init
  //
  REPORT_STATUS_CODE (
    EFI_PROGRESS_CODE,
    (EFI_SOFTWARE_DXE_CORE | EFI_SW_DXE_BS_ACPI_INIT)
    );

  Is14nmCpu = IsCpuAndRevision (CPU_SKX, REV_ALL) || IsCpuAndRevision (CPU_CLX, REV_ALL) || IsCpuAndRevision (CPU_CPX, REV_ALL);

  Status = gBS->LocateProtocol (&gEfiIioUdsProtocolGuid, NULL, &mIioUds2);
  if (EFI_ERROR (Status)) {

    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  mCpuCsrAccessVarPtr = GetSysCpuCsrAccessVar ();
  //
  // Update HOB variable for PCI resource information
  // Get the HOB list.  If it is not present, then ASSERT.
  //
  Status = EfiGetSystemConfigurationTable (&gEfiHobListGuid, &HobList);
  ASSERT_EFI_ERROR (Status);

  //
  // Initialize Platform Hooks
  //
  PlatformHookInit ();

  //
  // Install ACPI PLatform Tables
  //
  Status = AcpiPlatformEarlyAcpiTablesInstall ();
  ASSERT_EFI_ERROR (Status);

  //
  // Install ACPI PMSsdt Tables
  //
  Status = PmSsdtEarlyAcpiTablesInstall ();
  ASSERT_EFI_ERROR (Status);

  //
  // Install ACPI PLatform Tables that wasn't installed yet (NFIT/xHCI)
  //
  Status = AcpiPlatformLateAcpiTablesInstall ();
  ASSERT_EFI_ERROR (Status);

  //
  // Register ready to boot event handler
  //
  Status = EfiCreateEventReadyToBootEx (TPL_NOTIFY, AcpiOnReadyToBootCallback, NULL, &Event);
  ASSERT_EFI_ERROR (Status);

  //
  // Create PCI Enumeration Completed callback.
  //
  Event = EfiCreateProtocolNotifyEvent (&gEfiPciEnumerationCompleteProtocolGuid, TPL_CALLBACK,
                                        AcpiOnPciEnumCmplCallback, NULL, &RgstPtr);
  ASSERT (Event != NULL);

  //
  // Register EndOfDxe handler
  //
  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  AcpiOnEndOfDxeCallback,
                  NULL,
                  &gEfiEndOfDxeEventGroupGuid,
                  &Event
                  );
  ASSERT_EFI_ERROR (Status);

  // Register ExitBootServicesEvent handler
  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_CALLBACK,
                  AcpiOnExitBootServicesCallback,
                  NULL,
                  &gEfiEventExitBootServicesGuid,
                  &Event
                  );
  ASSERT_EFI_ERROR (Status);
  // APTIOV_SERVER_OVERRIDE_RC_START : Patch to resolve Legacy Boot issue
  Status = EfiCreateEventLegacyBootEx (
            TPL_CALLBACK,
            AcpiOnExitBootServicesCallback,
            NULL,
            &LegacyBootEvent
            );
  ASSERT_EFI_ERROR (Status);
  // APTIOV_SERVER_OVERRIDE_RC_END : Patch to resolve Legacy Boot issue
  return EFI_SUCCESS;
} // AcpiPlatformEntryPoint()

