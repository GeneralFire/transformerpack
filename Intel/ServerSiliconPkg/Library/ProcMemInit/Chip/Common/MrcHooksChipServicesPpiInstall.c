/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2011 - 2018 Intel Corporation. <BR>

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
#include <SysHost.h>
#include <Library/UncoreLib.h>
#include <Library/DebugLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Ppi/MrcHooksChipServicesPpi.h>
#include <Library/Mesh2MemIpLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/CsrAccessLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/UsraCsrLib.h>

//
// This function is a dummy for an obsolete PPI member.
//

VOID
EFIAPI
DummyPrintFunction (
  SYSHOST    *Host,
  UINT32     DbgLevel,
  UINT8      Socket,
  UINT8      Channel,
  UINT8      Dimm,
  UINT8      Rank,
  UINT8      Strobe,
  UINT8      Bit,
  CHAR8      *Format,
  CHAR8      *Marker
  )
{
  // Do nothing
}

STATIC MRC_HOOKS_CHIP_SERVICES_PPI mMrcHooksChipServicesPpi = {
  MRC_HOOKS_CHIP_SERVICES_PPI_REVISION,
  UsraCsrRead,                         // USRA_CSR_READ
  UsraCsrWrite,                        // USRA_CSR_WRITE
  SendMailBoxCmdToPcode,               // SEND_MAIL_BOX_CMD_TO_PCODE
  FatalError,                          // FATAL_ERROR
  DummyPrintFunction,                  // DEBUG_PRINT_MEM_FUNCTION
  GetSetM2mSaiRegisters,
  SetM2mPrmrrMask,
  SetM2mPrmrrBase
};

STATIC EFI_PEI_PPI_DESCRIPTOR mInstallMrcHooksChipServicesPpi = {
  EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
  &gMrcHooksChipServicesPpiGuid,
  &mMrcHooksChipServicesPpi
};

//
// Below are the MrcHooksChipServicesPpi installation
//
EFI_STATUS
InstallMrcHooksChipServicesPpi (
  PSYSHOST Host
  )
{
  EFI_STATUS                       Status = EFI_SUCCESS;

#ifdef IA32

  EFI_PEI_SERVICES                 **PeiServices;

  PeiServices  = (EFI_PEI_SERVICES**) (UINTN) Host->var.common.oemVariable;

  //
  // Initialize MrcHooksChipServices PPI
  //
  Status = PeiServicesInstallPpi (&mInstallMrcHooksChipServicesPpi);
  ASSERT_EFI_ERROR (Status);
  RcDebugPrint (SDBG_DEFAULT, "InstallPpi MrcHooksChipServicesPpiDesc Status = %08x\n", Status);
  if (EFI_ERROR (Status)) {
    Host->MrcHooksChipServicesPpi = 0;
    Status = EFI_NOT_READY;
  } else {
    Host->MrcHooksChipServicesPpi = (UINT32)&mMrcHooksChipServicesPpi;
    RcDebugPrint (SDBG_DEFAULT, "CacheMrcChipHooksServicesPpi in HOST: Host->MrcHooksChipServicesPpi = %08x\n", Host->MrcHooksChipServicesPpi);
    Status = EFI_SUCCESS;
  }

#endif // #ifdef IA32

  return Status;
}

