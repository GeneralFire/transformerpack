/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 Intel Corporation. <BR>

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

#include "OpaPlatCfg.h"

typedef struct {
    UINT8 SocketNumber;
    EFI_GUID ConfigFileGuid;
} OPA_PLAT_CONFIG_FILELIST;

CHAR16                          N1vVarName[] = L"hfi1-socket0-configuration";

OPA_PLAT_CONFIG_FILELIST SocketConfigFilesMapping[] = {
    0, { 0x4FD1BC5E, 0x0A53, 0x4501, { 0xB9, 0x13, 0x56, 0xD3, 0x62, 0x98, 0x9E, 0x13 } }, //Socket 0
    1, { 0x2CAD98FC, 0x1897, 0x4837, { 0xB3, 0x13, 0xE6, 0xF0, 0x95, 0xF4, 0xF8, 0x4C } }  //Socket 1
};


/**

  Locate given guid in any of FV

  @param EFI_GUID - Guid to be located

  @retval UINTN - Size of guid located data or file
          EFI_FIRMWARE_VOLUME2_PROTOCOL * - Instance of FV where guid exist
**/

EFI_FIRMWARE_VOLUME2_PROTOCOL *
LocateFV2Handle(
  IN EFI_GUID FileGuid,
  OUT UINTN *Size
  )
{
  EFI_STATUS                    Status;
  UINTN                         Count;
  UINTN                         NumHandles;
  EFI_HANDLE                    *HandleBuffer;
  EFI_FIRMWARE_VOLUME2_PROTOCOL *FwVolInstance;
  UINT32                        FvStatus;
  EFI_FV_FILETYPE               FileType;
  EFI_FV_FILE_ATTRIBUTES        Attributes;

  FwVolInstance = NULL;

  Status = gBS->LocateHandleBuffer(
    ByProtocol,
    &gEfiFirmwareVolume2ProtocolGuid,
    NULL,
    &NumHandles,
    &HandleBuffer
    );
  ASSERT_EFI_ERROR(Status);

  for (Count = 0; Count < NumHandles; Count++) {
    Status = gBS->HandleProtocol(
      HandleBuffer[Count],
      &gEfiFirmwareVolume2ProtocolGuid,
      (VOID **)&FwVolInstance
      );
    ASSERT_EFI_ERROR(Status);

    if (FwVolInstance != NULL){
      ///
      /// Locate Config file and get Size of file
      ///
      Status = FwVolInstance->ReadFile(
        FwVolInstance,
        &FileGuid,
        NULL,
        Size,
        &FileType,
        &Attributes,
        &FvStatus
        );
      if (Status == EFI_SUCCESS) {
        return FwVolInstance;
      }
    } //endif (FwVolInstance != NULL)
  } //endfor
  return FwVolInstance;
}


/**
  Form Variable name string and Create NV variable for OPA platform config file

  @param VOID * - File buffer pointer containing OPA platform config file
         UINTN  - Size of File buffer
     UINTN  - CPU socket number to which platform config file associated with

  @retval VOID

**/
VOID
CreatePlatCfgNVVariable(
  VOID *FileBuffer,
  UINTN FileSize
  )
{
  EFI_STATUS                      Status;

  Status = gRT->SetVariable(
    N1vVarName,
    &gOPAPlatCfgNvVarGuid,
    EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
    FileSize,
    FileBuffer
    );

  DEBUG((EFI_D_INFO, "OPA Platform config NV Variable = %s, Status = %r\n", N1vVarName, Status));

  //
  // String format L"hfi1-socketN-configuration"; Where N= 0, 1, 2, ....
  // Create Next VariableName
  //
  N1vVarName[11]++;
}

/**
  Driver Entry point acts as primary function to locate OPA platform config files from FV
  and create NV varaible with platform config data

  @param EFI_HANDLE
         EFI_SYSTEM_TABLE


  @retval VOID

**/
EFI_STATUS
EFIAPI
OpaPlatCfgEntryPoint(
  IN EFI_HANDLE                         ImageHandle,
  IN EFI_SYSTEM_TABLE                   *SystemTable
  )
{
    EFI_STATUS                    Status;
    UINTN                         Count;
    UINTN                         Size;
    EFI_FIRMWARE_VOLUME2_PROTOCOL *FwVolInstance;
    VOID                          *ConfigFileBuffer;
    //
    // Variables to SetVariable Parameter
    //
    EFI_FV_FILETYPE               FileType;
    EFI_FV_FILE_ATTRIBUTES        Attributes;
    UINT32                        FvStatus;

    //
    // Assign Default Values
    //
    FwVolInstance = NULL;
    ConfigFileBuffer = NULL;

    DEBUG((EFI_D_INFO, "OPA Platform config Data file Handle driver\n"));
    //
    // Loop through each Platform config file entry
    //
    for (Count = 0; Count < sizeof(SocketConfigFilesMapping) / sizeof(OPA_PLAT_CONFIG_FILELIST); Count++){

      //
      // get FWVolProtocol instance
      //
      FwVolInstance = LocateFV2Handle(SocketConfigFilesMapping[Count].ConfigFileGuid, &Size);
      if (FwVolInstance == NULL) {
        //
        // Config file does not present in any FV volume. continue to next config file
        //
        continue;
      }

      Status = gBS->AllocatePool(EfiBootServicesData, Size, (VOID **)&ConfigFileBuffer);

      //
      // Read Config file to memory buffer
      //
      Status = FwVolInstance->ReadFile(
        FwVolInstance,
        &SocketConfigFilesMapping[Count].ConfigFileGuid,
        ((VOID **)&ConfigFileBuffer),
        &Size,
        &FileType,
        &Attributes,
        &FvStatus
        );

      //
      // Truncate 4 bytes from buffer before sending
      // 4 bytes are header and not part of config file.
      //
      if (!EFI_ERROR(Status) && (Size > 4)) {
        CreatePlatCfgNVVariable((UINT8 *)ConfigFileBuffer+4, Size-4);
      }

      if (ConfigFileBuffer != NULL){
        Status = gBS->FreePool(ConfigFileBuffer);
      }
    } //for
    return EFI_SUCCESS;
}
