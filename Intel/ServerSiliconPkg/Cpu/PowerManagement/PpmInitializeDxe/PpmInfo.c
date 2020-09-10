/** @file
  Code File for CPU Power Management

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2006 - 2020 Intel Corporation. <BR>

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

#include "PpmInitialize.h"
#include <Library/CpuConfigLib.h>
#include <Library/BaseLib.h>
#include <Library/SystemInfoLib.h>
#include <Guid/SecurityPolicy.h>

PPM_FROM_PPMINFO_HOB     *mPpmInfo;
IIO_UDS                  *mUdsHobPtr;
PPM_POLICY_CONFIGURATION *mPpmPolicy;
EFI_MP_SERVICES_PROTOCOL *mMpService = NULL;
PPM_WRAP_STRUCT          mPpmWrapPointer;

VOID
EFIAPI
InitializeCpuInfoStruct (
  VOID
  )
{
  EFI_STATUS                  Status;
  EFI_HOB_GUID_TYPE           *GuidHob;
  VOID                        *HobList;
  EFI_GUID                    UniversalDataGuid = IIO_UNIVERSAL_DATA_GUID;
  UINT32                      RegEax;
  UINT32                      NumOfBitShift;
  CPU_CSR_ACCESS_VAR          *CpuCsrAccessVarPtr;
  SECURITY_POLICY             *pSecurityPolicy = NULL;

  AsmCpuid (1, &RegEax, NULL, NULL, NULL);  //CPUID_VERSION_INFO
  //
  // Find out the bit shift so it can be used to identify APs on other sockets
  //
  AsmCpuidEx (0xB, 1, &NumOfBitShift, NULL, NULL, NULL); //CPUID_EXTENDED_TOPOLOGY

  //
  // Locate CPU PPM Policy Protocol
  //
  Status = gBS->LocateProtocol (&gPpmPolicyProtocolGuid, NULL, &mPpmPolicy);
  ASSERT_EFI_ERROR (Status);

  Status = EfiGetSystemConfigurationTable (&gEfiHobListGuid, &HobList);
  ASSERT_EFI_ERROR (Status);

  GuidHob    = GetFirstGuidHob (&UniversalDataGuid);
  ASSERT (GuidHob != NULL);
  if (GuidHob == NULL) {
    return;
  }
  mUdsHobPtr = GET_GUID_HOB_DATA (GuidHob);

  CpuCsrAccessVarPtr = GetSysCpuCsrAccessVar ();

  Status = gBS->AllocatePool (EfiBootServicesData, sizeof (PPM_FROM_PPMINFO_HOB), (VOID **) &mPpmInfo);
  ASSERT_EFI_ERROR (Status);
  ZeroMem (mPpmInfo, sizeof (PPM_FROM_PPMINFO_HOB));


  //
  // GetSecurityPolicy in order to determine SkipSignalPpmDone
  //
  GuidHob = GetFirstGuidHob (&gSecurityPolicyDataGuid);
  if (GuidHob == NULL) {
    mPpmInfo->SkipSignalPpmDone = 0;
  } else {
    pSecurityPolicy = (SECURITY_POLICY *) GET_GUID_HOB_DATA (GuidHob);
    mPpmInfo->SkipSignalPpmDone = pSecurityPolicy->SkipSignalPpmDone;
  }

  mPpmInfo->SocketPresentBitMap = CpuCsrAccessVarPtr->socketPresentBitMap;
  mPpmInfo->SlaveDieBitMap      = CpuCsrAccessVarPtr->SlaveDieBitMap;
  mPpmInfo->DfxRstCplBitsEn     = mUdsHobPtr->SystemStatus.DfxRstCplBitsEn;
  mPpmInfo->NumOfBitShift       = (UINT32) (NumOfBitShift & 0x1F);

  Status = gBS->LocateProtocol (
                  &gEfiMpServiceProtocolGuid,
                  NULL,
                  &mMpService
                  );
  ASSERT_EFI_ERROR (Status);

  mPpmLib.Info          = mPpmInfo;
  mPpmLib.Info->CapId4Csr.Data = UsraCsrRead (0, 0, CAPID4_PCU_FUN3_REG);
  mPpmWrapPointer.PpmPolicy = mPpmPolicy;
  mPpmWrapPointer.Ppm       = &mPpmLib;

  mPpmLib.Info->HwpmSupport = IsHwpmSupported (mPpmLib.Info->SocketPresentBitMap);

  return;
}
