/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 Intel Corporation. <BR>

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

#include "EmulationDfxSetup.h"

#define EMULATION_SETUP_SIGNATURE SIGNATURE_32 ('E', 'M', 'U', 'L')

typedef struct _INTEL_EMULATION_SETUP_INSTANCE {
  UINTN Signature;
  EFI_HANDLE ImageHandle;
  EFI_HANDLE DeviceHandle;
  EFI_HII_CONFIG_ACCESS_PROTOCOL ConfigAccess;
  VOID *StartOpCodeHandle;
  VOID *EndOpCodeHandle;

  EFI_HII_HANDLE HiiHandle;
  EMULATION_DFX_CONFIGURATION  EmulationConfig;

} INTEL_EMULATION_SETUP_INSTANCE;

INTEL_EMULATION_SETUP_INSTANCE mPrivateData;

typedef struct {
  VENDOR_DEVICE_PATH Node;
  EFI_DEVICE_PATH_PROTOCOL End;
} HII_VENDOR_DEVICE_PATH;

HII_VENDOR_DEVICE_PATH mHiiVendorDevicePath =
  {
    {
      {
        HARDWARE_DEVICE_PATH,
        HW_VENDOR_DP,
        { (UINT8)(sizeof(VENDOR_DEVICE_PATH)),
          (UINT8)((sizeof(VENDOR_DEVICE_PATH)) >> 8)
        }
      },
     EMULATION_FORMSET_GUID
    },
    {
     END_DEVICE_PATH_TYPE,
     END_ENTIRE_DEVICE_PATH_SUBTYPE,
      {
      (UINT8)(END_DEVICE_PATH_LENGTH), (UINT8)((END_DEVICE_PATH_LENGTH) >> 8)
      }
    }
  };

extern UINT8 EmulationDfxSetupFormsBin[];
extern EFI_GUID gEmulationFormSetGuid;



/**

 Entry point of the Emulation screen setup driver.

 @param ImageHandle   EFI_HANDLE: A handle for the image that is initializing this driver
 @param SystemTable   EFI_SYSTEM_TABLE: A pointer to the EFI system table

 @retval EFI_SUCCESS:              Driver initialized successfully
 @retval EFI_UNSUPPORTED           Driver has unsupported needs

 **/
EFI_STATUS
EFIAPI
InstallEmuationSetupScreen (
  IN EFI_HANDLE ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_STATUS Status;
  EFI_IFR_GUID_LABEL * StartLabel;
  EFI_IFR_GUID_LABEL * EndLabel;

  mPrivateData.Signature = EMULATION_SETUP_SIGNATURE;
  mPrivateData.ImageHandle = ImageHandle;

  mPrivateData.DeviceHandle = NULL;


  Status = SystemTable->BootServices->InstallMultipleProtocolInterfaces(
      &mPrivateData.DeviceHandle, &gEfiDevicePathProtocolGuid, &mHiiVendorDevicePath,
      NULL);
  ASSERT_EFI_ERROR(Status);


  mPrivateData.HiiHandle = HiiAddPackages(&gEmulationFormSetGuid, mPrivateData.DeviceHandle,
                                          EmulationDfxSetupFormsBin, EmulationDfxSetupStrings,
                                          NULL);

  ASSERT(mPrivateData.HiiHandle != NULL);

  //
  // Initialize the container for dynamic opcodes
  //
  mPrivateData.StartOpCodeHandle = HiiAllocateOpCodeHandle();
  ASSERT(mPrivateData.StartOpCodeHandle != NULL);

  mPrivateData.EndOpCodeHandle = HiiAllocateOpCodeHandle();
  ASSERT(mPrivateData.EndOpCodeHandle != NULL);

  if((mPrivateData.StartOpCodeHandle == NULL) || (mPrivateData.EndOpCodeHandle == NULL)){
    if(mPrivateData.StartOpCodeHandle != NULL){
      HiiFreeOpCodeHandle(mPrivateData.StartOpCodeHandle);
    }
    if(mPrivateData.EndOpCodeHandle != NULL){
      HiiFreeOpCodeHandle (mPrivateData.EndOpCodeHandle);
    }
    return EFI_OUT_OF_RESOURCES;
  }


  //
  // Create Hii Extend Label OpCode as the Top opcode
  //
  StartLabel = (EFI_IFR_GUID_LABEL *) HiiCreateGuidOpCode(mPrivateData.StartOpCodeHandle,
                                                          &gEfiIfrTianoGuid, NULL,
                                                          sizeof(EFI_IFR_GUID_LABEL));
  StartLabel->ExtendOpCode = EFI_IFR_EXTEND_OP_LABEL;
  StartLabel->Number = VFR_FORMLABLE_EMULATION_SETUP_TOP;

  //
  // Create Hii Extend Label OpCode as the Bottom opcode
  //
  EndLabel = (EFI_IFR_GUID_LABEL *) HiiCreateGuidOpCode(mPrivateData.EndOpCodeHandle,
                                                        &gEfiIfrTianoGuid, NULL,
                                                        sizeof(EFI_IFR_GUID_LABEL));
  EndLabel->ExtendOpCode = EFI_IFR_EXTEND_OP_LABEL;
  EndLabel->Number = VFR_FORMLABLE_EMULATION_SETUP_BOTTOM;

  return EFI_SUCCESS;
}
