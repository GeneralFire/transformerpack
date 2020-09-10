/** @file
  Platform Memory Size PPI as defined in Tiano
  PPI for describing the minimum platform memory size in order to successfully
  pass control into DXE

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1999 - 2003 Intel Corporation. <BR>

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

#ifndef _PEI_PLATFORM_MEMORY_SIZE_H_
#define _PEI_PLATFORM_MEMORY_SIZE_H_

#define PEI_PLATFORM_MEMORY_SIZE_PPI_GUID \
  { \
    0x9a7ef41e, 0xc140, 0x4bd1, {0xb8, 0x84, 0x1e, 0x11, 0x24, 0xb, 0x4c, 0xe6 } \
  }

typedef struct _PEI_PLATFORM_MEMORY_SIZE_PPI PEI_PLATFORM_MEMORY_SIZE_PPI;

typedef
EFI_STATUS
(EFIAPI *PEI_GET_MINIMUM_PLATFORM_MEMORY_SIZE) (
  IN      EFI_PEI_SERVICES                       **PeiServices,
  IN PEI_PLATFORM_MEMORY_SIZE_PPI                * This,
  IN OUT  UINT64                                 *MemorySize
  );

typedef struct _PEI_PLATFORM_MEMORY_SIZE_PPI {
  PEI_GET_MINIMUM_PLATFORM_MEMORY_SIZE  GetPlatformMemorySize;
} PEI_PLATFORM_MEMORY_SIZE_PPI;

extern EFI_GUID gPeiPlatformMemorySizePpiGuid;

#endif
