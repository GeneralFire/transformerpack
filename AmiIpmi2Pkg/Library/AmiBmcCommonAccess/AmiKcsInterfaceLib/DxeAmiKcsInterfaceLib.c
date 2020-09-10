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

/** @file DxeAmiKcsInterfaceLib.c
    KCS Transport Dxe phase Implementation library functions.

*/

//---------------------------------------------------------------------------

#include <Token.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/AmiKcsInterfaceLib.h>

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
    if (Interface->Kcs == NULL) {
        Interface->Kcs = (AMI_KCS_SYSTEM_INTERFACE *)AllocateRuntimeZeroPool (sizeof (AMI_KCS_SYSTEM_INTERFACE));
        if (Interface->Kcs == NULL) {
            DEBUG ((DEBUG_ERROR, "%a - Failed to allocate memory for Interface->Kcs! \n", __FUNCTION__));
            return EFI_OUT_OF_RESOURCES;
        }
    }
    Interface->Kcs->DataPort    = IPMI_KCS_DATA_PORT;
    Interface->Kcs->CommandPort = IPMI_KCS_COMMAND_PORT;
    return EFI_SUCCESS;
}

//**********************************************************************
//**********************************************************************
//**                                                                  **
//**        (C)Copyright 1985-2018, American Megatrends, Inc.         **
//**                                                                  **
//**                       All Rights Reserved.                       **
//**                                                                  **
//**         5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093     **
//**                                                                  **
//**                       Phone: (770)-246-8600                      **
//**                                                                  **
//**********************************************************************
//**********************************************************************
