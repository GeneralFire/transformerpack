/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2008 - 2013 Intel Corporation. <BR>

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

#include "ForceEfiBoot.h"

/**

  Look for the designated marker file and check for the force boot signature.

  @param[in] Device         SimpleFileSystem device handle
  @param[in] FileName       File name for the image

  @retval EFI_SUCCESS       Successfully found the file with the force boot signature.
  @retval EFI_NOT_FOUND     The file is not found.

**/
STATIC
EFI_STATUS
EfiBootCheckForMarkerFile (
  IN EFI_HANDLE Device,
  IN CHAR16     *FileName
  )
{
  EFI_STATUS                        Status;
  EFI_SIMPLE_FILE_SYSTEM_PROTOCOL   *Volume;
  EFI_FILE_PROTOCOL                 *Root;
  EFI_FILE_PROTOCOL                 *ThisFile;
  UINTN                             BufferSize;
  UINT64                            FileSize;
  EFI_FILE_INFO                     *Info;
  UINT8                             *Buffer;

  Buffer    = NULL;
  Root      = NULL;
  ThisFile  = NULL;
  Info      = NULL;

  //
  // Handle the file system interface to the device
  //
  Status = gBS->HandleProtocol (
                  Device,
                  &gEfiSimpleFileSystemProtocolGuid,
                  (VOID *) &Volume
                  );
  if (EFI_ERROR (Status)) {
    goto CleanUp;
  }

  Status = Volume->OpenVolume (
                     Volume,
                     &Root
                     );
  if (EFI_ERROR (Status)) {
    goto CleanUp;
  }

  Status = Root->Open (
                   Root,
                   &ThisFile,
                   FileName,
                   EFI_FILE_MODE_READ,
                   0
                   );
  if (EFI_ERROR (Status)) {
    goto CleanUp;
  }

  //
  // Get file size.  We don't know how much info GetInfo wants to return,
  // so we guess, and if we fail, we try again with the corrected BufferSize.
  //
  BufferSize  = SIZE_OF_EFI_FILE_INFO + 200;

  do {
    Info = NULL;
    Status = gBS->AllocatePool (
                    EfiBootServicesData,
                    BufferSize,
                    (VOID**)&Info
                    );
    if (EFI_ERROR (Status)) {
      goto CleanUp;
    }
    Status = ThisFile->GetInfo (
                         ThisFile,
                         &gEfiFileInfoGuid,
                         &BufferSize,
                         Info
                         );
    if (!EFI_ERROR (Status)) {
      break;
    }
    if (Status != EFI_BUFFER_TOO_SMALL) {
      goto CleanUp;
    }
    gBS->FreePool (Info);
  } while (TRUE);

  FileSize = Info->FileSize;
  gBS->FreePool (Info);
  Info = NULL;

  //
  // Read file.
  //
  if (FileSize > MAX_BUFFER) {
    BufferSize = MAX_BUFFER;
  } else {
    BufferSize = FileSize;
  }

  Status = gBS->AllocatePool (
                  EfiBootServicesData,
                  BufferSize,
                  (VOID**)&Buffer
                  );
  if (EFI_ERROR (Status)) {
    goto CleanUp;
  }

  Status = ThisFile->Read (ThisFile, &BufferSize, Buffer);
  if (EFI_ERROR (Status)) {
    goto CleanUp;
  }

  //
  // Now check the "Force EFI Boot" signature.
  // It must be in the first line of the file.
  //
  if (AsciiStrnCmp (
        (const CHAR8 *)Buffer,
        FORCE_BOOT_SIGNATURE,
        sizeof(FORCE_BOOT_SIGNATURE) - 1
        ) == 0) {
    Status = EFI_SUCCESS;
  } else {
    Status = EFI_NOT_FOUND;
  }

CleanUp:
    if (Info != NULL) {
      gBS->FreePool (Info);
    }
    if (Buffer != NULL) {
      gBS->FreePool (Buffer);
    }
    if (ThisFile != NULL) {
      ThisFile->Close (ThisFile);
    }
    if (Root != NULL) {
      Root->Close (Root);
    }
    return Status;
}

/**

  Check if the marker file with the designated signature exists.

  @retval TRUE              The marker file with the designated signature exists.
  @retval FALSE             File is not found, or signature does not match.

**/
BOOLEAN
IsForceEfiBoot (
  VOID
  )
{
  EFI_BLOCK_IO_PROTOCOL     *BlkIo;
  UINTN                     Index;
  EFI_STATUS                Status;
  UINTN                     NumberFileSystemHandles;
  EFI_HANDLE                *FileSystemHandles;
  BOOLEAN                   MarkerFileFound;

  Status            = EFI_SUCCESS;
  MarkerFileFound   = FALSE;

  //
  // The original implementation (Romley) ignored the marker if we were entering
  // Setup or the Boot First menu.  Now you must physically remove the media.
  //

  //
  // Try all removable media.  It has to have a SimpleFileSystem and a BlkIo.
  //
  gBS->LocateHandleBuffer (
         ByProtocol,
         &gEfiSimpleFileSystemProtocolGuid,
         NULL,
         &NumberFileSystemHandles,
         &FileSystemHandles
         );
  for (Index = 0; Index < NumberFileSystemHandles; Index++) {
    Status = gBS->HandleProtocol (
                    FileSystemHandles[Index],
                    &gEfiBlockIoProtocolGuid,
                    (VOID **) &BlkIo
                    );
    if (!EFI_ERROR (Status) && BlkIo->Media->RemovableMedia) {
      //
      // Check the Removable Media for the marker file.
      //
      Status = EfiBootCheckForMarkerFile (
                 FileSystemHandles[Index],
                 MARKER_FILE_NAME
                 );
      if (!EFI_ERROR (Status)) {
        MarkerFileFound = TRUE;
        break;
      }
    }
  }
  return MarkerFileFound;
}

/**

  Create an internal shell's device path for booting.

  @retval NULL                                  Failure.
  @retval Pointer-to-EFI_DEVICE_PATH_PROTOCOL   Success.

**/
EFI_DEVICE_PATH_PROTOCOL *
CreateShellDevicePath (
  VOID
  )
{
  UINTN                             FvHandleCount;
  EFI_HANDLE                        *FvHandleBuffer;
  UINTN                             Index;
  EFI_STATUS                        Status;
  EFI_FIRMWARE_VOLUME2_PROTOCOL     *Fv;
  EFI_FV_FILETYPE                   Type;
  UINTN                             Size;
  EFI_FV_FILE_ATTRIBUTES            Attributes;
  UINT32                            AuthenticationStatus;
  EFI_DEVICE_PATH_PROTOCOL          *DevicePath;
  MEDIA_FW_VOL_FILEPATH_DEVICE_PATH ShellNode;

  FvHandleBuffer    = NULL;
  DevicePath        = NULL;
  Status            = EFI_SUCCESS;

  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiFirmwareVolume2ProtocolGuid,
                  NULL,
                  &FvHandleCount,
                  &FvHandleBuffer
                  );
  if (EFI_ERROR (Status)) {
    goto CreateShellDevicePath_Exit;
  }

  for (Index = 0; Index < FvHandleCount; Index++) {
    Status = gBS->HandleProtocol (
                    FvHandleBuffer[Index],
                    &gEfiFirmwareVolume2ProtocolGuid,
                    (VOID **) &Fv
                    );
    if (EFI_ERROR (Status)) {
      continue;
    }

    Status = Fv->ReadFile (
                   Fv,
                   PcdGetPtr(PcdShellFile),
                   NULL,
                   &Size,
                   &Type,
                   &Attributes,
                   &AuthenticationStatus
                   );
    if (EFI_ERROR (Status)) {
      continue;
    }

    DEBUG ((EFI_D_INFO | EFI_D_LOAD, "Shell is found in FV.\n"));

    Status = gBS->HandleProtocol (
                    FvHandleBuffer[Index],
                    &gEfiDevicePathProtocolGuid,
                    (VOID *) &DevicePath
                    );
    if (EFI_ERROR (Status)) {
      continue;
    }
    //
    // Build the shell device path.
    //
    ShellNode.Header.Type     = MEDIA_DEVICE_PATH;
    ShellNode.Header.SubType  = MEDIA_PIWG_FW_FILE_DP;
    SetDevicePathNodeLength (
      &ShellNode.Header,
      sizeof (MEDIA_FW_VOL_FILEPATH_DEVICE_PATH)
      );
    CopyMem (
      &ShellNode.FvFileName,
      PcdGetPtr(PcdShellFile),
      sizeof (EFI_GUID)
      );
    DevicePath = AppendDevicePathNode (
                   DevicePath,
                   (EFI_DEVICE_PATH_PROTOCOL *) &ShellNode
                   );
    DEBUG ((EFI_D_INFO | EFI_D_LOAD, "Shell's device path is created.\n"));
    break; // bail out.
  }

CreateShellDevicePath_Exit:

  if (FvHandleBuffer) {
    FreePool (FvHandleBuffer);
    FvHandleBuffer = NULL;
  }

  return DevicePath;
}

/**

  Launch the internal EFI shell.

  @param[in] ParentImageHandle  Parent's Image Handle when Loading image.

  @retval EFI_STATUS            Propagating the return value from LoadImage() &
                                StartImage().

**/
EFI_STATUS
LaunchInternalShell (
  EFI_HANDLE ParentImageHandle
  )
{
  EFI_STATUS                Status;
  EFI_HANDLE                ImageHandle;
  EFI_DEVICE_PATH_PROTOCOL  *DevicePath;

  Status        = EFI_SUCCESS;
  DevicePath    = CreateShellDevicePath ();

  if (DevicePath) {

    //
    // The original implementation (Romley) connected all drivers to
    // all controllers at this point.
    //

    Status = gBS->LoadImage (
                    TRUE,
                    ParentImageHandle,
                    DevicePath,
                    NULL,
                    0,
                    &ImageHandle
                    );

    if (!EFI_ERROR(Status)) {
      Status = gBS->StartImage (
                      ImageHandle,
                      0,
                      NULL
                      );
    }
  }

  return Status;
}
