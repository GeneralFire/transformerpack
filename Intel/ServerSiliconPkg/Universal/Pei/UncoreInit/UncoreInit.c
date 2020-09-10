/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2005 - 2020 Intel Corporation. <BR>

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

#include "Uefi.h"
#include <Library/ProcMemInitLib.h>
#include <Library/SysHostPointerLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/RcMemAllocLib.h>
#include <Library/MemoryCoreLib.h>
#include <Library/HbmCoreLib.h>
#include <Library/SsaInitializeLib.h>

/**

  Driver entry point

  @param FfsHeader -  Not used.
  @param PeiServices - General purpose services available to every PEIM.

  @retval EFI_SUCCESS:  Memory initialization completed successfully.
  @retval All other error conditions encountered result in an ASSERT.

**/
EFI_STATUS
EFIAPI
PeimProcMemInit (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  PSYSHOST Host;

  Host = RcAllocatePool (sizeof (SYSHOST));

  SetSysHostPointer (Host);

  //
  // Initialize data structures
  //
  ZeroMem ((VOID *) Host, sizeof (SYSHOST));

  //
  // HBM allocation services
  //
  HbmHostAllocation ();

  //
  // Initialize the size of SysHost for later comparision
  // in DXE to avoid mismatched structure sizes. Do this
  // after restore of nvram to ensure fresh data.
  //

  PcdSet32S (PcdPeiSyshostMemorySize, sizeof (SYSHOST));

  Host->var.common.oemVariable = (UINT32) PeiServices;

  ProcMemInitMain (Host);

  return EFI_SUCCESS;
}

/**

  Driver entry point

  @param FfsHeader -  Not used.
  @param PeiServices - General purpose services available to every PEIM.

  @retval EFI_SUCCESS:  Memory initialization completed successfully.
  @retval All other error conditions encountered result in an ASSERT.

**/
EFI_STATUS
EFIAPI
UncoreInit (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  return PeimProcMemInit (FileHandle, PeiServices);
}
