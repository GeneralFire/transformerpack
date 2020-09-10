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

/** @file AmiIpmiBootLib.h
    AmiIpmiBootLib function declarations.

*/

#ifndef _AMI_IPMI_BOOT_LIB_H_
#define _AMI_IPMI_BOOT_LIB_H_

//---------------------------------------------------------------------------

#include <AmiIpmiNetFnChassisDefinitions.h>
#include <AmiIpmiBmcCommon.h>

//---------------------------------------------------------------------------

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
  IN AMI_IPMI_TRANSPORT_INSTANCE    *IpmiTransport
);

/** @internal
    Send the Get boot options command to get the boot flags parameter.

    @param[in]  IpmiTransport       Pointer to Ipmi transport instance.
    @param[out] BootFlagsResponse   Pointer to get the boot flags response.

    @retval EFI_INVALID_PARAMETER   Invalid parameter.
    @retval Others                  Return status of function call used.

**/
EFI_STATUS
AmiIpmiGetBootFlags (
  IN  AMI_IPMI_TRANSPORT_INSTANCE               *IpmiTransport,
  OUT AMI_GET_BOOT_OPTIONS_BOOT_FLAG_RESPONSE   *BootFlagsResponse
);

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
  OUT AMI_GET_BOOT_OPTIONS_BOOT_INFO_ACK_RESPONSE   *BootInfoAckResponse
);

//---------------------------------------------------------------------------

#endif

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
