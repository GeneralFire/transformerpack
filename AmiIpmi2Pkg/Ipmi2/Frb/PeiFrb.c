//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************


/** @file PeiFrb.c
    Does FRB initialization for POST.
    Enables FRB2 timer and reports timer expiration Events to BMC if any.

*/

//---------------------------------------------------------------------------

#include "PeiFrb.h"

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

/**
    If system is in S3 or recovery state, then stop frb2 timer

    @param[in] PeiServices          Describes the list of possible PEI
                                    Services.
    @param[in] NotifyDescriptor     Pointer to notify descriptor.
    @param[in] Ppi                  Pointer to Ppi.

    @retval EFI_SUCCESS     FRB2 timer is stopped on S3 resume or recovery
                            mode.
    @retval EFI_UNSUPPORTED System is not in recovery or S3 resume mode.
    @retval EFI_ABORTED     Timer is already stopped or Timer is not FRB2.
    @retval Others          Error status from function calls used.

**/
EFI_STATUS
EFIAPI
StopFrb2Timer (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi )
{
    PEI_IPMI_TRANSPORT_PPI                  *IpmiTransportPpi;
    EFI_STATUS                              Status;
    UINT8                                   FrbTimerDataSize;
    UINT8                                   ResponseDataSize;
    EFI_BOOT_MODE                           BootMode;
    AMI_BMC_GET_WATCHDOG_TIMER_RESPONSE     GetWatchDogTimerData;
    AMI_BMC_SET_WATCHDOG_TIMER_REQUEST      *SetWatchdogTimerDataPtr = NULL;
    UINT8                                   CompletionCode;

    Status = (*PeiServices)->GetBootMode (
                                (CONST EFI_PEI_SERVICES**)PeiServices,
                                &BootMode );
    
    DEBUG ((DEBUG_INFO, "GetBootMode Status: %r BootMode: %x \n", Status, BootMode));
    ASSERT_EFI_ERROR(Status);

    // Checking if boot mode is S3 resume or recovery mode.
    if ((BootMode == BOOT_ON_S3_RESUME) || \
        (BootMode == BOOT_IN_RECOVERY_MODE)) {
        // Locate IPMI Transport protocol to send commands to BMC.
        Status = (*PeiServices)->LocatePpi (
                                    (CONST EFI_PEI_SERVICES**)PeiServices,
                                    &gEfiPeiIpmiTransportPpiGuid,
                                    0,
                                    NULL,
                                    (VOID **)&IpmiTransportPpi );
        if (EFI_ERROR(Status)) {
            return Status;
        }

        // Get FRB status to check whether timer is already stopped or not.
        FrbTimerDataSize = sizeof (GetWatchDogTimerData);
        Status = IpmiTransportPpi->SendIpmiCommand (
                                        IpmiTransportPpi,
                                        IPMI_NETFN_APP,
                                        AMI_BMC_LUN,
                                        IPMI_APP_GET_WATCHDOG_TIMER,
                                        NULL,
                                        0,
                                        (UINT8*)&GetWatchDogTimerData,
                                        &FrbTimerDataSize );
        
        DEBUG ((DEBUG_INFO, " IPMI_APP_GET_WATCHDOG_TIMER Status: %r CompletionCode: %x\n", Status, GetWatchDogTimerData.CompletionCode));
        
        if (EFI_ERROR(Status) || (GetWatchDogTimerData.CompletionCode)) {
            return Status;
        }

        // If timer is already stopped or timer type is FRB2 then return EFI_ABORTED.
        if ((!GetWatchDogTimerData.TimerUse.Bits.TimerRunning) || \
            (GetWatchDogTimerData.TimerUse.Bits.TimerUse != AmiIpmiFrb2)) {
            return EFI_ABORTED;
        } else {
            /* Disable timer by using set watchdog timer with timer value
               and DontStopTimer bit value as zero. while disabling don't
               clear global expiration flag. Also retaining previous other
               values.*/
            SetWatchdogTimerDataPtr = (AMI_BMC_SET_WATCHDOG_TIMER_REQUEST *)&GetWatchDogTimerData;
            SetWatchdogTimerDataPtr->TimerUse.Bits.TimerRunning = AMI_IPMI_STOP_TIMER_WHEN_SET_WDT;
            SetWatchdogTimerDataPtr->InitialCountdownValue = 0;
    
            FrbTimerDataSize = sizeof (AMI_BMC_SET_WATCHDOG_TIMER_REQUEST);
            ResponseDataSize = sizeof (CompletionCode);
    
            Status = IpmiTransportPpi->SendIpmiCommand (
                                            IpmiTransportPpi,
                                            IPMI_NETFN_APP,
                                            AMI_BMC_LUN,
                                            IPMI_APP_SET_WATCHDOG_TIMER,
                                            (UINT8*)SetWatchdogTimerDataPtr,
                                            FrbTimerDataSize,
                                            &CompletionCode,
                                            &ResponseDataSize );
            
            DEBUG ((DEBUG_INFO, " IPMI_APP_SET_WATCHDOG_TIMER Status: %r \n", Status));
            return Status;
        }
    }
    return EFI_UNSUPPORTED;
}

/** @internal
    Read the FRB setup options and update Set watch dog timer with it.

    @param[in]  PeiServices             Describes the list of possible PEI
                                        Services.
    @param[out] SetWatchDogTimerData    Pointer to Set Watch dog timer
                                        structure.
    @param[out] FrbEnabled              Pointer to retain FRB2 setup option.

    @return VOID    Nothing.

**/
VOID
GetFrbSetupOptions (
  IN CONST  EFI_PEI_SERVICES                    **PeiServices,
  OUT       AMI_BMC_SET_WATCHDOG_TIMER_REQUEST  *SetWatchDogTimerData,
  OUT       BOOLEAN                             *FrbEnabled )
{
    EFI_PEI_READ_ONLY_VARIABLE2_PPI     *PeiVariable;
    SERVER_MGMT_CONFIGURATION_DATA      ServerMgmtConfiguration;
    UINTN                               Size;
    EFI_STATUS                          Status;

    // Locate the Setup configuration value.
    Status = (*PeiServices)->LocatePpi (
                                PeiServices,
                                &gEfiPeiReadOnlyVariable2PpiGuid,
                                0,
                                NULL,
                                (VOID **)&PeiVariable );
    
    DEBUG ((DEBUG_INFO, " gEfiPeiReadOnlyVariable2PpiGuid status %r\n", Status));
    ASSERT_EFI_ERROR (Status);

    if (!EFI_ERROR(Status)) {
        Size = sizeof(SERVER_MGMT_CONFIGURATION_DATA);
        Status = PeiVariable->GetVariable (
                                PeiVariable,
                                L"ServerSetup",
                                &gEfiServerMgmtSetupVariableGuid,
                                NULL,
                                &Size,
                                &ServerMgmtConfiguration );
        DEBUG ((DEBUG_INFO, " PeiVariable->GetVariable status %r \n", Status));
    }
    if (EFI_ERROR(Status)) {

        // Data Byte-2 TimeoutAction.
        SetWatchDogTimerData->TimerActions.TimeoutAction = AmiIpmiNormalBoot;

        // Data Byte-5 and Byte-6.
        SetWatchDogTimerData->InitialCountdownValue = DEFAULT_FRB2_TIMEOUT * AMI_MIN_TO_100MS;  // Default minutes
    } else {
        *FrbEnabled = (BOOLEAN)ServerMgmtConfiguration.Frb2Enable;

        // Data Byte-2 TimeoutAction.
        SetWatchDogTimerData->TimerActions.TimeoutAction = ServerMgmtConfiguration.Frb2TimeoutPolicy;

        //  least significant and most significant data bytes(5 and 6) of countdown value,The Watchdog Timer decrement is one count/100 ms
        SetWatchDogTimerData->InitialCountdownValue = ServerMgmtConfiguration.Frb2Timeout * AMI_MIN_TO_100MS;
    }
}

/**
    Enables FRB2 timer based on setup option.

    @param[in] FileHandle    Pointer to image file handle.
    @param[in] PeiServices   Describes the list of possible PEI Services.

    @retval EFI_SUCCESS         FRB initialization completed successfully.
    @retval EFI_UNSUPPORTED     FRB is disable in setup.
    @retval Others              Error status returned from function.

**/
EFI_STATUS
EFIAPI
InitializePeiFrb (
  IN        EFI_PEI_FILE_HANDLE    FileHandle,
  IN  CONST EFI_PEI_SERVICES       **PeiServices )
{
    EFI_STATUS                          Status;
    PEI_IPMI_TRANSPORT_PPI              *IpmiTransportPpi;
    AMI_BMC_SET_WATCHDOG_TIMER_REQUEST  SetWatchDogTimerData;
    UINT8                               FrbTimerDataSize;
    BOOLEAN                             FrbEnabled = TRUE; // Default is Enabled.
    UINT8                               ResponseDataSize;
    UINT8                               CompletionCode;
    static EFI_PEI_NOTIFY_DESCRIPTOR    MasterBootModeCallbackNotify[] = {
                                                                            EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | \
                                                                            EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
                                                                            &gEfiPeiMasterBootModePpiGuid,
                                                                            StopFrb2Timer
                                                                         };

    // Locate Ipmi Transport Ppi to send commands to BMC.
    Status = (*PeiServices)->LocatePpi (
                                PeiServices,
                                &gEfiPeiIpmiTransportPpiGuid,
                                0,
                                NULL,
                                (VOID **)&IpmiTransportPpi );
    
    DEBUG ((DEBUG_INFO, " gEfiPeiIpmiTransportPpiGuid Status: %r \n", Status));
    
    if (EFI_ERROR(Status)) {
        return Status;
    }

    // Hook to disable Frb3 timer.
    DisableFrb3Timer (PeiServices);

    ZeroMem (
       &SetWatchDogTimerData,
       sizeof (AMI_BMC_SET_WATCHDOG_TIMER_REQUEST) );
    // Get Frb2 info from setup.
    GetFrbSetupOptions (
        PeiServices,
        &SetWatchDogTimerData,
        &FrbEnabled );
    
    DEBUG ((DEBUG_INFO, " FrbEnabled: %x \n", FrbEnabled));

    // Enable Frb2 timer if enabled in Setup.
    if (FrbEnabled) {
        // Data Byte-1.
        SetWatchDogTimerData.TimerUse.Bits.TimerUse = AmiIpmiFrb2;
        SetWatchDogTimerData.TimerUse.Bits.TimerRunning = AMI_IPMI_STOP_TIMER_WHEN_SET_WDT;
        SetWatchDogTimerData.TimerUse.Bits.TimerUseExpirationFlagLog = AMI_IPMI_CLEAR_FLAG;

        // Data Byte-2.
        SetWatchDogTimerData.TimerActions.PreTimeoutIntr = AmiIpmiNoTimeoutInterrupt;

        // Data Byte-3.
        SetWatchDogTimerData.PretimeoutInterval = 0;

        // Data Byte-4.
        SetWatchDogTimerData.TimerUseExpirationFlagsClear.Frb2ExpirationFlag = AMI_IPMI_SET_FLAG;

        FrbTimerDataSize = sizeof (SetWatchDogTimerData);
        ResponseDataSize = sizeof (CompletionCode);

        Status = IpmiTransportPpi->SendIpmiCommand (
                                        IpmiTransportPpi,
                                        IPMI_NETFN_APP,
                                        AMI_BMC_LUN,
                                        IPMI_APP_SET_WATCHDOG_TIMER,
                                        (UINT8*)&SetWatchDogTimerData,
                                        FrbTimerDataSize,
                                        &CompletionCode,
                                        &ResponseDataSize );
        
        DEBUG ((DEBUG_INFO, " IPMI_APP_SET_WATCHDOG_TIMER Status: %r \n", Status));
        
        if (EFI_ERROR(Status)) {
            return Status;
        }

        // Issue reset watchdog timer command to start timer.
        ResponseDataSize = sizeof (CompletionCode);
        Status = IpmiTransportPpi->SendIpmiCommand (
                                        IpmiTransportPpi,
                                        IPMI_NETFN_APP,
                                        AMI_BMC_LUN,
                                        IPMI_APP_RESET_WATCHDOG_TIMER,
                                        NULL,
                                        0,
                                        &CompletionCode,
                                        &ResponseDataSize );
        
        DEBUG ((DEBUG_INFO, " IPMI_APP_RESET_WATCHDOG_TIMER Status: %r \n", Status));

        // Registering Notify for Master Boot Mode ppi if frb2 timer has started.
        if (!EFI_ERROR(Status)) {
            Status = (*PeiServices)->NotifyPpi (
                                        PeiServices,
                                        MasterBootModeCallbackNotify );
        }
        return Status;
    }

    return EFI_UNSUPPORTED;
}


