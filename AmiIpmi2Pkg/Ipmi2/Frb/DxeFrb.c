//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************


/** @file DxeFrb.c
    Implemented FRB Dxe protocol functions and register
    notifications to disable and enable Frb2 and OS watchdog timers.

*/

//---------------------------------------------------------------------------

#include "DxeFrb.h"

//---------------------------------------------------------------------------

/**
    This API gets the FRB timer status.

    @param[in]  This            Pointer to the FRB protocol.
    @param[in]  FrbType         Type of FRB timer to get data on.
    @param[out] FrbStatData     Timer data is returned in this structure.

    @retval EFI_SUCCESS             Data successfully retrieved and copied
                                    to structure.
    @retval EFI_UNSUPPORTED         Not supported for current FRB type.
    @retval EFI_INVALID_PARAMETER   Invalid parameter passed in.
    @retval EFI_NOT_FOUND           Frb timer is not same as timer in use.
    @retval Others                  Error status from functions calls used.

**/
EFI_STATUS
EFIAPI
GetFrbStatus (
  IN  EFI_SM_FRB_PROTOCOL       *This,
  IN  AMI_WATCHDOG_TIMER_USE    FrbType,
  OUT EFI_FRB_STAT              *FrbStatData )
{
    EFI_STATUS                              Status;
    AMI_BMC_GET_WATCHDOG_TIMER_RESPONSE     GetWatchDogTimerData;
    UINT8                                   FrbTimerDataSize;

    DEBUG ((DEBUG_INFO, "%a Entered... FrbType: %x \n", __FUNCTION__, FrbType));
    if (FrbStatData == NULL) {
        return EFI_INVALID_PARAMETER;
    }

    if (FrbType < AmiIpmiFrb2 || FrbType > AmiIpmiOsBootWdt) {
        return EFI_UNSUPPORTED;
    }

    FrbTimerDataSize = sizeof (GetWatchDogTimerData);
    Status = gIpmiTransport->SendIpmiCommand (
                                gIpmiTransport,
                                IPMI_NETFN_APP,
                                AMI_BMC_LUN,
                                IPMI_APP_GET_WATCHDOG_TIMER,
                                NULL,
                                0,
                                (UINT8 *)&GetWatchDogTimerData,
                                &FrbTimerDataSize );
    
    DEBUG ((DEBUG_INFO, " IPMI_APP_GET_WATCHDOG_TIMER Status: %r \n", Status));
    
    if (EFI_ERROR (Status) || (GetWatchDogTimerData.CompletionCode)) {
        return Status;
    }
    // Check the timer use field data,If its reserved need to set Watchdog timer type.
    if (GetWatchDogTimerData.TimerUse.Bits.TimerUse == AmiIpmiFrb2Reserved1) {
        return EFI_NOT_STARTED;
    }
    
    // Check the timer use field is it actually same as the FrbType.
    if (GetWatchDogTimerData.TimerUse.Bits.TimerUse != FrbType) {
        return EFI_NOT_FOUND;
    }
    
    FrbStatData->FrbTimeout           = (UINTN)(GetWatchDogTimerData.InitialCountdownValue * 100000);      // Timeout in MicroSeconds
    FrbStatData->FrbCurrentBootPolicy = GetWatchDogTimerData.TimerActions.TimeoutAction;
    FrbStatData->FrbTimeoutInterrupt  = GetWatchDogTimerData.TimerActions.PreTimeoutIntr;
    FrbStatData->FrbEnabled           = GetWatchDogTimerData.TimerUse.Bits.TimerRunning;           // This bit defines whether FRB is running or not.

    
    switch (FrbType) {
        case AmiIpmiFrb2:
            FrbStatData->FrbFailed = GetWatchDogTimerData.TimerUseExpirationFlagsClear.Frb2ExpirationFlag;  // FRB2 timer expiration flag.
            break;
        case AmiIpmiOsBootWdt:
            FrbStatData->FrbFailed = GetWatchDogTimerData.TimerUseExpirationFlagsClear.OsLoadExpirationFlag;  // OS load expiration flag.
            break;
        case AmiIpmiBiosPost:
            FrbStatData->FrbFailed = GetWatchDogTimerData.TimerUseExpirationFlagsClear.PostExpirationFlag;  // POST expiration flag.
            break;
        default :
            DEBUG ((DEBUG_INFO, "Invalid Frb Type: %x \n", FrbType));
            break;
    }

    DEBUG ((DEBUG_INFO, "%a Exiting... FrbStatData->FrbEnabled: %x FrbStatData->FrbFailed: %x \n", __FUNCTION__, FrbStatData->FrbEnabled, FrbStatData->FrbFailed));

    return EFI_SUCCESS;
}

/**
    This API sets the FRB timer status.

    @param[in] This     Pointer to the FRB protocol.
    @param[in] FrbType  Type of FRB timer to get data on.
    @param[in] Timeout  Timer timeout value in microseconds.

    @retval EFI_SUCCESS             Timer successfully set.
    @retval EFI_UNSUPPORTED         Not supported for current FRB type.
    @retval EFI_INVALID_PARAMETER   Invalid parameter passed in.

**/
EFI_STATUS
EFIAPI
SetFrbTimerValue (
  IN EFI_SM_FRB_PROTOCOL    *This,
  IN AMI_WATCHDOG_TIMER_USE FrbType,
  IN UINTN                  *Timeout )
{
   DEBUG ((DEBUG_INFO, "%a Entered FrbType: %x *Timeout: %x \n", __FUNCTION__, FrbType, *Timeout));

   if (FrbType < AmiIpmiFrb2 || FrbType > AmiIpmiOsBootWdt) {
       return EFI_UNSUPPORTED;
   }

   if (*Timeout <= 0) {
       return EFI_INVALID_PARAMETER;
   }

   // Save time out value in form of 100 millisecond per count.
   gFrbDataBuffer[FrbType].TimeOutValue = (UINT16)(*Timeout / 100000);

   DEBUG ((DEBUG_INFO, "%a Exit\n", __FUNCTION__));

   return EFI_SUCCESS;
}

/**
    This API set the FRB pre timeout policy.

    @param[in] This                 Pointer to the FRB protocol.
    @param[in] FrbType              Type of FRB timer to get data on.
    @param[in] PretimeoutInterval   Pre timeout interval in microseconds.
    @param[in] FrbTimeoutInterrupt  Type of FRB pre time out interrupt.

    @retval EFI_SUCCESS         Pre timeout interval successfully set.
    @retval EFI_UNSUPPORTED     Timer does not support pre timeout interval.

**/
EFI_STATUS
EFIAPI
SetFrbPreTimeOutPolicy (
  IN EFI_SM_FRB_PROTOCOL                *This,
  IN AMI_WATCHDOG_TIMER_USE             FrbType,
  IN UINTN                              *PretimeoutInterval,
  IN AMI_WATCHDOG_PRE_TIMEOUT_INTERRUPT FrbTimeoutInterrupt )
{
    DEBUG ((DEBUG_INFO, "%a Entered FrbType: %x FrbTimeoutInterrupt: %x \n", __FUNCTION__, FrbType, FrbTimeoutInterrupt));

    // Save the Pre timeout interrupt in volatile buffer.
    if (FrbType == AmiIpmiFrb2 || \
        FrbType == AmiIpmiOsBootWdt || \
        FrbType ==  AmiIpmiBiosPost) {
        if (PretimeoutInterval != NULL) {
            gFrbDataBuffer[FrbType].PretimeoutInterval = (*(UINT8 *)PretimeoutInterval);
        } else {
            gFrbDataBuffer[FrbType].PretimeoutInterval = 0;
        }
        gFrbDataBuffer[FrbType].PreTimeOutInterrupt = FrbTimeoutInterrupt;
        return EFI_SUCCESS;
    }
    return EFI_UNSUPPORTED;
}

/**
    This API sets the boot policy to take on an FRB watchdog timeout.

    @param[in] This             Pointer to the FRB protocol.
    @param[in] FrbType          Type of FRB timer to get data on.
    @param[in] FrbBootPolicy    Boot policy to take on FRB timeout.

    @retval EFI_SUCCESS         Boot policy set for selected FRB Type.
    @retval EFI_UNSUPPORTED     Boot policy or timer not supported.

**/
EFI_STATUS
EFIAPI
SetFrbBootPolicy (
  IN EFI_SM_FRB_PROTOCOL            *This,
  IN AMI_WATCHDOG_TIMER_USE         FrbType,
  IN AMI_WATCHDOG_TIMEOUT_ACTION    FrbBootPolicy )
{
    DEBUG ((DEBUG_INFO, "%a Entered FrbType: %x FrbBootPolicy: %x \n", __FUNCTION__, FrbType, FrbBootPolicy));

    // Save the FRB boot policy in volatile buffer.
    if (FrbType == AmiIpmiFrb2 || \
        FrbType == AmiIpmiOsBootWdt || \
        FrbType ==  AmiIpmiBiosPost) {
        gFrbDataBuffer[FrbType].BootPolicy = FrbBootPolicy;
        return EFI_SUCCESS;
    }
    return EFI_UNSUPPORTED;
}

/**
    This API enables and sets up the selected FRB timer.

    @param[in] This                 Pointer to the FRB protocol.
    @param[in] FrbType              Type of FRB timer to get data on.
    @param[in] ClearFrbGlobalStatus Clear global status of the FRB.

    @retval EFI_SUCCESS         FRB timer is enabled successfully.
    @retval EFI_DEVICE_ERROR    Couldn't Enable FRB-n because of device
                                failure.
    @retval EFI_UNSUPPORTED     FRB timer is not supported.
    @retval EFI_TIMEOUT         Residual count is already 0, timed out.
    @retval Others              Error status from functions calls used.

**/
EFI_STATUS
EFIAPI
EnableFrb (
  IN EFI_SM_FRB_PROTOCOL    *This,
  IN AMI_WATCHDOG_TIMER_USE FrbType,
  IN BOOLEAN                ClearFrbGlobalStatus )
{
    EFI_STATUS                              Status;
    AMI_BMC_GET_WATCHDOG_TIMER_RESPONSE     GetWatchDogTimerData;
    AMI_BMC_SET_WATCHDOG_TIMER_REQUEST      SetWatchDogTimerData;
    UINT8                                   FrbTimerDataSize;
    UINT8                                   ResponseDataSize;
    EFI_PEI_HOB_POINTERS                    HobList;
    EFI_BOOT_MODE                           BootMode;
    UINT8                                   CompletionCode;

    DEBUG ((DEBUG_INFO, "%a Entered... FrbType: %x\n", __FUNCTION__, FrbType));

    if (FrbType < AmiIpmiFrb2 || FrbType > AmiIpmiOsBootWdt) {
        return EFI_UNSUPPORTED;
    }

    if (FrbType == AmiIpmiFrb2) {
        // Code added to not enable frb2 timer in recovery mode.
        EfiGetSystemConfigurationTable (
            &gEfiHobListGuid,
            (VOID **)&HobList.Raw );

        BootMode = HobList.HandoffInformationTable->BootMode;
        if (HobList.Header->HobType != EFI_HOB_TYPE_HANDOFF) {
            DEBUG ((DEBUG_ERROR, "(DxeFrb) Hand off HOB missing!\n"));
            return EFI_NOT_FOUND;
        }
        if (BootMode == BOOT_IN_RECOVERY_MODE) {
            return EFI_UNSUPPORTED;
        }
    }

    // Get watchdog timer status to preserve reserved bits.
    FrbTimerDataSize = sizeof(GetWatchDogTimerData);
    Status = gIpmiTransport->SendIpmiCommand (
                                gIpmiTransport,
                                IPMI_NETFN_APP,
                                AMI_BMC_LUN,
                                IPMI_APP_GET_WATCHDOG_TIMER,
                                NULL,
                                0,
                                (UINT8 *)&GetWatchDogTimerData,
                                &FrbTimerDataSize );
    
    DEBUG ((DEBUG_INFO, "%a: IPMI_APP_GET_WATCHDOG_TIMER Status : %r\n", __FUNCTION__, Status));
    
    if (EFI_ERROR(Status) || (GetWatchDogTimerData.CompletionCode)) {
        return Status;
    }

    /* Load configuration parameters that was set by SetFrbBootPolicy,
       SetFrbPreTimeOutPolicy and SetFrbTimerValue APIs. otherwise load
       configuration parameters that was got from DisableFrb API.*/

    // Check if timer is already expired or not.
    if (gFrbDataBuffer[FrbType].TimeOutValue == 0) {
        return EFI_TIMEOUT;
    }

    ZeroMem (
       &SetWatchDogTimerData,
       sizeof (AMI_BMC_SET_WATCHDOG_TIMER_REQUEST) );
    // Data Byte-1
    SetWatchDogTimerData.TimerUse.Bits.TimerUse                  = FrbType;
    SetWatchDogTimerData.TimerUse.Bits.TimerRunning              = AMI_IPMI_STOP_TIMER_WHEN_SET_WDT;
    SetWatchDogTimerData.TimerUse.Bits.TimerUseExpirationFlagLog = AMI_IPMI_CLEAR_FLAG;

    // Data Byte-2
    SetWatchDogTimerData.TimerActions.TimeoutAction         = gFrbDataBuffer[FrbType].BootPolicy;
    SetWatchDogTimerData.TimerActions.PreTimeoutIntr        = gFrbDataBuffer[FrbType].PreTimeOutInterrupt;

    // Data Byte-3
    SetWatchDogTimerData.PretimeoutInterval                 = gFrbDataBuffer[FrbType].PretimeoutInterval;

    // Data Byte-4
    // Clear global expiration flags based on input.
    if (ClearFrbGlobalStatus) {
        SetWatchDogTimerData.TimerUseExpirationFlagsClear.Frb2ExpirationFlag    = AMI_IPMI_SET_FLAG;
        SetWatchDogTimerData.TimerUseExpirationFlagsClear.PostExpirationFlag    = AMI_IPMI_SET_FLAG;
        SetWatchDogTimerData.TimerUseExpirationFlagsClear.OsLoadExpirationFlag  = AMI_IPMI_SET_FLAG;
        SetWatchDogTimerData.TimerUseExpirationFlagsClear.SmsOsExpirationFlag   = AMI_IPMI_SET_FLAG;
        SetWatchDogTimerData.TimerUseExpirationFlagsClear.OemExpirationFlag     = AMI_IPMI_SET_FLAG;
    } else {
        switch (FrbType) {
            case AmiIpmiFrb2:
                SetWatchDogTimerData.TimerUseExpirationFlagsClear.Frb2ExpirationFlag = AMI_IPMI_SET_FLAG;
                break;
            case AmiIpmiOsBootWdt:
                SetWatchDogTimerData.TimerUseExpirationFlagsClear.OsLoadExpirationFlag = AMI_IPMI_SET_FLAG;
                break;
            case AmiIpmiBiosPost:
                SetWatchDogTimerData.TimerUseExpirationFlagsClear.PostExpirationFlag = AMI_IPMI_SET_FLAG;
                break;
            default :
                DEBUG ((DEBUG_INFO, "Invalid Frb Type: %x \n", FrbType));
                break;
        }
    }

    // Data Byte-5 and Byte-6
    SetWatchDogTimerData.InitialCountdownValue = gFrbDataBuffer[FrbType].TimeOutValue;

    FrbTimerDataSize    = sizeof (SetWatchDogTimerData);
    ResponseDataSize    = sizeof (CompletionCode);

    Status = gIpmiTransport->SendIpmiCommand (
                                gIpmiTransport,
                                IPMI_NETFN_APP,
                                AMI_BMC_LUN,
                                IPMI_APP_SET_WATCHDOG_TIMER,
                                (UINT8 *)&SetWatchDogTimerData,
                                FrbTimerDataSize,
                                &CompletionCode,
                                &ResponseDataSize );
    
    DEBUG ((DEBUG_INFO, " IPMI_APP_SET_WATCHDOG_TIMER Status: %r \n", Status));
    
    if (EFI_ERROR(Status)) {
        return Status;
    }

    // Issue reset watch dog timer command to start timer
    CompletionCode      = 0;
    ResponseDataSize    = sizeof (CompletionCode);
    Status = gIpmiTransport->SendIpmiCommand (
                                gIpmiTransport,
                                IPMI_NETFN_APP,
                                AMI_BMC_LUN,
                                IPMI_APP_RESET_WATCHDOG_TIMER,
                                NULL,
                                0,
                                &CompletionCode,
                                &ResponseDataSize );
    DEBUG ((DEBUG_INFO, "%a: IPMI_APP_RESET_WATCHDOG_TIMER Status: %r \n", __FUNCTION__, Status));

    return Status;
}

/**
    This API disables the specified FRB timer.

    @param[in] This     Pointer to the FRB protocol.
    @param[in] FrbType  Type of FRB timer to get data on.

    @retval EFI_SUCCESS         FRB timer is disabled successfully.
    @retval EFI_UNSUPPORTED     FRB timer is not supported.
    @retval EFI_ABORTED         Timer was already stopped.
    @retval EFI_NOT_FOUND       Timer running is not same as FrbType.
    @retval Others              Error status from functions calls used.

**/
EFI_STATUS
EFIAPI
DisableFrb (
  IN EFI_SM_FRB_PROTOCOL    *This,
  IN AMI_WATCHDOG_TIMER_USE FrbType )
{
    EFI_STATUS                              Status;
    AMI_BMC_GET_WATCHDOG_TIMER_RESPONSE     GetWatchDogTimerData;
    AMI_BMC_SET_WATCHDOG_TIMER_REQUEST      SetWatchDogTimerData;
    UINT8                                   FrbTimerDataSize;
    UINT8                                   ResponseDataSize;
    UINT8                                   CompletionCode;

    DEBUG ((DEBUG_INFO, "%a Entered... FrbType: %x \n", __FUNCTION__, FrbType));

    if (FrbType < AmiIpmiFrb2 || FrbType > AmiIpmiOsBootWdt) {
        return EFI_UNSUPPORTED;
    }

    // Get FRB status to check whether timer is already stopped or not.
    FrbTimerDataSize = sizeof (GetWatchDogTimerData);
    Status = gIpmiTransport->SendIpmiCommand (
                                gIpmiTransport,
                                IPMI_NETFN_APP,
                                AMI_BMC_LUN,
                                IPMI_APP_GET_WATCHDOG_TIMER,
                                NULL,
                                0,
                                (UINT8 *)&GetWatchDogTimerData,
                                &FrbTimerDataSize );
    
    DEBUG ((DEBUG_INFO, "%a: IPMI_APP_GET_WATCHDOG_TIMER Status: %r \n", __FUNCTION__, Status));
    
    if (EFI_ERROR(Status) || (GetWatchDogTimerData.CompletionCode)) {
        return Status;
    }

    // If timer already stopped then return EFI_ABORTED.
    if (!GetWatchDogTimerData.TimerUse.Bits.TimerRunning) {
        DEBUG ((DEBUG_ERROR, "%a: EfiFrb2 already disabled  \n", __FUNCTION__));
        return EFI_ABORTED;
    }

    // Timer running should be same as FrbType.
    if (GetWatchDogTimerData.TimerUse.Bits.TimerUse != FrbType) {
        return EFI_NOT_FOUND;
    }

    /* Store current count down and time out policies in volatile buffer. In case EnableFrb called
       without calling SetFrbTimer and set FRB policy APIs to configure FRB.*/
    gFrbDataBuffer[FrbType].TimeOutValue        = GetWatchDogTimerData.PresentCountdownValue;
    gFrbDataBuffer[FrbType].PretimeoutInterval  = GetWatchDogTimerData.PretimeoutInterval;
    gFrbDataBuffer[FrbType].PreTimeOutInterrupt = GetWatchDogTimerData.TimerActions.PreTimeoutIntr;
    gFrbDataBuffer[FrbType].BootPolicy          = GetWatchDogTimerData.TimerActions.TimeoutAction;

    /* Disable timer by using set watchdog timer with timer value and DontStopTimer bit value as zero.
       while disabling don't clear global expiration flag.*/
    ZeroMem (
       &SetWatchDogTimerData, 
       sizeof (AMI_BMC_SET_WATCHDOG_TIMER_REQUEST) );
    SetWatchDogTimerData.TimerUse.Bits.TimerUse      = FrbType;
    SetWatchDogTimerData.TimerUse.Bits.TimerRunning  = AMI_IPMI_STOP_TIMER_WHEN_SET_WDT;
    SetWatchDogTimerData.InitialCountdownValue       = AMI_IPMI_CLEAR_FLAG;

    FrbTimerDataSize = sizeof (SetWatchDogTimerData);
    ResponseDataSize = sizeof (CompletionCode);

    Status = gIpmiTransport->SendIpmiCommand (
                                gIpmiTransport,
                                IPMI_NETFN_APP,
                                AMI_BMC_LUN,
                                IPMI_APP_SET_WATCHDOG_TIMER,
                                (UINT8 *)&SetWatchDogTimerData,
                                FrbTimerDataSize,
                                &CompletionCode,
                                &ResponseDataSize );
    
    DEBUG ((DEBUG_INFO, "%a: IPMI_APP_SET_WATCHDOG_TIMER Status: %r \n", __FUNCTION__, Status));
    return Status;
}

/** @internal
    Enables OsWdt Timer.

    @param VOID     Nothing.

    @return EFI_STATUS     Return status of function calls used.

**/
EFI_STATUS
EnableOsWdtTimer (
  VOID )
{
    EFI_STATUS  Status = EFI_SUCCESS;
    UINTN       TimeOut;

    // Enable OsWdt Timer.
    if (gServerMgmtConfiguration.OsBootWdtEnable) {
        // Set the OsBootWdt Timeout value in  micro seconds.
        TimeOut = ((UINTN)gServerMgmtConfiguration.OsBootWdtTimeout)* 60 * 1000000; 
        Status = SetFrbTimerValue (
                    gFrbProtocol,
                    AmiIpmiOsBootWdt,
                    &TimeOut );

        // Set the OsBootWdt timeout policy.
        Status = SetFrbBootPolicy (
                    gFrbProtocol,
                    AmiIpmiOsBootWdt,
                    gServerMgmtConfiguration.OsBootWdtTimeoutPolicy );

        // Set Pre Timeout interrupt.
        Status = SetFrbPreTimeOutPolicy (
                    gFrbProtocol,
                    AmiIpmiOsBootWdt,
                    NULL,
                    AmiIpmiNoTimeoutInterrupt );

        // Start the OsBootWd timer.
        Status = EnableFrb (
                    gFrbProtocol,
                    AmiIpmiOsBootWdt,
                    FALSE );
    }
    DEBUG ((DEBUG_INFO, " Status(Oswdt expiry function) = %x\n", Status));
    return Status;
}

/**
    Disables FRB2. This function gets called each time the
    EFI_AMI_LEGACYBOOT_PROTOCOL_GUID gets signaled.

    @param[in] Event    Event which caused this handler.
    @param[in] Context  Context passed during Event Handler registration.

    @return VOID    Nothing.

**/
VOID
EFIAPI
FrbEndOfPostAmiLegacyBootEvent (
  IN EFI_EVENT  Event,
  IN VOID       *Context )
{
    EFI_STATUS  Status;
    VOID        *LegacyBootProtocol = NULL;

    DEBUG ((DEBUG_INFO, "IPMI FRB : Entering Legacy Boot using gAmiLegacyBootProtocolGuid. Disabling FRB and enabling OS WDT.\n"));
    Status = gBS->LocateProtocol(
                    &gAmiLegacyBootProtocolGuid,
                    NULL,
                    (VOID **)&LegacyBootProtocol );
    
    DEBUG ((DEBUG_INFO, "LocateProtocol gAmiLegacyBootProtocolGuid Status = %r\n", Status));

    if(!EFI_ERROR(Status)) {
        if (gServerMgmtConfiguration.Frb2Enable) {
            Status = DisableFrb (
                        gFrbProtocol,
                        AmiIpmiFrb2 );
            DEBUG ((DEBUG_INFO, "EfiFrb2 disabled Status = %r\n", Status));
        }
        
        if (gServerMgmtConfiguration.OsBootWdtEnable) {
            Status = EnableOsWdtTimer ();
            DEBUG ((DEBUG_INFO, "EfiOsBootWdt Enabled Status = %r\n", Status));
        }
    }
}

/**
    Disables FRB2 and enables WDT. This function gets called,
    In legacy boot. - When legacy boot event gets signaled.
    In UEFI boot    - When AMITSE_EVENT_BEFORE_BOOT_GUID is signaled.

    @param[in] Event    Event which caused this handler.
    @param[in] Context  Context passed during Event Handler registration.

    @return VOID    Nothing.

**/
VOID
EFIAPI
FrbEndOfPostEvent (
  IN EFI_EVENT  Event,
  IN VOID       *Context )
{
    EFI_STATUS      Status;
    DEBUG ((DEBUG_INFO, "IPMI FRB : UEFI Boot - Disabling FRB  and enabling OS WDT.\n"));
    
    if (gServerMgmtConfiguration.Frb2Enable) {
          Status = DisableFrb (
                    gFrbProtocol,
                    AmiIpmiFrb2 );
          DEBUG ((DEBUG_INFO, "EfiFrb2 disabled Status = %r\n", Status));
    }

    if (gServerMgmtConfiguration.OsBootWdtEnable) {
          Status = EnableOsWdtTimer ();
          DEBUG ((DEBUG_INFO, "EfiOsBootWdt Enabled Status = %r\n", Status));
    }
}

/**
    Disables FRB2. This function gets called each time when
    AmiTseSetupEnterGuid is installed.

    @param[in] Event    Event which caused this handler.
    @param[in] Context  Context passed during Event Handler registration.

    @return VOID    Nothing.

**/
VOID
EFIAPI
DisableTimersBeforeSetup (
  IN EFI_EVENT  Event,
  IN VOID       *Context )
{
    EFI_STATUS  Status;

    DEBUG ((DEBUG_INFO, "IPMI FRB : Entering Setup. Disabling FRB/OS WDT if they are active.\n"));
    
    if (gServerMgmtConfiguration.Frb2Enable) {
        Status = DisableFrb (
                    gFrbProtocol,
                    AmiIpmiFrb2 );
        DEBUG ((DEBUG_INFO, "EfiFrb2 disabled Before setup Status = %r\n", Status));
        
        if (gServerMgmtConfiguration.OsBootWdtEnable) {
            PcdSetBool (
                AmiPcdOsWdtDisabledOnSetupEntry,
                TRUE );
            Status = DisableFrb (
                        gFrbProtocol,
                        AmiIpmiOsBootWdt );
            DEBUG ((DEBUG_INFO, "EfiOsBootWdt disabled Before setup Status = %r\n", Status));
            DEBUG ((DEBUG_INFO, "AmiPcdOsWdtDisabledOnSetupEntry = %x\n", PcdGetBool (AmiPcdOsWdtDisabledOnSetupEntry)));
        }
    }
}

/**
    Disable and Enable FRB2 during OptionRom scan. This function gets called
    each time when OptionRomScan protocol installed.

    @param[in] Event    Event which caused this handler.
    @param[in] Context  Context passed during Event Handler registration.

    @return VOID    Nothing.

**/
VOID
EFIAPI
DisableEnableFrb2DuringOpRomScan (
  IN EFI_EVENT  Event,
  IN VOID       *Context )
{
    static BOOLEAN  FrbDisabled;
    EFI_STATUS      Status;

    DEBUG ((DEBUG_INFO, "IPMI FRB : OPROM Scan. Disable/reenable FRB wdt.\n"));

    if (!FrbDisabled) {
        Status = DisableFrb (
                    gFrbProtocol,
                    AmiIpmiFrb2 );
        
        if (!EFI_ERROR(Status)) {
            DEBUG ((DEBUG_INFO, "EfiFrb2 disabled Before OPRom Status = %r\n", Status));
            FrbDisabled = TRUE;
        }
    } else {
        Status = EnableFrb (
                    gFrbProtocol,
                    AmiIpmiFrb2,
                    FALSE );
        
        if (!EFI_ERROR(Status)) {
            FrbDisabled = FALSE;
            DEBUG ((DEBUG_INFO, "EfiFrb2 Enabled after OPRom Status = %r\n", Status));
        }
    }
}

/**
    Disables FRB2 timer when AMITSE password prompt enter event is signaled.

    @param[in] Event    Event which caused this handler.
    @param[in] Context  Context passed during Event Handler registration.

    @return VOID    Nothing.

**/
VOID
EFIAPI
DisableFrb2BeforePassword (
  IN EFI_EVENT  Event,
  IN VOID       *Context )
{
    EFI_STATUS      Status;
    DEBUG ((DEBUG_INFO, "IPMI FRB : Password Prompt Enter. Disable FRB wdt.\n"));
    
    Status = DisableFrb (
                gFrbProtocol,
                AmiIpmiFrb2 );
    DEBUG ((DEBUG_INFO, "EfiFrb2 Disabled Before Giving Password= %r\n", Status));
}

/**
    Enables FRB2 timer when AMITSE password prompt exit event is signaled.

    @param[in] Event    Event which caused this handler.
    @param[in] Context  Context passed during Event Handler registration.

    @return VOID    Nothing.

**/
VOID
EFIAPI
EnableFrb2AfterPassword (
  IN EFI_EVENT  Event,
  IN VOID       *Context )
{
    EFI_STATUS      Status;
    DEBUG ((DEBUG_INFO, "IPMI FRB : Password Prompt Exit. Enable FRB wdt.\n"));
    
    Status = EnableFrb (
                gFrbProtocol,
                AmiIpmiFrb2,
                FALSE );
    DEBUG ((DEBUG_INFO, "EfiFrb2 Enabled after Giving Password= %r\n", Status));
}

/**
    Disables FRB2 timer when event signaled before AMITSE Key Press prompt
    timeout start.

    @param[in] Event    Event which caused this handler.
    @param[in] Context  Context passed during Event Handler registration.

    @return VOID    Nothing.

**/
VOID
EFIAPI
DisableFrb2BeforePromptTimeOut (
  IN EFI_EVENT  Event,
  IN VOID       *Context )
{
    EFI_STATUS              Status;
    DEBUG ((DEBUG_INFO, "IPMI FRB : Prompt Timeout Enter. Disable FRB wdt.\n"));
    
    Status = DisableFrb (
               gFrbProtocol,
               AmiIpmiFrb2);
    DEBUG ((DEBUG_INFO, "EfiFrb2 Disabled Before PromptTimeOut Start = %r\n", Status));
}

/**
    Enables FRB2 timer when event signaled after AMITSE Key Press prompt
    timeout ends.

    @param[in] Event    Event which caused this handler.
    @param[in] Context  Context passed during Event Handler registration.

    @return VOID    Nothing.

**/
VOID
EFIAPI
EnableFrb2AfterPromptTimeOut (
  IN EFI_EVENT  Event,
  IN VOID       *Context )
{
    EFI_STATUS  Status;
    DEBUG ((DEBUG_INFO, "IPMI FRB : Prompt Timeout Exit. Enable FRB wdt.\n"));
    
    Status = EnableFrb (
                gFrbProtocol,
                AmiIpmiFrb2,
                FALSE );
    DEBUG ((DEBUG_INFO, "EfiFrb2 Enabled after Prompt TimeOut Ends= %r\n", Status));
}

/**
    Disables OS WDT on entering Shell.

    @param[in] Event    Event which caused this handler.
    @param[in] Context  Context passed during Event Handler registration.

    @return VOID    Nothing.

**/
VOID
EFIAPI
DisableOsWdtBeforeShell (
  IN EFI_EVENT  Event,
  IN VOID       *Context) {

    EFI_STATUS  Status;

    DEBUG ((DEBUG_INFO, "IPMI FRB : Shell Enter. Disable OS WDT.\n"));
    
    Status = DisableFrb (
                gFrbProtocol,
                AmiIpmiOsBootWdt );
    DEBUG ((DEBUG_INFO, "OS WDT is disabled on entering shell. Status = %r\n",Status));
}

/**
    Clears a flag to indicate first boot option is clicked and OS WDT can be
    started.

    @param[in] Event    Event which caused this handler.
    @param[in] Context  Context passed during Event Handler registration.

    @return VOID    Nothing.

**/
VOID
EFIAPI
ClearFlag (
  IN EFI_EVENT  Event,
  IN VOID       *Context )
{
    DEBUG ((DEBUG_INFO, "IPMI FRB : Boot option booted from setup.Clearing flag.\n"));
    PcdSetBool (
        AmiPcdOsWdtDisabledOnSetupEntry,
        FALSE );
    DEBUG ((DEBUG_INFO, "Flag set to False.\n"));
}

/**
    Entry point of the Dxe FRB Driver.

    @param[in] ImageHandle  Handle of this driver image.
    @param[in] SystemTable  Table containing standard EFI services.

    @retval EFI_OUT_OF_RESOURCES    Failed to allocate memory.
    @retval EFI_SUCCESS             Driver initialized and installed
                                    protocol successfully started.
    @retval Others                  Error status of functions calls used.

**/
EFI_STATUS
EFIAPI
InitializeDxeFrb (
  IN EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE     *SystemTable )
{
    EFI_STATUS                              Status;
    EFI_HANDLE                              FrbHandle;
    UINTN                                   Size;
    EFI_GUID                                EfiShellEnvironmentGuid = SHELL_ENVIRONMENT_PROTOCOL_GUID;
    EFI_GUID                                EfiShellProtocolGuid = EFI_SHELL_PROTOCOL_GUID;
    EFI_GUID                                AmiHddPasswordPromptEnterGuid = AMI_HDD_PASSWORD_PROMPT_ENTER_GUID;
    EFI_GUID                                AmiHddPasswordPromptExitGuid = AMI_HDD_PASSWORD_PROMPT_EXIT_GUID;
    VOID                                    *Frb2PostEventRegId = NULL;
    EFI_FRB_STAT                            FrbStatData;
    AMI_BMC_SET_WATCHDOG_TIMER_REQUEST      SetWatchDogTimerData = {0};

    DEBUG ((DEBUG_INFO, "%a Entered...\n", __FUNCTION__));

    //
    // Get Server Mgmt Setup Variable
    //
    Size = sizeof (SERVER_MGMT_CONFIGURATION_DATA);
    Status = gRT->GetVariable (
                    L"ServerSetup",
                    &gEfiServerMgmtSetupVariableGuid,
                    NULL,
                    &Size,
                    &gServerMgmtConfiguration );
    DEBUG ((DEBUG_INFO, "gRT->GetVariable Status %r \n", Status));
    if(EFI_ERROR(Status)) {
        //
        // load default value.
        //
        gServerMgmtConfiguration.Frb2Enable = AMI_IPMI_DO_NOT_STOP_TIMER_WHEN_SET_WDT; // enabled
        gServerMgmtConfiguration.OsBootWdtEnable = AMI_IPMI_STOP_TIMER_WHEN_SET_WDT; //disabled
        gServerMgmtConfiguration.OsBootWdtTimeout = DEFAULT_OSBOOTWDT_TIMEOUT; //Default time-out value.
        gServerMgmtConfiguration.OsBootWdtTimeoutPolicy = AmiIpmiHardReset; //hard reset
        //Data Byte-2 TimeoutAction
        SetWatchDogTimerData.TimerActions.TimeoutAction = AmiIpmiNormalBoot;
        //Data Byte-5 and Byte-6
        SetWatchDogTimerData.InitialCountdownValue = DEFAULT_FRB2_TIMEOUT * AMI_MIN_TO_100MS;  // Default 6 minutes
    }else {
         //
         // Data Byte-2 Field
         //
         SetWatchDogTimerData.TimerActions.TimeoutAction = gServerMgmtConfiguration.Frb2TimeoutPolicy;

         //
         // least significant and most significant data bytes(5 and 6) of countdown value,The Watchdog Timer decrement is one count/100 ms.
         //
         SetWatchDogTimerData.InitialCountdownValue = gServerMgmtConfiguration.Frb2Timeout * AMI_MIN_TO_100MS;
    }

    if ( ( gServerMgmtConfiguration.Frb2Enable ) || ( gServerMgmtConfiguration.OsBootWdtEnable ) ) {

        // Locate DXE IPMI Transport protocol
        Status = gBS->LocateProtocol (
                            &gEfiDxeIpmiTransportProtocolGuid,
                            NULL,
                            (VOID **)&gIpmiTransport );
        DEBUG ((DEBUG_INFO, " gEfiDxeIpmiTransportProtocolGuid Status: %r \n", Status));

        /* If IPMI transport protocol is not available, then there is no point
        to produce FRB protocol for later use so just return.*/
        if (EFI_ERROR(Status)) {
            return Status;
        }

        gFrbProtocol = (EFI_SM_FRB_PROTOCOL *)AllocateZeroPool (sizeof (EFI_SM_FRB_PROTOCOL));
        if (gFrbProtocol == NULL) {
            DEBUG ((DEBUG_ERROR, "Failed to allocate memory for gFrbProtocol! \n"));
            return EFI_OUT_OF_RESOURCES;
        }

        // Install FRB protocol for later use.
        gFrbProtocol->GetFrbStatData            = GetFrbStatus;
        gFrbProtocol->SetFrbTimerData           = SetFrbTimerValue;
        gFrbProtocol->SetFrbPreTimeoutPolicy    = SetFrbPreTimeOutPolicy;
        gFrbProtocol->SetFrbBootPolicy          = SetFrbBootPolicy;
        gFrbProtocol->EnableFrb                 = EnableFrb;
        gFrbProtocol->DisableFrb                = DisableFrb;

        FrbHandle = NULL;
        Status = gBS->InstallProtocolInterface (
                            &FrbHandle,
                            &gEfiSmFrbProtocolGuid,
                            EFI_NATIVE_INTERFACE,
                            gFrbProtocol );
        DEBUG ((DEBUG_INFO, " gEfiSgFrbProtocolGuid Install protocol : Status: %r \n", Status));
        if (EFI_ERROR (Status)) {
            // Error in installing the protocol. So free the allocated memory.
            FreePool (gFrbProtocol);
            return Status;
        }
        if (gServerMgmtConfiguration.Frb2Enable) {
            //
            // Getting Frb Status 
            //
            Status = GetFrbStatus(gFrbProtocol, AmiIpmiFrb2,&FrbStatData );
            DEBUG ((DEBUG_INFO, "Getting  FRB timer status:  %r \n", Status));
            if (( !EFI_ERROR(Status) && !FrbStatData.FrbEnabled ) || ( Status == EFI_NOT_STARTED )){
                // Enabled the Frb2 timer Incase of still its disabled or Watchdog timer type is not initialized.
                 Status= SetFrb( &SetWatchDogTimerData);
                 if( EFI_ERROR(Status) ){
                     DEBUG ((DEBUG_INFO, " Error in Setting FRB Status: %r \n", Status));
                     return Status;
                 }
            }

            /* Register Event for OptionRom Scan GUID to disable FRB2 Timer
            before OptionRom scan and Enable after OptioRom scan.*/
            EfiNamedEventListen (
               &gOpromStartEndProtocolGuid,
               TPL_CALLBACK,
               DisableEnableFrb2DuringOpRomScan,
               NULL,
               NULL );

            /* Register Event for Password Prompt Enter GUID to disable FRB2
            Timer before entering the password.*/
            EfiNamedEventListen (
                &gAmiTsePasswordPromptEnterGuid,
                TPL_NOTIFY,
                DisableFrb2BeforePassword,
                NULL,
                NULL );

            /* Register Event for Password Prompt Exit GUID to Enable FRB2
            Timer after entering the password.*/
            EfiNamedEventListen (
                &gAmiTsePasswordPromptExitGuid,
                TPL_NOTIFY,
                EnableFrb2AfterPassword,
                NULL,
                NULL );

            //
            // Register Event for HDD Password Prompt Enter GUID to disable FRB2 Timer before entering the password.
            //
            EfiNamedEventListen (
                &AmiHddPasswordPromptEnterGuid,
                TPL_NOTIFY,
                DisableFrb2BeforePassword,
                NULL,
                NULL);

            //
            // Register Event for HDD Password Prompt Exit GUID to Enable FRB2 Timer after entering the password.
            //
            EfiNamedEventListen (
                &AmiHddPasswordPromptExitGuid,
                TPL_NOTIFY,
                EnableFrb2AfterPassword,
                NULL,
                NULL );
            
            /* Register Event for Setup Prompt timeout start GUID to disable
            FRB2 Timer before waiting for key press.*/
            EfiNamedEventListen (
                &gAmiTseBeforeTimeOutGuid,
                TPL_NOTIFY,
                DisableFrb2BeforePromptTimeOut,
                NULL,
                NULL );

            /* Register Event for Setup Prompt timeout end GUID to enable FRB2
            Timer after waiting for key press.*/
            EfiNamedEventListen (
                &gAmiTseAfterTimeOutGuid,
                TPL_NOTIFY,
                EnableFrb2AfterPromptTimeOut,
                NULL,
                NULL );
        } 
        if (gServerMgmtConfiguration.OsBootWdtEnable) {
             /* Register Event for EDK-I Shell guid to disable OS Wdt Timer
             before booting to Shell.*/
             EfiNamedEventListen (
                 &EfiShellEnvironmentGuid,
                 TPL_NOTIFY,
                 DisableOsWdtBeforeShell,
                 NULL,
                 NULL );

             /* Register Event for EDK-II Shell guid to disable OS Wdt Timer
             before booting to Shell.*/
             EfiNamedEventListen (
                 &EfiShellProtocolGuid,
                 TPL_NOTIFY,
                 DisableOsWdtBeforeShell,
                 NULL,
                 NULL );

             /* Register Event for AmiTseAfterFirstBootOptionGuid to clear a flag
             indicating first boot option is clicked and OS WDT can be started.*/
             EfiNamedEventListen (
                 &gAmiTseAfterFirstBootOptionGuid,
                 TPL_NOTIFY,
                 ClearFlag,
                 NULL,
                 NULL );
        }
        
        /* Register Event for Setup enter GUID to disable FRB2/OS wdt Timer
            before entering to setup.*/
        EfiNamedEventListen (
            &gAmiTseSetupEnterGuid,
            TPL_CALLBACK,
            DisableTimersBeforeSetup,
            NULL,
            NULL );

        /* Register event to disable FRB2 and Enable OS Watchdog timers
            before legacy boot.*/
        EfiCreateProtocolNotifyEvent (
            &gAmiLegacyBootProtocolGuid,
            TPL_CALLBACK,
            FrbEndOfPostAmiLegacyBootEvent,
            NULL,
            &Frb2PostEventRegId );

        /* Register event for AMITSE_EVENT_BEFORE_BOOT_GUID to disable FRB2
            timer before Uefi boot.*/
        EfiNamedEventListen (
            &gAmiTseEventBeforeBootGuid,
            TPL_CALLBACK,
            FrbEndOfPostEvent,
            NULL,
            NULL );
        }else {
             DEBUG ((DEBUG_INFO, "Both FRB and WatchDog Timers are Disabled \n"));
             return EFI_UNSUPPORTED;
        }

    return EFI_SUCCESS;
}

/** 
 *  It will set all the watchdog timer bits 
 *  
 *  @param VOID
 *  @retval EFI_SUCCESS. 
    @retval EFI_INVALID_PARAMETER. 
 */
EFI_STATUS
SetFrb (
  IN AMI_BMC_SET_WATCHDOG_TIMER_REQUEST        *SetWatchDogTimerData )
{
    EFI_STATUS                                          Status;
    UINT8                                               FrbTimerDataSize;
    UINT8                                               ResponseDataSize;
    UINT8                                               CompletionCode;
    //
    // Data Byte-1
    //
    SetWatchDogTimerData->TimerUse.Bits.TimerUse = AmiIpmiFrb2;
    SetWatchDogTimerData->TimerUse.Bits.TimerRunning = 0;
    SetWatchDogTimerData->TimerUse.Bits.TimerUseExpirationFlagLog = 0;
    //
    // Data Byte-2
    //
    SetWatchDogTimerData->TimerActions.PreTimeoutIntr = AmiIpmiNoTimeoutInterrupt;
    //
    // Data Byte-3
    //
    SetWatchDogTimerData->PretimeoutInterval = 0;
    //
    // Data Byte-4
    //
    SetWatchDogTimerData->TimerUseExpirationFlagsClear.Frb2ExpirationFlag = 1;
    FrbTimerDataSize = sizeof(AMI_BMC_SET_WATCHDOG_TIMER_REQUEST);
    ResponseDataSize = sizeof (CompletionCode);

    Status = gIpmiTransport->SendIpmiCommand (
            gIpmiTransport,
            IPMI_NETFN_APP,
            AMI_BMC_LUN,
            IPMI_APP_SET_WATCHDOG_TIMER,
            (UINT8*)SetWatchDogTimerData,
            FrbTimerDataSize,
            &CompletionCode,
            &ResponseDataSize);

    DEBUG ((DEBUG_INFO, " EFI_APP_SET_WATCHDOG_TIMER Status: %r \n", Status));
    if (EFI_ERROR(Status)) {
        return Status;
    }
    //
    // Issue reset watchdog timer command to start timer.
    //
    CompletionCode   = 0;
    ResponseDataSize = sizeof (CompletionCode);
    Status = gIpmiTransport->SendIpmiCommand (
            gIpmiTransport,
            IPMI_NETFN_APP,
            AMI_BMC_LUN,
            IPMI_APP_RESET_WATCHDOG_TIMER,
            NULL,
            0,
            &CompletionCode,
            &ResponseDataSize );
    DEBUG ((DEBUG_INFO, " EFI_APP_RESET_WATCHDOG_TIMER Status: %r \n", Status));
    return Status;
}

