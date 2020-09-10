//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2020, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

/** @file  DxeFrb.h  
    Header file for DxeFrb module.

*/

#ifndef _DXE_FRB_H_
#define _DXE_FRB_H_

//---------------------------------------------------------------------------
#include <Uefi.h>
#include <Guid/HobList.h>
#include <AmiServerMgmtSetupVariable.h>
#include <Library/PcdLib.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Protocol/FrbProtocol.h>
#include <Protocol/IpmiTransportProtocol.h>

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

// EDK I Shell Guid
#define SHELL_ENVIRONMENT_PROTOCOL_GUID \
    { \
        0x47c7b221, 0xc42a, 0x11d2, { 0x8e, 0x57, 0x0, 0xa0, 0xc9, 0x69, 0x72, 0x3b } \
    }

// EDK II Shell Guid
#define EFI_SHELL_PROTOCOL_GUID \
    { \
        0x6302d008, 0x7f9b, 0x4f30, { 0x87, 0xac, 0x60, 0xc9, 0xfe, 0xf5, 0xda, 0x4e } \
    }

// HDD Password prompt entered Guid 
#define AMI_HDD_PASSWORD_PROMPT_ENTER_GUID \
    { \
        0x8e8d584a, 0x6e32, 0x44bf, { 0xb9, 0x6e, 0x1d, 0x27, 0x7, 0xc4, 0xeb, 0x5c } \
    }

// HDD Password prompt exit Guid 
#define AMI_HDD_PASSWORD_PROMPT_EXIT_GUID \
    { \
        0xe22af903, 0xfd6f, 0x4d22, { 0x94, 0xce, 0xf8, 0x49, 0xc6, 0x14, 0xc3, 0x45 }\
    }


/**
    FRB data buffer.
*/
typedef struct {
    /// Count down to start from.
    UINT16    TimeOutValue;
    /// PreTimeoutInterval.
    UINT8     PretimeoutInterval;
    /// Interrupt type to generate before timer expires.
    UINT8     PreTimeOutInterrupt;
    /// Policy to do after timer expires like hard reset or power down.
    UINT8     BootPolicy;
} FRB_DATA_BUFFER;

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

SERVER_MGMT_CONFIGURATION_DATA      gServerMgmtConfiguration;
FRB_DATA_BUFFER                     gFrbDataBuffer[AmiIpmiOsBootWdt + 1];
EFI_IPMI_TRANSPORT                  *gIpmiTransport;
EFI_SM_FRB_PROTOCOL                 *gFrbProtocol;

//---------------------------------------------------------------------------

EFI_STATUS
SetFrb (
  IN AMI_BMC_SET_WATCHDOG_TIMER_REQUEST        *SetWatchDogTimerData );
#endif
