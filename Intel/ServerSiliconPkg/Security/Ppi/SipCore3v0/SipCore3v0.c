/** @file

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

  Unless otherwise agreed by Intel in writing, you may not remove or alter this notice
  or any other notice embedded in Materials by Intel or Intel's suppliers or licensors in any way.
**/


#include "SipCore3v0.h"

// Might need a static global to speed things up
extern EFI_PEI_MP_SERVICES_PPI  *mMpServices;

// FIXME: Implement so that module globals not needed
extern CONST EFI_PEI_SERVICES  **mPeiServices;
extern UINT64                    mPrmrrSize;
extern UINT8                     mPrmrrCount;
extern UINT64                   *mPrmrrBase;
extern UINT64                    mPrmrrMask;

EFI_STATUS
EFIAPI
_ProgramPrmrr (
  IN SIP_CORE_3V0_PPI       *This,
  IN CONST EFI_PEI_SERVICES **PeiServices
  )
{
  EFI_STATUS Status = EFI_UNSUPPORTED;
  DEBUG ((EFI_D_INFO, "[SIP_CORE] %a BEGIN\n", __FUNCTION__));

  // Consume API inputs
  // FIXME: stop using global variables for MpServices calls
  mPeiServices = PeiServices;
  mPrmrrSize   = This->PrmrrData.PrmrrSize;
  mPrmrrCount  = This->PrmrrData.PrmrrCount;
  mPrmrrBase   = This->PrmrrData.PrmrrBase;
  mPrmrrMask   = This->PrmrrData.PrmrrMask;

  UINT8 Index;
  for (Index = 0; Index < mPrmrrCount; Index++) {
    DEBUG ((EFI_D_INFO, "[SIP_CORE] PRMRR[%d] = 0x%016lX\n", Index, mPrmrrBase[Index]));
  }

  //
  // Locate Ppi
  //
  if (mMpServices == NULL) {
    Status = SgxMpServicesData (PeiServices);
  }
  if (EFI_ERROR(Status) || mMpServices == NULL) {
    mMpServices = NULL;
    goto Return_SipCore3v0_ProgramPrmrr;
  }

  SgxDebugQueueAllocInit ();
  CoreScopeProgramCorePrmrr ();
  Status = mMpServices->StartupAllAPs (
    PeiServices,
    mMpServices,
    (EFI_AP_PROCEDURE) CoreScopeProgramCorePrmrr,
    FALSE,
    0,
    NULL
    );
  SgxDebugQueueConsume ();
  DEBUG ((EFI_D_INFO, "[SIP_CORE] CoreScopeProgramCorePrmrr %r\n", Status));
Return_SipCore3v0_ProgramPrmrr:
  DEBUG ((EFI_D_INFO, "[SIP_CORE] %a END Status: %r\n", __FUNCTION__, Status));
  return Status;
}

EFI_STATUS
EFIAPI
_LockPrmrr (
  IN SIP_CORE_3V0_PPI       *This,
  IN CONST EFI_PEI_SERVICES **PeiServices
  )
{
  EFI_STATUS Status = EFI_UNSUPPORTED;
  return Status;
}

/**
  PEIM entry point

  @param FileHandle         Pointer to the PEIM FFS file header.
  @param PeiServices        General purpose services available to every PEIM.

  @retval EFI_SUCCESS       Operation completed successfully.
  @retval Otherwise         SGX initialization failed.

**/
EFI_STATUS
EFIAPI
SipCore3v0EntryPoint (
  IN       EFI_PEI_FILE_HANDLE FileHandle,
  IN CONST EFI_PEI_SERVICES    **PeiServices
  )
{
  EFI_STATUS Status = EFI_UNSUPPORTED;

  STATIC SIP_CORE_3V0_PPI Ppi = {
    SIP_CORE_3V0_PPI_REVISION,
    {0},
    _ProgramPrmrr,
    _LockPrmrr
  };

  STATIC EFI_PEI_PPI_DESCRIPTOR PpiList = {
    EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
    &gSecuritySipCore3v0PpiGuid,
    &Ppi
  };

  DEBUG ((EFI_D_INFO, "[SIP_CORE] %a ENTRY\n", __FUNCTION__));
  Status = (*PeiServices)->InstallPpi (PeiServices, &PpiList);

  return Status;
}

