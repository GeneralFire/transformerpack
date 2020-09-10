//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2020, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

/** @file AmiCmosLib.c
    Contains routines that are board-specific.

**/

#include <Library/AmiCmosLib.h>

//<AMI_PHDR_START>
//----------------------------------------------------------------------------
//
// Procedure:   ReadWriteCmosBank2
//
// Description: This function is used to access addresses in the CMOS
//              register range (0x80-0xff), for PEI and DXE boot phases.
//
// Input:       **PeiServices       - PEI Services table pointer
//                                    (NULL in DXE phase)
//              AccessType          - ReadType or WriteType to specify the
//                                    type of access
//              CmosRegister        - The CMOS register to access
//              *CmosParameterValue - Pointer to the data variable to be
//                                    accessed
//
// Output:      EFI_STATUS (return value)
//                  EFI_SUCCESS - The access operation was successfull.
//                  Otherwise   - A valid EFI error code is returned.
//
// Modified:    None
//
// Referrals:   IoRead8, IoWrite8
//
// Notes:       This function is used when a translation from logical address
//              to index port value is required.
//----------------------------------------------------------------------------
//<AMI_PHDR_END>

#if CMOS_MANAGER_SUPPORT
EFI_STATUS ReadWriteCmosBank2 (
    IN CONST EFI_PEI_SERVICES         **PeiServices,  // NULL in DXE phase
    IN CMOS_ACCESS_TYPE         AccessType,
    IN UINT16                   CmosRegister,
    IN OUT UINT8                *CmosParameterValue )
{
    if ((CmosRegister < 0x80) || (CmosRegister > 0xff))
        return EFI_INVALID_PARAMETER;

    // Some chipsets require tranlation from the logical CMOS address to a
    // physical CMOS index port value. However, other chipsets do not require
    // a translation and the index/data port can be directly used for
    // accessing the second bank.

    IoWrite8(CMOS_IO_EXT_INDEX, (UINT8)CmosRegister);

    if (AccessType == ReadType) {
        *CmosParameterValue = IoRead8( CMOS_IO_EXT_DATA );
    } else {
        IoWrite8( CMOS_IO_EXT_DATA, *CmosParameterValue );
    }

    return EFI_SUCCESS;
}

/**
    This function is used to clear the battery invalid bit (or set
    the chipset Valid RAM and Time - VRT bit.

    @param None

    @retval None

    @note

**/
#endif  // #if CMOS_MANAGER_SUPPORT

VOID
CrbClearBatteryInvalidBit (VOID)
{
    // PORTING PORTING PORTING
    UINT8   Value;

    IoWrite8(CMOS_BANK1_INDEX, CMOS_RTC_STATUS_REGISTER);
    Value = IoRead8(CMOS_BANK1_DATA);
    Value |= 0x80;
    IoWrite8(CMOS_BANK1_DATA, Value);
    // PORTING PORTING PORTING
}

