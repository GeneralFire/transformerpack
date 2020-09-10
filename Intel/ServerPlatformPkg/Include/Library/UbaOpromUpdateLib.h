/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2008 - 2013 Intel Corporation. <BR>

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

#ifndef _PLATFORM_OPTION_ROM_UPDATE_LIB_
#define _PLATFORM_OPTION_ROM_UPDATE_LIB_


#include <Base.h>
#include <Uefi.h>

#include <Guid/SetupVariable.h>
#include <Guid/SocketVariable.h>
#include "OnboardNicStructs.h"

#define PLATFORM_OPTION_ROM_UPDATE_SIGNATURE  SIGNATURE_32 ('P', 'B', 'D', 'S')
#define PLATFORM_OPTION_ROM_UPDATE_VERSION    01


// {371BD79C-DE79-4c5f-AA2B-BC9EBEFA988F}
STATIC EFI_GUID  gPlatformOptionRomUpdateConfigDataGuid =
{ 0x371bd79c, 0xde79, 0x4c5f, { 0xaa, 0x2b, 0xbc, 0x9e, 0xbe, 0xfa, 0x98, 0x8f } };


typedef struct {
  EFI_GUID  FileName;
  UINTN     Segment;
  UINTN     Bus;
  UINTN     Device;
  UINTN     Function;
  UINT16    VendorId;
  UINT16    DeviceId;
} PC_PCI_OPTION_ROM_TABLE;


typedef
BOOLEAN
(*PLATFORM_PCIE_ROOT_PORT_CHECK_CALLBACK) (
  IN  UINTN                             Bus,
  IN  UINT32                            PcieSlotOpromBitMap
  );

typedef
EFI_STATUS
(*PLATFORM_GET_OPTIONROM_TABLE) (
  IN  PC_PCI_OPTION_ROM_TABLE         **OptionRomTable
  );

typedef
EFI_STATUS
(*PLATFORM_GET_NIC_SETUP_CONFIG) (
  IN  NIC_SETUP_CONFIGURATION_STUCT     **NicSetupConfigTable,
  IN  UINTN                             *NumOfConfig
  );

typedef
EFI_STATUS
(*PLATFORM_GET_NIC_CAPABILITY_TABLE) (
  IN  NIC_OPTIONROM_CAPBILITY_STRUCT    **NicCapabilityTable,
  IN  UINTN                             *NumOfNicCapTable
  );

typedef
EFI_STATUS
(*PLATFORM_SETUP_PCIE_SLOT_NUMBER ) (
  OUT UINT8                              *PcieSlotItemCtrl
  );

typedef struct
{
  UINT32                                    Signature;
  UINT32                                    Version;

  PLATFORM_PCIE_ROOT_PORT_CHECK_CALLBACK    CallCheckRootPort;
  PLATFORM_GET_OPTIONROM_TABLE              GetOptionRomTable;
  PLATFORM_GET_NIC_SETUP_CONFIG             GetNicSetupConfigTable;
  PLATFORM_GET_NIC_CAPABILITY_TABLE         GetNicCapabilityTable;
  PLATFORM_SETUP_PCIE_SLOT_NUMBER           SetupSlotNumber;

} PLATFORM_OPTION_ROM_UPDATE_DATA;


BOOLEAN
PlatformCheckPcieRootPort (
  IN  UINTN                             Bus,
  IN  UINT32                            PcieSlotOpromBitMap
);

EFI_STATUS
PlatformGetOptionRomTable (
  IN  PC_PCI_OPTION_ROM_TABLE         **OptionRomTable
);

EFI_STATUS
PlatformGetNicSetupConfigTable (
  IN  NIC_SETUP_CONFIGURATION_STUCT     **NicSetupConfigTable,
  IN  UINTN                             *NumOfConfig
  );

EFI_STATUS
PlatformGetNicCapabilityTable (
  IN  NIC_OPTIONROM_CAPBILITY_STRUCT    **NicCapabilityTable,
  IN  UINTN                             *NumOfNicCapTable
  );

EFI_STATUS
PlatformSetupPcieSlotNumber (
    OUT  UINT8                   *PcieSlotItemCtrl
);

#endif //_PLATFORM_OPTION_ROM_UPDATE_LIB_
