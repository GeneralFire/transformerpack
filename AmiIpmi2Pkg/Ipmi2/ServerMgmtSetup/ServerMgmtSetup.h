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

/** @file ServerMgmtSetup.h
    Contains headers and declarations for Server Mgmt Setup module.

*/

#ifndef _SERVER_MGMT_SETUP_H_
#define _SERVER_MGMT_SETUP_H_

//---------------------------------------------------------------------------

#include <Setup.h>
#include <ServerAutoId.h>
#include <AmiServerMgmtSetupVariable.h>
#include <ServerMgmtSetupStrDefs.h>

#ifndef VFRCOMPILE
#include <AmiDxeLib.h>
#include <Library/UefiLib.h>
#include <Library/HiiLib.h>
#include <Protocol/HiiString.h>
#endif

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

#define EFI_BDS_EVENT_BEFORE_SETUP_GUID  \
    { \
        0xdbc9fd21, 0xfad8, 0x45b0, { 0x9e, 0x78, 0x27, 0x15, 0x88, 0x67, 0xcc, 0x93 } \
    };

#ifdef VFRCOMPILE

// Define SERVER_MGMT_CONFIGURATION_DATA_VARSTORE if it is not defined.
#ifndef SERVER_MGMT_CONFIGURATION_DATA_VARSTORE
#define SERVER_MGMT_CONFIGURATION_DATA_VARSTORE \
        varstore SERVER_MGMT_CONFIGURATION_DATA, name = ServerSetup, \
            guid = SERVER_MGMT_CONFIGURATION_GUID;
#endif

// Define SYSTEM_ACCESS_VARSTORE if it is not defined.
#ifndef SYSTEM_ACCESS_VARSTORE
#define SYSTEM_ACCESS_VARSTORE \
        varstore SYSTEM_ACCESS, key = SYSTEM_ACCESS_KEY_ID, \
            name = SystemAccess, guid = SYSTEM_ACCESS_GUID;
#endif

// Define SUPPRESS_GRAYOUT_ENDIF if it is not defined.
#ifndef SUPPRESS_GRAYOUT_ENDIF
#define SUPPRESS_GRAYOUT_ENDIF endif; endif;
#endif
#endif

//---------------------------------------------------------------------------

#ifndef VFRCOMPILE
/** @internal
    @param[in] HiiHandle    Handle to HII database.
    @param[in] Class        Class of the HII form.

    @return VOID    Nothing.

**/
typedef VOID (STRING_INIT_FUNC) (
  IN EFI_HII_HANDLE  HiiHandle,
  IN UINT16          Class
);

/** @internal
    Function for use by legacy callbacks to retrieve the full callback
    parameters.

    @param VOID     Nothing.

    @return CALLBACK_PARAMETERS Pointer to the CALLBACK_PARAMTER structure
                                for consumption by legacy callbacks.

**/
CALLBACK_PARAMETERS* GetCallbackParameters (
  VOID
);


#endif // #ifndef VFRCOMPILE

#endif  // #ifndef _SERVER_MGMT_SETUP_H_

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
