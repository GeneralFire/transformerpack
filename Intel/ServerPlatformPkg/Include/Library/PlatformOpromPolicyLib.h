/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2015 - 2016 Intel Corporation. <BR>

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

#ifndef _PLAT_OPROM_
#define _PLAT_OPROM_
#include <Protocol/PciIo.h>

typedef enum _OPROM_LOAD_POLICY
{
  DONT_LOAD,
  EXCLUSIVE_LOAD,
  INCLUSIVE_LOAD
} OPROM_LOAD_POLICY;

/**
  Decide if BIOS embdded option roms should be loaded for a certain PCI device.

  @param  PciIo      PCI device to return the ROM image for.

  @retval TRUE       BIOS embedded option roms should not be loaded for the PCI device.
  @retval FALSE      BIOS embedded option roms could be loaded for the PCI device.
**/

BOOLEAN
PlatformOpromLoadDevicePolicy (
  IN EFI_PCI_IO_PROTOCOL *PciIo
  );

/**
  For devices that support multiple option roms like FCoE, PXE, iSCSI etc., this function decides if one of these BIOS embdded option roms should be loaded for a certain PCI device based on platform choices.

  @param  PciHandle      PCI device to return the ROM image for.
  @param  TableIndex     The index pointing to the option rom in the platform option rom table for the PCI device.

  @retval FALSE          The specific BIOS embedded option rom should not be loaded for the PCI device.
  @retval TRUE           The specific BIOS embedded option rom could be loaded for a certain PCI device.
**/

OPROM_LOAD_POLICY
PlatformOpromLoadTypePolicy (
  IN EFI_HANDLE PciHandle,
  IN UINTN      TableIndex
  );

/**
  Decide if a PCIe device option rom should be dispacthed.

  @param  PciHandle      PCI device handle.

  @retval FALSE          The specific PCIe option rom should not be dispatched for the PCI device.
  @retval TRUE           The specific PCIe option rom could be dispatched for a certain PCI device.

**/

BOOLEAN
PlatformOpromDispatchPolicy (
  IN  EFI_HANDLE                        DeviceHandle
);

/**
  Enable the legacy console redirection before dispatch the legacy ORPOM or disable the legacy console redirection after dispatch
  the legacy ORPOM based on setup option and SOL status.

  @param  Mode             Subfunction.
  @param  CheckIsAhciRom   If the device is legacy Ahci device.

  @retval

**/

VOID
PlatformOpromLegacyCRPolicy (
  IN     UINTN                           Mode,
  IN     BOOLEAN                         CheckIsAhciRom
);


#endif
