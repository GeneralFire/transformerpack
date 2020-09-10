/** @file
   Headfile for PCIe handshake between OS and firmware.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 - 2019 Intel Corporation. <BR>

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

#ifndef _RAS_ACPI_INT_H_
#define _RAS_ACPI_INT_H_
///
/// AML format for a dword constant.
///
#pragma pack(push, 1)
typedef struct {
  UINT8   DWordPrefix;
  UINT32  DWordData;
} OP_REGION_DWORD_CONST;
#pragma pack(pop)

///
/// AML format for an OpRegion.
///
/// From the APCI Spec:
///
/// DefOpRegion := OpRegionOp NameString RegionSpace RegionOffset RegionLen
/// OpRegionOp := ExtOpPrefix 0x80
/// RegionSpace := ByteData // 0x00 SystemMemory
///                         // 0x01 SystemIO
///                         // 0x02 PCI_Config
///                         // 0x03 EmbeddedControl
///                         // 0x04 SMBus
///                         // 0x05 SystemCMOS
///                         // 0x06 PciBarTarget
///                         // 0x07 IPMI
///                         // 0x08 GeneralPurposeIO
///                         // 0x09 GenericSerialBus
///                         // 0x80-0xFF: OEM Defined
/// RegionOffset := TermArg => Integer
/// RegionLen := TermArg => Integer
///
/// Note that this structure assumes that RegionOffset and RegionLen are dword
/// constants. This assumption is correct given that the ASL declared them
/// using dword-sized literals, such as 0xA5A5A5A5.
///
#pragma pack(push, 1)
typedef struct {
  UINT8   ExtOpPrefix;
  UINT8   ExtOpCode;
  UINT32  NameString;
  UINT8   RegionSpace;
  OP_REGION_DWORD_CONST RegionOffset;
  OP_REGION_DWORD_CONST RegionLen;
} ADDRESS_TRANSLATION_OP_REGION;
#pragma pack(pop)

#endif // #ifndef _RAS_ACPI_INT_H_
