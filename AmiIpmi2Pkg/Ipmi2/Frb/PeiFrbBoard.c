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

/** @file PeiFrbBoard.c
    Provides Platform hook for FRB3 timer support.

*/

//---------------------------------------------------------------------------

#include "PeiFrb.h"

//---------------------------------------------------------------------------

/** @internal
    Disables FRB3 timer if Platform has Frb3 Timer support.

    @param[in] PeiServices  Describes the list of possible PEI Services.

    @return VOID    Nothing.

**/
VOID
DisableFrb3Timer (
  IN CONST EFI_PEI_SERVICES **PeiServices )
{
    // If Platform has FRB3 timer support, add code here to disable FRB3 timer.
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
