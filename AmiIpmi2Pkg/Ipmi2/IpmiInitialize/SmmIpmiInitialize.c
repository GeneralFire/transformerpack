//*************************************************************************
//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************


/** @file SmmIpmiInitialize.c
    Smm Ipmi Transport Driver.

*/

//---------------------------------------------------------------------------

#include <Token.h>
#include <AmiServerMgmtSetupVariable.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/SmmServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/AmiIpmiLib.h>
#include <Library/AmiBmcCommonInterfaceLib.h>
#include <Protocol/IpmiTransportProtocol.h>

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

AMI_IPMI_BMC_INSTANCE_DATA  *gSmmIpmiInstance = NULL;

//---------------------------------------------------------------------------

/** @internal
    Performs self test and installs SMM IPMI transport Protocol.

    @param VOID     Nothing.

    @return EFI_STATUS  Return status of function calls used.

**/
EFI_STATUS
IpmiInstallSmmProtocol (
  VOID )
{
    EFI_STATUS                          Status;
    IPMI_SELF_TEST_RESULT_RESPONSE      BstStatus;
    EFI_HANDLE                          NewHandle = NULL;

    DEBUG ((DEBUG_INFO, "%a - Enter\n", __FUNCTION__));


    gSmmIpmiInstance->IpmiTransport.Revision            = EFI_IPMI_PROTOCOL_REVISION;

    Status = AmiIpmiGetSelfTestResults (
                &gSmmIpmiInstance->IpmiTransport,
                &BstStatus );

    DEBUG ((DEBUG_INFO, "SMM BMC status %x\n", gSmmIpmiInstance->BmcStatus));

    // Now install the Protocol if the BMC is not in a Hard Fail State.
    if (!EFI_ERROR (Status) && (gSmmIpmiInstance->BmcStatus != AmiBmcStatusHardFail)) {
        NewHandle = NULL;
        Status = gSmst->SmmInstallProtocolInterface (
                            &NewHandle,
                            &gEfiSmmIpmiTransportProtocolGuid,
                            EFI_NATIVE_INTERFACE,
                            &gSmmIpmiInstance->IpmiTransport );

        DEBUG ((DEBUG_INFO, "SMM gEfiSmmIpmiTransportProtocolGuid protocol status %r\n", Status));
        
        if (EFI_ERROR (Status)) {
            // Error in installing the protocol. So free the allocated memory.
            FreePool (gSmmIpmiInstance);
        }
    }
    DEBUG ((DEBUG_INFO, "%a - Exit\n", __FUNCTION__));
    return Status;
}

/**
    Notify call back to initialize the interfaces and install Smm Ipmi
    protocol.

    @param[in] Protocol     Pointer to the protocol guid.
    @param[in] Interface    Pointer to the protocol instance.
    @param[in] Handle       Handle on which the protocol is installed.
    
    @return EFI_STATUS  Status of Notify call back.

**/
EFI_STATUS
EFIAPI
SmmNotifyCallback (
  IN CONST  EFI_GUID    *Protocol,
  IN        VOID        *Interface,
  IN        EFI_HANDLE  Handle )
{
    EFI_STATUS  Status = EFI_SUCCESS;

    DEBUG ((DEBUG_INFO, "%a - Entry\n", __FUNCTION__));
    
    Status = AmiIpmiInitInterfaceData (gSmmIpmiInstance);
    
    // Return if there is no memory found to allocate.
    if (Status == EFI_OUT_OF_RESOURCES) {
        return Status;
    }

    switch (DEFAULT_SYSTEM_INTERFACE) {

        case AmiSysInterfaceKcs:
            if ((gSmmIpmiInstance->Interface.Kcs != NULL) && \
                (gSmmIpmiInstance->Interface.Kcs->InterfaceState != AmiIpmiInterfaceInitialized)) {
                DEBUG ((DEBUG_INFO, "AmiSysInterfaceKcs AmiIpmiInterfaceNotReady in %a \n", __FUNCTION__));
                return Status;
            }
            break;

        case AmiSysInterfaceBt:
            if (gSmmIpmiInstance->Interface.Bt.InterfaceState != AmiIpmiInterfaceInitialized) {
                DEBUG ((DEBUG_INFO, "AmiSysInterfaceBt AmiIpmiInterfaceNotReady in %a \n", __FUNCTION__));
                return Status;
            }
            break;

        case AmiSysInterfaceSsif:
            if (gSmmIpmiInstance->Interface.Ssif.InterfaceState != AmiIpmiInterfaceInitialized) {
              DEBUG ((DEBUG_INFO, "AmiSysInterfaceSsif AmiIpmiInterfaceNotReady in %a \n", __FUNCTION__));
              return Status;
            }
            break;

        case AmiSysInterfaceIpmb:
            if (gSmmIpmiInstance->Interface.Ipmb.InterfaceState != AmiIpmiInterfaceInitialized) {
              DEBUG ((DEBUG_INFO, "AmiSysInterfaceIpmb AmiIpmiInterfaceNotReady in %a \n", __FUNCTION__));
              return Status;
            }
            break;

        case AmiSysInterfaceUsb:
            if ((gSmmIpmiInstance->Interface.Usb != NULL) && \
                (gSmmIpmiInstance->Interface.Usb->InterfaceState != AmiIpmiInterfaceInitialized)) {
              DEBUG ((DEBUG_INFO, "AmiSysInterfaceUsb AmiIpmiInterfaceNotReady in %a \n", __FUNCTION__));
              return Status;
            }
            break;

        case AmiSysInterfaceOem1:
            if (gSmmIpmiInstance->Interface.Oem1.InterfaceState != AmiIpmiInterfaceInitialized) {
                DEBUG ((DEBUG_INFO, "AmiSysInterfaceOem1 AmiIpmiInterfaceNotReady in %a \n", __FUNCTION__));
                return Status;
            }
            break;

        case AmiSysInterfaceOem2:
            if (gSmmIpmiInstance->Interface.Oem2.InterfaceState != AmiIpmiInterfaceInitialized) {
                DEBUG ((DEBUG_INFO, "AmiSysInterfaceOem2 AmiIpmiInterfaceNotReady in %a \n", __FUNCTION__));
                return Status;
            }
            break;

        default:
            break;
    }

    if (gSmmIpmiInstance->IpmiTransport.Revision != EFI_IPMI_PROTOCOL_REVISION) {
        Status = IpmiInstallSmmProtocol ();
        DEBUG ((DEBUG_INFO, "%a - IpmiInstallSmmProtocol Status = %r\n", __FUNCTION__, Status));
    }

    DEBUG ((DEBUG_INFO, "%a Exit\n", __FUNCTION__));

    return Status;
}

/** @internal
    Registers Protocol call back

    @param ProtocolGuid       Pointer to Protocol GUID to register call back

    @retval EFI_INVALID_PARAMETER   If the ProtocolGuid is 0 or NULL.
    @retval Others                  Status of Notify registration.

**/
EFI_STATUS
SmmRegisterProtocolCallback (
  IN  EFI_GUID  *ProtocolGuid )
{
    EFI_STATUS  Status;
    VOID        *Registration;
    EFI_GUID    NullGuid;

    ZeroMem (
       &NullGuid,
       sizeof(EFI_GUID) );

    if ((ProtocolGuid != NULL) && CompareGuid (
                                    ProtocolGuid,
                                    &NullGuid )) {
        return EFI_INVALID_PARAMETER;
    }

    Status = gSmst->SmmRegisterProtocolNotify (
                        ProtocolGuid,
                        SmmNotifyCallback,
                        &Registration );
    return Status;
}

/**
    Setup and initialize the BMC for the SMM phase.
    1. In order to verify the BMC is functioning as expected, the BMC Self-test
    is performed. Updates the BMC status in Private data.
    2. Installs SMM IPMI transport protocol.
    3. Registers Software SMI callback to proxy the DXE calls to SMM IPMI protocol.
    This applicable only when both DXE and SMM uses same KCS interface.
    4. Notify the DxeIpmiBmc driver that SmmIpmiBmcTransport protocol is ready.

    @param[in] ImageHandle  Handle of this driver image.
    @param[in] SystemTable  Table containing standard EFI services.

    @retval EFI_SUCCESS             Successful driver initialization.
    @retval EFI_UNSUPPORTED         BmcSupport setup option is disabled.
    @retval EFI_OUT_OF_RESOURCES    Failed to allocate memory.
    @retval Others                  Error status while initializing the
                                    driver.

**/
EFI_STATUS
EFIAPI
InitializeIpmiSmmPhysicalLayer (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable )
{
    EFI_STATUS                          Status;
    SERVER_MGMT_CONFIGURATION_DATA      ServerMgmtConfiguration;
    AMI_IPMI_INTERFACE_STATE            InterfaceState = AmiIpmiInterfaceNotReady;
    UINTN                               Size;
    UINT32                              Attributes;
    UINT8                               Index;

    Size = sizeof (SERVER_MGMT_CONFIGURATION_DATA);

    Status = gRT->GetVariable (
                    L"ServerSetup",
                    &gEfiServerMgmtSetupVariableGuid,
                    &Attributes,
                    &Size,
                    &ServerMgmtConfiguration );

    DEBUG ((DEBUG_INFO, "GetVariable Status of ServerSetup %r ServerMgmtConfiguration.BmcSupport: %x \n", Status, ServerMgmtConfiguration.BmcSupport));

    if (EFI_ERROR(Status)) {
        ServerMgmtConfiguration.BmcSupport = DEFAULT_BMC_SUPPORT;
    }

    if (!ServerMgmtConfiguration.BmcSupport) {
        return EFI_UNSUPPORTED;
    }

    // Allocate memory for IPMI Instance.
    gSmmIpmiInstance = AllocateRuntimeZeroPool (sizeof (AMI_IPMI_BMC_INSTANCE_DATA));
    if (gSmmIpmiInstance == NULL) {
        DEBUG ((DEBUG_ERROR, "Failed to allocate memory for gSmmIpmiInstance! \n"));
        return EFI_OUT_OF_RESOURCES;
    }

    Status = AmiIpmiInitInterfaceData (gSmmIpmiInstance);

    // Return if there is no memory found to allocate.
    if (Status == EFI_OUT_OF_RESOURCES) {
        FreePool (gSmmIpmiInstance);
        return Status;
    }

    // Check if default interface data is initialized successfully else register notify protocol event.
    for (Index = AmiSysInterfaceKcs; Index < AmiSysInterfaceMax; Index++) {
        switch (Index) {
            case AmiSysInterfaceKcs:
                if ((gSmmIpmiInstance->Interface.Kcs != NULL) && \
                    (gSmmIpmiInstance->Interface.Kcs->InterfaceState == AmiIpmiInterfaceInitialized) && \
                    (DEFAULT_SYSTEM_INTERFACE == AmiSysInterfaceKcs)) {
                    InterfaceState = AmiIpmiInterfaceInitialized;
                    DEBUG ((DEBUG_INFO, "Default AmiSysInterfaceKcs AmiIpmiInterfaceInitialized\n")); 
                }
                break;

            case AmiSysInterfaceBt:
                if ((gSmmIpmiInstance->Interface.Bt.InterfaceState == AmiIpmiInterfaceInitialized) && \
                    (DEFAULT_SYSTEM_INTERFACE == AmiSysInterfaceBt)) {
                    InterfaceState = AmiIpmiInterfaceInitialized;
                    DEBUG ((DEBUG_INFO, "Default AmiSysInterfaceBt AmiIpmiInterfaceInitialized\n")); 
                }
                break;

            case AmiSysInterfaceSsif:
                if ((gSmmIpmiInstance->Interface.Ssif.InterfaceState == AmiIpmiInterfaceInitialized) && \
                    (DEFAULT_SYSTEM_INTERFACE == AmiSysInterfaceSsif)) {
                    InterfaceState = AmiIpmiInterfaceInitialized;
                    DEBUG ((DEBUG_INFO, "Default AmiSysInterfaceSsif AmiIpmiInterfaceInitialized\n")); 
                } else if (gSmmIpmiInstance->Interface.Ssif.InterfaceState == AmiIpmiInterfaceInitError) {
                    // Register protocol notify for Smm Smbus Protocol.
                    Status = SmmRegisterProtocolCallback (
                                &gSmmIpmiInstance->Interface.Ssif.SsifInterfaceApiGuid );
                    DEBUG ((DEBUG_INFO, "SMBUS Protocol is not ready. RegisterProtocolCallback Status: %r\n", Status));
                }
                break;

            case AmiSysInterfaceIpmb:
                if ((gSmmIpmiInstance->Interface.Ipmb.InterfaceState == AmiIpmiInterfaceInitialized) && \
                    (DEFAULT_SYSTEM_INTERFACE == AmiSysInterfaceIpmb)) {
                    InterfaceState = AmiIpmiInterfaceInitialized;
                    DEBUG ((DEBUG_INFO, "Default AmiSysInterfaceIpmb AmiIpmiInterfaceInitialized\n")); 
                } else if (gSmmIpmiInstance->Interface.Ipmb.InterfaceState == AmiIpmiInterfaceInitError) {
                    // Register Protocol notify for SMM I2C Protocol.
                    Status = SmmRegisterProtocolCallback (&gSmmIpmiInstance->Interface.Ipmb.IpmbInterfaceApiGuid);
                    DEBUG ((DEBUG_INFO, "I2C Protocol is not ready. RegisterProtocolCallback Status: %r\n", Status));
                }
                break;

            case AmiSysInterfaceUsb:
                if ((gSmmIpmiInstance->Interface.Usb != NULL) && \
                    (gSmmIpmiInstance->Interface.Usb->InterfaceState == AmiIpmiInterfaceInitialized) && \
                    (DEFAULT_SYSTEM_INTERFACE == AmiSysInterfaceUsb)) {
                    InterfaceState = AmiIpmiInterfaceInitialized;
                    DEBUG ((DEBUG_INFO, "Default AmiSysInterfaceUsb AmiIpmiInterfaceInitialized\n")); 
                }else if ((gSmmIpmiInstance->Interface.Usb != NULL) && \
                          (gSmmIpmiInstance->Interface.Usb->InterfaceState == AmiIpmiInterfaceInitError)) {
                    // Register Protocol notify for SMM USB Protocol.
                    Status = SmmRegisterProtocolCallback (
                                &gSmmIpmiInstance->Interface.Usb->UsbInterfaceApiGuid );
                    DEBUG ((DEBUG_INFO, "USB IO Protocol is not ready. RegisterProtocolCallback Status: %r\n", Status));
                }
                break;

            case AmiSysInterfaceOem1:
                if ((gSmmIpmiInstance->Interface.Oem1.InterfaceState == AmiIpmiInterfaceInitialized) && \
                    (DEFAULT_SYSTEM_INTERFACE == AmiSysInterfaceOem1)) {
                    InterfaceState = AmiIpmiInterfaceInitialized;
                    DEBUG ((DEBUG_INFO, "Default AmiSysInterfaceOem1 AmiIpmiInterfaceInitialized\n")); 
                } else if (gSmmIpmiInstance->Interface.Oem1.InterfaceState == AmiIpmiInterfaceInitError) {
                    // Register Protocol notify for SMM OEM Protocol.
                    Status = SmmRegisterProtocolCallback (
                                &gSmmIpmiInstance->Interface.Oem1.Oem1InterfaceApiGuid );
                    DEBUG ((DEBUG_INFO, "OEM1 Protocol is not ready. RegisterProtocolCallback Status: %r\n", Status));
                }
                break;

            case AmiSysInterfaceOem2:
                if ((gSmmIpmiInstance->Interface.Oem2.InterfaceState == AmiIpmiInterfaceInitialized) && \
                    (DEFAULT_SYSTEM_INTERFACE == AmiSysInterfaceOem2)) {
                    InterfaceState = AmiIpmiInterfaceInitialized;
                    DEBUG ((DEBUG_INFO, "Default AmiSysInterfaceOem2 AmiIpmiInterfaceInitialized\n")); 
                } else if (gSmmIpmiInstance->Interface.Oem2.InterfaceState == AmiIpmiInterfaceInitError) {
                    // Register Protocol notify for SMM OEM Protocol.
                    Status = SmmRegisterProtocolCallback (
                                &gSmmIpmiInstance->Interface.Oem2.Oem2InterfaceApiGuid );
                    DEBUG ((DEBUG_INFO, "OEM2 Protocol is not ready. RegisterProtocolCallback Status: %r\n", Status));
                }
                break;

            default:
                break;
        }
    }

    // Default Interface data should be initialized to install Ipmi Protocol.
    if (InterfaceState != AmiIpmiInterfaceInitialized) {
        DEBUG ((DEBUG_INFO, "Interface is not ready yet. \n"));  
        return EFI_SUCCESS;
    }

    // Install Ipmi Protocol interface.
    Status = IpmiInstallSmmProtocol ();

    DEBUG ((DEBUG_INFO, "%a - IpmiInstallSmmProtocol Status = %r\n", __FUNCTION__, Status)); 

    return Status;
}

//*************************************************************************
