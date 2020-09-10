//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

/** @file PeiIpmiInitialize.c
    Pei Ipmi Transport module.

*/

//---------------------------------------------------------------------------

#include "PeiIpmiInitialize.h"

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

extern INIT_HOST_BMC_INTERFACE INIT_HOST_BMC_INTERFACE_HOOK_LIST \
                                    EndOfInitHostBmcInterfaceHookList;
INIT_HOST_BMC_INTERFACE             *gInitHostBmcInterfaceHookList[] = { \
                                        INIT_HOST_BMC_INTERFACE_HOOK_LIST \
                                        NULL \
                                        };

extern BMC_INIT_DELAY_HOOK BMC_INIT_DELAY_HOOK_LIST EndOfBmcInitDelayHookList;
BMC_INIT_DELAY_HOOK                 *gBmcInitDelayHookList[] = { \
                                        BMC_INIT_DELAY_HOOK_LIST \
                                        NULL \
                                        };
static EFI_PEI_NOTIFY_DESCRIPTOR    gNotifyList[] = {
                                        (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | \
                                        EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
                                        &gEfiPeiMemoryDiscoveredPpiGuid,
                                        UpdateIpmiInstancePtr
                                        };

//---------------------------------------------------------------------------

/** @internal
    This routine calls the InitLpcBmc function and other platform hooks.

    @param[in] PeiServices   Describes the list of possible PEI Services.

    @return EFI_STATUS  Return Status of the hooks.

**/
EFI_STATUS
InitHostBmcInterfaceHook (
  IN CONST EFI_PEI_SERVICES     **PeiServices )
{
    EFI_STATUS  Status;
    UINTN       Index;

    for(Index = 0; gInitHostBmcInterfaceHookList[Index]; Index++) {
        Status = gInitHostBmcInterfaceHookList[Index] (PeiServices);
        if (EFI_ERROR(Status)) {
            break;
        }
    }

    return Status;
}

/** @internal
    This routine calls the BmcInitDelay function or other platform hooks.

    @param[in] PeiServices   Describes the list of possible PEI Services.

    @retval TRUE     If AC power failure occurred.
    @retval FALSE    If AC power failure does not occurred.

**/
BOOLEAN
BmcInitDelayHook (
  IN CONST EFI_PEI_SERVICES     **PeiServices )
{
    BOOLEAN     IsPowerFail;
    UINTN       Index;

    for(Index = 0; gBmcInitDelayHookList[Index]; Index++) {
        IsPowerFail = gBmcInitDelayHookList[Index] (PeiServices);
    }

    return IsPowerFail;
}

/** @internal
    Performs self test and installs Pei Ipmi transport Ppi.

    @param[in] PeiServices          Describes the list of possible PEI
                                    Services.
    @param[in, out] IpmiInstance    Pointer to Ipmi Instance data.

    @retval EFI_OUT_OF_RESOURCES    Failed to allocate memory.
    @retval EFI_UNSUPPORTED         Self test command is failed.
    @return EFI_SUCCESS             Self test command is passed and Pei Ipmi
                                    transport Ppi is installed successfully.

**/
EFI_STATUS
IpmiInstallPpi (
  IN CONST EFI_PEI_SERVICES             **PeiServices,
  IN OUT   AMI_IPMI_BMC_INSTANCE_DATA   *IpmiInstance )
{
    EFI_STATUS                          Status;
    EFI_PEI_PPI_DESCRIPTOR              *PeiIpmiPpiDesc = NULL;
    IPMI_SELF_TEST_RESULT_RESPONSE      BstStatus;
    IPMI_GET_DEVICE_ID_RESPONSE         GetDeviceIdResponse;

    IpmiInstance->IpmiTransport.Revision            = EFI_IPMI_PROTOCOL_REVISION;

    Status = AmiIpmiGetSelfTestResults (
                &IpmiInstance->IpmiTransport,
                &BstStatus );
    DEBUG ((DEBUG_INFO, "%a BMC Status %x\n", __FUNCTION__, IpmiInstance->BmcStatus));

    if (!EFI_ERROR (Status)) {
        // Now install the PPI if the BMC is not in a Hard Fail State.
        if (IpmiInstance->BmcStatus != AmiBmcStatusHardFail) {
            /* The Device ID information is only retrieved when the self test
               has passed or generated a SOFTFAIL. The BMC Status may be
               overwritten to show the BMC is in Force Update mode. But the BMC
               Status will always be preserved if a hard fail has occurred since
               this code won't execute.*/
            AmiIpmiGetDeviceId (
                &IpmiInstance->IpmiTransport,
                &GetDeviceIdResponse );

            // Allocate memory for Ppi Descriptor.
            PeiIpmiPpiDesc = (EFI_PEI_PPI_DESCRIPTOR*)AllocateZeroPool (sizeof (EFI_PEI_PPI_DESCRIPTOR));
            if (PeiIpmiPpiDesc == NULL) {
                DEBUG ((DEBUG_ERROR, "%a - Failed to allocate memory for PeiIpmiPpiDesc! \n", __FUNCTION__));
                // Failed to allocate memory, so return EFI_OUT_OF_RESOURCES.
                FreePool (IpmiInstance);
                return EFI_OUT_OF_RESOURCES;
            }

            PeiIpmiPpiDesc->Flags   = EFI_PEI_PPI_DESCRIPTOR_PPI | \
                                        EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST;
            PeiIpmiPpiDesc->Guid    = &gEfiPeiIpmiTransportPpiGuid;
            PeiIpmiPpiDesc->Ppi     = &IpmiInstance->IpmiTransport;
            Status = (*PeiServices)->InstallPpi (
                                        PeiServices,
                                        PeiIpmiPpiDesc );
            DEBUG ((DEBUG_INFO, "%a - gEfiPeiIpmiTransportPpiGuid PPI  Status %r\n", __FUNCTION__, Status));
        }
    }

    return Status;
}

/**
    Notify callback function for interfaces.

    @param[in] PeiServices          Describes the list of possible PEI
                                    Services.
    @param[in] NotifyDescriptor     Pointer to notify descriptor.
    @param[in] Ppi                  Pointer to Ppi.

    @return EFI_STATUS              Status of Notify call back.
    @retval EFI_NOT_FOUND           Ipmi hob is not found.
    @retval EFI_OUT_OF_RESOURCES    Failed to allocate memory.
    @retval EFI_SUCCESS             Interface is initialized and installed
                                    Ipmi Ppi successfully.
    @retval Others                  Error status while installing Ppi.

**/
EFI_STATUS
EFIAPI
NotifyCallback (
  IN EFI_PEI_SERVICES             **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR    *NotifyDescriptor,
  IN VOID                         *Ppi )
{
    EFI_STATUS                    Status = EFI_SUCCESS;
    AMI_IPMI_BMC_INSTANCE_DATA    *IpmiInstance;
    AMI_PEI_IPMI_DATA_HOB         *IpmiInstancePtrHob;
    EFI_HOB_GUID_TYPE             *GuidHob;

    DEBUG ((DEBUG_INFO, "%a - Entry\n", __FUNCTION__));

    GuidHob    = GetFirstGuidHob (&gAmiPeiIpmiHobGuid);

    ASSERT (GuidHob != NULL);
    if (GuidHob == NULL) {
        return EFI_NOT_FOUND;
    }

    IpmiInstancePtrHob = (AMI_PEI_IPMI_DATA_HOB *) GET_GUID_HOB_DATA(GuidHob);
    IpmiInstance = (AMI_IPMI_BMC_INSTANCE_DATA*)IpmiInstancePtrHob->IpmiInstance;

    Status = AmiIpmiInitInterfaceData (IpmiInstance);

    // Return if there is no memory exists to allocate.
    if (Status == EFI_OUT_OF_RESOURCES) {
        return Status;
    }

    switch (DEFAULT_SYSTEM_INTERFACE) {

        case AmiSysInterfaceKcs:
            if ((IpmiInstance->Interface.Kcs != NULL) && \
                (IpmiInstance->Interface.Kcs->InterfaceState != AmiIpmiInterfaceInitialized)) {
                DEBUG ((DEBUG_INFO, "AmiSysInterfaceKcs InterfaceNotReady in %a \n", __FUNCTION__));
                return Status;
            }
            break;

        case AmiSysInterfaceBt:
            if (IpmiInstance->Interface.Bt.InterfaceState != AmiIpmiInterfaceInitialized) {
                DEBUG ((DEBUG_INFO, "AmiSysInterfaceBt InterfaceNotReady in %a \n", __FUNCTION__));
                return Status;
            }
            break;

        case AmiSysInterfaceSsif:
            if (IpmiInstance->Interface.Ssif.InterfaceState != AmiIpmiInterfaceInitialized) {
                DEBUG ((DEBUG_INFO, "AmiSysInterfaceSsif InterfaceNotReady in %a \n", __FUNCTION__));
                return Status;
            }
            break;

        case AmiSysInterfaceIpmb:
            if (IpmiInstance->Interface.Ipmb.InterfaceState != AmiIpmiInterfaceInitialized) {
                DEBUG ((DEBUG_INFO, "AmiSysInterfaceIpmb InterfaceNotReady in %a \n", __FUNCTION__));
                return Status;
            }
            break;

        case AmiSysInterfaceUsb:
            if ((IpmiInstance->Interface.Usb != NULL) && (IpmiInstance->Interface.Usb->InterfaceState != AmiIpmiInterfaceInitialized)) {
                DEBUG ((DEBUG_INFO, "AmiSysInterfaceUsb InterfaceNotReady in %a \n", __FUNCTION__));
                return Status;
            }
            break;

        case AmiSysInterfaceOem1:
            if (IpmiInstance->Interface.Oem1.InterfaceState != AmiIpmiInterfaceInitialized) {
                DEBUG ((DEBUG_INFO, "AmiSysInterfaceOem1 InterfaceNotReady in %a \n", __FUNCTION__));
                return Status;
            }
            break;

        case AmiSysInterfaceOem2:
            if (IpmiInstance->Interface.Oem2.InterfaceState != AmiIpmiInterfaceInitialized) {
                DEBUG ((DEBUG_INFO, "AmiSysInterfaceOem2 InterfaceNotReady in %a \n", __FUNCTION__));
                return Status;
            }
            break;

        default:
            break;
    }

    if (IpmiInstance->IpmiTransport.Revision != EFI_IPMI_PROTOCOL_REVISION) {
        Status = IpmiInstallPpi (
                    (CONST EFI_PEI_SERVICES **)PeiServices,
                    IpmiInstance );
        DEBUG ((DEBUG_INFO, "%a - IpmiInstallPpi Status = %r\n", __FUNCTION__, Status));
    }
    
    DEBUG ((DEBUG_INFO, "%a Exit\n", __FUNCTION__));
    
    return Status;
}

/** @internal
    Registers callback for Ppi.

    @param[in] PeiServices  Describes the list of possible PEI Services.
    @param[in] PpiGuid      Pointer to Ppi guid to register call back.

    @retval EFI_INVALID_PARAMETER   Invalid parameter.
    @retval EFI_OUT_OF_RESOURCES    Failed to allocate memory.
    @retval Others                  Status of NotifyPpi().

**/
EFI_STATUS
RegisterPpiCallback (
  IN CONST EFI_PEI_SERVICES     **PeiServices,
  IN       EFI_GUID             *PpiGuid )
{
    EFI_STATUS                  Status;
    EFI_PEI_NOTIFY_DESCRIPTOR   *PpiNotifyDesc;
    EFI_GUID                    NullGuid;

    ZeroMem (
       &NullGuid,
       sizeof(EFI_GUID) );

    if ((PpiGuid == NULL) || ((PpiGuid != NULL) && \
        CompareGuid (
            PpiGuid,
            &NullGuid ))) {
        return EFI_INVALID_PARAMETER;
    }

    PpiNotifyDesc = (EFI_PEI_NOTIFY_DESCRIPTOR*)AllocateZeroPool (sizeof (EFI_PEI_NOTIFY_DESCRIPTOR));
    if (PpiNotifyDesc == NULL) {
        return EFI_OUT_OF_RESOURCES;
    }

    PpiNotifyDesc->Flags    = EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | \
                                EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST;
    PpiNotifyDesc->Guid     = PpiGuid;
    PpiNotifyDesc->Notify   = NotifyCallback;

    Status = (*PeiServices)->NotifyPpi (
                                PeiServices,
                                PpiNotifyDesc );

    return Status;
}

/**
    After memory is discovered, update the Ipmi Instance pointer in Hob.

    @param[in] PeiServices      Describes the list of possible PEI Services.
    @param[in] NotifyDescriptor Pointer to notify descriptor.
    @param[in] Ppi              Pointer to Ppi.

    @retval EFI_NOT_FOUND   Ipmi hob is not found.
    @retval EFI_ABORTED     IpmiInstance pointer is null in Hob.
    @retval EFI_SUCCESS     Updated the Ipmi instance pointer successfully.

**/
EFI_STATUS
EFIAPI
UpdateIpmiInstancePtr (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi )
{
    AMI_IPMI_BMC_INSTANCE_DATA    *IpmiInstance;
    EFI_HOB_GUID_TYPE             *GuidHob;
    AMI_PEI_IPMI_DATA_HOB         *IpmiInstancePtrHob;
    
    GuidHob = GetFirstGuidHob (&gAmiPeiIpmiHobGuid);

    ASSERT (GuidHob != NULL);
    if (GuidHob == NULL) {
        return EFI_NOT_FOUND;
    }

    IpmiInstancePtrHob = (AMI_PEI_IPMI_DATA_HOB *) GET_GUID_HOB_DATA (GuidHob);
    IpmiInstance = (AMI_IPMI_BMC_INSTANCE_DATA*)IpmiInstancePtrHob->IpmiInstance;

    if (!IpmiInstance) {
        return EFI_ABORTED;
    }
    DEBUG ((DEBUG_INFO, "IpmiInstance Signature in CAR Memory: %x\n", IpmiInstance->Signature));

    if ((UINTN)IpmiInstancePtrHob >= (UINTN)IpmiInstancePtrHob->PreMemIpmiDataHobPtr) {
        IpmiInstance = (AMI_IPMI_BMC_INSTANCE_DATA *) ((UINT8 *)IpmiInstance + \
                        (UINTN)((UINTN)IpmiInstancePtrHob - (UINTN)IpmiInstancePtrHob->PreMemIpmiDataHobPtr));

        if (IpmiInstance->Interface.Kcs) {
            IpmiInstance->Interface.Kcs = \
                    (AMI_KCS_SYSTEM_INTERFACE*)((UINT8 *)IpmiInstance->Interface.Kcs + \
                    (UINTN)((UINTN)IpmiInstancePtrHob - (UINTN)IpmiInstancePtrHob->PreMemIpmiDataHobPtr));
        }

        if (IpmiInstance->Interface.Usb) {
            IpmiInstance->Interface.Usb = \
                    (AMI_USB_SYSTEM_INTERFACE*)((UINT8 *)IpmiInstance->Interface.Usb + \
                    (UINTN)((UINTN)IpmiInstancePtrHob - (UINTN)IpmiInstancePtrHob->PreMemIpmiDataHobPtr));
            if (IpmiInstance->Interface.Usb->UsbCommBufferPtr) {
                IpmiInstance->Interface.Usb->UsbCommBufferPtr = \
                        IpmiInstance->Interface.Usb->UsbCommBufferPtr + \
                        (UINTN)((UINTN)IpmiInstancePtrHob - (UINTN)IpmiInstancePtrHob->PreMemIpmiDataHobPtr);
            }
        }
    } else {
        IpmiInstance = (AMI_IPMI_BMC_INSTANCE_DATA *)((UINT8 *)IpmiInstance - \
                       (UINTN)((UINTN)IpmiInstancePtrHob->PreMemIpmiDataHobPtr - (UINTN)IpmiInstancePtrHob));

        if (IpmiInstance->Interface.Kcs) {
            IpmiInstance->Interface.Kcs = \
                    (AMI_KCS_SYSTEM_INTERFACE*)((UINT8 *)IpmiInstance->Interface.Kcs - \
                    (UINTN)((UINTN)IpmiInstancePtrHob->PreMemIpmiDataHobPtr - (UINTN)IpmiInstancePtrHob));
        }

        if (IpmiInstance->Interface.Usb) {
            IpmiInstance->Interface.Usb = \
                    (AMI_USB_SYSTEM_INTERFACE*)((UINT8 *)IpmiInstance->Interface.Usb - \
                    (UINTN)((UINTN)IpmiInstancePtrHob->PreMemIpmiDataHobPtr - (UINTN)IpmiInstancePtrHob));
            if (IpmiInstance->Interface.Usb->UsbCommBufferPtr) {
                IpmiInstance->Interface.Usb->UsbCommBufferPtr = \
                        IpmiInstance->Interface.Usb->UsbCommBufferPtr - \
                        (UINTN)((UINTN)IpmiInstancePtrHob->PreMemIpmiDataHobPtr - (UINTN)IpmiInstancePtrHob);
            }
        }
    }

    IpmiInstancePtrHob->IpmiInstance = (UINTN)IpmiInstance;
    DEBUG ((DEBUG_INFO, "IpmiInstance Signature after Permanent Memory discovered: %x\n", IpmiInstance->Signature));

    return EFI_SUCCESS;
}

/**
    Setup and initialize the BMC for the PEI phase. In order to
    verify the BMC is functioning as expected, the BMC Self-test is
    performed. The results are then checked and any errors are reported to
    the error manager. Errors are collected throughout this routine and
    reported just prior to installing the driver. If there are more errors
    than BST_LOG_SIZE, then they will be ignored.

    @param[in] FileHandle    Pointer to image file handle.
    @param[in] PeiServices   Describes the list of possible PEI Services.

    @retval EFI_SUCCESS             Successful driver initialization.
    @retval EFI_UNSUPPORTED         BmcSupport setup option is disabled.
    @retval EFI_OUT_OF_RESOURCES    Failed to allocate memory.
    @retval Others                  Error status while initializing the
                                    driver.

**/
EFI_STATUS
InitializeIpmiPhysicalLayer (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices )
{
    EFI_STATUS                          Status = EFI_SUCCESS;
    AMI_IPMI_BMC_INSTANCE_DATA          *IpmiInstance = NULL;
    EFI_PEI_READ_ONLY_VARIABLE2_PPI     *PeiVariable;
    SERVER_MGMT_CONFIGURATION_DATA      ServerMgmtConfiguration;
    AMI_IPMI_INTERFACE_STATE            InterfaceState = AmiIpmiInterfaceNotReady;
    AMI_PEI_IPMI_DATA_HOB               *IpmiInstancePtrHob;
    UINTN                               Size;
    UINT8                               Index;

    // Locate the Setup configuration value.
    Status = (*PeiServices)->LocatePpi (
                                PeiServices,
                                &gEfiPeiReadOnlyVariable2PpiGuid,
                                0,
                                NULL,
                                (VOID **)&PeiVariable );

    if (EFI_ERROR(Status)) {
        DEBUG ((DEBUG_ERROR, "%a - Locate gEfiPeiReadOnlyVariable2PpiGuid Status %r\n", __FUNCTION__, Status));
        return Status;
    }

    Size = sizeof(SERVER_MGMT_CONFIGURATION_DATA);
    
    Status = PeiVariable->GetVariable (
                            PeiVariable,
                            L"ServerSetup",
                            &gEfiServerMgmtSetupVariableGuid,
                            NULL,
                            &Size,
                            &ServerMgmtConfiguration );

    if (EFI_ERROR(Status)) {
        DEBUG ((DEBUG_ERROR, "%a - PeiVariable->GetVariable Status %r\n", __FUNCTION__, Status));
        // Load default values.
        ServerMgmtConfiguration.BmcSupport = DEFAULT_BMC_SUPPORT;
      
        if (BMC_INIT_DELAY) {
            ServerMgmtConfiguration.WaitForBmc = DEFAULT_WAITFORBMC_POLICY;
        }
    }

    if (!ServerMgmtConfiguration.BmcSupport) {
        return EFI_UNSUPPORTED;
    }

    // Platform Hook to initialize BMC and Host interface.
    Status = InitHostBmcInterfaceHook (PeiServices);
    
    DEBUG ((DEBUG_INFO, "%a - InitHostBmcInterfaceHook %r! \n", __FUNCTION__, Status));

    if (EFI_ERROR(Status)) {
        return Status;
    }

    if (BMC_INIT_DELAY) {
        /* Add 30 Sec delay before sending Self Test command during First AC
           Power Cycle On. BMC starts simultaneously with BIOS so BMC takes
           30 Sec to respond. Give a delay of 30 Sec if the Power Failure
           (PWR_FLR) bit is set and WaitForBmc Setup option is enabled.*/
        if (ServerMgmtConfiguration.WaitForBmc && BmcInitDelayHook (PeiServices)) {
            DEBUG ((DEBUG_INFO, "%a - Giving BMC_INIT_FIXED_DELAY second delay.. start \n", __FUNCTION__));
            MicroSecondDelay (BMC_INIT_FIXED_DELAY);
            DEBUG ((DEBUG_INFO, "%a - Giving BMC_INIT_FIXED_DELAY second delay.. end \n", __FUNCTION__));
        }
    }

    // Allocate memory for IPMI Instance
    IpmiInstance = (AMI_IPMI_BMC_INSTANCE_DATA*)AllocateZeroPool (sizeof (AMI_IPMI_BMC_INSTANCE_DATA));
    if (IpmiInstance == NULL) {
        DEBUG ((DEBUG_ERROR, "%a - Failed to allocate memory for IpmiInstance! \n", __FUNCTION__));
        return EFI_OUT_OF_RESOURCES;
    }

    //Check for Memory initialization 
    Status = (*PeiServices)->LocatePpi (
                                 PeiServices,
                                 &gEfiPeiMemoryDiscoveredPpiGuid,
                                 0,
                                 NULL,
                                 NULL );

    if (EFI_ERROR(Status)) {
        /* Register Memory discovered PPI call back to update HOB with new
        IpmiInstance pointer.*/
        Status = (*PeiServices)->NotifyPpi (
                                    PeiServices,
                                    &gNotifyList[0] );
        if (EFI_ERROR(Status)) {
            // Failed to Register Memory discovered PPI call back
            DEBUG ((DEBUG_ERROR, "Failed to Register Memory discovered PPI call back. %r\n", Status));
            FreePool (IpmiInstance);
            return Status;
        }
    }

    Status = AmiIpmiInitInterfaceData (IpmiInstance);

    // Return if there is no memory found to allocate.
    if (Status == EFI_OUT_OF_RESOURCES) {
        FreePool (IpmiInstance);
        return Status;
    }

    /* Check default interface data initialized successfully else register
       notify Ppi.*/
    for (Index = AmiSysInterfaceKcs; Index < AmiSysInterfaceMax; Index++) {
        switch (Index) {
            case AmiSysInterfaceKcs:
                if ((IpmiInstance->Interface.Kcs != NULL) && \
                    (IpmiInstance->Interface.Kcs->InterfaceState == AmiIpmiInterfaceInitialized) && \
                    (DEFAULT_SYSTEM_INTERFACE == AmiSysInterfaceKcs)) {
                    InterfaceState = AmiIpmiInterfaceInitialized;
                    DEBUG ((DEBUG_INFO, "%a - Default AmiSysInterfaceKcs InterfaceInitialized\n", __FUNCTION__)); 
                }
                break;

            case AmiSysInterfaceBt:
                if ((IpmiInstance->Interface.Bt.InterfaceState == AmiIpmiInterfaceInitialized) && \
                    (DEFAULT_SYSTEM_INTERFACE == AmiSysInterfaceBt)) {
                    InterfaceState = AmiIpmiInterfaceInitialized;
                    DEBUG ((DEBUG_INFO, "%a - Default AmiSysInterfaceBt InterfaceInitialized\n", __FUNCTION__)); 
                }
                break;

            case AmiSysInterfaceSsif:
                if ((IpmiInstance->Interface.Ssif.InterfaceState == AmiIpmiInterfaceInitialized) && \
                    (DEFAULT_SYSTEM_INTERFACE == AmiSysInterfaceSsif)) {
                    InterfaceState = AmiIpmiInterfaceInitialized;
                    DEBUG ((DEBUG_INFO, "%a - Default AmiSysInterfaceSsif InterfaceInitialized\n", __FUNCTION__)); 
                } else if (IpmiInstance->Interface.Ssif.InterfaceState == AmiIpmiInterfaceInitError) {
                    // Register PPI notify for SMBUS PPI
                    Status = RegisterPpiCallback (
                                PeiServices,
                                &IpmiInstance->Interface.Ssif.SsifInterfaceApiGuid );
                    DEBUG ((DEBUG_INFO, "%a - SMBUS PPI is not ready. RegisterPpiCallback Status: %r\n", __FUNCTION__, Status));
                }
                break;

            case AmiSysInterfaceIpmb:
                if ((IpmiInstance->Interface.Ipmb.InterfaceState == AmiIpmiInterfaceInitialized) && \
                    (DEFAULT_SYSTEM_INTERFACE == AmiSysInterfaceIpmb)) {
                    InterfaceState = AmiIpmiInterfaceInitialized;
                    DEBUG ((DEBUG_INFO, "%a - Default AmiSysInterfaceIpmb InterfaceInitialized\n", __FUNCTION__)); 
                } else if (IpmiInstance->Interface.Ipmb.InterfaceState == AmiIpmiInterfaceInitError) {
                    // Register PPI notify for I2C PPI
                    Status = RegisterPpiCallback (
                                PeiServices,
                                &IpmiInstance->Interface.Ipmb.IpmbInterfaceApiGuid );
                    DEBUG ((DEBUG_INFO, "%a - I2C PPI is not ready. RegisterPpiCallback Status: %r\n", __FUNCTION__, Status));
                }
                break;

            case AmiSysInterfaceUsb:
                if ((IpmiInstance->Interface.Usb != NULL) && \
                    (IpmiInstance->Interface.Usb->InterfaceState == AmiIpmiInterfaceInitialized) && \
                    (DEFAULT_SYSTEM_INTERFACE == AmiSysInterfaceUsb)) {
                    InterfaceState = AmiIpmiInterfaceInitialized;
                    DEBUG ((DEBUG_INFO, "%a - Default AmiSysInterfaceUsb InterfaceInitialized\n", __FUNCTION__)); 
                } else if ((IpmiInstance->Interface.Usb != NULL) && \
                        (IpmiInstance->Interface.Usb->InterfaceState == AmiIpmiInterfaceInitError)) {
                    // Register PPI notify for USB PPI
                    Status = RegisterPpiCallback (
                                PeiServices,
                                &IpmiInstance->Interface.Usb->UsbInterfaceApiGuid );
                    DEBUG ((DEBUG_INFO, "%a - USB PPI is not ready. RegisterPpiCallback Status: %r\n", __FUNCTION__, Status));
                }
                break;

            case AmiSysInterfaceOem1:
                if ((IpmiInstance->Interface.Oem1.InterfaceState == AmiIpmiInterfaceInitialized) && \
                    (DEFAULT_SYSTEM_INTERFACE == AmiSysInterfaceOem1)) {
                    InterfaceState = AmiIpmiInterfaceInitialized;
                    DEBUG ((DEBUG_INFO, "%a - Default AmiSysInterfaceOem1 InterfaceInitialized\n", __FUNCTION__)); 
                } else if (IpmiInstance->Interface.Oem1.InterfaceState == AmiIpmiInterfaceInitError) {
                    // Register PPI notify for OEM PPI
                    Status = RegisterPpiCallback (
                                PeiServices,
                                &IpmiInstance->Interface.Oem1.Oem1InterfaceApiGuid );
                    DEBUG ((DEBUG_INFO, "%a - OEM1 PPI is not ready. RegisterPpiCallback Status: %r\n", __FUNCTION__, Status));
                }
                break;

            case AmiSysInterfaceOem2:
                if ((IpmiInstance->Interface.Oem2.InterfaceState == AmiIpmiInterfaceInitialized) && \
                    (DEFAULT_SYSTEM_INTERFACE == AmiSysInterfaceOem2)) {
                    InterfaceState = AmiIpmiInterfaceInitialized;
                    DEBUG ((DEBUG_INFO, "%a - Default AmiSysInterfaceOem2 InterfaceInitialized\n", __FUNCTION__)); 
                } else if (IpmiInstance->Interface.Oem2.InterfaceState == AmiIpmiInterfaceInitError) {
                    // Register PPI notify for OEM PPI
                    Status = RegisterPpiCallback (
                                PeiServices,
                                &IpmiInstance->Interface.Oem2.Oem2InterfaceApiGuid );
                    DEBUG ((DEBUG_INFO, "%a - OEM2 PPI is not ready. RegisterPpiCallback Status: %r\n", __FUNCTION__, Status));
                }
                break;

            default:
                break;
        }
    }

    // Create Guided hob to pass IPMI Instance data pointer to notify functions.
    IpmiInstancePtrHob = BuildGuidHob (
                              &gAmiPeiIpmiHobGuid,
                              sizeof(AMI_PEI_IPMI_DATA_HOB) );
    IpmiInstancePtrHob->IpmiInstance = (UINTN)IpmiInstance;
    IpmiInstancePtrHob->PreMemIpmiDataHobPtr = IpmiInstancePtrHob;

    // Default Interface data should be initialized to install Ipmi transport Ppi.
    if (InterfaceState != AmiIpmiInterfaceInitialized) {
        DEBUG ((DEBUG_INFO, "Interface not ready yet.\n"));  
        return EFI_SUCCESS;
    }

    // Install Ipmi transport Ppi.
    Status = IpmiInstallPpi (
                PeiServices,
                IpmiInstance );

    DEBUG ((DEBUG_INFO, "%a - IpmiInstallPpi Status = %r\n", __FUNCTION__, Status)); 

    return Status;
}

