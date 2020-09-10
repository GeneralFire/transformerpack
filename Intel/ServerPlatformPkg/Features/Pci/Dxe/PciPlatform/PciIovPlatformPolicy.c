/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2014 Intel Corporation. <BR>

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

#include <PiDxe.h>
#include "PciPlatform.h"
#include <Guid/SetupVariable.h>
#include <Library/SetupLib.h>

#ifdef EFI_PCI_IOV_SUPPORT

/**

    The GetSystemLowestPageSize() function retrieves the system lowest page size.

    @param This                 - Pointer to the EFI_PCI_IOV_PLATFORM_PROTOCOL instance.
    @param SystemLowestPageSize - The system lowest page size. (This system supports a
                                  page size of 2^(n+12) if bit n is set.)

    @retval EFI_SUCCESS           - The function completed successfully.
    @retval EFI_INVALID_PARAMETER - SystemLowestPageSize is NULL.

**/
EFI_STATUS
EFIAPI
GetSystemLowestPageSize (
  IN  EFI_PCI_IOV_PLATFORM_PROTOCOL           *This,
  OUT UINT32                                  *SystemLowestPageSize
)
{
  EFI_STATUS                            Status;
  UINT8                                 SystemPageSizeOption;

  Status = GetOptionData (&gEfiSetupVariableGuid,  OFFSET_OF (SYSTEM_CONFIGURATION, SystemPageSize), &SystemPageSizeOption, sizeof (UINT8));
  ASSERT_EFI_ERROR (Status);

  if (SystemLowestPageSize != NULL) {
    //
    // Convert page size option to page size
    // Option is n in 2^n
    // Page size is number of 4KiB pages
    //

    *SystemLowestPageSize = (UINT32) (1 << SystemPageSizeOption);
  }
  return EFI_SUCCESS;
}

/**

    The GetIovPlatformPolicy() function retrieves the platform policy regarding PCI IOV.

    @param This         - Pointer to the EFI_PCI_IOV_PLATFORM_PROTOCOL instance.
    @param PciIovPolicy - The platform policy for PCI IOV configuration.

    @retval EFI_SUCCESS           - The function completed successfully.
    @retval EFI_INVALID_PARAMETER - PciPolicy is NULL.

**/
EFI_STATUS
EFIAPI
GetIovPlatformPolicy (
  IN  EFI_PCI_IOV_PLATFORM_PROTOCOL           *This,
  OUT EFI_PCI_IOV_PLATFORM_POLICY             *PciIovPolicy
)
{
  EFI_STATUS                            Status;
  UINT8                                 PolicyEnable;
  UINT8                                 ARIEnable;
  UINT8                                 SRIOVEnable;
  UINT8                                 MRIOVEnable;

  PolicyEnable = 0;

  Status = GetOptionData (&gEfiSetupVariableGuid,  OFFSET_OF(SYSTEM_CONFIGURATION, ARIEnable), &ARIEnable, sizeof(UINT8));
  ASSERT_EFI_ERROR (Status);
  Status = GetOptionData (&gEfiSetupVariableGuid,  OFFSET_OF(SYSTEM_CONFIGURATION, SRIOVEnable), &SRIOVEnable, sizeof(UINT8));
  ASSERT_EFI_ERROR (Status);
  Status = GetOptionData (&gEfiSetupVariableGuid,  OFFSET_OF(SYSTEM_CONFIGURATION, MRIOVEnable), &MRIOVEnable, sizeof(UINT8));
  ASSERT_EFI_ERROR (Status);

  if (ARIEnable == TRUE) {
    PolicyEnable = PolicyEnable | EFI_PCI_IOV_POLICY_ARI;
  }

  if (SRIOVEnable == TRUE) {
    PolicyEnable = PolicyEnable | EFI_PCI_IOV_POLICY_SRIOV;
  }

  if (MRIOVEnable == TRUE) {
    PolicyEnable = PolicyEnable | EFI_PCI_IOV_POLICY_MRIOV;
  }

  if (PciIovPolicy != NULL) {
    //*PciIovPolicy = EFI_PCI_IOV_POLICY_ARI | EFI_PCI_IOV_POLICY_SRIOV;
    *PciIovPolicy = PolicyEnable;
  }
  return EFI_SUCCESS;
}

#endif

