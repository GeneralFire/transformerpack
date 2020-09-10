/** @file
  This is the driver which installs protocol for updating ME SMBIOS tables.

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

  This file contains a 'Sample Driver' and is licensed as such under the terms
  of your license agreement with Intel or your vendor. This file may be modified
  by the user, subject to the additional terms of the license agreement.

@par Specification Reference:
**/

#include <Base.h>
#include <Uefi.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/BaseLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/GbeLib.h>
#include <Library/HobLib.h>
#include <Library/PchPciBdfLib.h>
#include <Library/MeUtilsLib.h>
#include <Library/SetupLib.h>
#include <Library/MeTypeLib.h>
#include <Protocol/MeSmbiosUpdateProtocol.h>
#include <Protocol/HeciControlProtocol.h>
#include <Guid/SocketProcessorCoreVariable.h>
#include <Guid/SocketIioVariable.h>
#include <Register/PchRegs.h>
#include <Register/Msr.h>
#include <Register/Cpuid.h>
#include <Register/PchRegsLpc.h>
#include <MeBiosPayloadHob.h>
#include <MeSmbios.h>
#include <MebxDataHob.h>

EFI_STATUS
UpdateSmbios130 (
  VOID
  );

EFI_STATUS
UpdateSmbios131 (
  VOID
  );

ME_SMBIOS_TABLES_UPDATE_PROTOCOL  mMeSmbiosTablesUpdate = {
  UpdateSmbios130,
  UpdateSmbios131,
};


/**
  MBP gets Firmware update info from ME client

  @param[in] MeCapability         Structure of FirmwareUpdateInfo

  @retval EFI_SUCCESS             Get ME Firmware success
  @retval EFI_UNSUPPORTED         No MBP Data Protocol available
**/
EFI_STATUS
MbpGetMeFwInfo (
  IN OUT ME_CAP *MeCapability
  )
{
  MEFWCAPS_SKU          FwCapsSku;
  ME_BIOS_PAYLOAD_HOB   *MbpHob;

  MbpHob = NULL;

  //
  // Get Mbp Data HOB
  //
  MbpHob = GetFirstGuidHob (&gMeBiosPayloadHobGuid);
  if (MbpHob == NULL) {
    DEBUG ((DEBUG_ERROR, "HeciGetMeFwInfo: No MBP Data Protocol available\n"));
    return EFI_UNSUPPORTED;
  }

  MeCapability->MeEnabled = 1;

  FwCapsSku.Data          = MbpHob->MeBiosPayload.FwCapsSku.FwCapabilities.Data;

  if (FwCapsSku.Fields.KVM) {
    MeCapability->IntelKVM = 1;
  }

  switch (MbpHob->MeBiosPayload.FwPlatType.RuleData.Fields.PlatformBrand) {
    case IntelAmtBrand:
      MeCapability->IntelAmtFw        = 1;
      MeCapability->LocalWakeupTimer  = 1;
      break;

    case IntelStandardManageabilityBrand:
      MeCapability->IntelAmtFwStandard = 1;
      break;
  }

  MeCapability->MeMajorVer  = MbpHob->MeBiosPayload.FwVersionName.MajorVersion;
  MeCapability->MeMinorVer  = MbpHob->MeBiosPayload.FwVersionName.MinorVersion;
  MeCapability->MeBuildNo   = MbpHob->MeBiosPayload.FwVersionName.BuildVersion;
  MeCapability->MeHotFixNo  = MbpHob->MeBiosPayload.FwVersionName.HotfixVersion;

  return EFI_SUCCESS;
}

/**
  Returns the SMBIOS table with the specified type.

  @param[in]        Type            The type of the SMBIOS table which is searched.
  @param[out]       Record          If the functions finishes with success, then it will point the found SMBIOS Record.

  @retval EFI_SUCCESS               Record was found.
  @retval EFI_INVALID_PARAMETER     Table pointer is NULL.
  @retval EFI_NOT_FOUND             Record cannot be found.
**/
EFI_STATUS
GetSmbiosTable (
  IN     SMBIOS_TYPE              Type,
     OUT EFI_SMBIOS_TABLE_HEADER  **Table
  )
{
  EFI_STATUS                 Status;
  EFI_SMBIOS_HANDLE          SmbiosHandle;
  EFI_SMBIOS_PROTOCOL        *Smbios;
  EFI_SMBIOS_TABLE_HEADER    *Record;

  if (Table == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  *Table = NULL;

  Status = gBS->LocateProtocol (
                  &gEfiSmbiosProtocolGuid,
                  NULL,
                  (VOID**) &Smbios
                  );
  ASSERT_EFI_ERROR (Status);

  if (EFI_ERROR (Status)) {
    return Status;
  }

  SmbiosHandle = SMBIOS_HANDLE_PI_RESERVED;

  do {
    Status = Smbios->GetNext (
                       Smbios,
                       &SmbiosHandle,
                       NULL,
                       &Record,
                       NULL
                       );
    if (EFI_ERROR (Status)) {
      return EFI_NOT_FOUND;
    }
  } while (Record->Type != Type);

  *Table = Record;

  return EFI_SUCCESS;
}

/**
  This function makes boot time changes to the content of the SMBIOS type 130.

  @retval EFI_SUCCESS           Record was updated successfully.
  @retval EFI_NOT_FOUND         HECI protocol or SMBIOS type 130 record cannot be found.
**/
EFI_STATUS
EFIAPI
UpdateSmbios130 (
  VOID
  )
{
  EFI_STATUS                 Status;
  EFI_SMBIOS_TABLE_HEADER    *Record;
  ME_CAP                     MeCapabilities;
  UINT32                     MeMode;
  HECI_CONTROL               *Heci;
#if ME_TESTMENU_FLAG
  ME_RC_CONFIGURATION        MeSetup;
#endif // ME_TESTMENU_FLAG
  EFI_HOB_GUID_TYPE          *GuidHob;
  MEBX_DATA_HOB              MebxData;
  SMBIOS_TABLE_TYPE130       *SmbiosTableType130Data;

  DEBUG ((DEBUG_INFO, "%a() entry \n", __FUNCTION__));

  Status = GetSmbiosTable (MESMBIOS_TYPE_130, &Record);

  if (EFI_ERROR (Status)) {
    return EFI_NOT_FOUND;
  }

  SmbiosTableType130Data = (SMBIOS_TABLE_TYPE130*) Record;

  ZeroMem (&MebxData, sizeof (MEBX_DATA_HOB));
  GuidHob = GetFirstGuidHob (&gAmtMebxDataGuid);
  if (GuidHob != NULL) {
    CopyMem (&MebxData, GET_GUID_HOB_DATA (GuidHob), sizeof (MEBX_DATA_HOB));
  }

  ZeroMem (&MeCapabilities, sizeof (ME_CAP));

  Status = gBS->LocateProtocol (&gHeciControlProtocolGuid, NULL, &Heci);

  if (!EFI_ERROR (Status)) {
    MeBiosGetMeMode (&MeMode);
    if (MeMode == ME_MODE_NORMAL) {
      MbpGetMeFwInfo (&MeCapabilities);
    }
  } else {
    DEBUG ((DEBUG_ERROR, "HECI protocol not found!\n"));
  }

  if (MeCapabilities.IntelAmtFw == 1) {
    SmbiosTableType130Data->AmtSupported = 1;
    SmbiosTableType130Data->SrouEnabled = 1;     // If AMT supported, SRoU is always enabled
  } else {
    SmbiosTableType130Data->AmtSupported = 0;
    SmbiosTableType130Data->SrouEnabled = 0;
  }

  if (MeCapabilities.IntelAmtFw == 1) {
    SmbiosTableType130Data->AmtEnabled = MebxData.PlatformMngSel;
    SmbiosTableType130Data->SolEnabled = MebxData.AmtSol;
    SmbiosTableType130Data->NetworkEnabled = 1;  // If AMT enabled, LAN is always enabled
  }

  if (MeCapabilities.IntelKVM) {
    SmbiosTableType130Data->KvmEnabled = MebxData.KvmEnable;
  }

#if ME_TESTMENU_FLAG
  Status = GetSpecificConfigGuid (&gEfiMeRcVariableGuid, (VOID*) &MeSetup);

  if (!EFI_ERROR (Status)) {
    SmbiosTableType130Data->OemCapabilities1 &= ~0x1C;
    SmbiosTableType130Data->OemCapabilities1 |= (((MeSetup.BiosReflash & 0x1) << BIOS_REFLASH_BIT_OFFSET) |
                                                 ((MeSetup.BiosSetup & 0x1) << BIOS_SETUP_BIT_OFFSET) |
                                                 ((MeSetup.BiosPause & 0x1) << BIOS_PAUSE_BIT_OFFSET));
    SmbiosTableType130Data->OemCapabilities3 = ((MeSetup.SecureBootExposureToFw & 0x1) << BIOS_SECURE_BOOT_BIT_OFFSET) |
                                               (1 << SECURE_ERASE_BIT_OFFSET);
    if (MeCapabilities.IntelAmtFw == 1) {
      SmbiosTableType130Data->OemCapabilities4 |= (MeSetup.vProTbtDock & 0x1) << VPRO_TBT_DOCK_BIT_OFFSET;
    }
  }
#endif // ME_TESTMENU_FLAG
  SmbiosTableType130Data->OemCapabilities2 |= TERMINAL_EMULATION_VT100PLUS;

  return EFI_SUCCESS;
}

/**
  This function makes boot time changes to the content of the SMBIOS type 131.

  @retval EFI_SUCCESS           Record was updated successfully.
  @retval EFI_NOT_FOUND         HECI protocol or SMBIOS type 131 record cannot be found.
**/
EFI_STATUS
EFIAPI
UpdateSmbios131 (
  VOID
  )
{
  EFI_STATUS                        Status;
  EFI_SMBIOS_TABLE_HEADER           *Record;
  SMBIOS_TABLE_TYPE131              *SmbiosTableType131Data;
  CPUID_VERSION_INFO_ECX            Ecx;
  UINT32                            MeMode;
  MEBX_PROTOCOL                     *MebxProtocol;
  HECI_CONTROL                      *Heci;
  UINT8                             ProcessorVmxEnableOption;
  UINT8                             ProcessorLtsxEnableOption;
  UINT8                             VTdSupportOption;
  MSR_IA32_FEATURE_CONTROL_REGISTER Ia32FeatureControlMsr;

  DEBUG ((DEBUG_INFO, "%a() entry \n", __FUNCTION__));

  Status = GetSmbiosTable (MESMBIOS_TYPE_131, &Record);

  if (EFI_ERROR (Status)) {
    return EFI_NOT_FOUND;
  }

  SmbiosTableType131Data = (SMBIOS_TABLE_TYPE131*) Record;

  Status = gBS->LocateProtocol (&gHeciControlProtocolGuid, NULL, &Heci);

  if (!EFI_ERROR (Status)) {
    ZeroMem (&SmbiosTableType131Data->MeCapabilities, sizeof (ME_CAP));

    MeBiosGetMeMode (&MeMode);
    if (MeMode == ME_MODE_NORMAL) {
      Status = MbpGetMeFwInfo (&SmbiosTableType131Data->MeCapabilities);
      if (EFI_ERROR (Status)) {
        return Status;
      }
    }
  }

  //
  // CPU
  //
  Ia32FeatureControlMsr.Uint64 = AsmReadMsr64 (MSR_IA32_FEATURE_CONTROL);

  SmbiosTableType131Data->CpuCapabilities.VMXState     = Ia32FeatureControlMsr.Bits.EnableVmxOutsideSmx;
  SmbiosTableType131Data->CpuCapabilities.VTxEnabled   = SmbiosTableType131Data->CpuCapabilities.VMXState;
  SmbiosTableType131Data->CpuCapabilities.SMXState     = Ia32FeatureControlMsr.Bits.EnableVmxInsideSmx;
  SmbiosTableType131Data->CpuCapabilities.LtTxtEnabled = SmbiosTableType131Data->CpuCapabilities.SMXState;

  AsmCpuid (CPUID_VERSION_INFO, NULL, NULL, &Ecx.Uint32, NULL);
  SmbiosTableType131Data->CpuCapabilities.LtTxtCap     = Ecx.Bits.SMX;
  SmbiosTableType131Data->CpuCapabilities.VTxCap       = Ecx.Bits.VMX;

  //
  // MEBx Version
  //
  // Locate MEBx protocol to get MEBx version information.
  //
  Status = gBS->LocateProtocol (&gMebxProtocolGuid, NULL, (VOID**) &MebxProtocol);
  if (!EFI_ERROR (Status)) {
    SmbiosTableType131Data->MebxVersion.Major  = MebxProtocol->MebxVersion.Major;
    SmbiosTableType131Data->MebxVersion.Minor  = MebxProtocol->MebxVersion.Minor;
    SmbiosTableType131Data->MebxVersion.Hotfix = MebxProtocol->MebxVersion.Hotfix;
    SmbiosTableType131Data->MebxVersion.Build  = MebxProtocol->MebxVersion.Build;
  }

  //
  // PCH
  //
  SmbiosTableType131Data->PchCapabilities.FunctionNumber = LpcFuncNumber ();
  SmbiosTableType131Data->PchCapabilities.DeviceNumber   = LpcDevNumber ();
  SmbiosTableType131Data->PchCapabilities.BusNumber      = DEFAULT_PCI_BUS_NUMBER_PCH;
  SmbiosTableType131Data->PchCapabilities.DeviceID       = PciSegmentRead16 (LpcPciCfgBase () + PCI_DEVICE_ID_OFFSET);

  //
  // NetworkDevice
  //
  SmbiosTableType131Data->NetworkDevice.FunctionNumber   = GbeFuncNumber ();              // [2:0] PCI Device Function Number of Wired LAN
  SmbiosTableType131Data->NetworkDevice.DeviceNumber     = GbeDevNumber ();               // [7:3] PCI Device Device Number of Wired LAN
  SmbiosTableType131Data->NetworkDevice.BusNumber        = DEFAULT_PCI_BUS_NUMBER_PCH;    // [15:8] PCI Device Bus Number of Wired LAN
  SmbiosTableType131Data->NetworkDevice.DeviceID         = PciSegmentRead16 (GbePciCfgBase () + PCI_DEVICE_ID_OFFSET);

  //
  // BIOS
  //
  Status = GetOptionData (
             &gEfiSocketProcessorCoreVarGuid,
             OFFSET_OF (SOCKET_PROCESSORCORE_CONFIGURATION, ProcessorVmxEnable),
             &ProcessorVmxEnableOption,
             sizeof (ProcessorVmxEnableOption)
             );
  if (!EFI_ERROR (Status)) {
    SmbiosTableType131Data->BiosCapabilities.VTxSupport = !!ProcessorVmxEnableOption;
  }

  Status = GetOptionData (
             &gEfiSocketProcessorCoreVarGuid,
             OFFSET_OF (SOCKET_PROCESSORCORE_CONFIGURATION, ProcessorLtsxEnable),
             &ProcessorLtsxEnableOption,
             sizeof (ProcessorLtsxEnableOption)
             );
  if (!EFI_ERROR (Status)) {
    SmbiosTableType131Data->BiosCapabilities.TxtSupport = !!ProcessorLtsxEnableOption;
  }

  Status = GetOptionData (
             &gEfiSocketIioVariableGuid,
             OFFSET_OF (SOCKET_IIO_CONFIGURATION, VTdSupport),
             &VTdSupportOption,
             sizeof (VTdSupportOption)
             );

  if (!EFI_ERROR (Status)) {
    SmbiosTableType131Data->BiosCapabilities.VTdSupport = !!VTdSupportOption;
  }

  return EFI_SUCCESS;
}

/**
  This is the standard EFI driver entrypoint to install protocol to update ME SMBIOS tables from silicon package.

  @param[in] ImageHandle         Handle for the image of this driver
  @param[in] SystemTable         Pointer to the EFI System Table

  @retval EFI_SUCCESS            The protocol interface was installed.
  @retval EFI_UNSUPPORTED        The ME doesn't run the client FW.
  @retval EFI_OUT_OF_RESOURCES   Space for a new handle could not be allocated.
  @retval EFI_INVALID_PARAMETER  Handle or Protocol is NULL.
**/
EFI_STATUS
EFIAPI
MeSmbiosUpdateConfigEntryPoint (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS        Status;

  if (!MeTypeIsClient ()) {
    return EFI_UNSUPPORTED;
  }

  Status = gBS->InstallProtocolInterface (
                  &ImageHandle,
                  &gMeSmbiosTablesUpdateProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  &mMeSmbiosTablesUpdate
                  );
  ASSERT_EFI_ERROR (Status);
  return Status;
}
