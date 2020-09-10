/** @file
  Implementation of PCH RAS handlers.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2020 Intel Corporation. <BR>

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

#include "IioRasLibInternal.h"
#include <Library/PchCycleDecodingLib.h>
#include <IndustryStandard/Acpi51.h>
#include <Library/ErrorReportLib.h>
#include <Library/PchRasLib.h>
#include <Library/PeiDxeSmmGpioLib/GpioLibrary.h>
#include <PchAccess.h>
#include <Library/RasAcpiLib.h>


/**
Handler for PCH DMI

@param[in] Bus        Device BUS number to check

@retval EFI_SUCCESS.

**/
VOID
EFIAPI
PchDmiErrorHandler (
  VOID
  )
{
  UINT8     Device;
  UINT8     Function;
  UINT32    Sev;

  RAS_DEBUG((LEVEL_FUNC_FLOW,"PchDmiErrorHandler: Inside the function\n"));

  Device = 0;
  Function = 0;

  if (PchRasIsCrDmiError ()) {
    //
    // Report the correctable error.
    //
    ReportPcieErrorLog (0, DEFAULT_PCI_BUS_NUMBER_PCH, Device, Function, NULL, COR_ERROR_TYPE);
    Sev = GetSevByErrType (COR_ERROR_TYPE);
    RAS_DEBUG((LEVEL_FUNC_FLOW,"PchDmiErrorHandler Sev = 0x%x\n",Sev));
    AdjustOutputSeverity (Sev);

    PchRasDisableDmiCorrectError ();
  }


  if(PchRasIsEnrDmiError ()) {
    //
    // Report the Uncorrectable error.
    //
    ReportPcieErrorLog (0, DEFAULT_PCI_BUS_NUMBER_PCH, Device, Function, NULL, FATAL_ERROR_TYPE);

    Sev = GetSevByErrType (FATAL_ERROR_TYPE);
    RAS_DEBUG ((LEVEL_FUNC_FLOW,"PchDmiErrorHandler Sev = 0x%x\n",Sev));
    AdjustOutputSeverity (Sev);
  }

  PchRasClearDmiErrors (DEFAULT_PCI_BUS_NUMBER_PCH);
}

/**
  handle PCH PCI & root port devices errors.

  @param[in] Bus            Device's bus number.


  @retval    None
--*/
VOID
EFIAPI
PchErrorHandler (
  IN     UINT8           SevPerStack
  )
{
  UINT32           Index = 0;
  UINT8            Socket;
  UINT8            Bus;
  UINT8            Device;
  UINT8            Func;

  while (PchRasGetNextDevice (Index, &Socket, &Bus, &Device, &Func)) {

    Index++;

    if (!PcieIsDevicePresent (Socket, Bus, Device, Func, NULL)) {
      continue;
    }

    if (IsPcieDevice (Socket, Bus, Device, Func, NULL)) {
      if (IsPcieRootPort (Socket, Bus, Device, Func, NULL)) {
        PcieRootPortErrorHandler (Socket, Bus, Device, Func, NULL, SevPerStack);
      }
    } else {
      PciDeviceErrorHandler (Socket, Bus, Device, Func, NULL);
    }
  }
  PchRasClearSerrStatus ();
}

/**
  Addtional enable for PCH devices.

  @param[in] Bus            Device's bus number.
  @param[in] Device         Device's device number.
  @param[in] Function       Device's function number.
  @param[in] PCIE_INIT_PAR  Init parameters

  @retval    None
--*/
VOID
EFIAPI
PCHAddtionalEnable (
  IN      UINT8   Socket,
  IN      UINT8   Bus,
  IN      UINT8   Device,
  IN      UINT8   Function
  )
{
  UINT32    Data32;
  UINT16    AerCapOffset;

  AerCapOffset = PcieGetExtendedCapabilitiesOffset (Socket, Bus, Device, Function,
                   NULL, PCI_EXPRESS_EXTENDED_CAPABILITY_ADVANCED_ERROR_REPORTING_ID, 0xff);

  //
  // Override AER programming for PCH root ports
  //
  if (AerCapOffset != 0) {
    Data32 = MmioRead32 (MmPciAddressExt (0, Bus, Device, Function, AerCapOffset + PCIE_AER_OFFSET_REG_UEM, NULL));
    RAS_DEBUG((LEVEL_FUNC_FLOW,"PchEnableRootPortElog R_PCIEXP_AER_UEM = 0x%x\n",Data32));
    Data32 |= PCH_AER_UEM_OVERRIDE;
    MmioWrite32 (MmPciAddressExt (0, Bus, Device, Function, AerCapOffset + PCIE_AER_OFFSET_REG_UEM, NULL), Data32);
    Data32 = MmioRead32 (MmPciAddressExt (0, Bus, Device, Function, AerCapOffset + PCIE_AER_OFFSET_REG_UEM, NULL));
    RAS_DEBUG((LEVEL_FUNC_FLOW,"PchEnableRootPortElog R_PCIEXP_AER_UEM = 0x%x\n",Data32));
  }
}

/**
  Function to enable PCH DMI Errors

  @param[in] PcieErrEn       - Pcie error enable option.
  @param[in] PcieCorrErrEn   - correctable error enable.
  @param[in] PcieUncorrErren - uncorrectable error enable.
  @param[in] ErrCorMsk       - Correctable error Mask.
  @param[in] ErrUncMsk       - Uncorrectable error Mask.
  @param[in] ErrUncSev       - Uncorrectable error severity.
**/
VOID
EFIAPI
PchRasEnableDmiError(
  IN UINT8              PcieErrEn,
  IN UINT8              PcieCorrErrEn,
  IN UINT8              PcieUncorrErrEn,
  IN UINT32             ErrCorMsk,
  IN UINT32             ErrUncMsk,
  IN UINT32             ErrUncSev
  )
{
  PchRasEnableDmiErrorRepoting (PcieErrEn, PcieCorrErrEn, PcieUncorrErrEn, ErrCorMsk, ErrUncMsk, ErrUncSev);
}

/**
  Enable PCH RAS feature

  @param none
  @retval none
**/
VOID
EFIAPI
PchEnableErrors (
  VOID
  )
{
  UINT32           Index = 0;
  UINT8            Socket;
  UINT8            Bus;
  UINT8            Device;
  UINT8            Func;

  while (PchRasGetNextDevice (Index, &Socket, &Bus, &Device, &Func)) {

    Index++;

    if (!PcieIsDevicePresent (Socket, Bus, Device, Func, NULL)) {
      continue;
    }

    if (IsPcieRootPort (Socket, Bus, Device, Func, NULL)) {
      RasAcpiParamAddRootPortEntry (Socket, Bus, Device, Func);
    }

    PcieConfigDeviceAndSubDevices (Socket, Bus, Device, Func, NULL);
    PCHAddtionalEnable (Socket, Bus, Device, Func);
  }

}

