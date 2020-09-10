//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2020, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

/** @file CRBLib.c
    This file contains Chipset Reference Board related code that is needed 
    for both PEI & DXE stage.
    To avoid code duplication this file is made as a library and linked both 
    in PEI & DXE CRB FFS.
    Note: MAKE SURE NO PEI OR DXE SPECIFIC CODE IS NEEDED
*/

//----------------------------------------------------------------------------
// Include(s)
//----------------------------------------------------------------------------
#include "IioUniversalData.h"
#include <Library/BaseLib.h>
#include <Library/PcdLib.h> 
#include "Token.h"

#include <Include/SetupLibInternal.h>
#include <Ppi/ReadOnlyVariable2.h>
#include <Build/GenericSetupDataDefinition.h>
#include <Uefi/UefiSpec.h>

/**
    This eLink function is used to initialize the South Bridge
    for PEI Debugger support

    @param[in]       **PeiServices - pointer to a pointer to the PEI Services Table. 
    @param[in]       *ReadVariablePpi - pointer to EFI_PEI_READ_ONLY_VARIABLE2_PPI PPI. 
                     The pointer can be used to read and enumerate existing 
                     NVRAM variables.

    @return     BOOLEAN
    @retval     TRUE - the NVRAM data is compatible
                FALSE - the NVRAM data is not compatible
    @notes      This function is called by NVRAM PEIM. 

**/
BOOLEAN 
CrbIsNvramDataCompatible (
    IN CONST EFI_PEI_SERVICES                   **PeiServices,   
    IN CONST EFI_PEI_READ_ONLY_VARIABLE2_PPI    *ReadVariablePpi
)
{
    UINTN Size = 0;
    UINT8 Index = 0;
    EFI_STATUS Status;

    Status=ReadVariablePpi->GetVariable(
        ReadVariablePpi, L"Setup",
        &gEfiSetupVariableGuid, NULL,
        &Size, NULL
    );

    if ( ( Status == EFI_BUFFER_TOO_SMALL ) && ( Size != sizeof(SETUP_DATA) ) ) {
        return FALSE;
    }

    while ( mSetupInfo[Index].GuidValue != NULL ) {

        Size = 0;
        Status = ReadVariablePpi->GetVariable(
                ReadVariablePpi, mSetupInfo[Index].SetupName,
                mSetupInfo[Index].GuidValue, NULL,
                &Size, NULL
        );

        if ( ( Status == EFI_BUFFER_TOO_SMALL ) && ( Size != mSetupInfo[Index].VariableSize ) ) {
            return FALSE;
        }
        Index++;
    }

    return TRUE;

}

/**
  This function determines if the current NVRAM data 
  (mainly Setup-related variables) is compatible with the current firmware.
  This function is called by NVRAM Driver. 

  @param[in]       *ReadVariablePpi - pointer to EFI_PEI_READ_ONLY_VARIABLE2_PPI PPI. 
                    The pointer can be used to read existing NVRAM variables.

  @return     BOOLEAN
  @retval     TRUE - the NVRAM data is compatible
                FALSE - the NVRAM data is not compatible
**/
BOOLEAN 
CrbDxeIsNvramDataCompatible (
    IN EFI_GET_VARIABLE GetVariable
)
{
    UINTN Size = 0;
    UINT8 Index = 0;
    EFI_STATUS Status;

    Status=GetVariable(
        L"Setup",
        &gEfiSetupVariableGuid, NULL,
        &Size, NULL
    );

    if ( ( Status == EFI_BUFFER_TOO_SMALL ) && ( Size != sizeof(SETUP_DATA) ) ) {
        return FALSE;
    }

    while ( mSetupInfo[Index].GuidValue != NULL ) {

        Size = 0;
        Status=GetVariable(
                mSetupInfo[Index].SetupName,
                mSetupInfo[Index].GuidValue, NULL,
                &Size, NULL
        );

        if ( ( Status == EFI_BUFFER_TOO_SMALL ) && ( Size != mSetupInfo[Index].VariableSize ) ) {
            return FALSE;
        }
        Index++;
    }
    
    return TRUE;
}
