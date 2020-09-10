/** @file
  Install Mrc normal fv info .

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2019 Intel Corporation. <BR>

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
#include <PiPei.h>
#include <Uefi.h>
//APTIOV_SERVER_OVERRIDE_RC_START : for FSP
//#include <FlashMap.h>
//APTIOV_SERVER_OVERRIDE_RC_END : for FSP
#include <Ppi/FirmwareVolumeInfo.h>
#include <Guid/FirmwareFileSystem3.h>
#include <Pi/PiBootMode.h>
#include <Library/PeiServicesLib.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
//APTIOV_SERVER_OVERRIDE_RC_START : for FSP
//#include <Register/Intel/Cpuid.h>
//#include <Cpu/CpuIds.h>
//APTIOV_SERVER_OVERRIDE_RC_END : for FSP
#include <Ppi/MemoryDiscovered.h>
//APTIOV_SERVER_OVERRIDE_RC_START : for FSP
#include <Token.h>
//APTIOV_SERVER_OVERRIDE_RC_END : for FSP

EFI_STATUS
EFIAPI
PublishPostMemFVCallback (
  IN CONST EFI_PEI_SERVICES     **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  );

//TODO: Update PPI definitions once flash map is finalized.
//The PPI reported will include MRC FV, post memory FV and maybe CPX/ICX specific FV in Unify IFWI
// APTIOV_SERVER_OVERRIDE_RC_START :  for FSP
/*
static EFI_PEI_FIRMWARE_VOLUME_INFO_PPI mFvPreMemInfoPpi = {
  EFI_FIRMWARE_FILE_SYSTEM3_GUID,
  (VOID*)FLASH_REGION_FV_PRE_MEM_BASE,
  FLASH_REGION_FV_PRE_MEM_SIZE,
  NULL,
  NULL
};

static EFI_PEI_PPI_DESCRIPTOR     mPpiListFvPreMem = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiPeiFirmwareVolumeInfoPpiGuid,
  &mFvPreMemInfoPpi
};*/
//APTIOV_SERVER_OVERRIDE_RC_END : for FSP

static EFI_PEI_FIRMWARE_VOLUME_INFO_PPI mFvMrcNormalInfoPpi = {
  EFI_FIRMWARE_FILE_SYSTEM3_GUID,
//APTIOV_SERVER_OVERRIDE_RC_START :  for FSP
  (VOID*)FV_FSP_M_BASE,
  FV_FSP_M_SIZE,
//APTIOV_SERVER_OVERRIDE_RC_END : for FSP  
  NULL,
  NULL
};

static EFI_PEI_PPI_DESCRIPTOR     mPpiListFvMrcNormal = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiPeiFirmwareVolumeInfoPpiGuid,
  &mFvMrcNormalInfoPpi
};

//APTIOV_SERVER_OVERRIDE_RC_START : for FSP
/*static EFI_PEI_FIRMWARE_VOLUME_INFO_PPI mFvPostMemoryInfoPpi = {
  EFI_FIRMWARE_FILE_SYSTEM3_GUID,
  (VOID*)FLASH_REGION_FV_POST_MEMORY_BASE,
  FLASH_REGION_FV_POST_MEMORY_SIZE,
  NULL,
  NULL
};

static EFI_PEI_PPI_DESCRIPTOR     mPpiListFvPostMemory = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiPeiFirmwareVolumeInfoPpiGuid,
  &mFvPostMemoryInfoPpi
};*/
//APTIOV_SERVER_OVERRIDE_RC_END : for FSP

static EFI_PEI_FIRMWARE_VOLUME_INFO_PPI mFvFspSInfoPpi = {
  EFI_FIRMWARE_FILE_SYSTEM3_GUID,
//APTIOV_SERVER_OVERRIDE_RC_START : for FSP  
  (VOID*)FV_FSP_S_BASE,
  FV_FSP_S_SIZE,
//APTIOV_SERVER_OVERRIDE_RC_END : for FSP
  NULL,
  NULL
};

static EFI_PEI_PPI_DESCRIPTOR     mPpiListFvFspS = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiPeiFirmwareVolumeInfoPpiGuid,
  &mFvFspSInfoPpi
};

//APTIOV_SERVER_OVERRIDE_RC_START : for FSP
/*static EFI_PEI_FIRMWARE_VOLUME_INFO_PPI mFvFspSCpxInfoPpi = {
  EFI_FIRMWARE_FILE_SYSTEM3_GUID,
  (VOID*)FLASH_REGION_FV_FSPSCPX_BASE,
  FLASH_REGION_FV_FSPSCPX_SIZE,
  NULL,
  NULL
};

static EFI_PEI_PPI_DESCRIPTOR     mPpiListFvFspSCpx = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiPeiFirmwareVolumeInfoPpiGuid,
  &mFvFspSCpxInfoPpi
};*/
//APTIOV_SERVER_OVERRIDE_RC_END : for FSP

static EFI_PEI_NOTIFY_DESCRIPTOR mNotifyList = {
   EFI_PEI_PPI_DESCRIPTOR_NOTIFY_DISPATCH | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
   &gEfiPeiMemoryDiscoveredPpiGuid,
   PublishPostMemFVCallback
};

/**
  Publish Post-Memory Firmware Volumes once there is main memory

  @param[in]  PeiServices       General purpose services available to every PEIM.
  @param[in]  NotifyDescriptor  Notify that this module published.
  @param[in]  Ppi               PPI that was installed.

  @retval     EFI_SUCCESS       The function completed successfully.
**/
EFI_STATUS
EFIAPI
PublishPostMemFVCallback (
  IN CONST EFI_PEI_SERVICES     **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  )
{
  EFI_STATUS                    Status = EFI_SUCCESS;
  // APTIOV_SERVER_OVERRIDE_RC_START : for FSP
  /*
  UINT32                        CpuFamily;
  UINT32                        RegEax = 0;

  AsmCpuid (CPUID_VERSION_INFO, &RegEax, NULL, NULL, NULL);
  CpuFamily = (UINT32)((RegEax >> 4) & 0xFFFF);

  if (FLASH_REGION_FV_POST_MEMORY_BASE != 0) {
    Status = (**PeiServices).InstallPpi (PeiServices, &mPpiListFvPostMemory);
  }
  
  if ((CpuFamily == CPU_FAMILY_SKX) && (FLASH_REGION_FV_FSPSCPX_BASE != 0)) {
    DEBUG ((DEBUG_INFO, "Publish FSP-S of CPX on Unified IFWI\n"));
    Status = (**PeiServices).InstallPpi (PeiServices, &mPpiListFvFspSCpx);
  } else {
    */
    {
    // APTIOV_SERVER_OVERRIDE_RC_END : for FSP         
    DEBUG ((DEBUG_INFO, "Publish FSP-S\n"));
    Status = (**PeiServices).InstallPpi (PeiServices, &mPpiListFvFspS);
  }
  return Status;
}
/**

  Publish Mrc Normal Fv Entry.

  @param FileHandle         Pointer to the PEIM FFS file header.
  @param PeiServices        General purpose services available to every PEIM.

  @retval EFI_SUCCESS       Operation completed successfully.
  @retval Otherwise         Mrc normal fv info install failed.
**/
EFI_STATUS
EFIAPI
PublishMrcNormalFvEntry (
  IN EFI_PEI_FILE_HANDLE       FileHandle,
  IN CONST EFI_PEI_SERVICES    **PeiServices
  )
{
  EFI_STATUS        Status;
  // APTIOV_SERVER_OVERRIDE_RC_START : for FSP
  /*
  EFI_BOOT_MODE     BootMode;
  UINT32            CpuFamily;
  UINT32            RegEax = 0;

  AsmCpuid (CPUID_VERSION_INFO, &RegEax, NULL, NULL, NULL);
  CpuFamily = (UINT32)((RegEax >> 4) & 0xFFFF);

  BootMode = BOOT_WITH_FULL_CONFIGURATION;

  Status = PeiServicesGetBootMode (&BootMode);
  ASSERT_EFI_ERROR (Status);

  //TODO: Update PPI once flash map is finalized.
  if (BootMode != BOOT_IN_RECOVERY_MODE) {
    // Publish Pre-Mem CPX/ICX for Unify IFWI
    if ((CpuFamily == CPU_FAMILY_SKX || CpuFamily == CPU_FAMILY_ICX) &&
        (FLASH_REGION_FV_PRE_MEM_BASE != 0)) {
      Status = (**PeiServices).InstallPpi (PeiServices, &mPpiListFvPreMem);
    }
    */
    // APTIOV_SERVER_OVERRIDE_RC_END :  for FSP
    Status = (**PeiServices).InstallPpi (PeiServices, &mPpiListFvMrcNormal);
    Status = (**PeiServices).NotifyPpi (PeiServices, &mNotifyList);
//APTIOV_SERVER_OVERRIDE_RC_START :  for FSP	
  //}
//APTIOV_SERVER_OVERRIDE_RC_END :  for FSP  
  ASSERT_EFI_ERROR (Status);

  return Status;
}

