/** @file
  This file contaisn the functionality for the ME Error Injection.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2009 - 2019 Intel Corporation. <BR>

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

#include <Library/RasDebugLib.h>
#include <Library/MpExtensionLib.h>
#include <Library/PlatMemTopologyLib.h>
#include <Library/MmPciExtLib.h>
#include <IndustryStandard/Pci.h>
#include <IndustryStandard/PciExpress31.h>
#include <Library/PcieRasLib.h>
#include <Library/PcieStdLib.h>
#include <Register/IioRegDef.h>
#include <Register/PcieRegDef.h>
#include <Library/IioTopologyLib.h>
#include "WheaErrorInj.h"

extern UINT64                                        EinjIioErrInjLockSwSmi;
/**
    Check if given SBDF supports error injection.

    @param  PcieSBDF -  BYTE 3 - PCIe Segment
                        BYTE 2 - Bus Number
                        BYTE 1 - Device Number[BITs 7:3], Function Number BITs[2:0]
                        BYTE 0 - RESERVED

    @retval TRUE - Injection capable
    @retval FALSE - Injection not capable

**/
BOOLEAN
EFIAPI
ValidateSBDFForInjection (
  IN   UINT32      PcieSBDF
  )
{
  UINT8                     Seg;
  UINT8                     Bus;
  UINT8                     Dev;
  UINT8                     Func;
  UINT16                    EinjCapOffset;
  UINT16                    EinjCtl;
  UINT32                    PciInjdevAddr;
  BOOLEAN                   ValidFlag = FALSE;

  Seg = (PcieSBDF >> 24) & (0xFF);
  Bus = (PcieSBDF >> 16) & (0xFF);
  Dev = (PcieSBDF >> 11) & (0x1F);
  Func = (PcieSBDF >> 8) & (0x7);

  if (!PcieIsDevicePresent (Seg, Bus, Dev, Func, NULL) || PcieIsDmiPort (Seg, Bus, Dev, Func, NULL) || PcieIsRlink (Seg, Bus, Dev, Func, NULL)) {
    return ValidFlag;
  }
  //
  // IIO root port have ERRINJ device ID and Vendor Specific ID 3
  //
  EinjCapOffset = PcieGetExtendedCapabilitiesOffset (Seg, Bus, Dev, Func, NULL, PCI_EXPRESS_EXTENDED_CAPABILITY_VENDOR_SPECIFIC_ID, PCIE_EXT_CAP_HEADER_EINJ_VSECID);
  if(EinjCapOffset == 0) {
    return ValidFlag;
  }

  PciInjdevAddr = (UINT32) MmPciAddressExt(Seg, Bus, Dev, Func, PCIE_EXT_CAP_EINJ_CTL_REG_OFFSET + EinjCapOffset, NULL);
  EinjCtl = *((volatile UINT16 *) ((UINTN) PciInjdevAddr));
  if ((EinjCtl & B_PCIE_EXT_CAP_EINJ_CTL_EINJDIS) == 0) {
    ValidFlag = TRUE;
  } else {
    ValidFlag = FALSE;
  }

  if (!ValidFlag) {
    RAS_DEBUG ((LEVEL_REG, "Error: Injection is not available on this device (S%xB%x:D%x:F%x)\n", Seg, Bus, Dev, Func));
  }

  return ValidFlag;
}

/**

    This function gets the first available port that is connected with slot . This function is for SV usage.
    @param None   -

    @retval PcieSBDF -  BYTE 3 - PCIe Segment
                        BYTE 2 - Bus Number
                        BYTE 1 - Device Number[BITs 7:3], Function Number BITs[2:0]
                        BYTE 0 - RESERVED
    @retval 0x0 - Slot is not populated

**/

UINT32
EFIAPI
GetRootPortForInjection (
 VOID
 )
{
  UINT8                     Seg;
  UINT8                     Bus;
  UINT8                     Dev;
  UINT8                     Func;
  UINT8                     Port;
  UINT32                    PcieSBDF = 0;
  BOOLEAN                   Found = FALSE;
  UINT8                     Socket;

  for (Socket = 0; Socket < MAX_SOCKET; Socket++) {

    //Check if socket is populated.
    if (IsSocketPresent (Socket) == FALSE) {
      continue;
    }

    //skip DMI port.
    for (Port = 0; Port < NUMBER_PORTS_PER_SOCKET; Port++) {
      if(GetRootPortInfo(Socket, Port, &Seg, &Bus, &Dev, &Func)){
        if(PcieIsDmiPort (Seg, Bus, Dev, Func, NULL) || PcieIsRlink (Seg, Bus, Dev, Func, NULL)) {
          continue;
        }
        PcieSBDF = (Seg << 24) | ( Bus << 16) | (Dev << 11) | (Func << 0x08);
        // Check if the root port is capable of error injection
        Found = ValidateSBDFForInjection (PcieSBDF);
        if (Found ) {
          return PcieSBDF;
        }
      }
    }
  }

  return 0;
}

/**
  IIO Error Injection Lock Handler

  This function locks error injection in the IIO.

  @param[in] DispatchHandle       Dispatch handle
  @param[in] Context              Context information
  @param[in] CommunicationBuffer  Buffer used for communication between caller/callback function
  @param[in] SourceSize           Size of communication buffer

  @retval Status.

**/
EFI_STATUS
EFIAPI
IioErrorInjectionLockHandler (
  IN      EFI_HANDLE                          DispatchHandle,
  IN      CONST EFI_SMM_SW_REGISTER_CONTEXT   *DispatchContext, OPTIONAL
  IN OUT  VOID                                *CommBuffer,      OPTIONAL
  IN OUT  UINTN                               *CommBufferSize   OPTIONAL
  )
{
  MultiProcessorExecute (OneApPerPackage, ALL_PACKAGE, ALL_CORE, TRUE, LockInjLogic, NULL);
  return EFI_SUCCESS;
}

/**

    Inject Pcie related Errors.

    PcieSBDF must be a device capable of support error injection

    @param  PcieSBDF -  BYTE 3 - PCIe Segment
                                 BYTE 2 - Bus Number
                                 BYTE 1 - Device Number[BITs 7:3], Function Number BITs[2:0]
                                 BYTE 0 - RESERVED

    @param ErrorToInject   - BIT 8 set - Pci-ex fatal error (Receiver error with fatal error severity)
                                       BIT 7 set - Pci-ex Un correctable error (Receiver error with uncorrected non fatal error severity)
                                       BIT 6 - Pci-ex correctable error ( Completion timeout error)

    @retval EFI_SUCCESS - Injection Success

**/
EFI_STATUS
EFIAPI
InjectPciExError (
 IN   UINT32      PcieSBDF,
 IN   UINT32      ErrorToInject,
 IN   WHEA_EINJ_TRIGGER_ACTION_TABLE  *EinjAction,
 IN   WHEA_EINJ_PARAM_BUFFER          *EinjParam
 )
{
  UINT8                     Seg;
  UINT8                     Bus;
  UINT8                     Dev;
  UINT8                     Func;
  UINT16                    EinjCapOffset;
  UINT16                    EinjCtl;
  UINT32                    PciInjdevAddr = 00;
  UINT32                    UncErrSev;

  Seg  = (PcieSBDF >> 24) & (0xFF);
  Bus  = (PcieSBDF >> 16) & (0xFF);
  Dev  = (PcieSBDF >> 11) & (0x1F);
  Func = (PcieSBDF >> 8) & (0x7);

  //
  // IIO root port have ERRINJ device ID and Vendor Specific ID 3
  //
  EinjCapOffset = PcieGetExtendedCapabilitiesOffset (Seg, Bus, Dev, Func, NULL, PCI_EXPRESS_EXTENDED_CAPABILITY_VENDOR_SPECIFIC_ID, PCIE_EXT_CAP_HEADER_EINJ_VSECID);
  if(EinjCapOffset == 0) {
    return EFI_UNSUPPORTED;
  }

  MultiProcessorExecute (OneApPerPackage, ALL_PACKAGE, ALL_CORE, TRUE, UnLockInjLogic, NULL);

  PciInjdevAddr = (UINT32) MmPciAddressExt(Seg, Bus, Dev, Func, PCIE_EXT_CAP_EINJ_CTL_REG_OFFSET + EinjCapOffset, NULL);
  EinjCtl = *((volatile UINT16 *) ((UINTN) PciInjdevAddr));
  EinjCtl &= ~(B_PCIE_EXT_CAP_EINJ_CTL_EINJDIS | B_PCIE_EXT_CAP_EINJ_CTL_INJRCVERR | B_PCIE_EXT_CAP_EINJ_CTL_INJCTOERR);
  *((volatile UINT16 *) ((UINTN) PciInjdevAddr)) = EinjCtl;
  EinjCtl = *((volatile UINT16 *) ((UINTN) PciInjdevAddr));

  if (!PcieAerGetErrSev (Seg, Bus, Dev, Func, NULL, &UncErrSev)) {
    return EFI_UNSUPPORTED;
  }

  if (ErrorToInject == EFI_ACPI_6_2_EINJ_ERROR_PCI_EXPRESS_UNCORRECTABLE_FATAL ) {
    UncErrSev |= B_PCIE_AER_UESEV_CTO_SEV;
    EinjCtl |= B_PCIE_EXT_CAP_EINJ_CTL_INJCTOERR;
  } else if (ErrorToInject == EFI_ACPI_6_2_EINJ_ERROR_PCI_EXPRESS_UNCORRECTABLE_NONFATAL ) {
    UncErrSev &= ~B_PCIE_AER_UESEV_CTO_SEV;
    EinjCtl |= B_PCIE_EXT_CAP_EINJ_CTL_INJCTOERR;
  } else if (ErrorToInject == EFI_ACPI_6_2_EINJ_ERROR_PCI_EXPRESS_CORRECTABLE ){
    EinjCtl |= B_PCIE_EXT_CAP_EINJ_CTL_INJRCVERR;
  } else {
    EinjCtl &= ~(B_PCIE_EXT_CAP_EINJ_CTL_EINJDIS | B_PCIE_EXT_CAP_EINJ_CTL_INJRCVERR | B_PCIE_EXT_CAP_EINJ_CTL_INJCTOERR);
  }

  PcieAerProgramErrSev (Seg, Bus, Dev, Func, NULL, UncErrSev);

  if(EinjParam->PcieErrInjActionTable == TRUE) {
    EinjAction->Trigger0.InjectionAction                  = WHEA_EINJ_TRIGGER_ERROR_ACTION;
    EinjAction->Trigger0.Instruction                      = EFI_ACPI_6_2_EINJ_WRITE_REGISTER_VALUE;
    EinjAction->Trigger0.Flags                            = 0;
    EinjAction->Trigger0.Reserved0                        = 00;
    EinjAction->Trigger0.RegisterRegion.AddressSpaceId    = 00;
    EinjAction->Trigger0.RegisterRegion.RegisterBitWidth  = 0x20;
    EinjAction->Trigger0.RegisterRegion.RegisterBitOffset = 0x00;
    EinjAction->Trigger0.RegisterRegion.AccessSize        = 0x03;
    EinjAction->Trigger0.RegisterRegion.Address           = PciInjdevAddr;
    EinjAction->Trigger0.Value                            = EinjCtl;
    EinjAction->Trigger0.Mask                             = 0xffffffff;

    // Trigger action 1 is added to lock iio_err_inj lck after pcie error injection
    EinjAction->Trigger1.InjectionAction                  = WHEA_EINJ_TRIGGER_ERROR_ACTION;
    EinjAction->Trigger1.Instruction                      = EFI_ACPI_6_2_EINJ_WRITE_REGISTER_VALUE;
    EinjAction->Trigger1.Flags                            = 0;
    EinjAction->Trigger1.Reserved0                        = 00;
    EinjAction->Trigger1.RegisterRegion.AddressSpaceId    = 01;
    EinjAction->Trigger1.RegisterRegion.RegisterBitWidth  = 0x08;
    EinjAction->Trigger1.RegisterRegion.RegisterBitOffset = 0x00;
    EinjAction->Trigger1.RegisterRegion.AccessSize        = 0x01;
    EinjAction->Trigger1.RegisterRegion.Address           = 0xB2;
    EinjAction->Trigger1.Value                            = EinjIioErrInjLockSwSmi;
    EinjAction->Trigger1.Mask                             = 0xffffffff;
  } else {
    *((volatile UINT16 *) ((UINTN) PciInjdevAddr)) = EinjCtl;
    MultiProcessorExecute (OneApPerPackage, ALL_PACKAGE, ALL_CORE, TRUE, LockInjLogic, NULL);
  }

  return EFI_SUCCESS;
}


