//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************


/** @file IpmiInfoProtocol.h
    IPMI info protocol definitions

**/

#ifndef _IPMI_INFO_PROTOCOL_H_
#define _IPMI_INFO_PROTOCOL_H_

//----------------------------------------------------------------------

//----------------------------------------------------------------------
#define MAX_TIME_OUT_POLICY_VALUE 3
#define MAX_ENABLE_DISABLE_VALUE 1

typedef struct {
    /// Device Id of BMC.
    UINT8       BmcDeviceId;
    UINT8       BmcDeviceRevision;      ///< BMC Device Revision.
    UINT8       BmcFwMajorVersion;      ///< Major BMC Firmware version.
    UINT8       BmcFwMinorVersion;      ///< Minor BMC Firmware version.
    UINT8       SpecificationVersion;   ///< IPMI Specification Version.
    UINT8       DefaultInterfaceUsed;   ///< Interface used in IPMI 1.kcs 3.Bt 4.ssif
    UINT32      BmcSelfTestStatus;      ///< Bmc Self Test Status
}AMI_BASIC_BMC_INFO;

typedef struct {
    /// Ipmi BMC support.
    UINT8       BmcSupport;
    UINT8       Frb2Enable;         ///< Define Frb2 is enabled or disabled.
    UINT8       Frb2TimerOutPolicy; ///< Frb2 Timer-out policy.
    UINT8       Frb2TimerOutValue;  ///< Frb2 Timer-out value.
    UINT8       Frb3Enable;         ///< Define Frb3 is enabled or disabled.
    UINT8       Frb3TimerOutPolicy; ///< Frb3 Timer-out policy.
    UINT8       Frb3TimerOutValue;  ///< Frb3 Timer-out value.
} AMI_IPMI_SETUP_POLICY;

/**
    This API gets the Bmc basic information .

    @param[in] [out] BmcInfo            Pointer to the AMI_BASIC_BMC_INFO.

    @retval EFI_SUCCESS             Data successfully retrieved and copied
                                    to structure.
    @retval Others                  Error status from functions calls used.

**/
typedef
EFI_STATUS
(EFIAPI *AMI_GET_BMC_INFO) (
  IN OUT AMI_BASIC_BMC_INFO     *BmcInfo
);

/**
    This API gets the Ipmi setup policy.

    @param[in] [out] IpmiPolicy     Pointer to the AMI_IPMI_SETUP_POLICY.

    @retval EFI_SUCCESS             Data successfully retrieved and copied
                                    to structure.
    @retval Others                  Error status from functions calls used.

**/
typedef
EFI_STATUS
(EFIAPI *AMI_GET_IPMI_SETUP_POLICY) (
  IN OUT AMI_IPMI_SETUP_POLICY       *IpmiPolicy
);

/**
    This API sets the Ipmi setup policy.

    @param[in] [out] IpmiPolicy     Pointer to the AMI_IPMI_SETUP_POLICY.

    @retval EFI_SUCCESS             Data successfully retrieved and copied
                                    to structure.
    @retval Others                  Error status from functions calls used.

**/
typedef
EFI_STATUS
(EFIAPI *AMI_SET_IPMI_SETUP_POLICY) (
  IN OUT AMI_IPMI_SETUP_POLICY       *IpmiPolicy
);

/**
   IPMI Information PROTOCOL
*/
typedef struct _AMI_IPMI_INFO_PROTOCOL{
    /// Version of infoProtocol
    UINTN                       Version;
    AMI_GET_BMC_INFO            GetBmcInfo;  ///< Get Basic IPMI module and Bmc information.
    AMI_GET_IPMI_SETUP_POLICY   GetPolicy;   ///< Get Ipmi setup policy details.
    AMI_SET_IPMI_SETUP_POLICY   SetPolicy;   ///< Set Ipmi setup policy details.
} AMI_IPMI_INFO_PROTOCOL;

extern EFI_GUID gAmiIpmiInfoProtocolGuid;

//----------------------------------------------------------------------

#endif

