/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2006 - 2018 Intel Corporation. <BR>

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

#include <PlatPirqData.h>
#include <PlatDevData.h>
#include <IndustryStandard/LegacyBiosMpTable.h>

#ifndef V_INTEL_VID
#define V_INTEL_VID               0x8086
#endif

//
// Describes Local APICs' connections.
//
STATIC DEVICE_DATA_HW_LOCAL_INT      DeviceDataHwLocalInt1[] = {
  {
    {{0},{{0xFF,0},{0xFF,0},{0xFF,0}}},
    0x00,
    0xff,
    0x00,
    EfiLegacyMpTableEntryLocalIntTypeExtInt,
    EfiLegacyMpTableEntryLocalIntFlagsPolaritySpec,
    EfiLegacyMpTableEntryLocalIntFlagsTriggerSpec
  },
  {
    {{0},{{0xFF,0},{0xFF,0},{0xFF,0}}},
    0x00,
    0xff,
    0x01,
    EfiLegacyMpTableEntryLocalIntTypeInt,
    EfiLegacyMpTableEntryLocalIntFlagsPolaritySpec,
    EfiLegacyMpTableEntryLocalIntFlagsTriggerSpec
  },
};

//
// Describes system's address space mapping, specific to the system.
//
STATIC DEVICE_DATA_HW_ADDR_SPACE_MAPPING DeviceDataHwAddrSpace1[] = {
  //
  // Legacy IO addresses.
  //
  { {0}, EfiLegacyMpTableEntryExtSysAddrSpaceMappingIo,       0x0000,     0x1000    },
  /*
  { {0}, EfiLegacyMpTableEntryExtSysAddrSpaceMappingMemory,   0xXXXXXXXX, 0xXXXXXXXX},
  { {0}, EfiLegacyMpTableEntryExtSysAddrSpaceMappingPrefetch, 0xXXXXXXXX, 0xXXXXXXXX},
  */
};

//
// IRQ priority
//
STATIC EFI_LEGACY_IRQ_PRIORITY_TABLE_ENTRY  IrqPriorityTable1[] = {
  11,
  0,
  10,
  0,
  9,
  0,
  5,
  0,
  0,
  0,
  0,
  0,
  0,
  0
};

//
// Note : UpdateBusNumbers updates the bus numeber
//
STATIC EFI_LEGACY_PIRQ_TABLE  PirqTableHead1 [] = {
  {
    {
      EFI_PIRQ_TABLE_SIGNATURE,
      00,
      01,
      0000,
      00,
      00,
      0000,
      V_INTEL_VID,
      30,
      00000000,
      00,
      00,
      00,
      00,
      00,
      00,
      00,
      00,
      00,
      00,
      00,
      00
    }
  }
};

//
// Instantiation of the system device data.
//
DEVICE_DATA           mDeviceDataPlatformSRP = {
  DeviceDataHwLocalInt1,   sizeof (DeviceDataHwLocalInt1) / sizeof (DeviceDataHwLocalInt1[0]),
  DeviceDataHwAddrSpace1,  sizeof (DeviceDataHwAddrSpace1)/ sizeof (DeviceDataHwAddrSpace1[0])
};

//
// Instantiation of platform PIRQ data.
//
PLATFORM_PIRQ_DATA    mPlatformPirqDataPlatformSRP = {
  IrqPriorityTable1,    sizeof(IrqPriorityTable1) / sizeof(IrqPriorityTable1[0]),
  PirqTableHead1,       sizeof(PirqTableHead1) / sizeof(PirqTableHead1[0])
};

