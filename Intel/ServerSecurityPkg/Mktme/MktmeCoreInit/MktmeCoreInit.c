/** @file

@copyright
INTEL CONFIDENTIAL
Copyright 2017 - 2019 Intel Corporation. <BR>

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

#include <MktmeCoreInit.h>

// EFI_PEI
EFI_PEI_SERVICES                **mPeiServices = NULL;
EFI_PEI_MP_SERVICES_PPI         *mMpServicesPpi = NULL;

/**
Configure Core TME ACTIVATE MSR.

@param  None

@retval EFI_STATUS       Status of the operation
**/
EFI_STATUS
EFIAPI
ConfigureCoreMsr (VOID)
{
  EFI_STATUS  Status = EFI_SUCCESS;

  AsmWriteMsr64 (ICX_MSR_CORE_MKTME_ACTIVATION, 0);
  return Status;
}

/**
PEIM entry point

@param FileHandle         Pointer to the PEIM FFS file header.
@param PeiServices        General purpose services available to every PEIM.

@retval EFI_SUCCESS       Operation completed successfully.
@retval Otherwise         Mktme Core initialization failed.

**/
EFI_STATUS
EFIAPI
MktmeCoreInitEntryPoint (
  IN       EFI_PEI_FILE_HANDLE      FileHandle,
  IN CONST EFI_PEI_SERVICES         **PeiServices
  )
{
  EFI_STATUS                        Status = EFI_SUCCESS;
  MSR_TME_ACTIVATE_REGISTER         Tme_Activate;
  SECURITY_POLICY                   *SecurityPolicy;

  Tme_Activate.Uint64 = 0;

  DEBUG ((EFI_D_INFO, "MktmeCoreInit entry\n"));

  mPeiServices = (EFI_PEI_SERVICES **) PeiServices;

  Status = (*mPeiServices)->LocatePpi (
                              mPeiServices,
                              &gEfiPeiMpServicesPpiGuid,
                              0,
                              NULL,
                              &mMpServicesPpi
                              );

  if (EFI_ERROR (Status) || (mMpServicesPpi == NULL)) {
    DEBUG ((EFI_D_ERROR, "Error: Mp Services Ppi not found. Exiting TME Flow.\n"));
    return Status;
  }

  SecurityPolicy = GetSecurityPolicy ();

  if (SecurityPolicy == NULL) {
    DEBUG ((EFI_D_ERROR," GetCpuPolicy returned NULL. Exiting TME Flow.\n"));
    return EFI_D_ERROR;
  }

  if (SecurityPolicy->TmeCapability == 0) {
    DEBUG ((EFI_D_INFO, "Tme is not supported by the system. Exiting TME Flow.\n"));
    return EFI_SUCCESS;
  }

  Tme_Activate.Uint64 = AsmReadMsr64 (MSR_TME_ACTIVATE);
  if (Tme_Activate.Bits.MkTmeKeyidBits == 0) {
    DEBUG ((EFI_D_INFO, "MKTME is not enabled in BIOS. Exiting MKTME Core Init.\n"));
    return EFI_SUCCESS;
  }

  if (MsrTraceEnabled ()) {
    SetMsrTraceOutputMode (MSR_OUTPUT_ENABLE);
  }

  //Configure CORE TME ACTIVATE MSR
  Status = ConfigureCoreMsr ();
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "[MKTME] Can not configure Core Msr\n"));
    return Status;
  }
  Status = mMpServicesPpi->StartupAllAPs (
                          mPeiServices,
                          mMpServicesPpi,
                          (EFI_AP_PROCEDURE) ConfigureCoreMsr,
                          FALSE,
                          0,
                          NULL
                          );

  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "[MKTME] mMpServicesPpi can not configure core msr\n"));
    return Status;
  }

  if (MsrTraceEnabled ()) {
    SetMsrTraceOutputMode (MSR_OUTPUT_DISABLE);
  }
  DEBUG((EFI_D_INFO, "MktmeCoreInit exit\n"));
  return Status;
}
