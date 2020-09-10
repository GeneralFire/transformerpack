/** @file
  EFI PEIM to determine the bootmode of the platform

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

#include "BootMode.h"
#include <Library/PmcLib.h>
#include <Library/UbaGpioPlatformConfig.h>
#include <Library/PerformanceLib.h>
#include <Library/PchRtcLib.h>
#include <Ppi/ReadOnlyVariable2.h>
#include <Register/PchRegsLpc.h>

#define FAST_BOOT_MODE 0x13
EFI_PEI_PPI_DESCRIPTOR  mPpiListRecoveryBootMode = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiPeiBootInRecoveryModePpiGuid,
  NULL
};

EFI_PEI_PPI_DESCRIPTOR  mPpiListUpdateBootMode = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gUpdateBootModePpiGuid,
  NULL
};


/**
  Parse the status registers for figuring out the wake-up event.

  @param[out]  WakeUpType       Updates the wakeuptype based on the status registers
**/
VOID
GetWakeupEvent (
  OUT  UINT8   *WakeUpType
  )
{
  UINT16             Pm1Sts;
  UINTN              Gpe0Sts;
  UINTN              Gpe0PmeSts;
  UINTN              Gpe0RiSts;
  UINT16             AcpiBase;
  PERF_START (NULL, "WakeUp Event", "PEI", 0);
  DEBUG ((DEBUG_INFO, "GetWakeupEvent Entry()\n"));

  Gpe0Sts    = 0;
  Gpe0RiSts  = 0;
  Gpe0PmeSts = 0;

  AcpiBase = PmcGetAcpiBase ();

  ///
  /// Read the ACPI registers
  ///
  Pm1Sts     = IoRead16 (AcpiBase + R_ACPI_IO_PM1_STS);
  Gpe0Sts    = IoRead32 (AcpiBase + R_ACPI_IO_GPE0_STS_127_96);
  Gpe0RiSts  = Gpe0Sts & B_ACPI_IO_GPE0_STS_127_96_RI ;
  Gpe0PmeSts = Gpe0Sts & B_ACPI_IO_GPE0_STS_127_96_PME ;

  DEBUG ((DEBUG_INFO, "Gpe0Sts is: %02x\n", Gpe0Sts));
  DEBUG ((DEBUG_INFO, "ACPI Wake Status Register: %04x\n", Pm1Sts));

  ///
  /// Figure out the wake-up event
  ///
  if ((Pm1Sts & B_ACPI_IO_PM1_EN_PWRBTN) != 0 ) {
    *WakeUpType = SystemWakeupTypePowerSwitch;
  } else if ((Pm1Sts & B_ACPI_IO_PM1_STS_WAK) != 0) {
    if (Gpe0PmeSts != 0) {
      *WakeUpType = SystemWakeupTypePciPme;
    } else if (Gpe0RiSts != 0) {
      *WakeUpType = SystemWakeupTypeModemRing;
    } else if (Gpe0Sts != 0) {
      *WakeUpType = SystemWakeupTypeOther;
    } else {
      *WakeUpType = SystemWakeupTypeOther;
    }
  } else if ((Pm1Sts & B_ACPI_IO_PM1_STS_RTC) != 0) {
    *WakeUpType = SystemWakeupTypeApmTimer;
  } else {
    *WakeUpType = SystemWakeupTypeOther;
  }

  DEBUG ((DEBUG_INFO, "GetWakeupEvent : WakeUpType %x\n", *WakeUpType));
  PERF_END (NULL, "WakeUp Event", "PEI", 0);
}

//
// To check whether fast boot mode needs to enabled or not
//
UINT8
FastBootMode (
IN SYSTEM_CONFIGURATION    *SystemConfiguration
 )
{
  EFI_STATUS                            Status;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI       *PeiVariable;
  UINTN                                 VariableSize;
  UINT8                                 FastBoot = FALSE;

  // if UEFI OPTIMIZED BOOT is disable, then Fast Boot will not support
  if ((SystemConfiguration != NULL) && (SystemConfiguration->UefiOptimizedBootToggle == 0x02)) {
    return FastBoot;
  }

  Status = PeiServicesLocatePpi (
                      &gEfiPeiReadOnlyVariable2PpiGuid,
                      0,
                      NULL,
                      &PeiVariable
                    );
  if(Status == EFI_SUCCESS) {
    VariableSize = sizeof(FastBoot);
    Status = PeiVariable->GetVariable (
                PeiVariable,
                L"FastBoot",
                &gEfiSetupVariableGuid,
                NULL,
                &VariableSize,
                &FastBoot
                );
  }
  return FastBoot;
}

/**

    Determines the Boot Mode of the platform and sets the Boot Mode variable

    @param[in]  PeiServices       General purpose services available to every PEIM.
    @param[in]  NotifyDescriptor  Notify that this module published.
    @param[in]  Ppi               PPI that was installed.

    @retval Status - Success if BootMode was set

**/
EFI_STATUS
UpdateBootMode (
  IN CONST EFI_PEI_SERVICES     **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  )
{
  CHAR16                        *strBootMode;
  UINT32                        GpiValue = 0;
  SYSTEM_CONFIGURATION          SystemConfiguration;
  EFI_STATUS                    Status;
  UINT16                        SleepType;
  EFI_BOOT_MODE                 BootMode;
  UINT8                         WakeupType;

  Status = PeiServicesGetBootMode (&BootMode);
  ASSERT_EFI_ERROR (Status);

  //
  // Updates the wakeupType which will be used to update the same in Smbios table 01
  //
  GetWakeupEvent (&WakeupType);
  PcdSet8S (PcdWakeupType, WakeupType);

  if (IsSimicsEnvironment() == FALSE) {
    Status = GpioGetRcvPadVal (&GpiValue);
    if(Status == EFI_UNSUPPORTED){
      GpiValue = 0;
    }
  }

  if (IsManufacturingMode()) {
    //
    // MFG mode must override all other Boot Modes.
    //
    BootMode = BOOT_WITH_MFG_MODE_SETTINGS;
  } else if (GpiValue == 1) {
    BootMode = BOOT_IN_RECOVERY_MODE;
    Status = PeiServicesInstallPpi (&mPpiListRecoveryBootMode);
    ASSERT_EFI_ERROR (Status);
  } else if (PcdGetBool(PcdBootState)) {
    //
    // If this is first boot after flash, override sleep states.
    //
    BootMode = BOOT_WITH_FULL_CONFIGURATION;

  } else if ((PmcIsRtcBatteryGood () == FALSE) || IsCMOSBad ()) {
    BootMode = BOOT_WITH_DEFAULT_SETTINGS;
    //
    // If the BIOS defaults are loaded,
    // Admin password should be saved (to prevent its reset)
    //
    GetSpecificConfigGuid (&gEfiSetupVariableGuid, &SystemConfiguration);
    SaveAdminPassToHob(PeiServices, &SystemConfiguration);

  } else {
    //
    // Normal boot mode most of the time. Safe to override it with S states below.
    //
    BootMode = BOOT_WITH_MINIMAL_CONFIGURATION;

    if (GetSleepTypeAfterWakeup (PeiServices, &SleepType)) {
      switch (SleepType) {
        case V_ACPI_IO_PM1_CNT_S3:
          BootMode = BOOT_ON_S3_RESUME;
          break;
        case V_ACPI_IO_PM1_CNT_S4:
          BootMode = BOOT_ON_S4_RESUME;
          break;
        case V_ACPI_IO_PM1_CNT_S5:
          BootMode = BOOT_ON_S5_RESUME;
          DEBUG ((DEBUG_INFO, "\nOverriding S5_RESUME with BOOT_WITH_MINIMAL_CONFIGURATION.\n"));
          BootMode = BOOT_WITH_MINIMAL_CONFIGURATION;
          break;
        default:
          DEBUG ((EFI_D_INFO, "default case.\n"));  //Auto added. Please review.
          break;
      }
    }

    if (PmcIsRtcBatteryGood () == FALSE) {
      //
      // Report RTC battery failure
      //
      DEBUG ((DEBUG_ERROR, "RTC Power failure !! Could be due to a weak or missing battery.\n"));

      BootMode = BOOT_WITH_DEFAULT_SETTINGS;
    }
  }

  if (FastBootMode(&SystemConfiguration) == TRUE) {
    BootMode = FAST_BOOT_MODE;
  }

  switch (BootMode) {
    case BOOT_WITH_FULL_CONFIGURATION:
      strBootMode = L"BOOT_WITH_FULL_CONFIGURATION";
      break;
    case BOOT_WITH_MINIMAL_CONFIGURATION:
      strBootMode = L"BOOT_WITH_MINIMAL_CONFIGURATION";
      break;
    case BOOT_ASSUMING_NO_CONFIGURATION_CHANGES:
      strBootMode = L"BOOT_ASSUMING_NO_CONFIGURATION_CHANGES";
      break;
    case BOOT_WITH_FULL_CONFIGURATION_PLUS_DIAGNOSTICS:
      strBootMode = L"BOOT_WITH_FULL_CONFIGURATION_PLUS_DIAGNOSTICS";
      break;
    case BOOT_WITH_DEFAULT_SETTINGS:
      strBootMode = L"BOOT_WITH_DEFAULT_SETTINGS";
      break;
    case BOOT_ON_S4_RESUME:
      strBootMode = L"BOOT_ON_S4_RESUME";
      break;
    case BOOT_ON_S5_RESUME:
      strBootMode = L"BOOT_ON_S5_RESUME";
      break;
    case BOOT_ON_S2_RESUME:
      strBootMode = L"BOOT_ON_S2_RESUME";
      break;
    case BOOT_ON_S3_RESUME:
      strBootMode = L"BOOT_ON_S3_RESUME";
      break;
    case BOOT_ON_FLASH_UPDATE:
      strBootMode = L"BOOT_ON_FLASH_UPDATE";
      break;
    case BOOT_IN_RECOVERY_MODE:
      strBootMode = L"BOOT_IN_RECOVERY_MODE";
      break;
    case BOOT_WITH_MFG_MODE_SETTINGS:
      strBootMode = L"BOOT_WITH_MFG_MODE_SETTINGS";
      break;
    case FAST_BOOT_MODE:
      strBootMode = L"FAST_BOOT_MODE";
      break;
    default:
      strBootMode = L"Unknown boot mode";
  } // switch (BootMode)
  DEBUG ((EFI_D_INFO, "Setting BootMode to %s\n", strBootMode));

  Status = PeiServicesSetBootMode (BootMode);
  ASSERT_EFI_ERROR (Status);

  Status = PeiServicesInstallPpi (&mPpiListUpdateBootMode);
  ASSERT_EFI_ERROR (Status);

  return Status;
}

/**

  Get sleep type after wakeup

  @param PeiServices       Pointer to the PEI Service Table.
  SleepType         Sleep type to be returned.

  @retval TRUE              A wake event occured without power failure.
  @retval FALSE             Power failure occured or not a wakeup.

**/
BOOLEAN
GetSleepTypeAfterWakeup (
  IN  CONST EFI_PEI_SERVICES    **PeiServices,
  OUT UINT16                    *SleepType
  )
{
  UINT16  Pm1Sts;
  UINT16  Pm1Cnt;
  UINTN         PciPmcRegBase;
  PciPmcRegBase = MmPciBase (DEFAULT_PCI_BUS_NUMBER_PCH, PCI_DEVICE_NUMBER_PCH_PMC, PCI_FUNCTION_NUMBER_PCH_PMC);
  //
  // Read the ACPI registers
  //
  Pm1Sts  = IoRead16 (PCH_ACPI_BASE_ADDRESS + R_ACPI_IO_PM1_STS);
  Pm1Cnt  = IoRead16 (PCH_ACPI_BASE_ADDRESS + R_ACPI_IO_PM1_CNT);


  //
  // Get sleep type if a wake event occurred and there is no power failure
  //
  DEBUG ((DEBUG_ERROR, "\nGetSleepTypeAfterWakeup() Pm1Sts = %x, Pm1Cnt = %x\n", Pm1Sts, Pm1Cnt));
  if (((Pm1Sts & B_ACPI_IO_PM1_STS_WAK) != 0) && (PmcIsRtcBatteryGood () == TRUE) && (PmcIsPowerFailureDetected () == FALSE)) {
    *SleepType = Pm1Cnt & B_ACPI_IO_PM1_CNT_SLP_TYP;
    if((*SleepType == V_ACPI_IO_PM1_CNT_S3) && (Pm1Sts & B_ACPI_IO_PM1_STS_PRBTNOR)) {
      DEBUG((DEBUG_ERROR, "Power Button override during S3, force system to S5 boot path\n"));
      *SleepType = V_ACPI_IO_PM1_CNT_S5; // Power button override when S3, force the system to S5 boot path;
      IoWrite16(PCH_ACPI_BASE_ADDRESS + R_ACPI_IO_PM1_STS, (B_ACPI_IO_PM1_STS_WAK));
    }
    return TRUE;
  }
  //
  // Make sure to clear WAK STS bit since we decided it is not returning from S3
  //
  if ((Pm1Sts & B_ACPI_IO_PM1_STS_WAK) != 0) {
    IoWrite16 (PCH_ACPI_BASE_ADDRESS + R_ACPI_IO_PM1_STS, (B_ACPI_IO_PM1_STS_WAK));
    Pm1Cnt &= ~B_ACPI_IO_PM1_CNT_SLP_TYP;
    IoWrite32 (PCH_ACPI_BASE_ADDRESS + R_ACPI_IO_PM1_CNT, Pm1Cnt);
  }
  return FALSE;
}


/**
  Admin password is read and saved to HOB
  to prevent its cleared on defaults load.

  @param PeiServices          - pointer to the PEI Service Table
  @param SystemConfiguration  - pointer to Platform Setup Configuration

  @retval None.

**/
VOID
SaveAdminPassToHob (
  IN CONST EFI_PEI_SERVICES **PeiServices,
  IN SYSTEM_CONFIGURATION   *SystemConfiguration
  )
{
  BuildGuidDataHob (
        &gEfiAdminPasswordHobGuid,
        SystemConfiguration->AdminPassword,
        sizeof (SystemConfiguration->AdminPassword)
        );

}

/**

    Check to see if CMOS is bad or cleared

    @param None

    @retval TRUE  - CMOS is bad
    @retval FALSE - CMOS is good

**/
BOOLEAN
IsCMOSBad(
     VOID
)
{
  UINT8           Nmi;
  volatile UINT32 Data32;
  Nmi     = 0;
  Data32  = 0;
  //
  // Preserve NMI bit setting
  //

  Nmi = (ReadNmiEn () & 0x80);

  IoWrite8 (R_IOPORT_CMOS_STANDARD_INDEX, R_IOPORT_CMOS_IDX_DIAGNOSTIC_STATUS | Nmi);
  if (IoRead8 (R_IOPORT_CMOS_STANDARD_DATA) & (BIT6 + BIT7)) {
    return TRUE;
  } else {
    return FALSE;
  }
}
