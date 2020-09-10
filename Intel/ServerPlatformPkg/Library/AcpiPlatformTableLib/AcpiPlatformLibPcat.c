/** @file
  ACPI Platform Driver Hooks

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1996 - 2016 Intel Corporation. <BR>

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

EFI_CRYSTAL_RIDGE_PROTOCOL          *mCrystalRidgeProtocol;
#define MAX_PCAT_SIZE               0x200

/**

    This function  locates the CrystalRidge Protocol and calls
    into one of its interface function (UpdateAcpiPcatTable) to
    update/build the PCAT (Platform Capability Attribute Table).
    And this table gets published in Acpi Xsdt.

    @param *Table       - Pointer to Pcat table which will be
           build in CR Protocol and will be publised in Acpi
           Xsdt.

    @retval Status      - Return Status


**/
EFI_STATUS
UpdatePcatTable(
   IN OUT   EFI_ACPI_COMMON_HEADER  *Table
   )
{
  EFI_STATUS    Status;

  if (mCrystalRidgeProtocol == NULL) {
    Status = gBS->LocateProtocol(&gEfiCrystalRidgeGuid, NULL, &mCrystalRidgeProtocol);
    ASSERT_EFI_ERROR(Status);
  }

  mCrystalRidgeProtocol->UpdateNvmAcpiPcatTable ((UINT64 *)Table);

  return EFI_SUCCESS;
}
