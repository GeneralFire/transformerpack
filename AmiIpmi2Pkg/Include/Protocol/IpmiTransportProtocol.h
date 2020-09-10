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

/** @file IpmiTransportProtocol.h
    Ipmi transport protocol definitions.

*/

#ifndef _IPMI_TRANSPORT_PROTOCOL_H_
#define _IPMI_TRANSPORT_PROTOCOL_H_

//---------------------------------------------------------------------------

#include <AmiIpmiBmcCommon.h>

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

#define EFI_DXE_IPMI_TRANSPORT_PROTOCOL_GUID \
    { \
        0x4a1d0e66, 0x5271, 0x4e22, { 0x83, 0xfe, 0x90, 0x92, 0x1b, 0x74, 0x82, 0x13 } \
    }

#define EFI_SMM_IPMI_TRANSPORT_PROTOCOL_GUID \
    { \
        0x1dbd1503, 0xa60, 0x4230, { 0xaa, 0xa3, 0x80, 0x16, 0xd8, 0xc3, 0xde, 0x2f } \
    }

typedef AMI_IPMI_TRANSPORT_INSTANCE EFI_IPMI_TRANSPORT;

typedef AMI_IPMI_SEND_COMMAND       EFI_IPMI_SEND_COMMAND;

typedef AMI_IPMI_GET_BMC_STATUS     EFI_IPMI_GET_BMC_STATUS;

typedef AMI_IPMI_SEND_COMMAND_EX    EFI_IPMI_SEND_COMMAND_EX;

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

extern EFI_GUID gEfiDxeIpmiTransportProtocolGuid;
extern EFI_GUID gEfiSmmIpmiTransportProtocolGuid;

//----------------------------------------------------------------------

#endif  // #ifndef _IPMI_TRANSPORT_PROTOCOL_H_

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
