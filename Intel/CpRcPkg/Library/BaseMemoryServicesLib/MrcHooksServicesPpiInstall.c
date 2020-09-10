/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2011 - 2020 Intel Corporation. <BR>

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

#include <Uefi.h>
#include "SysHost.h"
#include <Library/DebugLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/MemoryAllocationLib.h>
#include "Ppi/MrcHooksServicesPpi.h"
#include <Library/ProcMemInitChipLib.h>
#include <Library/IoAccessLib.h>
#include <Library/CheckpointLib.h>
#include <Library/MemoryCoreLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/ProcSmbIpLib.h>
#include <Library/SpdAccessLib.h>

//
// Fill in for removed functions in the PPI with dummy so we don't expose NULL
// We don't change the PPI - must maintain backward compatibility
//

UINT32
DummyFunction (
  PSYSHOST Host
  )
{
  return 0; // Do nothing
}

INT32
DummyFunction2 (
  PSYSHOST    Host,
  CONST CHAR8 *Format,
  VA_LIST     Marker
  )
{
  return 0; // Do nothing
}


static MRC_HOOKS_SERVICES_PPI mMrcHooksServicesPpi = {
  MRC_HOOKS_SERVICES_PPI_REVISION,
  DummyFunction,                       // INITIALIZE_USB2_DEBUG_PORT
  DummyFunction,                       // DISCOVER_USB2_DEBUG_PORT_API
  DummyFunction2,                      // RC_VPRINTF
  WriteMsrPipeHL,                      // WRITE_MSR_PIPE
  SetPromoteWarningException,          // SET_PROMOTE_WARNING_EXCEPTION
  ClearPromoteWarningException,        // CLEAR_PROMOTE_WARNING_EXCEPTION  
  ReadProcSmb,                         // READ_SMB
  WriteProcSmb,                        // WRITE_SMB
  OutputCheckpoint,                    // OUTPUT_CHECKPOINT
  LogWarning,                          // LOG_WARNING
  OutputWarning,                       // OUTPUT_WARNING
  SpdReadByte,                         // READ_SPD
};

static EFI_PEI_PPI_DESCRIPTOR mInstallMrcHooksServicesPpi = {
  EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
  &gMrcHooksServicesPpiGuid,
  &mMrcHooksServicesPpi
};

//
// Below are the MrcHooksServicesPpi installation
//
EFI_STATUS
EFIAPI
InstallMrcHooksServicesPpi (
  PSYSHOST Host
  )
{
  EFI_STATUS                       Status = EFI_SUCCESS;

#ifdef IA32

  EFI_PEI_SERVICES                 **PeiServices;

  PeiServices  = (EFI_PEI_SERVICES**) (UINTN) Host->var.common.oemVariable;

  //
  // Initialize MrcHooksServices PPI
  //
  Status = PeiServicesInstallPpi (&mInstallMrcHooksServicesPpi);
  ASSERT_EFI_ERROR (Status);
  RcDebugPrint (SDBG_DEFAULT, "InstallPpi MrcHooksServicesPpiDesc Status = %08x\n", Status);
  if (EFI_ERROR (Status)) {
    Host->MrcHooksServicesPpi = 0;
    Status = EFI_NOT_READY;
  } else {
    Host->MrcHooksServicesPpi = (UINT32)&mMrcHooksServicesPpi;
    RcDebugPrint (SDBG_DEFAULT, "CacheMrcHooksServicesPpi in HOST: Host->MrcHooksServicesPpi = %08x\n", Host->MrcHooksServicesPpi);
    Status = EFI_SUCCESS;
  }

#endif // #ifdef IA32

  return Status;

}

