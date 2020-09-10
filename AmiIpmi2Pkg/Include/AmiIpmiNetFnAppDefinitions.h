//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

/** @file AmiIpmiNetFnAppDefinitions.h
    Ipmi NetFn Application additional commands and its structures.

*/

#ifndef _AMI_IPMI_NETFN_APP_DEFINITIONS_H_
#define _AMI_IPMI_NETFN_APP_DEFINITIONS_H_

//---------------------------------------------------------------------------

#include <IndustryStandard/IpmiNetFnApp.h>

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

#define AMI_IPMI_SPEC_VERSION_1_5                   0x51
#define AMI_IPMI_SPEC_VERSION_2_0                   0x02
#define AMI_IPMI_APP_SELFTEST_RESERVED              0xFF
#define AMI_IPMI_STOP_TIMER_WHEN_SET_WDT            0
#define AMI_IPMI_DO_NOT_STOP_TIMER_WHEN_SET_WDT     1

/**
    Watchdog timer type.
*/
typedef enum {
    AmiIpmiFrb2Reserved1,  ///< Timer type not initialized.
    AmiIpmiFrb2,           ///< Frb2.
    AmiIpmiBiosPost,       ///< Bios/Post.
    AmiIpmiOsBootWdt,      ///< Os Boot Wdt.
    AmiIpmiSmsOrOs,        ///< SMS/OS.
    AmiIpmiOem,            ///< OEM.
} AMI_WATCHDOG_TIMER_USE;

/**
    Watchdog pre-timeout interrupt.
*/
typedef enum {
    AmiIpmiNoTimeoutInterrupt,         ///< No Timeout Interrupt.
    AmiIpmiSystemManagementInterrupt,  ///< System Management Interrupt.
    AmiIpmiDiagnosticInterrupt,        ///< Diagnostic Interrupt.
    AmiIpmiMessagingInterrupt          ///< Messaging Interrupt.
} AMI_WATCHDOG_PRE_TIMEOUT_INTERRUPT;

/**
    Watchdog timeout action.
*/
typedef enum {
    AmiIpmiNormalBoot,      ///< Normal Boot.
    AmiIpmiHardReset,       ///< Hard Reset.
    AmiIpmiPowerDown,       ///< Power Down.
    AmiIpmiPowerCycle,      ///< Power Cycle.
} AMI_WATCHDOG_TIMEOUT_ACTION;

#pragma pack(1)
/**
    Constants and Structure definitions for Timer use expiration flags.
*/
typedef struct {
    /// Reserved.
    UINT8   ExpirationFlagReserved1 : 1;
    UINT8   Frb2ExpirationFlag : 1;         ///< FRB2 expiration flag.
    UINT8   PostExpirationFlag : 1;         ///< POST expiration flag.
    UINT8   OsLoadExpirationFlag : 1;       ///< OS load expiration flag.
    UINT8   SmsOsExpirationFlag : 1;        ///< SMS/OS expiration flag.
    UINT8   OemExpirationFlag : 1;          ///< OEM expiration flag.
    UINT8   ExpirationFlagReserved2 : 1;    ///< Reserved.
    UINT8   ExpirationFlagReserved3 : 1;    ///< Reserved.
} AMI_BMC_IPMI_WATCHDOG_TIMER_EXPIRATION_FLAGS;

/**
    Constants and Structure definitions for Timer Actions.
*/
typedef struct {
    /// Time out action.
    UINT8   TimeoutAction : 3;
    UINT8   Reserved1 : 1;        ///< Reserved.
    UINT8   PreTimeoutIntr : 3;   ///< Pre-timeout interrupt.
    UINT8   Reserved2 : 1;        ///< Reserved.
} AMI_BMC_IPMI_WATCHDOG_TIMER_ACTIONS;

/**
    Constants and Structure definitions for "Set WatchDog Timer" command to follow here.
*/
typedef struct {
    /// Timer use.
    IPMI_WATCHDOG_TIMER_USE                         TimerUse;
    /// Timer action.
    AMI_BMC_IPMI_WATCHDOG_TIMER_ACTIONS             TimerActions;
    /// Pre-timeout interval.
    UINT8                                           PretimeoutInterval;
    /// Timer use expiration flags.
    AMI_BMC_IPMI_WATCHDOG_TIMER_EXPIRATION_FLAGS    TimerUseExpirationFlagsClear;
    /// Initial count down value.
    UINT16                                          InitialCountdownValue;
} AMI_BMC_SET_WATCHDOG_TIMER_REQUEST;

/**
    Get watch dog timer response structure.
*/
typedef struct {
    /// Completion code.
    UINT8                                           CompletionCode;
    /// Timer use.
    IPMI_WATCHDOG_TIMER_USE                         TimerUse;
    /// Timer action.
    AMI_BMC_IPMI_WATCHDOG_TIMER_ACTIONS             TimerActions;
    /// Pre-timeout interval.
    UINT8                                           PretimeoutInterval;
    /// Timer use expiration flags.
    AMI_BMC_IPMI_WATCHDOG_TIMER_EXPIRATION_FLAGS    TimerUseExpirationFlagsClear;
    /// Initial count down value.
    UINT16                                          InitialCountdownValue;
    /// Present count down value.
    UINT16                                          PresentCountdownValue;
} AMI_BMC_GET_WATCHDOG_TIMER_RESPONSE;

/**
    Get Bmc global enables command response.
*/
typedef struct {
    /// Completion code.
    UINT8   CompletionCode;
    UINT8   ReceiveMsgQueueInterrupt : 1;     ///< Receive Message Queue Interrupt.
    UINT8   EventMsgBufferFullInterrupt : 1;  ///< Event Message Buffer Full Interrupt.
    UINT8   EventMsgBuffer : 1;               ///< Event Message Buffer.
    UINT8   SystemEventLogging : 1;           ///< System Event Logging.
    UINT8   Reserved : 1;                     ///< Reserved.
    UINT8   OEM0 : 1;                         ///< OEM0 interrupt.
    UINT8   OEM1 : 1;                         ///< OEM1 interrupt.
    UINT8   OEM2 : 1;                         ///< OEM2 interrupt.
} AMI_GET_BMC_GLOBAL_ENABLES_RESPONSE;

/**
    Channle access type.
 */
typedef enum {
    AmiChannelAccessTypeReserved0,              ///< Reserved0
    AmiChannelAccessTypeNonVolatile,            ///< NonVolatile
    AmiChannelAccessTypePresentVolatileSetting, ///< PresentVolatileSetting
    AmiChannelAccessTypeReserved1               ///< Reserved1
} AMI_CHANNEL_ACCESS_TYPE;

/**
    Channel access modes.
*/
typedef enum {
    AmiChannelAccessModeDisabled,          ///< Disabled Channel Access Mode.
    AmiChannelAccessModePreBootOnly,       ///< Pre-Boot Only Channel Access Mode.
    AmiChannelAccessModeAlwaysAvailable,   ///< Always Available Channel Access Mode.
    AmiChannelAccessModeShared             ///< Shared Channel Access Mode.
} AMI_CHANNEL_ACCESS_MODES;

/**
    Set user access command request structure.
*/
typedef struct {
    /// Channel number.
    // Byte1
    UINT8   ChannelNo:4;
    UINT8   EnableIpmiMessaging:1;          ///< Enable Ipmi Messaging.
    UINT8   EnableUserLinkAuthetication:1;  ///< Enable User Link Authetication.
    UINT8   UserRestrictedToCallback:1;     ///< User Restricted To Callback.
    UINT8   EnableChangeBitsInByte1: 1;     ///< Enable Change Bits In Byte1.
    // Byte2
    UINT8   UserID:6;                       ///< User ID.
    UINT8   Reserve1:2;                     ///< Reserved.
    // Byte3
    UINT8   UserPrivilegeLimit:4;           ///< User Privilege Limit.
    UINT8   Reserve2:4;                     ///< Reserved.
    // Byte4
    UINT8   UserSimultaneousSessionLimit:4; ///< User Simultaneous Session Limit.
    UINT8   Reserve3:4;                     ///< Reserved.
} AMI_SET_USER_ACCESS_REQUEST;

/**
    Get User Access command request structure.
*/
typedef struct {
    /// Channel number.
    // Byte1
    UINT8   ChannelNo:4;
    UINT8   Reserve1:4;     ///< Reserved.
    // Byte2
    UINT8   UserId:6;       ///< User ID.
    UINT8   Reserve2:2;     ///< Reserved.
} AMI_GET_USER_ACCESS_REQUEST;

/**
    Get User Access command response structure.
*/
typedef struct {
    /// Completion code.
    UINT8   CompletionCode;
    // Byte1
    UINT8   MaxUserId:6;                    ///< Maximum User Id.
    UINT8   Reserve1:2;                     ///< Reserved.
    // Byte2
    UINT8   CurrentUserId:6;                ///< Current User Id.
    UINT8   UserIdEnable:2;                 ///< Bmc User Access.
    // Byte3
    UINT8   FixedUserId:6;                  ///< Fixed User Id.
    UINT8   Reserve2:2;                     ///< Reserved.
    // Byte4
    UINT8   UserPrivilegeLimit:4;           ///< User Privilege Limit.
    UINT8   EnableIpmiMessaging:1;          ///< Enable Ipmi Messaging.
    UINT8   EnableUserLinkAuthetication:1;  ///< Enable User Link Authetication.
    UINT8   UserAccessAvailableConn:1;      ///< User Access Available Connection.
    UINT8   Reserve3:1;                     ///< Reserved.
} AMI_GET_USER_ACCESS_RESPONSE;

/**
    Get user name command request structure.
*/
typedef struct {
    /// User Id.
    UINT8   UserId:6;
    UINT8   Reserved:2;   ///< Reserved.
} AMI_GET_USER_NAME_REQUEST;

/**
    Get user name command response structure.
*/
typedef struct {
    /// Completion code.
    UINT8   CompletionCode;
    UINT8   UserName[16];   ///< User name.
} AMI_GET_USER_NAME_RESPONSE;

/**
    Set user name command request structure.
*/
typedef struct {
    /// User id.
    UINT8   UserId : 6;
    UINT8   Reserved : 2;   ///< Reserved.
    UINT8   UserName[16];   ///< User name.
} AMI_SET_USER_NAME_REQUEST;

/**
    Set user password operation type.
*/
typedef enum {
    AmiSetUserPasswordOperationDisableUser,  ///< Disable User operation.
    AmiSetUserPasswordOperationEnableUser,   ///< Enable User operation.
    AmiSetUserPasswordOperationSetPassword,  ///< Set Password operation.
    AmiSetUserPasswordOperationTestPassword  ///< Test Password operation.
} AMI_SET_USER_PASSWORD_OPERATION;

/**
    Set user password size.
*/
typedef enum {
    AmiSetPasswordSizeSixteenByte,  ///< Sixteen Byte.
    AmiSetPasswordSizeTwentyByte    ///< Twenty Byte.
} AMI_SET_USER_PASSWORD_SIZE;

/**
    Set user password command request structure with 20 byte password.
*/
typedef struct {
    /// User Id.
    UINT8   UserId:6;
    UINT8   Reserved1:1;        ///< Reserved.
    UINT8   PasswordSize:1;     ///< Password size.
    UINT8   Operation:2;        ///< Operation.
    UINT8   Reserved2:6;        ///< Reserved.
    UINT8   PasswordData[20];   ///< Password.
} AMI_SET_USER_PASSWORD_REQUEST;

/**
    SSIF read/write support.
*/
typedef enum {
    AmiSsifSinglePartRw,           ///< Single Part read-write.
    AmiSsifMultiPartRw,            ///< Multi Part read-write.
    AmiSsifMultiPartRwWithMiddle,  ///< Multi Part read-write With Middle.
    AmiSsifReserved                ///< Reserved.
} AMI_SSIF_READ_WRITE_SUPPORT;

/**
    Channel medium type.
*/
typedef enum {
    AmiChannelMediumTypeReserved,    ///< Reserved.
    AmiChannelMediumTypeIpmb,        ///< Ipmb.
    AmiChannelMediumTypeIcmb1_0,     ///< Icmb1_0.
    AmiChannelMediumTypeIcmb0_9,     ///< Icmb0_9.
    AmiChannelMediumType802_3_Lan,   ///< 802_3_Lan.
    AmiChannelMediumTypeRs_232,      ///< Rs_232.
    AmiChannelMediumTypeOtherLan,    ///< OtherLan.
    AmiChannelMediumTypePciSmBus,    ///< PciSmBus.
    AmiChannelMediumTypeSmBus1_0,    ///< SmBus1_0.
    AmiChannelMediumTypeSmBus2_0,    ///< SmBus2_0.
    AmiChannelMediumTypeUsb1_x,      ///< Usb1_x.
    AmiChannelMediumTypeUsb2_x,      ///< Usb2_x.
    AmiChannelMediumTypeBmc          ///< Bmc.
} AMI_CHANNEL_MEDIUM_TYPE;

/**
    Bmc communication address structure.
*/
typedef struct {
    /// BMC slave address.
    UINT8   SlaveAddress;
    UINT8   ChannelAddress; ///< Channel address.
} AMI_SM_IPMI_ADDRESS;

/**
    Host to Bmc communication address.
*/
typedef struct {
    /// Channel type.
    AMI_CHANNEL_MEDIUM_TYPE     ChannelType;
    AMI_SM_IPMI_ADDRESS         BmcAddress;     ///< Host to Bmc communication address type.
} AMI_SM_COM_ADDRESS;

/**
    Get message flag command response structure.
*/
typedef struct {
    /// Completion code.
    UINT8          CompletionCode;
    UINT8          ReceiveMessageAvailable : 1; ///< Receive Message Available.
    UINT8          EventMessageBufferFull : 1;  ///< Event Message Buffer Full
    UINT8          Reserved1 : 1;               ///< Reserved
    UINT8          WatchdogPreTimeoutInt : 1;   ///< Watchdog PreTimeout Int
    UINT8          Reserved2 : 1;               ///< Reserved
    UINT8          Oem0DataAvailable : 1;       ///< Oem0DataAvailable
    UINT8          Oem1DataAvailable : 1;       ///< Oem1DataAvailable
    UINT8          Oem2DataAvailable : 1;       ///< Oem2DataAvailable
} AMI_IPMI_GET_MESSAGE_FLAGS_RESPONSE;

/**
    Channnel states.
*/
typedef enum {
    AmiDisbleChannel = 0,   ///< Disble Channel.
    AmiEnableChannel,       ///< Enable Channel.
    AmiGetChannelState,     ///< Get Channel State.
    AmiChannelStateReserved ///< Channel State Reserved.
} AMI_CHANNEL_STATE;

/**
    Enable message channel command request structure.
*/
typedef struct {
    /// Channel Number.
    UINT8          ChannelNumber : 4;
    UINT8          Reserved1 : 4;       ///< Reserved.
    UINT8          ChannelState : 2;    ///< Channel State.
    UINT8          Reserved2 : 6;       ///< Reserved.
} AMI_IPMI_ENABLE_MESSAGE_CHANNEL_REQUEST;

/**
    Enable message channel command response structure.
*/
typedef struct {
    /// Completion code.
    UINT8          CompletionCode;
    UINT8          ChannelNumber : 4;   ///< Channel Number.
    UINT8          Reserved1 : 4;       ///< Reserved.
    UINT8          ChannelState : 1;    ///< Channel State.
    UINT8          Reserved2 : 7;       ///< Reserved.
} AMI_IPMI_ENABLE_MESSAGE_CHANNEL_RESPONSE;

/**
    Get message  command response structure.
*/
typedef struct {
    /// Channel Number.
    UINT8         ChannelNumber : 4;
    UINT8         PrivilegeLevel : 4;       ///< Privilege Level.
    UINT8         RequesterLun : 2;         ///<SMS LUN is 10b, Response will be available in Receive Message Queue.
    UINT8         NetFn : 6;                ///< NetFunction.
    UINT8         CheckSum1;                ///< CheckSum.
    UINT8         RequesterSlaveAddress;    ///< Requester Slave Address.
    UINT8         ResponderLun : 2;         ///< Responder Lun.
    UINT8         SequenceNumber : 6;       ///< SequenceNumber. 
    UINT8         Command;                  ///< Command.
    UINT8         CompletionCode;           ///< CompletionCode
    //UINT8         Data [];                ///<Data is varying length, Frame date while sending command
    //UINT8         CheckSum2;
} AMI_IPMI_GET_MESSAGE_RESPONSE;

/**
    Get message  command request structuree.
*/
typedef struct {
    /// Channel Number.
    UINT8         ChannelNumber : 4;
    UINT8         Athentication : 1;       ///< Athentication.
    UINT8         Encrption : 1;           ///< Encrption.
    UINT8         Tracking : 2;            ///< Tracking.
    UINT8         ResponderSlaveAddess;    ///< Responder slave address.
    UINT8         ResponderLun : 2;        ///< Responder LUN.
    UINT8         NetFn : 6;               ///< NetFunction.
    UINT8         CheckSum1;               ///< 2's complement checksum for <ResSlaveAddress + NetFn + Lun>.
    UINT8         RequesterSlaveAddress;   ///< BMC Slave address 0x20.
    UINT8         RequesterLun : 2;        ///< SMS LUN is 10b, Response will be available in Receive Message Queue.
    UINT8         SequenceNumber : 6;      ///< Sequence number for tracking the messages in Receive Message Queue.
    UINT8         Command;
    //UINT8         Data [];               //Data is varying length, Frame date while sending command
    //UINT8         CheckSum2              //2's complement checksum for <Req Slave Address + Requester LUN + Seq Number + Cmd + Data>
} AMI_IPMI_SEND_MESSAGE_REQUEST;

/**
    Set System Info Parameters Command.
*/
#define IPMI_APP_SET_SYSTEM_INFO 0x58

/**
    System Info String Encoding.
*/
typedef enum {
    AmiSysInfoAscii,    ///< Ascii
    AmiSysInfoUtf8,     ///< Utf8
    AmiSysInfoUnicode   ///< Unicode
} AMI_SYSTEM_INFO_STRING_ENCODING;

/**
    System parameter selector.
*/
typedef enum {
    AmiSysInfoSetInProgress,    ///< SetInProgress.
    AmiSysInfoFirmwareVersion,  ///< FirmwareVersion.
    AmiSysInfoSystemName,       ///< SystemName.
    AmiSysInfoPrimaryOsName,    ///< PrimaryOsName.
    AmiSysInfoPresentOsName,    ///< PresentOsName.
    AmiSysInfoPresentOsVersion, ///< PresentOsVersion.
    AmiSysInfoBmcUrl,           ///< BmcUrl.
    AmiSysInfoHyperviserUrl,    ///< HyperviserUrl.
} AMI_SYSTEM_INFO_PARAMETER_SELECTOR;

/**
    System info set state.
*/
typedef enum {
    AmiSysInfoStateSetComplete,     ///< SetComplete.
    AmiSysInfoStateSetInProgress,   ///< SetInProgress.
    AmiSysInfoStateCommitWrite,     ///< StateCommitWrite.
    AmiSysInfoStateReserved,        ///< StateReserved.
} AMI_SYSTEM_INFO_SET_STATE;

/**
    Set system info parameter command request Structure.
*/
typedef struct {
    /// Parameter selector.
    UINT8           ParamSelector;
    UINT8           SetSelector;          ///< Data 1
    UINT8           Data[16];             ///< Data 2:17
} AMI_SET_SYSTEM_INFO_REQUEST;

/**
    Get System Info Parameters Command.
*/
#define IPMI_APP_GET_SYSTEM_INFO 0x59

/**
    Get system info Command request Structure.
*/
typedef struct {
    /// Reserved.
    UINT8           Reserved : 7;
    UINT8           GetParam : 1;   ///< Get Parameter.
    UINT8           ParamSelector;  ///< Parameter Selector.
    UINT8           SetSelector;    ///< Set selector.
    UINT8           BlockSelector;  ///< Block selector.
} AMI_GET_SYSTEM_INFO_REQUEST;

/**
    Get system info command response Structure.
*/
typedef struct {
    /// Completion code.
    UINT8           CompletionCode;
    UINT8           ParamRevision;      /// Parameter Revision
    union {
        struct {
            UINT8       State : 2;      ///< State.
            UINT8       Reserved : 6;   ///< Reserved.
        }Progress;
        UINT8          SetSelector;     ///< Set Selector.
    }Data1;
    UINT8           Data[16];        ///< Data 2:17.
} AMI_GET_SYSTEM_INFO_RESPONSE;

/**
    Get system Guid Command response Structure.
*/
typedef struct {
    /// Completion code.
    UINT8       CompletionCode;
    UINT8       Node[6];           ///< Node.
    UINT8       Clock[2];          ///< Clock.
    UINT16      Time_High;         ///< Time High.
    UINT16      Time_Mid;          ///< Time Middle.
    UINT32      Time_Low;          ///< Time Low.
} AMI_GET_SYSTEM_GUID_RESPONSE;

/**
    Get Bt interface Capability Command response Structure.
*/
typedef struct {
    /// Completion code.
    UINT8       CompletionCode;
    UINT8       OutstaningReq;      ///<Number of outstanding requests supported.
    UINT8       InputBuffSize;      ///<Input (request) buffer message size in bytes.
    UINT8       OutputBuffSize;     ///<Output (response) buffer message size in bytes.
    UINT8       BmcReqToResTime;    ///<BMC Request-to-Response time, in seconds.
    UINT8       RecommandedRetires; ///<Recommended retries (1 based
} AMI_IPMI_BT_INTERFACE_CAPABILITY_RES;

/**
    Get System interface Capability Command request Structure.
*/
typedef struct {
    /// System Interface Type 0h = SSIF,1h = KCS ,2h = SMIC.
    UINT8       SystemInterfaceType:4;
    UINT8       Reserved:4;             ///< Reserved.
} AMI_IPMI_GET_SYSTEM_INTERFACE_CAPABILITY_REQ;

/**
    Get System interface Capability Command response Structure.
*/
typedef struct {
    /// Completion code.
    UINT8       CompletionCode;
    UINT8       Reserved;               ///< Reserved.
    UINT8       SsifVersion : 3;        ///< Ssif Version.
    UINT8       PecSupport : 1;         ///< Pec Support.
    UINT8       Reserved1 : 2;          ///< Reserved.
    UINT8       TransactionSupport : 2; ///< Transaction Support.
    UINT8       InputMessageSize;       ///< Input Message Size.
    UINT8       OutputMessageSize;      ///< Output Message Size.
} AMI_IPMI_GET_SYSTEM_INTERFACE_CAPABILITY_RES;

/**
    Get BMC global Enable Command response Structure.
*/
typedef struct {
    /// Completion code.
    UINT8       CompletionCode;
    UINT8       ReceiveMsgQueueInterrupt : 1;     ///< Receive Message Queue Interrupt Enable bit.
    UINT8       EventMsgBufferFullInterrupt : 1;  ///< Event Message Buffer Full Interrupt Enable bit.
    UINT8       EventMsgBuffer : 1;               ///< Event Message Buffer Enable bit.
    UINT8       SystemEventLogging : 1;           ///< System Event Logging Enable bit.
    UINT8       Reserved : 1;                     ///< Reserved.
    UINT8       OEM0 : 1;                         ///< oem0 Enable bit.
    UINT8       OEM1 : 1;                         ///< oem1 Enable bit.
    UINT8       OEM2 : 1;                         ///< Oem2 Enable bit.
} AMI_IPMI_BMC_GLOBAL_ENABLES_RES;
#pragma pack()

//---------------------------------------------------------------------------

#endif  // #ifndef _AMI_IPMI_NETFN_APP_DEFINITIONS_H_

