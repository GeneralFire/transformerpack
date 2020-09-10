//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************


/** @file AmiIpmiBootLib.c
    This file provides Wait for set in progress, Get boot flags etc using
    Get system boot options command.

*/

//---------------------------------------------------------------------------

#include <Token.h>

#include <Library/UefiBootServicesTableLib.h>
#include <Library/AmiIpmiBootLib.h>

//---------------------------------------------------------------------------

/** @internal
    Wait for Get System Boot Option Set-in-progress bit to clear.

    @param[in]  IpmiTransport   Pointer to Ipmi transport instance.

    @retval EFI_SUCCESS             Set in progress clear successful.
    @retval EFI_TIMEOUT             Set in progress clear failed.
    @retval EFI_INVALID_PARAMETER   Invalid parameter.
    @retval Others                  Error status returned form send ipmi command.

**/
EFI_STATUS
AmiIpmiGetSysBootOptionsWaitSetInProgressClear (
  IN AMI_IPMI_TRANSPORT_INSTANCE    *IpmiTransport )
{
    EFI_STATUS                                      Status;
    UINT8                                           ResponseDataSize;
    UINT8                                           Retries;
    IPMI_GET_BOOT_OPTIONS_REQUEST                   GetRequest;
    AMI_GET_BOOT_OPTIONS_SET_IN_PROGRESS_RESPONSE   GetSetInProgResponse;

    if (!IpmiTransport) {
        return EFI_INVALID_PARAMETER;
    }
    GetRequest.ParameterSelector.Bits.ParameterSelector = AmiBootOptionParamSetInProgress;
    GetRequest.ParameterSelector.Bits.Reserved          = 0x0;
    GetRequest.SetSelector          = AMI_IPMI_SELECTOR_NONE;
    GetRequest.BlockSelector        = AMI_IPMI_SELECTOR_NONE;

    // Wait for in-progress bit to clear.
    for (Retries = 0; Retries < AMI_IPMI_GET_SET_IN_PROGRESS_RETRY_COUNT; Retries++) {
        ResponseDataSize = sizeof (GetSetInProgResponse);
        Status = IpmiTransport->SendIpmiCommand (
                                    IpmiTransport,
                                    IPMI_NETFN_CHASSIS,
                                    AMI_BMC_LUN,
                                    IPMI_CHASSIS_GET_SYSTEM_BOOT_OPTIONS,
                                    (UINT8 *)&GetRequest,
                                    sizeof (GetRequest),
                                    (UINT8 *)&GetSetInProgResponse,
                                    &ResponseDataSize );
        if (EFI_ERROR (Status)) {
            return Status;
        }
        // Check for SetInProgress bit.
        if (GetSetInProgResponse.Param0.Bits.SetInProgress == AMI_IPMI_BIT_CLEAR)
            return EFI_SUCCESS;
        // Give some delay before sending the command again.
        gBS->Stall(AMI_IPMI_STALL);
    }
    // Return time out after retires.
    return EFI_TIMEOUT;
}

/** @internal
    Send the Get system boot options command to get the boot flags parameter.

    @param[in]  IpmiTransport       Pointer to Ipmi transport instance.
    @param[out] BootFlagsResponse   Pointer to get the boot flags response.

    @retval EFI_INVALID_PARAMETER   Invalid parameter.
    @retval Others                  Return status of function call used.

**/
EFI_STATUS
AmiIpmiGetBootFlags (
  IN  AMI_IPMI_TRANSPORT_INSTANCE               *IpmiTransport,
  OUT AMI_GET_BOOT_OPTIONS_BOOT_FLAG_RESPONSE   *BootFlagsResponse )
{
    EFI_STATUS                      Status;
    UINT8                           ResponseDataSize;
    IPMI_GET_BOOT_OPTIONS_REQUEST   GetRequest;

    if (!IpmiTransport || !BootFlagsResponse) {
        return EFI_INVALID_PARAMETER;
    }
    // Check the status of SetInProgress bit.
    Status = AmiIpmiGetSysBootOptionsWaitSetInProgressClear (IpmiTransport);
    if (!EFI_ERROR (Status)) {
        ResponseDataSize = sizeof (AMI_GET_BOOT_OPTIONS_BOOT_FLAG_RESPONSE);

        GetRequest.ParameterSelector.Bits.ParameterSelector = AmiBootOptionParamBootFlags;
        GetRequest.ParameterSelector.Bits.Reserved          = 0x0;
        GetRequest.SetSelector          = AMI_IPMI_SELECTOR_NONE;
        GetRequest.BlockSelector        = AMI_IPMI_SELECTOR_NONE;
    
        Status = IpmiTransport->SendIpmiCommand (
                                    IpmiTransport,
                                    IPMI_NETFN_CHASSIS,
                                    AMI_BMC_LUN,
                                    IPMI_CHASSIS_GET_SYSTEM_BOOT_OPTIONS,
                                    (UINT8*) &GetRequest,
                                    sizeof (GetRequest),
                                    (UINT8*) BootFlagsResponse,
                                    &ResponseDataSize );
    }
    return Status;
}

/** @internal
    This function reads the Boot Info Acknowledge data bytes from Bmc.

    @param[in]  IpmiTransport           Pointer to Ipmi transport instance.
    @param[out] BootInfoAckResponse     Pointer to return Boot Info Acknowledge.

    @retval EFI_INVALID_PARAMETER   Invalid parameter.
    @retval Others                  Return status of function call used.

**/
EFI_STATUS
AmiIpmiGetBootInfoAcknowledgeData (
  IN  AMI_IPMI_TRANSPORT_INSTANCE                   *IpmiTransport,
  OUT AMI_GET_BOOT_OPTIONS_BOOT_INFO_ACK_RESPONSE   *BootInfoAckResponse )
{
    EFI_STATUS                              Status;
    UINT8                                   ResponseDataSize;
    IPMI_GET_BOOT_OPTIONS_REQUEST           GetRequest;

    if (!IpmiTransport || !BootInfoAckResponse) {
        return EFI_INVALID_PARAMETER;
    }
    
    // Wait until Set In Progress field is cleared.
    Status = AmiIpmiGetSysBootOptionsWaitSetInProgressClear (IpmiTransport);
    if (!EFI_ERROR (Status)) {
        ResponseDataSize = sizeof (AMI_GET_BOOT_OPTIONS_BOOT_INFO_ACK_RESPONSE);
        GetRequest.ParameterSelector.Bits.ParameterSelector = AmiBootOptionParamBootInfoAck;
        GetRequest.ParameterSelector.Bits.Reserved          = 0x0;
        GetRequest.SetSelector          = AMI_IPMI_SELECTOR_NONE;
        GetRequest.BlockSelector        = AMI_IPMI_SELECTOR_NONE;
    
        Status = IpmiTransport->SendIpmiCommand (
                                    IpmiTransport,
                                    IPMI_NETFN_CHASSIS,
                                    AMI_BMC_LUN,
                                    IPMI_CHASSIS_GET_SYSTEM_BOOT_OPTIONS,
                                    (UINT8 *)&GetRequest,
                                    sizeof (GetRequest),
                                    (UINT8 *)BootInfoAckResponse,
                                    &ResponseDataSize );
    }
    return Status;
}


