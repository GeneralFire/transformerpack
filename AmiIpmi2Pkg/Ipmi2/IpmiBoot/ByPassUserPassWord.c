//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

/** @file ByPassUserPassWord.c
    Password prompt is not displayed in post when IPMI boot
    "UserPasswordBypass" flag is set for user level access.

*/

//---------------------------------------------------------------------------
#include <Uefi.h>
#include "IpmiBoot.h"
#include <AMIVfr.h>
#include <PwdLib.h>
#include <commonoem.h>


//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

extern UINT32       gPasswordType; // Provides what is the current access level.
EFI_IPMI_TRANSPORT  *gIpmiTransport;

//---------------------------------------------------------------------------

/**
    This is a replacement for the ProcessConInAvailability
    hook in TSE, to By pass password prompt for user level
    based on IPMI boot "UserPasswordBypass" flag.

    @param VOID     Nothing.

    @retval TRUE    If the screen was used to ask password.
    @retval FALSE   If the screen was not used to ask password.

**/
BOOLEAN
IpmiByPassUserPassWord (
  VOID )
{
    BOOLEAN                                         ByPassUserPassWord = FALSE;
    EFI_STATUS                                      Status;
    AMI_GET_BOOT_OPTIONS_BOOT_INFO_ACK_RESPONSE     BootInfoAckResponse;
    UINTN                                           BootFlagsDataSize;
    AMI_GET_BOOT_OPTIONS_BOOT_FLAG_RESPONSE         BootFlags;
    UINT8                                           SysAccessValue;
    UINT8                                           CurrentPasswordType;
    EFI_GUID                                        SysAccessGuid = SYSTEM_ACCESS_GUID;

    DEBUG ((DEBUG_INFO, "%a Entry...\n", __FUNCTION__));

    // Locate the Ipmi Transport Protocol.
    Status = gBS->LocateProtocol (
                    &gEfiDxeIpmiTransportProtocolGuid,
                    NULL,
                    (VOID **)&gIpmiTransport );
    
    DEBUG((DEBUG_INFO, "LocateProtocol gEfiDxeIpmiTransportProtocolGuid. Status: %r\n", Status));
    
    if (EFI_ERROR (Status)) {
        goto CallOriginalHook;
    }

    // Read the boot info acknowledge bytes from BMC.
    Status = AmiIpmiGetBootInfoAcknowledgeData (
                gIpmiTransport,
                &BootInfoAckResponse );

    if(EFI_ERROR(Status)) {
        goto CallOriginalHook;
    }
 
    /* Support user password bypass only if BootInitiatorAcknowledgeData is zero
       or BiosOrPostBit is set in BootInitiatorAcknowledgeData.*/
    if ((BootInfoAckResponse.Param4.BootInitiatorAcknowledgeData == 0) || \
        (BootInfoAckResponse.Param4.BootInitiatorAcknowledgeData & BIT0)) {

        // Read the boot flag bytes from BMC.
        Status = AmiIpmiGetBootFlags (
                    gIpmiTransport,
                    &BootFlags );
        if (BootFlags.Param5.BootFlagValid) {
            ByPassUserPassWord = BootFlags.Param5.UserPasswordBypass;
        } else {
            // Get the IpmiBootFlags NVRAM Variable stored in previous persistent boots.
            BootFlagsDataSize = sizeof(BootFlags);
            Status = gRT->GetVariable (
                             L"IpmiBootFlags",
                             &gEfiIpmiBootGuid,
                             NULL,
                             &BootFlagsDataSize,
                             &BootFlags );

            if (EFI_ERROR(Status)) {
                goto CallOriginalHook;
            } else {
                ByPassUserPassWord = BootFlags.Param5.UserPasswordBypass;
            }
        }
    } else {
        goto CallOriginalHook;
    }

    CurrentPasswordType = (UINT8)PasswordCheckInstalled ();

    if ((ByPassUserPassWord == TRUE) && \
        (AMI_PASSWORD_USER & CurrentPasswordType)) {
        /* If User Password is set,By Pass Password Prompt.
           Set Access privilege as Administrator.*/
        if (AMI_PASSWORD_USER == CurrentPasswordType) {
            gPasswordType = AMI_PASSWORD_ADMIN;
            SysAccessValue = SYSTEM_PASSWORD_ADMIN;
        } else {
            /* If Both Administrator and User Password set, By Pass Password Prompt.
               Set Access privilege as User.*/
            gPasswordType = AMI_PASSWORD_USER;
            SysAccessValue = SYSTEM_PASSWORD_USER;
        }
        Status = gRT->SetVariable (
                        L"SystemAccess",
                        &SysAccessGuid,
                        EFI_VARIABLE_BOOTSERVICE_ACCESS,
                        sizeof (SysAccessValue),
                        &SysAccessValue );
        return FALSE;
    }

    // User by pass password bit is not set so call Original Hook.
CallOriginalHook:
    DEBUG ((DEBUG_INFO, "%a Exiting..... User by pass password bit is not set so calling Original Hook \n", __FUNCTION__));
    return ProcessConInAvailability ();
}


