/** @file
  GUID used for VIRTUAL_LOCKSTEP_VARIABLE.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1999 - 2016 Intel Corporation. <BR>

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

#ifndef _VIRTUAL_LOCKSTEP_GUID_H_
#define _VIRTUAL_LOCKSTEP_GUID_H_

#define VIRTUAL_LOCKSTEP_VARIABLE_GUID { 0x9189541f, 0xac0c, 0x4368, { 0x90, 0x62, 0x70, 0xe1, 0x95, 0x7c, 0x34, 0x45 } }

#define VIRTUAL_LOCKSTEP_VARIABLE_NAME L"VLSEstablished"

#pragma pack(push,1)
typedef struct{
//
// VLS Happened in previous boot
//
// If there was a device failure that lead to a sparing operation and VLS
// then the next boot must be forced to a normal boot
// Save the information if VLS was established in a Boolean variable.
//

  BOOLEAN         VirtualLockstepEstablished;
} VIRTUAL_LOCKSTEP_VARIABLE_DATA;

extern EFI_GUID gEfiVirtualLockstepGuid;

#pragma pack(pop)
#endif
