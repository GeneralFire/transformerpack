//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************
/** @file Ast2600AmiSioLib.c
    This contains the get table functions of the Super IO.
**/
#include <Token.h>
#include <Protocol/AmiSio.h>

/**
    Enter SIO Configuration Mode

    @param  None

    @retval None
**/
VOID Ast2600EnterConfig()
{
    IoWrite8(AST2600_CONFIG_INDEX, AST2600_CONFIG_MODE_ENTER_VALUE);
    IoWrite8(AST2600_CONFIG_INDEX, AST2600_CONFIG_MODE_ENTER_VALUE);
}

/**
    Exit SIO Configuration Mode

    @param  None

    @retval None
**/
VOID Ast2600ExitConfig()
{
    IoWrite8(AST2600_CONFIG_INDEX, AST2600_CONFIG_MODE_EXIT_VALUE);
}

/**
    Select Logical Device with Logical Device Number(LDN).

    @param  Ldn  Logical Device Configure Area will be switched with this value.

    @retval None
**/
VOID Ast2600SelectLdn(UINT8 Ldn)
{
    IoWrite8(AST2600_CONFIG_INDEX, AST2600_LDN_SEL_REGISTER);
    IoWrite8(AST2600_CONFIG_DATA, Ldn);
}
