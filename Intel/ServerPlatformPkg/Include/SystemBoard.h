/** @file
  This protocol is EFI compatible.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2005 - 2017 Intel Corporation. <BR>

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

#ifndef _DXE_SYSTEM_BOARD_H_
#define _DXE_SYSTEM_BOARD_H_

#include <PlatPirqData.h>
#include <PlatDevData.h>
#include <Ppi/PchPolicy.h>

#define PCI_DEVICE_NUMBER_IMC0_CH_0     0x08
#define PCI_FUNCTION_NUMBER_IMC0_CH_0   0
#define PCI_DEVICE_ID_IMC0_CH_0         0x2014
#define BIOSGUARD_SUPPORT_ENABLED BIT0
#define OC_SUPPORT_ENABLED   BIT1

#ifndef __AML_OFFSET_TABLE_H
#define __AML_OFFSET_TABLE_H

typedef struct {
    char                   *Pathname;      /* Full pathname (from root) to the object */
    unsigned short         ParentOpcode;   /* AML opcode for the parent object */
    unsigned long          NamesegOffset;  /* Offset of last nameseg in the parent namepath */
    unsigned char          Opcode;         /* AML opcode for the data */
    unsigned long          Offset;         /* Offset for the data */
    unsigned long long     Value;          /* Original value of the data (as applicable) */
} AML_OFFSET_TABLE_ENTRY;
#endif

//
// Global variables for Option ROMs
//
#define NULL_ROM_FILE_GUID \
  { \
    0x00000000, 0x0000, 0x0000, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 \
  }

//
// {7301762E-4BF3-4b06-ADA0-766041BE36AF}
//
#define ONBOARD_VIDEO_OPTION_ROM_FILE_GUID \
  { \
    0x7301762e, 0x4bf3, 0x4b06, 0xad, 0xa0, 0x76, 0x60, 0x41, 0xbe, 0x36, 0xaf \
  }


//
// {688021EF-3A1B-468d-80D8-A888674B1B42}
//
#define ASPEED_VIDEO_OPTION_ROM_FILE_GUID \
  { \
    0x688021ef, 0x3a1b, 0x468d, 0x80, 0xd8, 0xa8, 0x88, 0x67, 0x4b, 0x1b, 0x42 \
  }

//
// {A6F63D76-6975-4144-96B3-26D624189B2B}
//
#define PXE_UNDI_OPTION_ROM_FILE_GUID \
  { \
    0xa6f63d76, 0x6975, 0x4144, 0x96, 0xb3, 0x26, 0xd6, 0x24, 0x18, 0x9b, 0x2b \
  }

#define PXE_BASE_OPTION_ROM_FILE_GUID \
  { \
    0x6f2bc426, 0x8d83, 0x4f17, 0xbe, 0x2c, 0x6b, 0x1f, 0xcb, 0xd1, 0x4c, 0x80 \
  }

//
// {7A39517D-DD10-4abb-868F-B859FBBBBA87}
//
#define IDE_RAID_OPTION_ROM_FILE_GUID \
  { \
    0x7a39517d, 0xdd10, 0x4abb, 0x86, 0x8f, 0xb8, 0x59, 0xfb, 0xbb, 0xba, 0x87 \
  }

#define BARTON_HILLS_DEVICE_ID 0x150E

#define SATA_RAID_OPTION_ROM_FILE_GUID \
  { \
    0xbd5d4ca5, 0x674f, 0x4584, 0x8c, 0xf9, 0xce, 0x4e, 0xa1, 0xf5, 0x4d, 0xd1 \
  }
#define SSATA_RAID_OPTION_ROM_FILE_GUID \
  { \
    0x2CC25173, 0xBD9F, 0x4C89, 0x89, 0xCC, 0x29, 0x25, 0x6A, 0x3F, 0xD9, 0xC3 \
  }

typedef struct {
  EFI_GUID  FileName;
  UINTN     Segment;
  UINTN     Bus;
  UINTN     Device;
  UINTN     Function;
  UINT16    VendorId;
  UINT16    DeviceId;
} PCI_OPTION_ROM_TABLE;

//
// System board information table
//
typedef struct {
  //
  // Pci option ROM data
  //
  PCI_OPTION_ROM_TABLE          *PciOptionRomTable;

  //
  // System CPU data
  //
  UINT32                        CpuSocketCount;

  //
  // System device and irq routing data
  //
  DEVICE_DATA                   *DeviceData;
  PLATFORM_PIRQ_DATA            *SystemPirqData;
} DXE_SYSTEM_BOARD_INFO;

#endif
