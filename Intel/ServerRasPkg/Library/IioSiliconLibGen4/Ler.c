/** @file
  Implementation of LER Iio RAS lib.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2019 Intel Corporation. <BR>

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
**/

#include <Library/CpuCsrAccessLib.h>
#include <RcRegs.h>
#include <Library/MmPciExtLib.h>
#include <Library/IoLib.h>
#include <Library/PlatMemTopologyLib.h>
#include <Library/RasDebugLib.h>
#include <Library/PcieStdLib.h>
#include <Register/IioRegDef.h>
#include <Library/PcieStdLib.h>

/**
  This Function will enable all the Pcie errors, that need to be reported as LER.

  @param[in]  Skt       The socket number of the root port.
  @param[in]  Bus       The PCI bus number of the root port.
  @param[in]  Device    The PCI device number of the root port.
  @param[in]  Function  The PCI function number of the root port.

  @retval    Status.
**/
VOID
EFIAPI
IioLerEnable (
  IN      UINT8   Skt,
  IN      UINT8   Bus,
  IN      UINT8   Device,
  IN      UINT8   Function
  )
{
  //
  //     Not support in GEN4
  //
}

/**
  This Function will DisableLerMAerrorLogging.

  @param[in] Ioh        The IOH number.

  @retval    Status.
**/
EFI_STATUS
EFIAPI
IioLerDisMcaErrLog (
  IN      UINT8   SocketId
  )
{
  return EFI_SUCCESS;
}
