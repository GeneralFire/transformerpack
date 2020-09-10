/** @file
  Memory Address Translation Protocol header file.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 Intel Corporation. <BR>

  The source code contained or described herein and all documents related to the
  source code ("Material") are owned by Intel Corporation or its suppliers or
  licensors. Title to the Material remains with Intel Corporation or its suppliers
  and licensors. The Material may contain trade secrets and proprietary and
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

#ifndef _MEMORY_ADDRESS_TRANSLATION_PROTOCOL_H_
#define _MEMORY_ADDRESS_TRANSLATION_PROTOCOL_H_

#include <Library/AddressDecodeLib.h>

#define MEMORY_ADDRESS_TRANSLATION_PROTOCOL_CURRENT_REVISION  1

/**
  Translate a system address to a DIMM address.

  @param[in]      SystemAddress       System address to be translated
  @param[out]     TranslatedAddress   Pointer to the DIMM Address structure

  @retval EFI_SUCCESS                 Translation was successful
  @retval EFI_INVALID_PARAMETER       System address was invalid
**/
typedef
EFI_STATUS
(EFIAPI *MEMORY_ADDRESS_TRANSLATION_FORWARD) (
  IN      UINTN                 SystemAddress,
  OUT     PTRANSLATED_ADDRESS   TranslatedAddress
  );

/**
  Translate a DIMM address to system address.

  @param[in,out]  TranslatedAddress   Pointer to the DIMM Address structure

  @retval EFI_SUCCESS                 Translation was successful
  @retval EFI_INVALID_PARAMETER       DIMM address was null or invalid
**/
typedef
EFI_STATUS
(EFIAPI *MEMORY_ADDRESS_TRANSLATION_REVERSE) (
  IN OUT  PTRANSLATED_ADDRESS   TranslatedAddress
  );

typedef struct {
  UINT64                              Revision;
  MEMORY_ADDRESS_TRANSLATION_FORWARD  SystemAddressToDimmAddress;
  MEMORY_ADDRESS_TRANSLATION_REVERSE  DimmAddressToSystemAddress;
} MEMORY_ADDRESS_TRANSLATION_PROTOCOL;

extern EFI_GUID gMemoryAddressTranslationProtocolGuid;

#endif // _MEMORY_ADDRESS_ENCODE_DECODE_PROTOCOL_H_
