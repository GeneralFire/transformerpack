//**********************************************************************
//*                                                                    *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.  *
//*                                                                    *
//*      All rights reserved. Subject to AMI licensing agreement.      *
//*                                                                    *
//**********************************************************************

/** @file
    Main file that implements the flash platform library for PCH platform.
**/

#include <Uefi.h>

#include "../SpiFlash.h"

EFI_STATUS
GetFlashRegion(
    FLASH_REGION_TYPE   Type,
    UINTN               *Base,
    UINTN               *Limit
);

/**
    Returns location of the system firmware flash region (a.k.a. BIOS) within flash device.

    Platform has to implement this function if non-standard firmware region mapping is employed.
    If this function returns EFI_UNSUPPORTED, flash module uses standard firmware region detection algorithm.

    @param[out] Address of the system firmware flash region
    @param[out] Size of the system firmware flash region
    @param[in] AdressOffset Defines format of the Address.
        TRUE - Address is a CPU address (can only be used for memory mapped regions)
        FALSE - Address is an offset within SPI image

    @retval EFI_SUCCESS Address and Size are updated with flash region information
    @retval EFI_UNSUPPORTED Functionality is not supported.
    @retval EFI_INVALID_PARAMETER Address or Size is NULL
    @retval EFI_DEVICE_ERROR Flash region information cannot be retrieved due to a hardware error
**/
EFI_STATUS
AmiGetSytemFirmwareRegion(
    UINTN   *Address,
    UINTN   *Size,
    BOOLEAN AdressOffset
)
{
    EFI_STATUS  Status;
    UINTN       BiosBase;
    UINTN       BiosLimit;
    UINT32      BiosLength;

    if( (Address == NULL) || (Size == NULL) )
        return EFI_INVALID_PARAMETER;

    Status = GetFlashRegion( FlashRegionBios, &BiosBase, &BiosLimit );
    if( EFI_ERROR(Status) )
        return EFI_DEVICE_ERROR;

    BiosLength = (UINT32)(BiosLimit - BiosBase + 1);
    if( AdressOffset == TRUE )
        *Address = (UINTN)((UINT32)0 - BiosLength);
    else
        *Address = BiosBase;

    *Size = BiosLength;

    return EFI_SUCCESS;
}