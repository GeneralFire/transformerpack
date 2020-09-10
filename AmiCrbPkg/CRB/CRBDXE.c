//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2020, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

/** @file CRBDXE.c

    This file contains code for Chipset Reference Board Template 
    initialization in the DXE stage
*/

#include <Token.h>
#include <IndustryStandard/Pci30.h>
#include <Efi.h>
#include <AmiLib.h>
#include <AmiDxeLib.h>
#include <Setup.h>
#include <Dxe.h>
#include <Pci.h>
#include <AmiHobs.h>
#include <UpdateSmbiosTable.h>
#include <AmiCspLib.h>
#if defined CIRA_SAMPLE_SUPPORT && CIRA_SAMPLE_SUPPORT == 1
#include <ME.h>
#include <Amt.h>
#endif
#include <CrbDxeInitElink.h>
#include <Protocol/PciIo.h>
#include <Protocol/PciRootBridgeIo.h>
#include <Protocol/BootScriptSave.h>
#if CMOS_MANAGER_SUPPORT && CSM_SUPPORT
#include <Guid/SetupVariable.h>
#include <Include/CmosAccess.h>
#include <Register/PchRegsSata.h>
#include <Library/UefiLib.h>
#endif

#include <Setup.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Protocol/AmiBoardInfo2.h> 
#include <Protocol/IioSystem.h> 

#include <PchPolicyCommon.h> 
#include <Library/PcdLib.h>
#include <Library/BaseMemoryLib.h>
#include <Protocol/AmiOptionRomPolicy.h>
#include <NetworkStackSetup.h>
#include <Library/UefiBootServicesTableLib.h>

EFI_GUID    gAmiBoardInfo2Guid = AMI_BOARD_INFO2_PROTOCOL_GUID;
VOID        *gEfiPciIoProtocolRegistration = NULL;;

#if CMOS_MANAGER_SUPPORT && CSM_SUPPORT
/**
  Read the user settings for SCU and SATA OROM and update into the
  CMOS token for use in the Get RSTe OROM Boot Info Int 15 handler.

  @param[in]  Event                 Event whose notification function is being invoked.
  @param[in]  Context               The pointer to the notification function's context,
                                    which is implementation-dependent.

    @return     VOID
**/

VOID
SetRsteBootInfo (
  IN  EFI_EVENT             Event,
  IN  VOID                  *Context
  )
{
// APTIOV_SERVER_OVERRIDE_START :   Revisit changes needed  
    /*

    EFI_STATUS					Status;
    PCH_SETUP      PchRcConfiguration;
    EFI_GUID 					SetupGuid = SETUP_GUID;
    UINTN 						VariableSize = 0;
    UINT8                               RsteBootInfo = 0; 
    UINT8						BitPos = 0;
    UINT8                               MaskValue = CMOSMGR_RSTE_BOOT_INFO_OFFSET_MASK;
    EFI_CMOS_ACCESS_INTERFACE           *CmosAccess;


    Status = pBS->LocateProtocol( &gAmiCmosAccessDxeProtocolGuid, NULL, &CmosAccess ); 
    if ( EFI_ERROR ( Status ) ){
      return;
    }
    VariableSize = sizeof( PCH_SETUP );
    Status = pRS->GetVariable(
    		L"PchSetup",
    		&gEfiPchSetupGuid,
    		NULL,
    		&VariableSize,
    		&PchRcConfiguration
    		);
    if ( EFI_ERROR ( Status ) ) {
	// PCH setup variable is moved to PCH_SETUP
        PchRcConfiguration.SataLegacyBootContSel = 1; //default value
        PchRcConfiguration.sSataLegacyBootContSel = 1; //default value
    }

    CmosAccess->Read( CmosAccess, CMOSMGR_RSTE_BOOT_INFO_OFFSET, &RsteBootInfo );
    RsteBootInfo &= ~CMOSMGR_RSTE_BOOT_INFO_OFFSET_MASK; // Both SATA and sSATA controller is selected as boot controller.  
	
    if (( PchRcConfiguration.sSataInterfaceMode == PchSataModeRaid && PchRcConfiguration.sSataLegacyBootContSel ) &&
    ( PchRcConfiguration.SataInterfaceMode == PchSataModeRaid && PchRcConfiguration.SataLegacyBootContSel )){ // PCH setup variable is moved to PCH_SETUP
	    goto RsteBootInfoExit; // Both SATA nor SCU controller is selected as boot controller.
    }
    
    if ( (!(PchRcConfiguration.sSataLegacyBootContSel)) && (!(PchRcConfiguration.SataLegacyBootContSel)) ) { // PCH setup variable is moved to PCH_SETUP
        RsteBootInfo |= CMOSMGR_RSTE_BOOT_INFO_OFFSET_MASK; // Neither SATA nor SCU controller is selected as boot controller.
    	goto RsteBootInfoExit;
    }

    // Get the Boot Info bits position from masked data. 
    while ( TRUE ) {
    	if ( MaskValue & BIT00 )
    		break;
    	BitPos++; 
    	MaskValue = MaskValue >> 1;       
    }          

    if (( PchRcConfiguration.sSataInterfaceMode == PchSataModeRaid) && PchRcConfiguration.sSataLegacyBootContSel ) // PCH setup variable is moved to PCH_SETUP
        RsteBootInfo |= ( BIT00 << BitPos );  // sSATA OROM is enabled.

    if (( PchRcConfiguration.SataInterfaceMode == PchSataModeRaid)&& PchRcConfiguration.SataLegacyBootContSel ) // PCH setup variable is moved to PCH_SETUP
        RsteBootInfo |= ( BIT01 << BitPos ); // SATA OROM is enabled.

RsteBootInfoExit:
    Status = CmosAccess->Write (CmosAccess, CMOSMGR_RSTE_BOOT_INFO_OFFSET, RsteBootInfo);
    if ( EFI_ERROR ( Status ) ){
        DEBUG ((EFI_D_ERROR," Get RSTe OROM Boot Info BIOS Services: Unable to write to CMOS variable holding RSTe Boot info. Status %r\n", Status ));
        return;
    }
	
    pBS->CloseEvent(Event);
  
// APTIOV_SERVER_OVERRIDE_END :   Revisit changes needed  
*/
}
#endif

/**
    Restricts connecting Network devices using BdsConnect Policy protocol
    
    @param   Event            The event that occurred
    @param   Context          For EFI compatibility.  Not used.

    @return  VOID
**/
VOID
EFIAPI
EfiPciIoProtocolReady (
  IN  EFI_EVENT         Event,
  IN  VOID              *Context )
{
    
    EFI_STATUS                     Status;
    EFI_HANDLE                     Handle;
    UINTN                          BufferSize = sizeof(Handle);
    AMI_OPTION_ROM_POLICY_PROTOCOL *OpRomPolicy = NULL;
    EFI_PCI_IO_PROTOCOL            *PciIo = NULL;
    PCI_TYPE00                      PciConfig;
    UINT8                           ItemPolicy;

    // Get Handle from Protocol registration
    Status = pBS->LocateHandle(
                    ByRegisterNotify, 
                    NULL, 
                    gEfiPciIoProtocolRegistration, 
                    &BufferSize, 
                    &Handle );
    if(EFI_ERROR(Status)) {
        //DEBUG ((EFI_D_ERROR,"Locate EfiPciIoProtocol handle = %r\n", Status ));
        return;
    }

    // Locate PciIo protocol on Handle
    Status = pBS->HandleProtocol(
                    Handle, 
                    &gEfiPciIoProtocolGuid, 
                    &PciIo );
    if (EFI_ERROR(Status)) {
        //DEBUG ((EFI_D_ERROR,"HandleProtocol for PciIo = %r\n", Status ));
        return;
    }

    // Locate AmiOptionRomPolicy protocol on Handle
    Status = pBS->HandleProtocol(   
                    Handle, 
                    &gAmiOptionRomPolicyProtocolGuid, 
                    &OpRomPolicy );
    if (EFI_ERROR(Status)) {
        //DEBUG ((EFI_D_ERROR,"HandleProtocol for OpRomPolicy = %r\n", Status ));
        return;
    }

    // Read Class code and subclass code
    Status = PciIo->Pci.Read (  
                    PciIo,
                    EfiPciIoWidthUint8,
                    0,
                    sizeof (PCI_TYPE00),
                    &PciConfig );
    if (EFI_ERROR(Status)) {
        DEBUG ((EFI_D_ERROR,"Get Class and subclass code Status = %r\n", Status ));
        return;
    }

    // Check for Network Controller class type
    if (IS_CLASS1 (&PciConfig, PCI_CLASS_NETWORK)) {
        DEBUG ((EFI_D_INFO,"Class code is Network.\n" ));

        // Do not disable Oprom for Fabric controller
        if (!IS_CLASS2 (&PciConfig, PCI_CLASS_NETWORK, 0x08)) {
            Status = OpRomPolicy->GetOptionRomPolicy(OpRomPolicy, &ItemPolicy,NULL);
            if (EFI_ERROR(Status)) {
                DEBUG ((EFI_D_ERROR,"GetOptionRomPolicy = %r\n", Status ));
                return;
            }
            DEBUG ((EFI_D_INFO,"OptionRomPolicy  = %d\n", ItemPolicy ));

            // Check if Oprom policy is UEFI
            if(ItemPolicy==AMI_OPTION_ROM_POLICY_UEFI){
                DEBUG ((EFI_D_INFO,"Oprom policy is UEFI, disable Oprom.\n" ));
                Status = OpRomPolicy->SetOptionRomPolicy( OpRomPolicy, AMI_OPTION_ROM_POLICY_DISABLE );
                DEBUG ((EFI_D_INFO,"SetOptionRomPolicy  = %r\n", Status )); 
            }else{
                DEBUG ((EFI_D_INFO,"OptionRomPolicy is not UEFI\n" ));
                return;
            }
        }else{
            DEBUG ((EFI_D_INFO,"This is Fabric controller, return..\n" ));
            return; 
        }
    }

    return;
}

/**
    Implements Optimized Boot functionality

    @param   VOID

    @return  VOID
**/
VOID 
OptimizedBoot (VOID)
{
    EFI_STATUS              Status = EFI_SUCCESS;
    EFI_GUID                SetupGuid = SETUP_GUID;
    UINTN                   SetupDataSize;
    SETUP_DATA              SetupData;
    UINT32                  Attributes = 0;
    NETWORK_STACK           NetworkStackSetupData;

    // Get Setup Data variable
    SetupDataSize = sizeof(SETUP_DATA);
    Status = pRS->GetVariable (  
                    L"Setup",
                    &SetupGuid,
                    &Attributes,
                    &SetupDataSize,
                    &SetupData );
    if(EFI_ERROR (Status)){
        DEBUG ((EFI_D_ERROR,"Optimized Boot Get Setup Variable = %r\n", Status ));
        return;
    }

    // Check for Optimized Boot setup option
    if (SetupData.OptimizedBoot){
        DEBUG ((EFI_D_INFO,"Optimized Boot = %d, Optimized is Enabled\n", SetupData.OptimizedBoot ));

#if CSM_SUPPORT
        // Check for Csm Support option
        if (SetupData.CsmSupport == 1){
            // CSM is enabled so disable CSM Support using Setup Variable.
            SetupData.CsmSupport = 0;
            Status = pRS->SetVariable( 
                            L"Setup", 
                            &SetupGuid, 
                            Attributes,  
                            SetupDataSize, 
                            &SetupData );
            if(EFI_ERROR (Status)){
                DEBUG ((EFI_D_ERROR,"Optimized Boot Set Setup Variable = %r\n", Status ));
                return;
            }
            DEBUG ((EFI_D_INFO,"Disable CSM Support.\n"));
        }
#endif // CSM_SUPPORT

        // Get NetworkStack support option
        SetupDataSize = sizeof(NETWORK_STACK);
        Status = pRS->GetVariable(
                        L"NetworkStackVar",
                        &gEfiNetworkStackSetupGuid, 
                        &Attributes, 
                        &SetupDataSize, 
                        &NetworkStackSetupData );
        if(EFI_ERROR (Status)){
            DEBUG ((EFI_D_ERROR,"Optimized Boot Get Network Stack Variable = %r\n", Status ));
            return;
        }

        // Create Notification event for gEfiPciIoProtocolGuid GUID
        if (NetworkStackSetupData.Enable == 0){
            DEBUG ((EFI_D_INFO,"Register protocol notification for PciIoProtocol, to disable Network devices \n" ));
            EfiCreateProtocolNotifyEvent (
                    &gEfiPciIoProtocolGuid,
                    TPL_CALLBACK,
                    EfiPciIoProtocolReady,
                    NULL,
                    &gEfiPciIoProtocolRegistration
                    );
        }
    }else{
        DEBUG ((EFI_D_INFO,"Optimized Boot = %d, Optimized is Disable\n", SetupData.OptimizedBoot ));
        return;
    }

    return;
}

/**
  BDS Hook function will signal gSignalBeforeEnterSetupGuid Event. 
  This function will install the gSignalBeforeEnterSetupGuid
  in the BDS phase to make the GUID to be signaled while 
  POST screen.

  @param[in]  Event                 Event whose notification function is being invoked.
  @param[in]  Context               The pointer to the notification function's context,
                                    which is implementation-dependent.
  @return     VOID

**/
VOID 
SignalBeforeEnterSetupEvent(
  IN  EFI_EVENT     Event,
  IN  VOID          *Context
)
{
  EFI_HANDLE  Handle = NULL; 
  EFI_STATUS Status;
  Status = pBS->InstallProtocolInterface (
                      &Handle, 
                      &gSignalBeforeEnterSetupGuid, 
                      EFI_NATIVE_INTERFACE,
                      NULL);
  if(EFI_ERROR(Status)){
      DEBUG((DEBUG_ERROR, "CRBLIB ERROR: gSignalBeforeEnterSetupGuid is not installed, Status=%r \n", Status));
  }
  Status = pBS->UninstallProtocolInterface (
          Handle, &gSignalBeforeEnterSetupGuid, NULL
          );
  if(EFI_ERROR(Status)){
      DEBUG((DEBUG_ERROR, "CRBLIB ERROR: gSignalBeforeEnterSetupGuid is not un-installed, Status=%r \n", Status));
  }
  return;
}

/**
    This function is the entry point for this DXE. This function
    initializes the CRB
    
    @param   ImageHandle                   Image handle
    @param   SystemTable                   Pointer to the system table

    @return  Status                        Return Status based on errors that occurred while waiting for
                                           time to expire.
**/
EFI_STATUS 
CrbDxeInit (
  IN EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE     *SystemTable 
)
{
    EFI_STATUS                  Status = EFI_SUCCESS;
    EFI_EVENT                  UpdateSmbiosEvent;
    VOID                        *BeforeEnterSetupRegistration;
    EFI_EVENT                   BeforeEnterSetupEvent;

#if defined CIRA_SAMPLE_SUPPORT && CIRA_SAMPLE_SUPPORT == 1
    AMI_AMT_PROTOCOL        *AmtProtocol;
    EFI_GUID                AmtProtocolGuid = AMI_AMT_PROTOCOL_GUID;
    EFI_GUID                SetupGuid = SETUP_GUID;
    UINTN                   SetupDataSize = NULL;
    SETUP_DATA              SetupData;
#endif

    InitAmiLib(ImageHandle, SystemTable);

#if defined CIRA_SAMPLE_SUPPORT && CIRA_SAMPLE_SUPPORT == 1
    SetupDataSize = sizeof(SETUP_DATA);
    Status = pRS->GetVariable (  
                                L"Setup",
                                &SetupGuid,
                                NULL,
                                &SetupDataSize,
                                &SetupData 
                              );

    if (SetupData.CIRATrigger == 1)
    {
        Status = pBS->LocateProtocol(&AmtProtocolGuid, NULL, &AmtProtocol);
        if (!EFI_ERROR(Status))
            AmtProtocol->SetCIRA(AmtProtocol, CIRA_ENABLE, CIRA_DEFFAULT_TIME);
    }
#endif
    
    Status = gBS->CreateEvent (
                    EVT_NOTIFY_SIGNAL,
                    TPL_CALLBACK,
                    SignalBeforeEnterSetupEvent,
                    NULL,
                    &BeforeEnterSetupEvent
                    );
    ASSERT_EFI_ERROR (Status);
    Status = gBS->RegisterProtocolNotify (
                    &gAmiTseBeforeTimeOutGuid,
                    BeforeEnterSetupEvent,
                    &BeforeEnterSetupRegistration
                    );
    ASSERT_EFI_ERROR (Status);

    //
    // Create the event updating SMBIOS tables
    //
    Status = gBS->CreateEventEx (
                    EVT_NOTIFY_SIGNAL,
                    TPL_CALLBACK,
                    UpdateSmbiosTables,
                    NULL,
                    &gEfiEventReadyToBootGuid,
                    &UpdateSmbiosEvent
                    );
    ASSERT_EFI_ERROR (Status);
 // RSTe Support Starts
#if CMOS_MANAGER_SUPPORT && CSM_SUPPORT
    {
        VOID                *AmiCmosAccessProtocolRegistration;

        EfiCreateProtocolNotifyEvent (&gAmiCmosAccessDxeProtocolGuid,
                                      TPL_CALLBACK,
                                      SetRsteBootInfo,
                                      NULL,
                                      &AmiCmosAccessProtocolRegistration );
    }
    
#endif

    // Handle Optimized Boot
    OptimizedBoot();

    return EFI_SUCCESS;
}
