//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2017, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************

/**
 * @file ReFlashFmp.c
 * @brief FMP protocol for system firmware
 */

#include <Protocol/FirmwareManagement.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include "ReFlash.h"
#include "ReFlashProgress.h"

extern EFI_GUID SystemFirmwareUpdateClass;
static CHAR16 ReflashImageIdName[] = L"Aptio BIOS";
static CHAR16 ReflashVersionName[] = L"";

EFI_STATUS EFIAPI
ReflashGetImageInfo (
  IN EFI_FIRMWARE_MANAGEMENT_PROTOCOL       *This,
  IN OUT    UINTN                           *ImageInfoSize,
  IN OUT    EFI_FIRMWARE_IMAGE_DESCRIPTOR   *ImageInfo,
  OUT       UINT32                          *DescriptorVersion,
  OUT       UINT8                           *DescriptorCount,
  OUT       UINTN                           *DescriptorSize,
  OUT       UINT32                          *PackageVersion,
  OUT       CHAR16                          **PackageVersionName
  );

EFI_STATUS EFIAPI
ReflashGetImage(
  IN  EFI_FIRMWARE_MANAGEMENT_PROTOCOL  *This,
  IN  UINT8                             ImageIndex,
  IN  OUT  VOID                         *Image,
  IN  OUT  UINTN                        *ImageSize
  )
{
    return EFI_UNSUPPORTED;
}

EFI_STATUS EFIAPI
ReflashSetImage(
  IN  EFI_FIRMWARE_MANAGEMENT_PROTOCOL                 *This,
  IN  UINT8                                            ImageIndex,
  IN  CONST VOID                                       *Image,
  IN  UINTN                                            ImageSize,
  IN  CONST VOID                                       *VendorCode,
  IN  EFI_FIRMWARE_MANAGEMENT_UPDATE_IMAGE_PROGRESS    Progress,
  OUT CHAR16                                           **AbortReason
  );

EFI_STATUS EFIAPI
ReflashCheckImage(
  IN  EFI_FIRMWARE_MANAGEMENT_PROTOCOL  *This,
  IN  UINT8                             ImageIndex,
  IN  CONST VOID                        *Image,
  IN  UINTN                             ImageSize,
  OUT UINT32                            *ImageUpdatable
  )
{
    return EFI_UNSUPPORTED;
}

EFI_STATUS EFIAPI
ReflashGetPackageInfo(
  IN  EFI_FIRMWARE_MANAGEMENT_PROTOCOL *This,
  OUT UINT32                           *PackageVersion,
  OUT CHAR16                           **PackageVersionName,
  OUT UINT32                           *PackageVersionNameMaxLen,
  OUT UINT64                           *AttributesSupported,
  OUT UINT64                           *AttributesSetting
  )
{
    return EFI_UNSUPPORTED;
}

EFI_STATUS EFIAPI
ReflashSetPackageInfo(
  IN  EFI_FIRMWARE_MANAGEMENT_PROTOCOL   *This,
  IN  CONST VOID                         *Image,
  IN  UINTN                              ImageSize,
  IN  CONST VOID                         *VendorCode,
  IN  UINT32                             PackageVersion,
  IN  CONST CHAR16                       *PackageVersionName
  )
{
    return EFI_UNSUPPORTED;
}

EFI_FIRMWARE_MANAGEMENT_PROTOCOL ReflashFmpProtocol = {
    ReflashGetImageInfo,
    ReflashGetImage,
    ReflashSetImage,
    ReflashCheckImage,
    ReflashGetPackageInfo,
    ReflashSetPackageInfo
};

EFI_STATUS EFIAPI
ReflashGetImageInfo (
  IN EFI_FIRMWARE_MANAGEMENT_PROTOCOL       *This,
  IN OUT    UINTN                           *ImageInfoSize,
  IN OUT    EFI_FIRMWARE_IMAGE_DESCRIPTOR   *ImageInfo,
  OUT       UINT32                          *DescriptorVersion,
  OUT       UINT8                           *DescriptorCount,
  OUT       UINTN                           *DescriptorSize,
  OUT       UINT32                          *PackageVersion,
  OUT       CHAR16                          **PackageVersionName
  )
{
    if (ImageInfoSize == NULL)
        return EFI_INVALID_PARAMETER;

    if (*ImageInfoSize < sizeof (EFI_FIRMWARE_IMAGE_DESCRIPTOR)) {
        *ImageInfoSize = sizeof (EFI_FIRMWARE_IMAGE_DESCRIPTOR);
        return EFI_BUFFER_TOO_SMALL;
    }

    SetMem(ImageInfo, *ImageInfoSize, 0);
    CopyMem(&ImageInfo->ImageTypeId, &SystemFirmwareUpdateClass, sizeof (EFI_GUID));
    ImageInfo->ImageIdName = ReflashImageIdName;
    ImageInfo->VersionName = ReflashVersionName;
    ImageInfo->AttributesSupported = ImageInfo->AttributesSetting =
        IMAGE_ATTRIBUTE_IMAGE_UPDATABLE | IMAGE_ATTRIBUTE_RESET_REQUIRED;
    ImageInfo->Size = (UINTN) PcdGet32(PcdRecoveryImageSize); 

    if (DescriptorVersion != NULL)
        *DescriptorVersion = EFI_FIRMWARE_IMAGE_DESCRIPTOR_VERSION;
    if (DescriptorCount != NULL)
        *DescriptorCount = 1;
    if (DescriptorSize != NULL)
        *DescriptorSize = sizeof (EFI_FIRMWARE_IMAGE_DESCRIPTOR);
    if (PackageVersion != NULL)
        *PackageVersion = 0xFFFFFFFF;
    if (PackageVersionName != NULL)
        *PackageVersionName = NULL;

    return EFI_SUCCESS;
}

EFI_STATUS EFIAPI
ReflashSetImage(
  IN  EFI_FIRMWARE_MANAGEMENT_PROTOCOL                 *This,
  IN  UINT8                                            ImageIndex,
  IN  CONST VOID                                       *Image,
  IN  UINTN                                            ImageSize,
  IN  CONST VOID                                       *VendorCode,
  IN  EFI_FIRMWARE_MANAGEMENT_UPDATE_IMAGE_PROGRESS    Progress,
  OUT CHAR16                                           **AbortReason
  )
{
    EFI_STATUS Status;

    fmp_progress.Extra = (progress_uefi *)Progress; 
    RecoveryBuffer = (UINT8 *)Image;
    RecoveryBufferSize = (UINT32)ImageSize;

    Status = Prologue(FALSE, TRUE);
    if(EFI_ERROR(Status))
        gRT->ResetSystem(EfiResetCold, Status, 0, NULL);

    Status = FlashControlFlow(&fmp_progress);
    if(EFI_ERROR(Status))
        gRT->ResetSystem(EfiResetCold, Status, 0, NULL);

    Status = Epilogue(FALSE);
    return Status;
}

EFI_STATUS InstallFmp(VOID)
{
    EFI_STATUS Status;
    EFI_HANDLE Handle = NULL;

    Status = gBS->InstallProtocolInterface(&Handle, 
                    &gEfiFirmwareManagementProtocolGuid,
                    EFI_NATIVE_INTERFACE,
                    &ReflashFmpProtocol);
    
    return Status;
}


//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2017, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************

