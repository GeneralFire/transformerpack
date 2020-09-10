/** @file
  Memory POR functions for a specifc processor

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2020 Intel Corporation. <BR>

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

#include <Base.h>
#include <Library/MemoryServicesLib.h>
#include <Library/SystemInfoLib.h>
#include <Library/PreSiliconEnvDetectLib.h>
#include <Library/KtiApi.h>
#include <SysHost.h>
#include "Include/MemPorLibInternal.h"
#include <Library/EmulationConfigurationLib.h>

extern DIMM_POR_ENTRY mDimmPorTable[];
extern UINT16 mDimmPorTableLength;

/** Get the system's socket configuration in a format that corresponds with POR table entries.

  @param[out]   PorSocketConfig   The system's socket configuration, corresponding to one of the POR table's SOCKET_*
                                  macros.

  @retval EFI_SUCCESS   The configuration was successfully determined.
  @retval !EFI_SUCCESS  The configuration was not successfully determined.

**/
EFI_STATUS
EFIAPI
GetPorSocketConfig (
  OUT UINT8 *PorSocketConfig
  )
{
  CPU_CSR_ACCESS_VAR    *CpuCsrAccessVar;
  TYPE_CHOPTYPE_ENUM    Chop;

  //
  // Get SocketConfig from platform data, which is decided by board type.
  // Each supported board should have its ScocketConfig being defined for POR.
  //
  CpuCsrAccessVar = GetSysCpuCsrAccessVar ();
  *PorSocketConfig = CpuCsrAccessVar->SocketConfig;

  //
  // For Simics/RCSIM, hardcode SocketConfig in SOCKET_UNDEFINED condition.
  //
  if ((IsSimicsEnvironment () || HybridSystemLevelEmulationEnabled ()) &&
      (*PorSocketConfig == SOCKET_UNDEFINED)) {
    Chop = GetChopType (GetCurrentSocketId ());
    if (Chop == TypeHcc) {
      *PorSocketConfig = SOCKET_1SWS;
    } else {
      *PorSocketConfig = SOCKET_4S;
    }
  }


  return EFI_SUCCESS;
}

/**
  Return pointer to and length of the memory POR table for the processor

  @param[out] MemPorTable       - Pointer to memory POR table pointer
  @param[out] TableLength       - Pointer to number of entries in the table

  @retval EFI_SUCCESS if table is found
          EFI_NOT_FOUND otherwise

**/
EFI_STATUS
GetMemoryPorTable (
  OUT DIMM_POR_ENTRY  **MemPorTable,
  OUT UINT16          *TableLength
  )
{
  *TableLength = mDimmPorTableLength;
  *MemPorTable = mDimmPorTable;

  return EFI_SUCCESS;
}
