/** @file
  SMM driver for SW SMI handling of address translation DSM.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 - 2020 Intel Corporation. <BR>

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

#ifndef _ADDRESS_TRANSLATION_DSM_H_
#define _ADDRESS_TRANSLATION_DSM_H_

///
/// AML format for a dword constant.
///
#pragma pack(push, 1)
typedef struct {
  UINT8   DWordPrefix;
  UINT32  DWordData;
} ADDRESS_TRANSLATION_DWORD_CONST;
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
  ADDRESS_TRANSLATION_DWORD_CONST RegionOffset;
  ADDRESS_TRANSLATION_DWORD_CONST RegionLen;
} ADDRESS_TRANSLATION_OP_REGION;
#pragma pack(pop)

///
/// Communication buffer between AML and SMM.
///
#pragma pack(push, 1)
typedef struct {
  UINT32  SwSmi;
  UINT32  Command;
  UINT32  Status;

  UINT64  SystemAddress;
  UINT64  NmSystemAddress;
  UINT64  SpareSystemAddress;
  UINT64  DevicePhysicalAddress;
  UINT64  ProcessorSocketId;
  UINT64  MemoryControllerId;
  UINT64  NmMemoryControllerId;
  UINT64  TargetId;
  UINT64  LogicalChannelId;
  UINT64  ChannelAddress;
  UINT64  NmChannelAddress;
  UINT64  ChannelId;
  UINT64  NmChannelId;
  UINT64  RankAddress;
  UINT64  NmRankAddress;
  UINT64  PhysicalRankId;
  UINT64  NmPhysicalRankId;
  UINT64  DimmSlotId;
  UINT64  NmDimmSlotId;
  UINT64  DimmRankId;
  UINT64  Row;
  UINT64  NmRow;
  UINT64  Column;
  UINT64  NmCol;
  UINT64  Bank;
  UINT64  NmBank;
  UINT64  BankGroup;
  UINT64  NmBankGroup;
  UINT64  LockStepRank;
  UINT64  LockStepPhysicalRank;
  UINT64  LockStepBank;
  UINT64  LockStepBankGroup;
  UINT64  ChipSelect;
  UINT64  NmChipSelect;
  UINT64  Node;
  UINT64  ChipId;
  UINT64  NmChipId;
} ADDRESS_TRANSLATION_BUFFER;
#pragma pack(pop)

//
// DSM commands
//
#define ADDRESS_TRANSLATION_GET_ADDRESS_PARAMETERS    1
#define ADDRESS_TRANSLATION_FORWARD_ADDRESS_TRANSLATE 2
#define ADDRESS_TRANSLATION_REVERSE_ADDRESS_TRANSLATE 3

//
// Return values
//
#define ADDRESS_TRANSLATION_SUCCESS         0
#define ADDRESS_TRANSLATION_UNKNOWN_FAILURE 1
#define ADDRESS_TRANSLATION_INVALID_COMMAND 2
#define ADDRESS_TRANSLATION_INTERNAL_ERROR  3

//
// Extend a parameter from a given type (UINT8, UINT16, or UINT32) to UINT64.
// If the parameter is equal to the maximum for its type, e.g. (UINT8) -1,
// change it to (UINT64) -1. Otherwise, pass the value unchanged.
//
#define EXTEND_PARAM(t, p)  (((p) == (t) -1) ? (UINT64) -1 : (UINT64) (p))

#endif // #ifndef _ADDRESS_TRANSLATION_DSM_H_
