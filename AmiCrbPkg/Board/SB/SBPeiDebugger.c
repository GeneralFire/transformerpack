//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2020, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

/** @file SBPeiDebugger.C
    This file contains PEI stage debugger code for SB template
    
    @note MAKE SURE NO PEI OR DXE SPECIFIC CODE IS NEEDED

**/

#include <Efi.h>
#include <AmiLib.h>
#include <AmiCspLib.h>
#include <AmiDebugPort.h>
#include "Token.h"

/**
    This eLink function is used to initialize the South Bridge
    for PEI Debugger support

    @param[in]       DebugPort  Debug transport interface structure

    @return     EFI_STATUS
    @retval     Status            EFI_SUCCESS         - Success will be returned.
**/

EFI_STATUS SBPEIDBG_Initialize (
 IN OUT PEI_DBG_PORT_INFO *DebugPort
)
{
 
 return EFI_SUCCESS;
}


/**
    This function searches the PCI address space for the PCI
    device specified for a particular capability ID and returns
    the offset in the PCI address space if one found

    @param[in]       Bus      PCI Bus number
    @param[in]       DevFunc  PCI Device and function number
    @param[in]       CapId    Capability ID to look for

    @return     UINT8
    @retval     Capability ID location if one found
                Otherwise returns 0
**/

UINT8 
FindCapPtrDbg(
 IN UINT8 Bus,
 IN UINT8 DevFunc,
 IN UINT8 CapId
)
{
 // Porting for Emmitsburg PCH
    
 return 0;          //No device.
}
