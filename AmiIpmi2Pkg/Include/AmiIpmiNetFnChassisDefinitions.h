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

/** @file AmiIpmiNetFnChassisDefinitions.h
    Ipmi NetFn Chassis additional commands and its structures.

*/

#ifndef _AMI_IPMI_NETFN_CHASSIS_DEFINITIONS_H_
#define _AMI_IPMI_NETFN_CHASSIS_DEFINITIONS_H_

//---------------------------------------------------------------------------

#include <IndustryStandard/IpmiNetFnChassis.h>

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

#define AMI_BOOT_INFO_HANDLED_BY_BIOS           (1 << 0)  // BIT0
#define AMI_BOOT_INFO_HANDLED_BY_OS_LOADER      (1 << 1)  // BIT1
#define AMI_BOOT_INFO_HANDLED_BY_OS             (1 << 2)  // BIT2
#define AMI_BOOT_INFO_HANDLED_BY_SMS            (1 << 3)  // BIT3
#define AMI_BOOT_INFO_HANDLED_BY_OEM            (1 << 4)  // BIT4


/**
    Power Restore Policy.
*/

typedef enum {
    AmiPolicyPowerOff,                  ///< Chassis stays powered off after AC/mains returns.
    AmiPolicyLastPowerState,            ///< After AC returns, power is restored to the state that was in effect when AC/mains was lost.
    AmiPolicyPowerRestore,              ///< Chassis always powers up after AC/mains returns.
    AmiPolicyUnknown                    ///< Unknown
} AMI_IPMI_BMC_POWER_RESTORE_POLICY;
/**
    Boot Options parameter types.
*/
typedef enum {
    AmiBootOptionParamSetInProgress,            ///< Set In Progress.
    AmiBootOptionParamServicePartitionSelector, ///< Service Partition Selector.
    AmiBootOptionParamServicePartitionScan,     ///< Service Partition Scan.
    AmiBootOptionParamBmcBootFlagValidBit,      ///< Bmc Boot Flag Valid Bit.
    AmiBootOptionParamBootInfoAck,              ///< Boot Info Ack.
    AmiBootOptionParamBootFlags,                ///< Boot Flags.
    AmiBootOptionParamBootInitiatorInfo,        ///< Boot Initiator Info.
    AmiBootOptionParamBootInitiatorMailbox      ///< Boot Initiator Mailbox.
} AMI_BOOT_OPTION_PARAMETER_TYPE;

/**
    Parameter 5 data 2 Force Boot Device Selection.
*/
typedef enum {
    AmiIpmiNoOverride,                      ///< No Override.
    AmiIpmiForcePxe,                        ///< Force Pxe.
    AmiIpmiForceBootHardDrive,              ///< Force Boot Hard Drive.
    AmiIpmiForceBootHardDriveSafeMode,      ///< Force Boot Hard Drive Safe Mode.
    AmiIpmiForceBootDiagnosticPartition,    ///< Force Boot DiagnosticPartition.
    AmiIpmiForceBootCdDvd,                  ///< Force Boot Cd/Dvd.
    AmiIpmiForceBootBiosSetup,              ///< Force Boot Bios Setup.
    AmiIpmiForceBootRemoteFloppy,           ///< Force Boot Remote Floppy.
    AmiIpmiForceBootRemoteCdDvd,            ///< Force Boot Remote Cd/Dvd.
    AmiIpmiForceBootPrimaryRemoteMedia,     ///< Force Boot Primary Remote Media.
    AmiIpmiForceBootRemoteHardDrive = 0xB,  ///< Force Boot Remote Hard Drive.
    AmiIpmiForceBootFloppy = 0xF            ///< Force Boot Floppy.
} AMI_IPMI_BOOT_DEVICE_SELECTOR;

#pragma pack (1)

/**
    Set/Get boot options related definitions.
*/

/**
    Boot Options Parameter# 5 - Boot Flags.
    It is same as IPMI_BOOT_OPTIONS_RESPONSE_PARAMETER_5.
    Since Data 5 and other members are not defined, using below structure.
*/
typedef struct {
    /// Reserved.
    // Data 1
    UINT8   Reserved:5;
    UINT8   BiosBootType:1;             ///< Bios boot type.
    UINT8   Persistent:1;               ///< Persistent bit.
    UINT8   BootFlagValid:1;            ///< Boot flag valid bit.
    // Data 2
    UINT8   LockoutResetButtons:1;      ///< Lock out reset buttons bit.
    UINT8   ScreenBlank:1;              ///< Screen blank bit.
    UINT8   BootDeviceSelector:4;       ///< Boot device selector.
    UINT8   LockKeyboard:1;             ///< Lock keyboard bit.
    UINT8   CmosClear:1;                ///< Cmos clear bit.
    // Data 3
    UINT8   ConsoleRedirection:2;       ///< Console redirection.
    UINT8   LockOutSleepButton:1;       ///< Lock out sleep button bit.
    UINT8   UserPasswordBypass:1;       ///< User password bypass bit.
    UINT8   ForceProgressEventTrap:1;   ///< Force progress trap bit.
    UINT8   FirmwareVerbosity:2;        ///< Verbosity.
    UINT8   LockoutPowerButton:1;       ///< Lock out power button bit.
    // Data 4
    UINT8   BiosMuxCtrlOverride:3;      ///< Bios Mux control override.
    UINT8   BiosSharedModeOverride:1;   ///< Bios shared mode override.
    UINT8   Reserved1:4;                ///< Reserved.
    // Data 5
    UINT8   DeviceInstanceSelector:5;   ///< Device instance selector.
    UINT8   Reserved2:3;                ///< Reserved.
} AMI_BOOT_OPTIONS_BOOT_FLAGS;

/**
    Get boot options response structure.
*/
typedef struct {
    /// Completion code.
    UINT8   CompletionCode;
    UINT8   ParameterVersion:4;     ///< Parameter version.
    UINT8   Reserved:4;             ///< Reserved.
    UINT8   ParameterSelector:7;    ///< Paramter selector.
    UINT8   ParameterValid:1;       ///< Parameter valid.
} AMI_GET_BOOT_OPTIONS_RESPONSE;

/**
    Get set in progress response structure.
*/
typedef struct {
    /// Response data.
    AMI_GET_BOOT_OPTIONS_RESPONSE           Response;
    /// Set in progress response.
    IPMI_BOOT_OPTIONS_RESPONSE_PARAMETER_0  Param0;
} AMI_GET_BOOT_OPTIONS_SET_IN_PROGRESS_RESPONSE;

/**
    Set system boot options common request structure.
*/
typedef struct {
    /// Parameter selector.
    UINT8   ParameterSelector:7;
    UINT8   MarkParameterInvalid:1;     ///< Mark parameter as invalid.
} AMI_SET_BOOT_OPTIONS_REQUEST;

/**
    Set boot info acknowledge request structure.
*/
typedef struct {
    /// Request data.
    AMI_SET_BOOT_OPTIONS_REQUEST            Request;
    /// Boot info acknowledge data.
    IPMI_BOOT_OPTIONS_RESPONSE_PARAMETER_4  Param4;
} AMI_SET_BOOT_OPTIONS_BOOT_INFO_ACK_REQUEST;

/**
    Set boot info acknowledge response structure.
*/
typedef struct {
    /// Response data.
    AMI_GET_BOOT_OPTIONS_RESPONSE           Response;
    /// Boot info acknowledge data.
    IPMI_BOOT_OPTIONS_RESPONSE_PARAMETER_4  Param4;
} AMI_GET_BOOT_OPTIONS_BOOT_INFO_ACK_RESPONSE;

/**
    Get boot flag response structure.
*/
typedef struct {
    /// Response data.
    AMI_GET_BOOT_OPTIONS_RESPONSE   Response;
    AMI_BOOT_OPTIONS_BOOT_FLAGS     Param5;     ///< Boot flags.
} AMI_GET_BOOT_OPTIONS_BOOT_FLAG_RESPONSE;

/**
    Set boot flag request structure.
*/
typedef struct {
    /// Request data.
    AMI_SET_BOOT_OPTIONS_REQUEST    Request;
    AMI_BOOT_OPTIONS_BOOT_FLAGS     Param5;     ///< Boot flags.
} AMI_SET_BOOT_OPTIONS_BOOT_FLAG_REQUEST;

/**
    Get chassis status command request structure.
*/
typedef struct {
    /// Completion code.
    UINT8   CompletionCode;
    struct {
        UINT8   PowerIsOn : 1;              ///< Power Is On.
        UINT8   PowerOverLoad : 1;          ///< Power OverLoad.
        UINT8   InterLock : 1;              ///< InterLock.
        UINT8   PowerFault : 1;             ///< Power Fault.
        UINT8   PowerControlFault : 1;      ///< Power Control Fault.
        UINT8   PowerRestorePolicy : 2;     ///< Power Restore Policy.
        UINT8   Reserved : 1;               ///< Reserved.
    } CurrentPowerState;                    ///< Current Power State.
    UINT8   LastPowerEvent;                 ///< Last Power Event.
    UINT8   MiscChassisState;               ///< Misc Chassis State.
    UINT8   FrontPanelButtonCapabilities;   ///< Front Panel Button Capabilities.
} AMI_IPMI_GET_CHASSIS_STATUS_RESPONSE;

/**
    Set power restore policy request structure.
*/
typedef struct {
    ///< Power Restore Policy.
    UINT8   PowerRestorePolicy:3;
    UINT8   Reserved:5;             ///< Reserved.
} AMI_IPMI_SET_POWER_RESTORE_POLICY_REQ;

/**
    Set power restore policy response structure.
*/
typedef struct {
    /// Completion code.
    UINT8   CompletionCode;
    UINT8   Support:3;      ///< power restore policy support.
    UINT8   Reserved:5;     ///< Reserved.
} AMI_IPMI_SET_POWER_RESTORE_POLICY_RES;

#pragma pack()

//---------------------------------------------------------------------------

#endif  // #ifndef _IPMI_NETFN_CHASSIS_DEFINITIONS_H_

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
