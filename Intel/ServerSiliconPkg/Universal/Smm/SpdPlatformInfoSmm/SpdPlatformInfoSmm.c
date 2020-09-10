/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation. <BR>

  The source code contained or described herein and all documents related to the
  source code ("Material") are owned by Intel Corporation or its suppliers or
  licensors. Title to the Material remains with Intel Corporation or its suppliers
  and licensors. The Material may contain trade secrets and proprietary and
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

#include "SpdPlatformInfoSmm.h"

SMM_SPD_PLATFORM_INFO_PROTOCOL *SmmSpdPlatformInfo;

EFI_SMM_BASE2_PROTOCOL    *mSmmBase = NULL;
EFI_SMM_SYSTEM_TABLE2     *mSmst = NULL;

/**
  SMM driver's entry point.

  Spd Platform Info Smm driver entry point
  Copies some Platform guid information to smm runtime memory

  @param[in] ImageHandle  The firmware allocated handle for the EFI image.
  @param[in] SystemTable  A pointer to the EFI System Table.

  @retval EFI_SUCCESS     The entry point is executed successfully.
  @retval Others          Some error occurs when executing this entry point.
**/
EFI_STATUS
EFIAPI
SpdPlatformInfoEntry (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTablePtr
  )
{
  EFI_STATUS                Status = EFI_SUCCESS;
  EFI_PLATFORM_INFO         *PlatformInfo;
  EFI_HOB_GUID_TYPE         *GuidHob;
  UINT8                     Socket = 0;
  EFI_HANDLE                Handle = NULL;

  DEBUG ((DEBUG_INFO, "\nSPD Platform Info Smm  Driver - Entry\n"));

  //
  // Retrieve SMM Base Protocol
  //
  Status = gBS->LocateProtocol (
                  &gEfiSmmBase2ProtocolGuid,
                  NULL,
                  (VOID**) &mSmmBase
                  );

  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  
  mSmmBase->GetSmstLocation (mSmmBase, &mSmst);

  Status = mSmst->SmmAllocatePool (EfiRuntimeServicesData, sizeof (SMM_SPD_PLATFORM_INFO_PROTOCOL), (VOID **)&SmmSpdPlatformInfo);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  ZeroMem (SmmSpdPlatformInfo, sizeof (SMM_SPD_PLATFORM_INFO_PROTOCOL));

  //
  // Get PlatformInfo GUID and HOB to retrieve interposer type
  //
  GuidHob = GetFirstGuidHob (&gEfiPlatformInfoGuid);
  ASSERT (GuidHob != NULL);
  if (GuidHob == NULL) {
    return Status;
  }

  PlatformInfo = GET_GUID_HOB_DATA (GuidHob);

  if (PlatformInfo == NULL) {
    DEBUG ((DEBUG_ERROR, "\n Error:Platform Info Not found, Did not published gSmmSpdPlatformInfoProtocolGuid\n"));
    return Status;
  }

  SmmSpdPlatformInfo->TypeRevisionId =  PlatformInfo->TypeRevisionId;
  SmmSpdPlatformInfo->BoardId =  PlatformInfo->BoardId;


  while (Socket < MAX_SOCKET) {
    SmmSpdPlatformInfo->InterposerType[Socket] = PlatformInfo->InterposerType[Socket];
    Socket++;
  }

  Status = mSmst->SmmInstallProtocolInterface (
                &Handle,
                &gSmmSpdPlatformInfoProtocolGuid,
                EFI_NATIVE_INTERFACE,
                SmmSpdPlatformInfo
                );

  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    mSmst->SmmFreePool(SmmSpdPlatformInfo);
    SmmSpdPlatformInfo = NULL;
    return Status;
  }

  DEBUG ((DEBUG_INFO, "\nSPD Platform Info Smm  Driver - Exit\n"));

  return Status;

}