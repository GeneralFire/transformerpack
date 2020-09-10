/** @file
  FVB Extension protocol that extends the FVB Class in a component fashion.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2004 Intel Corporation. <BR>

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

#ifndef _FVB_EXTENSION_H_
#define _FVB_EXTENSION_H_

 typedef struct _EFI_FVB_EXTENSION_PROTOCOL EFI_FVB_EXTENSION_PROTOCOL;

//  FVB Extension Function Prototypes
//
typedef
EFI_STATUS
(EFIAPI * EFI_FV_ERASE_CUSTOM_BLOCK) (
  IN EFI_FVB_EXTENSION_PROTOCOL   *This,
  IN EFI_LBA                              StartLba,
  IN UINTN                                OffsetStartLba,
  IN EFI_LBA                              LastLba,
  IN UINTN                                OffsetLastLba
);

//
// IPMI TRANSPORT PROTOCOL
//
 struct _EFI_FVB_EXTENSION_PROTOCOL {
  EFI_FV_ERASE_CUSTOM_BLOCK               EraseFvbCustomBlock;
 };

//

extern EFI_GUID                           gEfiFvbExtensionProtocolGuid;

#endif

