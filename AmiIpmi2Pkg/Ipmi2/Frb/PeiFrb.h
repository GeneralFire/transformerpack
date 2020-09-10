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

/** @file PeiFrb.h
    Header file for PeiFrb module.

*/

#ifndef _PEI_FRB_H_
#define _PEI_FRB_H_

//---------------------------------------------------------------------------

#include <Token.h>
#include <AmiServerMgmtSetupVariable.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Ppi/MasterBootMode.h>
#include <Ppi/ReadOnlyVariable2.h>
#include <Ppi/IpmiTransportPpi.h>

//---------------------------------------------------------------------------

/** @internal
    Disables FRB3 timer if Platform has Frb3 Timer support.

    @param[in] PeiServices  Describes the list of possible PEI Services.

    @return VOID    Nothing.

**/
VOID
DisableFrb3Timer (
  IN CONST EFI_PEI_SERVICES  **PeiServices
);

#endif

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
