//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2020, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************


/** @file IpmiBoot.c
    Sets the Boot Option Priority based on the IPMI Boot Flags.

*/

//---------------------------------------------------------------------------

#include <Setup.h>
#include "IpmiBoot.h"

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

EFI_IPMI_TRANSPORT              *gIpmiTransport;
BOOLEAN                         gPersistentBoot;
BOOLEAN                         gUefiBoot;
UINT8                           gDeviceInstanceSelector;
AMI_IPMI_BOOT_DEVICE_SELECTOR   gIpmiForceBootDevice;

//---------------------------------------------------------------------------

/** @internal
    This function Check the Redfish Feature supports,If redfish supports available then 
    the IPMI boot option will be handled by Redfish.

    @param  VOID    Nothing.

    @return  EFI_STATUS  Status returned by function calls used.

**/
EFI_STATUS
CheckRedfishSupport(
  VOID )
{
    EFI_STATUS              Status = EFI_NOT_FOUND;
#if REDFISH_BOOT_SUPPORT
    UINTN                   HandleCount = 0;
    EFI_HANDLE              *RestHandles = NULL;

    Status = gBS->LocateHandleBuffer(
                     ByProtocol,
                     &gAmiRedfishHiProtocolGuid,
                     NULL,
                     &HandleCount,
                     &RestHandles);

    if (!EFI_ERROR(Status))
        DEBUG ((DEBUG_INFO, "\n IPMI Boot: Check for Redfish Suport Status :  %r\n", Status));
#endif

    return Status;
}

#ifdef CSM_SUPPORT

/** @internal
    This function saves the Legacy device order details in NVRAM.

    @param  VOID    Nothing.

    @return VOID    Nothing.

**/
VOID
SaveLegacyDeviceOrderData (
  VOID )
{
    UINT8       *DeviceOrderBuffer;
    UINTN       DeviceOrderBufferSize;
    EFI_STATUS  Status;

    // Getting LegacyDevOrder variable and save in IpmiLegacyDevOrder variable.
    Status = GetVariable2 (
                    L"LegacyDevOrder",
                    &LegacyDevOrderGuid,
                    (VOID**)&DeviceOrderBuffer,
                    &DeviceOrderBufferSize );
    if (!EFI_ERROR(Status)) {
        Status = gRT->SetVariable (
                        L"IpmiLegacyDevOrder",
                        &gEfiIpmiBootGuid,
                        VAR_FLAG,
                        DeviceOrderBufferSize,
                        DeviceOrderBuffer );
        
        if (EFI_ERROR (Status)) {
            DEBUG ((DEBUG_ERROR, "IPMI Boot: IpmiLegacyDevOrder SetVariable fails.\n"));
        }
        // Freeing the memory allocated in GetVariableAndSize().
        gBS->FreePool (DeviceOrderBuffer);
        DeviceOrderBuffer = NULL;
    }

    // Getting OldLegacyDevOrder variable and save in IpmiOldLegacyDevOrder variable.
    Status = GetVariable2 (
                L"OldLegacyDevOrder",
                &LegacyDevOrderGuid,
                (VOID**)&DeviceOrderBuffer,
                &DeviceOrderBufferSize );
    if (!EFI_ERROR(Status)) {
        Status = gRT->SetVariable (
                          L"IpmiOldLegacyDevOrder",
                          &gEfiIpmiBootGuid,
                          VAR_FLAG,
                          DeviceOrderBufferSize,
                          DeviceOrderBuffer);
        
        if (EFI_ERROR (Status)) {
            DEBUG ((DEBUG_ERROR, "IPMI Boot: IpmiOldLegacyDevOrder SetVariable fails.\n"));
        }
        // Freeing the memory allocated in GetVariableAndSize().
        gBS->FreePool (DeviceOrderBuffer);
    }
}
#endif

/** @internal
    This function saves the matched boot option details in NVRAM for
    legacy case.

    @param[in] BootOptionNumber     Boot option number to save the entire
                                    structure in NVRAM.

    @return VOID    Nothing.

**/
VOID
SaveMatchedBootOptionData (
  IN UINT16 BootOptionNumber )
{
    CHAR16          BootStr[9];
    UINTN           NvramOptionSize;
    VOID            *Buffer;
    UINT8           *MatchedBootOptionDetails;
    EFI_STATUS      Status;

    Swprintf (BootStr, L"Boot%04X", BootOptionNumber);
    Status = GetVariable2 (
                BootStr,
                &EfiVariableGuid,
                &Buffer,
                &NvramOptionSize );
    if (!EFI_ERROR(Status)) {
        MatchedBootOptionDetails = AllocateZeroPool (NvramOptionSize + BOOT_OPTION_NUMBER_SIZE);
        if (MatchedBootOptionDetails != NULL) {
            *(UINT16 *)MatchedBootOptionDetails = BootOptionNumber;
            CopyMem (
                (MatchedBootOptionDetails + BOOT_OPTION_NUMBER_SIZE),
                Buffer,
                NvramOptionSize );

            Status = gRT->SetVariable (
                            L"IpmiMatchBootOptionData",
                            &gEfiIpmiBootGuid,
                            VAR_FLAG,
                            (NvramOptionSize + BOOT_OPTION_NUMBER_SIZE),
                            MatchedBootOptionDetails );
            
            if (EFI_ERROR (Status)) {
                DEBUG ((DEBUG_ERROR, "IPMI Boot: IpmiMatchBootOptionData SetVariable fails.\n"));
            }
            gBS->FreePool (MatchedBootOptionDetails);
        }
        // Freeing the memory allocated in GetVariable2().
        gBS->FreePool (Buffer);
    } // if (Buffer != NULL)

#ifdef CSM_SUPPORT
        SaveLegacyDeviceOrderData ();
#endif
}

/** @internal
    This function checks for the given device path type and sub type
    in the device path returns TRUE if present and FALSE if not.

    @param[in] Dp           The device path to be checked.
    @param[in] DpType       Type to be checked in Device path nodes.
    @param[in] DpSubType    SubType to be checked in Device path nodes.

    @retval TRUE    If device path is present.
    @retval FALSE   If device path is not present.

**/
BOOLEAN
CheckNodeInDevicePath (
  IN  EFI_DEVICE_PATH_PROTOCOL  *Dp,
  IN  UINT16                    DpType,
  IN  UINT16                    DpSubType )
{
    if (Dp) {
        for (;!isEndNode(Dp); Dp = NEXT_NODE(Dp)) {
            if (((Dp->Type == DpType) && (Dp->SubType == DpSubType))) {
                return TRUE;
            }
        }
    }
    return FALSE;
}

/**
    Save the IPMI Set Boot order to NVRAM Variable if IPMI forced boot
    happened.

    @param VOID     Nothing.

    @return VOID    Nothing.

**/
VOID
EFIAPI
SaveIpmiSetBootOrder (
  VOID )
{
    UINT16          *CurrBootOrder = NULL;
    UINTN           CurrBootOrderSize;
    VOID            *Buffer = NULL;
    UINTN           BufferSize;
    EFI_STATUS      Status;

    DEBUG ((DEBUG_INFO, "IPMI Boot: Entered %a\n", __FUNCTION__));

    //  Check for Redfish Features, return if Redfish support available.
    if(REDFISH_BOOT_SUPPORT){
       Status = CheckRedfishSupport ();
       if(!EFI_ERROR (Status))
           return;
    }

    // Checking if Ipmi force boot requested.
    Status = GetVariable2 (
                L"BootOrderBeforeIpmiBoot",
                &gEfiIpmiBootGuid,
                &Buffer,
                &BufferSize );

    if (!EFI_ERROR(Status)) {
        DEBUG ((DEBUG_INFO, "IPMI Boot: Saving IPMI Set BootOrder.\n"));
        // Saving Boot order set by Ipmi.
        Status = GetVariable2 (
                          L"BootOrder",
                          &EfiVariableGuid,
                          (VOID **)&CurrBootOrder,
                          &CurrBootOrderSize );

        if (!EFI_ERROR(Status)) {
            Status = gRT->SetVariable (
                            L"IpmiSetBootOrder",
                            &gEfiIpmiBootGuid,
                            VAR_FLAG,
                            CurrBootOrderSize,
                            CurrBootOrder );
            DEBUG ((DEBUG_INFO, "IPMI Boot: SetVariable for IpmiSetBootOrder variable status: %r.\n", Status));
            gBS->FreePool (CurrBootOrder);
        }
        // Freeing the memory allocated in GetVariableAndSize().
        gBS->FreePool (Buffer);
    }
    DEBUG ((DEBUG_INFO, "IPMI Boot: Exiting SaveIPMISetBootOrder.\n"));
}

#if CSM_SUPPORT
/** @internal
    Check if Ipmi forced device priority for legacy differ
    from Current device priority for legacy.

    @param VOID     Nothing.

    @retval TRUE    If Device priority is changed by User.
    @retval FALSE   If Device priority is not Changed by User.

**/
BOOLEAN
CheckDevicePriorityChange (
  VOID )
{
    VOID                    *Buffer1 = NULL;
    VOID                    *Buffer2 = NULL;
    LEGACY_DEVICE_ORDER     *DevOrder = NULL;
    LEGACY_DEVICE_ORDER     *OldDevOrder = NULL;
    UINT16*                 OldDev = NULL;
    UINT16*                 NewDev = NULL;
    UINTN                   Length = 0;
    UINTN                   BufferSize = 0;
    UINTN                   CurrSize = 0;
    EFI_STATUS              Status;

    /* Getting Current Legacy device order. If user change Legacy Priorities
       it will be reflected.*/
    Status = GetVariable2 (
                      L"LegacyDevOrder",
                      &LegacyDevOrderGuid,
                      &Buffer1,
                      &BufferSize );
    if (!EFI_ERROR(Status)) {
        // Getting Old Legacy device order.
        Status = GetVariable2 (
                       L"OldLegacyDevOrder",
                       &LegacyDevOrderGuid,
                       &Buffer2,
                       &BufferSize );

        if (!EFI_ERROR(Status)) {
            /* Checking Current Legacy Device order is same as Old Legacy device order.
               If user changes BBS priorities then it will not be same.*/
            if((Buffer1 != NULL) && (Buffer2 != NULL)) {
                DevOrder = (LEGACY_DEVICE_ORDER *)Buffer1;
                OldDevOrder = (LEGACY_DEVICE_ORDER *)Buffer2;
                do {
                    /* Getting Length of device data.
                       Each device denoted by UINT16 type.
                       Length = sizeof(Length) + sizeof(Device1) + ... + sizeof(DeviceN).
                       For two device, Length = 6. (2 + 2 + 2)*/
                    Length = DevOrder->Length - 2;
        
                    // Getting device number pointer.
                    OldDev = OldDevOrder->Device;
                    NewDev = DevOrder->Device;
        
                    /* Checking each device number using Length field
                       Each two bytes denotes device number.*/
                    while (Length) {
                        // Checking Current device number is same as Old device number.
                         if( *NewDev != *OldDev) {
                            return TRUE;
                        }
                        Length = Length - 2;
                        NewDev++; // Moving to next device number.
                        OldDev++;
                    }
                    // Getting completed Legacy device type data size.
                    CurrSize = CurrSize + ((UINT8*)NewDev - (UINT8*)DevOrder);
        
                    // Moving to next Legacy device type.
                    DevOrder = (LEGACY_DEVICE_ORDER *)NewDev;
                    OldDevOrder = (LEGACY_DEVICE_ORDER *)OldDev;
                } while(CurrSize < BufferSize);
                // Freeing the memory allocated in GetVariable2().
                gBS->FreePool (Buffer1);
            }
            gBS->FreePool (Buffer2);
        }
    }
    return FALSE;
}
#endif //#if CSM_SUPPORT

/**
    Clear the IPMI Boot NVRAM Variables if IPMI forced boot order or device
    differ from Current boot order or device.

    @param VOID     Nothing.

    @return VOID    Nothing.

**/
VOID
EFIAPI
ClearIpmiBootVariable (
  VOID )
{
    EFI_STATUS      Status;
    UINT16          *IpmiBootOrder = NULL;
    UINTN           IpmiBootOrderSize;
    UINT16          *CurrBootOrder = NULL;
    UINTN           CurrBootOrderSize;
    BOOLEAN         BootOrderChange = FALSE;
    UINTN           Index = 0;
    VOID            *Buffer = NULL;
    UINTN           BufferSize;

    DEBUG ((DEBUG_INFO, "IPMI Boot: Entered %a\n", __FUNCTION__));

    //  Check for Redfish Features, return if Redfish support available.
    if(REDFISH_BOOT_SUPPORT){
       Status = CheckRedfishSupport ();
       if(!EFI_ERROR (Status))
           return;
    }

    // Getting the boot order set by IPMI if previous boot is IPMI boot.
    Status = GetVariable2 (
                       L"IpmiSetBootOrder",
                       &gEfiIpmiBootGuid,
                       (VOID **)&IpmiBootOrder,
                       &IpmiBootOrderSize );
    if (!EFI_ERROR(Status)) {
        // Clear the "IpmiSetBootOrder" NVRAM Variable.
        Status = gRT->SetVariable (
                        L"IpmiSetBootOrder",
                        &gEfiIpmiBootGuid,
                        VAR_FLAG,
                        0,
                        NULL );
    } else {
        DEBUG ((DEBUG_ERROR, "IPMI Boot: Exiting as IPMI Set Boot Order not available.\n"));
        return;
    }

    // Getting Current boot order.
    Status = GetVariable2 (
                       L"BootOrder",
                       &EfiVariableGuid,
                       (VOID **)&CurrBootOrder,
                       &CurrBootOrderSize );
    if (!EFI_ERROR(Status)) {
        // Check if IPMI Set boot order different from Current boot order.
        while (Index < (CurrBootOrderSize / sizeof (UINT16))) {
            if (IpmiBootOrder[Index] != CurrBootOrder[Index]) {
                BootOrderChange = TRUE;
                DEBUG ((DEBUG_INFO, "IPMI Boot: Boot Order Changed by user.\n"));
            }
            Index = Index + 1;
        }
        gBS->FreePool (CurrBootOrder);
    }
    gBS->FreePool (IpmiBootOrder);  // Free the memory allocated for IpmiBootOrder.
#if CSM_SUPPORT
    if (!BootOrderChange) {
        if (CheckDevicePriorityChange ()) {
            BootOrderChange = TRUE;
            DEBUG ((DEBUG_INFO, "IPMI Boot: Legacy device priority Changed by user.\n"));
        }
    }
#endif //#if CSM_SUPPORT
    if (BootOrderChange) {
        // Clear "IpmiMatchBootOptionData" NVRAM Variable if the variable exists.
        Status = GetVariable2 (
                    L"IpmiMatchBootOptionData",
                    &gEfiIpmiBootGuid,
                    &Buffer,
                    &BufferSize );

        if (!EFI_ERROR(Status)) {
            Status = gRT->SetVariable (
                            L"IpmiMatchBootOptionData",
                            &gEfiIpmiBootGuid,
                            VAR_FLAG,
                            0,
                            NULL );
            gBS->FreePool (Buffer);
        }

        // Clear "IpmiBootFlags" NVRAM Variable if the variable exists.
        Status = GetVariable2 (
                          L"IpmiBootFlags",
                          &gEfiIpmiBootGuid,
                          &Buffer,
                          &BufferSize );

        if (!EFI_ERROR(Status)) {
            Status = gRT->SetVariable (
                            L"IpmiBootFlags",
                            &gEfiIpmiBootGuid,
                            VAR_FLAG,
                            0,
                            NULL );
            gBS->FreePool (Buffer);
        }   

        // Clear the "BootOrderBeforeIpmiBoot" NVRAM Variable if the variable exists.
        Status = GetVariable2 (
                         L"BootOrderBeforeIpmiBoot",
                         &gEfiIpmiBootGuid,
                         &Buffer,
                         &BufferSize);

        if (!EFI_ERROR(Status)) {
            Status = gRT->SetVariable (
                    L"BootOrderBeforeIpmiBoot",
                    &gEfiIpmiBootGuid,
                    VAR_FLAG,
                    0,
                    NULL );
            gBS->FreePool (Buffer);
        }
    }
    DEBUG ((DEBUG_INFO, "IPMI Boot: Exiting %a\n", __FUNCTION__));
}

/**
    Restore the Boot Order if last boot was an Ipmi forced boot.

    @param VOID     Nothing.

    @return VOID    Nothing.

**/
VOID
EFIAPI
RestoreBootorder (
  VOID )
{
    UINT16      *BootOrder;
    UINTN       BootOrderSize;
    UINTN       BootOptionSize;
    CHAR16      TempBootStr[9];
    VOID        *Buffer;
    EFI_STATUS  Status;
#ifdef CSM_SUPPORT
    UINT32      Attributes = 0;
#endif

    DEBUG ((DEBUG_INFO, "IPMI Boot: Entered %a\n", __FUNCTION__));

    //  Check for Redfish Features, return if Redfish support available.
    if(REDFISH_BOOT_SUPPORT){
       Status = CheckRedfishSupport ();
       if(!EFI_ERROR (Status))
           return;
    }

    /* Restoring the matched boot option details into the respective
       boot option.*/
    Status = GetVariable2 (
                     L"IpmiMatchBootOptionData",
                     &gEfiIpmiBootGuid,
                     &Buffer,
                     &BootOptionSize );

    if (!EFI_ERROR(Status)) {
        DEBUG ((DEBUG_INFO, "IPMI Boot: IpmiMatchBootOptionData Available.\n"));
        Swprintf (
            TempBootStr,
            L"Boot%04X",
            *((UINT16 *)Buffer) );
        
        Status = gRT->SetVariable (
                        TempBootStr,
                        &EfiVariableGuid,
                        BOOT_VARIABLE_ATTRIBUTES,
                        (BootOptionSize - BOOT_OPTION_NUMBER_SIZE),
                        ((UINT8 *)Buffer + BOOT_OPTION_NUMBER_SIZE) );
        
        // Clear the "IpmiMatchBootOptionData" NVRAM Variable.
        if (!EFI_ERROR(Status)) {
            Status = gRT->SetVariable (
                            L"IpmiMatchBootOptionData",
                            &gEfiIpmiBootGuid,
                            VAR_FLAG,
                            0,
                            NULL );
            if (!EFI_ERROR(Status)) {
                DEBUG ((DEBUG_INFO, "IPMI Boot: Clearing IpmiMatchBootOptionData.\n"));
            }
        }
        // Freeing the memory allocated in GetVariable2().
        gBS->FreePool (Buffer);
        Buffer = NULL;
    }

#ifdef CSM_SUPPORT
    BootOptionSize = 0;

    // Get the attributes for LegacyDevOrder variable.
    Status = gRT->GetVariable (
                    L"LegacyDevOrder",
                    &LegacyDevOrderGuid,
                    &Attributes,
                    &BootOptionSize,
                    Buffer);
                    
    if (!Attributes && (Status == EFI_BUFFER_TOO_SMALL)) {
        Buffer = AllocateZeroPool (BootOptionSize);
        Status = gRT->GetVariable (
                         L"LegacyDevOrder",
                         &LegacyDevOrderGuid,
                         &Attributes,
                         &BootOptionSize,
                         Buffer );
    }
    DEBUG ((DEBUG_INFO, "IPMI Boot: Get LegacyDevOrder variable Status:%r Attributes:%X Buffer:%X.\n", Status, Attributes, Buffer));
    
    if (Attributes) {
        if (Buffer != NULL) {
            gBS->FreePool (Buffer); // Freeing the memory allocated for Buffer.
        }
        Status = GetVariable2 (
                         L"IpmiLegacyDevOrder",
                         &gEfiIpmiBootGuid,
                         &Buffer,
                         &BootOptionSize );
        if (!EFI_ERROR(Status)) {
            // Restoring the Legacy device order details into the "LegacyDevOrder" variable.
            Status = gRT->SetVariable (
                            L"LegacyDevOrder",
                            &LegacyDevOrderGuid,
                            Attributes,
                            BootOptionSize,
                            ((UINT8 *)Buffer) );
            
            // Clear the "IpmiLegacyDevOrder" NVRAM Variable.
            if (!EFI_ERROR(Status)) {
                Status = gRT->SetVariable (
                                L"IpmiLegacyDevOrder",
                                &gEfiIpmiBootGuid,
                                VAR_FLAG,
                                0,
                                NULL );
                
                if (!EFI_ERROR(Status)) {
                    DEBUG ((DEBUG_INFO, "IPMI Boot: Restored LegacyDevOrder and Cleared IpmiLegacyDevOrder.\n"));
                }
            }
            // Freeing the memory allocated in GetVariableAndSize().
            gBS->FreePool (Buffer);
        } // if (Buffer != NULL)
        Attributes = 0;
    } // if (Attributes != 0)

    BootOptionSize = 0;
    Buffer = NULL;
    // Get the attributes for OldLegacyDevOrder variable.
    Status = gRT->GetVariable (
                    L"OldLegacyDevOrder",
                    &LegacyDevOrderGuid,
                    &Attributes,
                    &BootOptionSize,
                    Buffer);

    if (!Attributes && (Status == EFI_BUFFER_TOO_SMALL)) {
        Buffer = AllocateZeroPool (BootOptionSize);
        Status = gRT->GetVariable (
                        L"OldLegacyDevOrder",
                        &LegacyDevOrderGuid,
                        &Attributes,
                        &BootOptionSize,
                        Buffer );
    }

    DEBUG ((DEBUG_INFO, "IPMI Boot: Get OldLegacyDevOrder variable Attributes:%X Buffer:%X.\n", Attributes, Buffer));
    if (Attributes) {
        if (Buffer != NULL) {
            gBS->FreePool (Buffer); // Freeing the memory allocated for Buffer.
        }
        DEBUG ((DEBUG_INFO, "IPMI Boot: Get OldLegacyDevOrder variable Attributes:%X Buffer:%X.\n", Attributes, Buffer));
        /* Restoring the Old Legacy device order details into the
           "OldLegacyDevOrder" variable.*/
        Status = GetVariable2 (
                         L"IpmiOldLegacyDevOrder",
                         &gEfiIpmiBootGuid,
                         &Buffer,
                         &BootOptionSize );

        if (!EFI_ERROR(Status)) {
            Status = gRT->SetVariable (
                            L"OldLegacyDevOrder",
                            &LegacyDevOrderGuid,
                            Attributes,
                            BootOptionSize,
                            ((UINT8 *)Buffer) );
            
            // Clear the "IpmiOldLegacyDevOrder" NVRAM Variable.
            if (!EFI_ERROR(Status)) {
                Status = gRT->SetVariable (
                                L"IpmiOldLegacyDevOrder",
                                &gEfiIpmiBootGuid,
                                VAR_FLAG,
                                0,
                                NULL );
                
                if (!EFI_ERROR(Status)) {
                    DEBUG ((DEBUG_INFO, "IPMI Boot: Restored OldLegacyDevOrder and Cleared IpmiOldLegacyDevOrder.\n"));
                }
            }
            // Freeing the memory allocated in GetVariableAndSize().
            gBS->FreePool (Buffer);
        } // if (!EFI_ERROR(Status)
    } // if (Attributes != 0)
#endif // #ifdef (CSM_SUPPORT)
    
    // Get the previous boot order if the last boot was an Ipmi forced boot.
    Status = GetVariable2 (
                    L"BootOrderBeforeIpmiBoot",
                    &gEfiIpmiBootGuid,
                    (VOID **)&BootOrder,
                    &BootOrderSize );

    if (!EFI_ERROR(Status)) {
        DEBUG ((DEBUG_INFO, "IPMI Boot: Restoring Original boot order.\n"));

        /* Save the restored boot order back to NVRAM now too. Any
           modifications to the boot order will get saved again at the end of BDS.*/
        Status = gRT->SetVariable (
                        L"BootOrder",
                        &EfiVariableGuid,
                        BOOT_VARIABLE_ATTRIBUTES,
                        BootOrderSize,
                        BootOrder );
        ASSERT_EFI_ERROR(Status);

        // Clear the "BootOrderBeforeIpmiBoot" NVRAM Variable
        Status = gRT->SetVariable (
                        L"BootOrderBeforeIpmiBoot",
                        &gEfiIpmiBootGuid,
                        VAR_FLAG,
                        0,
                        NULL );
        // Freeing the memory allocated in GetVariable2().
        gBS->FreePool (BootOrder);
    }

    DEBUG ((DEBUG_INFO, "IPMI Boot: Exiting %a\n", __FUNCTION__));
}

/** @internal
    This function update the IpmiBoot global variables.

    @param[in] BootFlags    IpmiBoot flags data.

    @retval VOID    Nothing.

**/
VOID
UpdateIpmiBootGlobalVariables (
  IN AMI_GET_BOOT_OPTIONS_BOOT_FLAG_RESPONSE    BootFlags )
{
    gIpmiForceBootDevice    = BootFlags.Param5.BootDeviceSelector;
    gPersistentBoot         = BootFlags.Param5.Persistent;
#if defined (MDE_CPU_AARCH64)
    gUefiBoot = TRUE;
#else
    gUefiBoot = BootFlags.Param5.BiosBootType;
#endif
    gDeviceInstanceSelector = BootFlags.Param5.DeviceInstanceSelector;

    DEBUG ((DEBUG_INFO, "IPMI Boot: gIpmiForceBootDevice = %d gPersistentBoot = %d gUefiBoot = %d gDeviceInstanceSelector = %d\n", \
                         gIpmiForceBootDevice, gPersistentBoot, gUefiBoot, gDeviceInstanceSelector));
}

/** @internal
    Clear boot flags data after ipmi boot.

    @param VOID     Nothing.

    @return EFI_STATUS  Status returned by function calls used.

**/
EFI_STATUS
IpmiClearBootFlags (
  VOID )
{
    EFI_STATUS                              Status;
    AMI_SET_BOOT_OPTIONS_BOOT_FLAG_REQUEST  BootFlagRequest;
    UINT8                                   CompletionCode;
    UINT8                                   ResponseDataSize;

    // Check if Boot Flag Valid bit is set.
    Status = AmiIpmiGetSysBootOptionsWaitSetInProgressClear (gIpmiTransport);
    if (EFI_ERROR (Status)) {
        return Status;
    }

    ZeroMem (
       &BootFlagRequest,
       sizeof (AMI_SET_BOOT_OPTIONS_BOOT_FLAG_REQUEST) );
    BootFlagRequest.Request.ParameterSelector = AmiBootOptionParamBootFlags;
    ResponseDataSize = sizeof (CompletionCode);
    Status = gIpmiTransport->SendIpmiCommand (
                                gIpmiTransport,
                                IPMI_NETFN_CHASSIS,
                                AMI_BMC_LUN,
                                IPMI_CHASSIS_SET_SYSTEM_BOOT_OPTIONS,
                                (UINT8 *)&BootFlagRequest,
                                sizeof (BootFlagRequest),
                                &CompletionCode,
                                &ResponseDataSize );
    return Status;
}

/** @internal
    Set the boot acknowledgment info to BMC using Set boot options command.

    @param[in] BootInitAckData  Boot initiated acknowledge data.

    @return EFI_STATUS  Status returned by the function calls used.

**/
EFI_STATUS
IpmiSetBootInfoAck (
  IN UINT8  BootInitAckData )
{
    EFI_STATUS                                  Status;
    UINT8                                       ResponseDataSize;
    AMI_SET_BOOT_OPTIONS_BOOT_INFO_ACK_REQUEST  SetRequest;
    UINT8                                       CompletionCode;

    // Check if set in progress bit is clear.
    Status = AmiIpmiGetSysBootOptionsWaitSetInProgressClear (gIpmiTransport);
    if (EFI_ERROR (Status)) {
        return Status;
    }

    SetRequest.Request.ParameterSelector            = AmiBootOptionParamBootInfoAck;
    SetRequest.Request.MarkParameterInvalid         = 0;
    SetRequest.Param4.WriteMask                     = BootInitAckData;
    SetRequest.Param4.BootInitiatorAcknowledgeData  = 0;
    ResponseDataSize                                = sizeof (CompletionCode);
    
    Status = gIpmiTransport->SendIpmiCommand (
                                gIpmiTransport,
                                IPMI_NETFN_CHASSIS,
                                AMI_BMC_LUN,
                                IPMI_CHASSIS_SET_SYSTEM_BOOT_OPTIONS,
                                (UINT8 *)&SetRequest,
                                sizeof (SetRequest),
                                &CompletionCode,
                                &ResponseDataSize );
    return Status;
}

/**
    Sets the Boot Option Priority based on the Ipmi Boot Flags.

    @param VOID     Nothing.

    @return VOID    Nothing.

**/
VOID
EFIAPI
SetIpmiBootOptionPriorities (
  VOID )
{
    EFI_STATUS                                      Status;
    AMI_GET_BOOT_OPTIONS_BOOT_INFO_ACK_RESPONSE     BootInfoAckResponse;
    AMI_GET_BOOT_OPTIONS_BOOT_FLAG_RESPONSE         BootFlags;
    UINTN                                           BootFlagsDataSize;

    DEBUG ((DEBUG_INFO, "\n IPMI Boot: Entered %a\n", __FUNCTION__));

    // Locate the Ipmi Transport Protocol.
    Status = gBS->LocateProtocol (
                    &gEfiDxeIpmiTransportProtocolGuid,
                    NULL,
                    (VOID **)&gIpmiTransport );
    
    if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "%a: Error in locating the Ipmi Transport Protocol. Status: %r\n", __FUNCTION__, Status));
        return;
    }

    //  Check for Redfish Features, return if Redfish support available.
    if(REDFISH_BOOT_SUPPORT){
       Status = CheckRedfishSupport ();
       if(!EFI_ERROR (Status))
           return;
    }

    // Read the boot info acknowledge bytes from BMC.
    Status = AmiIpmiGetBootInfoAcknowledgeData (
                gIpmiTransport,
                &BootInfoAckResponse );
    
    if (EFI_ERROR(Status)) {
        return;
    }
    /* Support IPMI boot only if BootInitiatorAcknowledgeData is zero
       or BiosOrPostBit is set in BootInitiatorAcknowledgeData.*/
    if ((BootInfoAckResponse.Param4.BootInitiatorAcknowledgeData == 0) || \
        (BootInfoAckResponse.Param4.BootInitiatorAcknowledgeData & BIT0)) {

        gIpmiForceBootDevice = 0;
        // Read the boot flag bytes from BMC.
        Status = AmiIpmiGetBootFlags (
                    gIpmiTransport,
                    &BootFlags );
        if (EFI_ERROR(Status)) {
            return;
        }
        if (BootFlags.Param5.BootFlagValid) {
            // Update the IpmiBoot global variables.
            UpdateIpmiBootGlobalVariables (BootFlags);

            // If persistent boot, save the Boot flags into NVRAM to use it in further boots.
            if (gPersistentBoot) {
                Status = gRT->SetVariable (
                                L"IpmiBootFlags",
                                &gEfiIpmiBootGuid,
                                VAR_FLAG,
                                sizeof(BootFlags),
                                &BootFlags );
            }

            // Write to Boot Info Acknowledge "BIOS/POST has handled boot info".
            Status = IpmiSetBootInfoAck (AMI_BOOT_INFO_HANDLED_BY_BIOS);

            // Clear the BMC BootFlags.
            if (!EFI_ERROR(Status)) {
                Status = IpmiClearBootFlags ();
            }

            if (EFI_ERROR(Status) || (gIpmiForceBootDevice == AmiIpmiNoOverride)) {
                return;
            }
        } else {
            // Get the IpmiBootFlags NVRAM Variable stored in previous persistent boots.
            BootFlagsDataSize = sizeof(BootFlags);
            Status = gRT->GetVariable (
                            L"IpmiBootFlags",
                            &gEfiIpmiBootGuid,
                            NULL,
                            &BootFlagsDataSize,
                            &BootFlags );

            if (!EFI_ERROR(Status)) {
                // Update the IpmiBoot global variables.
                UpdateIpmiBootGlobalVariables (BootFlags);
            }

            if (EFI_ERROR(Status) || (gIpmiForceBootDevice == AmiIpmiNoOverride)) {
                return;
            }
        }
    } else {
        /* Clear the stored NVRAM BootFlags data, if any fields other than BIOS/POST
           bit is selected in BootInfoAcknowledge data.*/
        BootFlagsDataSize = sizeof(BootFlags);
        Status = gRT->GetVariable (
                        L"IpmiBootFlags",
                        &gEfiIpmiBootGuid,
                        NULL,
                        &BootFlagsDataSize,
                        &BootFlags );

        if (!EFI_ERROR(Status)) {
            Status = gRT->SetVariable (
                            L"IpmiBootFlags",
                            &gEfiIpmiBootGuid,
                            VAR_FLAG,
                            0,
                            &BootFlags );
        }
        return;
    }

    DEBUG ((DEBUG_INFO, "\n IPMI Boot: Setting requested Boot option to High priority\n"));

    // Currently PXE, HDD, CdRom and Floppy are supported for Ipmi boot.
    switch (gIpmiForceBootDevice) {

        case AmiIpmiForcePxe:
            Status = SetBootOptionToHighestPriority (
                        BBS_BEV_DEVICE,
                        MESSAGING_DEVICE_PATH,
                        MSG_MAC_ADDR_DP );
            
            if ((Status == EFI_NOT_FOUND) && (!gUefiBoot)) {
                Status = SetBootOptionToHighestPriority (
                            BBS_EMBED_NETWORK,
                            0,
                            0 );
            }
            break;

        case AmiIpmiForceBootHardDrive:
            Status = SetBootOptionToHighestPriority (
                        BBS_HARDDISK,
                        MEDIA_DEVICE_PATH,
                        MEDIA_HARDDRIVE_DP );
            break;

        case AmiIpmiForceBootHardDriveSafeMode:
            Status = SetBootOptionToHighestPriority (
                        BBS_HARDDISK,
                        MEDIA_DEVICE_PATH,
                        MEDIA_HARDDRIVE_DP);
            break;

        case AmiIpmiForceBootDiagnosticPartition:
            Status = SetBootOptionToHighestPriority (
                        BBS_HARDDISK,
                        MEDIA_DEVICE_PATH,
                        MEDIA_HARDDRIVE_DP);
            break;
        case AmiIpmiForceBootCdDvd:
            Status = SetBootOptionToHighestPriority (
                        BBS_CDROM,
                        MEDIA_DEVICE_PATH,
                        MEDIA_CDROM_DP );
            break;

        case AmiIpmiForceBootBiosSetup:
            {
                UINT32      BootFlow        = BOOT_FLOW_CONDITION_FIRST_BOOT;
                EFI_GUID    BootFlowGuid    = BOOT_FLOW_VARIABLE_GUID;
                Status = gRT->SetVariable (
                                L"BootFlow",
                                &BootFlowGuid,
                                EFI_VARIABLE_BOOTSERVICE_ACCESS,
                                sizeof(BootFlow),
                                &BootFlow );
            }
            break;

        case AmiIpmiForceBootFloppy:
            Status = SetBootOptionToHighestPriority (
                        BBS_FLOPPY,
                        ACPI_DEVICE_PATH,
                        ACPI_DP );
            break;

        case AmiIpmiForceBootRemoteFloppy:
        case AmiIpmiForceBootRemoteCdDvd:
        case AmiIpmiForceBootRemoteHardDrive:
            Status = RemoteDeviceBoot (gIpmiForceBootDevice);
            break;

/*
        case AmiIpmiForceBootPrimaryRemoteMedia:
            Status = SetBootOptionToHighestPriority (
                        BBS_HARDDISK,
                        0,
                        0,
                        TRUE );
            break;
*/
        default:
            Status = EFI_NOT_FOUND;
    }

    DEBUG ((DEBUG_INFO, " IPMI Boot: %a Exit. Status: %r\n", __FUNCTION__, Status));
}

/** @internal
    Save the current Boot Order for further use in the next boot.

    @param VOID     Nothing.

    @return VOID    Nothing.

**/
VOID
SaveCurrentBootorder (
  VOID )
{
    EFI_STATUS      Status = EFI_SUCCESS;
    UINTN           BootOptionCount = 0x00;
    UINTN           Index;
    UINT16          *BootOrderBuffer;
    BOOT_OPTION     *Option;
    DLINK           *Link;

    BootOrderBuffer = AllocateZeroPool (MAXIMUM_BOOT_OPTION_NUMBER * sizeof (Option->BootOptionNumber));
    if (BootOrderBuffer == NULL) {
        DEBUG((DEBUG_ERROR, "SaveCurrentBootorder: AllocateZeroPool Fails!!! \n"));
        return;
    }

    // Sorting BootOptionList based on priority - First boot option will be high priority device.
    SortList (
        BootOptionList,
        ComparePriorityThenBootOptionNumber );

    FOR_EACH_BOOT_OPTION (BootOptionList, Link, Option) {
        // Check for valid BootOptionNumber.
        if(Option->BootOptionNumber != INVALID_BOOT_OPTION_NUMBER) {
            for(Index = 0; Index < BootOptionCount; Index++) {
                // Check BootOptionNumber is already available in BootOrderBuffer.
                if (*(BootOrderBuffer + Index) == Option->BootOptionNumber) {
                    break;
                }
            }
            if (Index < BootOptionCount) {
                /* Current BootOptionNumber is already available in BootOrderBuffer,
                   goto next BootOptionNumber.*/
                continue;
            }
            // Store the BootOptionNumber in BootOrderBuffer.
            *(BootOrderBuffer + BootOptionCount) = Option->BootOptionNumber;
            BootOptionCount++;
        } // if(Option->BootOptionNumber != INVALID_BOOT_OPTION_NUMBER)
    } // FOR_EACH_BOOT_OPTION(BootOptionList,Link,Option)

    // Save current Boot Order for use on the next boot.
    Status = gRT->SetVariable (
                    L"BootOrderBeforeIpmiBoot",
                    &gEfiIpmiBootGuid,
                    VAR_FLAG,
                    ( sizeof(Option->BootOptionNumber)*BootOptionCount ),
                    BootOrderBuffer );
    
    if (EFI_ERROR(Status)) {
        DEBUG((DEBUG_ERROR, "SaveCurrentBootorder: BootOrderBeforeIpmiBoot SetVariable Fails!!! \n"));
    }
    gBS->FreePool (BootOrderBuffer);
}

/** @internal
    Set the highest priority to the requested USB compatible device Boot
    option.

    @param[in] RequestedUsbDeviceInstance   USB device instance to be
                                            selected.
    @param[in] BbsDevType                   BBS device Type of the boot
                                            option to be selected.
    @param[in] DpType                       Device path type of the boot
                                            option to be selected.
    @param[in] DpSubType                    Device path sub type of boot
                                            option.

    @return BOOT_OPTION*    Returns the address of requested USB boot option
                            address. If USB boot opiton is not exists,
                            returns NULL.

**/
BOOT_OPTION *
SetUsbDeviceBootOptionToHighestPriority (
  IN UINTN      RequestedUsbDeviceInstance,
  IN UINT16     BbsDevType,
  IN UINT16     DpType,
  IN UINT16     DpSubType )
{
    DLINK                       *Link;
    BOOT_OPTION                 *Option;
    EFI_DEVICE_PATH_PROTOCOL    *OptionDp;
    UINTN                       UsbDeviceCount = 0;

    // Proceed only if Boot Option list is not empty.
    if (!DListEmpty (BootOptionList)) {

        FOR_EACH_BOOT_OPTION (BootOptionList, Link, Option) {
            OptionDp = Option->FilePathList;
            // Skip the group headers.
            if (Option->GroupHeader) {
                continue;
            }

            /* Check for BIOS Boot type as UEFI boot and
               Check for the UEFI boot option (Not Legacy boot option).*/
            if (gUefiBoot && (Option->BbsEntry == NULL) && \
                    (Option->BbsIndex == INVALID_BBS_INDEX)) {
                /* Check for floppy controller first as it has different 
                   device path to verify. It has ACPI device path.*/
                if ((DpType == ACPI_DEVICE_PATH) && (DpSubType == ACPI_DP)) {
                    if (IS_ACPI_DEVICE_PATH ((ACPI_HID_DEVICE_PATH*)OptionDp)) {
                        continue;
                    } else {
                        // Check for USB floppy.
                        if (CheckNodeInDevicePath (
                                OptionDp,
                                MESSAGING_DEVICE_PATH,
                                MSG_USB_DP )) {
                            UsbDeviceCount = UsbDeviceCount + 1;
                        } else {
                            continue;
                        }
                    }
                } else {
                    // Check for other UEFI devices.
                    if (CheckNodeInDevicePath (
                            OptionDp,
                            DpType,
                            DpSubType )) {
                        /* Check whether the device is USB compatible device.
                           If it is a USB compatible device boot option,
                           then skip this boot option and move to next boot option.*/
                        if (CheckNodeInDevicePath (
                                OptionDp,
                                MESSAGING_DEVICE_PATH,
                                MSG_USB_DP )) {
                            UsbDeviceCount = UsbDeviceCount + 1;
                        } else {
                            continue;
                        }
                    } else {
                        continue;
                    }
                } // UEFI check ends.
            } else {
#if ((defined (BBS_USB_DEVICE_TYPE_SUPPORT) & (!BBS_USB_DEVICE_TYPE_SUPPORT)) && (defined (CSM_SUPPORT) & CSM_SUPPORT))
                if (!gUefiBoot && IS_BBS_DEVICE_PATH (\
                    (BBS_BBS_DEVICE_PATH*) OptionDp, BbsDevType)) {
                    /* Legacy Devices. Check class code to find out whether USB
                       compatible device or SATA compatible device.*/
                    if (IS_USB_DEVICE_CLASS_CODE (Option)) {
                        UsbDeviceCount = UsbDeviceCount + 1;
                    } else {
                        continue;
                    }
                } else {
                    continue;
                }
#elif ((defined (BBS_USB_DEVICE_TYPE_SUPPORT) & (BBS_USB_DEVICE_TYPE_SUPPORT)) && (defined (CSM_SUPPORT) & CSM_SUPPORT))
                 
                if (!gUefiBoot && IS_BBS_DEVICE_PATH(\
                    (BBS_BBS_DEVICE_PATH*) OptionDp, BbsDevType)) {
                    continue;
                } else {
                    switch(BbsDevType) {
                        case BBS_FLOPPY:
                            if (!gUefiBoot && IS_BBS_DEVICE_PATH (\
                                (BBS_BBS_DEVICE_PATH*) OptionDp, BBS_USB) && \
                                IS_USB_COMPATIBLE_DEVICE (Option, BAID_TYPE_RMD_FDD)) {
                                // Legacy Boot Option - USB floppy device path.
                                UsbDeviceCount = UsbDeviceCount + 1;
                            } else {
                                continue;
                            }
                            break;

                        case BBS_CDROM:
                            if (!gUefiBoot && IS_BBS_DEVICE_PATH (\
                                (BBS_BBS_DEVICE_PATH*) OptionDp, BBS_USB) && \
                                IS_USB_COMPATIBLE_DEVICE (Option, BAID_TYPE_CDROM)) {
                                // Legacy Boot Option - USB CDROM device path
                                UsbDeviceCount = UsbDeviceCount + 1;
                            } else {
                                continue;
                            }
                            break;

                        case BBS_HARDDISK:
                            if (!gUefiBoot && IS_BBS_DEVICE_PATH (\
                                (BBS_BBS_DEVICE_PATH*) OptionDp, BBS_USB) && \
                                IS_USB_COMPATIBLE_DEVICE (Option, BAID_TYPE_RMD_HDD)) {
                                // Legacy Boot Option - USB HDD device path
                                UsbDeviceCount = UsbDeviceCount + 1;
                            } else {
                                continue;
                            }
                            break;

                        default:
                            continue;
                    } 
                }
#endif
            }
            if (UsbDeviceCount == RequestedUsbDeviceInstance) {
                return Option;
            } else {
                continue;
            }
        }
    }
    return NULL;
}

/** @internal
    Set the highest priority to the requested SATA compatible device boot
    option.

    @param[in] BbsDevType   BBS Device Type of the boot option.
    @param[in] DpType       Device path type of the boot option.
    @param[in] DpSubType    Device path sub type of boot option.

    @retval EFI_SUCCESS     Requested boot option is found and set as high
                            priority.
    @retval EFI_NOT_FOUND   Proper boot option is not found to set priority.

**/
EFI_STATUS 
SetBootOptionToHighestPriority (
  IN UINT16     BbsDevType,
  IN UINT16     DpType,
  IN UINT16     DpSubType )
{
    DLINK                       *Link;
    BOOT_OPTION                 *Option;
    BOOT_OPTION                 *TempOption = NULL;
    BOOT_OPTION                 *UsbTempOption = NULL;
    EFI_DEVICE_PATH_PROTOCOL    *OptionDp;
    BOOLEAN                     HardDiskValidationResult = FALSE;
    BOOLEAN                     IsUsbDeviceFound = FALSE;
    BOOLEAN                     IsNonUsbDeviceFound = FALSE;
    UINT16                      BootNumber = 0;
    UINTN                       UsbDeviceCount = 0;
    UINT8                       DeviceInstanceCount = 0xFF;
    UINTN                       RequiredUsbDeviceInstance;
    UINT8                       LastDeviceFound;
    EFI_DEVICE_PATH_PROTOCOL    *DevPath;
    EFI_STATUS                  Status;

    DUMP_BOOT_OPTION_LIST (BootOptionList, "Before Setting IPMI Priorities");

    // Proceed only if Boot Option list is not empty.
    if (!DListEmpty (BootOptionList)) {

        FOR_EACH_BOOT_OPTION (BootOptionList, Link, Option) {
            LastDeviceFound = INVALID_DEVICE;
            OptionDp = Option->FilePathList;
            // Skip the group headers.
            if (Option->GroupHeader) {
                continue;
            }

            /* Check for BIOS Boot type as UEFI boot and
               Check for the UEFI boot option (Not Legacy boot option).*/
            if (gUefiBoot && (Option->BbsEntry == NULL) && \
                    (Option->BbsIndex == INVALID_BBS_INDEX)) {
                /* Check for floppy controller first as it has different
                   device path to verify. It has ACPI device path.*/
                if ((DpType == ACPI_DEVICE_PATH) && (DpSubType == ACPI_DP)) {
                    if (IS_ACPI_DEVICE_PATH ((ACPI_HID_DEVICE_PATH*)OptionDp)) {
                        LastDeviceFound = NON_USB_COMPATIBLE_DEVICE;
                    } else {
                        // Check for USB floppy.
                        if (CheckNodeInDevicePath (
                                OptionDp,
                                MESSAGING_DEVICE_PATH,
                                MSG_USB_DP )) {
                            LastDeviceFound = USB_COMPATIBLE_DEVICE;
                        } else {
                            continue;
                        }
                    }
                } else {
                    // Check for UEFI PXE Boot.
                    if ((DpType == MESSAGING_DEVICE_PATH) && \
                        (DpSubType == MSG_MAC_ADDR_DP)) {
                        if (!CheckNodeInDevicePath (
                                OptionDp,
                                DpType,
                                DpSubType )) {
                            continue;
                        }
                        LastDeviceFound = NON_USB_COMPATIBLE_DEVICE;
                    // Check for ISCSI.
                    } else if (DpType == MESSAGING_DEVICE_PATH && DpSubType == MSG_ISCSI_DP){
                        //Getting the full device path of boot option.
                        if (Option->DeviceHandle != NULL) {
                            Status = gBS->HandleProtocol (Option->DeviceHandle,
                                                          &gEfiDevicePathProtocolGuid,
                                                          (VOID *)&DevPath);
                            if (EFI_ERROR(Status) || (DevPath == NULL))
                                continue;
                        }

                        if ( !CheckNodeInDevicePath (DevPath, DpType, DpSubType)) {
                            continue;
                        }
                        DEBUG ((DEBUG_INFO, "\n Iscsi device found \n"));
                        LastDeviceFound = NON_USB_COMPATIBLE_DEVICE;

                    // Check for other UEFI devices.
                    } else if (CheckNodeInDevicePath (
                                    OptionDp,
                                    DpType,
                                    DpSubType )) {
                        /* Check whether the device is USB compatible device.
                           If it is a USB compatible device boot option,
                           then skip this boot option. Move to next boot option.*/
                        if (CheckNodeInDevicePath (
                                OptionDp,
                                MESSAGING_DEVICE_PATH,
                                MSG_USB_DP )) {
                            LastDeviceFound = USB_COMPATIBLE_DEVICE;
                        } else {
                            LastDeviceFound = NON_USB_COMPATIBLE_DEVICE;
                        }

                    // Check for boot loader file.
                    } else if ((DpType == MEDIA_DEVICE_PATH) && \
                                (DpSubType == MEDIA_HARDDRIVE_DP)) {
                        if (CheckNodeInDevicePath (    // Check if FILEPATH_DEVICE_PATH?
                                OptionDp,
                                MEDIA_DEVICE_PATH,
                                MEDIA_FILEPATH_DP )) {
                            if (CheckNodeInDevicePath (
                                    OptionDp,
                                    DpType,
                                    DpSubType )){
                                /* Check whether the device is USB compatible device.
                                   If it is a USB compatible device boot option,
                                   then skip this boot option. Move to next boot option*/
                                if (CheckNodeInDevicePath (
                                        OptionDp, \
                                        MESSAGING_DEVICE_PATH,
                                        MSG_USB_DP )) {
                                    LastDeviceFound = USB_COMPATIBLE_DEVICE;
                                } else {
                                    LastDeviceFound = NON_USB_COMPATIBLE_DEVICE;
                                }
                            } else {
                                continue;
                            }
                        } else {
                            continue;
                        }
                    } else {
                        continue;
                    }
                } // UEFI check ends.
            } else {
#if ((defined (BBS_USB_DEVICE_TYPE_SUPPORT) & (!BBS_USB_DEVICE_TYPE_SUPPORT)) && (defined (CSM_SUPPORT) & CSM_SUPPORT))
                if (!gUefiBoot && IS_BBS_DEVICE_PATH (\
                    (BBS_BBS_DEVICE_PATH*) OptionDp, BbsDevType)) {
                    /* Legacy Devices. Check class code to find out whether
                       USB compatible device or SATA compatible device.*/
                    if (IS_USB_DEVICE_CLASS_CODE (Option)) {
                        LastDeviceFound = USB_COMPATIBLE_DEVICE;
                    } else {
                        LastDeviceFound = NON_USB_COMPATIBLE_DEVICE;
                    }
                } else {
                    continue;
                }
#elif ((defined (BBS_USB_DEVICE_TYPE_SUPPORT) & BBS_USB_DEVICE_TYPE_SUPPORT) && (defined (CSM_SUPPORT) & CSM_SUPPORT))
                if (!gUefiBoot && IS_BBS_DEVICE_PATH (\
                    (BBS_BBS_DEVICE_PATH*) OptionDp, BbsDevType)) {
                    LastDeviceFound = NON_USB_COMPATIBLE_DEVICE;
                } else {
                    switch(BbsDevType) {
                        case BBS_FLOPPY:
                            if (!gUefiBoot && IS_BBS_DEVICE_PATH (\
                                (BBS_BBS_DEVICE_PATH*) OptionDp, BBS_USB) && \
                                IS_USB_COMPATIBLE_DEVICE (Option, \
                                BAID_TYPE_RMD_FDD)) {
                                LastDeviceFound = USB_COMPATIBLE_DEVICE;
                            } else {
                                continue;
                            }
                            break;

                        case BBS_CDROM:
                            if (!gUefiBoot && IS_BBS_DEVICE_PATH (\
                                (BBS_BBS_DEVICE_PATH*) OptionDp, BBS_USB) && \
                                IS_USB_COMPATIBLE_DEVICE (Option, \
                                BAID_TYPE_CDROM)) {
                                LastDeviceFound = USB_COMPATIBLE_DEVICE;
                            } else {
                                continue;
                            }
                            break;

                        case BBS_HARDDISK:
                            if (!gUefiBoot && IS_BBS_DEVICE_PATH (\
                                (BBS_BBS_DEVICE_PATH*) OptionDp, BBS_USB) && \
                                IS_USB_COMPATIBLE_DEVICE (Option, \
                                BAID_TYPE_RMD_HDD)) {
                                LastDeviceFound = USB_COMPATIBLE_DEVICE;
                            } else {
                                continue;
                            }
                            break;

                        default:
                            continue;

                    } 
                }
#endif
            }

            if ((gIpmiForceBootDevice == AmiIpmiForceBootHardDriveSafeMode) ||
                (gIpmiForceBootDevice == AmiIpmiForceBootDiagnosticPartition)) {

                HardDiskValidationResult = BootToDiagnosticPartitionOrHddRequestSafeMode (
                                                gUefiBoot,
                                                BbsDevType,
                                                DpType,
                                                DpSubType,
                                                Option );
                if (HardDiskValidationResult) {
                    continue;
                }
            }
            if (((!HardDiskValidationResult) &&
                (LastDeviceFound == USB_COMPATIBLE_DEVICE)) ||
                (LastDeviceFound == USB_COMPATIBLE_DEVICE)) {
                /* If it is a USB compatible device then increase
                   UsbDeviceCount and if it is first matching 
                   USB device then take back of this boot option.*/
                UsbDeviceCount = UsbDeviceCount + 1;
                if (!IsUsbDeviceFound) {
                    UsbTempOption = Option;
                    IsUsbDeviceFound = TRUE;
                }
                continue;
            }

            /* Increase the DeviceInstanceCount value by 1, if Non USB
               device found.*/
            DeviceInstanceCount = DeviceInstanceCount + 1;
            if (((gDeviceInstanceSelector &
                INTERNAL_EXTERNAL_DEVICE_INSTANCE_MASK) == 0x00) || \
                ((gDeviceInstanceSelector & \
                DEVICE_INSTANCE_SELECTION_MASK) == DeviceInstanceCount)) {

                /* Control comes here, if gDeviceInstanceSelector is equal to
                   DeviceInstanceCount for all Non USB internal case and
                   for all Non USB external cases.*/
                IsNonUsbDeviceFound = TRUE;
                if (!gUefiBoot) {
                    // Saving the matched boot option details in NVRAM.
                    SaveMatchedBootOptionData (Option->BootOptionNumber);
                }
                // Save the Current boot order.
                SaveCurrentBootorder ();
                // Set the Boot Option as Highest tag and Priority.
                Option->Priority = HIGHEST_BOOT_OPTION_PRIORITY;
                BootNumber = Option->BootOptionNumber;
                break;
            } else {
                /* Control comes here, if gDeviceInstanceSelector is not
                   equal to DeviceInstanceCount for all internal non USB
                   cases.*/
                IsNonUsbDeviceFound = TRUE;
                if (DeviceInstanceCount == 0x00) {
                    TempOption = Option;
                }
                continue;
            }
        } // FOR_EACH_BOOT_OPTION ends
        
        if ((((gDeviceInstanceSelector & DEVICE_INSTANCE_SELECTION_MASK) != \
            DeviceInstanceCount) && \
            (gDeviceInstanceSelector & INTERNAL_EXTERNAL_DEVICE_INSTANCE_MASK) \
            != 0x00) || ((!IsNonUsbDeviceFound) && IsUsbDeviceFound)) {
            /* Control comes here, if the specified device instance is not
               available in Non USB device cases or if not even a single
               Non USB device is found.*/
            if (IsNonUsbDeviceFound) {
                RequiredUsbDeviceInstance = (gDeviceInstanceSelector & \
                    DEVICE_INSTANCE_SELECTION_MASK) - DeviceInstanceCount;
            } else {
                RequiredUsbDeviceInstance = (gDeviceInstanceSelector & \
                                        DEVICE_INSTANCE_SELECTION_MASK) + 1;
            }
            
            /* Call the SetUsbDeviceBootOptionToHighestPriority function
               if it is internal device case and if
               RequiredUsbDeviceInstance is available.*/
            if ((UsbDeviceCount >= RequiredUsbDeviceInstance) &&  \
                ((gDeviceInstanceSelector & \
                INTERNAL_EXTERNAL_DEVICE_INSTANCE_MASK) != 0x00)) {

                Option = SetUsbDeviceBootOptionToHighestPriority (
                             RequiredUsbDeviceInstance,
                             BbsDevType,
                             DpType,
                             DpSubType );
            } else if (IsNonUsbDeviceFound) {
                /* If the RequiredUsbDeviceInstance is not available,
                   then set high priority to first Non USB device.*/
                Option = TempOption;
            } else {
                /* If the RequiredUsbDeviceInstance is not available,
                   then set high priority to first USB device.*/
                Option = UsbTempOption;
            }
            
            if (!gUefiBoot) {
                // Saving the matched boot option details in NVRAM.
                SaveMatchedBootOptionData (Option->BootOptionNumber);
            }
            // Set the Boot Option as Highest tag and Priority.
            if (Option != NULL) {
                // Save the Current boot order.
                SaveCurrentBootorder ();

                Option->Priority = HIGHEST_BOOT_OPTION_PRIORITY;
                BootNumber = Option->BootOptionNumber;
            }
        }
        
        /* Set the same Priority to all the same group device and
           Tag will be used to differentiate after comparing the Priority.*/
        if (!gUefiBoot && BootNumber) {
            FOR_EACH_BOOT_OPTION (BootOptionList, Link, Option){
                if (BootNumber == Option->BootOptionNumber) {
                    Option->Priority = HIGHEST_BOOT_OPTION_PRIORITY;
                }
            }
        }
    }
    if (BootNumber) {
        DUMP_BOOT_OPTION_LIST(BootOptionList,"After Setting IPMI Priorities");
        return EFI_SUCCESS;
    }
    return EFI_NOT_FOUND;
}

