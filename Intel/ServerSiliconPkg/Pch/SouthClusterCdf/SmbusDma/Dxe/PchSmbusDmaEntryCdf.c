/** @file
  PCH Smbus Dma Driver

@copyright
  INTEL CONFIDENTIAL
  Copyright 2018 Intel Corporation.

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
#include <Library/MmPciBaseLib.h>
#include <Library/PchPcrLib.h>
#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/BaseLib.h>
#include <Protocol/SmbusHc.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <PiDxe.h>
#include <Library/HobLib.h>
#include <Guid/HobList.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/ConfigBlockLib.h>
#include <Private/PchConfigHob.h>
#include <Private/Library/PchSmbusCommonLib.h>
#include <IndustryStandard/Pci30.h>
#include <Register/PchRegs.h>
#include <Register/PchRegsSmbus.h>

/**
  <b>SMBus DXE Module Entry Point</b>\n
  - <b>Introduction</b>\n
    The SMBus module is a DXE driver that provides a standard way to execute an SMBus command.

  - @pre
    - @link _PCH_POLICY PCH_POLICY_HOB @endlink
      - This module uses the parameters in SmbusConfig for platform reserved (non ARP capable) addresses.

  - @result
    The SMBus driver produces EFI_SMBUS_HC_PROTOCOL which is documented in the SMBus Host Controller
    Protocol Specification.

  @param[in] ImageHandle          ImageHandle of this module
  @param[in] SystemTable          EFI System Table

  @retval EFI_SUCCESS             Driver initializes successfully
  @retval Other values            Some error occurred
**/
EFI_STATUS
EFIAPI
InitializePchSmbusDma (
  IN      EFI_HANDLE            ImageHandle,
  IN      EFI_SYSTEM_TABLE      *SystemTable
  )
{

  UINT32                   SmbusSpeed;
  UINT64                   SmbusHostMemBase;
  UINT64                   SmbusHostPcieBase;
  PHYSICAL_ADDRESS         MemoryAddress;
  EFI_PEI_HOB_POINTERS     HobList;
  PCH_CONFIG_HOB           *PchConfigHob;
  EFI_STATUS               Status;


  DEBUG ((DEBUG_INFO, "InitializePchSmbusDma() Start\n"));

  SmbusHostPcieBase = PCI_SEGMENT_LIB_ADDRESS (
                         DEFAULT_PCI_SEGMENT_NUMBER_PCH,
                         PCI_BUS_NUMBER_SMBUS_HOST,
                         PCI_DEVICE_NUMBER_SMBUS_HOST,
                         PCI_FUNCTION_NUMBER_SMBUS_HOST,
                         0
                      );

  if (PciSegmentRead16 (SmbusHostPcieBase + PCI_VENDOR_ID_OFFSET) == 0xFFFF) {
    DEBUG ((DEBUG_INFO, "Smbus Dma is disabled.\n"));
    return EFI_SUCCESS;
  }

  //
  // Get PCH Config HOB.
  //
  HobList.Guid = GetFirstGuidHob (&gPchConfigHobGuid);
  ASSERT (HobList.Guid != NULL);
  PchConfigHob = (PCH_CONFIG_HOB *) GET_GUID_HOB_DATA (HobList.Guid);

  //
  //Setup Temporary base address
  //
  PciSegmentWrite32(SmbusHostPcieBase + R_SMBUS_HOST_SMTBAR, PCH_TEMP_BASE_ADDRESS);
  PciSegmentWrite32(SmbusHostPcieBase + PCI_COMMAND_OFFSET,
        EFI_PCI_COMMAND_MEMORY_SPACE | EFI_PCI_COMMAND_BUS_MASTER);


  ///
  /// If SMBus IOSF Clock Gating setting is enabled, then set IOSFDEVCLKGCTL ICGE bit register
  ///
  if (PchConfigHob->Smbus.SMBusIOSFClockGating) {
    PciSegmentOr16((UINTN) (SmbusHostPcieBase + R_SMBUS_IOSFDEVCLKGCTL), (UINT16) (V_SMBUS_IOSFDEVCLKGCTL_ICGE));
  } else {
    PciSegmentAnd16((UINTN) (SmbusHostPcieBase + R_SMBUS_IOSFDEVCLKGCTL), (UINT16) (~V_SMBUS_IOSFDEVCLKGCTL_ICGE));
  }


  SmbusHostMemBase = PciSegmentRead32(SmbusHostPcieBase + R_SMBUS_HOST_SMTBAR) & B_SMBUS_HOST_SMTBAR_MBA;

  switch(PchConfigHob->Smbus.SMBusHostSpeedConfig) {
    case PchSMBusStandard80Khz:
      SmbusSpeed = V_SMBUS_SMTBAR_SPGT_80KHZ;
      break;
    case PchSMBusStandard100Khz:
      SmbusSpeed = V_SMBUS_SMTBAR_SPGT_100KHZ;
      break;
    case PchSMBusFastMode400Khz:
      SmbusSpeed = V_SMBUS_SMTBAR_SPGT_400KHZ;
      break;
    case PchSMBusFastModePlus1Mhz:
      SmbusSpeed = V_SMBUS_SMTBAR_SPGT_1MHZ;
      break;
    default:
      SmbusSpeed = V_SMBUS_SMTBAR_SPGT_80KHZ;
  }

  MmioOr32(SmbusHostMemBase + R_SMBUS_SMTBAR_MCTRL, B_SMBUS_SMTBAR_MCTRL_SPDDIS);

  MmioAndThenOr32(SmbusHostMemBase + R_SMBUS_SMTBAR_SPGT, (UINT32) ~B_SMBUS_SMTBAR_SPGT_SPD, SmbusSpeed);

  //Host SMBUS config

  //Reserve Memory for MDBA and TBBA
  MemoryAddress = 0xFFFFFFFF;
  Status = gBS->AllocatePages (
             AllocateMaxAddress,
             EfiReservedMemoryType,
             EFI_SIZE_TO_PAGES (SIZE_4KB),
             &MemoryAddress
           );
  if(EFI_ERROR(Status)) {
    DEBUG ((DEBUG_INFO, "Host SMBus Config: Cannot allocate memory for Master Descriptor MDBA\n"));
  } else {
    //Program Base MDBA
    DEBUG ((DEBUG_INFO, "MemoryBase Address for MDBA 0x%08x\n", MemoryAddress));
    MmioWrite64(SmbusHostMemBase + R_SMBUS_SMTBAR_MDBA, MemoryAddress);
  }
  Status = gBS->AllocatePages (
             AllocateMaxAddress,
             EfiReservedMemoryType,
             EFI_SIZE_TO_PAGES (SIZE_64KB),
             &MemoryAddress
           );
  if(EFI_ERROR(Status)) {
    DEBUG ((DEBUG_INFO, "Host SMBus Config: Cannot allocate memory for Target Descriptor\n"));
  } else {
    //Program Base TBBA
    DEBUG ((DEBUG_INFO, "MemoryBase Address for TBBA 0x%08x\n", MemoryAddress));
    MmioWrite64(SmbusHostMemBase + R_SMBUS_SMTBAR_TBBA, MemoryAddress);
  }

  //PECI SMBus
  PchPcrAndThenOr32(PID_PECI_SMBUS, R_PECI_SMBUS_SPGT, (UINT32) ~B_PECI_SMBUS_SPGT_SPD, V_PECI_SMBUS_SPGT_100KHZ);

  PciSegmentWrite32(SmbusHostPcieBase + R_SMBUS_HOST_SMTBAR, 0);

  //Reset PCI command register to 0
  PciSegmentWrite32(SmbusHostPcieBase + PCI_COMMAND_OFFSET, 0);

  DEBUG ((DEBUG_INFO, "InitializePchSmbusDma() End\n"));

  return EFI_SUCCESS;
}
