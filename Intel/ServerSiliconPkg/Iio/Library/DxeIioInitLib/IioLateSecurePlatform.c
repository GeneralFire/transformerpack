/** @file
  Implement IIO initialization protocol to provide IIO initialization functions.
  This interfaces will be called by PciHostBridge driver in the PCI bus enumeration.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 - 2020 Intel Corporation. <BR>

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
#include <Base.h>
#include <Uefi.h>
#include <PiDxe.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/DxeServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiLib.h>
#include <Library/CpuAndRevisionLib.h>
#include <Library/HobLib.h>

#include <Protocol/MpService.h>
#include <Protocol/IioSystem.h>
#include <Register/ArchitecturalMsr.h>
#include <Cpu/CpuCoreRegs.h>
#include <RcRegs.h>
#include <IioSetupDefinitions.h>
#include <PcieRegs.h>
#include <Library/CpuConfigLib.h>
#include <Library/EmulationConfigurationLib.h>

#include <Library/IioDataHubLib.h>
#include <Library/IioAccessLib.h>

#include <LibraryPrivate/IioDebug.h>
#include <Library/KtiApi.h>
#include <Library/IioSecurePlatformLib.h>
// APTIOV_SERVER_OVERRIDE_RC_START : Changes done to resolve the Legacy Boot hang Issue when PAM LOCK BIT set
#include <Protocol/LegacyBiosExt.h>
// APTIOV_SERVER_OVERRIDE_RC_END : Changes done to resolve the Legacy Boot hang Issue when PAM LOCK BIT set

extern EFI_GUID gEfiEventExitBootServicesGuid;

//
// IQAT Table
//
typedef struct {
  //
  // IIO Stack
  //
  UINT8                    Stack;
  //
  // VRP Device
  //
  UINT16                   VrpDevice;
  //
  // VRP Function
  //
  UINT8                    VrpFunction;
  //
  // IQAT Device
  //
  UINT16                   IqatDevice;
  //
  // IQAT Function
  //
  UINT8                    IqatFunction;
  //
  // IQAT Devide ID
  //
  UINT16                   DeviceID;
} IQAT_TABLE;

IQAT_TABLE mIqatTableInfo[] = {
  { IIO_CB_RLINK, V_CDF_IQAT_VRP_DEVICE, V_CDF_IQAT_VRP_FUNCTION, V_CDF_IQAT_DEVICE, V_CDF_IQAT_FUNCTION, V_CDF_IQAT_DEVICE_ID },
  { IIO_CPM_NAC,  V_NAC_IQAT_VRP_DEVICE, V_NAC_IQAT_VRP_FUNCTION, V_NAC_IQAT_DEVICE, V_NAC_IQAT_FUNCTION, V_NAC_IQAT_DEVICE_ID }
};


/**

  Configure IQAT to use the Isolated Memory Region 2 (IMR2) as the Secure RAM

  @param Start        - IMR2 starting address
  @param Length       - IMR2 size in bytes

  @retval EFI_SUCCESS - IMR2 configured

**/
VOID
EFIAPI
IioIqatSecureRamConfig (
  VOID
  )
{
  EFI_IIO_SYSTEM_PROTOCOL                 *IioSystemProtocol;
  EFI_HOB_GUID_TYPE                       *GuidHob;
  IIO_GLOBALS                             *IioGlobalData;
  EFI_PHYSICAL_ADDRESS                    IqatBaseAddress;
  UINT32                                  IqatRamBaseAddrLo;
  UINT32                                  IqatRamBaseAddrLoOffset;
  UINT32                                  IqatRamBaseAddrHi;
  UINT32                                  IqatRamBaseAddrHiOffset;
  UINT16                                  IqatBaseAddrHigh;
  UINT32                                  IqatBaseAddrLow;
  UINT32                                  IqatPmisclbar;
  UINT8                                   IqatPmisclbarShift;
  UINT32                                  IqatPmiscubar;
  UINT64                                  IqatPmiscbar;
  BOOLEAN                                 IqatFound;
  UINT8                                   IioIndex;
  UINT8                                   IqatIndex;
  UINT32                                  VrpBuses;
  UINT16                                  IqatBus;
  UINT16                                  VendorID;
  UINT16                                  DeviceID;
  UINT16                                  Command;
  EFI_STATUS                              Status;

  if (IsCpuAndRevision (CPU_ICXD, REV_ALL) || IsCpuAndRevision (CPU_SNR, REV_ALL)) {
    DEBUG ((EFI_D_INFO, "IioIqatSecureRamConfig : Entry\n"));

    //
    // Need to get the IioGlobalData
    //
    IioSystemProtocol = NULL;
    Status = gBS->LocateProtocol (&gEfiIioSystemProtocolGuid, NULL, &IioSystemProtocol);
    if (EFI_ERROR(Status)) {
      DEBUG ((EFI_D_INFO, "\tError: Data not found\n"));
      DEBUG ((EFI_D_INFO, "IioIqatSecureRamConfig : Exit\n"));
      return; // Skip programming
    }
    IioGlobalData = IioSystemProtocol->IioGlobalData;

    //
    // Get IMR2 Base Address HOB if it was published
    //
    GuidHob = GetFirstGuidHob (&gImr2BaseAddressHobGuid);
    if (GuidHob == NULL) {
      DEBUG ((EFI_D_INFO, "\tExternal memory was not reserved for IAQT\n"));
      DEBUG ((EFI_D_INFO, "IioIqatSecureRamConfig : Exit\n"));
      return; // Skip programming
    }
    IqatBaseAddress = (EFI_PHYSICAL_ADDRESS)(*(EFI_PHYSICAL_ADDRESS *)GET_GUID_HOB_DATA (GuidHob));
    DEBUG((EFI_D_INFO, "\tIqatBaseAddress = 0x%016lx\n", (UINT64)IqatBaseAddress));

    //
    // Extracting High and Low address
    //
    IqatBaseAddrLow  = (UINT32) (B_NAC_TOP_CR_RAMBASEADDRLO_ADDR_MASK & IqatBaseAddress);
    IqatBaseAddrHigh = (UINT16) RShiftU64 (IqatBaseAddress, B_NAC_TOP_CR_RAMBASEADDRLO_ADDR_SHIFT);
    DEBUG ((EFI_D_INFO, "\tIqatBaseAddrHigh = 0x%08x\tIqatBaseAddrLow = 0x%08x\n",IqatBaseAddrHigh,IqatBaseAddrLow));

    //
    // Look for the IQAT Devices
    //
    IqatFound = FALSE;
    // Look for the CPM inside the NAC
    for (IqatIndex = 0; IqatIndex < sizeof(mIqatTableInfo)/sizeof(mIqatTableInfo[0]); IqatIndex++) {
      IqatBus = 0;
      for (IioIndex = 0; IioIndex < MaxIIO; IioIndex++) {
        if (!IioGlobalData->IioVar.IioVData.SocketPresent[IioIndex]){
          continue;
        }
        VrpBuses = (UINT32) IioPciExpressRead32 (
          IioIndex,IioGlobalData->IioVar.IioVData.SocketStackBus[IioIndex][mIqatTableInfo[IqatIndex].Stack],
          mIqatTableInfo[IqatIndex].VrpDevice,
          mIqatTableInfo[IqatIndex].VrpFunction,
          R_PCI_BRIDGE_BNUM);
        DEBUG ((EFI_D_INFO, "\tR_PCI_BRIDGE_BNUM[0x%08x] = 0x%08x\n",R_PCI_BRIDGE_BNUM,VrpBuses));
        IqatBus = (UINT16) ((VrpBuses & B_PCI_BRIDGE_BNUM_SCBN) >> V_PCI_BRIDGE_BNUM_SCBN_SHIFT);
        VendorID = (UINT16) IioPciExpressRead16 (IioIndex, IqatBus, mIqatTableInfo[IqatIndex].IqatDevice, mIqatTableInfo[IqatIndex].IqatFunction, PCI_VENDOR_ID_OFFSET);
        DeviceID = (UINT16) IioPciExpressRead16 (IioIndex, IqatBus, mIqatTableInfo[IqatIndex].IqatDevice, mIqatTableInfo[IqatIndex].IqatFunction, PCI_DEVICE_ID_OFFSET);
        if (VendorID == 0x8086 && DeviceID == mIqatTableInfo[IqatIndex].DeviceID) {
          DEBUG ((EFI_D_INFO, "\tIioIndex: 0x%02x, Bus: 0x%03x, Dev: 0x%02x, Fun: 0x%x, Vendor Id: 0x%04x, Device Id: 0x%04x\n", IioIndex, IqatBus, mIqatTableInfo[IqatIndex].IqatDevice, mIqatTableInfo[IqatIndex].IqatFunction, VendorID, DeviceID));
          IqatFound = TRUE;
          break;
        }
      } // End of for (IioIndex = 0; IioIndex < MaxIIO; IioIndex++)
      if (!IqatFound) {
        continue;
      }

      //
      // Enabling Memory Space for IQAT
      //
      Command = (UINT16) IioPciExpressRead16 (IioIndex, IqatBus, mIqatTableInfo[IqatIndex].IqatDevice, mIqatTableInfo[IqatIndex].IqatFunction, PCI_COMMAND_OFFSET);
      IioPciExpressWrite16 (IioIndex, IqatBus, mIqatTableInfo[IqatIndex].IqatDevice, mIqatTableInfo[IqatIndex].IqatFunction, PCI_COMMAND_OFFSET, (UINT16)(Command | EFI_PCI_COMMAND_MEMORY_SPACE));
      DEBUG ((EFI_D_INFO, "\tCOMMAND[0x%08x] = 0x%08x\n",PCI_COMMAND_OFFSET,(Command | EFI_PCI_COMMAND_MEMORY_SPACE)));

      //
      // Getting IQAT PMISCBAR
      //
      IqatPmisclbar = (UINT32) IioPciExpressRead32 (IioIndex, IqatBus, mIqatTableInfo[IqatIndex].IqatDevice, mIqatTableInfo[IqatIndex].IqatFunction, R_BASE_ADDRESS_OFFSET_2);
      DEBUG ((EFI_D_INFO, "\tR_BASE_ADDRESS_OFFSET_2[0x%08x] = 0x%08x\n",R_BASE_ADDRESS_OFFSET_2,IqatPmisclbar));
      IqatPmiscubar = (UINT32) IioPciExpressRead32 (IioIndex, IqatBus, mIqatTableInfo[IqatIndex].IqatDevice, mIqatTableInfo[IqatIndex].IqatFunction, R_BASE_ADDRESS_OFFSET_3);
      DEBUG ((EFI_D_INFO, "\tR_BASE_ADDRESS_OFFSET_3[0x%08x] = 0x%08x\n",R_BASE_ADDRESS_OFFSET_3,IqatPmiscubar));

      IqatPmisclbarShift = (UINT8)((DeviceID == V_NAC_IQAT_DEVICE_ID) ? B_NAC_TOP_CR_PMISCLBAR_ADDR_SHIFT : B_CDF_IQAT_TOP_CR_PMISCLBAR_ADDR_SHIFT);
      IqatPmiscbar =  (UINT64) LShiftU64 (IqatPmiscubar, IqatPmisclbarShift);
      IqatPmiscbar |= (UINT64) RShiftU64 (IqatPmisclbar, IqatPmisclbarShift);
      DEBUG((EFI_D_ERROR, "\tPMISCBAR[0x%08x] = 0x%08x\n",R_BASE_ADDRESS_OFFSET_2, (UINT64)IqatPmiscbar));
      IqatPmisclbar &= ~B_PCI_BAR_MEMORY_TYPE_MASK;

      //
      // Configure IQAT Secure Base Address
      //
      for (IioIndex = 0; IioIndex < MaxIIO; IioIndex++) {
        if (!IioGlobalData->IioVar.IioVData.SocketPresent[IioIndex]){
          continue;
        }
        IqatRamBaseAddrLoOffset = (UINT32)((DeviceID == V_NAC_IQAT_DEVICE_ID) ? R_NAC_TOP_CR_RAMBASEADDRLO : R_CDF_IQAT_CR_RAMBASEADDRLO);
        IqatRamBaseAddrLo = IioMmioRead32 ((IqatPmisclbar + IqatRamBaseAddrLoOffset));
        DEBUG ((EFI_D_INFO, "\tR_CR_RAMBASEADDRLO[0x%08x] = 0x%08x\n",IqatRamBaseAddrLoOffset,IqatRamBaseAddrLo));
        IqatRamBaseAddrLo = (UINT32) IqatBaseAddrLow;
        IioMmioWrite32 ((IqatPmisclbar + IqatRamBaseAddrLoOffset), IqatRamBaseAddrLo);
        IqatRamBaseAddrLo = IioMmioRead32 ((IqatPmisclbar + IqatRamBaseAddrLoOffset));
        DEBUG ((EFI_D_INFO, "\tR_CR_RAMBASEADDRLO[0x%08x] = 0x%08x\n",IqatRamBaseAddrLoOffset,IqatRamBaseAddrLo));

        IqatRamBaseAddrHiOffset = (UINT32)((DeviceID == V_NAC_IQAT_DEVICE_ID) ? R_NAC_TOP_CR_RAMBASEADDRHI : R_CDF_IQAT_CR_RAMBASEADDRHI);
        IqatRamBaseAddrHi = IioMmioRead32 ((IqatPmisclbar + IqatRamBaseAddrHiOffset));
        DEBUG ((EFI_D_INFO, "\tR_CR_RAMBASEADDRHI[0x%08x] = 0x%08x\n",IqatRamBaseAddrHiOffset,IqatRamBaseAddrHi));
        IqatRamBaseAddrHi = (UINT16) IqatBaseAddrHigh;
        IqatRamBaseAddrHi |= BIT17;
        IioMmioWrite32 ((IqatPmisclbar + IqatRamBaseAddrHiOffset), IqatRamBaseAddrHi);
        IqatRamBaseAddrHi = IioMmioRead32 ((IqatPmisclbar + IqatRamBaseAddrHiOffset));
        DEBUG ((EFI_D_INFO, "\tR_CR_RAMBASEADDRHI[0x%08x] = 0x%08x\n",IqatRamBaseAddrHiOffset,IqatRamBaseAddrHi));
      } // End of for (IioIndex = 0; IioIndex < MaxIIO; IioIndex++)
    }
    if (!IqatFound && IqatIndex == (sizeof(mIqatTableInfo)/sizeof(mIqatTableInfo[0]))) {
      DEBUG ((EFI_D_INFO, "\tError: IQAT not found\n"));
    }

    DEBUG ((EFI_D_INFO, "IioIqatSecureRamConfig : Exit\n"));
  }
}

/**

  Lock MSR for this thread.

  @param None

  @retval EFI_SUCCESS           - MSR Locked

**/
EFI_STATUS
EFIAPI
LockFeatureControl (
  IN  EFI_MP_SERVICES_PROTOCOL  *mMpService
  )
{
  UINTN                              ProcessorNumber;
  EFI_STATUS                         Status = EFI_SUCCESS;
  MSR_IA32_FEATURE_CONTROL_REGISTER  FeatureMsr;

  FeatureMsr.Uint32 = (UINT32) AsmReadMsr32 (MSR_IA32_FEATURE_CONTROL);
  //
  // Set MSR (0x3A) lock bit0 if it is not set.
  //
  if (FeatureMsr.Bits.Lock == 0) {
    FeatureMsr.Bits.Lock = 1;
    AsmWriteMsr32 (MSR_IA32_FEATURE_CONTROL, FeatureMsr.Uint32);
  } else {
    Status = mMpService->WhoAmI (mMpService, &ProcessorNumber);

    if (EFI_ERROR (Status)) {
      IIO_D_ERR ("Failed to find proc number - MP Services Protocol (%r)\n", Status);
      return Status;
    }

    //
    // Clear lock bit for S3 resume restore. Lock bit is restored at the end of pei event on S3
    //
    FeatureMsr.Bits.Lock = 0;
    WriteRegisterTable (
      ProcessorNumber,
      Msr,
      MSR_IA32_FEATURE_CONTROL,
      0,
      63,
      FeatureMsr.Uint32
    );
  }
  return EFI_SUCCESS;
}

VOID
SetMsrBiosDone (
  VOID
  )
{
  /*
   MSR_BIOS_DONE register is common MSR across all 10nm server programs. This register is not placed
   into CommonMsr.h file due to discrepancy of this register description across 10nm CRIF Files.
   Will use ICX_MSR_BIOS_DONE_REGISTER as temporary solution
  */
  ICX_MSR_BIOS_DONE_REGISTER          BiosDone;

  //
  // Set MSR_BIOS_DONE
  //
  BiosDone.Uint32 = AsmReadMsr32 (ICX_MSR_BIOS_DONE);
  BiosDone.Bits.EnableIaUntrusted = 1;
  AsmWriteMsr32 (ICX_MSR_BIOS_DONE, BiosDone.Uint32);
}

/**

Callback function to register Lock event;

@param Event
@param Context

@retval VOID

**/
VOID
EFIAPI
DummyNotificationFunction (
      EFI_EVENT                 Event,
      VOID                      *Context
  )
{

}

EFI_STATUS
EFIAPI
SetProcessorLocks (
  VOID
  )

{
  EFI_STATUS                    Status;
  EFI_IIO_SYSTEM_PROTOCOL       *IioSystemProtocol = NULL;
  IIO_GLOBALS                   *IioGlobalData = NULL;
  EFI_MP_SERVICES_PROTOCOL      *MpService = NULL;

  if (MsrTraceEnabled ()) {
    SetMsrTraceOutputMode (MSR_OUTPUT_ENABLE);
    IIO_D_LOG ("SetProcessorLocks Start\n");
  }

  Status = gBS->LocateProtocol (
    &gEfiMpServiceProtocolGuid,
    NULL,
    &MpService
    );

  if (EFI_ERROR (Status)) {
    IIO_D_ERR ("Failed to locate MP Services Protocol (%r)\n", Status);
    return Status;
  }

  //
  // Locate Iio System Protocol to be used to check Setup value for lock control
  //
  Status = gBS->LocateProtocol (&gEfiIioSystemProtocolGuid, NULL, &IioSystemProtocol);

  if (EFI_ERROR (Status)) {
    IIO_D_ERR ("Failed to locate gEfiIioSystemProtocolGuid (%r)\n", Status);
    return Status;
  }

  IioGlobalData = IioSystemProtocol->IioGlobalData;

  //
  // MSR (0X3A) bit0 IA32_FEATURE_CONTROL lock
  //
  if (IioGlobalData->SetupData.ProcessorMsrLockControl) {
    LockFeatureControl (MpService);
    Status = MpService->StartupAllAPs (
      MpService,
      (EFI_AP_PROCEDURE) LockFeatureControl,
      FALSE,
      NULL,
      0,
      (VOID *) MpService,
      NULL
      );
  }

  if (EFI_ERROR (Status) && Status != EFI_NOT_STARTED) {
    IIO_D_ERR ("Failed to lock Feature Control Msr (%r)\n", Status);
    return Status;
  }

  if (MsrTraceEnabled ()) {
    SetMsrTraceOutputMode (MSR_OUTPUT_DISABLE);
    IIO_D_LOG ("SetProcessorLocks End\n");
  }
  return EFI_SUCCESS;
}

/**

  Set MSR BIOS done to enter IA_UNTRUSTED mode

  @param None

  @retval EFI_SUCCESS           - IA_UNTRUSTED mode entered

**/
EFI_STATUS
EFIAPI
SetIAUntrustedMode (
  VOID
  )

{
  UINT32                        Bkp;
  EFI_STATUS                    Status;
  EFI_IIO_SYSTEM_PROTOCOL       *IioSystemProtocol = NULL;
  IIO_GLOBALS                   *IioGlobalData = NULL;
  EFI_MP_SERVICES_PROTOCOL      *MpService = NULL;
  EFI_EVENT                     BiosDoneEvent;

  BiosDoneEvent = NULL;
  //
  // If target platform is SKX then we are done. MSR 0x151 presents only in 10nm server programs
  //
  if (IsCpuAndRevision (CPU_SKX, REV_ALL) || IsCpuAndRevision (CPU_CLX, REV_ALL) || IsCpuAndRevision (CPU_CPX, REV_ALL)) {
    return EFI_SUCCESS;
  }

  if (MsrTraceEnabled ()) {
    SetMsrTraceOutputMode (MSR_OUTPUT_ENABLE);
    IIO_D_LOG ("SetIAUntrustedMode Start\n");
  }

  Status = gBS->LocateProtocol (
    &gEfiMpServiceProtocolGuid,
    NULL,
    &MpService
    );

  if (EFI_ERROR (Status)) {
    IIO_D_ERR ("Failed to locate MP Services Protocol (%r)\n", Status);
    return Status;
  }

  //
  // Locate Iio System Protocol to be used to check Setup value for lock control
  //
  Status = gBS->LocateProtocol (&gEfiIioSystemProtocolGuid, NULL, &IioSystemProtocol);

  if (EFI_ERROR (Status)) {
    IIO_D_ERR ("Failed to locate gEfiIioSystemProtocolGuid (%r)\n", Status);
    return Status;
  }

  IioGlobalData = IioSystemProtocol->IioGlobalData;

  //
  // Set MSR (0x151) BIT0 IA_UNTRUSTED
  //
  if (IioGlobalData->SetupData.DfxDisableBiosDone) {

    IIO_D_LOG ("MSR_BIOS_DONE[0x%X] - disabled for DFX\n", ICX_MSR_BIOS_DONE);

  } else {

    Bkp = AsmReadMsr32 (ICX_MSR_BIOS_DONE);
    SetMsrBiosDone ();
    Status = MpService->StartupAllAPs (MpService, (EFI_AP_PROCEDURE)SetMsrBiosDone, FALSE, NULL, 0, NULL, NULL);
    IIO_D_LOG ("MSR_BIOS_DONE[0x%X] 0x%08X -> 0x%08X\n", ICX_MSR_BIOS_DONE, Bkp, AsmReadMsr32 (ICX_MSR_BIOS_DONE));
    if (EFI_ERROR (Status) && Status != EFI_NOT_STARTED) {

      IIO_D_ERR ("Failed to move to IA_UNTRUSTED mode\n");
      return Status;
    }
    //
    // Signal Bios Done Event
    //
    Status = gBS->CreateEventEx (
      EVT_NOTIFY_SIGNAL,
      TPL_CALLBACK,
      DummyNotificationFunction,
      NULL,
      &gBiosDoneGuid,
      &BiosDoneEvent
    );

    ASSERT_EFI_ERROR (Status);
    if (!EFI_ERROR (Status)) {
      gBS->SignalEvent (BiosDoneEvent);
    }
  }

  if (MsrTraceEnabled ()) {
    SetMsrTraceOutputMode (MSR_OUTPUT_DISABLE);
    IIO_D_LOG ("SetIAUntrustedMode End\n");
  }

  return EFI_SUCCESS;
}

/**
  Iio EnfOfDxe callback function.

  @param[in] Event          - The instance of the event triggering this function
  @param[in] Context        - Pointer to Context, not used
**/
VOID
EFIAPI
IioOnEndOfDxe (
  IN  EFI_EVENT             Event,
  IN  VOID                  *Context
  )
{
  EFI_STATUS                Status;
  EFI_IIO_SYSTEM_PROTOCOL   *IioSystemProtocol;
  IIO_GLOBALS               *IioGlobalData;
  UINT8                     IioIndex;

  Status = gBS->LocateProtocol (&gEfiIioSystemProtocolGuid, NULL, &IioSystemProtocol);
  if (EFI_ERROR (Status)) {
    return;
  }

  IioGlobalData = IioSystemProtocol->IioGlobalData;

  for (IioIndex = 0; IioIndex < MaxIIO; IioIndex++) {
    if (!IioGlobalData->IioVar.IioVData.SocketPresent[IioIndex]) {
      continue;
    }
    if (!IioGlobalData->SetupData.SkipPortPersonalityLock) {
      //
      // Lock CAP registers for DMI/NTB/CXL/PCIe devices
      //
      IioLockPersonalityRegs (IioGlobalData, IioIndex);
    } else {
      IIO_D_LOG ("Personal registers lock skipped.\n");
    }
  }

  //
  // Set IMR2 as the IQAT Secure RAM once it has been enumerated.
  //
  IioIqatSecureRamConfig ();

  Status = SetProcessorLocks ();

  if (EFI_ERROR (Status)) {
    IIO_D_ERR ("SetProcessorLocks failed (%r)\n", Status);
  }
  return;
}

/**
  Set Bios Done for IA Untrusted Mode On DxeSmmReadyToLock NotifyEvent

  @param Event   - The instance of the event triggering this function
  @param Context - Pointer to Context, not used

  @retval None
**/
VOID
EFIAPI
SetIAUntrustedModeOnDxeSmmReadyToLock (
  IN   EFI_EVENT    Event,
  IN   VOID         *Context
  )
{
  EFI_STATUS  Status;
  VOID        *DxeSmmReadyToLock;

  //
  // Account for the initial call from EfiCreateProtocolNotifyEvent
  //
  Status = gBS->LocateProtocol (
                  &gEfiDxeSmmReadyToLockProtocolGuid,
                  NULL,
                  &DxeSmmReadyToLock
                  );
  if (EFI_ERROR (Status)) {
    return;
  }

  //
  // Close the event
  //
  gBS->CloseEvent (Event);

  Status = SetIAUntrustedMode ();

  if (EFI_ERROR (Status)) {
    IIO_D_ERR ("SetIAUntrustedMode failed (%r)\n", Status);
  }
  return;
}

/**

 Function that writes all the CSRs and MSRs defined in the SeCoE
 BIOS recommendations document that need to be done late in the
 boot process. These are all done as a ready to boot event.


// ******************   WARNING  **************************************
// *******  Settings within this function must not be changed *********
// *******  without reviewing the changes with SeCoE or the   *********
// *******  TXT BIOS owner.  Changes to this function will    *********
// *******  produce corresponding failures to the ACM Lock    *********
// *******  Configuration Checks and will prevent Trusted     *********
// *******  Boot.                                             *********
// ********************************************************************
// ********************************************************************


    @param IioGlobalData - Ptr to IIO_GLOBALS

    @retval None

**/
VOID
TxtLateSecurePlatform (
  IN  IIO_GLOBALS    *IioGlobalData
  )
{
  UINT8                       IioIndex;
#ifdef GNRSRF_HOST
  PAM0123_CFG_CHABC_SAD_STRUCT    Pam0123;
#else // GNRSRF_HOST
  PAM0123_CHABC_SAD_STRUCT    Pam0123;
#endif // GNRSRF_HOST

  IIO_D_DBG ("Init Late Secure the Platform (TXT)..\n");
  for (IioIndex = 0; IioIndex < MaxIIO; IioIndex++) {
    if (!IioGlobalData->IioVar.IioVData.SocketPresent[IioIndex]) {
      continue;
    }

    if (IsCpuAndRevision (CPU_SKX, REV_ALL) || IsCpuAndRevision (CPU_CLX, REV_ALL) || IsCpuAndRevision (CPU_CPX, REV_ALL)) {
#ifdef GNRSRF_HOST
      //
      // Temporarily using GNRSRF_HOST to workaround PAM registers name differences in GNR/SRF
      // until PAM registers access APIs are implemented in CHA IP library (HSD-ES 22010930974).
      //
      Pam0123.Data = IioReadCpuCsr32 (IioIndex, 0, PAM0123_CFG_CHABC_SAD_REG);
      Pam0123.Data |= BIT0; // lock
      IioNonS3WriteCpuCsr32 (IioIndex, 0, PAM0123_CFG_CHABC_SAD_REG, Pam0123.Data);
#else // GNRSRF_HOST
      Pam0123.Data = IioReadCpuCsr32 (IioIndex, 0, PAM0123_CHABC_SAD_REG);
      Pam0123.Data |= BIT0; // lock
      IioNonS3WriteCpuCsr32 (IioIndex, 0, PAM0123_CHABC_SAD_REG, Pam0123.Data);
#endif // GNRSRF_HOST
    }

  } // End of for (IioIndex = 0; IioIndex < MaxIIO; IioIndex++)
}

/**

  Routine to read and store the PAM registers to the S3 boot script

 @param EFI_EVENT                 Event

 @param VOID                      *Context

 @retval None
*/
VOID
EFIAPI
IioPamRegSave (
  IN      EFI_EVENT                 Event,
  IN      VOID                      *Context
  )
{
  EFI_STATUS                     Status;
  UINT8                          IioIndex;
  UINT8                          Stack;
#ifdef GNRSRF_HOST
  PAM0123_CFG_CHABC_SAD_STRUCT   Pam0123;
  PAM456_CFG_CHABC_SAD_STRUCT    Pam456;
#else // GNRSRF_HOST
  PAM0123_CHABC_SAD_STRUCT       Pam0123;
  PAM456_CHABC_SAD_STRUCT        Pam456;
#endif // GNRSRF_HOST
  LTDPR_IIO_VTD_STRUCT           Ltdpr;
  EFI_IIO_SYSTEM_PROTOCOL        *IioSystemProtocol = NULL;
  IIO_GLOBALS                    *IioGlobalData = NULL;

  IIO_D_DBG ("%a...\n", __FUNCTION__);
  //
  // Need to get the IioGlobalData
  //
  Status = gBS->LocateProtocol (&gEfiIioSystemProtocolGuid, NULL, &IioSystemProtocol);

  if (EFI_ERROR(Status)) {
    return;
  }

  IioGlobalData = IioSystemProtocol->IioGlobalData;

  for (IioIndex = 0; IioIndex < MaxIIO; IioIndex++) {
    if (!IioGlobalData->IioVar.IioVData.SocketPresent[IioIndex]) {
      continue;
    }

    //
    // Need to save PAM register settings to be restored on S3 resume.
    //
#ifdef GNRSRF_HOST
    Pam0123.Data = IioReadCpuCsr32 (IioIndex, 0, PAM0123_CFG_CHABC_SAD_REG);
#else // GNRSRF_HOST
    Pam0123.Data = IioReadCpuCsr32 (IioIndex, 0, PAM0123_CHABC_SAD_REG);
#endif // GNRSRF_HOST

    //
    // Save lock bit for restore, but do not set it yet since we are not at runtime yet
    //
    if ((IioGlobalData->SetupData.LockChipset) && (!IioGlobalData->IioVar.IioVData.EVMode)) {
      if (IsCpuAndRevision (CPU_SKX, REV_ALL) || IsCpuAndRevision (CPU_CLX, REV_ALL) || IsCpuAndRevision (CPU_CPX, REV_ALL)) {
        Pam0123.Data |= BIT0; // lock
      }
      for (Stack = 0; Stack < MAX_IIO_STACK; Stack++) {
        if (!IioIsStackPresent (IioIndex, Stack)) {
          continue;
        }
        Ltdpr.Data = IioReadCpuCsr32 (IioIndex, Stack, LTDPR_IIO_VTD_REG);
        Ltdpr.Data |= BIT0; // lock
        IioWriteCpuCsr32 (IioIndex, Stack, LTDPR_IIO_VTD_REG, Ltdpr.Data);
      }
    }
#ifdef GNRSRF_HOST
    //
    // Temporarily using GNRSRF_HOST to workaround PAM registers name differences in GNR/SRF
    // until PAM registers access APIs are implemented in CHA IP library (HSD-ES 22010930974).
    //
    IioWriteCpuCsr32 (IioIndex, 0, PAM0123_CFG_CHABC_SAD_REG, Pam0123.Data);

    Pam456.Data = IioReadCpuCsr32 (IioIndex, 0, PAM456_CFG_CHABC_SAD_REG);
    IioWriteCpuCsr32 (IioIndex, 0 ,PAM456_CFG_CHABC_SAD_REG, Pam456.Data);
#else // GNRSRF_HOST
    IioWriteCpuCsr32 (IioIndex, 0, PAM0123_CHABC_SAD_REG, Pam0123.Data);

    Pam456.Data = IioReadCpuCsr32 (IioIndex, 0, PAM456_CHABC_SAD_REG);
    IioWriteCpuCsr32 (IioIndex, 0 ,PAM456_CHABC_SAD_REG, Pam456.Data);
#endif // GNRSRF_HOST
  }
}

/**

    This function is to do TXT locks on ExitBootServices Event

    @param Event   - Pointer to EFI_EVENT
    @param Context - Pointer to recover function Context

    @retval None

**/
VOID
EFIAPI
OnExitBootServices (
  EFI_EVENT                 Event,
  VOID                      *Context
  )
{
  EFI_STATUS                    Status;
  EFI_IIO_SYSTEM_PROTOCOL       *IioSystemProtocol = NULL;
  IIO_GLOBALS                   *IioGlobalData = NULL;

  Status = gBS->LocateProtocol (&gEfiIioSystemProtocolGuid, NULL, &IioSystemProtocol);
  if (EFI_ERROR(Status)) {
    return;
  }
  IIO_D_DBG ("%a...\n", __FUNCTION__);

  IioGlobalData = IioSystemProtocol->IioGlobalData;

  //
  //
  // HSD 4168370 Add EVMode Support to the HSX EP BIOS
  //
  // Late TXT Secure Platform Locks
  //
  if ((IioGlobalData->SetupData.LockChipset) && (!IioGlobalData->IioVar.IioVData.EVMode)) {
    TxtLateSecurePlatform (IioGlobalData);
  }
}

/**

  Call back routine used to do all IIO post PCie port configuration, initialization

  @param IioGlobalData  - Pointer to IIO_GLOBALS

  @retval None
**/
EFI_STATUS
IioSecurePlatform (
  IIO_GLOBALS  *IioGlobalData
  )
{
  VOID            *Registration;
  EFI_STATUS      Status;
  EFI_EVENT       mExitBootServicesEvent;
  EFI_EVENT       mLegacyEvent;
  EFI_EVENT       ProcessorLocks;
  EFI_EVENT       SetIAUntrustedModeEvent;
  // APTIOV_SERVER_OVERRIDE_RC_START : Changes done to resolve the Legacy Boot hang Issue when PAM LOCK BIT set
  VOID            *EventRegistration;
  // APTIOV_SERVER_OVERRIDE_RC_END : Changes done to resolve the Legacy Boot hang Issue when PAM LOCK BIT set
  //
  // Save PAM registers for S3 restore just before Script lock
  //
  Status = gBS->CreateEventEx (
    EVT_NOTIFY_SIGNAL,
    TPL_CALLBACK,
    IioPamRegSave,
    NULL,
    &gEfiEndOfDxeEventGroupGuid,
    &Registration
    );
  ASSERT_EFI_ERROR (Status);

  //
  // Register EndOfDxe callback for IIO driver to lock FEATURE_CONTROL MSR
  //
  Status = gBS->CreateEventEx (
    EVT_NOTIFY_SIGNAL,
    TPL_CALLBACK,
    IioOnEndOfDxe,
    NULL,
    &gEfiEndOfDxeEventGroupGuid,
    &ProcessorLocks
    );
  ASSERT_EFI_ERROR (Status);

  //
  // Register DxeSmmReadyToLock NOTIFY to enter IA_UNTRUSTED mode
  //
  SetIAUntrustedModeEvent = EfiCreateProtocolNotifyEvent  (
                          &gEfiDxeSmmReadyToLockProtocolGuid,
                          TPL_NOTIFY,
                          SetIAUntrustedModeOnDxeSmmReadyToLock,
                          NULL,
                          &Registration
                          );
  ASSERT (SetIAUntrustedModeEvent != NULL);
// APTIOV_SERVER_OVERRIDE_RC_START : Changes done to resolve the Legacy Boot hang Issue when PAM LOCK BIT set
#if 0
  //
  // Register the event to inform PAM locking proc variable that it is at runtime.
  //
  Status = EfiCreateEventLegacyBootEx (
    TPL_NOTIFY,
    OnExitBootServices,
    NULL,
    &mLegacyEvent
    );
  ASSERT_EFI_ERROR (Status);
#endif
  
  //
  // Register for protocol notifactions
  //
  Status = gBS->CreateEvent (
              EVT_NOTIFY_SIGNAL,
              TPL_CALLBACK,
              OnExitBootServices,
              NULL,
              &mLegacyEvent
              );
  ASSERT_EFI_ERROR (Status);
  
  //
  // Register for protocol notifications on this event
  //
  Status = gBS->RegisterProtocolNotify (
              &gAmiLegacyBootProtocolGuid,
              mLegacyEvent,
              &EventRegistration
              );
  
  ASSERT_EFI_ERROR (Status);
// APTIOV_SERVER_OVERRIDE_RC_END : Changes done to resolve the Legacy Boot hang Issue when PAM LOCK BIT set

  //
  // Also need to register the callback for UEFI boot
  //
  Status = gBS->CreateEventEx (
    EVT_NOTIFY_SIGNAL,
    TPL_NOTIFY,
    OnExitBootServices,
    NULL,
    &gEfiEventExitBootServicesGuid,
    &mExitBootServicesEvent
    );

  ASSERT_EFI_ERROR (Status);

  return EFI_SUCCESS;
}

