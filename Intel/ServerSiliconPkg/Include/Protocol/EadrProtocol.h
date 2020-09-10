/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation. <BR>

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

#ifndef _EADR_PROTOCOL_H_
#define _EADR_PROTOCOL_H_

#include <Library/UefiLib.h>

#define EFI_EADR_PROTOCOL_VERSION_1   1
#define EFI_EADR_PROTOCOL_VERSION_CUR EFI_EADR_PROTOCOL_VERSION_1

/**
  Check if eADR is fired and if so execute eADR handler
**/
typedef
EFI_STATUS
(EFIAPI *EADR_CHECK_STATUS_AND_EXECUTE) (
  VOID
  );

typedef struct _EFI_EADR_PROTOCOL {
  UINT16                            ProtocolVersion;
  UINT8                             Reserved[6];

  EADR_CHECK_STATUS_AND_EXECUTE     EadrCheckStatusAndExecute;

} EFI_EADR_PROTOCOL;

extern EFI_GUID         gEadrProtocolGuid;

#endif // _EADR_PROTOCOL_H_
