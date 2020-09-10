//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************

/** @file PowerControlPolicy.c
    Reads the Power Control Policy from BMC and updates in Setup
    and Power Control Policy to BMC according to the Setup Option selected using Setup Callback.
 
**/

#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Protocol/IpmiTransportProtocol.h>
#include <Protocol/AMIPostMgr.h>
#include <Uefi/UefiSpec.h>
#include <Include/AmiIpmiNetFnChassisDefinitions.h>
#include <ServerMgmtSetup.h>
#include <AmiIpmiBmcCommon.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PrintLib.h>
#include <Setup.h>
#include <Library/HiiLib.h>

#define STRING_BUFFER_LENGTH    50

AMI_POST_MANAGER_PROTOCOL                *gAmiPostMngrProtocol = NULL;
static AMI_IPMI_TRANSPORT_INSTANCE       *gIpmiTransport = NULL;


/**
    Update the power control policy String in setup.

    @param[in] HiiHandle               Handle to HII database.
    @param[in] PowerRestorePolicy      Power restore policy.
    @param[in] ServerMgmtConfiguration Contain ServerMgnt Data.
    @param[in]                         Attributes 
    @return EFI_STATUS                 Return status of function calls used.
**/
EFI_STATUS
UpdatePowerRestorePolicyState (
  IN  EFI_HII_HANDLE                 HiiHandle,
  IN  UINT8                          PowerRestorePolicy,
  IN  SERVER_MGMT_CONFIGURATION_DATA ServerMgmtConfiguration,
  IN  UINT32                         Attributes
  )
{
    EFI_STATUS                      Status;
    EFI_STRING                      UpdateStr;
    UINTN                           Size;

    DEBUG((DEBUG_INFO, "%a Entered... \n", __FUNCTION__));

    switch(PowerRestorePolicy) {
         case AmiPolicyPowerOff: 
             UpdateStr = HiiGetString (
                            HiiHandle,
                            STRING_TOKEN(STR_PWR_CONTROL_POLICY_DONOT_POWER),
                            NULL );
             break;
         case AmiPolicyLastPowerState:
             UpdateStr = HiiGetString (
                             HiiHandle,
                             STRING_TOKEN(STR_PWR_CONTROL_POLICY_LAST_STATE),
                             NULL );
             break;
         case AmiPolicyPowerRestore:
             UpdateStr = HiiGetString (
                             HiiHandle,
                             STRING_TOKEN(STR_PWR_CONTROL_POLICY_RESTORE),
                             NULL );
             break;
         default:
             UpdateStr = HiiGetString (
                             HiiHandle,
                             STRING_TOKEN(STR_PWR_CONTROL_POLICY_UNKNOWN),
                             NULL );
    }
    if (UpdateStr != NULL){
        HiiSetString (
              HiiHandle,
              STR_CONFIG_PWR_CONTROL_POLICY_VALUE,
              UpdateStr,
              NULL );
        gBS->FreePool (UpdateStr);
    }
    //Restore to Default (unspecified) value. after updating STR_CONFIG_PWR_CONTROL_POLICY_VALUE
    ServerMgmtConfiguration.PowerControlPolicy = AmiPolicyUnknown;

    Size = sizeof(SERVER_MGMT_CONFIGURATION_DATA);
    Status = gRT->SetVariable (
                    L"ServerSetup",
                    &gEfiServerMgmtSetupVariableGuid,
                    Attributes,   // Attributes,
                    Size,
                    &ServerMgmtConfiguration );

    if (EFI_ERROR (Status)) {
        DEBUG((DEBUG_ERROR, "gRT->SetVariable Status :: %r \n", Status));
    }
    DEBUG((DEBUG_INFO, "%a Entered... \n", __FUNCTION__));
    return Status;
}

/**
    Displays error message pop up in setup.

    @param[in] HiiHandle  Handle to HII database.
    @param[in] Message    Error message token value.

    @retval VOID
**/

VOID
IpmiProtocolErrorMessagePopUp (
  IN EFI_HII_HANDLE HiiHandle,
  IN UINT16         Message 
  )
{
    UINT8               MsgBoxSel;
    EFI_STRING          CharPtrString;
    EFI_STRING          MessagePtrString;

    DEBUG((DEBUG_INFO, "%a Entered \n", __FUNCTION__));

    CharPtrString = HiiGetString (
                        HiiHandle,
                        STRING_TOKEN(STR_PWR_CONTROL_POLICY_INFO),
                        NULL );

    MessagePtrString = HiiGetString ( HiiHandle, Message, NULL);
    if ((CharPtrString != NULL) && (MessagePtrString != NULL)) {
        gAmiPostMngrProtocol->DisplayMsgBox (
                                  CharPtrString,
                                  MessagePtrString,
                                  MSGBOX_TYPE_OK,
                                  &MsgBoxSel );

        gBS->FreePool (CharPtrString);
        gBS->FreePool (MessagePtrString);
    }

    DEBUG((DEBUG_INFO, "%a Exited \n", __FUNCTION__));
}

/**
    This function changes the PowerControl Policy option in Setup
    and Sends command to BMC with selected option in Setup.

    @param[in] HiiHandle   A handle that was previously registered in the
                           HII Database.
    @param[in] Class       Formset Class of the Form Callback Protocol passed in
    @param[in] SubClass    Formset sub Class of the Form Callback Protocol passed in
    @param[in] Key Formset Key of the Form Callback Protocol passed in

    @return EFI_STATUS Return Status

**/
EFI_STATUS
PowerControlPolicyCallbackFunction (
  IN  EFI_HII_HANDLE     HiiHandle,
  IN  UINT16             Class,
  IN  UINT16             SubClass,
  IN  UINT16             Key
  )
{
    EFI_STATUS                              Status;
    SERVER_MGMT_CONFIGURATION_DATA          ServerMgmtConfiguration ;
    UINTN                                   Size = sizeof(SERVER_MGMT_CONFIGURATION_DATA);
    AMI_IPMI_SET_POWER_RESTORE_POLICY_REQ   SetPowerRestoreReg;
    AMI_IPMI_SET_POWER_RESTORE_POLICY_RES   SetPowerRestoreRes;
    UINTN                                   SetPowerRestoreResSize;
    UINT32                                  Attributes;
    CALLBACK_PARAMETERS                     *CallbackParameters;

    DEBUG((DEBUG_INFO, "%a Entered with Key: %x \n", __FUNCTION__, Key));

    CallbackParameters = GetCallbackParameters();

    // Check for the key and Return if Key value does not match
    if ((Key != PWR_CONTROL_POLICY_KEY)) {
        DEBUG((DEBUG_ERROR, "\nIPMI: Callback function is called with Wrong Key Value, Returning EFI_UNSUPPORTED\n"));
        return EFI_UNSUPPORTED;
    }

    // Get the call back parameters and verify the action
    if (CallbackParameters->Action == EFI_BROWSER_ACTION_DEFAULT_STANDARD) {
        DEBUG((DEBUG_ERROR, "IPMI: CallbackParameters->Action == EFI_BROWSER_ACTION_DEFAULT_STANDARD... so load the defaults \n"));
        return  EFI_UNSUPPORTED;
    }

    if ((CallbackParameters->Action != EFI_BROWSER_ACTION_CHANGED) && (CallbackParameters->Action != EFI_BROWSER_ACTION_SUBMITTED)) {
        DEBUG((DEBUG_INFO, "IPMI: CallbackParameters->Action != EFI_BROWSER_ACTION_CHANGED "\
                           "&& CallbackParameters->Action != EFI_BROWSER_ACTION_SUBMITTED... so return EFI_SUCCESS\n"));
        return  EFI_SUCCESS;
    }

    // Locate AmiPostMgrProtocol to display the message boxes in setup 
    Status = gBS->LocateProtocol (
                    &gAmiPostManagerProtocolGuid,
                    NULL,
                    (VOID **)&gAmiPostMngrProtocol );

    if (EFI_ERROR (Status)) {
        DEBUG((DEBUG_ERROR, "IPMI: gAmiPostManagerProtocolGuid locateProtocol Status: %r\n", Status));
        return Status;
    }

    //EFI_BROWSER_ACTION_CHANGED case
    if (CallbackParameters->Action == EFI_BROWSER_ACTION_CHANGED) {
        if (gIpmiTransport == NULL) {   // IPMI transport protocol not found.

            DEBUG((DEBUG_ERROR, "\nIPMI: IPMI transport protocol not found\n"));
            IpmiProtocolErrorMessagePopUp (
                                HiiHandle,
                                STRING_TOKEN(STR_IPMI_PROTOCOL_ERROR) );

            // Set  PowerControlPolicy browser data.
            CallbackParameters->Value->u8 = AmiPolicyUnknown;

            return EFI_NOT_FOUND;
        }
        DEBUG((DEBUG_INFO, "IPMI: CallbackParameters->Action == EFI_BROWSER_ACTION_CHANGED... so return EFI_SUCCESS\n"));
        return  EFI_SUCCESS;
    }

    //When Browser action is submit,then the valid policy selected will be set in BMC.
 
    if (gIpmiTransport == NULL) {   // IPMI transport protocol not found.
        DEBUG((DEBUG_ERROR, "\nIPMI: IPMI transport protocol not found\n"));
        return  EFI_SUCCESS;
    }

    //if no action valid policy selected from PowerControlPolicy browser data. 
    if (CallbackParameters->Value->u8  == AmiPolicyUnknown) {
        return EFI_SUCCESS;
    }

    // Get Server Management Setup Variable
    Size = sizeof(SERVER_MGMT_CONFIGURATION_DATA);
    Status = gRT->GetVariable (
                    L"ServerSetup",
                    &gEfiServerMgmtSetupVariableGuid,
                    &Attributes,   // Attributes
                    &Size,
                    &ServerMgmtConfiguration );

    if (EFI_ERROR (Status)) {
        DEBUG((DEBUG_ERROR, "gRT->GetVariable Status :: %r \n", Status));
        return Status;
    }

    ZeroMem (
        &SetPowerRestoreReg,
        sizeof (SetPowerRestoreReg) );

    SetPowerRestoreReg.PowerRestorePolicy = ServerMgmtConfiguration.PowerControlPolicy; //2: Restore Power, 1: Last Power State, 0: Power OFF

    // SET Chassis Power policy according to the Setup option.
    SetPowerRestoreResSize = sizeof(AMI_IPMI_SET_POWER_RESTORE_POLICY_RES);

    Status = gIpmiTransport->SendIpmiCommand (
                                gIpmiTransport,
                                IPMI_NETFN_CHASSIS,
                                AMI_BMC_LUN,
                                IPMI_CHASSIS_SET_POWER_RESTORE_POLICY,
                                (UINT8 *) &SetPowerRestoreReg,
                                sizeof(AMI_IPMI_SET_POWER_RESTORE_POLICY_REQ),
                                (UINT8 *) &SetPowerRestoreRes,
                                (UINT8 *) &SetPowerRestoreResSize );

    if (EFI_ERROR (Status)) {

        DEBUG((DEBUG_ERROR, "Set Power Restore Policy status :: %r ", Status ));
        IpmiProtocolErrorMessagePopUp (
                        HiiHandle,
                        STRING_TOKEN(STR_PWR_CONTROL_POLICY_ERROR));

        CallbackParameters->Value->u8 = AmiPolicyUnknown;
        ServerMgmtConfiguration.PowerControlPolicy = AmiPolicyUnknown;

        Size = sizeof(SERVER_MGMT_CONFIGURATION_DATA);
        Status = gRT->SetVariable (
                        L"ServerSetup",
                        &gEfiServerMgmtSetupVariableGuid,
                        Attributes,   // Attributes,
                        Size,
                        &ServerMgmtConfiguration );
        return Status;
    }

    DEBUG((DEBUG_INFO, "Power Restore Support = %x, SetPowerRestoreResSize = %x\n", SetPowerRestoreRes.Support, SetPowerRestoreResSize));
    DEBUG((DEBUG_INFO, "BMC Power Restore Policy Set's to :: %d\n",SetPowerRestoreReg.PowerRestorePolicy));

    //Updating  BMC configured power control policy
    CallbackParameters->Value->u8 = AmiPolicyUnknown;
    //Restore to Default (unspecified) value. Once selected configuration updated in BMC.
    Status= UpdatePowerRestorePolicyState(
                                HiiHandle,
                                SetPowerRestoreReg.PowerRestorePolicy,
                                ServerMgmtConfiguration,
                                Attributes );

    if (EFI_ERROR (Status)) {
        DEBUG((DEBUG_INFO, "UpdatePowerRestorePolicyState :: %r \n", Status));
        return Status;
    }

    DEBUG((DEBUG_INFO, "%a Exiting... \n", __FUNCTION__));
    return EFI_SUCCESS;
}

/**
    This function will get execute when AmiTseNVRAMUpdateGuid is available which changes
    the PowerControl Policy option in Setup and Sends command to BMC with selected option in Setup.

    @param[in] Event   The EFI event
    @param[in] Context Context for the callback

    @retval VOID
**/

VOID
SetPowerControlPolicyStatus (
    IN EFI_EVENT  Event,
    IN VOID  *Context
  )
{
    EFI_STATUS                              Status;
    SERVER_MGMT_CONFIGURATION_DATA          ServerMgmtConfiguration ;
    UINTN                                   Size = sizeof(SERVER_MGMT_CONFIGURATION_DATA);
    AMI_IPMI_SET_POWER_RESTORE_POLICY_REQ   SetPowerRestoreReg;
    AMI_IPMI_SET_POWER_RESTORE_POLICY_RES   SetPowerRestoreRes;
    UINTN                                   SetPowerRestoreResSize;
    UINT32                                  Attributes;

    DEBUG((DEBUG_INFO, "%a Entered \n", __FUNCTION__));

    // Get Server Management Setup Variable
    Size = sizeof(SERVER_MGMT_CONFIGURATION_DATA);
    Status = gRT->GetVariable (
                    L"ServerSetup",
                    &gEfiServerMgmtSetupVariableGuid,
                    &Attributes,   // Attributes
                    &Size,
                    &ServerMgmtConfiguration );

    if (EFI_ERROR (Status)) {
        DEBUG((DEBUG_ERROR, "gRT->GetVariable Status :: %r \n", Status));
        return;
    }

    if (ServerMgmtConfiguration.PowerControlPolicy == AmiPolicyUnknown) {
        DEBUG((DEBUG_ERROR, "Policy has been already updated by callback Function\n"));
        return;
    }
    ZeroMem (
        &SetPowerRestoreReg,
        sizeof (SetPowerRestoreReg) );

    SetPowerRestoreReg.PowerRestorePolicy = ServerMgmtConfiguration.PowerControlPolicy; //2: Restore Power, 1: Last Power State, 0: Power OFF

    // SET Chassis Power policy according to the Setup option.
    SetPowerRestoreResSize = sizeof(AMI_IPMI_SET_POWER_RESTORE_POLICY_RES);

    Status = gIpmiTransport->SendIpmiCommand (
                                gIpmiTransport,
                                IPMI_NETFN_CHASSIS,
                                AMI_BMC_LUN,
                                IPMI_CHASSIS_SET_POWER_RESTORE_POLICY,
                                (UINT8 *) &SetPowerRestoreReg,
                                sizeof(AMI_IPMI_SET_POWER_RESTORE_POLICY_REQ),
                                (UINT8 *) &SetPowerRestoreRes,
                                (UINT8 *) &SetPowerRestoreResSize );

    if (EFI_ERROR (Status)) {
        DEBUG((DEBUG_ERROR, "Set Power Restore Policy status :: %r ", Status ));
        ServerMgmtConfiguration.PowerControlPolicy = AmiPolicyUnknown;
        Size = sizeof(SERVER_MGMT_CONFIGURATION_DATA);
        Status = gRT->SetVariable (
                        L"ServerSetup",
                        &gEfiServerMgmtSetupVariableGuid,
                        Attributes,   // Attributes,
                        Size,
                        &ServerMgmtConfiguration );
        return;
    }
    DEBUG((DEBUG_INFO, "Power Restore Support = %x, SetPowerRestoreResSize = %x\n", SetPowerRestoreRes.Support, SetPowerRestoreResSize));
    DEBUG((DEBUG_INFO, "BMC Power Restore Policy Set's to :: %d\n",SetPowerRestoreReg.PowerRestorePolicy));

    Status= UpdatePowerRestorePolicyState(
                                (EFI_HII_HANDLE)Context,
                                SetPowerRestoreReg.PowerRestorePolicy,
                                ServerMgmtConfiguration,
                                Attributes);

    if (EFI_ERROR (Status)) {
        DEBUG((DEBUG_ERROR, "UpdatePowerRestorePolicyState :: %r \n", Status));
    }

    DEBUG((DEBUG_INFO, "%a Exiting... \n", __FUNCTION__));
    return ;
}

/**
    This function initializes the Power Control Policy information 
    related setup option values.

    @param[in] HiiHandle Handle to HII database
    @param[in] Class     Indicates the setup class

    @retval VOID

**/

VOID
InitPowerControlPolicyStrings (
    IN EFI_HII_HANDLE     HiiHandle,
    IN UINT16             Class
  )
{
    EFI_STATUS                              Status;
    UINTN                                   Size;
    AMI_IPMI_GET_CHASSIS_STATUS_RESPONSE    GetChassisStatusResponse;
    UINT32                                  Attributes;
    SERVER_MGMT_CONFIGURATION_DATA          ServerMgmtConfiguration;
    EFI_EVENT                               FunctionEvent = NULL;
    VOID                                    *Registration;
    UINT8                                   PowerRestorePolicyState;

    // Continue only for Server Mgmt setup page
    if (Class != SERVER_MGMT_CLASS_ID) {
        return;
    }

    DEBUG((DEBUG_INFO, "%a Entered... \n", __FUNCTION__));

    Status = gBS->LocateProtocol (
                    &gEfiDxeIpmiTransportProtocolGuid,
                    NULL,
                    (VOID **)&gIpmiTransport );

    if (EFI_ERROR (Status)) {
        DEBUG((DEBUG_ERROR, " gEfiDxeIpmiTransportProtocolGuid Status :: %r \n", Status));
        return;
    }

    // Get Server Management Setup Variable
    Size = sizeof(SERVER_MGMT_CONFIGURATION_DATA);
    Status = gRT->GetVariable (
                    L"ServerSetup",
                    &gEfiServerMgmtSetupVariableGuid,
                    &Attributes,   // Attributes
                    &Size,
                    &ServerMgmtConfiguration );

    if (EFI_ERROR (Status)) {
        DEBUG((DEBUG_ERROR, "gRT->GetVariable Status :: %r \n", Status));
        return;
    }

    DEBUG((DEBUG_INFO, "gServerMgmtConfiguration.PowerControlPolicy (SetUp Option Value):: %d \n",ServerMgmtConfiguration.PowerControlPolicy));
     
    //Get the Value from BMC
    Size = sizeof(AMI_IPMI_GET_CHASSIS_STATUS_RESPONSE);
    Status = gIpmiTransport->SendIpmiCommand (
                                        gIpmiTransport,
                                        IPMI_NETFN_CHASSIS,
                                        AMI_BMC_LUN,
                                        IPMI_CHASSIS_GET_STATUS,
                                        NULL,
                                        0,
                                        (UINT8 *) &GetChassisStatusResponse,
                                        (UINT8 *)&Size );

    if (EFI_ERROR (Status)) {
        DEBUG((DEBUG_ERROR, "IPMI_CHASSIS_GET_STATUS->SendIpmiCommand Status :: %r \n", Status));
        return;
    }

    DEBUG((DEBUG_INFO, "Power Restore Policy (BMC Data) :: %x \n", GetChassisStatusResponse.CurrentPowerState.PowerRestorePolicy));
    PowerRestorePolicyState = GetChassisStatusResponse.CurrentPowerState.PowerRestorePolicy;

    Status = UpdatePowerRestorePolicyState(
                                 HiiHandle,
                                 PowerRestorePolicyState,
                                 ServerMgmtConfiguration,
                                 Attributes );

    if (EFI_ERROR (Status)) {
        DEBUG((DEBUG_ERROR, "UpdatePowerRestorePolicyState :: %r \n", Status));
        return;
    }

    // Register a callback notification on "TseNVRAM Update Guid"
    Status = RegisterProtocolCallback( 
                             &gAmiTseNVRAMUpdateGuid,
                             SetPowerControlPolicyStatus,
                             HiiHandle,
                             &FunctionEvent,
                             &Registration );
    
    if (EFI_ERROR (Status)) {
        DEBUG((DEBUG_ERROR, "Failed to Register ProtocolCallback for TseNVRAM :: %r \n", Status));
        return;
    }

    DEBUG((DEBUG_INFO, "%a Exiting... \n", __FUNCTION__));
}

