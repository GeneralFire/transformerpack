/** @file
  Implementation of Pcie Aer Lib.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 Intel Corporation. <BR>

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

#include <Uefi/UefiBaseType.h>
#include <IndustryStandard/PciExpress31.h>
#include <Library/PcieStdLib.h>
#include <Library/MmPciExtLib.h>
#include <Library/RasDebugLib.h>
#include <Library/BaseLib.h>
#include <IndustryStandard/Pci.h>
#include "Register/PcieRegDef.h"
#include <Guid/Cper.h>
#include <Library/IoLib.h>

/**
  Check if PCIE device support RCEC or not.

  @param Socket    - Socket
  @param Bus       - Bus
  @param Device    - Device
  @param Function  - Function
  @param MmInfo    - Memory Map information of target device

  @retval BOOLEAN  - TRUE: RCEC is supported; FALSE:RCEC is not supported.

**/
BOOLEAN
EFIAPI
IsPcieRcecSupported (
  IN UINT8            Socket,
  IN UINT8            Bus,
  IN UINT8            Device,
  IN UINT8            Function,
  IN PCI_EXT_MM_INFO  *MmInfo
  )
{
  //RAS_DEBUG((LEVEL_REG,"PcieRasLib IsPcieRcecSupported."));

  if (PcieGetExtendedCapabilitiesOffset (Socket, Bus, Device, Function,
        MmInfo, PCI_EXPRESS_EXTENDED_CAPABILITY_EVENT_COLLECTOR_ENDPOINT_ASSOCIATION_ID, 0xff) != 0) {
    return TRUE;
  }

  return FALSE;
}

/**
  Get RCEC Next Bus, Last Bus Number and Associated Bit Map

  @param Socket    - Socket
  @param Bus       - Bus
  @param Device    - Device
  @param Function  - Function
  @param MmInfo    - Memory Map information of target device
  @param NextBus   - Next Bus Number of associated RCiEPs
  @param LastBus   - Last Bus Number of associated RCiEPs
  @param Abm       - Associate Bit Map of the device on the same Bus Number of RCEC
  @retval none
**/
VOID
EFIAPI
GetRcecNbnLbnAbm (
  IN  UINT8            Socket,
  IN  UINT8            Bus,
  IN  UINT8            Device,
  IN  UINT8            Function,
  IN  PCI_EXT_MM_INFO  *MmInfo,
  OUT UINT8            *NextBus,
  OUT UINT8            *LastBus,
  OUT UINT32           *Abm
  )
{

  UINT16        RcecCapOffset;
  UINT32        Data32;

  RcecCapOffset = PcieGetExtendedCapabilitiesOffset (Socket, Bus, Device, Function,
                    MmInfo, PCI_EXPRESS_EXTENDED_CAPABILITY_EVENT_COLLECTOR_ENDPOINT_ASSOCIATION_ID, 0xff);

  *NextBus = 0xff;
  *LastBus = 0;
  *Abm = 0;
  //
  // Get ABM, AbnLbn, AbnNbn for RCEC
  //
  if (RcecCapOffset != 0) {
    //
    // Check the capability version, if the version great or equal version 2 get next bus/last bus
    // from the ABN registers
    //
    Data32 = MmioRead32 (MmPciAddressExt (Socket, Bus, Device, Function, RcecCapOffset, MmInfo));
    if (((Data32 >> 16) & 0xf) >= 2) {
      Data32 = MmioRead32 (MmPciAddressExt (Socket, Bus, Device, Function, RcecCapOffset + ABNRCIEP_OFFSET, MmInfo));
      *NextBus = (UINT8) (Data32 >> 8);
      *LastBus = (UINT8) (Data32 >> 16);
      RAS_DEBUG((LEVEL_FUNC_FLOW,"GetRcecNbnLbnAbm NextBus = 0x%x LastBus = 0x%x\n", *NextBus, *LastBus));
    }

    Data32 = MmioRead32 (MmPciAddressExt (Socket, Bus, Device, Function, RcecCapOffset + ABMRCIEP_OFFSET, MmInfo));
    *Abm = Data32;
    RAS_DEBUG((LEVEL_FUNC_FLOW,"GetRcecNbnLbnAbm ABMRCIEP_OFFSET = 0x%x\n", *Abm));
  }
}