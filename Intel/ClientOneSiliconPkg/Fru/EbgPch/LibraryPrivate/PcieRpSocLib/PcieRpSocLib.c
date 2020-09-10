/** @file
  PCIE Root Port SoC library.
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
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/PchPciBdfLib.h>
#include <Library/PchIoExpanderBdfLib.h>
#include <Library/PchIoExpander.h>
#include <Library/PchInfoLib.h>
#include <Library/PcdLib.h>
#include <Library/PchPcieRpDev.h>
#include <Register/PchRegs.h>
#include <PchPcieRpInfo.h>
#include <PchLimits.h>

/**
  Get PCIE RP instance

  @param[in, out] RpIndex             Root Port index
  @param[in, out] RpDevice            Root Port device pointer

  @retval     EFI_SUCCESS           - Completted successfully
              EFI_INVALID_PARAMETER - RpDevice NULL
**/
EFI_STATUS
PchGetRpDevice (
  IN     UINT32             RpIndex,
  IN OUT PCH_RP_DEVICE      *RpDevice
  )
{
  if (RpDevice == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  ZeroMem (RpDevice, sizeof (PCH_RP_DEVICE));
  RpDevice->Segment           = DEFAULT_PCI_SEGMENT_NUMBER_PCH;
  RpDevice->Bus               = DEFAULT_PCI_BUS_NUMBER_PCH;
  RpDevice->Device            = PchPcieRpDevNumber (RpIndex);
  RpDevice->Function          = PchPcieRpFuncNumber (RpIndex);
  RpDevice->PciCfgBaseAddr    = PchPcieRpPciCfgBase (RpIndex);
  RpDevice->TempPciBusMax     = PcdGet8 (PcdSiliconInitTempPciBusMax);
  RpDevice->TempPciBusMin     = PcdGet8 (PcdSiliconInitTempPciBusMin);
  RpDevice->RootPortIndex     = RpIndex;
  RpDevice->CtrlIndex         = RpIndex / PCH_PCIE_CONTROLLER_PORTS;

  return EFI_SUCCESS;
}


/**
  Get IOE PCIE RP instance

  @param[in     ] IoeId                 I/O Expander Id (0 ... n - I/O Expander)
  @param[in, out] RpIndex               Root Port index
  @param[in, out] RpDevice              Root Port device pointer

  @retval     EFI_SUCCESS               Completted successfully
              EFI_INVALID_PARAMETER     RpDevice NULL or IoeId is out of range
  @retval     EFI_NOT_FOUND             Unable to find I/O Expander
  @retval     EFI_INCOMPATIBLE_VERSION  Found I/O Expander Info PPI is not compatible
**/
EFI_STATUS
IoeGetRpDevice (
  IN     UINT8              IoeId,
  IN     UINT32             RpIndex,
  IN OUT PCH_RP_DEVICE      *RpDevice
  )
{
  EFI_STATUS            Status;

  if (RpDevice == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (IoeId >= IO_EXPANDER_MAX) {
    return EFI_INVALID_PARAMETER;
  }

  ZeroMem (RpDevice, sizeof (PCH_RP_DEVICE));
  Status = PchGetSegmentByIoExpanderId (IoeId, &RpDevice->Segment);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  Status = PchGetBusByIoExpanderId (IoeId, &RpDevice->Bus);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  RpDevice->Device            = PchPcieRpDevNumber (RpIndex);
  RpDevice->Function          = PchPcieRpFuncNumber (RpIndex);
  RpDevice->PciCfgBaseAddr    = PchPcieRpPciCfgBaseByIoExpanderId (IoeId, RpIndex);
  RpDevice->TempPciBusMax     = RpDevice->Bus + PcdGet8 (PcdSiliconInitTempPciBusMax);
  RpDevice->TempPciBusMin     = RpDevice->Bus + PcdGet8 (PcdSiliconInitTempPciBusMin);
  RpDevice->RootPortIndex     = RpIndex;
  RpDevice->CtrlIndex         = RpIndex / PCH_PCIE_CONTROLLER_PORTS;
  RpDevice->Private           = (UINT32) IoeId;

  return EFI_SUCCESS;
}

