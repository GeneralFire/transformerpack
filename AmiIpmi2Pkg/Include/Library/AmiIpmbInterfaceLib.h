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

/** @file AmiIpmbInterfaceLib.h
    IPMB interface common function declarations and macros.

*/

#ifndef _AMI_IPMB_INTERFACE_LIB_H
#define _AMI_IPMB_INTERFACE_LIB_H

//---------------------------------------------------------------------------

#include <Pi/PiI2c.h>
#include <AmiIpmiBmcCommon.h>

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

#define AMI_IPMI_MAX_IPMB_CMD_DATA_SIZE    0xFF
#define AMI_IPMI_READ_FLAG                 1
#define AMI_IPMI_WRITE_FLAG                0
#define AMI_IPMI_SEQ_NO                    0     // IPMB Message Sequence Number.

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

/** @internal
    Locate I2c Ppi/Protocol instance and initialize interface pointer.

    @param[in, out] Interface   System interface pointer.

    @return EFI_STATUS  Status returned from functions used.

**/
EFI_STATUS
AmiIpmiGetI2cApiPtr (
  IN OUT AMI_SYSTEM_INTERFACE   *Interface
);

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
  IN EFI_I2C_REQUEST_PACKET     *RequestPacket
);

//---------------------------------------------------------------------------

#endif  // #ifndef _AMI_IPMB_INTERFACE_LIB_H

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
