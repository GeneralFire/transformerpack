/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2012 - 2020 Intel Corporation. <BR>

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

#include <PiPei.h>
#include <Library/MultiPlatSupportLib.h>
#include <Library/PlatformSetupVariableSyncLib.h>
#include <Library/HobLib.h>
#include <MultiPlatSupport.h>
#include <Ppi/MemoryDiscovered.h>
#include <Library/ReadFfsLib.h>
#include <Library/MemoryAllocationLib.h>
//APTIOV_SERVER_OVERRIDE_START: Remove missing header
//#include <Guid/AuthenticatedVariableFormat.h>
#include <Guid/VariableFormat.h>
//APTIOV_SERVER_OVERRIDE_END: Remove missing header


/**
  This function finds the matched default data and create GUID hob for it.

  @param[in] DefaultId    Specifies the type of defaults to retrieve.
  @param[in] BoardId      Specifies the platform board of defaults to retrieve.

  @retval EFI_SUCCESS            The matched default data is found.
  @retval EFI_NOT_FOUND          The matched default data is not found.
  @retval EFI_OUT_OF_RESOURCES   No enough resource to create HOB.

**/
EFI_STATUS
CreateDefaultVariableHob (
  IN UINT16  DefaultId,
  IN UINT16   BoardId
  )
{
  EFI_PEI_SERVICES           **PeiServices;
  UINTN                      FvInstance;
  EFI_FIRMWARE_VOLUME_HEADER *FvHeader;
  EFI_FFS_FILE_HEADER        *FfsHeader;
  UINT32                     FileSize;
  EFI_COMMON_SECTION_HEADER  *Section;
  UINT32                     SectionLength;
  BOOLEAN                    DefaultSettingIsFound;
  DEFAULT_DATA               *DefaultData;
  DEFAULT_INFO               *DefaultInfo;
  VARIABLE_STORE_HEADER      *VarStoreHeader;
  VARIABLE_STORE_HEADER      *VarStoreHeaderHob;
  UINT8                      *VarHobPtr;
  UINT8                      *VarPtr;
  UINT32                     VarDataOffset;
  UINT32                     VarHobDataOffset;
  BOOLEAN                    IsFirstSection;
  DATA_DELTA                 *DataDelta;
  UINTN                      DataDeltaSize;
  UINT32                     FFSSize = 0;
  UINTN                      Index;
  EFI_HOB_GUID_TYPE          *GuidHob = NULL;
  VARIABLE_STORE_HEADER      *VarStoreHeaderHobTemp = NULL;

  //
  // Get PeiService pointer
  //
  PeiServices = (EFI_PEI_SERVICES **)GetPeiServicesTablePointer ();

  //
  // Find the FFS file that stores all default data.
  //
  DefaultSettingIsFound = FALSE;
  FvInstance         = 0;
  FfsHeader          = NULL;
  while (((*PeiServices)->FfsFindNextVolume (PeiServices, FvInstance, (VOID **) &FvHeader) == EFI_SUCCESS) &&
         (!DefaultSettingIsFound)) {
    FfsHeader = NULL;
    while ((*PeiServices)->FfsFindNextFile (PeiServices, EFI_FV_FILETYPE_FREEFORM, FvHeader, (VOID **) &FfsHeader) == EFI_SUCCESS) {
      if (CompareGuid ((EFI_GUID *) FfsHeader, &gDefaultDataOptSizeFileGuid)) {
        DefaultSettingIsFound = TRUE;
        break;
      }
    }
    FvInstance ++;
  }

  //
  // FFS file is not found.
  //
  if (!DefaultSettingIsFound) {

    if(PcdGet32(PcdFailSafeVarFfsSize)!=0 ){
      //
      // Try to search other FVS
      //
      FfsHeader = (EFI_FFS_FILE_HEADER *) AllocatePool(PcdGet32(PcdFailSafeVarFfsSize) );
      if(FfsHeader == NULL) {
        return EFI_NOT_FOUND;
      }
      if(EFI_SUCCESS != ReadFFSFile( (EFI_FIRMWARE_VOLUME_HEADER *) PcdGet32(PcdFailSafeVarFvBase), gDefaultDataOptSizeFileGuid, 0, FfsHeader, &FFSSize, FALSE)) {
        return EFI_NOT_FOUND;
      }
      ASSERT(PcdGet32(PcdFailSafeVarFfsSize) <FFSSize);
    } else {
      return EFI_NOT_FOUND;
    }

  }

  GuidHob = GetFirstGuidHob (&gEfiAuthenticatedVariableGuid);
  if (GuidHob != NULL) {
    VarStoreHeaderHobTemp = (VARIABLE_STORE_HEADER *) GET_GUID_HOB_DATA (GuidHob);
  } else {
    GuidHob = GetFirstGuidHob (&gEfiVariableGuid);
    if (GuidHob != NULL) {
      VarStoreHeaderHobTemp = (VARIABLE_STORE_HEADER *) GET_GUID_HOB_DATA (GuidHob);
    }
  }

  if (!VarStoreHeaderHobTemp) {
    //
    // Find the matched default data for the input default ID and plat ID.
    //
    DefaultSettingIsFound = FALSE;
    VarStoreHeaderHob     = NULL;
    VarHobPtr      = NULL;
    DataDelta      = NULL;
    DataDeltaSize  = 0;
    IsFirstSection = TRUE;
    VarStoreHeader = NULL;
    Section  = (EFI_COMMON_SECTION_HEADER *)(FfsHeader + 1);
    FileSize = *(UINT32 *)(FfsHeader->Size) & 0x00FFFFFF;
    while (((UINTN) Section < (UINTN) FfsHeader + FileSize) && (!DefaultSettingIsFound)) {
      DefaultData = (DEFAULT_DATA *) (Section + 1);
      DefaultInfo = &(DefaultData->DefaultInfo[0]);
      SectionLength = *(UINT32 *)Section->Size & 0x00FFFFFF;
      if (IsFirstSection) {
        //
        // Create HOB to store default data so that Variable driver can use it.
        // Allocate more data for header alignment.
        //
        VarStoreHeader = (VARIABLE_STORE_HEADER *) ((UINT8 *) DefaultData + DefaultData->HeaderSize);
        VarStoreHeaderHob = (VARIABLE_STORE_HEADER *) BuildGuidHob (&VarStoreHeader->Signature, VarStoreHeader->Size + HEADER_ALIGNMENT - 1);
        if (VarStoreHeaderHob == NULL) {
          //
          // No enough hob resource.
          //
          return EFI_OUT_OF_RESOURCES;
        }

        //
        // Copy variable storage header.
        //
        CopyMem (VarStoreHeaderHob, VarStoreHeader, sizeof (VARIABLE_STORE_HEADER));
        //
        // Copy variable data.
        //
        VarPtr           = (UINT8 *) HEADER_ALIGN ((UINTN) (VarStoreHeader + 1));
        VarDataOffset    = (UINT32) ((UINTN) VarPtr - (UINTN) VarStoreHeader);
        VarHobPtr        = (UINT8 *) HEADER_ALIGN ((UINTN) (VarStoreHeaderHob + 1));
        VarHobDataOffset = (UINT32) ((UINTN) VarHobPtr - (UINTN) VarStoreHeaderHob);
        CopyMem (VarHobPtr, VarPtr, VarStoreHeader->Size - VarDataOffset);
        //
        // Update variable size.
        //
        VarStoreHeaderHob->Size = VarStoreHeader->Size - VarDataOffset + VarHobDataOffset;

        //
        // Update Delta Data
        //
        VarHobPtr = (UINT8 *) VarStoreHeaderHob - VarDataOffset + VarHobDataOffset;
      } else {
        //
        // Apply delta setting
        //
        DataDelta     = (DATA_DELTA *) ((UINT8 *) DefaultData + DefaultData->HeaderSize);
        DataDeltaSize = SectionLength - sizeof (EFI_COMMON_SECTION_HEADER) - DefaultData->HeaderSize;
        for (Index = 0; Index < DataDeltaSize / sizeof (DATA_DELTA); Index++) {
          *((UINT8 *) VarHobPtr + DataDelta[Index].Offset) = DataDelta[Index].Value;
        }
      }
      //
      // Find the matched DefaultId
      //
      while ((UINTN) DefaultInfo < (UINTN) DefaultData + DefaultData->HeaderSize) {
        if (DefaultInfo->DefaultId == DefaultId && DefaultInfo->BoardId == BoardId) {
          DefaultSettingIsFound = TRUE;
          break;
        }
        DefaultInfo ++;
      }
      //
      // Size is 24 bits wide so mask upper 8 bits.
      // SectionLength is adjusted it is 4 byte aligned.
      // Go to the next section
      //
      SectionLength = (SectionLength + 3) & (~3);
      ASSERT (SectionLength != 0);
      Section = (EFI_COMMON_SECTION_HEADER *)((UINT8 *)Section + SectionLength);
      IsFirstSection = FALSE;
    }
    //
    // Matched default data is not found.
    //
    if (!DefaultSettingIsFound) {
      //
      // Change created HOB type to be unused.
      //
      if (VarStoreHeaderHob != NULL) {
        ((EFI_HOB_GUID_TYPE *)((UINT8 *) VarStoreHeaderHob - sizeof (EFI_HOB_GUID_TYPE)))->Header.HobType = EFI_HOB_TYPE_UNUSED;
      }
      return EFI_NOT_FOUND;
    }
  } else {
    VarStoreHeaderHob = VarStoreHeaderHobTemp;
  }

  SyncSetupVariable(PeiServices,VarStoreHeaderHob,FALSE);

  return EFI_SUCCESS;
}
