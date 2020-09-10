//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************


/** @file AmiIpmiInitLib.c
    This file provides Get device id, self test routines, Get BMC Lan Channel.

*/

//---------------------------------------------------------------------------

#include <Token.h>
#include <Uefi.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Include/AmiIpmiNetFnAppDefinitions.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PcdLib.h>
#include <Library/AmiIpmiLib.h>

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
 
#define LAN_CHANNEL_MEDIUM_TYPE    0x04  // Channel Medium type of 4 denotes 802.3 LAN
#define MAX_CHANNEL_NUM            0x0B  //Maximum channel number (implementation specific) that can be assigned to LAN 

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
  OUT IPMI_GET_DEVICE_ID_RESPONSE   *GetDeviceIdResponse )
{
    EFI_STATUS                  Status;
    UINT8                       ResponseDataSize;
    AMI_IPMI_BMC_INSTANCE_DATA  *IpmiInstance;

    if (!IpmiTransport || !GetDeviceIdResponse) {
        return EFI_INVALID_PARAMETER;
    }
    ResponseDataSize = sizeof (*GetDeviceIdResponse);

    // Get the device ID information for the BMC.
    Status = IpmiTransport->SendIpmiCommand (
                                IpmiTransport,
                                IPMI_NETFN_APP,
                                AMI_BMC_LUN,
                                IPMI_APP_GET_DEVICE_ID,
                                NULL,
                                0,
                                (UINT8*)GetDeviceIdResponse,
                                &ResponseDataSize );
    // If there is no error then proceed to check the data returned by the BMC.
    if (!EFI_ERROR(Status)) {
        /* If the controller is in Update Mode then set the BMC Status to
           indicate the BMC is in force update mode.*/
        if (GetDeviceIdResponse->FirmwareRev1.Bits.UpdateMode!= 0) 
        {
            IpmiInstance = AMI_IPMI_INSTANCE_FROM_EFI_SM_IPMI_BMC_THIS (IpmiTransport);
            IpmiInstance->BmcStatus = AmiBmcStatusUpdateInProgress;
        }
    }
    DEBUG ((DEBUG_INFO, "%a - Get Device ID Command: Status %r\n", __FUNCTION__, Status));

    return Status;
}

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
  OUT IPMI_SELF_TEST_RESULT_RESPONSE    *BstStatus )
{
    EFI_STATUS                  Status;
    UINT8                       Index;
    UINT8                       ResponseDataSize;
    AMI_IPMI_BMC_INSTANCE_DATA  *IpmiInstance;
    
    if (!IpmiTransport || !BstStatus) {
        return EFI_INVALID_PARAMETER;
    }
    
    // Get the self test results.
    for (Index = 0; Index < IPMI_SELF_TEST_COMMAND_RETRY_COUNT; Index++) {

        ResponseDataSize = sizeof (*BstStatus);
        
        Status = IpmiTransport->SendIpmiCommand (
                                    IpmiTransport,
                                    IPMI_NETFN_APP,
                                    AMI_BMC_LUN,
                                    IPMI_APP_GET_SELFTEST_RESULTS,
                                    NULL,
                                    0,
                                    (UINT8 *)BstStatus,
                                    &ResponseDataSize );
        
        DEBUG ((DEBUG_INFO, "Get Self test results status %r\n", Status));
        
        if (!EFI_ERROR(Status)) {
            break;
        }
    }
    
    IpmiInstance = AMI_IPMI_INSTANCE_FROM_EFI_SM_IPMI_BMC_THIS (IpmiTransport);
    
    /*
      If Status indicates a Device error, then the BMC is not responding,
      so send an error.
    */
    if (EFI_ERROR (Status)) {
        IpmiInstance->BmcStatus = AmiBmcStatusHardFail;
    } else {
        /* Check the self test results.  Cases 55h - 58h are Ipmi defined
           test results. Additional Cases are device specific test results.*/
        switch (BstStatus->Result) {
            case IPMI_APP_SELFTEST_NO_ERROR: // 0x55
            case IPMI_APP_SELFTEST_NOT_IMPLEMENTED: // 0x56
            case AMI_IPMI_APP_SELFTEST_RESERVED: // 0xFF
                IpmiInstance->BmcStatus = AmiBmcStatusOk;
                break;

            case IPMI_APP_SELFTEST_ERROR: // 0x57
                IpmiInstance->BmcStatus = AmiBmcStatusSoftFail;
                break;

            default: // 0x58 and Other Device Specific Hardware Error.
                IpmiInstance->BmcStatus = AmiBmcStatusHardFail;
                break;
        }
    }
    return Status;
}

/**
    Gets the list of channel number for all available BMC LAN channels. 
    Lan channels will be updated to PCD PcdLanChannelPtr.
     
    @param   IpmiTransport        Pointer to Ipmi transport protocol
    @param   ChannelBuffer        Buffer to copy available channel number
    @param   ChannelCount         Size ChannelBuffer on input and channel count on output

    @retval EFI_INVALID_PARAMETER   Invalid parameter.
    @retval EFI_NOT_FOUND           PCD not found.
    @retval EFI_SUCCESS             GetLanChannelNumber success.
**/
EFI_STATUS
AmiIpmiGetLanChannelNumber (
  IN     AMI_IPMI_TRANSPORT_INSTANCE  *IpmiTransport,
  IN OUT UINT8               *ChannelBuffer,
  IN OUT UINT8               *ChannelCount 
  )
{
    EFI_STATUS                       Status;
    IPMI_GET_CHANNEL_INFO_RESPONSE   GetChannelInfoResponse;
    UINT8                            ResponseDataSize;
    UINT8                            *PcdLanChannelBufferPtr;
    UINT8                            PcdSize;
    UINT8                            ChannelNumber = 0 ;
    UINT8                            Index = 0;

    if (!IpmiTransport)
        return EFI_INVALID_PARAMETER;

    if (!ChannelBuffer || !ChannelCount || !*ChannelCount)
        return EFI_INVALID_PARAMETER;

    //Get PcdLanChannelPtr and size
    PcdLanChannelBufferPtr = PcdGetPtr(AmiPcdLanChannelPtr);
    PcdSize = (UINT8)PcdGetSize(AmiPcdLanChannelPtr);

    if (!PcdLanChannelBufferPtr || !PcdSize){
        return EFI_NOT_FOUND;
    }

    //Update Pcd if available channel number is not updated for this platform.
    //Pcd buffer should have at least one valid entry. Channel number 0 for Primary IPMB
    if (!PcdLanChannelBufferPtr[0]) {

        // Sends the get channel info command for all available BMC channel numbers
        for (ChannelNumber = 1; ChannelNumber <= MAX_CHANNEL_NUM; ChannelNumber++) {

            ResponseDataSize = sizeof (GetChannelInfoResponse);

            Status = IpmiTransport->SendIpmiCommand (
                                        IpmiTransport,
                                        IPMI_NETFN_APP,
                                        AMI_BMC_LUN,
                                        IPMI_APP_GET_CHANNEL_INFO,
                                        &ChannelNumber,
                                        sizeof (ChannelNumber),
                                        (UINT8 *)&GetChannelInfoResponse,
                                        &ResponseDataSize);
            DEBUG ((DEBUG_INFO, " %a:%r ChannelNo: %d, CompletionCode: %x\n", __FUNCTION__, Status, GetChannelInfoResponse.ChannelNumber.Bits.ChannelNo, GetChannelInfoResponse.CompletionCode));

            if (EFI_ERROR(Status))
                continue;

            // check for completion code and channel medium type is 802.3 LAN Ethernet.
            DEBUG ((DEBUG_INFO, "%a: Channel Medium Type: %x\n", __FUNCTION__, GetChannelInfoResponse.MediumType.Bits.ChannelMediumType));
            if (!GetChannelInfoResponse.CompletionCode && (GetChannelInfoResponse.MediumType.Bits.ChannelMediumType == LAN_CHANNEL_MEDIUM_TYPE)) {
                //If maximum LAN count is not reached, store the LAN channel number into buffer
                if (Index < PcdSize) {
                    PcdLanChannelBufferPtr[Index] = ChannelNumber;
                    Index ++;
                }
            }
        }
    }

    if (!Index && (ChannelNumber > MAX_CHANNEL_NUM)){
        return EFI_NOT_FOUND;
    }
    //If request buffer size is greater than Pcd Buffer size, update with actual buffer size.
    //If requested buffer size is lesser than Pcd Buffer size, copy only requested buffer size.
    if (*ChannelCount > PcdSize)
        *ChannelCount = PcdSize;

    CopyMem (ChannelBuffer, PcdLanChannelBufferPtr, *ChannelCount);
    return EFI_SUCCESS;
}

