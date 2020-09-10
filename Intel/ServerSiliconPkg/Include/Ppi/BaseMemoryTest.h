/** @file
  Base memory test PPI

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 Intel Corporation. <BR>

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

#ifndef _BASE_MEMORY_TEST_H_
#define _BASE_MEMORY_TEST_H_

#define PEI_BASE_MEMORY_TEST_GUID \
  { \
    0xb6ec423c, 0x21d2, 0x490d, {0x85, 0xc6, 0xdd, 0x58, 0x64, 0xea, 0xa6, 0x74} \
  }

typedef struct _PEI_BASE_MEMORY_TEST_PPI PEI_BASE_MEMORY_TEST_PPI;

typedef enum {
  Ignore,
  Quick,
  Sparse,
  Extensive
} PEI_MEMORY_TEST_OP;

typedef
EFI_STATUS
(EFIAPI *PEI_BASE_MEMORY_TEST) (
  IN  EFI_PEI_SERVICES                   **PeiServices,
  IN PEI_BASE_MEMORY_TEST_PPI            * This,
  IN  EFI_PHYSICAL_ADDRESS               BeginAddress,
  IN  UINT64                             MemoryLength,
  IN  PEI_MEMORY_TEST_OP                 Operation,
  OUT EFI_PHYSICAL_ADDRESS               * ErrorAddress
  );

struct _PEI_BASE_MEMORY_TEST_PPI {
  PEI_BASE_MEMORY_TEST  BaseMemoryTest;
};

extern EFI_GUID gPeiBaseMemoryTestPpiGuid;

#endif
