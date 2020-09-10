/** @file
  This PPI manages the legacy memory regions between 0xc0000 - 0xfffff

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2007 - 0000 Intel Corporation. <BR>

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

#ifndef _EFI_LEGACY_REGION_PPI_H_
#define _EFI_LEGACY_REGION_PPI_H_

#define PEI_LEGACY_REGION_PPI_GUID \
  { \
    0x1fae70ac, 0x8203, 0x48f0, 0xb9, 0xef, 0xe8, 0x73, 0xff, 0xec, 0xc1, 0xf2 \
  }

typedef struct _PEI_LEGACY_REGION_PPI PEI_LEGACY_REGION_PPI;

typedef
EFI_STATUS
(EFIAPI *PEI_LEGACY_REGION_DECODE) (
  IN  EFI_PEI_SERVICES                    **PeiServices,
  IN  PEI_LEGACY_REGION_PPI               * This,
  IN  UINT32                              Start,
  IN  UINT32                              Length,
  IN  BOOLEAN                             *On
  );

typedef
EFI_STATUS
(EFIAPI *PEI_LEGACY_REGION_LOCK) (
  IN  EFI_PEI_SERVICES                    **PeiServices,
  IN  PEI_LEGACY_REGION_PPI               * This,
  IN  UINT32                              Start,
  IN  UINT32                              Length,
  OUT UINT32                              *Granularity OPTIONAL
  );

typedef
EFI_STATUS
(EFIAPI *PEI_LEGACY_REGION_BOOT_LOCK) (
  IN  EFI_PEI_SERVICES                    **PeiServices,
  IN  PEI_LEGACY_REGION_PPI               * This,
  IN  UINT32                              Start,
  IN  UINT32                              Length,
  OUT UINT32                              *Granularity OPTIONAL
  );

typedef
EFI_STATUS
(EFIAPI *PEI_LEGACY_REGION_UNLOCK) (
  IN  EFI_PEI_SERVICES                    **PeiServices,
  IN  PEI_LEGACY_REGION_PPI               * This,
  IN  UINT32                              Start,
  IN  UINT32                              Length,
  OUT UINT32                              *Granularity OPTIONAL
  );

typedef struct _PEI_LEGACY_REGION_PPI {
  PEI_LEGACY_REGION_DECODE    Decode;
  PEI_LEGACY_REGION_LOCK      Lock;
  PEI_LEGACY_REGION_BOOT_LOCK BootLock;
  PEI_LEGACY_REGION_UNLOCK    UnLock;
} PEI_LEGACY_REGION_PPI;

extern EFI_GUID gPeiLegacyRegionPpiGuid;

#endif
