//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2020, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

/** @file AmiSmbiosLib.h
  This file contains South Bridge(LPC) chipset porting library SMBIOS 
  functions for both PEI & DXE stages.
  
*/

#ifndef __AMI_SMBIOS_LIB_H__
#define __AMI_SMBIOS_LIB_H__


#include <Uefi.h>
#include <Token.h>

UINT8 getWakeupTypeForSmbios (
    VOID
);

#endif // __AMI_SMBIOS_LIB_H__
