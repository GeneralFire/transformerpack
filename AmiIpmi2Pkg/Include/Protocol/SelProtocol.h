//*************************************************************************
//*************************************************************************
//**                                                                     **
//**        (C)Copyright 1985-2018, American Megatrends, Inc.            **
//**                                                                     **
//**                       All Rights Reserved.                          **
//**                                                                     **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093           **
//**                                                                     **
//**                       Phone: (770)-246-8600                         **
//**                                                                     **
//*************************************************************************
//*************************************************************************

/** @file SelProtocol.h
    Sel protocol structure, macros and function declarations.

*/

#ifndef _SEL_PROTOCOL_H_
#define _SEL_PROTOCOL_H_

//---------------------------------------------------------------------------

#include <Pi/PiStatusCode.h>

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

#define EFI_SM_SEL_PROTOCOL_GUID \
    { \
        0xbc5f861c, 0x86ed, 0x417e, { 0xbb, 0x7b, 0x6c, 0x2, 0x6b, 0xdc, 0x65, 0x23 } \
    }

typedef struct _EFI_SM_SEL_STATUS_CODE_PROTOCOL EFI_SM_SEL_STATUS_CODE_PROTOCOL;

/**
    Convert status code value and extended data to BMC SEL record format and
    send the command to log SEL in BMC.

    @param[in] This         Pointer to the Sel status code protocol.
    @param[in] Value        EFI status code Value.
    @param[in] CodeType     EFI status code type.
    @param[in] Instance     The enumeration of a hardware or software entity
                            within the system. A system may contain multiple
                            entities that match a class/subclass pairing.
                            The instance differentiates between them. An
                            instance of 0 indicates that instance
                            information is unavailable, not meaningful, or
                            not relevant. Valid instance numbers start with 1.
    @param[in] CallerId     This optional parameter may be used to identify
                            the caller. This parameter allows the status code
                            driver to apply different rules to different
                            callers.
    @param[in] Data         This optional parameter may be used to pass
                            additional data.

    @retval EFI_SUCCESS     Status code reported successfully.
    @retval EFI_UNSUPPORTED Unable to report status code due to disabled
                            setup options.
    @retval Others          Error status of function calls used.

**/
typedef
EFI_STATUS
(EFIAPI *EFI_SEL_REPORT_STATUS_CODE) (
  IN EFI_SM_SEL_STATUS_CODE_PROTOCOL     *This,
  IN EFI_STATUS_CODE_TYPE                CodeType,
  IN EFI_STATUS_CODE_VALUE               Value,
  IN UINT32                              Instance,
  IN EFI_GUID                            *CallerId, OPTIONAL
  IN EFI_STATUS_CODE_DATA                *Data OPTIONAL
);

/**
    Sel status code protocol.
*/
struct _EFI_SM_SEL_STATUS_CODE_PROTOCOL {
    /// Report status code.
    EFI_SEL_REPORT_STATUS_CODE    EfiSelReportStatusCode;
};

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

extern EFI_GUID gEfiSelStatusCodeProtocolGuid;

//---------------------------------------------------------------------------

#endif  // #ifndef _SEL_PROTOCOL_H_

//*************************************************************************
//*************************************************************************
//**                                                                     **
//**        (C)Copyright 1985-2018, American Megatrends, Inc.            **
//**                                                                     **
//**                       All Rights Reserved.                          **
//**                                                                     **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093           **
//**                                                                     **
//**                       Phone: (770)-246-8600                         **
//**                                                                     **
//*************************************************************************
//*************************************************************************
