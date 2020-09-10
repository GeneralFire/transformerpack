//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2020, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************


/** @file AmiIpmiNetFnTransportDefinitions.h
    Ipmi NetFn Transport additional commands and its structures.

*/

#ifndef _AMI_IPMI_NETFN_TRANSPORT_DEFINITIONS_H_
#define _AMI_IPMI_NETFN_TRANSPORT_DEFINITIONS_H_

//---------------------------------------------------------------------------

#include <IndustryStandard/IpmiNetFnTransport.h>

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

#define AMI_BMC_IPV6_DIS                    0x00
#define AMI_BMC_IPV6_ENABLED                0x01
#define AMI_BMC_IPV4_IPV6_ENABLED           0x02
#define AMI_DHCP_SLAAC                      0x01
#define AMI_DHCP_DHCP6                      0x02
#define AMI_ENABLE_DYNAMIC_ROUTER_ADDRESS   0x02
#define AMI_ENABLE_STATIC_ROUTER_ADDRESS    0x01

/**
    LAN Management Structure. It is extended version of IPMI_LAN_OPTION_TYPE
    to support IPv6 configuration as per latest Ipmi spec.
*/
typedef enum {
    /// IPv6/IPv4 Support (read only).
    AmiIpmiIpv4OrIpv6Support = 0x32,
    /// IPv6/IPv4 Addressing enables.
    AmiIpmiIpv4OrIpv6AddressEnable,
    /// IPv6 Header Static Traffic Class.
    AmiIpmiIpv6HdrStatTrafficClass,
    /// IPv6 Header Static Hop Limit.
    AmiIpmiIpv6HdrStatHopLimit,
    /// IPv6 Header Flow Label.
    AmiIpmiIpv6HdrFlowLabel,
    /// IPv6 Status (read only).
    AmiIpmiIpv6Status,
    /// IPv6 Static Address.
    AmiIpmiIpv6StaticAddress,
    /// IPv6 DHCPv6 Static DUID storage length (read only).
    AmiIpmiIpv6DhcpStaticDuidLen,
    /// IPv6 DHCPv6 Static DUIDs.
    AmiIpmiIpv6DhcpStaticDuid,
    /// IPv6 Dynamic (SLAAC/ DHCPv6) Address (read only).
    AmiIpmiIpv6DhcpAddress,
    /// IPv6 DHCPv6 Dynamic DUID storage length (read only).
    AmiIpmiIpv6DhcpDynamicDuidLen,
    /// IPv6 DHCPv6 Dynamic DUIDs.
    AmiIpmiIpv6DhcpDynamicDuid,
    /// IPv6 Router Address Configuration Control.
    AmiIpmiIpv6RouterConfig = 0x40,
    /// IPv6 Static Router 1 IP Address.
    AmiIpmiIpv6StaticRouter1IpAddr,
    /// Ipv6 Static Router1 Prefix length.
    AmiIpmiIpv6StaticRouter1PrefixLength = 0x43,
    /// Ipv6 Static Router1 Prefix Value.
    AmiIpmiIpv6StaticRouter1PrefixValue,
    /// Ipv6 Dynamic Router Info Set.
    AmiIpmiIpv6DynamicRouterInfoSet = 0x49,
    /// IPv6 Dynamic Router Info IP Address (read-only).
    AmiIpmiIpv6DynamicRouterIpAddr,
    /// IPv6 Dynamic Router Info Prefix Length.
    AmiIpmiIpv6DynamicRouterInfoPrefixLength =0x4c,
    /// IPv6 Dynamic Router Info Prefix Length.
    AmiIpmiIpv6DynamicRouterInfoPrefixValue
} AMI_LAN_OPTION_TYPE;

/**
    Serial multiplex setting.
*/
typedef enum {
    AmiGetPresentMuxSettingOrStatusOnly,    ///<  Reserved.
    AmiReqMuxToSystem,                      ///<  Request Mux To System.
    AmiReqMuxToBmc,                         ///<  Request Mux To Bmc.
    AmiForceMuxToSystem,                    ///<  Force Mux To System.
    AmiForceMuxToBmc,                       ///<  Force Mux To Bmc.
    AmiBlockMuxSwitchToSystem,              ///<  Block Mux Switch To System.
    AmiAllowMuxSwitchToSystem,              ///<  Allow Mux Switch To System.
    AmiBlockMuxSwitchToBmc,                 ///<  Block Mux Switch To Bmc.
    AmiAllowMuxSwitchToBmc                  ///<  Allow Mux Switch To Bmc.
} AMI_MUX_SETTING;

#pragma pack(1)

/**
    Get lan IP address response.
*/
typedef struct {
    /// Completion code.
    UINT8               CompletionCode;
    UINT8               ParamRev;       ///< Parameter revision.
    EFI_IPv4_ADDRESS    Ipv4Address;    ///< IPv4 IP address.
} AMI_GET_LAN_CONFIG_PARAM_IP_ADDRESS_RESPONSE;

/**
    Get lan IP address source response.
*/
typedef struct {
    /// Completion code.
    UINT8   CompletionCode;
    UINT8   ParamRev;           ///< Parameter revision.
    UINT8   AddressSrc : 4;     ///< Address source.
    UINT8   Reserved : 4;       ///< Reserved.
} AMI_GET_LAN_CONFIG_PARAM_IP_ADDRESS_SRC_RESPONSE;

/**
    Get lan MAC address response.
*/
typedef struct {
    /// Completion code.
    UINT8   CompletionCode;
    UINT8   Param;          ///< Parameter revision.
    UINT8   MacAddress[6];  ///< MAC address.
} AMI_GET_LAN_CONFIG_PARAM_MAC_ADDRESS_RESPONSE;

/**
    Get lan subnet mask response.
*/
typedef struct {
    /// Completion code.
    UINT8   CompletionCode;
    UINT8   Param;          ///< Parameter revision.
    UINT8   SubnetMask[4];  ///< IPv4 subnet mask address.
} AMI_GET_LAN_CONFIG_PARAM_SUBNET_MASK_RESPONSE;

/**
    LAN channel number while get command.
*/
typedef struct {
    /// Channel number.
    UINT8   ChannelNumber : 4;
    UINT8   Reserved : 3;       ///< Reserved.
    UINT8   GetParam : 1;       ///< Get parameter/Get revision only.
} AMI_GET_LAN_CHANNEL_NUM;

/**
    LAN channel number while set command.
*/
typedef struct {
    /// Channel number.
    UINT8   ChannelNumber : 4;
    UINT8   Reserved : 4;       ///< Reserved.
} AMI_SET_LAN_CHANNEL_NUM;

/**
    Get lan config parameters command request.
*/
typedef struct {
    /// LAN channel number.
    AMI_GET_LAN_CHANNEL_NUM LanChannel;
    UINT8                   ParameterSelector;  ///< Parameter selector.
    UINT8                   SetSelector;        ///< Set selector.
    UINT8                   BlockSelector;      ///< Block selector.
} AMI_GET_LAN_CONFIG_PARAM_REQUEST;

/**
    Set in progress parameter.
*/
typedef struct {
    /// Set-In Progress Bits.
    UINT8   SetInProgress:2;
    UINT8   Reserved:6;         ///< Reserved.
} AMI_IPMI_SET_IN_PROGRESS;

/**
    Get lan config command response structure with Parameter 0.
*/
typedef struct {
    /// Completion code.
    UINT8                       CompletionCode;
    UINT8                       ParamRev;       ///< Parameter Revision.
    AMI_IPMI_SET_IN_PROGRESS    Param0;         ///< Set In Progress.
} AMI_GET_LAN_CONFIG_PARAM_SET_IN_PROGRESS_RESPONSE;

/**
    Get lan config command response structure for IPv4 or Ipv6 support enable
*/
typedef struct {
    /// Completion code.
    UINT8   CompletionCode;
    UINT8   ParamRev;       ///< Parameter Revision.
    UINT8   Data1;          ///< Enable Details.
} AMI_GET_LAN_CONFIG_PARAM_IP_4_6_SUPPORT_ENABLE_RESPONSE;

/**
    Set lan config command request structure for IPv4 or Ipv6 support enable
*/
typedef struct {
    /// Channel to be updated.
    AMI_SET_LAN_CHANNEL_NUM LanChannel;
    UINT8                   ParameterSelector;  ///< Parameter selector.
    UINT8                   Data1;              ///< Enable Details.
} AMI_SET_LAN_CONFIG_IP_4_6_SUPPORT_ENABLE_REQUEST;

/**
    IPv6 address source.
*/
typedef struct {
    /// Address type.
    UINT8   Type : 4;
    UINT8   Reserved : 3;               ///< Reserved.
    UINT8   EnableStaticSource : 1;     ///< 1-Enable or 0-Disable static address support.
} AMI_IPMI_IPV6_ADDRESS_SOURCE;

/**
    IPv6 address.
*/
typedef struct {
    /// Set selector.
    UINT8                           SetSelector;
    AMI_IPMI_IPV6_ADDRESS_SOURCE    AddressSource;      ///< Address source.
    EFI_IPv6_ADDRESS                Ipv6Address;        ///< Static IP address.
    UINT8                           AddressPrefixLen;   ///< IP address prefix length.
    UINT8                           AddressStatus;      ///< IP address status.
} AMI_IPMI_IPV6_ADDRESS_PARAM;

/**
    Get lan config command response structure for IPv6 address.
*/
typedef struct {
    /// Completion code.
    UINT8                           CompletionCode;
    UINT8                           ParamRev;           ///< Parameter Revision.
    AMI_IPMI_IPV6_ADDRESS_PARAM     Ip6AddressParam;    ///< Ipv6 Address.
} AMI_GET_LAN_CONFIG_PARAM_IPV6_ADDRESS_RESPONSE;

/**
    Get lan config command response structure for IPv6 Router Control Data.
*/
typedef struct {
    /// Completion code.
    UINT8                           CompletionCode;
    UINT8                           ParamRev;              ///< Parameter Revision.
    UINT8                           Ip6RouterControlData;  ///< Ipv6 Address.
} AMI_GET_LAN_CONFIG_PARAM_IPV6_ROUTER_CONTROL_RESPONSE;

/**
    Get lan config command response structure for IPv6 Router Info Set.
*/
typedef struct{
    /// Completion code.
    UINT8                           CompletionCode;
    UINT8                           ParamRev;                 ///< Parameter Revision.
    UINT8                           Ip6DynamicRouterInfoSet;  ///< Ipv6 Address.
} AMI_GET_LAN_CONFIG_PARAM_IPV6_DYNAMIC_ROUTER_INFO_SET_RESPONSE;
/**
    Set lan config command request structure for IPv6 address.
*/
typedef struct {
    /// Channel to be updated.
    AMI_SET_LAN_CHANNEL_NUM         LanChannel;
    UINT8                           ParameterSelector;  ///< Parameter selector.
    AMI_IPMI_IPV6_ADDRESS_PARAM     Ip6AddressParam;    ///< Parameter value.
} AMI_SET_LAN_CONFIG_PARAM_IPV6_ADDRESS_REQUEST;

/**
    Set lan config command request structure for IPv6 router address source.
*/
typedef struct {
    /// Channel to be updated.
    AMI_SET_LAN_CHANNEL_NUM LanChannel;
    /// Parameter selector.
    UINT8                   ParameterSelector;
    /// 1-Enables Static router address, 2-Enables Dynamic router address.
    UINT8                   Data1;
} AMI_SET_LAN_CONFIG_PARAM_IPV6_ROUTER_CONFIG_REQUEST;

/**
    Get lan config command response structure for IPv6 Static router IP address.
*/
typedef struct {
    /// Completion code.
    UINT8               CompletionCode;
    UINT8               ParamRev;           ///< Parameter Revision.
    EFI_IPv6_ADDRESS    Ip6Address;         ///< Ipv6 router IP Address.
} AMI_GET_LAN_CONFIG_PARAM_IPV6_STATIC_ROUTER_IP_ADDRESS_RESPONSE;

/**
    Set lan config command request structure for router IPv6 address.
*/
typedef struct {
    /// Channel to be updated.
    AMI_SET_LAN_CHANNEL_NUM     LanChannel;
    UINT8                       ParameterSelector;  ///< Parameter selector.
    EFI_IPv6_ADDRESS            Ip6Address;         ///< Ipv6 router IP Address.
} AMI_SET_LAN_CONFIG_PARAM_IPV6_ROUTER_IP_ADDRESS_REQUEST;

/**
    Set lan config command request structure for IPv6 Router Prefix Value.
*/
typedef struct {
    /// Channel to be updated.
    AMI_SET_LAN_CHANNEL_NUM     LanChannel;
    UINT8                       ParameterSelector;  ///< Parameter selector.
    EFI_IPv6_ADDRESS            Ip6Address;         ///< Ipv6 router IP Address.
} AMI_SET_LAN_CONFIG_PARAM_IPV6_ROUTER_PREFIX_VALUE_REQUEST;

/**
    Set lan config command request structure for IPv6 Router Prefix Length.
*/
typedef struct {
    /// Channel to be updated.
    AMI_SET_LAN_CHANNEL_NUM     LanChannel;
    UINT8                       ParameterSelector;   ///< Parameter selector.
    UINT8                       RouterPrefixLength;  ///< Ipv6 router IP Address.
} AMI_SET_LAN_CONFIG_PARAM_IPV6_ROUTER_PREFIX_LENGTH_REQUEST;

/**
    IPv6 dynamic router IP address.
*/
typedef struct {
    /// Set selector.
    UINT8               SetSelector;
    EFI_IPv6_ADDRESS    Ipv6Address;    ///< Ipv6 router IP address.
} AMI_IPMI_IPV6_DYNAMIC_ROUTER_IP_ADDRESS;

/**
    Get lan config command response structure for dynamic router IP address.
*/
typedef struct {
    /// Completion code.
    UINT8                                       CompletionCode;
    /// Parameter Revision.
    UINT8                                       ParamRev;
    /// Ipv6 Address.
    AMI_IPMI_IPV6_DYNAMIC_ROUTER_IP_ADDRESS     Ip6AddressParam;
} AMI_GET_LAN_CONFIG_PARAM_IPV6_DYNAMIC_ROUTER_IP_ADDRESS_RESPONSE;

/**
    IPv6 dynamic router Prefix Length.
*/
typedef struct {
    /// Set selector.
    UINT8               SetSelector;
    UINT8               PrefixLength;    ///< Ipv6 router Prefix Length.
}AMI_IPMI_IPV6_DYNAMIC_ROUTER_PREFIX_LENGTH;

/**
    Get lan config command response structure for dynamic router Prefix Length.
*/
typedef struct {
    /// Completion code.
    UINT8                                       CompletionCode;
    /// Parameter Revision.
    UINT8                                       ParamRev;
    /// Ipv6 Address.
    AMI_IPMI_IPV6_DYNAMIC_ROUTER_PREFIX_LENGTH  IpDynamicRouterPrefixLength; 
}AMI_GET_LAN_CONFIG_PARAM_IPV6_DYNAMIC_ROUTER_PREFIX_LENGTH_RESPONSE;

/**
    Get lan config command response structure for dynamic router Prefix Value.
*/
typedef struct {
    /// Completion code.
    UINT8                                       CompletionCode;
    /// Parameter Revision.
    UINT8                                       ParamRev;
    /// Ipv6 Address.
    AMI_IPMI_IPV6_DYNAMIC_ROUTER_IP_ADDRESS     IpDynamicRouterPrefixValue; 
}AMI_GET_LAN_CONFIG_PARAM_IPV6_DYNAMIC_ROUTER_PREFIX_VALUE_RESPONSE;

/**
    Get lan config command response structure for Static router1 Prefix Length.
*/
typedef struct {
    /// Completion code.
    UINT8                                       CompletionCode;
    /// Parameter Revision.
    UINT8                                       ParamRev;
    /// Ipv6 Address.
    UINT8                                       Ip6RouterPrefixLength;
}AMI_GET_LAN_CONFIG_PARAM_IPV6_STATIC_ROUTER1_PREFIX_LENGTH_RESPONSE;

/**
    Get lan config command response structure for Static router1 Prefix Value.
*/
typedef struct {
    /// Completion code.
    UINT8                                       CompletionCode;
    /// Parameter Revision.
    UINT8                                       ParamRev;
    /// Ipv6 Address.
    EFI_IPv6_ADDRESS                            IpPrefixValue; 
}AMI_GET_LAN_CONFIG_PARAM_IPV6_STATIC_ROUTER1_PREFIX_VALUE_RESPONSE;
/**
    Set lan config command request structure for IPv4 address.
*/
typedef struct {
    /// Channel to be updated.
    AMI_SET_LAN_CHANNEL_NUM     LanChannel;
    UINT8                       ParameterSelector;  ///< Parameter selector.
    EFI_IPv4_ADDRESS            Ipv4Address;        ///< IPv4 IP address.
} AMI_SET_LAN_CONFIG_PARAM_IPV4_ADDRESS_REQUEST;

/**
    Set lan config command request structure for IPv4 subnet mask.
*/
typedef struct {
    /// Channel to be updated.
    AMI_SET_LAN_CHANNEL_NUM     LanChannel;
    UINT8                       ParameterSelector;  ///< Parameter selector.
    UINT8                       SubnetMask[4];      ///< IPv4 subnet mask.
} AMI_SET_LAN_CONFIG_PARAM_IPV4_SUBNET_MASK_REQUEST;

/**
    Set lan config command request structure for MAC address.
*/
typedef struct {
    /// Channel to be updated.
    AMI_SET_LAN_CHANNEL_NUM     LanChannel;
    UINT8                       ParameterSelector;  ///< Parameter selector.
    UINT8                       MacAddress[6];      ///< MAC address.
} AMI_SET_LAN_CONFIG_PARAM_GATEWAY_MAC_ADDRESS_REQUEST;

/**
    Ip address source structure.
*/
typedef struct {
    /// Address source.
    UINT8   Source : 4;
    UINT8   Reserved : 4;   ///< Reserved.
} AMI_IP_ADDRESS_SOURCE;

/**
    Set lan config command request structure for Ip address source.
*/
typedef struct {
    /// Channel to be updated.
    AMI_SET_LAN_CHANNEL_NUM     LanChannel;
    UINT8                       ParameterSelector;  ///< Parameter selector.
    AMI_IP_ADDRESS_SOURCE       AddressSource;      ///< MAC address.
} AMI_SET_LAN_CONFIG_PARAM_IP_ADDRESS_SOURCE_REQUEST;

/**
    Set Serial/Modem Mux command response structure.
*/
typedef struct {
    /// Completion code.
    UINT8                                       CompletionCode;
    IPMI_SET_SERIAL_MODEM_MUX_COMMAND_RESPONSE  MuxSetting;     ///< Mux setting.
} AMI_SET_SERIAL_MODEM_MUX_RESPONSE;

#pragma pack()

//---------------------------------------------------------------------------

#endif  // #ifndef _AMI_IPMI_NETFN_TRANSPORT_DEFINITIONS_H_


