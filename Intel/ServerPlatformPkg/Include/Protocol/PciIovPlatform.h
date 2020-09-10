/** @file
  This file declares PCI IOV platform protocols.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2005 - 2009 Intel Corporation. <BR>

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

#ifndef _PCI_IOV_PLATFORM_H_
#define _PCI_IOV_PLATFORM_H_


//
// Protocol for GUID.
//

typedef struct _EFI_PCI_IOV_PLATFORM_PROTOCOL EFI_PCI_IOV_PLATFORM_PROTOCOL;

typedef    UINT32   EFI_PCI_IOV_PLATFORM_POLICY;

#define     EFI_PCI_IOV_POLICY_ARI           0x0001
#define     EFI_PCI_IOV_POLICY_SRIOV         0x0002
#define     EFI_PCI_IOV_POLICY_MRIOV         0x0004

typedef
EFI_STATUS
(EFIAPI * EFI_PCI_IOV_PLATFORM_GET_SYSTEM_LOWEST_PAGE_SIZE) (
  IN  EFI_PCI_IOV_PLATFORM_PROTOCOL           *This,
  OUT UINT32                                  *SystemLowestPageSize
)
/**

    The GetSystemLowestPageSize() function retrieves the system lowest page size.

    @param This                 - Pointer to the EFI_PCI_IOV_PLATFORM_PROTOCOL instance.
    @param SystemLowestPageSize - The system lowest page size. (This system supports a
                                  page size of 2^(n+12) if bit n is set.)

    @retval EFI_SUCCESS           - The function completed successfully.
    @retval EFI_INVALID_PARAMETER - SystemLowestPageSize is NULL.

**/
;

typedef
EFI_STATUS
(EFIAPI * EFI_PCI_IOV_PLATFORM_GET_PLATFORM_POLICY) (
  IN  EFI_PCI_IOV_PLATFORM_PROTOCOL           *This,
  OUT EFI_PCI_IOV_PLATFORM_POLICY             *PciIovPolicy
)
/**

    The GetPlatformPolicy() function retrieves the platform policy regarding PCI IOV.

    @param This         - Pointer to the EFI_PCI_IOV_PLATFORM_PROTOCOL instance.
    @param PciIovPolicy - The platform policy for PCI IOV configuration.

    @retval EFI_SUCCESS           - The function completed successfully.
    @retval EFI_INVALID_PARAMETER - PciPolicy is NULL.

**/
;

typedef struct _EFI_PCI_IOV_PLATFORM_PROTOCOL {
  EFI_PCI_IOV_PLATFORM_GET_SYSTEM_LOWEST_PAGE_SIZE          GetSystemLowestPageSize;
  EFI_PCI_IOV_PLATFORM_GET_PLATFORM_POLICY                  GetPlatformPolicy;
} EFI_PCI_IOV_PLATFORM_PROTOCOL;

extern EFI_GUID   gEfiPciIovPlatformProtocolGuid;

#endif
