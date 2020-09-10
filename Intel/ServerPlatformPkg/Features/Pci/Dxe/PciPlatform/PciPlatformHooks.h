/** @file
  This code supports a the private implementation
  of the Legacy BIOS Platform protocol

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2004 - 2019 Intel Corporation. <BR>

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

#ifndef _PCI_PLATFORM_HOOKS_H_
#define _PCI_PLATFORM_HOOKS_H_

#include <Library/PreSiliconEnvDetectLib.h>
#include <Library/IoLib.h>
#include <Library/MmPciBaseLib.h>

VOID
ChipsetCallback (
  IN  EFI_HANDLE                                    RootBridgeHandle,
  IN  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_PCI_ADDRESS   PciAddress,
  IN  EFI_PCI_ENUMERATION_PHASE                     Phase,
  EFI_PCI_CALLBACK_CONTEXT                          *Context
  );

EFI_STATUS
PciTreeTraverse (
  IN  UINT8                                        Socket,
  IN  UINT8                                        Stack,
  IN  UINT8                                        StartBus
  );

#endif
