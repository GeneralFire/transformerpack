//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

/** @file DxeFrbHook.c
    Implemented TSE hook to disable and enable OS watchdog timer while
    entering into shell.

*/

//---------------------------------------------------------------------------

#include <Token.h>
#include <Uefi.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/PcdLib.h>
#include <Library/DebugLib.h>
#include <Protocol/FrbProtocol.h>
#include <AmiServerMgmtSetupVariable.h>

//---------------------------------------------------------------------------

/**
    Enables OS WDT on exiting Shell based on OS WDT disable state.

    @param VOID     Nothing.

    @return VOID    Nothing.

**/
VOID
EFIAPI
EnableOsWdtTimerHook (
  VOID )
{
    UINTN                               Size;
    EFI_STATUS                          Status;
    EFI_SM_FRB_PROTOCOL                 *FrbProtocol;
    SERVER_MGMT_CONFIGURATION_DATA      ServerMgmtConfiguration;

    // Get Server Mgmt Setup Variable.
    Size = sizeof (SERVER_MGMT_CONFIGURATION_DATA);
    Status = gRT->GetVariable (
                    L"ServerSetup",
                    &gEfiServerMgmtSetupVariableGuid,
                    NULL,
                    &Size,
                    &ServerMgmtConfiguration );
    DEBUG ((DEBUG_INFO, "gRT->GetVariable for ServerSetup Status %r \n", Status));
    
    if (EFI_ERROR(Status)) {
        ServerMgmtConfiguration.OsBootWdtEnable = 0x00;         //default is disabled in setup
    }
    
    if (ServerMgmtConfiguration.OsBootWdtEnable) {
        DEBUG ((DEBUG_INFO, "IPMI FRB : Shell Exit. Enable OS WDT if it is not disabled while entering into setup.\n"));

        /* If PcdOsWdtTimerDisable is enabled then boot was done from setup
           and OS WDT is stopped in setup. Else enable the OS WDT.*/
        if (PcdGetBool (AmiPcdOsWdtDisabledOnSetupEntry)) {
            DEBUG ((DEBUG_INFO, "IPMI FRB : PcdOsWdtTimerDisable is set. Boot was done from Setup.\n"));
            DEBUG ((DEBUG_INFO, "OS WDT is stopped in setup.\n"));
        } else {
            // Locate EFI_SM_FRB_PROTOCOL.
            Status = gBS->LocateProtocol (
                            &gEfiSmFrbProtocolGuid,
                            NULL,
                            (VOID **)&FrbProtocol );
            if (!EFI_ERROR (Status)) {
                Status = FrbProtocol->EnableFrb (
                                        FrbProtocol,
                                        AmiIpmiOsBootWdt,
                                        FALSE );
                DEBUG ((DEBUG_INFO, "OS WDT is enabled while exiting from shell. Status = %r\n", Status));
            }
        }
    }
}


