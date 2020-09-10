/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 - 2018 Intel Corporation. <BR>

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

#include <Library/BaseLib.h>
#include <Library/HobLib.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Library/DebugPrintErrorLevelLib.h>
#include <Library/BaseMemoryLib.h>
#include <Upi/KtiHost.h>
#include <Ppi/UpiPolicyPpi.h>
#include <Library/SysHostPointerLib.h>
#include "SystemInfoVar.h"
#include <Library/PeiServicesLib.h>


EFI_PEI_PPI_DESCRIPTOR     mPpiListSystemInfo = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gSystemInfoPpiGuid,
  NULL
};

//
// Callback to initialize KtiHostInData HOB
//
EFI_STATUS
EFIAPI
KtiHostInDataHobInitNotifyCallback (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  );

static EFI_PEI_NOTIFY_DESCRIPTOR  mPeiKtiHostInDataHobInitNotifyList = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gUpiSiPolicyPpiGuid,
  (EFI_PEIM_NOTIFY_ENTRY_POINT) KtiHostInDataHobInitNotifyCallback
};

/**
  Notify function to initialize KtiHostInData HOB

  @param[in]  PeiServices       General purpose services available to every PEIM.
  @param[in]  NotifyDescriptor  Notify that this module published.
  @param[in]  Ppi               PPI that was installed.

  @retval     EFI_SUCCESS       The function completed successfully.
  @retval     EFI_UNSUPPORTED   The device is not supported
**/
EFI_STATUS
EFIAPI
KtiHostInDataHobInitNotifyCallback (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  )
{
  EFI_STATUS                   Status;
  UPI_POLICY_PPI               *UpiPolicyPpi;
  VOID                         *GuidHob;

  //
  // Locate UpiPolicyPpi
  //
  Status = (*PeiServices)->LocatePpi (PeiServices, &gUpiSiPolicyPpiGuid, 0, NULL, &UpiPolicyPpi);
  if (!EFI_ERROR(Status)) {
    //
    // Initilize KtiHostInDataHob with UpiSiPolicyPpi
    //
    GuidHob = GetFirstGuidHob (&gEfiKtiHostInDataHobGuid);
    ASSERT(GuidHob != NULL);

    CopyMem ((VOID*) (GET_GUID_HOB_DATA (GuidHob)), (VOID* ) (&UpiPolicyPpi->Upi), sizeof (UpiPolicyPpi->Upi));
  }

  return EFI_SUCCESS;
}

EFI_STATUS
BuildGenericDataHob(
EFI_GUID  *HobGuid,
UINT32    DataLength
)
{
  UINT8     *HobData;

  HobData = BuildGuidHob(HobGuid, DataLength);
  if (HobData == NULL) {
    ASSERT(HobData != NULL);
    return EFI_OUT_OF_RESOURCES;
  }
  ZeroMem(HobData, DataLength);

  return EFI_SUCCESS;
}

/**
  PEIM entry point

  @param FileHandle         Pointer to the PEIM FFS file header.
  @param PeiServices        General purpose services available to every PEIM.

  @retval EFI_SUCCESS       Operation completed successfully.

**/
EFI_STATUS
EFIAPI
SiliconDataInitPei (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_STATUS                   Status;
  VOID                         *HobData;
  SYSTEM_INFO_VAR              *SystemInfoVar;

  //
  // Build the KTI Hobs.
  //
  BuildGenericDataHob(&gEfiKtiHostInDataHobGuid, (UINT32)sizeof(KTI_HOST_IN));
  BuildGenericDataHob(&gEfiKtiHostOutDataHobGuid, (UINT32)sizeof(KTI_HOST_OUT));
  BuildGenericDataHob(&gEfiKtiHostNvramDataHobGuid, (UINT32)sizeof(KTI_HOST_NVRAM));

  //
  // Initialize System Info Var and cache its address into Syshost.
  //

  HobData = BuildGuidHob (&gSystemInfoVarHobGuid, (UINT32)sizeof (SYSTEM_INFO_VAR));
  if (HobData == NULL) {
    ASSERT (HobData != NULL);
    return EFI_OUT_OF_RESOURCES;
  }
  ZeroMem (HobData, (UINT32)sizeof (SYSTEM_INFO_VAR));
  //
  // Set Usra Trace default
  //
  SystemInfoVar = (SYSTEM_INFO_VAR*)HobData;
  SystemInfoVar->UsraTraceControl = PcdGetBool (PcdUsraTraceStartAtEarlierStage);
  SystemInfoVar->UsraTraceConfiguration = PcdGet16 (PcdUsraTraceConfigurationValue);
  SystemInfoVar->CpuCsrAccessVarInfoDumped = FALSE;
  SetSysInfoVarPointer((VOID*)HobData);

  Status = PeiServicesInstallPpi (&mPpiListSystemInfo);
  ASSERT_EFI_ERROR (Status);

  //
  // Install callback function to initialize KtiHostInData HOB.
  //
  Status = (*PeiServices)->NotifyPpi (PeiServices, &mPeiKtiHostInDataHobInitNotifyList);
  ASSERT_EFI_ERROR (Status);

  return EFI_SUCCESS;
}
