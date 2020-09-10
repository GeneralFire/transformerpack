/** @file
 Routines for Rst remapping

@copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2020 Intel Corporation.

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

@par Specification Reference:
**/

#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/SataSocLib.h>
#include <Library/PeiRstPrivateLib.h>
#include <Library/PchPolicyLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/PeiRstPolicyLib.h>
#include <Library/PchInfoLib.h>
#include <Register/PchRegs.h>
#include <PcieRegs.h>
#include <Register/SataRegs.h>
#include <Library/PchPciBdfLib.h>

#include "PeiRstPrivateLibInternal.h"

/**
  Programms the Intel RST Feature Capabilities register to inform the RST driver which
  features are supported.
  Caller of this function is responsible for making sure that AHCI BAR has been set.

  @param[in] AhciBar     AHCI BAR
  @param[in] SataMode    SATA Mode
  @param[in] RstConfig   RST Config
**/
STATIC
VOID
RstInitializeRstFeatures (
  IN UINT32            AhciBar,
  IN SATA_MODE         SataMode,
  IN RST_CONFIG        *RstConfig
  )
{
  UINT16  WordReg;

  // PCH BIOS Spec Section 14.1.5.2 Enable Flexible RAID OROM Features
  // RST features can be customized by programming
  // SW Feature Mask (SFM) register before loading the RAID Option ROM. The RAID
  // OROM will enable the desired features based on the setting in that register, please refer to
  // PCH EDS for more details.
  // For example, if the platform desired features are RAID0, RAID1, RAID5, RAID10 and
  // RST Smart Storage caching. System BIOS should set RSTF (ABAR + C8h [15:0]) to 022Fh before
  // loading RAID OROM.
  //
  WordReg = 0;

  if (RstConfig->HddUnlock == TRUE) {
    //
    // If set to "1", indicates that the HDD password unlock in the OS is enabled
    //
    WordReg |= B_SATA_MEM_AHCI_SFM_HDDLK;
  }

  if (RstConfig->LedLocate == TRUE) {
    //
    // If set to "1", indicates that the LED/SGPIO hardware is attached
    //
    WordReg |= B_SATA_MEM_AHCI_SFM_LEDL;
  }

  if (SataMode == SataModeRaid) {
    if (RstConfig->LegacyOrom == TRUE) {
      //
      // If set to "1", then Legacy OROM is enabled
      //
      WordReg |= B_SATA_MEM_AHCI_SFM_LEGACY;
    }

    if (RstConfig->Raid0 == TRUE) {
      //
      // If set to "1", then RAID0 is enabled in Flexible RAID Option ROM.
      //
      WordReg |= B_SATA_MEM_AHCI_SFM_R0E;
    }

    if (RstConfig->Raid1 == TRUE) {
      //
      // If set to "1", then RAID1 is enabled in FD-OROM.
      //
      WordReg |= B_SATA_MEM_AHCI_SFM_R1E;
    }

    if (RstConfig->Raid10 == TRUE) {
      //
      // If set to "1", then RAID10 is enabled in FD-OROM.
      //
      WordReg |= B_SATA_MEM_AHCI_SFM_R10E;
    }

    if (RstConfig->Raid5 == TRUE) {
      //
      // If set to "1", then RAID5 is enabled in FD-OROM.
      //
      WordReg |= B_SATA_MEM_AHCI_SFM_R5E;
    }

    if (RstConfig->Irrt == TRUE) {
      //
      // If set to "1", then Intel Rapid Recovery Technology is enabled.
      //
      WordReg |= B_SATA_MEM_AHCI_SFM_RSTE;
    }

    if (RstConfig->OromUiBanner == TRUE) {
      //
      // If set to "1" then the OROM UI is shown.  Otherwise, no OROM banner or information
      // will be displayed if all disks and RAID volumes are Normal.
      //
      WordReg |= B_SATA_MEM_AHCI_SFM_IRSTOROM;
    }

    if (RstConfig->IrrtOnly == TRUE) {
      //
      // If set to "1", then only IRRT volumes can span internal and eSATA drives. If cleared
      // to "0", then any RAID volume can span internal and eSATA drives.
      //
      WordReg |= B_SATA_MEM_AHCI_SFM_IROES;
    }
    //
    // Enable the RST Smart Storage caching bit to support Smart Storage caching.
    //
    if (RstConfig->SmartStorage == TRUE) {
      WordReg |= B_SATA_MEM_AHCI_SFM_SEREQ;
    }
    //
    // Program the delay of the OROM UI Splash Screen in a normal status.
    //
    WordReg |= (UINT16) (RstConfig->OromUiDelay << N_SATA_MEM_AHCI_SFM_OUD);

    if (RstConfig->OptaneMemory == TRUE) {
      ///
      /// If set to "1", then Optane(TM) Memory Acceleration is enabled
      ///
      WordReg |= B_SATA_MEM_AHCI_SFM_OMA;
    }

    if (RstConfig->CpuAttachedStorage == TRUE) {
      ///
      /// If set to "1", then CPU Attached Storage feature is enabled
      ///
      WordReg |= B_SATA_MEM_AHCI_SFM_CPU_STORAGE;
    }
  }

  //
  // SW Feature Mask (SFM) is a Write-Once register.
  //
  MmioWrite16 (AhciBar + R_SATA_MEM_AHCI_SFM, WordReg);
}

/**
  Program AHCI Bar

  @param[in]      PciSataRegBase        SATA controller address
  @param[in]      AhciBar               AHCI BAR
**/
STATIC
VOID
RstSetAhciBar (
  IN     UINT64       PciSataRegBase,
  IN     UINT32       AhciBar
  )
{
  ///
  /// Disable command register memory space decoding
  ///
  PciSegmentAnd16 (PciSataRegBase + PCI_COMMAND_OFFSET, (UINT16) ~EFI_PCI_COMMAND_MEMORY_SPACE);

  ///
  /// Set the AHCI BAR
  ///

  PciSegmentWrite32 (PciSataRegBase + R_SATA_CFG_AHCI_BAR, AhciBar);

  ///
  /// Enable command register memory space decoding
  ///
  PciSegmentOr16 (PciSataRegBase + PCI_COMMAND_OFFSET, (UINT16) EFI_PCI_COMMAND_MEMORY_SPACE);
}

/**
  Clear AHCI Bar

  @param[in]      PciSataRegBase        SATA controller address
**/
STATIC
VOID
RstClearAhciBar (
  IN     UINT64       PciSataRegBase
  )
{
  ///
  /// Disable command register memory space decoding
  ///
  PciSegmentAnd16 (PciSataRegBase + PCI_COMMAND_OFFSET, (UINT16) ~EFI_PCI_COMMAND_MEMORY_SPACE);

  ///
  /// Set Ahci Bar to zero
  ///
  PciSegmentWrite32 (PciSataRegBase + R_SATA_CFG_AHCI_BAR, 0);
}

/**
  Function to configure RST (Intel RST Driver is required)

  @param[in] SiPolicy         The Silicon Policy PPI instance

**/
VOID
ConfigureRst (
  IN SI_POLICY_PPI    *SiPolicy
  )
{
  UINT64                PciSataRegBase;
  UINT32                AhciBar;
  SATA_CONFIG           *SataConfig;
  RST_CONFIG            *RstConfig;
  BOOLEAN               SataHardwareEnabled;
  UINT32                SataCtrlIndex;

  DEBUG ((DEBUG_INFO, "ConfigureRst (): Started\n"));

  for (SataCtrlIndex = 0; SataCtrlIndex < MaxSataControllerNum (); SataCtrlIndex++) {
    DEBUG ((DEBUG_INFO, "ConfigureRst for Controller %d\n", SataCtrlIndex));

    SataConfig                  = GetPchSataConfig (SiPolicy, SataCtrlIndex);
    RstConfig                   = GetPchRstConfig (SiPolicy, SataCtrlIndex);
    PciSataRegBase              = SataPciCfgBase (SataCtrlIndex);

    AhciBar = PcdGet32 (PcdSiliconInitTempMemBaseAddr);

    SataHardwareEnabled = (PciSegmentRead16 (PciSataRegBase + PCI_DEVICE_ID_OFFSET) != 0xFFFF);

    if (SataHardwareEnabled) {
      RstSetAhciBar (PciSataRegBase, AhciBar);

      RstInitializeRstFeatures (AhciBar, SataConfig->SataMode, RstConfig);
      RstConfigurePcieStorageRemapping (SiPolicy, PciSataRegBase, AhciBar, SataConfig, RstConfig);

      RstClearAhciBar (PciSataRegBase);
    }
  }
  DEBUG ((DEBUG_INFO, "ConfigureRst: ConfigureRst() Ended\n"));
}

