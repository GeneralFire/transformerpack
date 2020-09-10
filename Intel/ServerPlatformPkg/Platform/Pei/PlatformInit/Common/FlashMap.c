/** @file
  Build GUIDed HOBs for platform specific flash map.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1999 - 2019 Intel Corporation. <BR>

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

#include <FlashLayout.h>
#include "FlashMap.h"

#include <Ppi/FlashMap.h>
#include <Protocol/FirmwareVolumeBlock.h>
#include <Guid/FlashMapHob.h>
#include <Guid/SystemNvDataGuid.h>
#include <Guid/SystemNvDataHobGuid.h>
#include <Guid/FirmwareFileSystem3.h>

#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/PeiServicesLib.h>


EFI_GUID                            mFvBlockGuid      = EFI_FIRMWARE_VOLUME_BLOCK_PROTOCOL_GUID;

EFI_GUID                            mFfsGuid          = EFI_FIRMWARE_FILE_SYSTEM3_GUID;

EFI_GUID                            mSystemDataGuid   = EFI_SYSTEM_NV_DATA_HOB_GUID;


#define NULL_GUID { 0x00000000, 0x0000, 0x0000, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

static EFI_FLASH_AREA_DATA          mFlashAreaData[]  = {
    /* FVMAIN region */
    {
        FLASH_REGION_FVMAIN_BASE,
        FLASH_REGION_FVMAIN_SIZE,
        EFI_FLASH_AREA_FV | EFI_FLASH_AREA_MEMMAPPED_FV,
        EFI_FLASH_AREA_MAIN_BIOS,
        0, 0, 0,
        NULL_GUID,
    },
    /* OEM_FV region */
    {
        FLASH_REGION_OEM_FV_BASE,
        FLASH_REGION_OEM_FV_SIZE,
        EFI_FLASH_AREA_FV | EFI_FLASH_AREA_MEMMAPPED_FV,
        EFI_FLASH_AREA_MAIN_BIOS,
        0, 0, 0,
        NULL_GUID,
    },
    /* FVWHEA region */
    {
        FLASH_REGION_FVWHEA_BASE,
        FLASH_REGION_FVWHEA_SIZE,
        EFI_FLASH_AREA_FV | EFI_FLASH_AREA_MEMMAPPED_FV,
        EFI_FLASH_AREA_MAIN_BIOS,
        0, 0, 0,
        NULL_GUID,
    },
    /* MICROCODE_FV region */
    {
        FLASH_REGION_MICROCODE_FV_BASE,
        FLASH_REGION_MICROCODE_FV_SIZE,
        EFI_FLASH_AREA_FV | EFI_FLASH_AREA_MEMMAPPED_FV,
        EFI_FLASH_AREA_GUID_DEFINED,
        0, 0, 0,
        EFI_SYSTEM_NV_DATA_HOB_GUID,
    },
    /* RUNTIME_UPDATABLE region */
    {
        FLASH_REGION_RUNTIME_UPDATABLE_BASE,
        FLASH_REGION_RUNTIME_UPDATABLE_SIZE,
        EFI_FLASH_AREA_FV | EFI_FLASH_AREA_MEMMAPPED_FV,
        EFI_FLASH_AREA_GUID_DEFINED,
        0, 0, 0,
        EFI_SYSTEM_NV_DATA_HOB_GUID,
    },
    /* NV_FTW_SPARE region */
    {
        FLASH_REGION_NV_FTW_SPARE_BASE,
        FLASH_REGION_NV_FTW_SPARE_SIZE,
        EFI_FLASH_AREA_FV | EFI_FLASH_AREA_MEMMAPPED_FV,
        EFI_FLASH_AREA_FTW_BACKUP,
        0, 0, 0,
        NULL_GUID,
    },
    /* BIOS_ACM_FV_HEADER region */
    {
        FLASH_REGION_BIOS_ACM_FV_BASE,
        FLASH_REGION_BIOS_ACM_FV_SIZE,
        EFI_FLASH_AREA_SUBFV | EFI_FLASH_AREA_MEMMAPPED_FV,
        EFI_FLASH_AREA_UNUSED,
        0, 0, 0,
        NULL_GUID,
    },
    /* FV_MRC{Normal,Recovery} region: need to be updated according to Normal or Recovery mode, now we make a place holder */
    {
        FLASH_REGION_FV_MRC_NORMAL_BASE,
        FLASH_REGION_FV_MRC_NORMAL_SIZE,
        EFI_FLASH_AREA_FV | EFI_FLASH_AREA_MEMMAPPED_FV,
        EFI_FLASH_AREA_RECOVERY_BIOS,
        0, 0, 0,
        NULL_GUID,
    },
    /* FV_SECPEI region */
    {
        FLASH_REGION_FV_SECPEI_BASE,
        FLASH_REGION_FV_SECPEI_SIZE,
        EFI_FLASH_AREA_FV | EFI_FLASH_AREA_MEMMAPPED_FV,
        EFI_FLASH_AREA_RECOVERY_BIOS,
        0, 0, 0,
        NULL_GUID,
    },

};

#define NUM_FLASH_AREA_DATA (sizeof (mFlashAreaData) / sizeof (mFlashAreaData[0]))

static EFI_HOB_FLASH_MAP_ENTRY_TYPE mFlashMapHobData[] = {

   /* RUNTIME_UPDATABLE.NV_VARIABLE_STORE Subregion */
   {
       EFI_HOB_TYPE_GUID_EXTENSION,
       sizeof (EFI_HOB_FLASH_MAP_ENTRY_TYPE ),
       0,
       EFI_FLASH_MAP_HOB_GUID,
       0, 0, 0,
       EFI_FLASH_AREA_EFI_VARIABLES,
       { 0x00000000, 0x0000, 0x0000, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
       1,
       {
           EFI_FLASH_AREA_SUBFV | EFI_FLASH_AREA_MEMMAPPED_FV,
           0,
           FLASH_REGION_RUNTIME_UPDATABLE_SUBREGION_NV_VARIABLE_STORE_BASE,
           FLASH_REGION_RUNTIME_UPDATABLE_SUBREGION_NV_VARIABLE_STORE_SIZE,
           { 0x00000000, 0x0000, 0x0000, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
       },
   },
   /* RUNTIME_UPDATABLE.NV_FTW_WORKING Subregion */
   {
       EFI_HOB_TYPE_GUID_EXTENSION,
       sizeof (EFI_HOB_FLASH_MAP_ENTRY_TYPE ),
       0,
       EFI_FLASH_MAP_HOB_GUID,
       0, 0, 0,
       EFI_FLASH_AREA_FTW_STATE,
       { 0x00000000, 0x0000, 0x0000, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
       1,
       {
           EFI_FLASH_AREA_SUBFV | EFI_FLASH_AREA_MEMMAPPED_FV,
           0,
           FLASH_REGION_RUNTIME_UPDATABLE_SUBREGION_NV_FTW_WORKING_BASE,
           FLASH_REGION_RUNTIME_UPDATABLE_SUBREGION_NV_FTW_WORKING_SIZE,
           { 0x00000000, 0x0000, 0x0000, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
       },
   },
};

#define NUM_HOB_FLASH_MAP_DATA  (sizeof (mFlashMapHobData) / sizeof (mFlashMapHobData[0]))


/**

  Build GUID HOBs for platform specific flash map.

  @param FfsHeader     Pointer this FFS file header.
  @param PeiServices   General purpose services available to every PEIM.

  @retval EFI_SUCCESS   Guid HOBs for platform flash map is built.
  @retval Otherwise     Failed to build the Guid HOB data.

**/
EFI_STATUS
PeimInitializeFlashMap (
  IN EFI_PEI_FILE_HANDLE       *FileHandle,
  IN CONST EFI_PEI_SERVICES          **PeiServices
  )
{
  EFI_STATUS                    Status;
  UINTN                         Index;
  EFI_FLASH_AREA_HOB_DATA       FlashHobData;
  EFI_HOB_FLASH_MAP_ENTRY_TYPE  *Hob;
  EFI_BOOT_MODE                 BootMode;

  Status = PeiServicesGetBootMode(&BootMode);
  ASSERT_EFI_ERROR (Status);

  //
  // Build flash area entries as GUIDed HOBs.
  //
  for (Index = 0; Index < NUM_FLASH_AREA_DATA; Index++) {
    SetMem (&FlashHobData, sizeof (EFI_FLASH_AREA_HOB_DATA), 0);

    FlashHobData.AreaType               = mFlashAreaData[Index].AreaType;
    FlashHobData.NumberOfEntries        = 1;
    FlashHobData.SubAreaData.Attributes = mFlashAreaData[Index].Attributes;

    if (FixedPcdGetBool (PcdFlashFvMrcCpxBaseIsUnified) && (mFlashAreaData[Index].Base == FLASH_REGION_FV_MRC_NORMAL_BASE)) {
      FlashHobData.SubAreaData.Base     = (EFI_PHYSICAL_ADDRESS) (UINTN) mFlashAreaData[Index].Base - FLASH_REGION_FV_IBB2_OFFSET;
    } else {
      FlashHobData.SubAreaData.Base     = (EFI_PHYSICAL_ADDRESS) (UINTN) mFlashAreaData[Index].Base;
    }
    FlashHobData.SubAreaData.Length     = (EFI_PHYSICAL_ADDRESS) (UINTN) mFlashAreaData[Index].Length;


    switch (FlashHobData.AreaType) {
    case EFI_FLASH_AREA_RECOVERY_BIOS:
    case EFI_FLASH_AREA_MAIN_BIOS:
      CopyMem (
        &FlashHobData.AreaTypeGuid,
        &mFfsGuid,
        sizeof (EFI_GUID)
        );
      CopyMem (
        &FlashHobData.SubAreaData.FileSystem,
        &mFvBlockGuid,
        sizeof (EFI_GUID)
        );
      break;

    case EFI_FLASH_AREA_GUID_DEFINED:
      CopyMem (
        &FlashHobData.AreaTypeGuid,
        &mSystemDataGuid,
        sizeof (EFI_GUID)
        );
      CopyMem (
        &FlashHobData.SubAreaData.FileSystem,
        &mFvBlockGuid,
        sizeof (EFI_GUID)
        );
      break;

    default:
      break;
    }

    Hob = BuildGuidDataHob (
            &gEfiFlashMapHobGuid,
            &FlashHobData,
            sizeof (EFI_FLASH_AREA_HOB_DATA)
            );
    ASSERT (Hob);
  }

  for (Index = 0; Index < NUM_HOB_FLASH_MAP_DATA; Index++) {
    Status = PeiServicesCreateHob (
              EFI_HOB_TYPE_GUID_EXTENSION,
              (UINT16) (sizeof (EFI_HOB_FLASH_MAP_ENTRY_TYPE)),
              &Hob
              );
    if (EFI_ERROR (Status)) {
      return Status;
    }

    CopyMem (
      Hob,
      &mFlashMapHobData[Index],
      sizeof (EFI_HOB_FLASH_MAP_ENTRY_TYPE)
      );
    if (mFlashMapHobData[Index].AreaType == EFI_FLASH_AREA_EFI_VARIABLES) {
      Hob->Entries[0].Base    = Hob->Entries[0].Base + EFI_RUNTIME_UPDATABLE_FV_HEADER_LENGTH;
      Hob->Entries[0].Length  = Hob->Entries[0].Length - EFI_RUNTIME_UPDATABLE_FV_HEADER_LENGTH;
    }
  }

  return EFI_SUCCESS;
}
