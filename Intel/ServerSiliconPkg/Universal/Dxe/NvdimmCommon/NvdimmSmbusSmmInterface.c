/** @file NvdimmSmbusSmmInterface.c
  Implementation of NVDIMM SMBUS SMM Interface

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2020 Intel Corporation. <BR>

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
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DxeServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Protocol/NvdimmSmbusSmmProtocol.h>
#include "NvdimmSmbusSmmInterface.h"

NVDIMM_SMBUS_SMM_INTERFACE               *mNvdimmSmbusSmmInterface = NULL;
EFI_NVDIMM_SMBUS_SMM_INTERFACE_PROTOCOL  mNvdimmSmbusSmmInterfaceProtocol;

/**
  Installs NVIDMM SMBUS SMM interface protocol

  @param[in] Handle   Handle to install the protocol on.

  @retval EFI_SUCCESS NVIDMM SMBUS SMM interface protocol installed successfully.

**/
EFI_STATUS
InstallNvdimmSmbusSmmInterfaceProtocol (
  IN EFI_HANDLE  *Handle
  )
{
  EFI_STATUS            Status;
  EFI_PHYSICAL_ADDRESS  NvdimmSmbusSmmInterface = 0xFFFFFFFF;

  Status = gBS->AllocatePages(
    AllocateMaxAddress,
    EfiACPIMemoryNVS,
    EFI_SIZE_TO_PAGES(sizeof(NVDIMM_SMBUS_SMM_INTERFACE)),
    &NvdimmSmbusSmmInterface
    );

  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "[PMem] ERROR: SMBUS NONSMM<->SMM memory allocation failed with status (%r)\n", Status));
    return Status;
  }

  DEBUG ((DEBUG_INFO, "[PMem] SMBUS NONSMM<->SMM interface address: 0x%llx\n", NvdimmSmbusSmmInterface));
  mNvdimmSmbusSmmInterface = (NVDIMM_SMBUS_SMM_INTERFACE *)NvdimmSmbusSmmInterface;
  ZeroMem (mNvdimmSmbusSmmInterface, sizeof(NVDIMM_SMBUS_SMM_INTERFACE));

  ZeroMem (&mNvdimmSmbusSmmInterfaceProtocol, sizeof(EFI_NVDIMM_SMBUS_SMM_INTERFACE_PROTOCOL));
  mNvdimmSmbusSmmInterfaceProtocol.Revision = EFI_NVDIMM_SMBUS_SMM_INTERFACE_VERSION_1_0;
  mNvdimmSmbusSmmInterfaceProtocol.Area = mNvdimmSmbusSmmInterface;

  Status = gBS->InstallProtocolInterface(
    Handle,
    &gEfiNvdimmSmbusSmmInterfaceProtocolGuid,
    EFI_NATIVE_INTERFACE,
    &mNvdimmSmbusSmmInterfaceProtocol
    );

  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "[PMem] ERROR: SMBUS NONSMM<->SMM interface protocol installation failed with status (%r)\n", Status));
  }
  return Status;
}
