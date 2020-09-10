//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************


/** @file SerialMuxControl.c
    Functions to switch the serial port MUX.

*/

//---------------------------------------------------------------------------

#include <Token.h>
#include <AmiIpmiNetFnTransportDefinitions.h>
#include <AmiIpmiNetFnChassisDefinitions.h>
#include <AmiServerMgmtSetupVariable.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Protocol/IpmiTransportProtocol.h>
#include <Library/BaseMemoryLib.h>

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

#define MAX_IPMI_CHANNELS       0x10
#define REQUEST_ACCEPTED        0x01
#define REQUEST_REJECTED        0x00
#define REQUEST_TO_SYSTEM       0x00
#define FORCE_TO_BMC            0x01
#define FORCE_TO_SYSTEM         0x02

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

EFI_IPMI_TRANSPORT      *gIpmiTransport = NULL;
UINT8                   gSerialChannel = 0;

//---------------------------------------------------------------------------

/** @internal
    Switch the serial port MUX.

    @param[in] MuxSetting   MUX Setting.

    @retval EFI_ABORTED     Ipmi command failure.
    @return Others          Return status of send ipmi command.

**/
EFI_STATUS
SwitchSerialMux (
  IN AMI_MUX_SETTING    MuxSetting )
{
    EFI_STATUS                                  Status;
    IPMI_SET_SERIAL_MODEM_MUX_COMMAND_REQUEST   SetMux;
    AMI_SET_SERIAL_MODEM_MUX_RESPONSE           MuxConfig;
    UINT32                                      ResponseDataSize;
    UINT8                                       RetryCount = 0;

    DEBUG ((DEBUG_INFO, "%a Entry... MuxSetting: %x \n", __FUNCTION__, MuxSetting));
    SetMux.ChannelNumber.Bits.ChannelNo = gSerialChannel;
    SetMux.MuxSetting.Bits.MuxSetting = MuxSetting;
    SetMux.ChannelNumber.Bits.Reserved = 0x00;
    SetMux.MuxSetting.Bits.Reserved = 0x00;

    ResponseDataSize = sizeof (MuxConfig);

    // Set serial MUX command.
    Status = gIpmiTransport->SendIpmiCommand (
                                gIpmiTransport,
                                IPMI_NETFN_TRANSPORT,
                                AMI_BMC_LUN,
                                IPMI_TRANSPORT_SET_SERIAL_MUX,
                                (UINT8 *) &SetMux,
                                sizeof (SetMux),
                                (UINT8 *) &MuxConfig,
                                (UINT8 *) &ResponseDataSize );
    DEBUG ((DEBUG_INFO, " IPMI_TRANSPORT_SET_SERIAL_MUX Status: %r CommandStatus: %x \n", Status, MuxConfig.MuxSetting.MuxSetting.Bits.CommandStatus));
    if (!EFI_ERROR(Status) && ((MuxConfig.CompletionCode) || (MuxConfig.MuxSetting.MuxSetting.Bits.CommandStatus == REQUEST_REJECTED))) {
        while (RetryCount++ < 5) {
            // Try for five times, if the command still fails then return EFI_ABORTED.
            ResponseDataSize = sizeof (MuxConfig);
            // Set serial MUX command.
            Status = gIpmiTransport->SendIpmiCommand (
                                        gIpmiTransport,
                                        IPMI_NETFN_TRANSPORT,
                                        AMI_BMC_LUN,
                                        IPMI_TRANSPORT_SET_SERIAL_MUX,
                                        (UINT8 *) &SetMux,
                                        sizeof (SetMux),
                                        (UINT8 *) &MuxConfig,
                                        (UINT8 *) &ResponseDataSize );
            DEBUG ((DEBUG_INFO, " RetryCount: %x IPMI_TRANSPORT_SET_SERIAL_MUX Status: %r CommandStatus: %x \n", RetryCount, Status, MuxConfig.MuxSetting.MuxSetting.Bits.CommandStatus));
            if (!EFI_ERROR(Status) && (!MuxConfig.CompletionCode) && (MuxConfig.MuxSetting.MuxSetting.Bits.CommandStatus == REQUEST_ACCEPTED)){
                return Status;
            }
        }
        return EFI_ABORTED;
    }
    return Status;
}

/**
    Notification callback to switch the MUX while ready to boot to
    OS. Switching MUX depends on the BIOS MUX Control bits.

    @param[in] Event    Event which caused this handler.
    @param[in] Context  Context passed during Event Handler registration.

    @return VOID    Nothing.

**/
VOID
EFIAPI
OnBootSharedModeMuxSwitching (
  IN EFI_EVENT  Event,
  IN VOID       *Context )
{
    EFI_STATUS                                  Status;
    AMI_GET_BOOT_OPTIONS_BOOT_FLAG_RESPONSE     BootFlagsResponse;
    UINT8                                       ResponseDataSize;
    IPMI_GET_BOOT_OPTIONS_REQUEST               GetRequest;

    // Read Boot options command request.
    GetRequest.ParameterSelector.Bits.ParameterSelector    = AmiBootOptionParamBootFlags;
    GetRequest.ParameterSelector.Bits.Reserved             = 0;
    GetRequest.SetSelector          = AMI_IPMI_SELECTOR_NONE;
    GetRequest.BlockSelector        = AMI_IPMI_SELECTOR_NONE;
    ResponseDataSize                = sizeof (BootFlagsResponse);
    
    Status = gIpmiTransport->SendIpmiCommand (
                                gIpmiTransport,
                                IPMI_NETFN_CHASSIS,
                                AMI_BMC_LUN,
                                IPMI_CHASSIS_GET_SYSTEM_BOOT_OPTIONS,
                                (UINT8*) &GetRequest,
                                sizeof (GetRequest),
                                (UINT8*) &BootFlagsResponse,
                                &ResponseDataSize );
    
    DEBUG ((DEBUG_INFO, " IPMI_CHASSIS_GET_SYSTEM_BOOT_OPTIONS Status: %r\n", Status));
    
    if (!EFI_ERROR (Status)) {
        DEBUG ((DEBUG_INFO, " BootFlagValid: %x BiosMuxCtrlOverride: %x \n", BootFlagsResponse.Param5.BootFlagValid, BootFlagsResponse.Param5.BiosMuxCtrlOverride));
    
        /* Check for Boot Flag valid bit. Based on BiosMuxCtrlOverride bit,
           Set MUX accordingly.*/
        if (BootFlagsResponse.Param5.BootFlagValid) {
            if (BootFlagsResponse.Param5.BiosMuxCtrlOverride == REQUEST_TO_SYSTEM) {
                SwitchSerialMux (AmiReqMuxToSystem);
            } else if (BootFlagsResponse.Param5.BiosMuxCtrlOverride == FORCE_TO_BMC) {
                SwitchSerialMux (AmiForceMuxToBmc);
            } else if (BootFlagsResponse.Param5.BiosMuxCtrlOverride == FORCE_TO_SYSTEM) {
                SwitchSerialMux (AmiForceMuxToSystem);
            }
        }
    }
}

/**
    Notification callback to switch the MUX to system while ready to boot to
    OS.

    @param[in] Event    Event which caused this handler.
    @param[in] Context  Context passed during Event Handler registration.

    @return VOID    Nothing.

**/
VOID
EFIAPI
OnBootForceMuxToSystem (
  IN EFI_EVENT  Event,
  IN VOID       *Context )
{
    SwitchSerialMux (AmiForceMuxToSystem);
}

/** @internal
    Control the serial MUX to System or BMC depending on Channel Access Mode.

    @param VOID     Nothing.

    @retval EFI_DEVICE_ERROR    Error in send ipmi command.
    @retval EFI_SUCCESS         Serial MUX switched to system successfully.

**/
EFI_STATUS
SerialMuxSwitching (
  VOID )
{
    EFI_STATUS                          Status;
    IPMI_GET_CHANNEL_ACCESS_REQUEST     CommandData;
    UINT32                              ResponseDataSize;
    IPMI_GET_CHANNEL_ACCESS_RESPONSE    ResponseData;
    EFI_EVENT                           ReadyToBootEvent;

    ResponseDataSize = sizeof (ResponseData);
    CommandData.ChannelNumber.Bits.ChannelNo = gSerialChannel;
    CommandData.AccessType.Bits.MemoryType = AmiChannelAccessTypeNonVolatile;
    CommandData.ChannelNumber.Bits.Reserved = 0x00;
    CommandData.AccessType.Bits.Reserved = 0x00;

    // Get Channel Access command.
    Status = gIpmiTransport->SendIpmiCommand (
                                gIpmiTransport,
                                IPMI_NETFN_APP,
                                AMI_BMC_LUN,
                                IPMI_APP_GET_CHANNEL_ACCESS,
                                (UINT8 *) &CommandData,
                                sizeof (CommandData),
                                (UINT8 *) &ResponseData,
                                (UINT8 *) &ResponseDataSize );
    
    DEBUG ((DEBUG_INFO, " IPMI_APP_GET_CHANNEL_ACCESS Status: %r CompletionCode: %x \n", Status, ResponseData.CompletionCode));
    
    if (EFI_ERROR (Status) || (ResponseData.CompletionCode)) {
        return EFI_DEVICE_ERROR;
    }
    DEBUG ((DEBUG_INFO, " IPMI_APP_GET_CHANNEL_ACCESS  AccessMode: %x \n", ResponseData.ChannelAccess.Bits.AccessMode));
    switch (ResponseData.ChannelAccess.Bits.AccessMode) 
    {
        // At power-on by default MUX is Set to System.
        case AmiChannelAccessModeDisabled:
            break;

        case AmiChannelAccessModePreBootOnly:
            // Switch MUX for system use.
            SwitchSerialMux (AmiReqMuxToSystem);

            // Switch the Serial MUX to the system before Boot to OS.
            Status = EfiCreateEventReadyToBootEx (
                        TPL_CALLBACK,
                        (EFI_EVENT_NOTIFY)OnBootForceMuxToSystem,
                        NULL,
                        &ReadyToBootEvent );
            break;

        case AmiChannelAccessModeAlwaysAvailable:
            /* TODO: In order to avoid confusion with run-time software, BIOS will
               typically hide or disable serial port when the OS load process starts.*/
            break;

        case AmiChannelAccessModeShared:
            // Switch MUX for system use.
            SwitchSerialMux (AmiReqMuxToSystem);

            // Switch the Serial MUX depends on the BootFlag before OS Boot.
            Status = EfiCreateEventReadyToBootEx (
                        TPL_CALLBACK,
                        (EFI_EVENT_NOTIFY)OnBootSharedModeMuxSwitching,
                        NULL,
                        &ReadyToBootEvent );
            break;
        default:
            break;
    }

    return EFI_SUCCESS;
}

/** @internal
    Get the Serial MUX channel.

    @param VOID     Nothing.

    @retval EFI_SUCCESS     Serial Mux channel is found.
    @retval EFI_NOT_FOUND   Serial Mux channel is not found.

**/
EFI_STATUS
GetChannelInfo (
  VOID )
{
    EFI_STATUS                        Status;
    IPMI_GET_CHANNEL_INFO_RESPONSE    ChannelInfo;
    UINT8                             ChannelNumber = 0;
    UINT8                             ResponseDataSize;

    for (ChannelNumber = 0; ChannelNumber < MAX_IPMI_CHANNELS; ChannelNumber++) {
        DEBUG ((DEBUG_INFO, "Channel Number: %x \n", ChannelNumber));

        /* According to Get channel info command description, 0x0E is used to get
           information about the channel this command is being executed from.
           So do not send command for 0x0E channel number.*/
        if (ChannelNumber == 0xE) {
            continue;
        }
        ResponseDataSize = sizeof (ChannelInfo);

        // Get Channel Info command.
        Status = gIpmiTransport->SendIpmiCommand (
                                    gIpmiTransport,
                                    IPMI_NETFN_APP,
                                    AMI_BMC_LUN,
                                    IPMI_APP_GET_CHANNEL_INFO,
                                    &ChannelNumber,
                                    sizeof (ChannelNumber),
                                    (UINT8 *)&ChannelInfo,
                                    &ResponseDataSize );
        
        DEBUG ((DEBUG_INFO, " IPMI_APP_GET_CHANNEL_INFO Status: %r CompletionCode: %x \n", Status, ChannelInfo.CompletionCode));
        
        if (!EFI_ERROR (Status) && (!ChannelInfo.CompletionCode)) {
            // Check for Serial channel number.
            DEBUG ((DEBUG_INFO, " Channel MediumType: %x \n", ChannelInfo.MediumType.Bits.ChannelMediumType));
            if (ChannelInfo.MediumType.Bits.ChannelMediumType == AmiChannelMediumTypeRs_232) {
                gSerialChannel = ChannelInfo.ChannelNumber.Bits.ChannelNo;
                DEBUG ((DEBUG_INFO, " Serial Channel Number: %x \n", gSerialChannel));
                return EFI_SUCCESS;
            }
        }
    }
    return EFI_NOT_FOUND;
}

/**
    Get the channel information and control the MUX based on the
    Channel Access Mode.

    @param[in] ImageHandle  Handle of this driver image.
    @param[in] SystemTable  Table containing standard EFI services.

    @retval EFI_SUCCESS     Driver is initialized successfully.
    @retval EFI_UNSUPPORTED Error status returned by the functions used.

**/
EFI_STATUS
EFIAPI
SerialMuxControlEntryPoint (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable )
{
    EFI_STATUS                      Status;
    SERVER_MGMT_CONFIGURATION_DATA  ServerMgmtConfiguration;
    UINTN                           ServerMgmtConfigurationSize;

    DEBUG ((DEBUG_INFO, "%a Entry... \n", __FUNCTION__));

    ServerMgmtConfigurationSize = sizeof (ServerMgmtConfiguration);
    Status = gRT->GetVariable (
                    L"ServerSetup",
                    &gEfiServerMgmtSetupVariableGuid,
                    NULL,
                    &ServerMgmtConfigurationSize,
                    &ServerMgmtConfiguration );
    
    DEBUG ((DEBUG_INFO, " gEfiServerMgmtSetupVariableGuid Status: %r \n", Status));
    DEBUG ((DEBUG_INFO, " ServerMgmtConfiguration.SerialMux: %X \n", ServerMgmtConfiguration.SerialMux));

    if (EFI_ERROR(Status) || (!ServerMgmtConfiguration.SerialMux)) {
        return EFI_UNSUPPORTED;
    }

    Status = gBS->LocateProtocol (
                    &gEfiDxeIpmiTransportProtocolGuid,
                    NULL,
                    (VOID **)&gIpmiTransport );
    
    DEBUG ((DEBUG_INFO, " gEfiDxeIpmiTransportProtocolGuid Status: %r \n", Status));
    
    if (EFI_ERROR (Status)) {
        return EFI_UNSUPPORTED;
    }

    // Get Channel information.
    Status = GetChannelInfo();

    if (!EFI_ERROR(Status)) {
        // Depends on the Channel Access Mode, control the MUX.
        SerialMuxSwitching ();
    } else {
        Status = EFI_UNSUPPORTED;
    }
    DEBUG ((DEBUG_INFO, "%a Exiting... \n", __FUNCTION__));
    return Status;
}


