/** @file
  Perform PCH USB related functions in DXE phase

@copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation.

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
#include <Library/PciSegmentLib.h>
#include <Library/PchPciBdfLib.h>
#include <Library/IioDmiApi.h>
#include <Register/UsbRegs.h>

#include "PchInit.h"


enum USB_DMA_REQUEST_SIZE {
  PchUsbDmaRequestSize128    = 0x0,
  PchUsbDmaRequestSize256    = 0x1,
  PchUsbDmaRequestSize512    = 0x2,
  PchUsbDmaRequestSize64     = 0x7
};

/**
  Perform the remaining configuration on USB.
  Set the XHCC2.WRREQSZCTRL and XHCC2.RDREQSZCTRL based on the DMI DEVCTL MPS configured value

  @retval None
**/
VOID
ConfigureUsbOnEndOfDxe (
  VOID
  )
{
  UINT32  Data32Or;
  UINT32  Data32And;
  UINT32  UsbBufSize;

  DEBUG ((DEBUG_INFO, "%a start\n", __FUNCTION__));

  switch (IioDmiGetMaxPayloadSize ()) {
  case PchPcieMaxPayload128:
    UsbBufSize = PchUsbDmaRequestSize128;
    DEBUG ((DEBUG_INFO, "PchUsbDmaRequestSize=128B\n"));
    break;

  case PchPcieMaxPayload256:
    UsbBufSize = PchUsbDmaRequestSize256;
    DEBUG ((DEBUG_INFO, "PchUsbDmaRequestSize=256B\n"));
    break;

  default:
    UsbBufSize = PchUsbDmaRequestSize512;
    DEBUG ((DEBUG_INFO, "PchUsbDmaRequestSize=512B\n"));
    break;
  };

  Data32And = (UINT32)~(B_XHCI_CFG_XHCC2_WRREQSZCTRL | B_XHCI_CFG_XHCC2_RDREQSZCTRL);
  Data32Or  = (UsbBufSize << N_XHCI_CFG_XHCC2_WRREQSZCTRL) | UsbBufSize;
  PciSegmentAndThenOr32 (
    PchXhciPciCfgBase () + R_XHCI_CFG_XHCC2,
    Data32And,
    Data32Or
    );

  DEBUG ((DEBUG_INFO, "%a end\n", __FUNCTION__));
}
