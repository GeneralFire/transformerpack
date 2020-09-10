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

/** @file PeiIpmiCmosClear.c
    Implements ELINK for IsResetConfigMode.

*/

//---------------------------------------------------------------------------

#include <Token.h>
#include <AmiIpmiBmcCommon.h>
#include <Library/DebugLib.h>
#include <Ppi/ReadOnlyVariable2.h>

//---------------------------------------------------------------------------

/**
    Read NVRAM variable. Based on read status return boolean value.

    @param[in] PeiServices      Describes the list of possible PEI Services.
    @param[in] ReadVariablePpi  Pointer to PPI to access NVRAM.

    @retval TRUE    If variable is found.
    @retval FALSE   If variable is not found.

**/
BOOLEAN
EFIAPI
IsIpmiCmosClear (
  IN       EFI_PEI_SERVICES                 **PeiServices,
  IN CONST EFI_PEI_READ_ONLY_VARIABLE2_PPI  *ReadVariablePpi )
{
    EFI_STATUS      Status;
    UINTN           Size;
    UINT8           Value;

    DEBUG ((DEBUG_INFO, "%a Entry...\n", __FUNCTION__));
    Size = sizeof (Value);
    Status = ReadVariablePpi->GetVariable (
                                ReadVariablePpi,
                                L"IpmiCmosClear",
                                &gEfiIpmiCmosClearVariableGuid,
                                NULL,
                                &Size,
                                &Value );
    
    DEBUG ((DEBUG_INFO, "ReadVariablePpi->GetVariable Status %r  \n", Status));
    
    if (EFI_ERROR (Status)) {
        return FALSE;
    }
    DEBUG ((DEBUG_INFO, "%a Exiting...\n", __FUNCTION__));
    return TRUE;
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
