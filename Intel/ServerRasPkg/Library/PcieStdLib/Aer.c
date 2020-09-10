/** @file
  Implementation of Pcie Aer Lib.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 Intel Corporation. <BR>

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

#include <Uefi/UefiBaseType.h>
#include <IndustryStandard/PciExpress31.h>
#include <Library/PcieStdLib.h>
#include <Library/MmPciExtLib.h>
#include <Library/RasDebugLib.h>
#include <Library/BaseLib.h>
#include <IndustryStandard/Pci.h>
#include "Register/PcieRegDef.h"
#include <Guid/Cper.h>
#include <Library/IoLib.h>
#include <PCIeErrorTypes.h>

/**
  This function is to check if AER contains root error.

  @param[in] Bus            PCIe device's bus number.
  @param[in] Device         PCIe device's device number.
  @param[in] Function       PCIe device's function number.
  @param[in] MmInfo         Memory Map information of target device.

  @retval    Value          TRUE    root error field is valid
                            FALSE   root error filed is invalid.
--*/
BOOLEAN
EFIAPI
DoesAerContainRootError (
  IN UINT8            Socket,
  IN UINT8            Bus,
  IN UINT8            Device,
  IN UINT8            Function,
  IN PCI_EXT_MM_INFO  *MmInfo
  )
{
  UINT32  PortType;
  PortType = PcieGetPortType (Socket, Bus, Device, Function, MmInfo);

  if ((PortType  == PCIE_DEVICE_PORT_TYPE_ROOT_PORT) ||
    (PortType == PCIE_DEVICE_PORT_TYPE_ROOT_COMPLEX_EVENT_COLLECTOR)) {
    return TRUE;
  } else {
    return FALSE;
  }
}

/**
  Check if PCIE device support AER or not.

  @param Bus       - Bus
  @param Device    - Device
  @param Function  - Function
  @param MmInfo    - Memory Map information of target device.

  @retval BOOLEAN  - TRUE: AER is supported; FALSE:AER is not supported.

**/
BOOLEAN
EFIAPI
IsPcieAerSupported (
  IN UINT8            Socket,
  IN UINT8            Bus,
  IN UINT8            Device,
  IN UINT8            Function,
  IN PCI_EXT_MM_INFO  *MmInfo
  )
{
  //RAS_DEBUG((LEVEL_REG,"PcieStdLib IsPcieAerSupported."));

  if (PcieGetExtendedCapabilitiesOffset (Socket, Bus, Device, Function, MmInfo,
                   PCI_EXPRESS_EXTENDED_CAPABILITY_ADVANCED_ERROR_REPORTING_ID, 0xff) != 0) {
    return TRUE;
  }

  return FALSE;
}

/**
  Program PCIE device's AER Correctable error mask.

  @param Bus         - Bus
  @param Device      - Device
  @param Function    - Function
  @param MmInfo      - Memory Map information of target device
  @param CorrErrMsk  - Correctable error mask

**/
VOID
EFIAPI
PcieAerProgramCorrErrMsk (
  IN UINT8            Socket,
  IN UINT8            Bus,
  IN UINT8            Device,
  IN UINT8            Function,
  IN PCI_EXT_MM_INFO  *MmInfo,
  IN UINT32           CorrErrMsk
  )
{
  UINT16 AerCapOffset;

  AerCapOffset = PcieGetExtendedCapabilitiesOffset (Socket, Bus, Device, Function,
                   MmInfo, PCI_EXPRESS_EXTENDED_CAPABILITY_ADVANCED_ERROR_REPORTING_ID, 0xff);
  if (AerCapOffset != 0) {
    RAS_DEBUG ((LEVEL_REG, "      [AER] Input CorrErrMsk 0x%x \n", CorrErrMsk));
    MmioWrite32 (MmPciAddressExt (Socket, Bus, Device, Function, AerCapOffset + PCIE_AER_OFFSET_REG_CEM, MmInfo), CorrErrMsk);
  } else {
    RAS_DEBUG ((LEVEL_REG, "      PcieAer is not supported\n"));
  }
}

/**
  Program PCIE device's AER Uncorrectable error mask.

  @param Bus           - Bus
  @param Device        - Device
  @param Function      - Function
  @param MmInfo        - Memory Map information of target device
  @param UnCorrErrMsk  - Uncorrectable error mask
**/
VOID
EFIAPI
PcieAerProgramUncErrMsk (
  IN UINT8            Socket,
  IN UINT8            Bus,
  IN UINT8            Device,
  IN UINT8            Function,
  IN PCI_EXT_MM_INFO  *MmInfo,
  IN UINT32           UncErrMsk
  )
{
  UINT16 AerCapOffset;

  AerCapOffset = PcieGetExtendedCapabilitiesOffset (Socket, Bus, Device, Function,
                   MmInfo, PCI_EXPRESS_EXTENDED_CAPABILITY_ADVANCED_ERROR_REPORTING_ID, 0xff);
  if (AerCapOffset != 0) {
    RAS_DEBUG ((LEVEL_REG, "      [AER] Input UncErrMsk 0x%x \n", UncErrMsk));
    MmioWrite32 (MmPciAddressExt (Socket, Bus, Device, Function, AerCapOffset + PCIE_AER_OFFSET_REG_UEM, MmInfo), UncErrMsk);
  } else {
    RAS_DEBUG ((LEVEL_REG, "      PcieAer is not supported\n"));
  }
}

/**
  Get PCIE device's AER error severity.

  @param Socket    - Socket
  @param Bus       - Bus
  @param Device    - Device
  @param Function  - Function
  @param MmInfo    - Memory Map information of target device
  @param ErrSev    - Error severity

  return    FALSE - No severity return,  TRUE - Severity return
**/
BOOLEAN
EFIAPI
PcieAerGetErrSev (
  IN UINT8            Socket,
  IN UINT8            Bus,
  IN UINT8            Device,
  IN UINT8            Function,
  IN PCI_EXT_MM_INFO  *MmInfo,
  IN UINT32           *ErrSev
  )
{
  UINT16 AerCapOffset;

  AerCapOffset = PcieGetExtendedCapabilitiesOffset (Socket, Bus, Device, Function,
                   MmInfo, PCI_EXPRESS_EXTENDED_CAPABILITY_ADVANCED_ERROR_REPORTING_ID, 0xff);

  if (AerCapOffset != 0) {
    *ErrSev = MmioRead32 (MmPciAddressExt (Socket, Bus, Device, Function, AerCapOffset + PCIE_AER_OFFSET_REG_UESEV, MmInfo));
    RAS_DEBUG ((LEVEL_REG, "      [AER] Get ErrSev 0x%x \n", ErrSev));
    return TRUE;
  } else {
    RAS_DEBUG ((LEVEL_REG, "      PcieAer is not supported\n"));
    return FALSE;
  }
}

/**
  Program PCIE device's AER error severity.

  @param Socket    - Socket
  @param Bus       - Bus
  @param Device    - Device
  @param Function  - Function
  @param MmInfo    - Memory Map information of target device
  @param ErrSev    - Error severity
**/
VOID
EFIAPI
PcieAerProgramErrSev (
  IN UINT8            Socket,
  IN UINT8            Bus,
  IN UINT8            Device,
  IN UINT8            Function,
  IN PCI_EXT_MM_INFO  *MmInfo,
  IN UINT32           ErrSev
  )
{
  UINT16 AerCapOffset;

  AerCapOffset = PcieGetExtendedCapabilitiesOffset (Socket, Bus, Device, Function,
                   MmInfo, PCI_EXPRESS_EXTENDED_CAPABILITY_ADVANCED_ERROR_REPORTING_ID, 0xff);

  if (AerCapOffset != 0) {
    RAS_DEBUG ((LEVEL_REG, "      [AER] Input ErrSev 0x%x \n", ErrSev));
    MmioWrite32 (MmPciAddressExt (Socket, Bus, Device, Function, AerCapOffset + PCIE_AER_OFFSET_REG_UESEV, MmInfo), ErrSev);

  } else {
    RAS_DEBUG ((LEVEL_REG, "      PcieAer is not supported\n"));
  }
}

/**
  Program PCIE device's Advanced Error Capabilities and Control Register (Offset 18h).

  @param Socket    - Socket
  @param Bus       - Bus
  @param Device    - Device
  @param Function  - Function
  @param MmInfo    - Memory Map information of target device
  @param AeccConf  - AeccConf   Configuration data of AECC
**/
VOID
EFIAPI
PcieAerProgramAecc (
  IN UINT8            Socket,
  IN UINT8            Bus,
  IN UINT8            Device,
  IN UINT8            Function,
  IN PCI_EXT_MM_INFO  *MmInfo,
  IN UINT32           AeccConf
  )
{
  UINT16          AerCapOffset;
  UINT32          AeccData;

  AerCapOffset = PcieGetExtendedCapabilitiesOffset (Socket, Bus, Device, Function,
                   MmInfo, PCI_EXPRESS_EXTENDED_CAPABILITY_ADVANCED_ERROR_REPORTING_ID, 0xff);

  if (AerCapOffset != 0) {
    RAS_DEBUG ((LEVEL_REG, "      [AER] Input AeccConf 0x%x \n", AeccConf));
    if (AeccConf & (B_PCIE_AER_AECC_ECRC_GEN_EN | B_PCIE_AER_AECC_ECRC_CHK_EN)) {
      AeccData = MmioRead32 (MmPciAddressExt (Socket, Bus, Device, Function, AerCapOffset + PCIE_AER_OFFSET_REG_AECC, MmInfo));
      if (AeccConf & B_PCIE_AER_AECC_ECRC_GEN_EN) {
        if (AeccData & B_PCIE_AER_AECC_ECRC_GEN_CAP) {
          AeccData |= B_PCIE_AER_AECC_ECRC_GEN_EN;
        }
      }
      if (AeccConf & B_PCIE_AER_AECC_ECRC_CHK_EN) {
        if (AeccData & B_PCIE_AER_AECC_ECRC_CHK_CAP) {
          AeccData |= B_PCIE_AER_AECC_ECRC_CHK_EN;
        }
      }
      MmioWrite32 (MmPciAddressExt (Socket, Bus, Device, Function, AerCapOffset + PCIE_AER_OFFSET_REG_AECC, MmInfo), AeccData);
    }
  } else {
    RAS_DEBUG ((LEVEL_REG, "      PcieAer is not supported\n"));
  }
}

/**
  Clear PCIE device's AER correctable error status.
  @param Socket    - Socket
  @param Bus       - Bus
  @param Device    - Device
  @param Function  - Function
  @param MmInfo    - Memory Map information of target device
**/
VOID
EFIAPI
PcieAerClearCorrErrSts (
  IN UINT8            Socket,
  IN UINT8            Bus,
  IN UINT8            Device,
  IN UINT8            Function,
  IN PCI_EXT_MM_INFO  *MmInfo
  )
{
  UINT32  CorrErrSts;
  UINT32  UncErrSts;
  UINT16  AerCapOffset;

  AerCapOffset = PcieGetExtendedCapabilitiesOffset (Socket, Bus, Device, Function,
                   MmInfo, PCI_EXPRESS_EXTENDED_CAPABILITY_ADVANCED_ERROR_REPORTING_ID, 0xff);

  if (AerCapOffset != 0) {

    CorrErrSts = MmioRead32 (MmPciAddressExt (Socket, Bus, Device, Function, AerCapOffset + PCIE_AER_OFFSET_REG_CES, MmInfo));
    if (CorrErrSts != 0) {
      RAS_DEBUG ((LEVEL_REG, "      Clearing Aer correct status on Bus:0x%x, Dev:0x%x func:0x%x  CorErrSts:0x%x \n", Bus, Device, Function, CorrErrSts));
      MmioWrite32 (MmPciAddressExt (Socket, Bus, Device, Function, AerCapOffset + PCIE_AER_OFFSET_REG_CES, MmInfo), CorrErrSts);
      CorrErrSts = MmioRead32 (MmPciAddressExt (Socket, Bus, Device, Function, AerCapOffset + PCIE_AER_OFFSET_REG_CES, MmInfo));
      RAS_DEBUG ((LEVEL_REG, "      After Clearing Aer correct status on Bus:0x%x, Dev:0x%x func:0x%x  CorErrSts:0x%x \n", Bus, Device, Function, CorrErrSts));

      //
      // Need to take care for Advisory Non-fatal Error, this is a correctable error which also might have
      // some uncorrectable error status set.
      //
      if (CorrErrSts & B_PCIE_AER_CES_ADV_NON_FAT_ERR) {
        //
        // Following non-fatal error may cause Advisory Non-Fatal Error, there errors will
        // be cleared mandatorily. If only correctable error happened, this is reasonable.
        // if both correctable and uncorrectable error happened, uncorrectable will be handled firstly
        // it is also ok in this case.
        //
        UncErrSts = B_PCIE_AER_UES_POI_TLP_ERR |
                    B_PCIE_AER_UES_CTO_ERR |
                    B_PCIE_AER_UES_CPL_ABT_ERR |
                    B_PCIE_AER_UES_UNE_CPL_ERR |
                    B_PCIE_AER_UES_UNS_REQ_ERR;
        MmioWrite32 (MmPciAddressExt (Socket, Bus, Device, Function, AerCapOffset + PCIE_AER_OFFSET_REG_UES, MmInfo), UncErrSts);
      }
    }
  } else {
    RAS_DEBUG ((LEVEL_REG, "      PcieAer is not supported\n"));
  }

}

/**
  Clear PCIE device's AER uncorrectable error status.
  @param Socket    - Socket
  @param Bus       - Bus
  @param Device    - Device
  @param Function  - Function
  @param MmInfo    - Memory Map information of target device
**/
VOID
EFIAPI
PcieAerClearUncErrSts (
  IN UINT8            Socket,
  IN UINT8            Bus,
  IN UINT8            Device,
  IN UINT8            Function,
  IN PCI_EXT_MM_INFO  *MmInfo
  )
{
  UINT32  UncErrSts;
  UINT16  AerCapOffset;

  AerCapOffset = PcieGetExtendedCapabilitiesOffset (Socket, Bus, Device, Function,
                   MmInfo, PCI_EXPRESS_EXTENDED_CAPABILITY_ADVANCED_ERROR_REPORTING_ID, 0xff);

  if (AerCapOffset != 0) {

    UncErrSts = MmioRead32 (MmPciAddressExt (Socket, Bus, Device, Function, AerCapOffset + PCIE_AER_OFFSET_REG_UES, MmInfo));

    if (UncErrSts != 0) {
      RAS_DEBUG ((LEVEL_REG, "      Clearing Aer uncorrect status on Bus:0x%x, Dev:0x%x func:0x%x UncorrectSts:0x%x \n", Bus, Device, Function, UncErrSts));
      MmioWrite32 (MmPciAddressExt (Socket, Bus, Device, Function, AerCapOffset + PCIE_AER_OFFSET_REG_UES, MmInfo), UncErrSts);
    }
  } else {
    RAS_DEBUG ((LEVEL_REG, "      PcieAer is not supported\n"));
  }
}

/**
  Clear PCIE device's Root Error Status.
  @param Socket    - Socket
  @param Bus       - Bus
  @param Device    - Device
  @param Function  - Function
  @param MmInfo    - Memory Map information of target device
  @param Severity  - Severity
**/
VOID
EFIAPI
PcieAerClearRootErrSts (
  IN UINT8            Socket,
  IN UINT8            Bus,
  IN UINT8            Device,
  IN UINT8            Function,
  IN PCI_EXT_MM_INFO  *MmInfo,
  IN UINT32           Severity
  )
{
  if (DoesAerContainRootError (Socket, Bus, Device, Function, MmInfo)) {
    PcieAerClearRootErrStsAtom (Socket, Bus, Device, Function, MmInfo, Severity);
  }
}

/**
  Clear PCIE device's Root Error Status without checking port type.
  @param Socket    - Socket
  @param Bus       - Bus
  @param Device    - Device
  @param Function  - Function
  @param MmInfo    - Memory Map information of target device
  @param Severity  - Severity
**/
VOID
EFIAPI
PcieAerClearRootErrStsAtom (
  IN UINT8            Socket,
  IN UINT8            Bus,
  IN UINT8            Device,
  IN UINT8            Function,
  IN PCI_EXT_MM_INFO  *MmInfo,
  IN UINT32           Severity
  )
{
  UINT16  AerCapOffset;
  UINT32  AerRootErrSts;

  AerCapOffset = PcieGetExtendedCapabilitiesOffset (Socket, Bus, Device, Function,
                   MmInfo, PCI_EXPRESS_EXTENDED_CAPABILITY_ADVANCED_ERROR_REPORTING_ID, 0xff);

  if (AerCapOffset != 0) {
      AerRootErrSts = 0;
      if (Severity & FATAL_ERROR_TYPE) {
        AerRootErrSts |= (BIT2|BIT3|BIT4|BIT6);
      }
      if (Severity & NONFATAL_ERROR_TYPE) {
        AerRootErrSts |= (BIT2|BIT3|BIT5);
      }
      if (Severity & COR_ERROR_TYPE) {
        AerRootErrSts |= (BIT0|BIT1);
      }
      MmioWrite32 (MmPciAddressExt (Socket, Bus, Device, Function, AerCapOffset + PCIE_AER_OFFSET_REG_RES, MmInfo), AerRootErrSts);
  } else {
    RAS_DEBUG ((LEVEL_REG, "      PcieAer is not supported\n"));
  }
}

/**
  Get PCIE device's AER cap structer.

  @param Socket          - Socket
  @param Bus             - Bus
  @param Device          - Device
  @param Function        - Function
  @param MmInfo          - Memory Map information of target device
  @param AerErrDataCap   - Point to get Pcie error data capability.
**/
EFI_STATUS
EFIAPI
PcieAerGetErrCapData (
  IN  UINT8            Socket,
  IN  UINT8            Bus,
  IN  UINT8            Device,
  IN  UINT8            Function,
  IN  PCI_EXT_MM_INFO  *MmInfo,
  OUT VOID             *AerErrDataCap
  )
{
  UINT8    PciPrimaryStatus;
  UINT16   Offset;
  UINT8    *AerPointer;
  UINT16   Index;
  EFI_STATUS  Status = EFI_NOT_FOUND;

  //RAS_DEBUG ((LEVEL_REG, "PcieStdLib PcieAerGetErrCapData Start!\n"));

  Index = 0;
  PciPrimaryStatus = MmioRead8 (MmPciAddressExt (Socket, Bus, Device, Function, PCI_PRIMARY_STATUS_OFFSET, MmInfo));
  //RAS_DEBUG ((LEVEL_REG, "PcieStdLib PcieAerGetErrCapData PciPrimaryStatus = 0x%x!\n",PciPrimaryStatus));
  if (PciPrimaryStatus & EFI_PCI_STATUS_CAPABILITY) {
    Offset = PcieGetExtendedCapabilitiesOffset (Socket, Bus, Device, Function,
               MmInfo, PCI_EXPRESS_EXTENDED_CAPABILITY_ADVANCED_ERROR_REPORTING_ID, 0xff);
    if (Offset != 0) {
      // Copy AER to buffer
      AerPointer = ((EFI_PCIE_ERROR_DATA_AER*) AerErrDataCap)->PcieAer;
      for (Index = 0; Index < sizeof(PCI_EXPRESS_EXTENDED_CAPABILITIES_ADVANCED_ERROR_REPORTING) ; Index++) {
        *AerPointer++ = MmioRead8 (MmPciAddressExt (Socket, Bus, Device, Function, Offset + Index, MmInfo));
      }
      Status = EFI_SUCCESS;
    }
  }

  //RAS_DEBUG ((LEVEL_REG, "PcieStdLib PcieAerGetErrCapData End!\n"));
  return Status;
}

/**
  Get PCIE device's AER uncorrectable error status.

  @param Socket    - Socket
  @param Bus       - Bus
  @param Device    - Device
  @param Function  - Function
  @param MmInfo    - Memory Map information of target device
  @retval UINT32   - Pcie AER uncorrectable error status.
**/
UINT32
EFIAPI
PcieAerGetUncErrSts (
  IN UINT8            Socket,
  IN UINT8            Bus,
  IN UINT8            Device,
  IN UINT8            Function,
  IN PCI_EXT_MM_INFO  *MmInfo
  )
{
  UINT32  UncErrSts;
  UINT16  AerCapOffset;

  AerCapOffset = PcieGetExtendedCapabilitiesOffset (Socket, Bus, Device, Function,
                   MmInfo, PCI_EXPRESS_EXTENDED_CAPABILITY_ADVANCED_ERROR_REPORTING_ID, 0xff);

  if (AerCapOffset != 0) {
    UncErrSts = MmioRead32 (MmPciAddressExt (Socket, Bus, Device, Function, AerCapOffset + PCIE_AER_OFFSET_REG_UES, MmInfo));
    RAS_DEBUG ((LEVEL_REG, "PcieAerGetUncErrSts UncErrSts = 0x%x\n", UncErrSts));
    return UncErrSts;
  } else {
    RAS_DEBUG ((LEVEL_REG, "      PcieAer is not supported\n"));
    return 0;
  }
}

/**
  Get PCIE device's AER uncorrectable error masks.

  @param Socket    - Socket ID
  @param Bus       - Bus
  @param Device    - Device
  @param Function  - Function
  @param MmInfo    - Memory Map information of target device
  @retval UINT32   - Pcie AER uncorrectable error masks.
**/
UINT32
EFIAPI
PcieAerGetUncErrMsk (
  IN UINT8            Socket,
  IN UINT8            Bus,
  IN UINT8            Device,
  IN UINT8            Function,
  IN PCI_EXT_MM_INFO  *MmInfo
  )
{
  UINT32  UncErrMsk;
  UINT16  AerCapOffset;

  AerCapOffset = PcieGetExtendedCapabilitiesOffset (Socket, Bus, Device, Function,
                   MmInfo, PCI_EXPRESS_EXTENDED_CAPABILITY_ADVANCED_ERROR_REPORTING_ID, 0xff);

  if (AerCapOffset != 0) {
    UncErrMsk = MmioRead32 (MmPciAddressExt (Socket, Bus, Device, Function, AerCapOffset + PCIE_AER_OFFSET_REG_UEM, MmInfo));
    RAS_DEBUG ((LEVEL_REG, "PcieAerGetUncErrMsk UncErrMsk = 0x%x\n", UncErrMsk));
    return UncErrMsk;
  } else {
    RAS_DEBUG ((LEVEL_REG, "      PcieAer is not supported\n"));
    return 0;
  }
}


/**
  Get PCIE device's AER correctable error masks.

  @param Socket        - Socket
  @param Bus           - Bus
  @param Device        - Device
  @param Function      - Function
  @param MmInfo        - Memory Map information of target device
  @retval EFI_STATUS   - Pcie AER correctable error status.
**/
UINT32
EFIAPI
PcieAerGetCorrErrMsk (
  IN UINT8            Socket,
  IN UINT8            Bus,
  IN UINT8            Device,
  IN UINT8            Function,
  IN PCI_EXT_MM_INFO  *MmInfo
  )
{
  UINT16 AerCapOffset;
  UINT32 CorrErrMsk;

  AerCapOffset = PcieGetExtendedCapabilitiesOffset (Socket, Bus, Device, Function,
                   MmInfo, PCI_EXPRESS_EXTENDED_CAPABILITY_ADVANCED_ERROR_REPORTING_ID, 0xff);
  if (AerCapOffset != 0) {
    CorrErrMsk = MmioRead32 (MmPciAddressExt (Socket, Bus, Device, Function, AerCapOffset + PCIE_AER_OFFSET_REG_CEM, MmInfo));
    RAS_DEBUG ((LEVEL_REG, "PcieAerGetCorrErrMsk CorrErrMsk = 0x%x\n", CorrErrMsk));
    return CorrErrMsk;
  } else {
    RAS_DEBUG ((LEVEL_REG, "      PcieAer is not supported\n"));
    return 0;
  }
}

/**
  Get PCIE device's AER correctable error status.

  @param Socket        - Socket
  @param Bus           - Bus
  @param Device        - Device
  @param Function      - Function
  @param MmInfo        - Memory Map information of target device
  @retval EFI_STATUS   - Pcie AER correctable error status.
**/
UINT32
EFIAPI
PcieAerGetCorrErrSts (
  IN UINT8            Socket,
  IN UINT8            Bus,
  IN UINT8            Device,
  IN UINT8            Function,
  IN PCI_EXT_MM_INFO  *MmInfo
  )
{
  UINT32  CorrErrSts;
  UINT16  AerCapOffset;

  AerCapOffset = PcieGetExtendedCapabilitiesOffset (Socket, Bus, Device, Function,
                   MmInfo, PCI_EXPRESS_EXTENDED_CAPABILITY_ADVANCED_ERROR_REPORTING_ID, 0xff);
  if (AerCapOffset != 0) {
    CorrErrSts = MmioRead32 (MmPciAddressExt (Socket, Bus, Device, Function, AerCapOffset + PCIE_AER_OFFSET_REG_CES, MmInfo));
    RAS_DEBUG ((LEVEL_REG, "PcieAerGetCorrErrSts CorrErrSts = 0x%x\n", CorrErrSts));
    return CorrErrSts;
  } else {
    RAS_DEBUG ((LEVEL_REG, "      PcieAer is not supported\n"));
    return 0;
  }

}

/**
  Get PCIE device's AER correctable error status.

  @param Socket        - Socket
  @param Bus           - Bus
  @param Device        - Device
  @param Function      - Function
  @param MmInfo        - Memory Map information of target device
  @param CorrErrMask   - CorrErrMask
  @param UnCorrErrMask - UnCorrErrMask
  @param ErrorSev      - ErrorSev
  @param AeccConf      - AeccConf
  @retval VOID
**/
VOID
PcieAerConfig (
  IN UINT8            Socket,
  IN UINT8            Bus,
  IN UINT8            Device,
  IN UINT8            Function,
  IN PCI_EXT_MM_INFO  *MmInfo,
  IN UINT32           CorrErrMask,
  IN UINT32           UnCorrErrMask,
  IN UINT32           ErrorSev,
  IN UINT32           AeccConf
  )
{
  if (IsPcieAerSupported (Socket, Bus, Device, Function, MmInfo)) {
    PcieAerClearCorrErrSts (Socket,Bus, Device, Function, MmInfo);
    PcieAerClearUncErrSts (Socket, Bus, Device, Function, MmInfo);
    PcieAerClearRootErrSts (Socket,Bus, Device, Function, MmInfo, (COR_ERROR_TYPE | NONFATAL_ERROR_TYPE | FATAL_ERROR_TYPE));
    PcieAerProgramCorrErrMsk (Socket, Bus, Device, Function, MmInfo, CorrErrMask);
    PcieAerProgramUncErrMsk (Socket, Bus, Device, Function, MmInfo, UnCorrErrMask);
    PcieAerProgramErrSev (Socket, Bus, Device, Function, MmInfo, ErrorSev);
    PcieAerProgramAecc (Socket, Bus, Device, Function, MmInfo, AeccConf);
  }
}

/**
  Get PCIE device's AER root port error status.

  @param Socket      - Socket
  @param Bus         - Bus
  @param Device      - Device
  @param Function    - Function
  @param MmInfo      - Memory Map information of target device
  @param RpErrSts    - Root Error Status
  @retval EFI_STATUS - EFI_SUCCESS Root Error Status returned, EFI_UNSUPPORTED AER or Root Status Not supported.
**/
EFI_STATUS
EFIAPI
PcieAerGetRpErrSts (
  IN UINT8            Socket,
  IN UINT8            Bus,
  IN UINT8            Device,
  IN UINT8            Function,
  IN PCI_EXT_MM_INFO  *MmInfo,
  IN OUT  UINT32      *RpErrSts
  )
{
  UINT16  AerCapOffset;

  if (!IsPcieRootPort(Socket, Bus, Device, Function, MmInfo) && !IsPcieRcecSupported(Socket, Bus, Device, Function, MmInfo)) {
    return EFI_UNSUPPORTED;
  };

  AerCapOffset = PcieGetExtendedCapabilitiesOffset (Socket, Bus, Device, Function,
                   MmInfo, PCI_EXPRESS_EXTENDED_CAPABILITY_ADVANCED_ERROR_REPORTING_ID, 0xff);
  if (AerCapOffset != 0) {
    *RpErrSts = MmioRead32 (MmPciAddressExt (Socket, Bus, Device, Function, AerCapOffset + PCIE_AER_OFFSET_REG_RES, MmInfo));
    RAS_DEBUG ((LEVEL_REG, "GetPcieRootErrSts RpErrSts = 0x%x\n", *RpErrSts));
  } else {
    *RpErrSts = 0;
    RAS_DEBUG ((LEVEL_REG, "      PcieAer is not supported\n"));
    return EFI_UNSUPPORTED;
  }
  return EFI_SUCCESS;
}

/**
  Get PCIE device's AER error source identification register.
  Only root port or error collector implements ERRSID register.
  Caller should pay attention about it. DMI down stream also implements it.

  @param Socket        - Socket
  @param Bus           - Bus
  @param Device        - Device
  @param Function      - Function
  @param MmInfo        - Memory Map information of target device
  @retval EFI_STATUS   - Pcie AER error source identification register.
**/
UINT32
EFIAPI
PcieAerGetErrSid (
  IN UINT8            Socket,
  IN UINT8            Bus,
  IN UINT8            Device,
  IN UINT8            Function,
  IN PCI_EXT_MM_INFO  *MmInfo
  )
{
  UINT32  ErrSid;
  UINT16  AerCapOffset;

  AerCapOffset = PcieGetExtendedCapabilitiesOffset (Socket, Bus, Device, Function,
                   MmInfo, PCI_EXPRESS_EXTENDED_CAPABILITY_ADVANCED_ERROR_REPORTING_ID, 0xff);
  if (AerCapOffset != 0) {
    ErrSid = MmioRead32 (MmPciAddressExt (Socket, Bus, Device, Function, AerCapOffset + PCIE_AER_OFFSET_REG_ERRSID, MmInfo));
    RAS_DEBUG ((LEVEL_REG, "PcieAerGetErrSid ERRSID = 0x%x\n", ErrSid));
    return ErrSid;
  } else {
    RAS_DEBUG ((LEVEL_REG, "      PcieAer is not supported\n"));
    return 0;
  }
}
