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

/** @file SelPpi.h
    Sel Ppi structure, macros and function declarations.

*/

#ifndef _SEL_PPI_H_
#define _SEL_PPI_H_

//---------------------------------------------------------------------------

#include <Pi/PiStatusCode.h>
#include "IpmiTransportPpi.h"

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

#define EFI_PEI_SEL_PPI_GUID \
    { \
        0xe50b6e5d, 0x359b, 0x4be0, { 0xa0, 0xb0, 0x7, 0x15, 0xa3, 0x1, 0xa6, 0x6 } \
    }

typedef struct _PEI_SEL_STATUS_CODE_PPI PEI_SEL_STATUS_CODE_PPI;

/**
    Convert status code value and extended data to BMC SEL record format and
    send the command to log SEL in BMC.

    @param[in] PeiServices  Pointer to the PEI Core data Structure.
    @param[in] PeiSelPpi    Pointer to the PEI SEL PPI.
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
    @retval EFI_UNSUPPORTED PeiServices is null or unable to report status
                            code due to disabled setup options.
    @retval EFI_NOT_FOUND   Error while filling the Sel record.
    @retval Others          Error status of function calls used.

**/
typedef
EFI_STATUS
(EFIAPI *EFI_PEI_SEL_REPORT_STATUS_CODE) (
  IN CONST  EFI_PEI_SERVICES         **PeiServices,
  IN        PEI_SEL_STATUS_CODE_PPI  *This,
  IN        EFI_STATUS_CODE_TYPE     CodeType,
  IN        EFI_STATUS_CODE_VALUE    Value,
  IN        UINT32                   Instance,
  IN CONST  EFI_GUID                 *CallerId, OPTIONAL
  IN CONST  EFI_STATUS_CODE_DATA     *Data  OPTIONAL
);

/**
    Pei Sel Ppi.
*/
struct _PEI_SEL_STATUS_CODE_PPI {
    /// Log Sel status codes setup option.
    UINT8                             LogSelStatusCodes;
    /// Report Status code.
    EFI_PEI_SEL_REPORT_STATUS_CODE    EfiSelReportStatusCode;
};

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

extern EFI_GUID gEfiPeiSelPpiGuid;

//---------------------------------------------------------------------------

#endif  // #ifndef _SEL_PPI_H_

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
