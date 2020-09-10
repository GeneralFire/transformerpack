//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************
/** @file Ast2600Setup.h
  GUID or structure Of setup related routines.
**/

#ifndef _AST2600_SETUP_H_
#define _AST2600_SETUP_H_

#ifdef __cplusplus
extern "C" {
#endif
//----------------------------------------------------------------------
// Include Files
//----------------------------------------------------------------------
#include <SioSetup.h>

#ifdef VFRCOMPILE
//----------------------------------------------------------------------
// Constants, Macros and Type Definitions
//----------------------------------------------------------------------

//----------------------------------------------------------------------
//                        Below is for SD files                         
//----------------------------------------------------------------------
#pragma pack(1)

LDX_XV_DATA(COMA)

LDX_XV_DATA(COMB)

LDX_XV_DATA(COMC)

LDX_XV_DATA(COMD)

#pragma pack()

#endif

/****** DO NOT WRITE BELOW THIS LINE *******/
#ifdef __cplusplus
}
#endif

#endif //_AST2600_SETUP_H_
