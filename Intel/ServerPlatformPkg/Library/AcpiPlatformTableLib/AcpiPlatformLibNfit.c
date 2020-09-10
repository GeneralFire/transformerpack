/** @file
  ACPI Platform Driver Hooks

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1996 - 2019 Intel Corporation. <BR>

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

//
// Statements that include other files
//
#include "AcpiPlatformLibLocal.h"

/**

    This function locates the CrystalRidge protocol and JedecNvdimm protocol
    and calls the update ACPI tables functions defined there to update/build
    the NVDIMM F/W Interface Table (NFIT). It builds the NFIT table which gets
    published in ACPI XSDT.

    @param[in,out] Table      Pointer to NFIT which will be build in
                              CR Protocol and will be publised in ACPI XSDT.

    @retval EFI_SUCCESS       Table successfully updated.
    @retval EFI_UNSUPPORTED   Table not updated.

**/
EFI_STATUS
UpdateNfitTable(
  IN OUT   EFI_ACPI_COMMON_HEADER  *Table
  )
{
  EFI_NFIT_TABLE_UPDATE_PROTOCOL    *NfitTableUpdateProtocol = NULL;
  EFI_STATUS                        Status;

  Status = gBS->LocateProtocol (&gEfiNfitTableUpdateProtocolGuid, NULL, &NfitTableUpdateProtocol);

  if (!EFI_ERROR (Status)) {
    Status = NfitTableUpdateProtocol->UpdateAcpiTable ((UINT64*) Table);
  }
  else{
    DEBUG((DEBUG_ERROR, "Cannot find NfitTableUpdateProtocol\n"));
  }
  DEBUG((DEBUG_INFO, "NFIT Update Status: 0x%x\n", Status));

  return Status;
}
