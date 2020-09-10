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

/** @file SmmAmiKcsInterfaceLib.c
    KCS Transport Smm phase Implementation library functions.

*/

//---------------------------------------------------------------------------

#include <Token.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/AmiKcsInterfaceLib.h>
#include <Protocol/IpmiTransportProtocol.h>

//---------------------------------------------------------------------------

/** @internal
    Allocate memory for KCS interface and initialize the variables.

    @param[in, out] Interface   System interface pointer.

    @retval EFI_OUT_OF_RESOURCES    Failed to allocate memory.
    @retval EFI_SUCCESS             Interface initialized successfully.

**/
EFI_STATUS
AmiIpmiGetKcsApiPtr (
  IN OUT AMI_SYSTEM_INTERFACE   *Interface )
{
    EFI_STATUS                  Status;
    AMI_IPMI_BMC_INSTANCE_DATA  *DxeIpmiInstance;
    EFI_IPMI_TRANSPORT          *DxeIpmiTransport;

    /* If SMM and non-SMM KCS IO BASE address are same, then use DXE KCS
       Interface data "KcsTransportLocked" semaphore to avoid command loss
       in SMM.*/
    if (IPMI_KCS_BASE_ADDRESS == IPMI_SMM_KCS_BASE_ADDRESS) {
        // Locate Dxe Ipmi Transport protocol.
        Status = gBS->LocateProtocol (
                        &gEfiDxeIpmiTransportProtocolGuid,
                        NULL,
                        (VOID **)&DxeIpmiTransport );
        
        DEBUG ((DEBUG_INFO, "Locate DXE IPMI Transport protocol Status:%r! \n", Status));
        if (!EFI_ERROR (Status)) {
            DxeIpmiInstance = AMI_IPMI_INSTANCE_FROM_EFI_SM_IPMI_BMC_THIS (DxeIpmiTransport);

            DEBUG ((DEBUG_INFO, "KCS Interface Data pointer before :%x! \n", Interface->Kcs));
            Interface->Kcs = DxeIpmiInstance->Interface.Kcs;
            DEBUG ((DEBUG_INFO, "KCS Interface Data pointer After :%x! \n", Interface->Kcs));
            return Status;
        }
    }
    
    if (Interface->Kcs == NULL) {
        Interface->Kcs = (AMI_KCS_SYSTEM_INTERFACE *)AllocateRuntimeZeroPool (sizeof (AMI_KCS_SYSTEM_INTERFACE));
        if (Interface->Kcs == NULL) {
            DEBUG ((DEBUG_ERROR, "%a - Failed to allocate memory for Interface->Kcs! \n", __FUNCTION__));
            return EFI_OUT_OF_RESOURCES;
        }
    }
    
    Interface->Kcs->DataPort    = IPMI_SMM_KCS_DATA_PORT;
    Interface->Kcs->CommandPort = IPMI_SMM_KCS_COMMAND_PORT;
    Interface->Kcs->KcsRetryCounter = IPMI_SMM_KCS_COMMAND_PORT_READ_RETRY_COUNTER;
    
    return EFI_SUCCESS;
}

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
