//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2020, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************
/** @file AmiIScsiConfig.c
    Provides the AMI Override functions for the IScsi EDKII driver 

**/

#include "AmiIScsi.h"

CHAR16   *PopUpTitle             = NULL;
extern   CHAR16                  mVendorStorageName[];
extern   HII_VENDOR_DEVICE_PATH  mIScsiHiiVendorDevicePath;

#if (ISCSI_FORCEFULLY_SET_INITIATOR_NAME)
BOOLEAN gIsInitiatorNameForceSet = FALSE;
CHAR8   gForcedInitiatorName[ISCSI_NAME_MAX_SIZE] = {"0"};
#endif

/**
  Callback function when user presses "Change Priority".

  @retval EFI_INVALID_PARAMETER  Any parameter is invalid.
  @retval EFI_OUT_OF_RESOURCES   Does not have sufficient resources to finish this
                                 operation.
  @retval EFI_SUCCESS            The operation is completed successfully.

**/
EFI_STATUS
IScsiConfigDisplayPriorityList (
    ISCSI_CONFIG_IFR_NVDATA     *IfrNvData
)
{
    EFI_STATUS                  Status;
    VOID                        *StartOpCodeHandle;
    EFI_IFR_GUID_LABEL          *StartLabel;
    VOID                        *EndOpCodeHandle;
    EFI_IFR_GUID_LABEL          *EndLabel;
    VOID                        *OptionsOpCodeHandle;
    EFI_STRING_ID               StringToken = 0;
    UINT8                       Index;
    UINT8                       *PriorityOrder;
    UINTN                       PriorityOrderSize;
    CHAR16                      *CharPtr;

    Status = IScsiCreateOpCode (
               PRIORITY_ENTRY_LABEL,
               &StartOpCodeHandle,
               &StartLabel,
               &EndOpCodeHandle,
               &EndLabel
               );
    if (EFI_ERROR (Status)) {
        return Status;
    }
    ASSERT (StartOpCodeHandle != NULL);

    OptionsOpCodeHandle = NULL;

    //
    // Create Option OpCode.
    //
    OptionsOpCodeHandle = HiiAllocateOpCodeHandle ();
    if (OptionsOpCodeHandle == NULL) {
        Status = EFI_OUT_OF_RESOURCES;
        goto Error;
    }  
    PriorityOrder = IScsiGetVariableAndSize (
                                         L"PriorityOrder",
                                         &gIScsiConfigGuid,
                                         &PriorityOrderSize
                                         );
    
    if ((PriorityOrder == NULL) && (PriorityOrderSize == 0)) {
        Status = IScsiSetDefaultPriority();
        if (EFI_ERROR(Status)) {
            return Status;
        }
        PriorityOrder = IScsiGetVariableAndSize (
                                                 L"PriorityOrder",
                                                 &gIScsiConfigGuid,
                                                 &PriorityOrderSize
                                                 );
    }
    for (Index=0; Index< ISCSI_MAX_ATTEMPT_TYPE;Index++) {
         if (PriorityOrder[Index] == SETUP_ATTEMPT) {
            CharPtr=NetLibHiiGetString(mCallbackInfo->RegisteredHandle,STRING_TOKEN(STR_HOST_ATTEMPT),NULL);
            StringToken = HiiSetString( mCallbackInfo->RegisteredHandle, 0, CharPtr , NULL);
        }
        if (PriorityOrder[Index] == REDFISH_ATTEMPT) {
            CharPtr=NetLibHiiGetString(mCallbackInfo->RegisteredHandle,STRING_TOKEN(STR_REDFISH_ATTEMPT),NULL);
            StringToken = HiiSetString( mCallbackInfo->RegisteredHandle, 0, CharPtr , NULL);
        }
        if (PriorityOrder[Index] == RSD_ATTEMPT) {
            CharPtr=NetLibHiiGetString(mCallbackInfo->RegisteredHandle,STRING_TOKEN(STR_RSD_ATTEMPT),NULL);
            StringToken = HiiSetString( mCallbackInfo->RegisteredHandle, 0, CharPtr , NULL);
        }
        HiiCreateOneOfOptionOpCode (
            OptionsOpCodeHandle,
            StringToken,
            0,
            EFI_IFR_NUMERIC_SIZE_1,
            PriorityOrder[Index]
            );
        IfrNvData->PriorityOrderedList[Index] = PriorityOrder[Index];
    }

    HiiCreateOrderedListOpCode (
      StartOpCodeHandle,                               // Container for dynamic created opcodes
      DYNAMIC_PRIORITY_LIST_QUESTION_ID,               // Question ID
      CONFIGURATION_VARSTORE_ID,                       // VarStore ID
      DYNAMIC_PRIORITY_LIST_VAR_OFFSET,                // Offset in Buffer Storage
      STRING_TOKEN (STR_PRIORITY_CHANGE_ENTRY),        // Question prompt text  
      STRING_TOKEN (STR_PRIORITY_CHANGE_ENTRY_HELP),   // Question help text
      0,                                               // Question flag
      EFI_IFR_UNIQUE_SET,                              // Ordered list flag, e.g. EFI_IFR_UNIQUE_SET
      EFI_IFR_NUMERIC_SIZE_1,                          // Data type of Question value
      ISCSI_MAX_ATTEMPT_TYPE,                                               // Maximum container
      OptionsOpCodeHandle,                             // Option Opcode list
      NULL                                             // Default Opcode is NULL
      );

    Status = HiiUpdateForm (
               mCallbackInfo->RegisteredHandle, // HII handle
               &gIScsiConfigGuid,               // Formset GUID
               FORMID_PRIORITY_FORM,            // Form ID
               StartOpCodeHandle,               // Label for where to insert opcodes
               EndOpCodeHandle                  // Replace data
               );

  Error:
    HiiFreeOpCodeHandle (StartOpCodeHandle);
    HiiFreeOpCodeHandle (EndOpCodeHandle);
    if (OptionsOpCodeHandle != NULL) {
      HiiFreeOpCodeHandle (OptionsOpCodeHandle);
    }
 
    return Status;
}

/**
  Callback function when user presses "Commit Changes and Exit" in Change Priority Order.

  @param[in]  IfrNvData          The IFR nv data.

  @retval EFI_OUT_OF_RESOURCES   Does not have sufficient resources to finish this
                                 operation.
  @retval EFI_NOT_FOUND          Cannot find the corresponding variable.
  @retval EFI_SUCCESS            The operation is completed successfully.

**/
EFI_STATUS
IScsiConfigChangePriority (
    IN ISCSI_CONFIG_IFR_NVDATA  *IfrNvData
)
{
    EFI_STATUS                    Status = EFI_SUCCESS;
    UINT8                         Index;
    UINTN                         Type;
    UINT8                         NewIndex = 0;
    UINT8                         AttemptConfigIndex;
    ISCSI_ATTEMPT_CONFIG_NVDATA   *AttemptConfigData;
    UINT8                         *AttemptConfigOrder;
    UINT8                         *AttemptConfigOrderTmp;
    UINTN                         AttemptConfigOrderSize;
    UINT8                         *PriorityOrder;
    UINTN                         PriorityOrderSize;
     
    AttemptConfigOrder = IScsiGetVariableAndSize (
                           L"AttemptOrder",
                           &gIScsiConfigGuid,
                           &AttemptConfigOrderSize
                           );
    if (AttemptConfigOrder == NULL) {
      return EFI_NOT_FOUND;
    }

    AttemptConfigOrderTmp = AllocateZeroPool (AttemptConfigOrderSize);
    if (AttemptConfigOrderTmp == NULL) {
      Status = EFI_OUT_OF_RESOURCES;
      goto Exit;
    }
    
    PriorityOrder = AllocateZeroPool (ISCSI_MAX_ATTEMPT_TYPE);
        if (PriorityOrder == NULL) {
          Status = EFI_OUT_OF_RESOURCES;
          goto Exit;
        }
    PriorityOrderSize   = ISCSI_MAX_ATTEMPT_TYPE;
    for (Type = 0; Type < ISCSI_MAX_ATTEMPT_TYPE; Type++) {
        //
        // The real content ends with 0.
        //
        if (IfrNvData->PriorityOrderedList[Type] == 0) {
            break;
        }
        for (Index = 0; Index < AttemptConfigOrderSize; Index ++) {
            
            AttemptConfigIndex = AttemptConfigOrder[Index];
            AttemptConfigData  = IScsiConfigGetAttemptByConfigIndex (AttemptConfigIndex);
            if (AttemptConfigData == NULL) {
                continue;
            }
            if (AttemptConfigData->AttemptType == IfrNvData->PriorityOrderedList[Type]) {
                //
                // Reorder the Attempt List.
                //
                RemoveEntryList (&AttemptConfigData->Link);
                InsertTailList (&mPrivate->AttemptConfigs, &AttemptConfigData->Link);
       
                AttemptConfigOrderTmp[NewIndex] = AttemptConfigIndex;
                NewIndex++;
                //
                // Mark it to be deleted - 0.
                //
                AttemptConfigOrder[Index] = 0;
            }
        }
        PriorityOrder[Type] = IfrNvData->PriorityOrderedList[Type];
    }
    //
    // Adjust the attempt order in NVR.
    //
    for (; NewIndex < AttemptConfigOrderSize / sizeof (UINT8); NewIndex++) {
        for (Index = 0; Index < AttemptConfigOrderSize / sizeof (UINT8); Index++) {
            if (AttemptConfigOrder[Index] != 0) {
                AttemptConfigOrderTmp[NewIndex] = AttemptConfigOrder[Index];
                AttemptConfigOrder[Index]   = 0;
                continue;
            }
        }
    }
    Status = gRT->SetVariable (
                              L"PriorityOrder",
                              &gIScsiConfigGuid,
                              EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_NON_VOLATILE,
                              PriorityOrderSize,
                              PriorityOrder
                              );

    FreePool (PriorityOrder);
    if (EFI_ERROR (Status)) {
        return Status;
    }
    Status = gRT->SetVariable (
                    L"AttemptOrder",
                    &gIScsiConfigGuid,
                    EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_NON_VOLATILE,
                    AttemptConfigOrderSize,
                    AttemptConfigOrderTmp
                    );

    Exit:
      if (AttemptConfigOrderTmp != NULL) {
          FreePool (AttemptConfigOrderTmp);
      }

    FreePool (AttemptConfigOrder);
    return Status;
}

#if (ISCSI_FORCEFULLY_SET_INITIATOR_NAME)
/**
  Default hook that returns EFI_UNSUPPORTED so that Add an Attempt, Delete Attempt 
  and Change Attempt Order remains grayed-out when there's no INIT NAME override.
  @param[in]  VOID
  @param[out] EFI_UNSUPPORTED       Default return value.

**/

EFI_STATUS
DefaultIscsiInitiatorNameHook (
    OUT CHAR8  *Buffer
)
{
    return EFI_UNSUPPORTED;
}

/**
  Function to forcefully set / get initiator name.

  @param[in, out]  BufferSize    Size of the buffer in bytes pointed to by Buffer /
                                 Actual size of the variable data buffer.
  @param[out]      Buffer        Pointer to the buffer for data to be read.
                                 The data is a null-terminated UTF-8 encoded string.
                                 The maximum length is 223 characters, including the null-terminator.
  @param[in]       BOOLEAN       BOOLEAN to verify whether it is Form init or get/set.

**/
VOID 
ForcefullySetInitiatorName (
    IN OUT UINTN    *BufferSize,
    IN OUT VOID     *Buffer,
    IN     BOOLEAN  FormInit
) 
{ 
    if (FormInit) {
        EFI_STATUS    Status;
        CHAR8         Buffer[ISCSI_NAME_MAX_SIZE] = {"0"};
        Status = DefaultIscsiInitiatorNameHook(Buffer);
        if (!EFI_ERROR(Status) && Buffer[0] != '\0') {
            // Check Whether the name is in IQN format or not.
            Status = IScsiNormalizeName(Buffer, AsciiStrLen(Buffer));
            if (!EFI_ERROR(Status)) {
                //
                // Copy the Initiator name to the variable and use it in 
                // IScsiGetInitiatorName()/IScsiSetInitiatorName().
                //
                AsciiStrCpy(gForcedInitiatorName,Buffer);
                gIsInitiatorNameForceSet = TRUE;
            }
        }
    }
    else {
        if (gIsInitiatorNameForceSet) {
            AsciiStrCpy((CHAR8*)Buffer,gForcedInitiatorName);
            *BufferSize = AsciiStrLen(gForcedInitiatorName);  // BufferSize here is excluding NULL character.
        }
    }
}
#endif

/**
  Function to change Attempt Order with Priority.

  @param[in]  IfrNvData          The IFR nv data.

  @retval EFI_OUT_OF_RESOURCES   Does not have sufficient resources to finish this
                                 operation.
  @retval EFI_NOT_FOUND          Cannot find the corresponding variable.
  @retval EFI_SUCCESS            The operation is completed successfully.

**/
EFI_STATUS
OrderAttemptsWithPriority (
    IN ISCSI_CONFIG_IFR_NVDATA  *IfrNvData
)
{
    EFI_STATUS                  Status;
    UINTN                       Type;
    UINTN                       Indexj;
    UINTN                       NewIndex = 0;
    UINT8                       *PriorityOrder;
    UINTN                       PriorityOrderSize;
    UINT8                       Index;
    UINT8                       AttemptConfigIndex;
    ISCSI_ATTEMPT_CONFIG_NVDATA *AttemptConfigData;
    UINT8                       *AttemptConfigOrder;
    UINT8                       *AttemptConfigOrderTmp;
    UINTN                       AttemptConfigOrderSize;
    
    AttemptConfigOrder = IScsiGetVariableAndSize (
                             L"AttemptOrder",
                             &gIScsiConfigGuid,
                             &AttemptConfigOrderSize
                             );
    if (AttemptConfigOrder == NULL) {
        return EFI_NOT_FOUND;
    }

    AttemptConfigOrderTmp = AllocateZeroPool (AttemptConfigOrderSize);
    if (AttemptConfigOrderTmp == NULL) {
        Status = EFI_OUT_OF_RESOURCES;
        goto Exit;
    }
    PriorityOrder = IScsiGetVariableAndSize (
                                           L"PriorityOrder",
                                           &gIScsiConfigGuid,
                                           &PriorityOrderSize
                                           );
    if ((PriorityOrder == NULL) && (PriorityOrderSize == 0)) {
        Status = IScsiSetDefaultPriority();
        if (EFI_ERROR(Status)) {
            return Status;
        }
        PriorityOrder = IScsiGetVariableAndSize (
                                             L"PriorityOrder",
                                             &gIScsiConfigGuid,
                                             &PriorityOrderSize
                                             );
    }
    for (Type = 0; Type < ISCSI_MAX_ATTEMPT_TYPE; Type++) {
        for (Index = 0; Index < AttemptConfigOrderSize; Index++) {
            if (PriorityOrder[Type] == SETUP_ATTEMPT) {
                //
                // The real content ends with 0.
                //
                if (IfrNvData->DynamicOrderedList[Index] == 0) {
                    break;
                }

                AttemptConfigIndex = IfrNvData->DynamicOrderedList[Index];
                AttemptConfigData  = IScsiConfigGetAttemptByConfigIndex (AttemptConfigIndex);
                if (AttemptConfigData == NULL) {
                    Status = EFI_NOT_FOUND;
                    goto Exit;
                }
            }
            else {
                AttemptConfigIndex = AttemptConfigOrder[Index];
                AttemptConfigData  = IScsiConfigGetAttemptByConfigIndex (AttemptConfigIndex);
                if (AttemptConfigData == NULL) {
                    continue;
                }
            }
            if (AttemptConfigData->AttemptType == PriorityOrder[Type] ) {   
                RemoveEntryList (&AttemptConfigData->Link);
                InsertTailList (&mPrivate->AttemptConfigs, &AttemptConfigData->Link);
                AttemptConfigOrderTmp[NewIndex] = AttemptConfigIndex;
                NewIndex ++;
                for (Indexj = 0; Indexj < AttemptConfigOrderSize / sizeof (UINT8); Indexj++) {
                    if (AttemptConfigOrder[Indexj] == AttemptConfigIndex) {
                        AttemptConfigOrder[Indexj] = 0;
                        break;
                    }
                }
            }
        }
    }
    for (; NewIndex < AttemptConfigOrderSize / sizeof (UINT8); NewIndex++) {
        for (Indexj = 0; Indexj < AttemptConfigOrderSize / sizeof (UINT8); Indexj++) {
            if (AttemptConfigOrder[Indexj] != 0) {
                AttemptConfigOrderTmp[NewIndex] = AttemptConfigOrder[Indexj];
                AttemptConfigOrder[Indexj]   = 0;
                continue;
            }
        }
    }
    Status = gRT->SetVariable (
                      L"AttemptOrder",
                      &gIScsiConfigGuid,
                      EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_NON_VOLATILE,
                      AttemptConfigOrderSize,
                      AttemptConfigOrderTmp
                      );

    Exit:
        if (AttemptConfigOrderTmp != NULL) {
            FreePool (AttemptConfigOrderTmp);
        }
        FreePool (AttemptConfigOrder);
        
    return Status;
}

/**
  This function is called to provide results data to the driver.
  This data consists of a unique key that is used to identify
  which data is either being passed back or being asked for.

  @param[in]       This          Points to the EFI_HII_CONFIG_ACCESS_PROTOCOL.
  @param[in]       Action        Specifies the type of action taken by the browser.
  @param[in]       QuestionId    A unique value which is sent to the original
                                 exporting driver so that it can identify the type
                                 of data to expect. The format of the data tends to
                                 vary based on the opcode that generated the callback.
  @param[in]       Type          The type of value for the question.
  @param[in, out]  Value         A pointer to the data being sent to the original
                                 exporting driver.
  @param[out]      ActionRequest On return, points to the action requested by the
                                 callback function.

  @retval EFI_SUCCESS            The callback successfully handled the action.
  @retval EFI_OUT_OF_RESOURCES   Not enough storage is available to hold the
                                 variable and its data.
  @retval EFI_DEVICE_ERROR       The variable could not be saved.
  @retval EFI_UNSUPPORTED        The specified Action is not supported by the
                                 callback.
**/
EFI_STATUS
EFIAPI
AmiIScsiFormCallback (
    IN CONST  EFI_HII_CONFIG_ACCESS_PROTOCOL   *This,
    IN        EFI_BROWSER_ACTION               Action,
    IN        EFI_QUESTION_ID                  QuestionId,
    IN        UINT8                            Type,
    IN OUT    EFI_IFR_TYPE_VALUE               *Value,
    OUT       EFI_BROWSER_ACTION_REQUEST       *ActionRequest
)
{
    ISCSI_FORM_CALLBACK_INFO    *Private;
    UINTN                       BufferSize;
    CHAR8                       *IScsiName;
    CHAR8                       IpString[ISCSI_NAME_MAX_SIZE];
    CHAR8                       LunString[ISCSI_LUN_STR_MAX_LEN];   
    UINT64                      Lun;
    EFI_IP_ADDRESS              HostIp;
    EFI_IP_ADDRESS              SubnetMask;
    EFI_IP_ADDRESS              Gateway;
    ISCSI_CONFIG_IFR_NVDATA     *IfrNvData;
    ISCSI_CONFIG_IFR_NVDATA     OldIfrNvData;
    EFI_STATUS                  Status;
    CHAR16                      UnicodeIsIdString[ISID_CONFIGURABLE_STORAGE]={0};
    CHAR16                      *CharPtr;
    CHAR16                      *ChapStatusStr;  
    EFI_INPUT_KEY               Key;
    ISCSI_NIC_INFO              *NicInfo;

    NicInfo = NULL;

    PopUpTitle=NetLibHiiGetString(mCallbackInfo->RegisteredHandle,STRING_TOKEN(STR_MSGBOX_TITLE),NULL);
    ChapStatusStr=NULL;
    if ((Action == EFI_BROWSER_ACTION_FORM_OPEN) || (Action == EFI_BROWSER_ACTION_FORM_CLOSE)) {
      //
      // Do nothing for UEFI OPEN/CLOSE Action
      //
#if( RSD_ISCSI_SUPPORT )
        if((Action == EFI_BROWSER_ACTION_FORM_OPEN) && (QuestionId == KEY_RSD_UPDATE_ATTEMPT_LIST)) {
            Private = ISCSI_FORM_CALLBACK_INFO_FROM_FORM_CALLBACK (This);
            RsdIscsiAttemptsList(Private);
        }
#endif
            
        
      return EFI_SUCCESS;
    }

    if ((Action != EFI_BROWSER_ACTION_CHANGING) && (Action != EFI_BROWSER_ACTION_CHANGED)) {
        //
        // All other type return unsupported.
        //
        return EFI_UNSUPPORTED;
    }

    if ((Value == NULL) || (ActionRequest == NULL)) {
        return EFI_INVALID_PARAMETER;
    }

    Private = ISCSI_FORM_CALLBACK_INFO_FROM_FORM_CALLBACK (This);

    //
    // Retrieve uncommitted data from Browser
    //

    BufferSize = sizeof (ISCSI_CONFIG_IFR_NVDATA);
    IfrNvData = AllocateZeroPool (BufferSize);
    if (IfrNvData == NULL) {
        return EFI_OUT_OF_RESOURCES;
    }

    IScsiName = (CHAR8 *) AllocateZeroPool (ISCSI_NAME_MAX_SIZE);
    if (IScsiName == NULL) {
        FreePool (IfrNvData);
        return EFI_OUT_OF_RESOURCES;
    }

    Status = EFI_SUCCESS;

    ZeroMem (&OldIfrNvData, BufferSize); 
   
    HiiGetBrowserData (&gIScsiConfigGuid, L"ISCSI_CONFIG_IFR_NVDATA", BufferSize, (UINT8 *) IfrNvData);    
    CopyMem (&OldIfrNvData, IfrNvData, BufferSize);

    if (Action == EFI_BROWSER_ACTION_CHANGING) {
#if(RSD_ISCSI_SUPPORT)
      if(QuestionId >= KEY_RSD_UPDATE_ATTEMPT_BASE && QuestionId <=KEY_RSD_UPDATE_ATTEMPT_BASE+10) {  
          RsdIscsiAttemptDetails(Private,QuestionId);
          return EFI_SUCCESS; 
      }
#endif
              
      switch (QuestionId) {    
      case KEY_ADD_ATTEMPT:
          //
          // Check whether iSCSI initiator name is configured already.
          //
          mPrivate->InitiatorNameLength = ISCSI_NAME_MAX_SIZE;
          Status = gIScsiInitiatorName.Get (
                                       &gIScsiInitiatorName,
                                       &mPrivate->InitiatorNameLength,
                                       mPrivate->InitiatorName
                                       );

          if(EFI_ERROR(Status)) {
              CharPtr=NetLibHiiGetString(mCallbackInfo->RegisteredHandle,STRING_TOKEN(STR_NO_INITIATOR_NAME),NULL);
              AMICreatePopUpEx (PopUpTitle,CharPtr);
              if(CharPtr != NULL)
              FreePool (CharPtr);
              break;
          }  
        
          Status = AmiIScsiConfigAddAttempt();
          break;

      case KEY_DELETE_ATTEMPT:    
          // Taking Backup of both DynamicOrderedList and DeleteAttempts List
          CopyMem (
          OldIfrNvData.DynamicOrderedList,
          IfrNvData->DynamicOrderedList,
          sizeof (IfrNvData->DeleteAttemptList)+sizeof (IfrNvData->DynamicOrderedList)
          );
     
          Status = IScsiConfigDisplayDeleteAttempts (IfrNvData);
          break;

      case KEY_ORDER_ATTEMPT_CONFIG:
          //
          // Order the attempt according to user input.
          //
          CopyMem (
          OldIfrNvData.DynamicOrderedList,
          IfrNvData->DynamicOrderedList,
          sizeof (IfrNvData->DynamicOrderedList)
          );
          AmiIScsiConfigDisplayOrderAttempts (IfrNvData);
          break;    
      case KEY_PRIORITY_CHANGE_CONFIG:
          //
          // Change the Priority according to user input.
          //
          IScsiConfigDisplayPriorityList(IfrNvData);
          break;
     
      default:
          Status = AmiIScsiConfigProcessDefault (QuestionId, IfrNvData);     
          if((Status == EFI_SUCCESS) && (QuestionId != KEY_INITIATOR_NAME) ) {         
              if (QuestionId == KEY_SAVE_DELETE_ATTEMPT || QuestionId == KEY_IGNORE_DELETE_ATTEMPT)
                  break; 
              if (QuestionId == KEY_SAVE_ORDER_CHANGES  || QuestionId == KEY_IGNORE_ORDER_CHANGES ) 
                  break;
              if(QuestionId == KEY_SAVE_PRIORITY_CHANGES)
                  break; 
                
              // Convert ISID to Unicode string format and display in setup by setting token value using HiiSetString() function.
              //
              IScsiConvertIsIdToString (UnicodeIsIdString, Private->Current->SessionConfigData.IsId);

              HiiSetString( Private->RegisteredHandle, STR_ISCSI_OUI_FROMAT_ISID_VALUE, UnicodeIsIdString, NULL);
                
              //
              // Check CHAP secret and Reverse CHAP Secret and update status with "Installed" or "Not Installed".
              //
              // Update the status of the CHAP secret: Instlled or Not Installed
              //

              if (( (StrLen (IfrNvData->CHAPSecret) >= ISCSI_CHAP_SECRET_MIN_LEN) && \
                   (StrLen (IfrNvData->CHAPSecret) <= ISCSI_CHAP_SECRET_MAX_LEN) ) == TRUE ) {
                  CharPtr=NetLibHiiGetString(Private->RegisteredHandle,STRING_TOKEN(STR_CHAP_STATUS_INSTALLED),NULL);
                  HiiSetString( Private->RegisteredHandle, STR_ISCSI_CHAP_STATUS, CharPtr , NULL);
                  if(CharPtr != NULL)
                      FreePool (CharPtr);
              } else {
                  CharPtr=NetLibHiiGetString(Private->RegisteredHandle,STRING_TOKEN(STR_CHAP_STATUS_NOT_INSTALLED),NULL);
                  HiiSetString( Private->RegisteredHandle, STR_ISCSI_CHAP_STATUS, CharPtr , NULL);
                  if(CharPtr != NULL)
                      FreePool (CharPtr);
              }
              //
              // Update the status of the Reverse CHAP secret: Instlled or Not Installed
              //
              if ( ( (StrLen (IfrNvData->ReverseCHAPSecret) >= ISCSI_CHAP_SECRET_MIN_LEN) && \
                  (StrLen (IfrNvData->ReverseCHAPSecret) <= ISCSI_CHAP_SECRET_MAX_LEN) ) == TRUE ) {
                   CharPtr=NetLibHiiGetString(Private->RegisteredHandle,STRING_TOKEN(STR_CHAP_STATUS_INSTALLED),NULL);
                   HiiSetString( Private->RegisteredHandle, STR_ISCSI_REVERSE_CHAP_STATUS, CharPtr , NULL);
                   if(CharPtr != NULL)
                       FreePool (CharPtr);
                   } else {
                       CharPtr=NetLibHiiGetString(Private->RegisteredHandle,STRING_TOKEN(STR_CHAP_STATUS_NOT_INSTALLED),NULL);
                       HiiSetString( Private->RegisteredHandle, STR_ISCSI_REVERSE_CHAP_STATUS, CharPtr , NULL);
                       if(CharPtr != NULL)
                           FreePool (CharPtr);
                  }
          }      
          break;
      }
    } else if (Action == EFI_BROWSER_ACTION_CHANGED) {
        switch (QuestionId) {
            case KEY_INITIATOR_NAME:
                UnicodeStrToAsciiStrS (IfrNvData->InitiatorName, IScsiName, ISCSI_NAME_MAX_SIZE);
                BufferSize  = AsciiStrSize (IScsiName);

                Status      = gIScsiInitiatorName.Set (&gIScsiInitiatorName, &BufferSize, IScsiName);
                if (EFI_ERROR (Status)) {
                    CharPtr=NetLibHiiGetString(mCallbackInfo->RegisteredHandle,STRING_TOKEN(STR_INVALID_ISCSI_NAME),NULL);
                    AMICreatePopUpEx (PopUpTitle,CharPtr);
                    if (CharPtr != NULL)
                        FreePool (CharPtr);
                    // Clear the invalid Iscsi initiator name saved in Ifr data and set with 
                    // valid iscsi name. Set the flag IsInitiatorNameValid whether FALSE(Not valid) or TRUE (valid) depending upon Iscsi Name entered.
                    //
                    //
                    // Clear Invalid data and update with last entered valid Input
                    //
                    ZeroMem (IfrNvData->InitiatorName, sizeof (CHAR16)*ISCSI_NAME_MAX_SIZE);
                    //
                    // Get the last entered valid initiator name and update.
                    //
                    BufferSize    = ISCSI_NAME_MAX_SIZE;
                    ZeroMem (IScsiName, BufferSize);
                    if ( !EFI_ERROR (gIScsiInitiatorName.Get (&gIScsiInitiatorName, &BufferSize, IScsiName)) ) {
                        AsciiStrToUnicodeStrS (IScsiName, IfrNvData->InitiatorName, 
                                sizeof(IfrNvData->InitiatorName) / sizeof(IfrNvData->InitiatorName[0]));
                    }
                }
                //
                // Validity of Initiator name has already been verified.
                // So update Flag based on the size of InitiatorName.
                //
                if ( !StrLen (IfrNvData->InitiatorName) ) {
                    //
                    // Update flag for invalid Initiator name as FALSE
                    //
                    IfrNvData->IsInitiatorNameValid = FALSE;
                } else {
                    //
                    // Update flag for Valid Initiator name as TRUE
                    //
                    IfrNvData->IsInitiatorNameValid = TRUE;
                }
                //
                // Set Status as EFI_SUCCESS to update IfrNvData in Form browser data
                //
                Status = EFI_SUCCESS;
                //
                // Clear the invalid Iscsi initiator name saved in Ifr data and set with 
                // valid iscsi name. Set the flag IsInitiatorNameValid whether FALSE(Not valid) or TRUE (valid) 
                // depending upon Iscsi Name entered.
                *ActionRequest = EFI_BROWSER_ACTION_REQUEST_FORM_APPLY;
                break;

            case KEY_SAVE_ATTEMPT_CONFIG:
                Status = AmiIScsiConvertIfrNvDataToAttemptConfigData (IfrNvData, Private->Current);
                if (EFI_ERROR (Status)) {
                    break;
                }

                *ActionRequest = EFI_BROWSER_ACTION_REQUEST_FORM_SUBMIT_EXIT;
                break;

            case KEY_SAVE_ORDER_CHANGES:
                //
                // Sync the Attempt Order to NVR.
                //
                Status = OrderAttemptsWithPriority (IfrNvData);
                if (EFI_ERROR (Status)) {
                    break;
                }

                IScsiConfigUpdateAttempt ();
                *ActionRequest = EFI_BROWSER_ACTION_REQUEST_FORM_SUBMIT_EXIT;
                break;     
            case KEY_SAVE_PRIORITY_CHANGES:
                //
                // Sync the Attempt Order to NVR based on priority.
                //
                Status = IScsiConfigChangePriority (IfrNvData);
                if (EFI_ERROR (Status)) {
                    break;
                }

                IScsiConfigUpdateAttempt ();
                *ActionRequest = EFI_BROWSER_ACTION_REQUEST_FORM_SUBMIT_EXIT;
                break;        
            case KEY_IGNORE_ORDER_CHANGES:
                CopyMem (
                   IfrNvData->DynamicOrderedList,
                   OldIfrNvData.DynamicOrderedList,
                   sizeof (IfrNvData->DynamicOrderedList)
                   );
                *ActionRequest = EFI_BROWSER_ACTION_REQUEST_FORM_DISCARD_EXIT;
                break;

            case KEY_SAVE_DELETE_ATTEMPT:
                //
                // Delete the Attempt Order from NVR
                //
                Status = IScsiConfigDeleteAttempts (IfrNvData);
                if (EFI_ERROR (Status)) {
                    break;
                }

                IScsiConfigUpdateAttempt ();
                *ActionRequest = EFI_BROWSER_ACTION_REQUEST_FORM_SUBMIT_EXIT;
                break;

            case KEY_IGNORE_DELETE_ATTEMPT:    
                // Restoring both DynamicOrderedList and DeleteAttempts List
                CopyMem (
                    IfrNvData->DynamicOrderedList,
                    OldIfrNvData.DynamicOrderedList,
                    sizeof (IfrNvData->DeleteAttemptList)+sizeof (IfrNvData->DynamicOrderedList)
                    );
               // Restoring both DynamicOrderedList and DeleteAttempts List   
               *ActionRequest = EFI_BROWSER_ACTION_REQUEST_FORM_DISCARD_EXIT;
               break;

           case KEY_IP_MODE:
               switch (Value->u8) {
                   case IP_MODE_IP6:
                       NicInfo = IScsiGetNicInfoByIndex (Private->Current->NicIndex);
                       if(NicInfo == NULL) {
                           break;
                       }

                       if(!NicInfo->Ipv6Available) {
                           //
                           // Current NIC doesn't Support IPv6, hence use IPv4.
                           //
                           IfrNvData->IpMode = IP_MODE_IP4;

                           CreatePopUp (
                           EFI_LIGHTGRAY | EFI_BACKGROUND_BLUE,
                           &Key,
                           L"Current NIC doesn't Support IPv6!",
                           NULL
                           );
                       }

                   case IP_MODE_IP4:
                       ZeroMem (IfrNvData->LocalIp, sizeof (IfrNvData->LocalIp));
                       ZeroMem (IfrNvData->SubnetMask, sizeof (IfrNvData->SubnetMask));
                       ZeroMem (IfrNvData->Gateway, sizeof (IfrNvData->Gateway));
                       ZeroMem (IfrNvData->TargetIp, sizeof (IfrNvData->TargetIp));
                       Private->Current->AutoConfigureMode = 0;
                       ZeroMem (&Private->Current->SessionConfigData.LocalIp, sizeof (EFI_IP_ADDRESS));
                       ZeroMem (&Private->Current->SessionConfigData.SubnetMask, sizeof (EFI_IPv4_ADDRESS));
                       ZeroMem (&Private->Current->SessionConfigData.Gateway, sizeof (EFI_IP_ADDRESS));
                       ZeroMem (&Private->Current->SessionConfigData.TargetIp, sizeof (EFI_IP_ADDRESS));

                       break;
               }

               break;

          case KEY_LOCAL_IP:   
              // Check for in valid characters in Ip Address. If found, display error message.
              //
              Status = CheckInvalidCharinIpAddress(IfrNvData->LocalIp, IfrNvData->IpMode);
              if ( EFI_ERROR (Status) ) {
                  CharPtr=NetLibHiiGetString(mCallbackInfo->RegisteredHandle,STRING_TOKEN(STR_INVALID_IP_ADDRESS),NULL);
                  AMICreatePopUpEx (PopUpTitle,CharPtr);
                  if(CharPtr != NULL)
                      FreePool (CharPtr);
                  // Clear invalid data and update with last entered valid input
                  //
                  ZeroMem (IfrNvData->LocalIp, sizeof (IfrNvData->LocalIp));
                  //
                  // Changed the type cast as it is causing the build error with tool chain VS2008.
                  //
                  IScsiIpToStr ((VOID*)&Private->Current->SessionConfigData.LocalIp, FALSE, IfrNvData->LocalIp);   
                  //
                  // Changed the type cast as it is causing the build error with tool chain VS2008.
                  //
                  //
                  // Set Status as EFI_SUCCESS to update IfrNvData in Form browser data
                  //
                  Status = EFI_SUCCESS;
                  break;
              }

              Status = NetLibStrToIp4 (IfrNvData->LocalIp, &HostIp.v4);
              if (EFI_ERROR (Status) || 
                  ((Private->Current->SessionConfigData.SubnetMask.Addr[0] != 0) && 
                    !NetIp4IsUnicast (NTOHL (HostIp.Addr[0]), NTOHL(*(UINT32*)Private->Current->SessionConfigData.SubnetMask.Addr)))) {       
                   CharPtr=NetLibHiiGetString(mCallbackInfo->RegisteredHandle,STRING_TOKEN(STR_INVALID_IP_ADDRESS),NULL);
                   AMICreatePopUpEx (PopUpTitle,CharPtr);
                   if(CharPtr != NULL)
                       FreePool (CharPtr);
                   // Clear invalid data and update with last entered valid input
                   //
                   ZeroMem (IfrNvData->LocalIp, sizeof (IfrNvData->LocalIp));
                   //
                   // Changed the type cast as it is causing the build error with tool chain VS2008.
                   //
                   IScsiIpToStr ((VOID*)&Private->Current->SessionConfigData.LocalIp, FALSE, IfrNvData->LocalIp);
                   //
                   //  Changed the type cast as it is causing the build error with tool chain VS2008.
                   //
                   //
                   // Set Status as EFI_SUCCESS to update IfrNvData in Form browser data
                   //
                   Status = EFI_SUCCESS;
              } else {
                  CopyMem (&Private->Current->SessionConfigData.LocalIp, &HostIp.v4, sizeof (HostIp.v4));
              }

              break;

          case KEY_SUBNET_MASK:   
              // Check for in valid characters in Ip Address. If found, display error message.
              //
              Status = CheckInvalidCharinIpAddress(IfrNvData->SubnetMask, IfrNvData->IpMode);
              if ( EFI_ERROR (Status) ) {
                  CharPtr=NetLibHiiGetString(mCallbackInfo->RegisteredHandle,STRING_TOKEN(STR_INVALID_SUBNETMASK),NULL);
                  AMICreatePopUpEx (PopUpTitle,CharPtr);
                  if(CharPtr != NULL)
                      FreePool (CharPtr);
                  // Clear invalid data and update with last entered valid input
                  //
                  ZeroMem (IfrNvData->SubnetMask, sizeof (IfrNvData->SubnetMask));
                  //
                  // Changed the type cast as it is causing the build error with tool chain VS2008.
                  //
                  IScsiIpToStr ((VOID*)&Private->Current->SessionConfigData.SubnetMask, FALSE, IfrNvData->SubnetMask);   
                  //
                  // Changed the type cast as it is causing the build error with tool chain VS2008.
                  //
                  //
                  // Set Status as EFI_SUCCESS to update IfrNvData in Form browser data
                  //
                  Status = EFI_SUCCESS;
                  break;
              }

              Status = NetLibStrToIp4 (IfrNvData->SubnetMask, &SubnetMask.v4);
              if (EFI_ERROR (Status) || ((SubnetMask.Addr[0] != 0) && (IScsiGetSubnetMaskPrefixLength (&SubnetMask.v4) == 0))) {        
                   CharPtr=NetLibHiiGetString(mCallbackInfo->RegisteredHandle,STRING_TOKEN(STR_INVALID_SUBNETMASK),NULL);
                   AMICreatePopUpEx (PopUpTitle,CharPtr);
                   if(CharPtr != NULL)
                       FreePool (CharPtr);
                   //
                   // Clear invalid data and update with last entered valid input
                   //
                   ZeroMem (IfrNvData->SubnetMask, sizeof (IfrNvData->SubnetMask));
                   //
                   // Changed the type cast as it is causing the build error with tool chain VS2008.
                   //
                   IScsiIpToStr ((VOID*)&Private->Current->SessionConfigData.SubnetMask, FALSE, IfrNvData->SubnetMask);
                   //
                   // Changed the type cast as it is causing the build error with tool chain VS2008.
                   //
                   //
                   // Set Status as EFI_SUCCESS to update IfrNvData in Form browser data
                   //
                   Status = EFI_SUCCESS;      
              } else {
                   CopyMem (&Private->Current->SessionConfigData.SubnetMask, &SubnetMask.v4, sizeof (SubnetMask.v4));
              }

              break;

          case KEY_GATE_WAY:    
              // Check for in valid characters in Ip Address. If found, display error message.
              Status = CheckInvalidCharinIpAddress(IfrNvData->Gateway, IfrNvData->IpMode);
              if ( EFI_ERROR (Status) ) {
                  CharPtr=NetLibHiiGetString(mCallbackInfo->RegisteredHandle,STRING_TOKEN(STR_INVALID_GATEWAY),NULL);
                  AMICreatePopUpEx (PopUpTitle,CharPtr);
                  if(CharPtr != NULL)
                      FreePool (CharPtr);
                  //
                  // Clear invalid data and update with last entered valid input
                  //
                  ZeroMem (IfrNvData->Gateway, sizeof (IfrNvData->Gateway));
                  //
                  // Changed the type cast as it is causing the build error with tool chain VS2008.
                  //
                  IScsiIpToStr ((VOID*)&Private->Current->SessionConfigData.Gateway, FALSE, IfrNvData->Gateway);   
                  //
                  // Changed the type cast as it is causing the build error with tool chain VS2008.
                  //
                  //
                  // Set Status as EFI_SUCCESS to update IfrNvData in Form browser data
                  //
                  Status = EFI_SUCCESS;
                  break;
              }

              Status = NetLibStrToIp4 (IfrNvData->Gateway, &Gateway.v4);
              if (EFI_ERROR (Status) || 
                  ((Gateway.Addr[0] != 0) && 
                  (Private->Current->SessionConfigData.SubnetMask.Addr[0] != 0) && 
                  !NetIp4IsUnicast (NTOHL (Gateway.Addr[0]), NTOHL(*(UINT32*)Private->Current->SessionConfigData.SubnetMask.Addr)))) {
                  CharPtr=NetLibHiiGetString(mCallbackInfo->RegisteredHandle,STRING_TOKEN(STR_INVALID_GATEWAY),NULL);
                  AMICreatePopUpEx (PopUpTitle,CharPtr);
                  if(CharPtr != NULL)
                      FreePool (CharPtr);
                  // Clear invalid data and update with last entered valid input
                  //
                  ZeroMem (IfrNvData->Gateway, sizeof (IfrNvData->Gateway));
                  //
                  // Changed the type cast as it is causing the build error with tool chain VS2008.
                  //
                  IScsiIpToStr ((VOID*)&Private->Current->SessionConfigData.Gateway, FALSE, IfrNvData->Gateway);
                  //
                  // Changed the type cast as it is causing the build error with tool chain VS2008.
                  //
                  //
                  // Set Status as EFI_SUCCESS to update IfrNvData in Form browser data
                  //
                  Status = EFI_SUCCESS;
      
              } else {
                  CopyMem (&Private->Current->SessionConfigData.Gateway, &Gateway.v4, sizeof (Gateway.v4));
              }

              break;

          case KEY_TARGET_IP:    
        
              if(IfrNvData->Enabled == ISCSI_DISABLED) {
                  Status = EFI_SUCCESS;
                  break;
              }
   

              UnicodeStrToAsciiStrS (IfrNvData->TargetIp, IpString, sizeof (IpString));
              Status = IScsiAsciiStrToIp (IpString, IfrNvData->IpMode, &HostIp);
              if (EFI_ERROR (Status) || !IpIsUnicast (&HostIp, IfrNvData->IpMode)) {
                  //
                  // The target is expressed in URL format or an invalid Ip address, just save.
                  //
                  Private->Current->SessionConfigData.DnsMode = TRUE;
                  ZeroMem (&Private->Current->SessionConfigData.TargetIp, sizeof (Private->Current->SessionConfigData.TargetIp));
                  UnicodeStrToAsciiStrS (IfrNvData->TargetIp, Private->Current->SessionConfigData.TargetUrl, ISCSI_NAME_MAX_SIZE);
              } else {
                  Private->Current->SessionConfigData.DnsMode = FALSE;        
                  //
                  //  Clearing target ip variable, copying target ip address based on ip address type
                  //
                  ZeroMem ( (UINT8 *)&Private->Current->SessionConfigData.TargetIp, sizeof(EFI_IP_ADDRESS));
                  if ( IfrNvData->IpMode == IP_MODE_IP4 ) {
                      CopyMem (&Private->Current->SessionConfigData.TargetIp, &HostIp.v4, sizeof (HostIp.v4));
                  } else if ( IfrNvData->IpMode == IP_MODE_IP6 ) {
                      CopyMem (&Private->Current->SessionConfigData.TargetIp, &HostIp.v6, sizeof (HostIp.v6));
                  }      
              }

              break;

          case KEY_TARGET_NAME:
              UnicodeStrToAsciiStrS (IfrNvData->TargetName, IScsiName, ISCSI_NAME_MAX_SIZE);
              Status = IScsiNormalizeName (IScsiName, AsciiStrLen (IScsiName));
              if (EFI_ERROR (Status)) {     
                  CharPtr=NetLibHiiGetString(mCallbackInfo->RegisteredHandle,STRING_TOKEN(STR_INVALID_ISCSI_NAME),NULL);
                  AMICreatePopUpEx (PopUpTitle,CharPtr);
                  if(CharPtr != NULL)
                      FreePool (CharPtr);
                  // Clear invalid data and update with last entered valid input
                  ZeroMem (IfrNvData->TargetName, sizeof (CHAR16)*ISCSI_NAME_MAX_SIZE);
                  AsciiStrToUnicodeStrS (Private->Current->SessionConfigData.TargetName, IfrNvData->TargetName, 
                          sizeof(IfrNvData->TargetName)/sizeof(IfrNvData->TargetName[0]));
                  //
                  // Set Status as EFI_SUCCESS to update IfrNvData in Form browser data
                  //
                  Status = EFI_SUCCESS;     
              } else {
                  AsciiStrCpyS (Private->Current->SessionConfigData.TargetName, ISCSI_NAME_MAX_SIZE, IScsiName);
              }

               break;

           case KEY_DHCP_ENABLE:
               if (IfrNvData->InitiatorInfoFromDhcp == 0) {
                   IfrNvData->TargetInfoFromDhcp = 0;
               }

               break;

           case KEY_BOOT_LUN:
               UnicodeStrToAsciiStrS (IfrNvData->BootLun, LunString, sizeof (LunString));
               Status = IScsiAsciiStrToLun (LunString, (UINT8 *) &Lun);
               if (EFI_ERROR (Status)) {      
                   CharPtr=NetLibHiiGetString(mCallbackInfo->RegisteredHandle,STRING_TOKEN(STR_INVALID_LUN),NULL);
                   AMICreatePopUpEx (PopUpTitle,CharPtr);
                   if(CharPtr != NULL)
                       FreePool (CharPtr);
                   // Clear invalid data and update with last entered valid input
                   //
                   ZeroMem (IfrNvData->BootLun, sizeof (IfrNvData->BootLun));
                   IScsiLunToUnicodeStr ((UINT8*)&Private->Current->SessionConfigData.BootLun, IfrNvData->BootLun);
                   //
                   // Set Status as EFI_SUCCESS to update IfrNvData in Form browser data
                   //
                   Status = EFI_SUCCESS;
      
               } else {
                   CopyMem (Private->Current->SessionConfigData.BootLun, &Lun, sizeof (Lun));
               }

               break;

           case KEY_AUTH_TYPE:
               switch (Value->u8) {
                   case ISCSI_AUTH_TYPE_CHAP:      
                       //
                       // Clearing CHAP status to Not Installed
                       //
                       ChapStatusStr = HiiGetString(Private->RegisteredHandle,STR_CHAP_STATUS_NOT_INSTALLED,NULL); 
                       HiiSetString( Private->RegisteredHandle, STR_ISCSI_CHAP_STATUS, ChapStatusStr, NULL);
                       HiiSetString( Private->RegisteredHandle, STR_ISCSI_REVERSE_CHAP_STATUS, ChapStatusStr, NULL);
                       IfrNvData->CHAPType = ISCSI_CHAP_MUTUAL;  // Kept Mutual based on default value in VFR
                       //
                       // Clearing CHAP status to Not Installed
                       //       
                       break;
                   default:
                       break;
               }

               break;

           case KEY_CHAP_NAME:
               UnicodeStrToAsciiStrS (
                   IfrNvData->CHAPName,
                   Private->Current->AuthConfigData.CHAP.CHAPName,
                   sizeof (Private->Current->AuthConfigData.CHAP.CHAPName)
               );
               break;

           case KEY_CHAP_SECRET:    
               //
               // Check CHAP secret and update status with "Installed" or "Not Installed". 
               // Convert the Unicode string to Ascii string format only for valid input
               // Update CHAP Status: Installed or Not Installed.
               //
               if ( ( (StrLen (IfrNvData->CHAPSecret) >= ISCSI_CHAP_SECRET_MIN_LEN) && \
                   (StrLen (IfrNvData->CHAPSecret) <= ISCSI_CHAP_SECRET_MAX_LEN) ) == TRUE ) {
                    UnicodeStrToAsciiStrS (
                         IfrNvData->CHAPSecret,
                         Private->Current->AuthConfigData.CHAP.CHAPSecret,
                         sizeof (Private->Current->AuthConfigData.CHAP.CHAPSecret)
                    );

               CharPtr=NetLibHiiGetString(Private->RegisteredHandle,STRING_TOKEN(STR_CHAP_STATUS_INSTALLED),NULL);
               HiiSetString( Private->RegisteredHandle, STR_ISCSI_CHAP_STATUS, CharPtr , NULL);
               if(CharPtr != NULL)
                   FreePool (CharPtr);
               } else {
                   //
                   // Clear the CHAPSecret in Private data.
                   //
                   ZeroMem (Private->Current->AuthConfigData.CHAP.CHAPSecret, ISCSI_CHAP_SECRET_MAX_LEN);
                   CharPtr=NetLibHiiGetString(Private->RegisteredHandle,STRING_TOKEN(STR_CHAP_STATUS_NOT_INSTALLED),NULL);
                   HiiSetString( Private->RegisteredHandle, STR_ISCSI_CHAP_STATUS, CharPtr , NULL);
                   if(CharPtr != NULL)
                       FreePool (CharPtr);
               }
               //
               // Check CHAP secret and update status with "Installed" or "Not Installed". 
               //      
               break;

           case KEY_REVERSE_CHAP_NAME:
               UnicodeStrToAsciiStrS (
                     IfrNvData->ReverseCHAPName,
                     Private->Current->AuthConfigData.CHAP.ReverseCHAPName,
                     sizeof (Private->Current->AuthConfigData.CHAP.ReverseCHAPName)
               );
               break;

           case KEY_REVERSE_CHAP_SECRET:    
               //
               // Check Reverse CHAP Secret and update status with "Installed" or "Not Installed". 
               // Convert the Unicode string to Ascii string format only for valid input
               // Update Reverse CHAP Status: Installed or Not Installed.
               //
               if ( ( (StrLen (IfrNvData->ReverseCHAPSecret) >= ISCSI_CHAP_SECRET_MIN_LEN) && \
                   (StrLen (IfrNvData->ReverseCHAPSecret) <= ISCSI_CHAP_SECRET_MAX_LEN) ) == TRUE ) {
                   UnicodeStrToAsciiStrS (
                       IfrNvData->ReverseCHAPSecret,
                       Private->Current->AuthConfigData.CHAP.ReverseCHAPSecret,
                       sizeof (Private->Current->AuthConfigData.CHAP.ReverseCHAPSecret)
                   );

                   CharPtr=NetLibHiiGetString(Private->RegisteredHandle,STRING_TOKEN(STR_CHAP_STATUS_INSTALLED),NULL);
                   HiiSetString( Private->RegisteredHandle, STR_ISCSI_REVERSE_CHAP_STATUS, CharPtr , NULL);
                   if(CharPtr != NULL)
                      FreePool (CharPtr);
               } else {
               //
               // Clear the ReverseCHAPSecret in Private data.
               //
               ZeroMem (Private->Current->AuthConfigData.CHAP.ReverseCHAPSecret, ISCSI_CHAP_SECRET_MAX_LEN);
               CharPtr=NetLibHiiGetString(Private->RegisteredHandle,STRING_TOKEN(STR_CHAP_STATUS_NOT_INSTALLED),NULL);
               HiiSetString( Private->RegisteredHandle, STR_ISCSI_REVERSE_CHAP_STATUS, CharPtr , NULL);
               if(CharPtr != NULL)
                   FreePool (CharPtr);
               }
               //
               // Check Reverse CHAP Secret and update status with "Installed" or "Not Installed". 
               //
               break;

           case KEY_CONFIG_ISID:    
               Status = AmiIScsiParseIsIdFromString (IfrNvData->IsId, Private->Current->SessionConfigData.IsId);
               if ( !EFI_ERROR (Status) ) {
                   //
                   // User configured ISID is valid so Convert ISID to Unicode string format and 
                   // display in setup by the setting token value using HiiSetString() function
                   //
                   IScsiConvertIsIdToString (IfrNvData->IsId, Private->Current->SessionConfigData.IsId);
                   HiiSetString( Private->RegisteredHandle, STR_ISCSI_OUI_FROMAT_ISID_VALUE, IfrNvData->IsId, NULL);
               } else {
                   //
                   // User configured ISID is invalid so Convert last valid ISID to Unicode string format and 
                   // save in Ifr Data
                   //
                    IScsiConvertIsIdToString (IfrNvData->IsId, Private->Current->SessionConfigData.IsId);
                   //
                   // Set Status as EFI_SUCCESS to update IfrNvData in Form browser data
                   //
                   Status = EFI_SUCCESS;
              }
      
              break;

          default:
              break;
        }
    }

    if (!EFI_ERROR (Status)) {
        //
        // Pass changed uncommitted data back to Form Browser.
        //
        BufferSize = sizeof (ISCSI_CONFIG_IFR_NVDATA);    
        HiiSetBrowserData (&gIScsiConfigGuid, L"ISCSI_CONFIG_IFR_NVDATA", BufferSize, (UINT8 *) IfrNvData, NULL);
    }

    FreePool (IfrNvData);
    FreePool (IScsiName);

    return Status;   
}

/**
  Convert the IFR data to iSCSI configuration data.

  @param[in]       IfrNvData              Point to ISCSI_CONFIG_IFR_NVDATA.
  @param[in, out]  Attempt                The iSCSI attempt config data.

  @retval EFI_INVALID_PARAMETER  Any input or configured parameter is invalid.
  @retval EFI_NOT_FOUND          Cannot find the corresponding variable.
  @retval EFI_OUT_OF_RESOURCES   The operation is failed due to lack of resources.
  @retval EFI_ABORTED            The operation is aborted.
  @retval EFI_SUCCESS            The operation is completed successfully.

**/
EFI_STATUS
AmiIScsiConvertIfrNvDataToAttemptConfigData (
    IN ISCSI_CONFIG_IFR_NVDATA          *IfrNvData,
    IN OUT ISCSI_ATTEMPT_CONFIG_NVDATA  *Attempt
)
{
    EFI_IP_ADDRESS              HostIp;
    EFI_IP_ADDRESS              SubnetMask;
    EFI_IP_ADDRESS              Gateway;
    CHAR16                      *MacString;
    CHAR16                      *AttemptName1;
    CHAR16                      *AttemptName2;
    ISCSI_ATTEMPT_CONFIG_NVDATA *ExistAttempt;
    ISCSI_ATTEMPT_CONFIG_NVDATA *SameNicAttempt;
    CHAR16                      IScsiMode[64];
    CHAR16                      IpMode[64];
    ISCSI_NIC_INFO              *NicInfo;
    CHAR8                       *AllocatedLanguages = NULL;
    CHAR8                       *Language = NULL;
    CHAR16                      *CharPtr= NULL; 
    UINT8                       *AttemptConfigOrder;
    UINTN                       AttemptConfigOrderSize;
    UINT8                       *AttemptOrderTmp;
    UINTN                       TotalNumber;
    EFI_STATUS                  Status;

    if (IfrNvData == NULL || Attempt == NULL) {
        return EFI_INVALID_PARAMETER;
    }
    PopUpTitle=NetLibHiiGetString(mCallbackInfo->RegisteredHandle,STRING_TOKEN(STR_MSGBOX_TITLE),NULL);
    //
    // Update those fields which don't have INTERACTIVE attribute.
    //
    Attempt->SessionConfigData.ConnectRetryCount     = IfrNvData->ConnectRetryCount;
    Attempt->SessionConfigData.ConnectTimeout        = IfrNvData->ConnectTimeout;
    Attempt->SessionConfigData.IpMode                = IfrNvData->IpMode;

    if (IfrNvData->IpMode < IP_MODE_AUTOCONFIG) {
        Attempt->SessionConfigData.InitiatorInfoFromDhcp = IfrNvData->InitiatorInfoFromDhcp;
        Attempt->SessionConfigData.TargetPort            = IfrNvData->TargetPort;

        if (Attempt->SessionConfigData.TargetPort == 0) {
            Attempt->SessionConfigData.TargetPort = ISCSI_WELL_KNOWN_PORT;
        }

        Attempt->SessionConfigData.TargetInfoFromDhcp = IfrNvData->TargetInfoFromDhcp;
    }

    Attempt->AuthenticationType = IfrNvData->AuthenticationType;
    Attempt->AuthConfigData.CHAP.CHAPType = IfrNvData->CHAPType; 

    //When loading defaults, Fields with Interactive Flags 
    // must also be loaded values from IfrNvData.

    if (!Attempt->SessionConfigData.InitiatorInfoFromDhcp) {
        NetLibStrToIp4 (IfrNvData->LocalIp, &Attempt->SessionConfigData.LocalIp.v4);
        NetLibStrToIp4 (IfrNvData->SubnetMask, &Attempt->SessionConfigData.SubnetMask);
        NetLibStrToIp4 (IfrNvData->Gateway, &Attempt->SessionConfigData.Gateway.v4);
    }
    if (IfrNvData->IpMode == IP_MODE_IP4) {
        NetLibStrToIp4 (IfrNvData->TargetIp, &Attempt->SessionConfigData.TargetIp.v4);
    } else if (IfrNvData->IpMode == IP_MODE_IP6) {
        NetLibStrToIp6 (IfrNvData->TargetIp, &Attempt->SessionConfigData.TargetIp.v6);
    }

    UnicodeStrToAsciiStr (
      IfrNvData->TargetName,
      Attempt->SessionConfigData.TargetName
    );
    UnicodeStrToAsciiStr (
      IfrNvData->CHAPName,
      Attempt->AuthConfigData.CHAP.CHAPName
    );
    UnicodeStrToAsciiStr (
      IfrNvData->CHAPSecret,
      Attempt->AuthConfigData.CHAP.CHAPSecret
    );
    UnicodeStrToAsciiStr (
      IfrNvData->ReverseCHAPName,
      Attempt->AuthConfigData.CHAP.ReverseCHAPName
    );
    UnicodeStrToAsciiStr (
      IfrNvData->ReverseCHAPSecret,
      Attempt->AuthConfigData.CHAP.ReverseCHAPSecret
    );
    //
    // Only do full parameter validation if iSCSI is enabled on this device.
    //
    if (IfrNvData->Enabled != ISCSI_DISABLED) {
        if (Attempt->SessionConfigData.ConnectTimeout < CONNECT_MIN_TIMEOUT) {   
            CharPtr=NetLibHiiGetString(mCallbackInfo->RegisteredHandle,STRING_TOKEN(STR_INVALID_CONNECT_MIN_TIMEOUT),NULL);
            AMICreatePopUpEx (PopUpTitle,CharPtr);
            if(CharPtr!=NULL)
                FreePool (CharPtr);    

            return EFI_INVALID_PARAMETER;
        }

        //
        // Validate the address configuration of the Initiator if DHCP isn't deployed.
        //
        if (!Attempt->SessionConfigData.InitiatorInfoFromDhcp) {
            CopyMem (&HostIp.v4, &Attempt->SessionConfigData.LocalIp, sizeof (HostIp.v4));
            CopyMem (&SubnetMask.v4, &Attempt->SessionConfigData.SubnetMask, sizeof (SubnetMask.v4));
            CopyMem (&Gateway.v4, &Attempt->SessionConfigData.Gateway, sizeof (Gateway.v4));

            if ((Gateway.Addr[0] != 0)) {
                if (SubnetMask.Addr[0] == 0) {        
                    CharPtr=NetLibHiiGetString(mCallbackInfo->RegisteredHandle,STRING_TOKEN(STR_GATEWAY_SET_SUBNETMASK_ZERO),NULL);
                    AMICreatePopUpEx (PopUpTitle,CharPtr);
                    if(CharPtr!=NULL)
                        FreePool (CharPtr);

                    return EFI_INVALID_PARAMETER;
                } else if (!IP4_NET_EQUAL (HostIp.Addr[0], Gateway.Addr[0], SubnetMask.Addr[0])) {
         

                CharPtr=NetLibHiiGetString(mCallbackInfo->RegisteredHandle,STRING_TOKEN(STR_IP_GATEWAY_IN_DIFF_SUBNET),NULL);
                AMICreatePopUpEx (PopUpTitle,CharPtr);
                if(CharPtr!=NULL)
                    FreePool (CharPtr);


                return EFI_INVALID_PARAMETER;
                }
            }
        }
      
        //Get Target info via DHCP is failed if DnsMode is TRUE 
        if(Attempt->SessionConfigData.TargetInfoFromDhcp)
            Attempt->SessionConfigData.DnsMode = FALSE;

        // Validate target configuration if DHCP isn't deployed.

        if (!Attempt->SessionConfigData.TargetInfoFromDhcp && Attempt->SessionConfigData.IpMode < IP_MODE_AUTOCONFIG) {
            if (!Attempt->SessionConfigData.DnsMode) {
                if (!IpIsUnicast (&Attempt->SessionConfigData.TargetIp, IfrNvData->IpMode)) {
                    CharPtr=NetLibHiiGetString(mCallbackInfo->RegisteredHandle,STRING_TOKEN(STR_INVALID_TARGET_IP),NULL);
                    AMICreatePopUpEx (PopUpTitle,CharPtr);
                    if(CharPtr!=NULL)
                        FreePool (CharPtr);
                    return EFI_INVALID_PARAMETER;
                }
            } else {
                if (Attempt->SessionConfigData.TargetUrl[0] == '\0') {
                    CharPtr=NetLibHiiGetString(mCallbackInfo->RegisteredHandle,STRING_TOKEN(STR_INVALID_TARGET_URL),NULL);
                    AMICreatePopUpEx (PopUpTitle,CharPtr);
                    if(CharPtr!=NULL)
                        FreePool (CharPtr);
                    return EFI_INVALID_PARAMETER;
                }
            }
            //
            // Validate iSCSI target name configuration again
            // The format of iSCSI target name is already verified in AmiIScsiFormCallback() when
            // user input the name; here we only check the case user does not input the name.
            //
            if (Attempt->SessionConfigData.TargetName[0] == '\0') {
                CharPtr=NetLibHiiGetString(mCallbackInfo->RegisteredHandle,STRING_TOKEN(STR_INVALID_TARGETNAME),NULL);
                AMICreatePopUpEx (PopUpTitle,CharPtr);
                if(CharPtr!=NULL)
                    FreePool (CharPtr);
                return EFI_INVALID_PARAMETER;
            }
        }
        //
        // Validate the authentication info.
        //
        if (IfrNvData->AuthenticationType == ISCSI_AUTH_TYPE_CHAP) {
            if ((IfrNvData->CHAPName[0] == '\0') || (IfrNvData->CHAPSecret[0] == '\0')) {
                CharPtr=NetLibHiiGetString(mCallbackInfo->RegisteredHandle,STRING_TOKEN(STR_INVALID_CHAP_NAME),NULL);
                AMICreatePopUpEx (PopUpTitle,CharPtr);
                if(CharPtr!=NULL)
                    FreePool (CharPtr);
                return EFI_INVALID_PARAMETER;
            }
            if ((IfrNvData->CHAPType == ISCSI_CHAP_MUTUAL) &&
                    ((IfrNvData->ReverseCHAPName[0] == '\0') || (IfrNvData->ReverseCHAPSecret[0] == '\0'))
                    ) {
                CharPtr=NetLibHiiGetString(mCallbackInfo->RegisteredHandle,STRING_TOKEN(STR_INVALID_REVERSE_CHAP_NAME),NULL);
                AMICreatePopUpEx (PopUpTitle,CharPtr);
                if(CharPtr!=NULL)
                    FreePool (CharPtr);
                return EFI_INVALID_PARAMETER;
            }
        }
        //
        // Check whether this attempt uses NIC which is already used by existing attempt.
        //
        SameNicAttempt = IScsiConfigGetAttemptByNic (Attempt, IfrNvData->Enabled);
        if (SameNicAttempt != NULL) {
            AttemptName1 = (CHAR16 *) AllocateZeroPool (ATTEMPT_NAME_SIZE * sizeof (CHAR16));
            if (AttemptName1 == NULL) {
                return EFI_OUT_OF_RESOURCES;
            }
            AttemptName2 = (CHAR16 *) AllocateZeroPool (ATTEMPT_NAME_SIZE * sizeof (CHAR16));
            if (AttemptName2 == NULL) {
                FreePool (AttemptName1);
                return EFI_OUT_OF_RESOURCES;
            }      
            AsciiStrToUnicodeStrS (Attempt->AttemptName, AttemptName1, ATTEMPT_NAME_SIZE);
            AsciiStrToUnicodeStrS (SameNicAttempt->AttemptName, AttemptName2, ATTEMPT_NAME_SIZE);
            CharPtr=NetLibHiiGetString(mCallbackInfo->RegisteredHandle,STRING_TOKEN(STR_ATTEMPTS_SAME_NIC),NULL);
            UnicodeSPrint (
              mPrivate->PortString,
              (UINTN) ISCSI_NAME_IFR_MAX_SIZE,
              CharPtr,
              AttemptName1,
              AttemptName2
            );
            if(CharPtr!=NULL)
                FreePool (CharPtr);
            AMICreatePopUpEx (PopUpTitle,mPrivate->PortString);    
            FreePool (AttemptName1);
            FreePool (AttemptName2);
        }
    }
    CharPtr= NULL;  
    //
    // Update the iSCSI Mode data and record it in attempt help info.
    //
    if (IfrNvData->Enabled == ISCSI_DISABLED) {  
        CharPtr=NetLibHiiGetString(mCallbackInfo->RegisteredHandle,STRING_TOKEN(STR_ISCSI_MODE_DISABLED),NULL);
        UnicodeSPrint(IScsiMode, 64, CharPtr); 
    } else if (IfrNvData->Enabled == ISCSI_ENABLED) {
        CharPtr=NetLibHiiGetString(mCallbackInfo->RegisteredHandle,STRING_TOKEN(STR_ISCSI_MODE_ENABLED),NULL);
        UnicodeSPrint(IScsiMode, 64, CharPtr); 
    } else if (IfrNvData->Enabled == ISCSI_ENABLED_FOR_MPIO) {
        CharPtr=NetLibHiiGetString(mCallbackInfo->RegisteredHandle,STRING_TOKEN(STR_ISCSI_MODE_ENABLED_FOR_MPIO),NULL);
        UnicodeSPrint(IScsiMode, 64, CharPtr);
    }
   
    //Release string pointer which is assigned in if or else if blocks
    if(CharPtr != NULL) {
        FreePool (CharPtr);
        CharPtr= NULL;
    }

    if (IfrNvData->IpMode == IP_MODE_IP4) {
        CharPtr=NetLibHiiGetString(mCallbackInfo->RegisteredHandle,STRING_TOKEN(STR_IP_MODE_IP4),NULL);
        UnicodeSPrint(IpMode, 64, CharPtr);
    } else if (IfrNvData->IpMode == IP_MODE_IP6) { 
        CharPtr=NetLibHiiGetString(mCallbackInfo->RegisteredHandle,STRING_TOKEN(STR_IP_MODE_IP6),NULL);
        UnicodeSPrint(IpMode, 64, CharPtr);
    } else if (IfrNvData->IpMode == IP_MODE_AUTOCONFIG) { 
        CharPtr=NetLibHiiGetString(mCallbackInfo->RegisteredHandle,STRING_TOKEN(STR_IP_MODE_AUTOCONFIG),NULL);
        UnicodeSPrint(IpMode, 64, CharPtr);
    }
    if(CharPtr != NULL) {
        FreePool (CharPtr);
        CharPtr= NULL;
    }

    NicInfo = IScsiGetNicInfoByIndex (Attempt->NicIndex);
    if (NicInfo == NULL) {
        return EFI_NOT_FOUND;
    }

    MacString = (CHAR16 *) AllocateZeroPool (ISCSI_MAX_MAC_STRING_LEN * sizeof (CHAR16));
    if (MacString == NULL) {
        return EFI_OUT_OF_RESOURCES;
    }

    AsciiStrToUnicodeStrS (Attempt->MacString, MacString, ISCSI_MAX_MAC_STRING_LEN);
    AllocatedLanguages = HiiGetSupportedLanguages (mCallbackInfo->RegisteredHandle);
    if (AllocatedLanguages != NULL) {
        for (; *AllocatedLanguages != '\0'; ) {
            Language = AllocatedLanguages;
            for (; *AllocatedLanguages != 0 && *AllocatedLanguages != ';'; AllocatedLanguages++);
            if (*AllocatedLanguages != 0) {
                *(AllocatedLanguages++) = '\0';
            }

            CharPtr=NetLibHiiGetString(mCallbackInfo->RegisteredHandle,STRING_TOKEN(STR_PFA_MODE_IPVER),Language);            
            
            UnicodeSPrint (
                  mPrivate->PortString,
                  (UINTN) ISCSI_NAME_IFR_MAX_SIZE,
                  CharPtr,
                  MacString,
                  NicInfo->BusNumber,
                  NicInfo->DeviceNumber,
                  NicInfo->FunctionNumber,
                  IScsiMode,
                  IpMode
            );
            
            Attempt->AttemptTitleHelpToken = HiiSetString (
                                                 mCallbackInfo->RegisteredHandle,
                                                 Attempt->AttemptTitleHelpToken,
                                                 mPrivate->PortString,
                                                 Language
                                                 );
        }
    }
  
    if(CharPtr != NULL)  {
        FreePool (CharPtr);
        CharPtr = NULL;  
    }

    if (Attempt->AttemptTitleHelpToken == 0) {
        FreePool (MacString);
        return EFI_OUT_OF_RESOURCES;
    }

    //
    // Check whether this attempt is an existing one.
    //
    ExistAttempt = IScsiConfigGetAttemptByConfigIndex (Attempt->AttemptConfigIndex);
    if (ExistAttempt != NULL) {
        ASSERT (ExistAttempt == Attempt);

        if (IfrNvData->Enabled == ISCSI_DISABLED &&
            Attempt->SessionConfigData.Enabled != ISCSI_DISABLED) {
            
            //
            // User updates the Attempt from "Enabled"/"Enabled for MPIO" to "Disabled".
            //
            if (Attempt->SessionConfigData.Enabled == ISCSI_ENABLED_FOR_MPIO) {
                if (mPrivate->MpioCount < 1) {
                    return EFI_ABORTED;
                }

                if (--mPrivate->MpioCount == 0) {
                    mPrivate->EnableMpio = FALSE;
                }
            } else if (Attempt->SessionConfigData.Enabled == ISCSI_ENABLED) {
                if (mPrivate->SinglePathCount < 1) {
                    return EFI_ABORTED;
                }
                mPrivate->SinglePathCount--;
            }
        } else if (IfrNvData->Enabled == ISCSI_ENABLED_FOR_MPIO &&
               Attempt->SessionConfigData.Enabled == ISCSI_ENABLED) {
            //
            // User updates the Attempt from "Enabled" to "Enabled for MPIO".
            //
            if (mPrivate->SinglePathCount < 1) {
                return EFI_ABORTED;
            }
            mPrivate->EnableMpio = TRUE;
            mPrivate->MpioCount++;
            mPrivate->SinglePathCount--;
        } else if (IfrNvData->Enabled == ISCSI_ENABLED &&
               Attempt->SessionConfigData.Enabled == ISCSI_ENABLED_FOR_MPIO) {
            //
            // User updates the Attempt from "Enabled for MPIO" to "Enabled".
            //
            if (mPrivate->MpioCount < 1) {
                return EFI_ABORTED;
            }
            if (--mPrivate->MpioCount == 0) {
                mPrivate->EnableMpio = FALSE;
            }
            mPrivate->SinglePathCount++;
        } else if (IfrNvData->Enabled != ISCSI_DISABLED &&
               Attempt->SessionConfigData.Enabled == ISCSI_DISABLED) {
            //
            // User updates the Attempt from "Disabled" to "Enabled"/"Enabled for MPIO".
            //
            if (IfrNvData->Enabled == ISCSI_ENABLED_FOR_MPIO) {
                mPrivate->EnableMpio = TRUE;
                mPrivate->MpioCount++;
            } else if (IfrNvData->Enabled == ISCSI_ENABLED) {
                mPrivate->SinglePathCount++;
            }
        }
    } else if (ExistAttempt == NULL) {
        //
        // When a new attempt is created, pointer of the attempt is saved to
        // mCallbackInfo->Current in AmiIScsiConfigProcessDefault. If input Attempt
        // does not match any existing attempt, it should be a new created attempt.
        // Save it to system now.
        
        // Save current order number for this attempt.
        AttemptConfigOrder = IScsiGetVariableAndSize (
                           L"AttemptOrder",
                           &gIScsiConfigGuid,
                           &AttemptConfigOrderSize
                           );
        TotalNumber = AttemptConfigOrderSize / sizeof (UINT8);
        TotalNumber++;
        //
        // Append the new created attempt order to the end.
        //
        AttemptOrderTmp = AllocateZeroPool (TotalNumber * sizeof (UINT8));
        if (AttemptOrderTmp == NULL) {
            if (AttemptConfigOrder != NULL) {
                FreePool (AttemptConfigOrder);
            }
            return EFI_OUT_OF_RESOURCES;
        }
        if (AttemptConfigOrder != NULL) {
            CopyMem (AttemptOrderTmp, AttemptConfigOrder, AttemptConfigOrderSize);
            FreePool (AttemptConfigOrder);
        }
        
        AttemptOrderTmp[TotalNumber - 1] = Attempt->AttemptConfigIndex;
        AttemptConfigOrder               = AttemptOrderTmp;
        AttemptConfigOrderSize           = TotalNumber * sizeof (UINT8);
        
        Status = gRT->SetVariable (
                    L"AttemptOrder",
                    &gIScsiConfigGuid,
                    EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_NON_VOLATILE,
                    AttemptConfigOrderSize,
                    AttemptConfigOrder
                    );
        FreePool (AttemptConfigOrder);
        if (EFI_ERROR (Status)) {
            return Status;
        }
        
        //
        // Insert new created attempt to array.
        //

#if(ENABLE_CHAP_SECRET_ENCRYPTION)
        // Here the attempt count gets incremented. So, if CMOS is removed, all the attempts get deleted at once.
        // With the following condition, we can trigger the user hook to delete all the entries.
        if(mPrivate->AttemptCount == 0)
            Status = PrepareToEncrypt(NULL,ClearAllEntries);
 
#endif
        InsertTailList (&mPrivate->AttemptConfigs, &Attempt->Link);
        mPrivate->AttemptCount++;
        
        if (IfrNvData->Enabled == ISCSI_ENABLED_FOR_MPIO) {
            //
            // This new Attempt is enabled for MPIO; enable the multipath mode.
            //
            mPrivate->EnableMpio = TRUE;
            mPrivate->MpioCount++;
        } else if (IfrNvData->Enabled == ISCSI_ENABLED) {
            mPrivate->SinglePathCount++;
        }

        IScsiConfigUpdateAttempt ();
    }
    Attempt->SessionConfigData.Enabled = IfrNvData->Enabled; 
    Attempt->AttemptType = SETUP_ATTEMPT;
    //
    // Record the user configuration information in NVR.
    //
    UnicodeSPrint (mPrivate->PortString, (UINTN) ISCSI_NAME_IFR_MAX_SIZE, L"Attempt %d", Attempt->AttemptConfigIndex);

    FreePool (MacString);

#if(ENABLE_CHAP_SECRET_ENCRYPTION)
    Status = PrepareToEncrypt(Attempt,EncryptSecrets);
        if(!(Status == EFI_SUCCESS || Status == EFI_UNSUPPORTED))
            return Status;
#endif

    Status = gRT->SetVariable (
        mPrivate->PortString,
        &gEfiIScsiInitiatorNameProtocolGuid,
        ISCSI_CONFIG_VAR_ATTR,
        sizeof (ISCSI_ATTEMPT_CONFIG_NVDATA),
        Attempt
        );

#if(ENABLE_CHAP_SECRET_ENCRYPTION)
    Status = PrepareToEncrypt(Attempt,DecryptSecrets);
#endif
    return Status;
}

/**
  Callback function when user presses "Add an Attempt".

  @retval EFI_OUT_OF_RESOURCES   Does not have sufficient resources to finish this
                                 operation.
  @retval EFI_SUCCESS            The operation is completed successfully.

**/
EFI_STATUS
AmiIScsiConfigAddAttempt (
    VOID
)
{
    LIST_ENTRY                    *Entry;
    ISCSI_NIC_INFO                *NicInfo;
    EFI_STRING_ID                 PortTitleToken;
    EFI_STRING_ID                 PortTitleHelpToken;
    CHAR16                        MacString[ISCSI_MAX_MAC_STRING_LEN];
    CHAR16                        MacControllerName[ISCSI_NAME_IFR_MAX_SIZE];
    EFI_STATUS                    Status;
    VOID                          *StartOpCodeHandle;
    EFI_IFR_GUID_LABEL            *StartLabel;
    VOID                          *EndOpCodeHandle;
    EFI_IFR_GUID_LABEL            *EndLabel;  
    CHAR16                        *CharPtr;
    CHAR16                        *ControllerName = NULL;
    
    Status = IScsiCreateOpCode (
             MAC_ENTRY_LABEL,
             &StartOpCodeHandle,
             &StartLabel,
             &EndOpCodeHandle,
             &EndLabel
             );
    if (EFI_ERROR (Status)) {
        return Status;
    }
    // Update string when MAC is not found
    if (IsListEmpty(&mPrivate->NicInfoList)) {
        HiiCreateSubTitleOpCode (
        StartOpCodeHandle,                         // Container for dynamic created opcodes
        STRING_TOKEN(STR_MAC_STRING_NOT_FOUND),    // Text
        STRING_TOKEN(STR_EMPTY),                   // Help String
        0,
        0
        );
    } else { 
        //
        // Ask user to select a MAC for this attempt.
        //
        NET_LIST_FOR_EACH (Entry, &mPrivate->NicInfoList) {
            NicInfo = NET_LIST_USER_STRUCT (Entry, ISCSI_NIC_INFO, Link);
            IScsiMacAddrToStr (
                    &NicInfo->PermanentAddress,
                    NicInfo->HwAddressSize,
                    NicInfo->VlanId,
                    MacString
                    );
            
            CharPtr=NetLibHiiGetString(mCallbackInfo->RegisteredHandle,STRING_TOKEN(STR_MAC),NULL);
            Status = GetControllerName(NicInfo->PermanentAddress,&ControllerName);
            if(!EFI_ERROR(Status))
            {
                UnicodeSPrint (MacControllerName, (UINTN) ISCSI_NAME_IFR_MAX_SIZE,L"%s (%s)", MacString, ControllerName);
                UnicodeSPrint (mPrivate->PortString, (UINTN) ISCSI_NAME_IFR_MAX_SIZE, CharPtr, MacControllerName);
            }
            else
            {
                UnicodeSPrint (mPrivate->PortString, (UINTN) ISCSI_NAME_IFR_MAX_SIZE, CharPtr, MacString);
            }
            if(CharPtr != NULL) {
                FreePool (CharPtr);
                CharPtr= NULL;
            }
            PortTitleToken = HiiSetString (
                       mCallbackInfo->RegisteredHandle,
                       0,
                       mPrivate->PortString,
                       NULL
                       );
            if (PortTitleToken == 0) {
                Status = EFI_INVALID_PARAMETER;
                goto Exit;
            }
            CharPtr=NetLibHiiGetString(mCallbackInfo->RegisteredHandle,STRING_TOKEN(STR_PFA),NULL);
            UnicodeSPrint (
                    mPrivate->PortString,
                    (UINTN) ISCSI_NAME_IFR_MAX_SIZE,
                    CharPtr,
                    NicInfo->BusNumber,
                    NicInfo->DeviceNumber,
                    NicInfo->FunctionNumber
                    );
            
            if(CharPtr != NULL) {
                FreePool (CharPtr);
                CharPtr= NULL;
            }
            PortTitleHelpToken = HiiSetString (mCallbackInfo->RegisteredHandle, 0, mPrivate->PortString, NULL);
            if (PortTitleHelpToken == 0) {
                Status = EFI_INVALID_PARAMETER;
                goto Exit;
            }
            
            HiiCreateGotoOpCode (
                    StartOpCodeHandle,                      // Container for dynamic created opcodes
                    FORMID_ATTEMPT_FORM,
                    PortTitleToken,
                    PortTitleHelpToken,
                    EFI_IFR_FLAG_CALLBACK,                  // Question flag
                    (UINT16) (KEY_MAC_ENTRY_BASE + NicInfo->NicIndex)
                    );
        }  
    }
    // Update string when MAC is not found

    Status = HiiUpdateForm (
             mCallbackInfo->RegisteredHandle, // HII handle
             &gIScsiConfigGuid,               // Formset GUID
             FORMID_MAC_FORM,                 // Form ID
             StartOpCodeHandle,               // Label for where to insert opcodes
             EndOpCodeHandle                  // Replace data
             );

Exit:
    HiiFreeOpCodeHandle (StartOpCodeHandle);
    HiiFreeOpCodeHandle (EndOpCodeHandle);

    return Status;
}

/**
  Callback function when user presses "Change Attempt Order".

  @retval EFI_INVALID_PARAMETER  Any parameter is invalid.
  @retval EFI_OUT_OF_RESOURCES   Does not have sufficient resources to finish this
                                 operation.
  @retval EFI_SUCCESS            The operation is completed successfully.

**/
EFI_STATUS
AmiIScsiConfigDisplayOrderAttempts (
    ISCSI_CONFIG_IFR_NVDATA     *IfrNvData
) 
{
    EFI_STATUS                  Status;
    UINT8                       Index;
    LIST_ENTRY                  *Entry;
    ISCSI_ATTEMPT_CONFIG_NVDATA *AttemptConfigData;
    VOID                        *StartOpCodeHandle;
    EFI_IFR_GUID_LABEL          *StartLabel;
    VOID                        *EndOpCodeHandle;
    EFI_IFR_GUID_LABEL          *EndLabel;
    VOID                        *OptionsOpCodeHandle;

    Status = IScsiCreateOpCode (
             ORDER_ENTRY_LABEL,
             &StartOpCodeHandle,
             &StartLabel,
             &EndOpCodeHandle,
             &EndLabel
             );
    if (EFI_ERROR (Status)) {
        return Status;
    }
    ASSERT (StartOpCodeHandle != NULL);

    OptionsOpCodeHandle = NULL;

    //
    // If no attempt to be ordered, update the original form and exit.
    //
    if (mPrivate->AttemptCount == 0) {
        goto Exit;
    }

    //
    // Create Option OpCode.
    //
    OptionsOpCodeHandle = HiiAllocateOpCodeHandle ();
    if (OptionsOpCodeHandle == NULL) {
        Status = EFI_OUT_OF_RESOURCES;
        goto Error;
    }

    Index = 0;

    NET_LIST_FOR_EACH (Entry, &mPrivate->AttemptConfigs) {
        AttemptConfigData = NET_LIST_USER_STRUCT (Entry, ISCSI_ATTEMPT_CONFIG_NVDATA, Link);
        if(AttemptConfigData->AttemptType == SETUP_ATTEMPT) {
            HiiCreateOneOfOptionOpCode (
                    OptionsOpCodeHandle,
                    AttemptConfigData->AttemptTitleToken,
                    0,
                    EFI_IFR_NUMERIC_SIZE_1,
                    AttemptConfigData->AttemptConfigIndex
                    );  
        }
        IfrNvData->DynamicOrderedList[Index] = AttemptConfigData->AttemptConfigIndex;
        Index++;
    }

    ASSERT (Index == mPrivate->AttemptCount);

    HiiCreateOrderedListOpCode (
    StartOpCodeHandle,                          // Container for dynamic created opcodes
    DYNAMIC_ORDERED_LIST_QUESTION_ID,           // Question ID
    CONFIGURATION_VARSTORE_ID,                  // VarStore ID
    DYNAMIC_ORDERED_LIST_VAR_OFFSET,            // Offset in Buffer Storage
    STRING_TOKEN (STR_ORDER_ATTEMPT_ENTRY),     // Question prompt text  
    STRING_TOKEN (STR_ORDER_ATTEMPT_ENTRY_HELP),     // Question help text
    0,                                          // Question flag
    EFI_IFR_UNIQUE_SET,                         // Ordered list flag, e.g. EFI_IFR_UNIQUE_SET
    EFI_IFR_NUMERIC_SIZE_1,                     // Data type of Question value
    ISCSI_MAX_ATTEMPTS_NUM,                     // Maximum container
    OptionsOpCodeHandle,                        // Option Opcode list
    NULL                                        // Default Opcode is NULL
    );

Exit:
    Status = HiiUpdateForm (
             mCallbackInfo->RegisteredHandle, // HII handle
             &gIScsiConfigGuid,               // Formset GUID
             FORMID_ORDER_FORM,               // Form ID
             StartOpCodeHandle,               // Label for where to insert opcodes
             EndOpCodeHandle                  // Replace data
             );

Error:
    HiiFreeOpCodeHandle (StartOpCodeHandle);
    HiiFreeOpCodeHandle (EndOpCodeHandle);
    if (OptionsOpCodeHandle != NULL) {
        HiiFreeOpCodeHandle (OptionsOpCodeHandle);
    }

    return Status;
}

/**
  Callback function when a user presses "Attempt *" or when a user selects a NIC to
  create the new attempt.

  @param[in]  KeyValue           A unique value which is sent to the original
                                 exporting driver so that it can identify the type
                                 of data to expect.
  @param[in]  IfrNvData          The IFR nv data.

  @retval EFI_OUT_OF_RESOURCES   Does not have sufficient resources to finish this
                                 operation.
  @retval EFI_NOT_FOUND          Cannot find the corresponding variable.
  @retval EFI_UNSUPPORTED        Can not create more attempts.
  @retval EFI_SUCCESS            The operation is completed successfully.

**/
EFI_STATUS
AmiIScsiConfigProcessDefault (
    IN  EFI_QUESTION_ID              KeyValue,
    IN  ISCSI_CONFIG_IFR_NVDATA      *IfrNvData
)
{
    BOOLEAN                     NewAttempt;
    ISCSI_ATTEMPT_CONFIG_NVDATA *AttemptConfigData;
    UINT8                       CurrentAttemptConfigIndex;
    ISCSI_NIC_INFO              *NicInfo;
    UINT8                       NicIndex;
    CHAR16                      MacString[ISCSI_MAX_MAC_STRING_LEN];
    UINT8                       *AttemptConfigOrder;
    UINTN                       AttemptConfigOrderSize;
    UINTN                       Index;
    EFI_INPUT_KEY               Key;
    CHAR16                      *CharPtr;
    AttemptConfigData = NULL;
    //
    // Is User creating a new attempt?
    //
    NewAttempt = FALSE;

    if ((KeyValue >= KEY_MAC_ENTRY_BASE) &&
      (KeyValue <= (UINT16) (mPrivate->MaxNic + KEY_MAC_ENTRY_BASE))) {
        //
        // User has pressed "Add an Attempt" and then selects a NIC.
        //
        NewAttempt = TRUE;
    } else if ((KeyValue >= KEY_ATTEMPT_ENTRY_BASE) &&
             (KeyValue <= (ISCSI_MAX_ATTEMPTS_NUM + KEY_ATTEMPT_ENTRY_BASE))) {
        //
        // User has pressed "Attempt *".
        //
        NewAttempt = FALSE;
    } else {
        //
        // Don't process anything.
        //
        return EFI_SUCCESS;
    }

    if (NewAttempt) {
        //
        // Determine which NIC user has selected for the new created attempt.
        //
        NicIndex = (UINT8) (KeyValue - KEY_MAC_ENTRY_BASE);
        NicInfo = IScsiGetNicInfoByIndex (NicIndex);
        if (NicInfo == NULL) {
            return EFI_NOT_FOUND;
        }

        //
        // Create an attempt following the initialized attempt order.
        //
        AttemptConfigOrder = IScsiGetVariableAndSize (
                           L"InitialAttemptOrder",
                           &gIScsiConfigGuid,
                           &AttemptConfigOrderSize
                           );
        
        if (AttemptConfigOrder == NULL) {
            return EFI_NOT_FOUND;
        }
        
        for (Index = 0; Index < AttemptConfigOrderSize / sizeof (UINT8); Index++) {
            UnicodeSPrint (
                    mPrivate->PortString,
                    (UINTN) ISCSI_NAME_IFR_MAX_SIZE,
                    L"Attempt %d",
                    (UINTN) AttemptConfigOrder[Index]
                    );
            GetVariable2 (
                   mPrivate->PortString,
                   &gEfiIScsiInitiatorNameProtocolGuid,
                   (VOID**)&AttemptConfigData,
                   NULL
                   );
            if (AttemptConfigData == NULL || (((AttemptConfigData->AttemptType == REDFISH_ATTEMPT)||(AttemptConfigData->AttemptType == RSD_ATTEMPT)) && 
                    (AttemptConfigData->SessionConfigData.Enabled)) ||
                    ((AttemptConfigData->AttemptType == SETUP_ATTEMPT)&& (AttemptConfigData->Actived == ISCSI_ACTIVE_ENABLED))) {
                continue;
            }
            break;
        }
        
        if (Index > PcdGet8 (PcdMaxIScsiAttemptNumber)) {
            CreatePopUp (
                    EFI_LIGHTGRAY | EFI_BACKGROUND_BLUE,
                    &Key,
                    L"Can not create more attempts, Please configure the PcdMaxIScsiAttemptNumber if needed!",
                    NULL
                    );
            return EFI_UNSUPPORTED;
        }
        
        if (AttemptConfigOrder != NULL) {
            FreePool (AttemptConfigOrder);
        }    
        if (AttemptConfigData->Actived == ISCSI_ACTIVE_ENABLED){
            AttemptConfigData = IScsiConfigGetAttemptByConfigIndex (AttemptConfigData->AttemptConfigIndex);
            if (AttemptConfigData == NULL) {
                return EFI_NOT_FOUND;
            }
            goto End;
        }
        //
        // Record the MAC info in Config Data.
        //
        IScsiMacAddrToStr (
                &NicInfo->PermanentAddress,
                NicInfo->HwAddressSize,
                NicInfo->VlanId,
                MacString
                );
        
        ASSERT (AttemptConfigData != NULL);
        UnicodeStrToAsciiStrS (MacString, AttemptConfigData->MacString, sizeof (AttemptConfigData->MacString));
        AttemptConfigData->NicIndex = NicIndex;
        AttemptConfigData->Actived = ISCSI_ACTIVE_ENABLED;
        
        //
        // Generate OUI-format ISID based on MAC address.
        //
        CopyMem (AttemptConfigData->SessionConfigData.IsId, &NicInfo->PermanentAddress, 6);
        AttemptConfigData->SessionConfigData.IsId[0] =
                (UINT8) (AttemptConfigData->SessionConfigData.IsId[0] & 0x3F);
        
        //
        // Add the help info for the new attempt.
        //    
        CharPtr=NetLibHiiGetString(mCallbackInfo->RegisteredHandle,STRING_TOKEN(STR_MAC_INFO),NULL);
        UnicodeSPrint (
                mPrivate->PortString,
                (UINTN) ISCSI_NAME_IFR_MAX_SIZE,
                CharPtr,
                MacString,
                NicInfo->BusNumber,
                NicInfo->DeviceNumber,
                NicInfo->FunctionNumber
                );
        if(CharPtr != NULL)
            FreePool (CharPtr);
        
        AttemptConfigData->AttemptTitleHelpToken  = HiiSetString (
                                                  mCallbackInfo->RegisteredHandle,
                                                  0,
                                                  mPrivate->PortString,
                                                  NULL
                                                  );
        if (AttemptConfigData->AttemptTitleHelpToken == 0) {
            FreePool (AttemptConfigData);
            return EFI_OUT_OF_RESOURCES;
        }
    } else {
        //
        // Determine which Attempt user has selected to configure.
        // Get the attempt configuration data.
        //
        CurrentAttemptConfigIndex = (UINT8) (KeyValue - KEY_ATTEMPT_ENTRY_BASE);
        AttemptConfigData = IScsiConfigGetAttemptByConfigIndex (CurrentAttemptConfigIndex);
        if (AttemptConfigData == NULL) {
            DEBUG ((DEBUG_ERROR, "Corresponding configuration data can not be retrieved!\n"));
            return EFI_NOT_FOUND;
        }
    }

    //
    // Clear the old IFR data to avoid sharing it with other attempts.
    //
End:
    if (IfrNvData->AuthenticationType == ISCSI_AUTH_TYPE_CHAP) {
        ZeroMem (IfrNvData->CHAPName, sizeof (IfrNvData->CHAPName));
        ZeroMem (IfrNvData->CHAPSecret, sizeof (IfrNvData->CHAPSecret));
        ZeroMem (IfrNvData->ReverseCHAPName, sizeof (IfrNvData->ReverseCHAPName));
        ZeroMem (IfrNvData->ReverseCHAPSecret, sizeof (IfrNvData->ReverseCHAPSecret));
    }

    IScsiConvertAttemptConfigDataToIfrNvData (AttemptConfigData, IfrNvData);

    //
    // Update current attempt to be a new created attempt or an existing attempt.
    //
    mCallbackInfo->Current = AttemptConfigData;

    return EFI_SUCCESS;
}

/**

  This function allows the caller to request the current
  configuration for one or more named elements. The resulting
  string is in <ConfigAltResp> format. Also, any and all alternative
  configuration strings shall be appended to the end of the
  current configuration string. If they are, they must appear
  after the current configuration. They must contain the same
  routing (GUID, NAME, PATH) as the current configuration string.
  They must have an additional description indicating the type of
  alternative configuration the string represents,
  "ALTCFG=<StringToken>". That <StringToken> (when
  converted from Hex UNICODE to binary) is a reference to a
  string in the associated string pack.

  @param[in]  This       Points to the EFI_HII_CONFIG_ACCESS_PROTOCOL.

  @param[in]  Request    A null-terminated Unicode string in
                         <ConfigRequest> format. Note that this
                         includes the routing information as well as
                         the configurable name / value pairs. It is
                         invalid for this string to be in
                         <MultiConfigRequest> format.

  @param[out] Progress   On return, points to a character in the
                         Request string. Points to the string's null
                         terminator if request was successful. Points
                         to the most recent "&" before the first
                         failing name / value pair (or the beginning
                         of the string if the failure is in the first
                         name / value pair) if the request was not successful.

  @param[out] Results    A null-terminated Unicode string in
                         <ConfigAltResp> format which has all values
                         filled in for the names in the Request string.
                         String to be allocated by the called function.

  @retval EFI_SUCCESS             The Results string is filled with the
                                  values corresponding to all requested
                                  names.

  @retval EFI_OUT_OF_RESOURCES    Not enough memory to store the
                                  parts of the results that must be
                                  stored awaiting possible future
                                  protocols.

  @retval EFI_INVALID_PARAMETER   For example, passing in a NULL
                                  for the Request parameter
                                  would result in this type of
                                  error. In this case, the
                                  Progress parameter would be
                                  set to NULL.

  @retval EFI_NOT_FOUND           Routing data doesn't match any
                                  known driver. Progress set to the
                                  first character in the routing header.
                                  Note: There is no requirement that the
                                  driver validate the routing data. It
                                  must skip the <ConfigHdr> in order to
                                  process the names.

  @retval EFI_INVALID_PARAMETER   Illegal syntax. Progress set
                                  to most recent "&" before the
                                  error or the beginning of the
                                  string.

  @retval EFI_INVALID_PARAMETER   Unknown name. Progress points
                                  to the & before the name in
                                  question.

**/
EFI_STATUS
EFIAPI
AmiIScsiFormExtractConfig (
    IN  CONST EFI_HII_CONFIG_ACCESS_PROTOCOL   *This,
    IN  CONST EFI_STRING                       Request,
    OUT EFI_STRING                             *Progress,
    OUT EFI_STRING                             *Results
)
{
    EFI_STATUS                       Status;
    CHAR8                            *InitiatorName;
    UINTN                            BufferSize;
    ISCSI_CONFIG_IFR_NVDATA          *IfrNvData;
    ISCSI_FORM_CALLBACK_INFO         *Private;
    EFI_STRING                       ConfigRequestHdr;
    EFI_STRING                       ConfigRequest;
    BOOLEAN                          AllocatedRequest;
    UINTN                            Size;
    CHAR16                           *CharPtr;
    // Variables created for getting admin/user privileges
    UINT8                            SysAccessValue;
    UINTN                            SysAccessValueSize = sizeof(SysAccessValue);
    // Getting SystemAccess varstore guid
    EFI_GUID                         SystemAccessGuid = SYSTEM_ACCESS_GUID; 

    if (This == NULL || Progress == NULL || Results == NULL) {
        return EFI_INVALID_PARAMETER;
    }

    *Progress = Request;
    if ((Request != NULL) && !HiiIsConfigHdrMatch (Request, &gIScsiConfigGuid, mVendorStorageName)) {
        return EFI_NOT_FOUND;
    }

    ConfigRequestHdr = NULL;
    ConfigRequest    = NULL;
    AllocatedRequest = FALSE;
    Size             = 0;

    Private = ISCSI_FORM_CALLBACK_INFO_FROM_FORM_CALLBACK (This);
    IfrNvData = AllocateZeroPool (sizeof (ISCSI_CONFIG_IFR_NVDATA));
    if (IfrNvData == NULL) {
        return EFI_OUT_OF_RESOURCES;
    }


    if (Private->Current!= NULL) {
        IScsiConvertAttemptConfigDataToIfrNvData (Private->Current, IfrNvData);
    }

    //
    // Extract all AttemptConfigData to Keyword stroage of IfrNvData.
    //
    IScsiConvertAttemptConfigDataToIfrNvDataByKeyword (IfrNvData);
 
    //Get variable from SystemAccess NVRAM variable and store in our varstore variable

    Status = gRT->GetVariable (
              L"SystemAccess",
              &SystemAccessGuid,
              NULL,
              &SysAccessValueSize,
              &SysAccessValue );

    if ( !EFI_ERROR(Status) ) {
        IfrNvData->SystemAccessVar = SysAccessValue;
    } 

    BufferSize    = ISCSI_NAME_MAX_SIZE;
    InitiatorName = (CHAR8 *) AllocateZeroPool (BufferSize);
    if (InitiatorName == NULL) {
        FreePool (IfrNvData);
        return EFI_OUT_OF_RESOURCES;
    }

    Status = gIScsiInitiatorName.Get (&gIScsiInitiatorName, &BufferSize, InitiatorName);
    if (EFI_ERROR (Status)) {
        IfrNvData->InitiatorName[0] = L'\0';
        // Update flag for invalid Initiator name as FALSE.
        IfrNvData->IsInitiatorNameValid = FALSE;    
    } else {
        AsciiStrToUnicodeStrS (
        InitiatorName,
        IfrNvData->InitiatorName,
        sizeof (IfrNvData->InitiatorName) / sizeof (IfrNvData->InitiatorName[0])
        );
        //Update flag for Valid Initiator name as TRUE.
        IfrNvData->IsInitiatorNameValid = TRUE;     
    }
 
    //
    // Convert buffer data to <ConfigResp> by helper function BlockToConfig().
    //
    BufferSize = sizeof (ISCSI_CONFIG_IFR_NVDATA);
    ConfigRequest = Request;
    if ((Request == NULL) || (StrStr (Request, L"OFFSET") == NULL)) {
        //
        // Request has no request element, construct full request string.
        // Allocate and fill a buffer large enough to hold the <ConfigHdr> template
        // followed by "&OFFSET=0&WIDTH=WWWWWWWWWWWWWWWW" followed by a Null-terminator
        //
        ConfigRequestHdr = HiiConstructConfigHdr (&gIScsiConfigGuid, mVendorStorageName, Private->DriverHandle);
        Size = (StrLen (ConfigRequestHdr) + 32 + 1) * sizeof (CHAR16);
        ConfigRequest = AllocateZeroPool (Size);
        if (ConfigRequest == NULL) {
            FreePool (IfrNvData);
            FreePool (InitiatorName);
            return EFI_OUT_OF_RESOURCES;
        }
        AllocatedRequest = TRUE;
        
        CharPtr=NetLibHiiGetString(mCallbackInfo->RegisteredHandle,STRING_TOKEN(STR_OFFSET_WIDTH ),NULL);
        UnicodeSPrint (ConfigRequest, Size, CharPtr, ConfigRequestHdr, (UINT64)BufferSize);
        
        if(CharPtr != NULL)
            FreePool (CharPtr);  
        FreePool (ConfigRequestHdr);
    }

    Status = gHiiConfigRouting->BlockToConfig (
                                gHiiConfigRouting,
                                ConfigRequest,
                                (UINT8 *) IfrNvData,
                                BufferSize,
                                Results,
                                Progress
                                );
    FreePool (IfrNvData);
    FreePool (InitiatorName);

    //
    // Free the allocated config request string.
    //
    if (AllocatedRequest) {
        FreePool (ConfigRequest);
        ConfigRequest = NULL;
    }
    //
    // Set Progress string to the original request string.
    //
    if (Request == NULL) {
        *Progress = NULL;
    } else if (StrStr (Request, L"OFFSET") == NULL) {
        *Progress = Request + StrLen (Request);
    }

    return Status;
}

/**
  Initialize the iSCSI configuration form.

  @param[in]  DriverBindingHandle The iSCSI driverbinding handle.

  @retval EFI_SUCCESS             The iSCSI configuration form is initialized.
  @retval EFI_OUT_OF_RESOURCES    Failed to allocate memory.

**/
EFI_STATUS
AmiIScsiConfigFormInit (
    IN EFI_HANDLE  DriverBindingHandle
)
{
    EFI_STATUS                  Status;
    ISCSI_FORM_CALLBACK_INFO    *CallbackInfo;

    CallbackInfo = (ISCSI_FORM_CALLBACK_INFO *) AllocateZeroPool (sizeof (ISCSI_FORM_CALLBACK_INFO));
    if (CallbackInfo == NULL) {
        return EFI_OUT_OF_RESOURCES;
    }

    CallbackInfo->Signature   = ISCSI_FORM_CALLBACK_INFO_SIGNATURE;
    CallbackInfo->Current     = NULL;

  
    CallbackInfo->ConfigAccess.RouteConfig   = IScsiFormRouteConfig;
    CallbackInfo->ConfigAccess.Callback      = AmiIScsiFormCallback;
    CallbackInfo->ConfigAccess.ExtractConfig = AmiIScsiFormExtractConfig;
    //
    // Install Device Path Protocol and Config Access protocol to driver handle.
    //
    Status = gBS->InstallMultipleProtocolInterfaces (
                  &CallbackInfo->DriverHandle,
                  &gEfiDevicePathProtocolGuid,
                  &mIScsiHiiVendorDevicePath,
                  &gEfiHiiConfigAccessProtocolGuid,
                  &CallbackInfo->ConfigAccess,
                  NULL
                  );
    ASSERT_EFI_ERROR (Status);

    //
    // Publish our HII data.
    //
    CallbackInfo->RegisteredHandle = HiiAddPackages (
                                     &gIScsiConfigGuid,
                                     CallbackInfo->DriverHandle,
                                     IScsiDxeStrings,
                                     AmiIScsiConfigVfrBin,
                                     NULL
                                     );
    if (CallbackInfo->RegisteredHandle == NULL) {
        gBS->UninstallMultipleProtocolInterfaces (
            // Passing EFI_HANDLE to UninstallMultipleProtocolInterfaces
           CallbackInfo->DriverHandle,              
           &gEfiDevicePathProtocolGuid,
           &mIScsiHiiVendorDevicePath,
           &gEfiHiiConfigAccessProtocolGuid,
           &CallbackInfo->ConfigAccess,
           NULL
           );
        FreePool(CallbackInfo);
        return EFI_OUT_OF_RESOURCES;
    }

    mCallbackInfo = CallbackInfo;
#if (ISCSI_FORCEFULLY_SET_INITIATOR_NAME)
    ForcefullySetInitiatorName(NULL, NULL,TRUE);
#endif

    return EFI_SUCCESS;
}

/**
  Function to get Contoller Name corresponding to the NIC.

  @param[in]       MacAddress           MAC Address.
  @param[in, out]  ControllerName       The Controller name of the NIC.
  
  @retval EFI_SUCCESS            The operation is completed successfully.

**/

EFI_STATUS GetControllerName (EFI_MAC_ADDRESS MacAddress, CHAR16 **ControllerName)
{
    EFI_STATUS                                  Status;
    UINTN                                       HandleCount, Index;
    EFI_HANDLE                                  *HandleBuffer = NULL;
    EFI_NETWORK_INTERFACE_IDENTIFIER_PROTOCOL   *Nii;
    EFI_DEVICE_PATH_PROTOCOL                    *DevPath;
    EFI_HANDLE                                  ControllerHandle;
    EFI_COMPONENT_NAME2_PROTOCOL                *ComponentName2 = NULL;
    EFI_COMPONENT_NAME_PROTOCOL                 *ComponentName = NULL;
    UINT32                                      Dindex;
    EFI_OPEN_PROTOCOL_INFORMATION_ENTRY         *OpenInfoBuffer = NULL;
    UINTN                                       EntryCount;
    CHAR16                                      MacString[ISCSI_MAX_MAC_STRING_LEN];
    CHAR16                                      MacString2[ISCSI_MAX_MAC_STRING_LEN];
    EFI_SIMPLE_NETWORK_PROTOCOL                 *Snp;
    
    //Locate all Network UNDI protocol Handle
    Status = gBS->LocateHandleBuffer (
		ByProtocol,
		&gEfiNetworkInterfaceIdentifierProtocolGuid_31, 
		NULL, 
		&HandleCount, 
		&HandleBuffer
	);
    if(EFI_ERROR(Status)) {
        return Status;
    }
	
    IScsiMacAddrToStr(&MacAddress, 6, 0, MacString);
	
    for(Index = 0;Index < HandleCount; Index++) {
        Status = gBS->HandleProtocol (
			HandleBuffer[Index],
			&gEfiNetworkInterfaceIdentifierProtocolGuid_31,
			(VOID **) &Nii
		);
        if (EFI_ERROR(Status)) {
            continue;
        }
		
        Status = gBS->HandleProtocol (
			HandleBuffer[Index],
			&gEfiDevicePathProtocolGuid,
			(VOID *)&DevPath
		);
        if (EFI_ERROR(Status)) {
            continue;    
        }
		
        //Locate controller Handle
        Status = gBS->LocateDevicePath(
			&gEfiPciIoProtocolGuid,
            &DevPath,
            &ControllerHandle
		);
        if (EFI_ERROR(Status)) {
            continue;
        }
		
        Status = gBS->HandleProtocol (
			HandleBuffer[Index],
			&gEfiSimpleNetworkProtocolGuid,
			(VOID **) &Snp
		);
        if (EFI_ERROR(Status)) {
            continue;
        }
		
        IScsiMacAddrToStr(&Snp->Mode->PermanentAddress, 6, 0, MacString2);
        if(StrCmp(MacString2, MacString)) {
            continue;
        }
        //Get Controller Name of matched NIC
        Status = gBS->OpenProtocolInformation ( 
			ControllerHandle, 
			(EFI_GUID *) &gEfiPciIoProtocolGuid, 
			&OpenInfoBuffer, 
			&EntryCount
		);
        if (EFI_ERROR(Status)) {
            continue;
        }
		
        for (Dindex = 0; Dindex  < EntryCount; Dindex++) {
            Status = gBS->HandleProtocol (
				OpenInfoBuffer[Dindex].AgentHandle,
				&gEfiComponentName2ProtocolGuid,
				(VOID **)&ComponentName2
			);
            if (!EFI_ERROR(Status)) {
                Status = ComponentName2->GetControllerName (
                  	ComponentName2,
                    ControllerHandle,
                    OpenInfoBuffer[Dindex].ControllerHandle,
                    ComponentName2->SupportedLanguages,
                    ControllerName
                );  
                if (!EFI_ERROR(Status)) {
                    goto EXIT;    
                }
            }
			
            Status = gBS->HandleProtocol (
				OpenInfoBuffer[Dindex].AgentHandle,
				&gEfiComponentNameProtocolGuid,
				(VOID **)&ComponentName
			);
            if (!EFI_ERROR(Status)) {
                Status = ComponentName->GetControllerName (                
                	ComponentName,
                    ControllerHandle,
                    OpenInfoBuffer[Dindex].ControllerHandle,
                    ComponentName->SupportedLanguages,
                    ControllerName
                );
                if (!EFI_ERROR(Status)) {
                    goto EXIT;    
                }
            }
        }
    }  
    EXIT:
    if(HandleBuffer != NULL) {
        FreePool(HandleBuffer);
    }
    if(OpenInfoBuffer != NULL) {
        FreePool(OpenInfoBuffer);
    }    
    return Status;
}
