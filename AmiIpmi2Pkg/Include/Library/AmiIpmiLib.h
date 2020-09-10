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

/** @file AmiIpmiLib.h
    AmiIpmiLib function declarations.

*/

#ifndef _AMI_IPMI_INIT_LIB_H_
#define _AMI_IPMI_INIT_LIB_H_

//---------------------------------------------------------------------------

#include <AmiIpmiBmcCommon.h>

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

/** @internal
    Execute the Get Device ID command and update the response data. Also updates
    the BmcStatus if Bmc is in Force Update Mode.

    @param[in]  IpmiTransport         Pointer to Ipmi transport instance.
    @param[out] GetDeviceIdResponse   Pointer to Get device id command response.

    @retval EFI_INVALID_PARAMETER   Invalid parameter.
    @retval Others                  Return status of function call used.

**/
EFI_STATUS
AmiIpmiGetDeviceId (
  IN  AMI_IPMI_TRANSPORT_INSTANCE   *IpmiTransport,
  OUT IPMI_GET_DEVICE_ID_RESPONSE   *GetDeviceIdResponse
);

/** @internal
    Execute the Get Self test results command for 5 times and updates
    the BmcStatus in IpmiInstance and response data.

    @param[in]  IpmiTransport   Pointer to Ipmi transport instance.
    @param[out] BstStatus       Pointer to get self test results command response.

    @retval EFI_INVALID_PARAMETER   Invalid parameter.
    @retval Others                  Return status of function call used.

**/
EFI_STATUS
AmiIpmiGetSelfTestResults (
  IN  AMI_IPMI_TRANSPORT_INSTANCE       *IpmiTransport,
  OUT IPMI_SELF_TEST_RESULT_RESPONSE    *BstStatus
);

/**
    Gets the list of channel number for all available BMC LAN channels. 
    Lan channels will be updated to PCD PcdLanChannelPtr.

    @param[in]   IpmiTransport        Pointer to Ipmi transport protocol
    @param[in, out]   ChannelBuffer        Buffer to copy available channel number
    @param[in, out]   ChannelCount         Size ChannelBuffer on input and channel count on output

    @retval EFI_INVALID_PARAMETER   Invalid parameter.
    @retval EFI_NOT_FOUND           PCD not found.
    @retval EFI_SUCCESS             GetLanChannelNumber success.
**/
EFI_STATUS
AmiIpmiGetLanChannelNumber (
  IN     AMI_IPMI_TRANSPORT_INSTANCE  *IpimiTransport,
  IN OUT UINT8               *ChannelBuffer,
  IN OUT UINT8               *ChannelCount 
);
//---------------------------------------------------------------------------

#endif  // #ifndef _AMI_IPMI_INIT_LIB_H_

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
