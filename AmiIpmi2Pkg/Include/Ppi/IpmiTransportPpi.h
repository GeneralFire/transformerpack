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

/** @file IpmiTransportPpi.h
    Ipmi transport ppi definitions.

*/

#ifndef _IPMI_TRANSPORT_PPI_H_
#define _IPMI_TRANSPORT_PPI_H_

//---------------------------------------------------------------------------

#include <AmiIpmiBmcCommon.h>

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

#define EFI_PEI_IPMI_TRANSPORT_PPI_GUID \
    { \
        0x8122cebd, 0xf4fd, 0x4ea8, { 0x97, 0x6c, 0xf0, 0x3, 0xad, 0xdc, 0x4c, 0xb4 } \
    }

typedef AMI_IPMI_SEND_COMMAND       EFI_PEI_IPMI_SEND_COMMAND;

typedef AMI_IPMI_GET_BMC_STATUS     EFI_PEI_IPMI_GET_BMC_STATUS;

typedef AMI_IPMI_SEND_COMMAND_EX    EFI_PEI_IPMI_SEND_COMMAND_EX;

typedef AMI_IPMI_TRANSPORT_INSTANCE PEI_IPMI_TRANSPORT_PPI;

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

extern EFI_GUID gEfiPeiIpmiTransportPpiGuid;

//---------------------------------------------------------------------------

#endif  // #ifndef _IPMI_TRANSPORT_PPI_H_

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
