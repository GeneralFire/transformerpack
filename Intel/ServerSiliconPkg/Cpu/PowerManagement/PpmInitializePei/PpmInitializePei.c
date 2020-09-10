/** @file
  CPU Power Management PEIM

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


#include "PpmInitializePei.h"


PPM_POLICY_CONFIGURATION    *mPpmPolicy = NULL;
EDKII_PEI_MP_SERVICES2_PPI  *mMpService = NULL;
PPM_FROM_PPMINFO_HOB        *mPpmInfo   = NULL;
EFI_CPU_PM_STRUCT           mPpmLib     = { 0 };

STATIC
VOID
EFIAPI
InitializeCpuInfoStruct (
  IN CONST  EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_STATUS                  Status;
  EFI_HOB_GUID_TYPE           *GuidHob;
  EFI_GUID                    UniversalDataGuid = IIO_UNIVERSAL_DATA_GUID;
  UINT32                      RegEax;
  UINT32                      NumOfBitShift;
  CPU_CSR_ACCESS_VAR          *CpuCsrAccessVarPtr;
  PPM_POLICY_PPI              *PpmPolicyPpi;
  IIO_UDS                     *mUdsHobPtr;


  AsmCpuid (1, &RegEax, NULL, NULL, NULL);  //CPUID_VERSION_INFO
  //
  // Find out the bit shift so it can be used to identify APs on other sockets
  //
  AsmCpuidEx (0xB, 1, &NumOfBitShift, NULL, NULL, NULL); //CPUID_EXTENDED_TOPOLOGY

  Status = PeiServicesLocatePpi (&gPpmPolicyPpiGuid, 0, NULL, (VOID **)&PpmPolicyPpi);
  ASSERT_EFI_ERROR (Status);

  mPpmPolicy = &(PpmPolicyPpi->Policy);

  GuidHob    = GetFirstGuidHob (&UniversalDataGuid);
  ASSERT (GuidHob != NULL);
  if (GuidHob == NULL) {
    return;
  }
  mUdsHobPtr = GET_GUID_HOB_DATA (GuidHob);

  CpuCsrAccessVarPtr = GetSysCpuCsrAccessVar ();

  Status = (*PeiServices)->AllocatePool (PeiServices, sizeof (PPM_FROM_PPMINFO_HOB), (VOID **) &mPpmInfo);
  ASSERT_EFI_ERROR (Status);
  ZeroMem (mPpmInfo, sizeof (PPM_FROM_PPMINFO_HOB));

  mPpmInfo->SocketPresentBitMap = CpuCsrAccessVarPtr->socketPresentBitMap;
  mPpmInfo->SlaveDieBitMap      = CpuCsrAccessVarPtr->SlaveDieBitMap;
  mPpmInfo->DfxRstCplBitsEn     = mUdsHobPtr->SystemStatus.DfxRstCplBitsEn;
  mPpmInfo->NumOfBitShift       = (UINT32) (NumOfBitShift & 0x1F);

  //
  // Get MP Services2 Ppi
  //
  Status = PeiServicesLocatePpi (
             &gEdkiiPeiMpServices2PpiGuid,
             0,
             NULL,
             (VOID **)&mMpService
             );
  ASSERT_EFI_ERROR (Status);


  mPpmLib.Info          = mPpmInfo;
  mPpmLib.Info->CapId4Csr.Data = UsraCsrRead (0, 0, CAPID4_PCU_FUN3_REG);

  mPpmLib.Info->HwpmSupport = IsHwpmSupported (mPpmLib.Info->SocketPresentBitMap);

  return;
}

/**

    Initialize Power Management CSRs for each socket.

    @param PPMPolicy Pointer to PPM Policy protocol instance

    @retval None
**/
VOID
CpuPmProgramCsrs (
  VOID
  )
{
  DEBUG ( (EFI_D_ERROR, " \n\n ::PEI Power Management CSR Programming\n\n") );

  CommonPpmProgramCsrs (&mPpmLib, mPpmPolicy, mPpmInfo->SocketPresentBitMap);
}

/**

    Initialize Power Management MSRs for each thread.

    @param PPMPolicy Pointer to PPM Policy protocol instance

    @retval None
**/
VOID
EFIAPI
CpuPmProgramMsrs (
  VOID
  )
{
  UINTN                     ProcessorNumber = 0;

  mMpService->WhoAmI(mMpService, &ProcessorNumber);

  CommonPpmProgramMsrs (&mPpmLib, mPpmPolicy, ProcessorNumber);

  return;
}

/**

  The function puts MSR programing and P state transition together to improve boot performance.

  Arguments: None

  @retval EFI_SUCCESS

**/
EFI_STATUS
EFIAPI
CpuPmMpInitialize (
  VOID
  )
{
  CpuPmProgramMsrs ();

  return EFI_SUCCESS;
}

/**
    PEI Processor Power Management related Register Programming

    @retval Status
    @retval EFI_SUCCESS
**/
EFI_STATUS
EFIAPI
PpmInitializePeiEntry (
  IN  EFI_PEI_FILE_HANDLE               FileHandle,
  IN CONST EFI_PEI_SERVICES             **PeiServices
  )
{
  EFI_STATUS                Status = EFI_SUCCESS;

  DEBUG ((EFI_D_ERROR, "PEI PPM Initialization Entry\n"));

  InitializeCpuInfoStruct (PeiServices);

  InitializeCpuPPMLib((EFI_CPU_PM_STRUCT *)&mPpmLib, mPpmPolicy);

  if (mPpmPolicy->RunCpuPpmInPei == 0) {
    // Return if Setup choice is to run PPM code in DXE
    return EFI_SUCCESS;
  }

  DEBUG((EFI_D_ERROR, "Run PPM in PEI\n"));

  Status = FillThreadSocketMapping (mPpmPolicy);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR(Status)) {
    return Status;
  }

  if (UbiosGenerationEnabled ()) {
    DEBUG ((EFI_D_INFO, "PEI PPM CSR Trace Start\n"));
  }

  CpuPmProgramCsrs();

  if (UbiosGenerationEnabled ()) {
    DEBUG ((EFI_D_INFO, "PEI PPM CSR Trace End\n"));
  }

  if(MsrTraceEnabled ()) {
    DEBUG ((EFI_D_INFO, "PEI PPM MSR Trace Start\n"));
    SetMsrTraceOutputMode (MSR_OUTPUT_ENABLE);
  }

  mMpService->StartupAllAPs(mMpService,
                            (EFI_AP_PROCEDURE) CpuPmMpInitialize,
                            FALSE,
                            0,
                            NULL
                            );

  CpuPmMpInitialize ();

  if (MsrTraceEnabled ()) {
    DEBUG ((EFI_D_INFO, "PEI PPM MSR Trace End\n"));
    SetMsrTraceOutputMode (MSR_OUTPUT_DISABLE);
  }

  return Status;
}

