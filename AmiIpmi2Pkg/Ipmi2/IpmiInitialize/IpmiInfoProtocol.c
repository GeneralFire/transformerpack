//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

/** @file IpmiInfoProtocol.c
   Install Ipmi Info protocol.

**/

//----------------------------------------------
#include <DxeIpmiInitialize.h>
#include <Protocol/IpmiInfoProtocol.h>


#if defined(DxeFrb_SUPPORT)
#define FRB_DISABLED 0
#else
#define FRB_DISABLED 1
#endif

//----------------------------------------------
AMI_IPMI_INFO_PROTOCOL *gIpmiInfoProtocol = NULL;
UINT32                  gCrcValue = 0;

/** @internal
    Function to get basic BMC info.

    @param[in] [out] BmcInfo    Pointer to the AMI_BASIC_BMC_INFO.

    @return EFI_STATUS  Status returned from functions used.

**/
EFI_STATUS
EFIAPI
AmiGetBmcInfo (
  IN OUT AMI_BASIC_BMC_INFO     *BmcInfo)
{
    EFI_STATUS                          Status ;
    EFI_BMC_SELF_TEST_LOG_PROTOCOL     *BmcSelfTestProtocol = NULL;

    if (!BmcInfo)
        return EFI_INVALID_PARAMETER;

    Status = gBS->LocateProtocol (
                   &gEfiBmcSelfTestLogProtocolGuid,
                   NULL,
                   (VOID **)&BmcSelfTestProtocol );

    if (!EFI_ERROR (Status)){
        if (BmcSelfTestProtocol->IsGetDeviceIdCommandPassed) {
            BmcInfo->BmcDeviceId = BmcSelfTestProtocol->GetDeviceIdResponse.DeviceId;
            BmcInfo->BmcDeviceRevision = BmcSelfTestProtocol->GetDeviceIdResponse.DeviceRevision.Bits.DeviceRevision;
            BmcInfo->BmcFwMajorVersion = BmcSelfTestProtocol->GetDeviceIdResponse.FirmwareRev1.Bits.MajorFirmwareRev;
            BmcInfo->BmcFwMinorVersion = BmcSelfTestProtocol->GetDeviceIdResponse.MinorFirmwareRev;
            BmcInfo->SpecificationVersion = BmcSelfTestProtocol->GetDeviceIdResponse.SpecificationVersion;
        }
        BmcInfo->DefaultInterfaceUsed = DEFAULT_SYSTEM_INTERFACE ;
        BmcInfo->BmcSelfTestStatus = BmcSelfTestProtocol->BmcStatus;
        DEBUG ((DEBUG_INFO, " Basic info data filled  \n"));
    }
   return Status;
}

/** @internal
    Function to  get the Ipmi setup policy.

    @param  [in] [out] IpmiPolicy     Pointer to the AMI_IPMI_SETUP_POLICY.

    @return EFI_STATUS  Status returned from functions used.

**/
EFI_STATUS
EFIAPI
AmiIpmiGetPolicy (
   IN OUT AMI_IPMI_SETUP_POLICY       *IpmiPolicy)
{
    EFI_STATUS                      Status ;
    UINTN                           Size;
    UINT32                          Attributes;
    SERVER_MGMT_CONFIGURATION_DATA  ServerMgmtConfiguration;

    if ((!IpmiPolicy) || (FRB_DISABLED))
        return EFI_INVALID_PARAMETER;

    Size = sizeof (SERVER_MGMT_CONFIGURATION_DATA);
    Status = gRT->GetVariable (
                    L"ServerSetup",
                    &gEfiServerMgmtSetupVariableGuid,
                    &Attributes,
                    &Size,
                    &ServerMgmtConfiguration );

    if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, " gEfiServerMgmtSetupVariableGuid  GetVariable Status: %r \n", Status));
        return Status;
    }

    IpmiPolicy->BmcSupport = ServerMgmtConfiguration.BmcSupport;
#if (!FRB_DISABLED)
    IpmiPolicy->Frb2Enable = ServerMgmtConfiguration.Frb2Enable;
    IpmiPolicy->Frb2TimerOutPolicy = ServerMgmtConfiguration.Frb2TimeoutPolicy ;
    IpmiPolicy->Frb2TimerOutValue = ServerMgmtConfiguration.Frb2Timeout ;
    IpmiPolicy->Frb3Enable = ServerMgmtConfiguration.OsBootWdtEnable;
    IpmiPolicy->Frb3TimerOutPolicy = ServerMgmtConfiguration.OsBootWdtTimeoutPolicy;
    IpmiPolicy->Frb3TimerOutValue = ServerMgmtConfiguration.OsBootWdtTimeout ;
#endif
    Size = sizeof(AMI_IPMI_SETUP_POLICY);
    Status = gBS->CalculateCrc32 (
                    (UINT8*)IpmiPolicy,
                    Size,
                    &gCrcValue );

    DEBUG ((DEBUG_ERROR, " gBS->CalculateCrc32  Status: %r  gCrcValue %x \n", Status,gCrcValue));

   return Status;
}

/** @internal
    Function to  Set the Ipmi setup policy.

    @param  [in] [out] IpmiPolicy     Pointer to the AMI_IPMI_SETUP_POLICY.

    @return EFI_STATUS  Status returned from functions used.

**/
EFI_STATUS
EFIAPI
AmiIpmiSetPolicy (
   IN OUT AMI_IPMI_SETUP_POLICY       *IpmiPolicy)
{
    
    EFI_STATUS                      Status ;
    UINTN                           Size;
    UINT32                          Attributes;
    SERVER_MGMT_CONFIGURATION_DATA  ServerMgmtConfiguration;
    UINT32                          CrcValue = 0;

    if ((!IpmiPolicy) || (FRB_DISABLED))
        return EFI_INVALID_PARAMETER;

    if ((IpmiPolicy->BmcSupport > MAX_ENABLE_DISABLE_VALUE) || (IpmiPolicy->Frb2Enable > MAX_ENABLE_DISABLE_VALUE) || (IpmiPolicy->Frb3Enable > MAX_ENABLE_DISABLE_VALUE))
        return EFI_INVALID_PARAMETER;

    if ((IpmiPolicy->Frb2TimerOutPolicy > MAX_TIME_OUT_POLICY_VALUE) || (IpmiPolicy->Frb3TimerOutPolicy > MAX_TIME_OUT_POLICY_VALUE))
        return EFI_INVALID_PARAMETER;

    if ((IpmiPolicy->Frb2TimerOutValue < 1 || IpmiPolicy->Frb2TimerOutValue > 30) || \
        (IpmiPolicy->Frb3TimerOutValue < 1 || IpmiPolicy->Frb3TimerOutValue > 30)) 
        return EFI_INVALID_PARAMETER;

    Size = sizeof(AMI_IPMI_SETUP_POLICY);
    Status = gBS->CalculateCrc32 (
                    (UINT8*)IpmiPolicy,
                    Size,
                    &CrcValue );
    if((CrcValue == gCrcValue) || EFI_ERROR (Status)){
        DEBUG ((DEBUG_ERROR, "   Status of CalculateCrc32 %r NO Change in Policy  gCrcValue()%x CrcValue()%x \n",Status,gCrcValue,CrcValue));
        return Status;
    }

    Size = sizeof (SERVER_MGMT_CONFIGURATION_DATA);
    Status = gRT->GetVariable (
                        L"ServerSetup",
                        &gEfiServerMgmtSetupVariableGuid,
                        &Attributes,
                        &Size,
                        &ServerMgmtConfiguration);

    if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, " gEfiServerMgmtSetupVariableGuid  GetVariable Status: %r \n", Status));
        return Status;
    }
 
    ServerMgmtConfiguration.BmcSupport = IpmiPolicy->BmcSupport;
#if (!FRB_DISABLED)
    ServerMgmtConfiguration.Frb2Enable = IpmiPolicy->Frb2Enable;
    ServerMgmtConfiguration.OsBootWdtEnable = IpmiPolicy->Frb3Enable;
    DEBUG ((DEBUG_INFO, " ServerMgmtConfiguration Enable/disable parameter filled  \n"));

    ServerMgmtConfiguration.Frb2TimeoutPolicy = IpmiPolicy->Frb2TimerOutPolicy;
    ServerMgmtConfiguration.OsBootWdtTimeoutPolicy = IpmiPolicy->Frb3TimerOutPolicy;
    DEBUG ((DEBUG_INFO, " ServerMgmtConfiguration Timeout policy parameter filled  \n"));

    ServerMgmtConfiguration.Frb2Timeout = IpmiPolicy->Frb2TimerOutValue;
    ServerMgmtConfiguration.OsBootWdtTimeout = IpmiPolicy->Frb3TimerOutValue;
    DEBUG ((DEBUG_INFO, " ServerMgmtConfiguration Timeout  value filled  \n"));
#endif
    Status = gRT->SetVariable (
                    L"ServerSetup",
                    &gEfiServerMgmtSetupVariableGuid,
                    Attributes,   // Attributes
                    Size,
                    &ServerMgmtConfiguration );

    DEBUG ((DEBUG_INFO, " gEfiServerMgmtSetupVariableGuid  SetVariable Status: %r \n", Status));

   return Status;
}
/** @internal
    Install  Ipmi Info protocol.

    @param  VOID    Nothing.

    @return EFI_STATUS  Status returned from functions used.

**/
EFI_STATUS
IpmiInstallInfoProtocol (
   IN  EFI_HANDLE         NewHandle )
{
    EFI_STATUS                          Status ;

    if (NewHandle == NULL)
        return EFI_INVALID_PARAMETER;
 
    //
    // Allocate memory for IPMI protocol
    //
    gIpmiInfoProtocol = (AMI_IPMI_INFO_PROTOCOL*)AllocateRuntimeZeroPool (sizeof (AMI_IPMI_INFO_PROTOCOL));
    if (gIpmiInfoProtocol == NULL) {
        DEBUG ((DEBUG_ERROR, "Failed to allocate memory for gIpmiInstance! \n"));
        return EFI_OUT_OF_RESOURCES;
    }

    gIpmiInfoProtocol->GetBmcInfo = AmiGetBmcInfo ;
    gIpmiInfoProtocol->GetPolicy = AmiIpmiGetPolicy;
    gIpmiInfoProtocol->SetPolicy = AmiIpmiSetPolicy;

    Status = gBS->InstallProtocolInterface (
                        &NewHandle,
                        &gAmiIpmiInfoProtocolGuid,
                        EFI_NATIVE_INTERFACE,
                        gIpmiInfoProtocol );
    DEBUG ((DEBUG_INFO, "Install gAmiIpmiInfoProtocolGuid protocol status %r\n", Status));
    if (EFI_ERROR (Status)) {
        // Error in installing the protocol. So free the allocated memory.
        FreePool (gIpmiInfoProtocol);
    }

   return Status;
}
