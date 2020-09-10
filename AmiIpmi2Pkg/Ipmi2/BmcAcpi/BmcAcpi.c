//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************


/** @file BmcAcpi.c
    Server Management BmcAcpi Driver. This driver sets BMC ACPI device and
    power states.

*/

//---------------------------------------------------------------------------

#include <Token.h>
#include <Library/DebugLib.h>
#include <Library/SmmServicesTableLib.h>
#include <Protocol/SmmSwDispatch2.h>
#include <Protocol/IpmiTransportProtocol.h>

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

#define EFI_ACPI_POWER_STATE_S0     0x0
#define EFI_ACPI_POWER_STATE_S1     0x1
#define EFI_ACPI_POWER_STATE_S3     0x3
#define EFI_ACPI_POWER_STATE_S4     0x4
#define EFI_ACPI_POWER_STATE_S5     0x5
#define EFI_ACPI_DEVICE_STATE_D0    0x0
#define EFI_ACPI_DEVICE_STATE_D1    0x1
#define EFI_ACPI_DEVICE_STATE_D3    0x3

// These values need to be modified as per the BMC_ACPI_SWSMI SDL token.
#define BMC_ACPI_SWSMI_S0           BMC_ACPI_SWSMI + 0x00
#define BMC_ACPI_SWSMI_S1           BMC_ACPI_SWSMI + 0x01
#define BMC_ACPI_SWSMI_S3           BMC_ACPI_SWSMI + 0x03
#define BMC_ACPI_SWSMI_S4           BMC_ACPI_SWSMI + 0x04
#define BMC_ACPI_SWSMI_S5           BMC_ACPI_SWSMI + 0x05

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

static EFI_IPMI_TRANSPORT   *gIpmiTransport;

//---------------------------------------------------------------------------

/** @internal
    Sends a command to BMC to set the current device and power states.

    @param[in] PowerState   ACPI System Power State.
    @param[in] DeviceState  ACPI System Device State.

    @return VOID    Nothing.

**/
VOID
SetAcpiPowerState (
  IN UINT8  PowerState,
  IN UINT8  DeviceState )
{
    EFI_STATUS                          Status;
    IPMI_SET_ACPI_POWER_STATE_REQUEST   SetAcpiPowerStateRequest;
    UINT8                               CompletionCode;
    UINT8                               ResponseDataSize = sizeof (CompletionCode);
    SetAcpiPowerStateRequest.SystemPowerState.Bits.PowerState   = PowerState;
    SetAcpiPowerStateRequest.SystemPowerState.Bits.StateChange  = AMI_IPMI_SET_FLAG;
    SetAcpiPowerStateRequest.DevicePowerState.Bits.PowerState   = DeviceState;
    SetAcpiPowerStateRequest.DevicePowerState.Bits.StateChange  = AMI_IPMI_SET_FLAG;

    Status = gIpmiTransport->SendIpmiCommand (
                                gIpmiTransport,
                                IPMI_NETFN_APP,
                                AMI_BMC_LUN,
                                IPMI_APP_SET_ACPI_POWERSTATE,
                                (UINT8 *)&SetAcpiPowerStateRequest,
                                sizeof (SetAcpiPowerStateRequest),
                                &CompletionCode,
                                &ResponseDataSize );
    DEBUG ((DEBUG_INFO, "%a SendIpmiCommand status: %r", __FUNCTION__, Status));
}

/**
    SMI handler to set power state in BMC

    @param[in]      DispatchHandle  The unique handle assigned to this handler
                                    by SmiHandlerRegister().
    @param[in]      RegisterContext Points to an optional handler context which
                                    was specified when the handler was registered.
    @param[in, out] CommBuffer      A pointer to a collection of data in memory
                                    that will be conveyed from a non-SMM
                                    environment into an SMM environment.
    @param[in, out] CommBufferSize  The size of the CommBuffer.

    @retval EFI_SUCCESS     Given power state is set successfully.

**/
EFI_STATUS
EFIAPI
BmcAcpiSwSmiCallback (
  IN       EFI_HANDLE   DispatchHandle,
  IN CONST VOID         *RegisterContext, OPTIONAL
  IN OUT   VOID         *CommBuffer, OPTIONAL
  IN OUT   UINTN        *CommBufferSize OPTIONAL )
{
    switch (((EFI_SMM_SW_REGISTER_CONTEXT *)RegisterContext)->SwSmiInputValue) {

        case BMC_ACPI_SWSMI_S0:
            // SW ACPI SMI parameter for S0 (set S0, D0)
            SetAcpiPowerState (
                EFI_ACPI_POWER_STATE_S0,
                EFI_ACPI_DEVICE_STATE_D0 );
            break;

        case BMC_ACPI_SWSMI_S1:
            // SW ACPI SMI parameter for S1 (set S1, D0)
            SetAcpiPowerState (
                EFI_ACPI_POWER_STATE_S1,
                EFI_ACPI_DEVICE_STATE_D0 );
            break;

        case BMC_ACPI_SWSMI_S3:
            // SW ACPI SMI parameter for S3 (set S3, D3)
            SetAcpiPowerState (
                EFI_ACPI_POWER_STATE_S3,
                EFI_ACPI_DEVICE_STATE_D3 );
            break;

        case BMC_ACPI_SWSMI_S4:
            // SW ACPI SMI parameter for S4 (set S4, D3)
            SetAcpiPowerState (
                EFI_ACPI_POWER_STATE_S4,
                EFI_ACPI_DEVICE_STATE_D3 );
            break;

        case BMC_ACPI_SWSMI_S5:
            // SW ACPI SMI parameter for S5 (set S5, D3)
            SetAcpiPowerState (
                EFI_ACPI_POWER_STATE_S5,
                EFI_ACPI_DEVICE_STATE_D3 );
            break;

        default:
            break;
    }
    return EFI_SUCCESS;
}

/**
    Registers the SW SMI for Sending the Power state to BMC.

    @param ImageHandle  Handle of this driver image.
    @param SystemTable  Table containing standard EFI services.

    @retval EFI_SUCCESS Successful driver initialization

**/
EFI_STATUS
EFIAPI
InitializeBmcAcpi (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable )
{
    EFI_STATUS                        Status;
    EFI_SMM_SW_DISPATCH2_PROTOCOL     *SwDispatch = NULL;
    EFI_SMM_SW_REGISTER_CONTEXT       SwContext;
    EFI_HANDLE                        SwHandle;
    UINT8                             Index;

    Status = gSmst->SmmLocateProtocol (
                        &gEfiSmmIpmiTransportProtocolGuid,
                        NULL,
                        (VOID **)&gIpmiTransport );
    
    ASSERT_EFI_ERROR (Status);
    if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "Couldn't find SMM IPMI TRANSPORT protocol: %r\n", Status));
        return Status;
    }

    Status = gSmst->SmmLocateProtocol (
                        &gEfiSmmSwDispatch2ProtocolGuid,
                        NULL,
                        (VOID **)&SwDispatch );
    
    ASSERT_EFI_ERROR (Status);
    if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "Couldn't find SmgSwDispatch2 protocol: %r\n", Status));
        return Status;
    }

    // Register the SW SMI for Sending the Power state to BMC.
    for (Index = BMC_ACPI_SWSMI; Index < BMC_ACPI_SWSMI_MAX; Index++) {
        SwContext.SwSmiInputValue = Index;
        Status = SwDispatch->Register (
                                SwDispatch,
                                BmcAcpiSwSmiCallback,
                                &SwContext,
                                &SwHandle );
        
        if (EFI_ERROR (Status)) {
            DEBUG ((DEBUG_ERROR, "Couldn't register the BMC ACPIh SW SMI %d handler.Status: %r\n",Index,Status));
            return Status;
        }
    }
    return EFI_SUCCESS;
}

