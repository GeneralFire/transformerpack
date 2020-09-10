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

/** @file AmiServerMgmtSetupVariable.h
    ServerMgmtSetup page related definitions.

*/

#ifndef _AMI_SERVER_MGMT_SETUP_VARIABLE_H
#define _AMI_SERVER_MGMT_SETUP_VARIABLE_H

//---------------------------------------------------------------------------

#include <ServerMgmtSetupDataDefinitions.h>

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

#define SERVER_MGMT_CONFIGURATION_GUID \
    { \
        0x1239999, 0xfc0e, 0x4b6e, { 0x9e, 0x79, 0xd5, 0x4d, 0x5d, 0xb6, 0xcd, 0x20 } \
    }

#define SERVER_MGMT_CLASS_ID        20
#define SERVER_MGMT_SUBCLASS_ID     0x00

#ifndef VFRCOMPILE
extern EFI_GUID gEfiServerMgmtSetupVariableGuid;
#endif

//---------------------------------------------------------------------------

#endif // #ifndef _AMI_SERVER_MGMT_SETUP_VARIABLE_H

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
