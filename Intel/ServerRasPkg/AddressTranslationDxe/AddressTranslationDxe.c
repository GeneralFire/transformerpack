/** @file
  Address Translation DXE driver.

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

#include <PiDxe.h>
#include <Library/AddressDecodeLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Protocol/MemoryAddressTranslation.h>

MEMORY_ADDRESS_TRANSLATION_PROTOCOL mMemoryAddressTranslationProtocol = {
  MEMORY_ADDRESS_TRANSLATION_PROTOCOL_CURRENT_REVISION,
  SystemAddressToDimmAddress,
  DimmAddressToSystemAddress
};

/**
  The entry point function.

  @param[in]  ImageHandle   Handle to the image of this driver.
  @param[in]  SystemTable   A pointer to the EFI System Table.

  @retval EFI_SUCCESS       The entry point was executed successfully.
**/
EFI_STATUS
EFIAPI
AddressTranslationDxeEntryPoint (
  IN EFI_HANDLE             ImageHandle,
  IN EFI_SYSTEM_TABLE       *SystemTable
  )
{
  return gBS->InstallMultipleProtocolInterfaces (
    &ImageHandle,
    &gMemoryAddressTranslationProtocolGuid,
    &mMemoryAddressTranslationProtocol,
    NULL
    );
}
