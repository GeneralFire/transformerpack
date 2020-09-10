/** @file
  PCIE root port library.
  All function in this library is available for PEI, DXE, and SMM,
  But do not support UEFI RUNTIME environment call.

@copyright
  INTEL CONFIDENTIAL
  Copyright 2019 Intel Corporation.

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

#include <Base.h>
#include <Uefi/UefiBaseType.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/PchInfoLib.h>
#include <Library/PchPcrLib.h>
#include <Library/PchPcieRpLib.h>
#include <PcieRegs.h>
#include <Register/PchRegs.h>
#include <PchBdfAssignment.h>
#include <PchPcieRpInfo.h>
#include <Register/PchPcieRpRegs.h>
#include <Register/PchPcrRegs.h>

#include "PchPcieRpLibInternal.h"

GLOBAL_REMOVE_IF_UNREFERENCED PCH_PCIE_CONTROLLER_INFO mPchPcieControllerInfo[] = {
  { PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_1,  PID_PCIE0_P0, 0  },
  { PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_2,  PID_PCIE0_P1, 1  },
  { PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_3,  PID_PCIE0_P2, 2  },
  { PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_4,  PID_PCIE0_P3, 3  },

  { PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_5,  PID_PCIE1_P0, 4  },
  { PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_6,  PID_PCIE1_P1, 5  },
  { PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_7,  PID_PCIE1_P2, 6  },
  { PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_8,  PID_PCIE1_P3, 7  },

  { PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_9,  PID_PCIE2_P0, 8  },
  { PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_10, PID_PCIE2_P1, 9  },
  { PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_11, PID_PCIE2_P2, 10 },
  { PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_12, PID_PCIE2_P3, 11 },

  { PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_13, PID_PCIE3_P0, 12 },
  { PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_14, PID_PCIE3_P1, 13 },
  { PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_15, PID_PCIE3_P2, 14 },
  { PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_16, PID_PCIE3_P3, 15 }
};

/**
  Get the PCIe controller topology description for the PCH

  @param[out] PchPcieControllerInfo    On output pointer to the info table
  @param[out] PchPcieControlerInfoSize On output size of the info table in the number of items
**/
VOID
PchPcieGetControllerInfo (
  OUT PCH_PCIE_CONTROLLER_INFO  **PchPcieControllerInfo,
  OUT UINT32                    *PchPcieControllerInfoSize
  )
{
  *PchPcieControllerInfo = mPchPcieControllerInfo;
  *PchPcieControllerInfoSize = ARRAY_SIZE (mPchPcieControllerInfo);
}

/**
  Get Pch Pcie Root Port Device and Function Number by Root Port physical Number

  @param[in]  RpNumber              Root port physical number. (0-based)
  @param[out] RpDev                 Return corresponding root port device number.
  @param[out] RpFun                 Return corresponding root port function number.

  @retval     EFI_SUCCESS           Root port device and function is retrieved
  @retval     EFI_INVALID_PARAMETER RpNumber is invalid
**/
EFI_STATUS
EFIAPI
GetPchPcieRpDevFun (
  IN  UINTN   RpNumber,
  OUT UINTN   *RpDev,
  OUT UINTN   *RpFun
  )
{
  if (RpNumber >= GetPchMaxPciePortNum ()) {
    DEBUG ((DEBUG_ERROR, "GetPchPcieRpDevFun invalid RpNumber %x", RpNumber));
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }
  *RpDev = mPchPcieControllerInfo[RpNumber].DevNum;
  *RpFun = 0;

  return EFI_SUCCESS;
}

/**
  Get Root Port physical Number by Pch Pcie Root Port Device and Function Number

  @param[in]  RpDev                 Root port device number.
  @param[in]  RpFun                 Root port function number.
  @param[out] RpNumber              Return corresponding physical Root Port index (0-based)

  @retval     EFI_SUCCESS           Physical root port is retrieved
**/
EFI_STATUS
EFIAPI
GetPchPcieRpNumber (
  IN  UINTN   RpDev,
  IN  UINTN   RpFun,
  OUT UINTN   *RpNumber
  )
{
  (VOID)RpFun;

  if (RpDev >= PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_1 && RpDev <= PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_12) {
    *RpNumber = (RpDev - PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_1);
  } else if (RpDev >= PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_13 && RpDev <= PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_16) {
    *RpNumber = (RpDev - PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_13) + 12;
  } else {
    DEBUG ((DEBUG_ERROR, "PciePortNum () RpDev=%d\n", RpDev));
    ASSERT ((RpDev >= PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_1  && RpDev <= PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_12) ||
            (RpDev >= PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_13 && RpDev <= PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_16));
    return 0;
  }
  return EFI_SUCCESS;
}

/**
  This function returns PID according to PCIe controller index

  @param[in] ControllerIndex     PCIe controller index

  @retval PCH_SBI_PID    Returns PID for SBI Access
**/
PCH_SBI_PID
PchGetPcieControllerSbiPid (
  IN UINT32  ControllerIndex
  )
{
  ASSERT (ControllerIndex < GetPchMaxPcieControllerNum());
  return mPchPcieControllerInfo[ControllerIndex * PCH_PCIE_CONTROLLER_PORTS].Pid;
}

/**
  This function returns PID according to Root Port Number

  @param[in] RpIndex     Root Port Index (0-based)

  @retval PCH_SBI_PID    Returns PID for SBI Access
**/
PCH_SBI_PID
GetRpSbiPid (
  IN UINTN  RpIndex
  )
{
  return mPchPcieControllerInfo[RpIndex].Pid;
}
