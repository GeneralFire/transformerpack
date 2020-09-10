//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2020, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************


/** @file Ip6BmcLanConfig.c
    This driver is used to configure IPV6 BMC LAN. And also to get these
    parameters from a DHCP server or as user input from BIOS setup.

*/

//---------------------------------------------------------------------------

#include <Library/NetLib.h>
#include "BmcLanConfigCommon.h"

//---------------------------------------------------------------------------

#define PARAMETER_NOT_SUPPORTED     0x80
#define IPV6_SUPPORT_DISABLED       0
#define IPV6_SUPPORTED              1
#define IPV6_NOT_SUPPORTED          2

EFI_IPMI_TRANSPORT                  *gIpmiTransport;
SERVER_MGMT_CONFIGURATION_DATA      gServerMgmtConfiguration;

//---------------------------------------------------------------------------

/** @internal
    Set the LAN IPV6 address source values from BIOS setup to BMC.

    @param[in] LanChannelNumber     LAN channel to use.

    @param[in] IsStaticSource       Selection of source as static or dynamic.

    @return EFI_STATUS      Return status of the function calls used.

**/
EFI_STATUS
SetIp6AddressSource (
  IN UINT8      LanChannelNumber,
  IN UINT8      IsStaticSource )
{
    EFI_STATUS                                      Status;
    AMI_GET_LAN_CONFIG_PARAM_REQUEST                GetParamCommand;
    AMI_SET_LAN_CONFIG_PARAM_IPV6_ADDRESS_REQUEST   SetParamCommand;
    AMI_GET_LAN_CONFIG_PARAM_IPV6_ADDRESS_RESPONSE  Ipv6AddressData;
    UINT8                                           CommandDataSize;
    UINT8                                           ResponseDataSize;
    UINT8                                           CompletionCode;

    DEBUG ((DEBUG_INFO, "%a Entered... \n", __FUNCTION__));

    // Get IPV6 address.
    GetParamCommand.LanChannel.ChannelNumber    = LanChannelNumber;
    GetParamCommand.LanChannel.GetParam         = 0;
    GetParamCommand.LanChannel.Reserved         = 0;
    GetParamCommand.ParameterSelector           = AmiIpmiIpv6StaticAddress;
    GetParamCommand.BlockSelector               = AMI_IPMI_SELECTOR_NONE;
    GetParamCommand.SetSelector                 = AMI_IPMI_SELECTOR_NONE;
    ResponseDataSize                            = sizeof (Ipv6AddressData);

    // Wait until Set In Progress field is cleared.
    Status = IpmiWaitSetInProgressClear (
                gIpmiTransport,
                LanChannelNumber );
    if (EFI_ERROR(Status)) {
       return Status;
    }

    Status = gIpmiTransport->SendIpmiCommand (
                                gIpmiTransport,
                                IPMI_NETFN_TRANSPORT,
                                AMI_BMC_LUN,
                                IPMI_TRANSPORT_GET_LAN_CONFIG_PARAMETERS,
                                (UINT8 *)&GetParamCommand,
                                sizeof (GetParamCommand),
                                (UINT8 *)&Ipv6AddressData,
                                &ResponseDataSize );

    DEBUG ((DEBUG_INFO, " IPMI : IPMI_TRANSPORT_GET_LAN_CONFIG_PARAMETERS Status: %r\n", Status));

    if (EFI_ERROR(Status)) {
        return Status;
    }

    SetParamCommand.LanChannel.ChannelNumber    = LanChannelNumber;
    SetParamCommand.LanChannel.Reserved         = 0;
    SetParamCommand.ParameterSelector           = AmiIpmiIpv6StaticAddress;
    SetParamCommand.Ip6AddressParam.SetSelector = Ipv6AddressData.Ip6AddressParam.SetSelector;
    SetParamCommand.Ip6AddressParam.AddressSource.EnableStaticSource = IsStaticSource;
    SetParamCommand.Ip6AddressParam.AddressSource.Reserved = 0;
    SetParamCommand.Ip6AddressParam.AddressSource.Type = 0;

    CopyMem (
        SetParamCommand.Ip6AddressParam.Ipv6Address.Addr,
        Ipv6AddressData.Ip6AddressParam.Ipv6Address.Addr,
        sizeof (Ipv6AddressData.Ip6AddressParam.Ipv6Address.Addr) );
    SetParamCommand.Ip6AddressParam.AddressPrefixLen = Ipv6AddressData.Ip6AddressParam.AddressPrefixLen;

    CommandDataSize = sizeof (SetParamCommand);

    // Wait until Set In Progress field is cleared.
    Status = IpmiWaitSetInProgressClear (
                gIpmiTransport,
                LanChannelNumber );
    if (EFI_ERROR(Status)) {
       return Status;
    }

    ResponseDataSize = sizeof (CompletionCode);
    Status = gIpmiTransport->SendIpmiCommand (
                                gIpmiTransport,
                                IPMI_NETFN_TRANSPORT,
                                AMI_BMC_LUN,
                                IPMI_TRANSPORT_SET_LAN_CONFIG_PARAMETERS,
                                (UINT8 *)&SetParamCommand,
                                CommandDataSize,
                                &CompletionCode,
                                &ResponseDataSize );

    DEBUG ((DEBUG_INFO, " IPMI: IPMI_TRANSPORT_SET_LAN_CONFIG_PARAMETERS Status: %r\n", Status));

    DEBUG ((DEBUG_INFO, "%a Exiting... \n", __FUNCTION__));

    return Status;
}

/** @internal
    Configure the LAN IPV6 Dynamic Router address source values from BIOS
    setup to BMC.

    @param[in] LanChannelNumber     LAN channel to use.

    @return EFI_STATUS      Return status of the function calls used.

**/
EFI_STATUS
SetIp6RouterAddressSource (
  IN UINT8      LanChannelNumber,
  IN UINT8      IsStaticSource )
{
    EFI_STATUS                                          Status;
    AMI_SET_LAN_CONFIG_PARAM_IPV6_ROUTER_CONFIG_REQUEST SetIpv6RouterConfig;
    UINT8                                               CommandDataSize;
    UINT8                                               ResponseDataSize;
    UINT8                                               CompletionCode;

    DEBUG ((DEBUG_INFO, "%a Entered... \n", __FUNCTION__));

    // Set Router IP address.
    SetIpv6RouterConfig.LanChannel.ChannelNumber    = LanChannelNumber;
    SetIpv6RouterConfig.LanChannel.Reserved         = 0;
    SetIpv6RouterConfig.ParameterSelector           = AmiIpmiIpv6RouterConfig;
    SetIpv6RouterConfig.Data1                       = IsStaticSource;
    CommandDataSize                                 = sizeof (SetIpv6RouterConfig);

    // Wait until Set In Progress field is cleared.
    Status = IpmiWaitSetInProgressClear (
                gIpmiTransport,
                LanChannelNumber );
    if (EFI_ERROR(Status)) {
       return Status;
    }

    ResponseDataSize = sizeof (CompletionCode);
    Status = gIpmiTransport->SendIpmiCommand (
                                gIpmiTransport,
                                IPMI_NETFN_TRANSPORT,
                                AMI_BMC_LUN,
                                IPMI_TRANSPORT_SET_LAN_CONFIG_PARAMETERS,
                                (UINT8 *)&SetIpv6RouterConfig,
                                CommandDataSize,
                                &CompletionCode,
                                &ResponseDataSize );

    DEBUG ((DEBUG_INFO, " IPMI: Set IPv6 Router Config Control Data Status: %r, CompletionCode: %x\n", Status, CompletionCode));

    DEBUG ((DEBUG_INFO, "%a Exiting... \n", __FUNCTION__));

    return Status;
}

/** @internal
    Set the LAN IPV6 address source values from BIOS setup to BMC.

    @param[in] LanChannelNumber     LAN channel to use.
    @param[in] Ip6Addr              Ipv6 Address to set.
    @param[in] PrefixLength         Ipv6 Address Prefix length.

    @return EFI_STATUS      Return status of the function calls used.

**/
EFI_STATUS
SetIp6StaticAddressParam (
  IN UINT8              LanChannelNumber,
  IN EFI_IPv6_ADDRESS   Ip6Addr,
  IN UINT8              PrefixLength )
{
    AMI_SET_LAN_CONFIG_PARAM_IPV6_ADDRESS_REQUEST   SetParamCommand;
    EFI_STATUS                                      Status;
    UINT8                                           CommandDataSize;
    UINT8                                           CompletionCode;
    UINT8                                           ResponseDataSize;

    DEBUG ((DEBUG_INFO, "%a Entry...\n", __FUNCTION__));

    SetParamCommand.LanChannel.ChannelNumber    = LanChannelNumber;
    SetParamCommand.LanChannel.Reserved         = 0;
    SetParamCommand.ParameterSelector           = AmiIpmiIpv6StaticAddress;
    SetParamCommand.Ip6AddressParam.SetSelector = AMI_IPMI_SELECTOR_NONE;

    SetParamCommand.Ip6AddressParam.AddressSource.EnableStaticSource = AMI_IPMI_SET_FLAG;
    SetParamCommand.Ip6AddressParam.AddressSource.Reserved = 0;
    SetParamCommand.Ip6AddressParam.AddressSource.Type = 0;     // Means source is Static.

    CopyMem (
        SetParamCommand.Ip6AddressParam.Ipv6Address.Addr,
        Ip6Addr.Addr,
        sizeof (Ip6Addr.Addr) );
    SetParamCommand.Ip6AddressParam.AddressPrefixLen = PrefixLength;

    CommandDataSize = sizeof (SetParamCommand);
    ResponseDataSize = sizeof (CompletionCode);
    
    // Wait until Set In Progress field is cleared.
    Status = IpmiWaitSetInProgressClear (
                gIpmiTransport,
                LanChannelNumber );
    if (EFI_ERROR(Status)) {
       return Status;
    }

    Status = gIpmiTransport->SendIpmiCommand (
                                gIpmiTransport,
                                IPMI_NETFN_TRANSPORT,
                                AMI_BMC_LUN,
                                IPMI_TRANSPORT_SET_LAN_CONFIG_PARAMETERS,
                                (UINT8 *)&SetParamCommand,
                                CommandDataSize,
                                &CompletionCode,
                                &ResponseDataSize );

    DEBUG ((DEBUG_INFO, " IPMI: IPMI_TRANSPORT_SET_LAN_CONFIG_PARAMETERS Status: %r\n", Status));

    DEBUG ((DEBUG_INFO, "%a Exiting...\n", __FUNCTION__));

    return Status;
}

/** @internal
    Set the LAN IPV6 static router address source values from BIOS setup
    to BMC.

    @param[in] LanChannelNumber     LAN channel to use.

    @param[in] Ip6Addr              Static Router Ipv6 Address to set.

    @param[in] PrefixLength         Ipv6 Address Prefix length.

    @return EFI_STATUS      Return status of the function calls used.

**/
EFI_STATUS
SetIp6StaticRouterAddressParam (
  IN  UINT8             LanChannelNumber,
  IN  EFI_IPv6_ADDRESS  Ip6Addr,
  IN  EFI_IPv6_ADDRESS  RouterPrefixValue,
  IN  UINT8             RouterPrefixLength )
{
    EFI_STATUS                                                 Status;
    AMI_SET_LAN_CONFIG_PARAM_IPV6_ROUTER_IP_ADDRESS_REQUEST    SetIpv6RouterAddress;
    AMI_SET_LAN_CONFIG_PARAM_IPV6_ROUTER_PREFIX_VALUE_REQUEST  SetIpv6RouterPrefixValue;
    AMI_SET_LAN_CONFIG_PARAM_IPV6_ROUTER_PREFIX_LENGTH_REQUEST SetIpv6RouterPrefixLength;
    UINT8                                                      CommandDataSize;
    UINT8                                                      ResponseDataSize;
    UINT8                                                      CompletionCode;

    DEBUG ((DEBUG_INFO, "%a Entry...\n", __FUNCTION__));


    // Set Static Router IP address.
    SetIpv6RouterAddress.LanChannel.ChannelNumber   = LanChannelNumber;
    SetIpv6RouterAddress.LanChannel.Reserved        = 0;
    SetIpv6RouterAddress.ParameterSelector          = AmiIpmiIpv6StaticRouter1IpAddr;

    CopyMem (
        SetIpv6RouterAddress.Ip6Address.Addr,
        Ip6Addr.Addr,
        sizeof (Ip6Addr.Addr) );
    CommandDataSize = sizeof (SetIpv6RouterAddress);

    // Wait until Set In Progress field is cleared
    Status = IpmiWaitSetInProgressClear (
                gIpmiTransport,
                LanChannelNumber );
    if (EFI_ERROR(Status)) {
       return Status;
    }

    CompletionCode    = 0;
    ResponseDataSize  = sizeof (CompletionCode);
    Status = gIpmiTransport->SendIpmiCommand (
                                gIpmiTransport,
                                IPMI_NETFN_TRANSPORT,
                                AMI_BMC_LUN,
                                IPMI_TRANSPORT_SET_LAN_CONFIG_PARAMETERS,
                                (UINT8 *)&SetIpv6RouterAddress,
                                CommandDataSize,
                                &CompletionCode,
                                &ResponseDataSize );

    DEBUG ((DEBUG_INFO, " IPMI: IPMI_TRANSPORT_SET_STATIC_ROUTER_IP_ADDR Status: %r, CompletionCode: %x\n", Status, CompletionCode));

    // Set Static Router Prefix Value.
    SetIpv6RouterPrefixValue.LanChannel.ChannelNumber   = LanChannelNumber;
    SetIpv6RouterPrefixValue.LanChannel.Reserved        = 0;
    SetIpv6RouterPrefixValue.ParameterSelector          = AmiIpmiIpv6StaticRouter1PrefixValue;

    CopyMem (
        SetIpv6RouterPrefixValue.Ip6Address.Addr,
        RouterPrefixValue.Addr,
        sizeof (RouterPrefixValue.Addr) );
    CommandDataSize = sizeof (SetIpv6RouterPrefixValue);

    // Wait until Set In Progress field is cleared
    Status = IpmiWaitSetInProgressClear (
                gIpmiTransport,
                LanChannelNumber );
    if (EFI_ERROR(Status)) {
       return Status;
    }

    CompletionCode   = 0;
    ResponseDataSize  = sizeof (CompletionCode);
    Status = gIpmiTransport->SendIpmiCommand (
                                gIpmiTransport,
                                IPMI_NETFN_TRANSPORT,
                                AMI_BMC_LUN,
                                IPMI_TRANSPORT_SET_LAN_CONFIG_PARAMETERS,
                                (UINT8 *)&SetIpv6RouterPrefixValue,
                                CommandDataSize,
                                &CompletionCode,
                                &ResponseDataSize );

    DEBUG ((DEBUG_INFO, " IPMI: IPMI_TRANSPORT_SET_STATIC_ROUTER_PREFIX_VAL Status: %r, CompletionCode: %x\n", Status, CompletionCode));

    // Set Static Router Prefix Length.
    SetIpv6RouterPrefixLength.LanChannel.ChannelNumber   = LanChannelNumber;
    SetIpv6RouterPrefixLength.LanChannel.Reserved        = 0;
    SetIpv6RouterPrefixLength.ParameterSelector          = AmiIpmiIpv6StaticRouter1PrefixLength;
    SetIpv6RouterPrefixLength.RouterPrefixLength         = RouterPrefixLength;

    CommandDataSize = sizeof (SetIpv6RouterPrefixLength);

    // Wait until Set In Progress field is cleared
    Status = IpmiWaitSetInProgressClear (
                gIpmiTransport,
                LanChannelNumber );
    if (EFI_ERROR(Status)) {
       return Status;
    }

    CompletionCode    = 0;
    ResponseDataSize  = sizeof (CompletionCode);
    Status = gIpmiTransport->SendIpmiCommand (
                                gIpmiTransport,
                                IPMI_NETFN_TRANSPORT,
                                AMI_BMC_LUN,
                                IPMI_TRANSPORT_SET_LAN_CONFIG_PARAMETERS,
                                (UINT8 *)&SetIpv6RouterPrefixLength,
                                CommandDataSize,
                                &CompletionCode,
                                &ResponseDataSize );

    DEBUG ((DEBUG_INFO, " IPMI: IPMI_TRANSPORT_SET_STATIC_ROUTER_PREFIX_LENGTH Status: %r, CompletionCode: %x\n", Status, CompletionCode));
    DEBUG ((DEBUG_INFO, "%a Exiting...\n", __FUNCTION__));

    return Status;
}

/** @internal
    Checks in the LAN IPV6 addressing is supported in BMC or not.

    @param[in] LanChannelNumber     LAN channel to use.

    @retval EFI_UNSUPPORTED     Error completion code or IPv6 is disabled
                                in BMC.
    @retval Others              Return status of the function calls used.

**/
EFI_STATUS
GetIp6Supported (
  IN  UINT8      LanChannelNumber,
  IN OUT  UINT8      *GetIPV6Status
  )
{
    EFI_STATUS                                              Status;
    AMI_GET_LAN_CONFIG_PARAM_REQUEST                        CommandData;
    AMI_GET_LAN_CONFIG_PARAM_IP_4_6_SUPPORT_ENABLE_RESPONSE ResponseData;
    UINT8                                                   ResponseDataSize;

    DEBUG ((DEBUG_INFO, "%a Entered....\n", __FUNCTION__));

    // Get Ipv6 support details.
    CommandData.LanChannel.ChannelNumber    = LanChannelNumber;
    CommandData.LanChannel.GetParam         = 0;
    CommandData.LanChannel.Reserved         = 0;
    CommandData.ParameterSelector           = AmiIpmiIpv4OrIpv6AddressEnable;
    CommandData.SetSelector                 = AMI_IPMI_SELECTOR_NONE;
    CommandData.BlockSelector               = AMI_IPMI_SELECTOR_NONE;
    ResponseDataSize                        = sizeof(ResponseData);

    // Wait until Set In Progress field is cleared.
    Status = IpmiWaitSetInProgressClear (
                gIpmiTransport,
                LanChannelNumber );
    if (EFI_ERROR (Status)) {
        *GetIPV6Status = IPV6_SUPPORT_DISABLED;
        return Status;
    }

    Status = gIpmiTransport->SendIpmiCommand (
                                gIpmiTransport,
                                IPMI_NETFN_TRANSPORT,
                                AMI_BMC_LUN,
                                IPMI_TRANSPORT_GET_LAN_CONFIG_PARAMETERS,
                                (UINT8 *)&CommandData,
                                sizeof(CommandData),
                                (UINT8 *)&ResponseData,
                                &ResponseDataSize );

    DEBUG ((DEBUG_INFO, "IPMI : IPMI_TRANSPORT_GET_LAN_CONFIG_PARAMETERS Status: %r\n", Status));
    DEBUG ((DEBUG_INFO, "IPMI : LanChannelNumber: %x Completion Code: %x\n", LanChannelNumber, ResponseData.CompletionCode));

    if (!EFI_ERROR(Status) && (ResponseData.CompletionCode == PARAMETER_NOT_SUPPORTED)) {
       *GetIPV6Status = IPV6_NOT_SUPPORTED;
       return EFI_UNSUPPORTED;
    }
    if (!EFI_ERROR(Status)) {
        if(ResponseData.Data1 == AMI_BMC_IPV6_DIS){
            DEBUG ((DEBUG_ERROR, "IPMI : Ipv6 Support is Disabled \n"));
            *GetIPV6Status = AMI_BMC_IPV6_DIS;
        } else if ((ResponseData.Data1 == AMI_BMC_IPV6_ENABLED) || (ResponseData.Data1 == AMI_BMC_IPV4_IPV6_ENABLED)) {
            DEBUG ((DEBUG_ERROR, "IPMI : Ipv6 Support is Enabled \n"));
            *GetIPV6Status = AMI_BMC_IPV6_ENABLED;
        }
    }
    DEBUG ((DEBUG_INFO, "%a Exiting...\n", __FUNCTION__));

    return Status;
}

/**
    This function is entry point of this driver and will configure Ipv6
    address to BMC LAN channel using static information available through
    setup or using a DHCP client as per user request.

    @param[in] ImageHandle  Handle of this driver image.
    @param[in] SystemTable  Table containing standard EFI services.

    @retval EFI_SUCCESS     Driver is initialized successfully.
    @retval Others          Error status of function calls used.

**/
EFI_STATUS
EFIAPI
InitializeIp6BmcLanConfig (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable )
{
    EFI_STATUS          Status;
    UINTN               Size;
    UINT32              Attributes;
    EFI_IPv6_ADDRESS    Ipv6Addr,RouterPrefixValue;
    UINT8               StaticSupport = 0;
    UINT8               RouterSupport = 0;
    CHAR16              StaticIp6Lan[40];
    CHAR16              StaticIp6LanRouterIp[40];
    CHAR16              StaticRouterPrefixValue[40];
    UINT8               BmcLan1= 0;
    UINT8               BmcLan2 = 0;
    UINT8               ChannelNumberBuffer[BMC_LAN_COUNT];
    UINT8               ChannelCount = BMC_LAN_COUNT;
    UINT16              BmcRouterLan1 =0;
    UINT16              BmcRouterLan2 =0;

    DEBUG ((DEBUG_INFO, "%a Entry... \n", __FUNCTION__));

    Status = gBS->LocateProtocol (
                    &gEfiDxeIpmiTransportProtocolGuid,
                    NULL,
                    (VOID **)&gIpmiTransport );

    DEBUG ((DEBUG_INFO, "LocateProtocol gEfiDxeIpmiTransportProtocolGuid Status: %r \n", Status));

    if (EFI_ERROR (Status)) {
        return Status;
    }

    Size = sizeof (gServerMgmtConfiguration);
    Status = gRT->GetVariable (
                    L"ServerSetup",
                    &gEfiServerMgmtSetupVariableGuid,
                    &Attributes,
                    &Size,
                    &gServerMgmtConfiguration );

    DEBUG ((DEBUG_INFO, "IPMI : GetVariable status of Server Setup %r \n", Status));
    if (EFI_ERROR(Status)) {
        BmcLan1 = IpmiUnspecified;
        BmcRouterLan1 = IpmiUnspecified;
#if (BMC_LAN_COUNT == 2)
        BmcLan2 = IpmiUnspecified;
        BmcRouterLan2 = IpmiUnspecified;
#endif
    } else {
        BmcLan1 = gServerMgmtConfiguration.Ip6BmcLan1;
        BmcRouterLan1 = gServerMgmtConfiguration.Ip6RouterLan1;
        DEBUG ((DEBUG_INFO, "IPMI : BmcRouterLan1 = %x\n", BmcRouterLan1));
#if (BMC_LAN_COUNT == 2)
        BmcLan2 = gServerMgmtConfiguration.Ip6BmcLan2;
        BmcRouterLan2 = gServerMgmtConfiguration.Ip6RouterLan2;
        DEBUG ((DEBUG_INFO, "IPMI : BmcRouterLan2 = %x\n", BmcRouterLan2));
#endif
    }

    Status = AmiIpmiGetLanChannelNumber (gIpmiTransport, ChannelNumberBuffer, &ChannelCount);
    DEBUG ((DEBUG_INFO, "%a ChannelNumberBuffer[0]: %x ChannelNumberBuffer[1]: %x ChannelCount = %x\n",__FUNCTION__, ChannelNumberBuffer[0], ChannelNumberBuffer[1], ChannelCount));
    if (EFI_ERROR(Status))
        return Status;

    if (ChannelNumberBuffer[0] != 0) {
        // Check if Ipv6 supported or not.
        Status = GetIp6Supported (ChannelNumberBuffer[0],&gServerMgmtConfiguration.Ip6Support1);
        DEBUG ((DEBUG_INFO, "BMC LAN1 Ip6 configuration Status: %r\n", Status));

        if (gServerMgmtConfiguration.Ip6Support1 == IPV6_SUPPORTED) {
            // Setting to unspecified since request is processed.
            gServerMgmtConfiguration.Ip6BmcLan1 = IpmiUnspecified;
            gServerMgmtConfiguration.Ip6RouterLan1 = IpmiUnspecified;
            if (BmcLan1 == IpmiStaticAddrsss) {  // Static address.
                StaticSupport = 1;
            } else {
                StaticSupport = 0;
            }
            if (BmcRouterLan1 == IpmiStaticAddrsss) {  // Static address.
                RouterSupport =IpmiStaticAddrsss;
            } else if (BmcRouterLan1 == IpmiDynamicAddressBmcDhcp) {
                RouterSupport = IpmiDynamicAddressBmcDhcp;
            }
            DEBUG ((DEBUG_INFO, "IPMI : RouterSupport = %x\n", RouterSupport));
            if (BmcLan1 != IpmiUnspecified) {
                // Enable or Disable the Static support to the BMC.
                SetIp6AddressSource (
                    ChannelNumberBuffer[0],
                    StaticSupport );

                if (StaticSupport) {
                    /* Setting Static Ipv6 address. Local copy is made to prevent
                       ASSERT due to alignment checking in NetLibStrToIp6().*/
                    CopyMem (
                        (UINT8*)StaticIp6Lan,
                        (UINT8*)gServerMgmtConfiguration.StaticIp6Lan1,
                        sizeof (gServerMgmtConfiguration.StaticIp6Lan1) );
                    DEBUG ((DEBUG_INFO, "InitializeIp6BmcLanConfig: BMC LAN1 Static Ipv6 Address: %S\n", StaticIp6Lan));

                    NetLibStrToIp6 (
                        StaticIp6Lan,
                        &Ipv6Addr );
                        
                    SetIp6StaticAddressParam (
                        ChannelNumberBuffer[0],
                        Ipv6Addr,
                        gServerMgmtConfiguration.PrefixLength1 );

                }
            }
            if (BmcRouterLan1 != IpmiUnspecified) {

                SetIp6RouterAddressSource (
                    ChannelNumberBuffer[0],
                    RouterSupport);

                DEBUG ((DEBUG_INFO, "IPMI : SetIp6RouterAddressSource RouterSupport = %x\n", RouterSupport));
                if (RouterSupport == IpmiStaticAddrsss) {

                    CopyMem (
                        (UINT8*)StaticIp6LanRouterIp,
                        (UINT8*)gServerMgmtConfiguration.StaticIp6Lan1Router1Ip,
                        sizeof (gServerMgmtConfiguration.StaticIp6Lan1Router1Ip) );
                        DEBUG ((DEBUG_INFO, "InitializeIp6BmcLanConfig: BMC LAN1 Static Router Ipv6 Address: %S\n", StaticIp6LanRouterIp));

                    NetLibStrToIp6 (
                        StaticIp6LanRouterIp,
                        &Ipv6Addr );

                    CopyMem (
                        (UINT8*)StaticRouterPrefixValue,
                        (UINT8*)gServerMgmtConfiguration.RouterPrefixValue1,
                        sizeof (gServerMgmtConfiguration.RouterPrefixValue1 ));

                    DEBUG ((DEBUG_INFO, "InitializeIp6BmcLanConfig: BMC LAN1 Static Router Ipv6 Prefix Value: %S\n", StaticRouterPrefixValue));

                    NetLibStrToIp6(
                        StaticRouterPrefixValue,
                        &RouterPrefixValue );

                    SetIp6StaticRouterAddressParam (
                        ChannelNumberBuffer[0],
                        Ipv6Addr,
                        RouterPrefixValue,
                        gServerMgmtConfiguration.RouterPrefixLength1 );
                }
            }
        }
    }

#if (BMC_LAN_COUNT == 2)
    if (ChannelNumberBuffer[1] != 0) {
        // Check if Ipv6 supported or not.
        Status = GetIp6Supported (ChannelNumberBuffer[1],&gServerMgmtConfiguration.Ip6Support2);
        DEBUG ((DEBUG_INFO, "BMC LAN2 Ip6 configuration Status: %r\n", Status));

        if (gServerMgmtConfiguration.Ip6Support2 == IPV6_SUPPORTED) {
            // Setting to unspecified since request is processed.
            gServerMgmtConfiguration.Ip6BmcLan2 = IpmiUnspecified;
            gServerMgmtConfiguration.Ip6RouterLan2 = IpmiUnspecified;

            if (BmcLan2 == IpmiStaticAddrsss) {  // Static address.
                StaticSupport = 1;
            } else {
                StaticSupport = 0;
            }
            if (BmcRouterLan2 == IpmiStaticAddrsss) {  // Static address.
                RouterSupport = 1;
            } else if (BmcRouterLan2 == IpmiDynamicAddressBmcDhcp) {
                RouterSupport = 2;
            }
            DEBUG ((DEBUG_INFO, "IPMI : RouterSupport = %x\n", RouterSupport));
            if (BmcLan2 != IpmiUnspecified) {
                // Setting LAN2 address source to BMC.
                SetIp6AddressSource (
                    ChannelNumberBuffer[1],
                    StaticSupport );


                if (StaticSupport) {
                    /* Setting Static Ipv6 address. Local copy is made to prevent
                       ASSERT due to alignment checking in NetLibStrToIp6().*/
                    CopyMem (
                        (UINT8*)StaticIp6Lan,
                        (UINT8*)gServerMgmtConfiguration.StaticIp6Lan2,
                        sizeof (gServerMgmtConfiguration.StaticIp6Lan2) );
                    DEBUG ((DEBUG_INFO, "InitializeIp6BmcLanConfig: BMC LAN2 Static Ipv6 Address: %S\n", StaticIp6Lan));

                    NetLibStrToIp6 (
                        StaticIp6Lan,
                        &Ipv6Addr );

                    SetIp6StaticAddressParam (
                        ChannelNumberBuffer[1],
                        Ipv6Addr,
                        gServerMgmtConfiguration.PrefixLength2 );
                }
            }
            if (BmcRouterLan2 != IpmiUnspecified) {
                SetIp6RouterAddressSource (
                    ChannelNumberBuffer[1],
                    RouterSupport );

                DEBUG ((DEBUG_INFO, "IPMI : SetIp6RouterAddressSource RouterSupport = %x\n", RouterSupport));
                if (RouterSupport == IpmiStaticAddrsss) {
                    CopyMem (
                        (UINT8*)StaticIp6LanRouterIp,
                        (UINT8*)gServerMgmtConfiguration.StaticIp6Lan2Router1Ip,
                        sizeof (gServerMgmtConfiguration.StaticIp6Lan2Router1Ip) );
                    DEBUG ((DEBUG_INFO, "InitializeIp6BmcLanConfig: BMC LAN2 Static Router Ipv6 Address: %S\n", StaticIp6LanRouterIp));

                    NetLibStrToIp6 (
                        StaticIp6LanRouterIp,
                        &Ipv6Addr );

                    CopyMem (
                        (UINT8*)StaticRouterPrefixValue,
                        (UINT8*)gServerMgmtConfiguration.RouterPrefixValue2,
                        sizeof (gServerMgmtConfiguration.RouterPrefixValue2 ));

                    DEBUG ((DEBUG_INFO, "InitializeIp6BmcLanConfig: BMC LAN1 Static Router Ipv6 Prefix Value: %S\n", StaticRouterPrefixValue));

                    NetLibStrToIp6(
                        StaticRouterPrefixValue,
                        &RouterPrefixValue );

                    SetIp6StaticRouterAddressParam (
                        ChannelNumberBuffer[1],
                        Ipv6Addr,
                        RouterPrefixValue,
                        gServerMgmtConfiguration.RouterPrefixLength2 );
                }
            }
        }
    }
#endif

    Status = gRT->SetVariable (
                    L"ServerSetup",
                    &gEfiServerMgmtSetupVariableGuid,
                    Attributes,   // Attributes
                    sizeof(SERVER_MGMT_CONFIGURATION_DATA),
                    &gServerMgmtConfiguration );
    DEBUG ((DEBUG_INFO, "IPMI :SetVariable status of ServerSetup %r \n", Status));

    DEBUG ((DEBUG_INFO, "%a Exiting... \n", __FUNCTION__));
    return EFI_SUCCESS;
}


