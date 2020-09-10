/** @file
  FruCpuFeatureSgx3v0Ppi.h

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation. <BR>

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

#ifndef _FRU_CPU_FEATURE_SGX_3V0_PPI_H_
#define _FRU_CPU_FEATURE_SGX_3V0_PPI_H_

//#include <Library/Mesh2MemIpLib.h>
//#include <KtiHost.h>
#include <Uefi/UefiBaseType.h>

///
/// PPI revision information
/// This PPI will be extended in a backwards compatible manner over time
/// Added interfaces should be documented here with the revisions added
/// Revision 1:  Initial revision
///

extern EFI_GUID gSecurityFruCpuFeatureSgx3v0PpiGuid;

#define FRU_CPU_FEATURE_SGX_3V0_PPI_REVISION  1

typedef struct _FRU_CPU_FEATURE_SGX_3V0_PPI FRU_CPU_FEATURE_SGX_3V0_PPI;

#define MAX_PRMRR_SPR_CPU 8

typedef struct {
  UINT64         PrmrrSize;
  UINT8          PrmrrCount;                  // Indicate the total number of PRMRR in a system allocated
  UINT64         PrmrrBase[MAX_PRMRR_SPR_CPU];
  UINT64         PrmrrMask;
} FRU_CPU_FEATURE_SGX_3V0_PRMRR_DATA;

typedef
EFI_STATUS
(EFIAPI *FRU_CPU_FEATURE_SGX_3V0_PROGRAM_PRMRR) (
  FRU_CPU_FEATURE_SGX_3V0_PPI *This,
  IN CONST EFI_PEI_SERVICES   **PeiServices
  );

typedef
EFI_STATUS
(EFIAPI *FRU_CPU_FEATURE_SGX_3V0_LOCK_PRMRR) (
  FRU_CPU_FEATURE_SGX_3V0_PPI *This,
  IN CONST EFI_PEI_SERVICES   **PeiServices
  );

struct _FRU_CPU_FEATURE_SGX_3V0_PPI {
  UINT32                    Revision;
  FRU_CPU_FEATURE_SGX_3V0_PRMRR_DATA    PrmrrData;
  FRU_CPU_FEATURE_SGX_3V0_PROGRAM_PRMRR ProgramPrmrr;
  FRU_CPU_FEATURE_SGX_3V0_LOCK_PRMRR    LockPrmrr;
};

#endif  //_FRU_CPU_FEATURE_SGX_3V0_PPI_
