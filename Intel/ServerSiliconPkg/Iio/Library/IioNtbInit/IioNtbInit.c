/** @file
  Implement NTB port initialization

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 - 2020 Intel Corporation. <BR>

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

#include <IioSetupDefinitions.h>
#include <PcieRegs.h>

#include <Library/DebugLib.h>
#include <Library/IioDataHubLib.h>
#include <Library/IioAccessLib.h>

#ifndef IA32
#include <Library/S3BootScriptLib.h>
#endif

/**
Routine Description:
 Sets the given port Max. Payload size as per the user defined system configuration variable.
 This routine translates the PCI Port address to map correctly to port specific setup option.

Arguments:
  IioGlobalData  - Pointer to IIO_GLOBALS
  IioIndex       - index to CPU/IIO

Returns:
  None

**/
VOID
NtbSetMPL (
  IIO_GLOBALS                     *IioGlobalData,
  UINT8                           IioIndex,
  UINT8                           PortIndex
  )
{
  PCI_REG_PCIE_DEVICE_CONTROL     DevCtrl;
  UINT8                           MaxPortNumberPerSocket;
  IIO_CONFIG                      *SetupData;

  SetupData = GetIioConfig ();

  MaxPortNumberPerSocket = GetMaxPciePortNumberPerSocket (IioIndex);
  if (SetupData->IioPcieConfig.PcieMaxPayload[(IioIndex * MaxPortNumberPerSocket) + PortIndex] != PCIE_MAX_PAYLOAD_AUTO) {
    GetPcieCapReg16 (IioIndex, PortIndex, R_PCIE_DCTL_OFFSET, &(DevCtrl.Uint16));

    DevCtrl.Bits.MaxPayloadSize = 0x00; //Bits[7:5] 000: 128B max payload size 001: 256B max payload size 002: 512B max payload size
    DevCtrl.Bits.MaxPayloadSize |= (SetupData->IioPcieConfig.PcieMaxPayload[(IioIndex * MaxPortNumberPerSocket) + PortIndex] << 5);

    SetPcieCapReg16 (IioGlobalData, IioIndex, PortIndex, R_PCIE_DCTL_OFFSET, DevCtrl.Uint16);
  }
}
