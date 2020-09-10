//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************


/** @file ServerMgmtSetup.c
    This driver is to initialize server management setup screen.

*/

//---------------------------------------------------------------------------

#include <UefiHii.h>
#include <Library/PrintLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Protocol/HiiConfigAccess.h>
#include <Protocol/HiiDatabase.h>
#include <Protocol/HiiUtilities.h>
#include <ServerMgmtSetup.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <AmiIpmiBmcCommon.h>
#include <Protocol/IpmiTransportProtocol.h>
#include <Library/AmiSelLib.h>

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

#define NUMBER_OF_FORMSETS  (sizeof(gSetupCallBack)/sizeof(CALLBACK_INFO))
#define STRING_SIZE         1024
#define STRING_BUFFER_LENGTH  20

/**
    These Data Structure define a structure used to match a specific
    Callback Protocol to an HII Form through the use of Class and SubClass
    values
**/
typedef struct {
    /// Callback.
    EFI_HII_CONFIG_ACCESS_PROTOCOL  Callback;
    UINT16                          Class;      ///< Formset class.
    UINT16                          SubClass;   ///< Formset sub-class.
} SETUP_CALLBACK;

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

/**
    This function is called to provide results data to the driver.
    This data consists of a unique key that is used to identify
    which data is either being passed back or being asked for.
    
    @param[in] This             Points to the EFI_HII_CONFIG_ACCESS_PROTOCOL.
    @param[in] Action           Specifies the type of action taken by the browser.
    @param[in] QuestionId       A unique value which is sent to the original
                                exporting driver so that it can identify the type
                                of data to expect. The format of the data tends to 
                                vary based on the opcode that generated the callback.
    @param[in] Type             The type of value for the question.
    @param[in] Value            A pointer to the data being sent to the original
                                exporting driver.
    @param[in] ActionRequest    On return, points to the action requested by the
                                callback function.
    
    @retval EFI_SUCCESS             The callback successfully handled the action.
    @retval EFI_OUT_OF_RESOURCES    Not enough storage is available to hold the
                                    variable and its data.
    @retval EFI_DEVICE_ERROR        The variable could not be saved.
    @retval EFI_UNSUPPORTED         The specified Action is not supported by the
                                    callback.

**/
EFI_STATUS
EFIAPI
Callback (
  IN CONST  EFI_HII_CONFIG_ACCESS_PROTOCOL  *This,
  IN        EFI_BROWSER_ACTION              Action,
  IN        EFI_QUESTION_ID                 KeyValue,
  IN        UINT8                           Type,
  IN        EFI_IFR_TYPE_VALUE              *Value,
  OUT       EFI_BROWSER_ACTION_REQUEST      *ActionRequest
);

/* Brings the definitions of the SDL token defined list of callbacks into this
   file as a list of functions that can be called.*/
#define ITEM_CALLBACK(Class,Subclass,Key,Callback) Callback
extern SETUP_ITEM_CALLBACK_HANDLER SERVER_MGMT_SETUP_ITEM_CALLBACK_LIST EndOfList;
#undef ITEM_CALLBACK

// This creates an array of callbacks to be used.
#define ITEM_CALLBACK(Class,Subclass,Key,Callback) {Class,Subclass,Key,&Callback}
SETUP_ITEM_CALLBACK ServerMgmtSetupItemCallback[] = { SERVER_MGMT_SETUP_ITEM_CALLBACK_LIST {0, 0, 0, NULL} };

CALLBACK_PARAMETERS                 *gCallbackParametersPtr = NULL;
EFI_HANDLE                          gThisImageHandle = NULL;
SERVER_MGMT_CONFIGURATION_DATA      gServerMgmtConfiguration;
static EFI_HII_STRING_PROTOCOL      *gHiiString = NULL;
static EFI_HII_DATABASE_PROTOCOL    *gHiiDatabase = NULL;

extern STRING_INIT_FUNC SERSVER_MGMT_STRING_INIT_LIST EndOfFunctionList;
STRING_INIT_FUNC *gStringInitFunc[] = { SERSVER_MGMT_STRING_INIT_LIST NULL };

/**
    These Variable definitions define the different formsets and what Callback
    protocol should be used for each one

**/
SETUP_CALLBACK gServerMgmtCallbackProtocol = {
                                               {NULL, NULL, Callback},
                                               SERVER_MGMT_CLASS_ID,
                                               0
                                             };

/**
    This array contains the different HII packages that are used in the system

    @note
    The HiiHandle is updated in the LoadResources function when the HII Packages
    are loaded.

**/
CALLBACK_INFO gSetupCallBack[] = {
    // Last field in every structure will be filled by the Setup
    {
        &gEfiServerMgmtSetupVariableGuid,
        &gServerMgmtCallbackProtocol.Callback,
        SERVER_MGMT_CLASS_ID,
        0,
        0
    }
};

//----------------------------------------------------------------------

/** @internal
    Function for use by legacy callbacks to retrieve the full callback
    parameters.

    @param VOID     Nothing.

    @return CALLBACK_PARAMETERS Pointer to the CALLBACK_PARAMTER structure
                                for consumption by legacy callbacks.

**/
CALLBACK_PARAMETERS* GetCallbackParameters (
  VOID )
{
    return gCallbackParametersPtr;
}

/**
    This function is called to provide results data to the driver.
    This data consists of a unique key that is used to identify
    which data is either being passed back or being asked for.
    
    @param[in] This             Points to the EFI_HII_CONFIG_ACCESS_PROTOCOL.
    @param[in] Action           Specifies the type of action taken by the browser.
    @param[in] QuestionId       A unique value which is sent to the original
                                exporting driver so that it can identify the type
                                of data to expect. The format of the data tends to 
                                vary based on the opcode that generated the callback.
    @param[in] Type             The type of value for the question.
    @param[in] Value            A pointer to the data being sent to the original
                                exporting driver.
    @param[in] ActionRequest    On return, points to the action requested by the
                                callback function.
    
    @retval EFI_SUCCESS             The callback successfully handled the action.
    @retval EFI_OUT_OF_RESOURCES    Not enough storage is available to hold the
                                    variable and its data.
    @retval EFI_DEVICE_ERROR        The variable could not be saved.
    @retval EFI_UNSUPPORTED         The specified Action is not supported by the
                                    callback.

**/
EFI_STATUS
EFIAPI
Callback (
  IN CONST  EFI_HII_CONFIG_ACCESS_PROTOCOL  *This,
  IN        EFI_BROWSER_ACTION              Action,
  IN        EFI_QUESTION_ID                 KeyValue,
  IN        UINT8                           Type,
  IN        EFI_IFR_TYPE_VALUE              *Value,
  OUT       EFI_BROWSER_ACTION_REQUEST      *ActionRequest )
{
    SETUP_CALLBACK          *SetupCallback = (SETUP_CALLBACK*)This;
    CALLBACK_PARAMETERS     CallbackParameters;
    EFI_STATUS              Status;
    SETUP_ITEM_CALLBACK     *ItemCallback = ServerMgmtSetupItemCallback;

    gCallbackParametersPtr = &CallbackParameters;

    CallbackParameters.This             = (VOID*)This;
    CallbackParameters.Action           = Action;
    CallbackParameters.KeyValue         = KeyValue;
    CallbackParameters.Type             = Type;
    CallbackParameters.Value            = Value;
    CallbackParameters.ActionRequest    = ActionRequest;
    if (ActionRequest) {
        *ActionRequest = EFI_BROWSER_ACTION_REQUEST_NONE;
    }
    Status = EFI_UNSUPPORTED;

    while(ItemCallback->UpdateItem) {

        if ((ItemCallback->Class == SetupCallback->Class) &&
            (ItemCallback->SubClass == SetupCallback->SubClass) &&
            (ItemCallback->Key == KeyValue)) {

            Status = ItemCallback->UpdateItem (
                                        gSetupCallBack[0].HiiHandle,
                                        ItemCallback->Class,
                                        ItemCallback->SubClass,
                                        KeyValue );
            if (Status != EFI_UNSUPPORTED) {
                break;
            }
        }
        ItemCallback++;
    }
    gCallbackParametersPtr = NULL;
    return Status;
}

/**
    This function is called for each Formset and initializes strings based
    on the porting provided and then updates the HII database

    @param[in] HiiHandle    Handle to HII database.
    @param[in] CallBackInfo Pointer to an instance of CALLBACK_INFO
                            that works with HiiHandle.

    @return VOID    Nothing.

**/
VOID
InitFunction (
  IN EFI_HII_HANDLE     HiiHandle,
  IN CALLBACK_INFO      *CallBackInfo )
{
    UINT16  Index;

    if (!CallBackInfo || !CallBackInfo->HiiHandle) {
        return;
    }

    for (Index = 0; gStringInitFunc[Index] != NULL; Index++) {
        gStringInitFunc[Index] (
            HiiHandle,
            CallBackInfo->Class );
    }
}

/**
    This function publishes all HII resources and initializes the HII databases
    There is a token ALWAYS_PUBLISH_HII_RESOURCES that would call this function
    on every boot not just when the user tries to enter Setup.

    @param[in] Event    Event which caused this handler.
    @param[in] Context  Context passed during Event Handler registration.

    @return VOID    Nothing.

**/
VOID
EFIAPI
SetupCallback (
  IN EFI_EVENT  Event,
  IN VOID       *Context )
{
    static BOOLEAN  ResourcesLoaded = FALSE;

    // Locate the HII based protocols.
    if (!gHiiString &&
        EFI_ERROR(gBS->LocateProtocol (
                        &gEfiHiiStringProtocolGuid,
                        NULL,
                        (VOID **)&gHiiString ))) {
        return;
    }
    if (!gHiiDatabase &&
        EFI_ERROR(gBS->LocateProtocol (
                        &gEfiHiiDatabaseProtocolGuid,
                        NULL,
                        (VOID **)&gHiiDatabase ))) {
        return;
    }
    if (Event) {
        gBS->CloseEvent (Event);
    }
    if (!ResourcesLoaded) {
        ResourcesLoaded = TRUE;
        LoadResources(
            gThisImageHandle,
            NUMBER_OF_FORMSETS,
            gSetupCallBack,
            InitFunction );
    }
}

/**
    Entry point of the Server Management screen setup driver.

    @param[in] ImageHandle  Handle of this driver image.
    @param[in] SystemTable  Table containing standard EFI services.

    @retval EFI_SUCCESS     Driver initialized successfully.
    @retval EFI_UNSUPPORTED Unable to create nvram variable.

**/
EFI_STATUS
EFIAPI
InstallServerMgmtSetupScreen (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable )
{
    UINTN       Size;
    EFI_STATUS  Status;
    UINT32      Attributes;

    InitAmiLib (
        ImageHandle,
        SystemTable );

    gThisImageHandle = ImageHandle;
    Size = sizeof(SERVER_MGMT_CONFIGURATION_DATA);
    
    Status = gRT->GetVariable (
                    L"ServerSetup",
                    &gEfiServerMgmtSetupVariableGuid,
                    &Attributes,
                    &Size,
                    &gServerMgmtConfiguration );
    
    if (Status == EFI_NOT_FOUND) {
        SetupCallback (NULL,NULL);
        
        Status = gRT->SetVariable (
                        L"ServerSetup",
                        &gEfiServerMgmtSetupVariableGuid,
                        Attributes,
                        sizeof(SERVER_MGMT_CONFIGURATION_DATA),
                        &gServerMgmtConfiguration );
        
        // Unable to create nvram variable, so return.
        if (EFI_ERROR(Status)) {
            return EFI_UNSUPPORTED;
        }
    } else {
#if ALWAYS_PUBLISH_HII_RESOURCES
    {
        VOID            *Registration;
        EFI_EVENT       BdsEventBeforeSetup;
        EFI_GUID        BdsBeforeSetupGuid = EFI_BDS_EVENT_BEFORE_SETUP_GUID;

        // Register call back notification on "BDS event before setup GUID".
        RegisterProtocolCallback (
            &BdsBeforeSetupGuid,
            SetupCallback,
            NULL,
            &BdsEventBeforeSetup,
            &Registration );
    }
#else
    {
        VOID            *Registration;
        EFI_EVENT       SetupEnterEvent;
        EFI_GUID        SetupEnterGuid = AMITSE_SETUP_ENTER_GUID;

        // Register callback notification on "Setup Enter GUID".
        RegisterProtocolCallback (
            &SetupEnterGuid,
            SetupCallback,
            NULL,
            &SetupEnterEvent,
            &Registration );
    }
#endif
    }
    return EFI_SUCCESS;
}

/**
    This function initializes the BMC SEl Delete support
    information in setup.

    @param HiiHandle A handle that was previously registered in the
                     HII Database.
    @param Class    Formset Class of the Form Callback Protocol passed in

    @retval VOID

**/
#if DxeSelStatusCode_SUPPORT
VOID
InitSelDelSupport(
  IN EFI_HII_HANDLE HiiHandle,
  IN UINT16         Class )
{
    EFI_STATUS                         Status;
    SERVER_MGMT_CONFIGURATION_DATA     ServerMgmtConfiguration;
    UINTN                              Size;
    UINT32                             Attributes;
    EFI_IPMI_TRANSPORT                 *IpmiTransport = NULL;
    BOOLEAN                            Delsupport;
    EFI_STRING                         SelStr;

    DEBUG ((DEBUG_INFO, "%a Entry...\n", __FUNCTION__));
    //
    // Locate IpmiTransport protocol
    //
    Status = gBS->LocateProtocol (
                     &gEfiDxeIpmiTransportProtocolGuid,
                     NULL,
                     (VOID**)&IpmiTransport );

    if (EFI_ERROR (Status)) {
            DEBUG((DEBUG_ERROR,"%a LocateProtocol gEfiDxeIpmiTransportProtocolGuid Status:%x ",__FUNCTION__,Status));
            return;
    }

    Size = sizeof (ServerMgmtConfiguration);
    Status = gRT->GetVariable (
                    L"ServerSetup",
                    &gEfiServerMgmtSetupVariableGuid,
                    &Attributes,
                    &Size,
                    &ServerMgmtConfiguration );

    DEBUG ((DEBUG_INFO, "IPMI : Status of ServerSetup GetVariable() = %r\n",Status));

    if (EFI_ERROR(Status)) {
        return;
    }

    if( AmiIpmiIsSelFull (
            IpmiTransport,
            &Delsupport )  == TRUE ){
        SelStr = HiiGetString (
                        HiiHandle,
                        STRING_TOKEN(CHECK_SEL_FULL),
                        NULL );
    } else {
        SelStr = HiiGetString (
                        HiiHandle,
                        STRING_TOKEN(STR_SEL_BLANK),
                        NULL );
    }
    if ( SelStr != NULL) {
         HiiSetString(
                HiiHandle,
                STRING_TOKEN(STR_CHECK_SEL_FULL),
                SelStr,
                NULL );
         gBS->FreePool(SelStr);
    }

    ServerMgmtConfiguration.SelDelSupport = Delsupport;
    DEBUG ((DEBUG_INFO, " ServerMgmtConfiguration.SelDelSupport: %X \n", ServerMgmtConfiguration.SelDelSupport));

    Status = gRT->SetVariable (
                        L"ServerSetup",
                        &gEfiServerMgmtSetupVariableGuid,
                        Attributes,   // Attributes,
                        Size,
                        &ServerMgmtConfiguration
                        );
    DEBUG ((DEBUG_INFO, "%a Exit...\n", __FUNCTION__));
}
#endif 

