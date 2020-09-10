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

/** @file AmiSelLib.h
    Sel library function declarations.

*/

#ifndef _AMI_SEL_LIB_H_
#define _AMI_SEL_LIB_H_

//---------------------------------------------------------------------------

#include <AmiStatusCodeConversion.h>
#include <AmiIpmiBmcCommon.h>
#include <AmiIpmiNetFnStorageDefinitions.h>

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

/** @internal
    This function walks through the table based upon Value field of
    Status Code and extract EFI status code to SEL conversion entry.

    @param[in] EfiStatusCode    EFI status code value.
    @param[in] StatusCodeType   EFI status code type.

    @return AMI_STATUS_CODE_TO_SEL_ENTRY*   Pointer to conversion entry if
                                            found. Else return NULL.

**/
AMI_STATUS_CODE_TO_SEL_ENTRY*
AmiIpmiConvertEfiStatusCodeToSelEntry (
  IN EFI_STATUS_CODE_VALUE  EfiStatusCode,
  IN EFI_STATUS_CODE_TYPE   StatusCodeType
);

/** @internal
    Get Reservation Id to access Sel area. 

    @param[in] IpmiTransport    Pointer to Ipmi transport instance.
    @param[out] ReserveSelResponse  Contains Sel reservation id.

    @retval EFI_INVALID_PARAMETER   Invalid parameter.
    @retval Others                  Return status of function call used.

**/
EFI_STATUS
AmiIpmiGetReservationId (
  IN  AMI_IPMI_TRANSPORT_INSTANCE   *IpmiTransport,
  OUT AMI_RESERVE_SEL_RESPONSE      *ReserveSelResponse
);

/** @internal
    This function checks for the erasure status.

    @param[in] IpmiTransport    Pointer to Ipmi transport instance.
    @param[in, out] ReserveSelResponse  Contains Sel reservation id.

    @retval EFI_SUCCESS             Sel is erased successfully.
    @retval EFI_NO_RESPONSE         If time out occurred and still Sel is not erased
                                    successfully.
    @retval EFI_INVALID_PARAMETER   Invalid parameter.
    @retval Others                  Error status returned from send ipmi command.

**/
EFI_STATUS
AmiIpmiWaitTillErased (
  IN     AMI_IPMI_TRANSPORT_INSTANCE   *IpmiTransport,
  IN OUT AMI_RESERVE_SEL_RESPONSE       *ReserveSelResponse
);

/** @internal
    This function erases/Delete the SEL entries.

    @param[in] IpmiTransport    Pointer to Ipmi transport instance.
    @param[in] Eraseoption      Erase(1) or delete(2) sel entry.

    @retval EFI_SUCCESS             SEL is erased successfully.
    @retval EFI_INVALID_PARAMETER   Invalid parameter.
    @retval EFI_NO_RESPONSE         If time out occurred or still SEL is not erased
                                    successfully.

**/
EFI_STATUS
AmiIpmiEraseSelElogData (
  IN AMI_IPMI_TRANSPORT_INSTANCE   *IpmiTransport,
  IN UINT8                         Eraseoption
);

/** @internal
    This function verifies if the BMC SEL is full and provide delete SEL support information.

    @param[in]  IpmiTransport    Pointer to Ipmi transport instance.
    @param[out] Delsupport       Delete support

    @retval TRUE    If Sel is full.
    @retval FALSE   If Sel is not full or Invalid parameter.

**/ 
BOOLEAN
AmiIpmiIsSelFull (
  IN     AMI_IPMI_TRANSPORT_INSTANCE   *IpmiTransport,
  IN OUT BOOLEAN                       *Delsupport
);

/** @internal
    This function fills the event format and logs the event if the status
    code is found in table.

    @param[in]  IpmiTransport   Pointer to Ipmi transport instance.
    @param[in]  Value           EFI status code Value.
    @param[in]  CodeType        EFI status code type.
    @param[out] EventData       Pointer to event data.

    @retval EFI_INVALID_PARAMETER   Invalid parameter.
    @retval EFI_DEVICE_ERROR        If status code is not found in the
                                    conversion table.
    @retval Others                  Status returned by function calls used.

**/
EFI_STATUS
AmiIpmiFillAndLogEventRecord (
  IN  AMI_IPMI_TRANSPORT_INSTANCE   *IpmiTransport,
  IN  EFI_STATUS_CODE_VALUE         Value,
  IN  EFI_STATUS_CODE_TYPE          CodeType,
  OUT VOID                          *EventData
);

//---------------------------------------------------------------------------

#endif  // #ifndef _AMI_SEL_LIB_H_

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
