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

/** @file DxeAmiIpmbInterfaceLib.c
    IPMB Transport Dxe phase implementation.

*/

//---------------------------------------------------------------------------

#include <Token.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/AmiIpmbInterfaceLib.h>
#include <Protocol/I2cMaster.h>

//---------------------------------------------------------------------------

/** @internal
    Send Ipmi command through Ipmb interface.

    @param[in] Interface        Pointer to System interface.
    @param[in] SlaveAddress     I2C device slave address.
    @param[in] RequestPacket    Pointer to an EFI_I2C_REQUEST_PACKET
                                structure describing the I2C transaction.

    @return EFI_STATUS  Status of the Send I2c command.

**/
EFI_STATUS
AmiIpmiI2cSendCommand (
  IN AMI_SYSTEM_INTERFACE       *Interface,
  IN UINTN                      SlaveAddress,
  IN EFI_I2C_REQUEST_PACKET     *RequestPacket )
{
    EFI_STATUS                  Status = EFI_NOT_FOUND;
    EFI_I2C_MASTER_PROTOCOL     *I2cMasterTransmit = NULL;
    
    I2cMasterTransmit = (EFI_I2C_MASTER_PROTOCOL *)Interface->Ipmb.IpmbInterfaceApiPtr;
    
    if (I2cMasterTransmit != NULL) {
        Status = I2cMasterTransmit->StartRequest (
                                        I2cMasterTransmit,
                                        SlaveAddress,
                                        RequestPacket,
                                        NULL,
                                        NULL );
    }
    
    DEBUG ((DEBUG_INFO, "I2cMasterTransmit->StartRequest Status = %r\n", Status)); 
    
    return Status;
}

/** @internal
    Locate I2c Ppi/Protocol instance and initialize interface pointer.

    @param[in, out] Interface   System interface pointer.

    @return EFI_STATUS  Status returned from functions used.

**/
EFI_STATUS
AmiIpmiGetI2cApiPtr (
  IN OUT AMI_SYSTEM_INTERFACE   *Interface )
{
    EFI_STATUS                  Status;
    EFI_I2C_MASTER_PROTOCOL     *I2cMasterTransmit = NULL;
    
    Interface->Ipmb.IpmbInterfaceApiGuid = gEfiI2cMasterProtocolGuid;
    
    // Locate the I2C DXE Protocol for Communication.
    Status = gBS->LocateProtocol (
                    &gEfiI2cMasterProtocolGuid, 
                    NULL, 
                    (VOID **)&I2cMasterTransmit );
    
    DEBUG ((DEBUG_INFO, "LocateProtocol gEfiI2cMasterProtocolGuid Status = %r\n", Status)); 
    
    if (!EFI_ERROR(Status)) {
        Interface->Ipmb.IpmbInterfaceApiPtr = (UINTN)I2cMasterTransmit;
    }
    
    return Status;
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
