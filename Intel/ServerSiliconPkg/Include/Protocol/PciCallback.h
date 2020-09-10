/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1999 - 2017 Intel Corporation. <BR>

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

#ifndef _EFI_PCI_CALLBACK_H
#define _EFI_PCI_CALLBACK_H

#include <Include/IndustryStandard/Pci22.h>
#include <Protocol/CpuIo2.h>
#include <Protocol/PciRootBridgeIo.h>


//
// Global Id for PCI callback
//
#define EFI_PCI_CALLBACK_PROTOCOL_GUID \
  { \
    0x1ca0e202, 0xfe9e, 0x4776, 0x9f, 0xaa, 0x57, 0xc, 0x19, 0x61, 0x7a, 0x06 \
  }

typedef struct _EFI_PCI_CALLBACK_PROTOCOL EFI_PCI_CALLBACK_PROTOCOL;

typedef enum {
  EfiPciEnumerationDeviceScanning    = 1,
  EfiPciEnumerationBusNumberAssigned = 2,
  EfiPciEnumerationResourceAssigned  = 4,
} EFI_PCI_ENUMERATION_PHASE;

typedef struct {
  PCI_TYPE00                              PciHeader;
  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL        *PciRootBridgeIo;
  EFI_CPU_IO2_PROTOCOL                    *CpuIo;
} EFI_PCI_CALLBACK_CONTEXT;

typedef
VOID
(EFIAPI *EFI_PCI_CALLBACK_FUNC) (
  IN  EFI_HANDLE                                   RootBridgeHandle,
  IN  EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL_PCI_ADDRESS  PciAddress,
  IN  EFI_PCI_ENUMERATION_PHASE                    Phase,
  IN  EFI_PCI_CALLBACK_CONTEXT                     *Context
);

typedef
EFI_STATUS
(EFIAPI *EFI_REGISTER_PCI_CALLBACK) (
  IN  EFI_PCI_CALLBACK_PROTOCOL                   *This,
  IN  EFI_PCI_CALLBACK_FUNC                        Function,
  IN  EFI_PCI_ENUMERATION_PHASE                    Phase
)
/*++

Routine Description:

  Register a callback during PCI bus enumeration

Arguments:

  This       -  Protocol instance pointer.
  Function   -  Callback function pointer.
  Phase      -  PCI enumeration phase.

Returns:

  EFI_SUCCESS           - Function has registed successfully
  EFI_UNSUPPORTED       - The function has been regisered
  EFI_InVALID_PARAMETER - The parameter is incorrect

--*/
;

//
// Protocol definition
//
typedef struct _EFI_PCI_CALLBACK_PROTOCOL {
  EFI_REGISTER_PCI_CALLBACK   RegisterPciCallback;
} EFI_PCI_CALLBACK_PROTOCOL;

extern EFI_GUID gEfiPciCallbackProtocolGuid;

#endif
