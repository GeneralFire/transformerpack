/** @file
  SATA SoC library.
  All function in this library is available for PEI, DXE, and SMM,
  But do not support UEFI RUNTIME environment call.

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

#include <Base.h>
#include <Uefi/UefiBaseType.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/PchPciBdfLib.h>
#include <Library/SataSocLib.h>
#include <Register/PchRegs.h>
#include <SataSocIntegration.h>

/**
  Get SATA controller instance

  @param[in]  SataCtrlIndex       SATA controller index
  @param[out] SataController      Pointer to SATA controller structure

  @retval     EFI_SUCCESS           - Completted successfully
              EFI_INVALID_PARAMETER - SataController NULL or SataCtrlIndex out of the range
**/
EFI_STATUS
SataGetController (
  IN  UINT32              SataCtrlIndex,
  OUT SATA_CONTROLLER     *SataController
  )
{
  if (SataController == NULL) {
    DEBUG ((DEBUG_ERROR, "%a: Null pointer at SataController!\n", __FUNCTION__));
    return EFI_INVALID_PARAMETER;
  }
  if (SataCtrlIndex >= MaxSataControllerNum ()) {
    DEBUG ((DEBUG_ERROR, "%a: Controller number out of the range!\n", __FUNCTION__));
    return EFI_INVALID_PARAMETER;
  }

  SataController->SataCtrlIndex     = SataCtrlIndex;
  SataController->Segment           = DEFAULT_PCI_SEGMENT_NUMBER_PCH;
  SataController->Bus               = DEFAULT_PCI_BUS_NUMBER_PCH;
  SataController->Device            = SataDevNumber (SataCtrlIndex);
  SataController->Function          = SataFuncNumber (SataCtrlIndex);
  SataController->PciCfgBaseAddr    = SataPciCfgBase (SataCtrlIndex);
  SataController->TotalCtrlPortNum  = MaxSataPortNum (SataCtrlIndex);

  return EFI_SUCCESS;
}

/**
  Get Maximum Sata Controller Number

  @retval Maximum Sata Controller Number
**/
UINT8
MaxSataControllerNum (
  VOID
  )
{
  //
  // Defined in SoC integration header
  //
  return MAX_SATA_CONTROLLER;
}

/**
  Get Maximum Sata Port Number

  @param[in]  SataCtrlIndex       SATA controller index

  @retval     Maximum Sata Port Number
**/
UINT8
MaxSataPortNum (
  IN UINT32      SataCtrlIndex
  )
{
  ASSERT (SataCtrlIndex < MaxSataControllerNum ());

  //
  // Number of ports defined in SoC integration header
  //
  return MAX_SATA_PORTS;
}

/**
  Check if SATA controller supports RST remapping

  @param[in]  SataCtrlIndex       SATA controller index

  @retval     TRUE                Controller supports remapping
  @retval     FALSE               Controller does not support remapping

**/
BOOLEAN
IsRemappingSupportedOnSata (
  IN UINT32  SataCtrlIndex
  )
{
  return FALSE;
}

/**
  Checks if SoC supports the SATA PGD power down on given
  SATA controller.

  @param[in] SataCtrlIndex  SATA controller index

  @retval TRUE   SATA PGD power down supported
  @retval FALSE  SATA PGD power down not supported
**/
BOOLEAN
IsSataPowerGatingSupported (
  IN UINT32 SataCtrlIndex
  )
{
  return FALSE;
}