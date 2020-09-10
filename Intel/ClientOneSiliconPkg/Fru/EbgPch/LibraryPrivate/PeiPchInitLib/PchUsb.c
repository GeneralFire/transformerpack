/** @file
  Initializes PCH USB Host Controller.

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
#include "PchUsb.h"
#include <Ppi/Spi.h>
#include <Library/PeiHsioLib.h>
#include <Library/UsbHostControllerInitLib.h>
#include <Library/PostCodeLib.h>
#include <Library/UsbLib.h>
#include <Library/Usb2PhyLib.h>
#include <Library/PchPciBdfLib.h>
#include <Register/PchRegs.h>
#include <Register/UsbRegs.h>

/*
  Performs PCH USB Controllers initialization

  @param[in] SiPolicy     The Silicon Policy PPI instance
*/
VOID
PchUsbConfigure (
  IN  SI_POLICY_PPI               *SiPolicy
  )
{
  EFI_STATUS            Status;
  USB_CONFIG            *UsbConfig;
  USB2_PHY_CONFIG       *Usb2PhyConfig;
  USB3_HSIO_CONFIG      *Usb3HsioConfig;
  UINT64                ControllerPciMmBase;
  UINT8                 InterruptPin;

  Status = GetConfigBlock ((VOID *) SiPolicy, &gUsbConfigGuid, (VOID *) &UsbConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gUsb2PhyConfigGuid, (VOID *) &Usb2PhyConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPolicy, &gUsb3HsioConfigGuid, (VOID *) &Usb3HsioConfig);
  ASSERT_EFI_ERROR (Status);

  ControllerPciMmBase = PchXhciPciCfgBase ();

  //
  // Tune the USB 2.0 high-speed signals quality.
  //
  PostCode (0xB04);
  Usb2PhyProgramming (Usb2PhyConfig);

  //
  // Configure USB3 ModPHY turning.
  //
  PostCode (0xB05);
  XhciUsb3HsioTuning (Usb3HsioConfig);

  //
  // Configure PCH xHCI
  //
  PostCode (0xB06);
  XhciConfigure (UsbConfig, ControllerPciMmBase);

  if (PciSegmentRead16 (ControllerPciMmBase + PCI_VENDOR_ID_OFFSET) != 0xFFFF) {
    ///
    /// Configure xHCI interrupt
    ///
    InterruptPin = ItssGetDevIntPin (SiPolicy, PchXhciDevNumber (), PchXhciFuncNumber ());

    PciSegmentWrite8 (ControllerPciMmBase + PCI_INT_PIN_OFFSET, InterruptPin);
  }

  //
  // Set all necessary lock bits in xHCI controller
  //
  XhciLockConfiguration (UsbConfig, ControllerPciMmBase);
}
