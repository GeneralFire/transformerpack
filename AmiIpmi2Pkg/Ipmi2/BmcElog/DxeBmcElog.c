//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************


/** @file DxeBmcElog.c
    Server Management BMC ELOG Driver. This driver is able to talk to the
    IPMI aware controller and logs the data into a SEL repository.

*/

//---------------------------------------------------------------------------

#include "BmcElogCommon.h"

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

BMC_ELOG_INSTANCE_DATA  *gRedirProtoPrivate = NULL;
EFI_IPMI_TRANSPORT      *gIpmiTransport = NULL;

//---------------------------------------------------------------------------

/**
    This is the standard EFI driver point.
    1. Get device ID information from BMC & check for SEL device support.
    2. Install the Redir Elog protocol for EfiElogRedirSmIPMI data type
    3. Activate BMC event logging capability.

    @param[in] ImageHandle  Handle of this driver image.
    @param[in] SystemTable  Table containing standard EFI services.

    @retval EFI_SUCCESS             Successful driver initialization.
    @retval EFI_UNSUPPORTED         Sel is not supported or invalid spec version.
    @retval EFI_OUT_OF_RESOURCES    Failed to allocate memory.
    @retval Others                  Error status returned from functions used.

**/
EFI_STATUS
EFIAPI
InitializeDxeBmcElog (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable )
{
    EFI_HANDLE                     NewHandle;
    EFI_STATUS                     Status;
    IPMI_GET_DEVICE_ID_RESPONSE    GetDeviceIdResponse;
    BOOLEAN                        EnableElog = TRUE;
    BOOLEAN                        ElogStatus = TRUE;

    DEBUG ((DEBUG_INFO, "%a Entry... \n", __FUNCTION__));

    Status = gBS->LocateProtocol (
                    &gEfiDxeIpmiTransportProtocolGuid,
                    NULL,
                    (VOID **)&gIpmiTransport );
    
    DEBUG ((DEBUG_INFO, " gEfiDxeIpmiTransportProtocolGuid Status: %r \n", Status));
    
    if (EFI_ERROR (Status)) {
        return Status;
    }

    // Get the device ID information from the BMC.
    Status = AmiIpmiGetDeviceId (
                gIpmiTransport,
                &GetDeviceIdResponse );
    if (EFI_ERROR (Status)) {
        return Status;
    }

    DEBUG ((DEBUG_INFO, "GetDeviceIdResponse.DeviceSupport.Bits.SelDeviceSupport %X\n", GetDeviceIdResponse.DeviceSupport.Bits.SelDeviceSupport));
    if (!GetDeviceIdResponse.DeviceSupport.Bits.SelDeviceSupport) {
        DEBUG ((DEBUG_ERROR, "SEL device is not supported. So returning EFI_UNSUPPORTED \n"));
        return EFI_UNSUPPORTED;
    }

    DEBUG ((DEBUG_INFO, "GetDeviceIdResponse.SpecificationVersion %X\n", GetDeviceIdResponse.SpecificationVersion));
    if (!((GetDeviceIdResponse.SpecificationVersion == AMI_IPMI_SPEC_VERSION_1_5) || \
            (GetDeviceIdResponse.SpecificationVersion == AMI_IPMI_SPEC_VERSION_2_0))) {
        DEBUG ((DEBUG_ERROR, "IPMI Spec version is not supported. So returning EFI_UNSUPPORTED \n"));
        return EFI_UNSUPPORTED;
    }

    // Allocate memory for Redir Protocol private data.
    gRedirProtoPrivate = AllocateRuntimeZeroPool (sizeof (BMC_ELOG_INSTANCE_DATA));
    if (gRedirProtoPrivate == NULL) {
        DEBUG ((DEBUG_ERROR, "Failed to allocate memory for gRedirProtoPrivate! \n"));
        return EFI_OUT_OF_RESOURCES;
    }

    // Fill the Redir Protocol data.
    gRedirProtoPrivate->Signature                   = EFI_SM_ELOG_REDIR_SIGNATURE;
    gRedirProtoPrivate->DataType                    = EfiElogRedirSmIPMI;
    gRedirProtoPrivate->BmcElog.ActivateEventLog    = EfiActivateBmcElog;
    gRedirProtoPrivate->BmcElog.EraseEventLogData   = EfiEraseBmcElogData;
    gRedirProtoPrivate->BmcElog.GetEventLogData     = EfiGetBmcElogData;
    gRedirProtoPrivate->BmcElog.SetEventLogData     = EfiSetBmcElogData;

    // Install the protocol.
    NewHandle = NULL;
    Status = gBS->InstallProtocolInterface (
                    &NewHandle,
                    &gEfiRedirElogProtocolGuid,
                    EFI_NATIVE_INTERFACE,
                    &gRedirProtoPrivate->BmcElog );
    
    DEBUG ((DEBUG_INFO, "gEfiRedirElogProtocolGuid protocol  status %r\n", Status));
    
    if (EFI_ERROR (Status)) {
        // Error in installing the protocol. So free the allocated memory.
        FreePool (gRedirProtoPrivate);
        return Status;
    }

    // Activate BMC event logging capability.
    EfiActivateBmcElog (
        &gRedirProtoPrivate->BmcElog,
        EfiElogRedirSmIPMI,
        &EnableElog,
        &ElogStatus );

    DEBUG ((DEBUG_INFO, "%a Exiting... \n", __FUNCTION__));

    return EFI_SUCCESS;
}


