/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1999 - 2012 Intel Corporation. <BR>

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

#ifndef _CPU_RAS_PROTOCOL_H_
#define _CPU_RAS_PROTOCOL_H_


//
// Forward reference for pure ANSI compatability
//

#define EFI_CPU_RAS_PROTOCOL_GUID \
   { \
      0xf084ff45, 0xf9fa, 0x4e9e, 0x8d, 0xff, 0xe2, 0xd7, 0x80, 0xd2, 0x2c, 0xc2 \
   }


typedef
EFI_STATUS
(EFIAPI *EFI_CPU_RAS_ONLINE) (
);

typedef
EFI_STATUS
(EFIAPI *EFI_CPU_RAS_OFFLINE) (
);


typedef struct _EFI_CPU_RAS_PROTOCOL {
  UINT8                           FirstCpuRasEntry;
  UINT8                           SocketToBeOl;
  UINT8                           RASEvent;
  EFI_CPU_RAS_ONLINE              CpuRasOnline;
  EFI_CPU_RAS_OFFLINE             CpuRasOffline;
} EFI_CPU_RAS_PROTOCOL;

extern EFI_GUID gEfiCpuRasProtocolGuid;


#endif
