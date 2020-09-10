/** @file
  Initializes/updates PCH related HOBs in PEI

@copyright
  INTEL CONFIDENTIAL
  Copyright 2019 - 2020 Intel Corporation.

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

#include <Library/GbeLib.h>
#include <Library/SataSocLib.h>
#include "PchInitPei.h"
#include <PchPcieRpInfo.h>
#include <Register/PchPcieRpRegs.h>
#include <Register/P2sbRegs.h>
#include <Library/PeiEnterprisePcieRpInitLib.h>

/**
  The function update pch info hob in the end of PchInit.
**/
VOID
BuildPchInfoHob (
  VOID
  )
{
  PCH_RP_HANDLE             RpHandle[PCH_MAX_PCIE_ROOT_PORTS];
  PCH_RP_PRIVATE_CONFIG     RpPrivate[PCH_MAX_PCIE_ROOT_PORTS];
  PCH_RP_DEVICE             RpDevice[PCH_MAX_PCIE_ROOT_PORTS];
  PCH_RP_CALLBACK           RpCallback[PCH_MAX_PCIE_ROOT_PORTS];
  EFI_STATUS                Status;
  UINT32                    RpIndex;
  UINT32                    PhysLane;
  UINT8                     LaneNum;
  PCH_INFO_HOB              *PchInfoHob;
  BOOLEAN                   CridSupport;
  UINT8                     OrgRid;
  UINT8                     NewRid;

  PchInfoHob = BuildGuidHob (&gPchInfoHobGuid, sizeof (PCH_INFO_HOB));
  ASSERT (PchInfoHob != 0);
  if (PchInfoHob == NULL) {
    return;
  }

  PchInfoHob->Revision = PCH_INFO_HOB_REVISION;

  PchInfoHob->PciePortLaneEnabled = 0;
  for (RpIndex = 0; RpIndex < GetPchMaxPciePortNum (); RpIndex++) {
    PchRpHandleInit (NULL, NULL, RpIndex, &RpHandle[RpIndex], &RpPrivate[RpIndex], &RpDevice[RpIndex], &RpCallback[RpIndex]);
  }

  for (RpIndex = 0; RpIndex < GetPchMaxPciePortNum (); RpIndex++) {
    PhysLane = PchPciePhysicalLane (&RpHandle[RpIndex], &RpHandle[RpDevice[RpIndex].CtrlIndex * PCH_PCIE_CONTROLLER_PORTS], 0);
    if (PchFiaGetPcieLaneNum (PhysLane, &LaneNum)) {
      PchInfoHob->PciePortLaneEnabled |= BIT0 << RpIndex;
    }
  }

  PchInfoHob->HpetBusNum    = V_P2SB_CFG_HBDF_BUS;
  PchInfoHob->HpetDevNum    = V_P2SB_CFG_HBDF_DEV;
  PchInfoHob->HpetFuncNum   = V_P2SB_CFG_HBDF_FUNC;
  PchInfoHob->IoApicBusNum  = V_P2SB_CFG_IBDF_BUS;
  PchInfoHob->IoApicDevNum  = V_P2SB_CFG_IBDF_DEV;
  PchInfoHob->IoApicFuncNum = V_P2SB_CFG_IBDF_FUNC;

  //
  // This must be done before PMC hidden.
  //
  CridSupport = FALSE;
  OrgRid      = 0;
  NewRid      = 0;
  Status = PmcDetectCrid0 (&CridSupport, &OrgRid, &NewRid);
  DEBUG ((DEBUG_INFO, "CridSupport = %x %x %x\n", CridSupport, OrgRid, NewRid));
  if (!EFI_ERROR (Status)) {
    PchInfoHob->CridSupport = CridSupport;
    PchInfoHob->CridOrgRid  = OrgRid;
    PchInfoHob->CridNewRid  = NewRid;
  }
}

/**
  The function update pch config hob in the end of PchInit.

  @param[in]      SiPolicy               The SI Policy PPI instance
**/
VOID
BuildPchConfigHob (
  IN     SI_POLICY_PPI          *SiPolicy
  )
{
  PCH_CONFIG_HOB              *PchConfigHob;
  SI_PREMEM_POLICY_PPI        *SiPreMemPolicy;
  PCH_SMBUS_PREMEM_CONFIG     *SmbusPreMemConfig;
  PCH_GENERAL_CONFIG          *PchGeneralConfig;
  PCH_INTERRUPT_CONFIG        *InterruptConfig;
  SERIAL_IO_CONFIG            *SerialIoConfig;
  PCH_PCIE_CONFIG             *PcieRpConfig;
  PCH_DMI_CONFIG              *PchDmiConfig;
  PCH_LOCK_DOWN_CONFIG        *LockDownConfig;
  PCH_PM_CONFIG               *PmConfig;
  SATA_CONFIG                 *SataConfig;
  RST_CONFIG                  *RstConfig;
  PCH_FLASH_PROTECTION_CONFIG *FlashProtectionConfig;
  GBE_CONFIG                  *GbeConfig;
  PCH_TRACE_HUB_PREMEM_CONFIG *PchTraceHubPreMemConfig;
  EFI_STATUS                  Status;
  UINT32                      SataCtrlIndex;
  PCH_ESPI_CONFIG             *EspiConfig;

  PchConfigHob = BuildGuidHob (&gPchConfigHobGuid, sizeof (PCH_CONFIG_HOB));
  ASSERT (PchConfigHob != 0);
  if (PchConfigHob == NULL) {
    return;
  }

  //
  // Get Policy settings through the SiPreMemPolicy PPI
  //
  Status = PeiServicesLocatePpi (
             &gSiPreMemPolicyPpiGuid,
             0,
             NULL,
             (VOID **) &SiPreMemPolicy
             );
  if (Status != EFI_SUCCESS) {
    //
    // SI_PREMEM_POLICY_PPI must be installed at this point
    //
    ASSERT (FALSE);
    return;
  }
  Status = GetConfigBlock ((VOID *) SiPreMemPolicy, &gSmbusPreMemConfigGuid, (VOID *) &SmbusPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPreMemPolicy, &gPchTraceHubPreMemConfigGuid, (VOID *) &PchTraceHubPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gPchGeneralConfigGuid, (VOID *) &PchGeneralConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gInterruptConfigGuid, (VOID *) &InterruptConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gSerialIoConfigGuid, (VOID *) &SerialIoConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gPchPcieConfigGuid, (VOID *) &PcieRpConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gPchDmiConfigGuid, (VOID *) &PchDmiConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gLockDownConfigGuid, (VOID *) &LockDownConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gPmConfigGuid, (VOID *) &PmConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gFlashProtectionConfigGuid, (VOID *) &FlashProtectionConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gGbeConfigGuid, (VOID *) &GbeConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gEspiConfigGuid, (VOID *) &EspiConfig);
  ASSERT_EFI_ERROR (Status);

  PchConfigHob->Smbus.NumRsvdSmbusAddresses = SmbusPreMemConfig->NumRsvdSmbusAddresses;
  CopyMem (
    PchConfigHob->Smbus.RsvdSmbusAddressTable,
    SmbusPreMemConfig->RsvdSmbusAddressTable,
    sizeof (PchConfigHob->Smbus.RsvdSmbusAddressTable)
    );

  PchConfigHob->General.Crid = PchGeneralConfig->Crid;
  PchConfigHob->Interrupt.NumOfDevIntConfig = InterruptConfig->NumOfDevIntConfig;
  PchConfigHob->Interrupt.GpioIrqRoute = InterruptConfig->GpioIrqRoute;
  CopyMem (
    PchConfigHob->Interrupt.DevIntConfig,
    InterruptConfig->DevIntConfig,
    sizeof (PchConfigHob->Interrupt.DevIntConfig)
    );

  CopyMem (
    PchConfigHob->SerialIo.UartDeviceConfig,
    SerialIoConfig->UartDeviceConfig,
    sizeof (PchConfigHob->SerialIo.UartDeviceConfig)
    );

  CopyMem (
    PchConfigHob->PcieRp.RootPort,
    PcieRpConfig->RootPort,
    sizeof (PchConfigHob->PcieRp.RootPort)
    );

  PchConfigHob->LockDown.GlobalSmi = LockDownConfig->GlobalSmi;
  PchConfigHob->LockDown.BiosInterface = LockDownConfig->BiosInterface;
  PchConfigHob->LockDown.BiosLock = LockDownConfig->BiosLock;
  PchConfigHob->Pm.PsOnEnable            = PmConfig->PsOnEnable;
#if FixedPcdGet8(PcdEmbeddedEnable) == 0x1
  PchConfigHob->Pm.EnableTimedGpio0      = PmConfig->EnableTimedGpio0;
  PchConfigHob->Pm.EnableTimedGpio1      = PmConfig->EnableTimedGpio1;
#endif
  PchConfigHob->Espi.BmeMasterSlaveEnabled = EspiConfig->BmeMasterSlaveEnabled;
  for (SataCtrlIndex = 0; SataCtrlIndex < MaxSataControllerNum (); SataCtrlIndex++) {
    SataConfig = GetPchSataConfig (SiPolicy, SataCtrlIndex);
    PchConfigHob->Sata[SataCtrlIndex].Enable = SataConfig->Enable;
    PchConfigHob->Sata[SataCtrlIndex].TestMode = SataConfig->TestMode;
    CopyMem (
      PchConfigHob->Sata[SataCtrlIndex].PortSettings,
      SataConfig->PortSettings,
      sizeof (PchConfigHob->Sata[SataCtrlIndex].PortSettings)
      );
  }
  SataCtrlIndex = 0;
  RstConfig = GetPchRstConfig (SiPolicy, SataCtrlIndex);
  CopyMem (
    PchConfigHob->Rst.RstHardwareRemappedStorageConfig,
    RstConfig->HardwareRemappedStorageConfig,
    sizeof (PchConfigHob->Rst.RstHardwareRemappedStorageConfig)
    );

  CopyMem (
    PchConfigHob->ProtectRange,
    FlashProtectionConfig->ProtectRange,
    sizeof (PchConfigHob->ProtectRange)
    );

  PchConfigHob->PchTraceHub.PchTraceHubMode = PchTraceHubPreMemConfig->TraceHub.EnableMode;
  PchConfigHob->PchTraceHub.MemReg0Size     = PchTraceHubPreMemConfig->TraceHub.MemReg0Size;
  PchConfigHob->PchTraceHub.MemReg1Size     = PchTraceHubPreMemConfig->TraceHub.MemReg1Size;
  PchConfigHob->PchTraceHub.AetEnableMode   = PchTraceHubPreMemConfig->TraceHub.AetEnabled;

}
