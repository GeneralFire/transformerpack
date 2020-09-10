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

/** @file SmmAmiUsbInterfaceLib.c
    USB Transport Smm phase Implementation library functions.

*/

//---------------------------------------------------------------------------

#include <Token.h>
#include <Library/DebugLib.h>
#include <Library/AmiUsbInterfaceLib.h>

//---------------------------------------------------------------------------

/** @internal
    Issues USB transaction to BMC USB interface.

    @param[in]      Interface       Pointer to System interface.
    @param[in]      Opcode          Opcode of the command.
    @param[in]      Sector          Data/Command sector.
    @param[in]      EndpointAddr    USB End point address.
    @param[in, out] Data            Pointer to input data.
    @param[in, out] DataSize        Size of input data.
    @param[out]     CompletionCode  Pointer to the completion code.

    @retval EFI_UNSUPPORTED Usb transaction is not supported.

**/
EFI_STATUS
AmiIpmiIssueBmcUsbTransaction (
  IN     AMI_SYSTEM_INTERFACE   *Interface,
  IN     UINT8                  Opcode,
  IN     UINT8                  Sector,
  IN     UINT8                  EndpointAddr,
  IN OUT UINT8                  *Data,
  IN OUT UINT32                 DataSize,
  OUT    UINT8                  *CompletionCode )
{
    return EFI_UNSUPPORTED;
}

/** @internal
    Allocate memory for Usb interface, checks for Bmc usb interface
    and initializes the variables.

    @param[in, out] Interface   Pointer to System interface.

    @retval EFI_UNSUPPORTED     Usb interface is not supported in smm.

**/
EFI_STATUS
AmiIpmiGetUsbApiPtr (
  IN OUT AMI_SYSTEM_INTERFACE   *Interface )
{
    DEBUG((DEBUG_INFO, "%a Entry...\n", __FUNCTION__));
    DEBUG((DEBUG_INFO, "USB interface is not supported for SMM, so return EFI_UNSUPPORTED.\n"));
    return EFI_UNSUPPORTED;
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
