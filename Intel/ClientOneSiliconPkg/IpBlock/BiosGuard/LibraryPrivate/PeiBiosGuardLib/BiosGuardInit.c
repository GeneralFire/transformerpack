/** @file
  BIOS Guard EarlyPost initializations.

@copyright
  INTEL CONFIDENTIAL
  Copyright 2011 - 2020 Intel Corporation.

  The source code contained or described herein and all documents related to the
  source code ("Material") are owned by Intel Corporation or its suppliers or
  licensors. Title to the Material remains with Intel Corporation or its suppliers
  and licensors. The Material may contain trade secrets and proprietary and
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

  This file contains an 'Intel Peripheral Driver' and is uniquely identified as
  "Intel Reference Module" and is licensed for Intel CPUs and chipsets under
  the terms of your license agreement with Intel or your vendor. This file may
  be modified by the user, subject to additional terms of the license agreement.

@par Specification
**/

#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/RngLib.h>
#include <Library/CpuPlatformLib.h>
#include <Library/CpuCommonLib.h>
#include <Library/BiosGuardInit.h>
#include <Library/SoftwareGuardLib.h>
#include "CpuAccess.h"
#include <Library/ConfigBlockLib.h>
#include <Library/PostCodeLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/HobLib.h>
#include <IndustryStandard/FirmwareVersionInfo.h>
#include <Library/PmcLib.h>
#include <Register/PmcRegs.h>
#include <Register/CommonMsr.h>
#include <Library/IoLib.h>
#include <Library/MsrFruLib.h>

#define  BIOS_GUARD_STRING       "BIOS Guard"

/**
  Check if BIOS Guard Module is Debug Signed

  @param[in] ModulePtr - Input is the BIOS Guard module address.

  @retval TRUE  - BIOS Guard Module is Debug Signed
  @retval FALSE - BIOS Guard Module is Production Signed
**/
BOOLEAN
IsBiosGuardModuleDebugSigned (
  IN EFI_PHYSICAL_ADDRESS   ModulePtr
  )
{
  UINT32                  ModuleId;

  ///
  /// Read BIOS Guard Module ID - (Size of BIOS HEADER (128 Bytes) + Offset of Internal Header Module ID (12 Bytes))
  ///
  ModuleId = *(UINT32*)(UINTN)(ModulePtr + BIOSGUARD_INTERNAL_HEADER_MODULE_ID_OFFSET);
  DEBUG ((DEBUG_INFO, "BIOS Guard Module ID = 0x%X\n", ModuleId));

  ///
  /// Check if Module is Debug Signed
  ///
  return ((ModuleId & BIT31) != 0) ? TRUE : FALSE;
}

/**
  Perform BIOS Guard initialization.

  @param[in] SiPolicyPpi  - SI Policy PPI

  @retval VOID - No value to return
**/
VOID
BiosGuardInit (
  IN SI_POLICY_PPI     *SiPolicyPpi
  )
{
  EFI_STATUS                        Status;
  MSR_BIOS_INFO_FLAGS_REGISTER      MsrBiosInfoFlags;
  BOOLEAN                           ResetRequired;
  UINT8                             Index;
  volatile UINT32                   EphemeralAuthValue;
  UINT8                             EcData;
  PLATFORM_SEND_EC_COMMAND          SendEcCmd;
  EFI_BOOT_MODE                     BootMode;
  BIOS_GUARD_CONFIG                 *BiosGuardConfig;
  SI_PREMEM_POLICY_PPI              *SiPreMemPolicyPpi;
  CPU_SECURITY_PREMEM_CONFIG        *CpuSecurityPreMemConfig;
  BOOLEAN                           RngSuccess;
  SMBIOS_TABLE_TYPE_OEM_INTEL_FVI   *BiosGuardFviRecord;
  UINT8                             BiosGuardFviRecordSize;
  INTEL_FIRMWARE_VERSION_INFO       BiosGuardFviData;
  UINT8                             BiosGuardFviCount;
  UINTN                             *DestPtr;
  VOID                              *BiosGuardFviHobPtr;
  UINT16                            Pm1Sts;
  UINT16                            Pm1Cnt;
  UINT16                            AcpiBase;
  MSR_PLAT_FRMW_PROT_CTRL_REGISTER  PlatFrmwProtCtrl;
  RngSuccess             = FALSE;
  BiosGuardFviCount      = 1;
  BiosGuardFviRecordSize = sizeof (SMBIOS_STRUCTURE) + sizeof (UINT8) + (BiosGuardFviCount * sizeof (INTEL_FIRMWARE_VERSION_INFO)) + sizeof (BIOS_GUARD_STRING) + sizeof (UINT8);
  DestPtr                = NULL;
  BiosGuardFviHobPtr     = NULL;
  Pm1Sts                 = 0;
  Pm1Cnt                 = 0;
  AcpiBase               = 0;

  DEBUG ((DEBUG_INFO, "BiosGuardInit Start \n"));
  PostCode (0xC28);

  Status = PeiServicesLocatePpi (
                &gSiPreMemPolicyPpiGuid,
                0,
                NULL,
                (VOID **) &SiPreMemPolicyPpi
                );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "gSiPreMemPolicyPpiGuid Service was not located\n"));
    ASSERT_EFI_ERROR (Status);
    return;
  }

  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gCpuSecurityPreMemConfigGuid, (VOID *) &CpuSecurityPreMemConfig);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Unable to Get gCpuSecurityPreMemConfig block\n"));
    ASSERT_EFI_ERROR (Status);
    return;
  }

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gBiosGuardConfigGuid, (VOID *) &BiosGuardConfig);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Unable to Get gBiosGuardConfig block\n"));
    ASSERT_EFI_ERROR (Status);
    return;
  }

  ResetRequired = FALSE;
  if (!MsrIsPfatEnabled ()) {
    DEBUG ((DEBUG_INFO, "BIOS Guard Feature is not supported\n"));
    return;
  }
  if (CpuSecurityPreMemConfig->BiosGuard == TRUE) {
    DEBUG ((DEBUG_INFO, "BIOS Guard Module is Enabled\n"));
    DEBUG ((DEBUG_INFO, "BIOS Guard Attributes = %x\n", BiosGuardConfig->BiosGuardAttr));

    SendEcCmd = (PLATFORM_SEND_EC_COMMAND) (UINTN) BiosGuardConfig->SendEcCmd;

    ///
    /// Read BIOS Guard Control Register
    ///
    PlatFrmwProtCtrl.Uint64 = AsmReadMsr64 (MSR_PLAT_FRMW_PROT_CTRL);
    if (PlatFrmwProtCtrl.Bits.PfatLock == 1) {
      if (PlatFrmwProtCtrl.Bits.PfatEnable == 0) {
        ///
        /// Reset required as the BIOS Guard CTRL MSR is locked and needs to be toggled
        ///
        DEBUG ((DEBUG_INFO, "Reset required to unlock BIOS Guard CONTROL MSR in order to Enable BIOS Guard\n"));
        ResetRequired = TRUE;
      }
    } else {
      if ((BiosGuardConfig->BiosGuardAttr & EnumEcPresent) && (BiosGuardConfig->BiosGuardAttr & EnumEcBiosGuardProtected) && (SendEcCmd != NULL)) {
        DEBUG ((DEBUG_INFO, "EC is Present and EC FW supports BIOS Guard\n"));

        Status = SendEcCmd (SendEcCmds, BiosGuardConfig->EcCmdProvisionEav, 0 , NULL);
        if (Status == EFI_SUCCESS) {
          RngSuccess = GetRandomNumber32 ((UINT32 *)&EphemeralAuthValue);
          if (!RngSuccess) {
            DEBUG ((DEBUG_ERROR, "Fail to get a 32-bits random number!!!\n"));
          } else {
            DEBUG ((DEBUG_INFO, "BIOSGUARD:EphemeralAuthValue = %x\n", EphemeralAuthValue));
          }

          for (Index = 0; (Index < (sizeof (EphemeralAuthValue))); Index++) {
            Status = SendEcCmd (SendEcValue, 0, (UINT8)((EphemeralAuthValue >> (Index * 8)) & 0xFF), NULL);
            if (EFI_ERROR (Status)) {
              DEBUG ((DEBUG_ERROR, "BIOSGUARD:EphemeralAuthValue SendEcValue index %d Command Failed\n", Index));
            }
          }

          Status = SendEcCmd (ReceiveEcValue, 0, 0, &EcData);
          if (EFI_ERROR (Status)) {
            DEBUG ((DEBUG_ERROR, "BIOSGUARD:ReceiveEcCmd Command Failed\n"));
          }

          ///
          /// BIOS will assume all non-zero values indicate provisioning failure and require a reset
          ///
          if (EcData != 0) {
            DEBUG ((DEBUG_INFO, "Reset required due to provisioning failure\n"));
            ResetRequired = TRUE;
          }

          AsmWriteMsr32 (MSR_PFAT_EC_PASSWORD, EphemeralAuthValue);
          EphemeralAuthValue = 0;
          Status = SendEcCmd (SendEcCmds, BiosGuardConfig->EcCmdLock, 0, NULL);
          if (EFI_ERROR (Status)) {
            DEBUG ((DEBUG_ERROR, "BIOSGUARD:EcCmdLock Send Command Failed\n"));
          }

          Status = SendEcCmd (ReceiveEcValue, 0, 0, &EcData);
          if (EFI_ERROR (Status)) {
            DEBUG ((DEBUG_ERROR, "BIOSGUARD:EcCmdLock Recieve Command Failed\n", Index));
          }

          ///
          /// BIOS will assume all non-zero values indicate unsuccessful configuration lock and require a reset
          ///
          if (EcData != 0) {
            DEBUG ((DEBUG_INFO, "Reset required due to unsuccessful configuration lock\n"));
            ResetRequired = TRUE;
          }
        } else {
          DEBUG ((DEBUG_ERROR, "EC is not detected or BIOS Guard doesn't protect EC.\n"));
        }
      }
      AsmWriteMsr64 (MSR_PLAT_FRMW_PROT_HASH_0, BiosGuardConfig->BgpdtHash[0]);
      AsmWriteMsr64 (MSR_PLAT_FRMW_PROT_HASH_1, BiosGuardConfig->BgpdtHash[1]);
      AsmWriteMsr64 (MSR_PLAT_FRMW_PROT_HASH_2, BiosGuardConfig->BgpdtHash[2]);
      AsmWriteMsr64 (MSR_PLAT_FRMW_PROT_HASH_3, BiosGuardConfig->BgpdtHash[3]);
      PlatFrmwProtCtrl.Bits.PfatEnable = 1;
      PlatFrmwProtCtrl.Bits.PfatLock = 1;
      AsmWriteMsr64 (MSR_PLAT_FRMW_PROT_CTRL, PlatFrmwProtCtrl.Uint64);
    }

    //
    // Initialize BIOS Guard FVI record.
    //
    BiosGuardFviRecord = (SMBIOS_TABLE_TYPE_OEM_INTEL_FVI *) AllocateZeroPool (BiosGuardFviRecordSize);
    if (BiosGuardFviRecord != NULL) {
      BiosGuardFviRecord->Header.Type   = PcdGet8 (PcdFviSmbiosType);
      BiosGuardFviRecord->Header.Length = sizeof (SMBIOS_STRUCTURE) + sizeof (UINT8) + (BiosGuardFviCount * sizeof (INTEL_FIRMWARE_VERSION_INFO));
      BiosGuardFviRecord->Header.Handle = 0;
      BiosGuardFviRecord->Count         = BiosGuardFviCount;

      //
      // Initialize BIOS Guard FVI data.
      //
      BiosGuardFviData.ComponentName        = 1;
      BiosGuardFviData.VersionString        = 0;
      BiosGuardFviData.Version.MajorVersion = *(UINT8 *)(UINTN)(BiosGuardConfig->BiosGuardModulePtr + BIOSGUARD_REVISION_ID_MAJOR_OFFSET);
      BiosGuardFviData.Version.MinorVersion = *(UINT8 *)(UINTN)(BiosGuardConfig->BiosGuardModulePtr + BIOSGUARD_REVISION_ID_MINOR_OFFSET);
      BiosGuardFviData.Version.Revision     = 0;
      BiosGuardFviData.Version.BuildNumber  = *(UINT16 *)(UINTN)(BiosGuardConfig->BiosGuardModulePtr + BIOSGUARD_BUILD_NUMBER_OFFSET);

      //
      // Concatenate FVI data to FVI record
      //
      DestPtr = (UINTN *)((UINTN)BiosGuardFviRecord + sizeof (SMBIOS_STRUCTURE) + sizeof (UINT8));
      CopyMem (DestPtr, &BiosGuardFviData, sizeof (INTEL_FIRMWARE_VERSION_INFO));

      //
      // Concatenate FVI strings to FVI record
      //
      DestPtr = (UINTN *)((UINTN)DestPtr + (BiosGuardFviCount * sizeof (INTEL_FIRMWARE_VERSION_INFO)));
      CopyMem (DestPtr, BIOS_GUARD_STRING, sizeof (BIOS_GUARD_STRING));

      BiosGuardFviHobPtr = BuildGuidDataHob (
                             &gIntelSmbiosDataHobGuid,
                             (VOID *) BiosGuardFviRecord,
                             BiosGuardFviRecordSize
                             );
      ASSERT (BiosGuardFviHobPtr != NULL);

      FreePool (BiosGuardFviRecord);
    } else {
      ASSERT_EFI_ERROR (EFI_OUT_OF_RESOURCES);
    }

    ///
    /// Check if BIOS is on S3 flow
    ///
    Status = PeiServicesGetBootMode (&BootMode);
    if ((Status == EFI_SUCCESS) && (BootMode == BOOT_ON_S3_RESUME)) {
      ///
      /// Enable BIOS Guard Flash Wear-Out Protection mitigation on S3 flow.
      ///
      if (BiosGuardConfig->BiosGuardAttr & EnumFlashwearoutProtection) {
        MsrBiosInfoFlags.Uint64 = AsmReadMsr64 (MSR_BIOS_INFO_FLAGS);
        MsrBiosInfoFlags.Bits.BiosInfoFlagsData = 1;
        AsmWriteMsr64 (MSR_BIOS_INFO_FLAGS, MsrBiosInfoFlags.Uint64);
      }
    }
  } else {
    DEBUG ((DEBUG_INFO, "BIOS Guard Module is Disabled\n"));
    PlatFrmwProtCtrl.Uint64 = AsmReadMsr64 (MSR_PLAT_FRMW_PROT_CTRL);
    if (PlatFrmwProtCtrl.Bits.PfatLock == 1) {
      if (PlatFrmwProtCtrl.Bits.PfatEnable == 1) {
        ///
        /// Reset required as the BIOS Guard CTRL MSR is locked and needs to be toggled
        ///
        DEBUG ((DEBUG_INFO, "Reset required to unlock BIOS Guard CONTROL MSR in order to Disable BIOS Guard\n"));
        ResetRequired = TRUE;
      }
    } else {
      PlatFrmwProtCtrl.Bits.PfatEnable = 0;
      PlatFrmwProtCtrl.Bits.PfatLock = 1;
      AsmWriteMsr64 (MSR_PLAT_FRMW_PROT_CTRL, PlatFrmwProtCtrl.Uint64);
    }
  }

  if (ResetRequired) {
    ///
    /// Perform Cold Reset
    /// In order to account for a reset during sleep states, clear WAK_STS, SLP_EN, and SLP_TYP
    ///
    DEBUG ((DEBUG_INFO, "BiosGuardInit Reset \n"));

    AcpiBase = PmcGetAcpiBase ();

    ///
    /// WAK_STS can only be cleared by setting the bit
    ///
    DEBUG ((DEBUG_INFO, "Clear WAK_STS before CF9 Cold Reset\n"));
    Pm1Sts = IoRead16 ((AcpiBase + R_ACPI_IO_PM1_STS));
    Pm1Sts |= B_ACPI_IO_PM1_STS_WAK;

    DEBUG ((DEBUG_INFO, "Clear SLP_EN and SLP_TYP before CF9 Cold Reset\n"));
    Pm1Cnt = IoRead16 ((AcpiBase + R_ACPI_IO_PM1_CNT));
    Pm1Cnt &= ~(B_ACPI_IO_PM1_CNT_SLP_EN | B_ACPI_IO_PM1_CNT_SLP_TYP);

    IoWrite16 ((AcpiBase + R_ACPI_IO_PM1_STS), (UINT16) Pm1Sts);
    IoWrite16 ((AcpiBase + R_ACPI_IO_PM1_CNT), (UINT16) Pm1Cnt);

    PostCode (0xC29);
    PerformWarmOrColdReset (COLD_RESET);
  }

  return;
}
