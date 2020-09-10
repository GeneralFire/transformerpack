/** @file
  Implementation of vtd silicon interface

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 - 2019 Intel Corporation. <BR>

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

#include <Library/CpuCsrAccessLib.h>
#include <RcRegs.h>
#include <Library/MmPciExtLib.h>
#include <Library/IoLib.h>

/**
  This function enable forwarding of Poison indication with the data.

   @param Bus       - Bus
   @param Device    - Device
   @param Function  - Function
   @param CorrErrMsk  - Correctable error mask

  @retval    none.
**/
VOID
EFIAPI
IioEnablePoison (
  IN      UINT8   Socket,
  IN      UINT8   Bus,
  IN      UINT8   Device,
  IN      UINT8   Function
  )
{
  ERRUNCMSK_IIO_PCIEGen4_STRUCT     UncErrMask;
  //
  // Set ERRUNCMSK for Port
  //
  UncErrMask.Data = MmioRead32 (MmPciAddressExt (Socket, Bus,
                              Device,
                              Function,
                              ONLY_REGISTER_OFFSET (ERRUNCMSK_IIO_PCIEGen4_REG), NULL));
  UncErrMask.Bits.ptlpem = 0;
  UncErrMask.Bits.ptlpebm = 0;
  MmioWrite32 (MmPciAddressExt (Socket, Bus,
              Device,
              Function,
              ONLY_REGISTER_OFFSET (ERRUNCMSK_IIO_PCIEGen4_REG),
			  NULL),
              UncErrMask.Data);
}

/**
  This function disable forwarding of Poison indication with the data.

   @param Bus       - Bus
   @param Device    - Device
   @param Function  - Function
   @param CorrErrMsk  - Correctable error mask

  @retval    none.
**/
VOID
EFIAPI
IioDisablePoison (
  IN      UINT8   Socket,
  IN      UINT8   Bus,
  IN      UINT8   Device,
  IN      UINT8   Function
  )
{
  ERRUNCMSK_IIO_PCIEGen4_STRUCT     UncErrMask;

  UncErrMask.Data = MmioRead32 (MmPciAddressExt (Socket, Bus,
                              Device,
                              Function,
                              ONLY_REGISTER_OFFSET (ERRUNCMSK_IIO_PCIEGen4_REG),
							  NULL));
  UncErrMask.Bits.ptlpem = 1;
  UncErrMask.Bits.ptlpebm = 1;
  MmioWrite32 (MmPciAddressExt (Socket, Bus,
              Device,
              Function,
              ONLY_REGISTER_OFFSET (ERRUNCMSK_IIO_PCIEGen4_REG),
			  NULL),
              UncErrMask.Data);
}

