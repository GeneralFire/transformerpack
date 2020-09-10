//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2020, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************
/** @file AmiIScsi.c
    Provides the AMI Override functions for the IScsi EDKII driver 

**/

#include "AmiIScsi.h"

#define  ValidateUnicodeChar(Char) ( (Char >= '0') && (Char <= 'F') )
extern BOOLEAN SameNicIndex;

#if (ENABLE_CHAP_SECRET_ENCRYPTION)
#include <AmiIScsiElink.h>
typedef  EFI_STATUS (CHAP_SECRET_ENCRYPT_HOOK)(CHAR8 *CHAPSecret,UINT8 *CHAPLen,CHAR8 *RevCHAPSecret,UINT8 *RevCHAPLen,UINT8 AttemptID,UINT8 OpCode);
extern   CHAP_SECRET_ENCRYPT_HOOK CHAP_SECRET_ENCRYPTION_MGMT_HOOK_LIST EndOfChapSecretMgmtHook;
CHAP_SECRET_ENCRYPT_HOOK* ChapSecretMgmtHookList[] = {CHAP_SECRET_ENCRYPTION_MGMT_HOOK_LIST NULL};
void  * memset (void *dest, char ch, unsigned int count);
#endif

#if (ENABLE_CHAP_SECRET_ENCRYPTION)
/**
  This function will be replaced by the hooks if any.

    @param[in]  CHAPSecret      The CHAP secret of the attempt.
    @param[in]  CHAPLen         The length of the CHAP secret.
    @param[in]  RevCHAPSecret   The reverse CHAP secret of the attempt.
    @param[in]  RevCHAPLen      The length of the reverse CHAP secret.
    @param[in]  AttemptID       The unique ID of the attempt.
    @param[in]  OpCode          The type of operation to be performed.

    @retval[out] EFI_UNSUPPORTED        Default return type.
**/

EFI_STATUS DefaultIscsiManageEncryptionHook (
    CHAR8   *ChapSecret,
    UINT8   *ChapLen,
    CHAR8   *RevChapSecret,
    UINT8   *RevChapLen,
    UINT8   AttemptId,
    UINT8   OpCode
)
{
    // This is just a dummy function to support hook implementation.
    // The real hook should be able to override this dummy function
    // and it has to define its own encryption/decryption/storage logics.
    return EFI_UNSUPPORTED;
}

/**
  This function determines whether the decrypted secret meets the required length.

  @param[in]    ChapData        The attempt's CHAP data.

  @retval[out]  TRUE            If the Chap secrets meets the requirements.
  @retval[out]  FALSE           If the Chap secrets do not meet the requirements.

**/
BOOLEAN IsValidChapLength (
    ISCSI_CHAP_AUTH_CONFIG_NVDATA  ChapData
)
{
    BOOLEAN BoolStatus1 = FALSE;
    BOOLEAN BoolStatus2 = FALSE;
    
    if((AsciiStrLen(ChapData.CHAPSecret) >= ISCSI_CHAP_SECRET_MIN_LEN) && 
            (AsciiStrLen(ChapData.CHAPSecret) <= ISCSI_CHAP_SECRET_MAX_LEN))
        BoolStatus1 = TRUE;
    // if chap type is mutual, then validate the length of the reverse chap secret also.
    if(ChapData.CHAPType == ISCSI_CHAP_MUTUAL) {
        if((AsciiStrLen(ChapData.ReverseCHAPSecret) >= ISCSI_CHAP_SECRET_MIN_LEN) && 
                (AsciiStrLen(ChapData.ReverseCHAPSecret) <= ISCSI_CHAP_SECRET_MAX_LEN))
            BoolStatus2 = TRUE;
        if(BoolStatus1 && BoolStatus2)
            return TRUE;
    }
    return BoolStatus1;
}

/**
  This function will be called for all the chap secret manipulations. This function
  triggers the hook.

    @param[in]  Attempt                 The iSCSI attempt configuration data.
    @param[in]  OpCode                  The type of operation to be performed.

    @retval EFI_SUCCESS                 In case of encryption, if the encryption is success.
                                        In case of decryption, if the decryption is success.
                                        In case of clearing an entry, if the attempt's encrypted secret is deleted.
                                        In case of clearing all attempts, if all the attempts' encrypted secrets are deleted.
                                        
    @retval EFI_UNSUPPORTED             If encryption is not supported.
    @retval EFI_NOT_FOUND               If the unique ID is not found in the storage list.
**/
EFI_STATUS PrepareToEncrypt (
    ISCSI_ATTEMPT_CONFIG_NVDATA  *Attempt, 
    UINT8 OpCode
)
{
    EFI_STATUS  Status = EFI_UNSUPPORTED;
    UINTN       List = 0;
    // send the required copies of the attempt
    CHAR8       ChapCopy[ISCSI_CHAP_SECRET_STORAGE] = {0};
    CHAR8       RevChapCopy[ISCSI_CHAP_SECRET_STORAGE] = {0};
    UINT8       ChapLen = 0;
    UINT8       RevChapLen = 0;
    UINT8       AttemptID = 0;
    
    // if the Authentication type is none, then return EFI_UNSUPPORTED from here
    if(((OpCode != ClearAllEntries && OpCode != ClearAnEntry)) && Attempt->AuthenticationType == 0)
        return EFI_UNSUPPORTED;
    
    // if the OpCode is other than ClearAllEntries, save the AttemptID
    if(OpCode != ClearAllEntries)
        AttemptID = Attempt->AttemptConfigIndex;
    
    for (List = 0; ChapSecretMgmtHookList[List] && (Status != EFI_SUCCESS); List++) {
        if(OpCode == ClearAllEntries) {
            // if all entries has to be cleared, pass 0 as AttemptID.
            Status = ChapSecretMgmtHookList[List](NULL, 0, NULL, 0, 0, OpCode);
        } else if(OpCode == ClearAnEntry) {
            // call with AttemptID if a particular entry has to be cleared.
            Status = ChapSecretMgmtHookList[List](NULL, 0, NULL, 0, AttemptID, OpCode);
        } else {
            ChapLen = (UINT8)AsciiStrLen(Attempt->AuthConfigData.CHAP.CHAPSecret);
            ChapLen++;  // includes NULL terminator
            CopyMem(ChapCopy,Attempt->AuthConfigData.CHAP.CHAPSecret,ChapLen);
            // If chap type is MUTUAL, only then copy the details.
             if(Attempt->AuthConfigData.CHAP.CHAPType == ISCSI_CHAP_MUTUAL) {
                RevChapLen = (UINT8)AsciiStrLen(Attempt->AuthConfigData.CHAP.ReverseCHAPSecret);
                RevChapLen++;   // includes NULL terminator
                CopyMem(RevChapCopy,Attempt->AuthConfigData.CHAP.ReverseCHAPSecret,RevChapLen);
             }
            Status = ChapSecretMgmtHookList[List]((char*)ChapCopy, &ChapLen, (char*)RevChapCopy, &RevChapLen, AttemptID, OpCode);
        }
        switch(OpCode) {
        case EncryptSecrets:
            // if the status returned is other than EFI_UNSUPPORTED, NULL the secrets.
            if(Status != EFI_UNSUPPORTED) {
                memset(Attempt->AuthConfigData.CHAP.CHAPSecret,0,ISCSI_CHAP_SECRET_STORAGE);
                memset(Attempt->AuthConfigData.CHAP.ReverseCHAPSecret,0,ISCSI_CHAP_SECRET_STORAGE);
            }
            break;
        case DecryptSecrets:
            if(!EFI_ERROR(Status)) {
                // appending NULL at the end in case the decryption fails.
                ChapCopy[ISCSI_CHAP_SECRET_STORAGE-1] = '\0';
                CopyMem(Attempt->AuthConfigData.CHAP.CHAPSecret,ChapCopy,ISCSI_CHAP_SECRET_STORAGE);
                if(Attempt->AuthConfigData.CHAP.CHAPType == ISCSI_CHAP_MUTUAL) {
                    CopyMem(Attempt->AuthConfigData.CHAP.ReverseCHAPSecret,RevChapCopy,ISCSI_CHAP_SECRET_STORAGE);
                }
                // Check the lengths of the secrets.

                if(!IsValidChapLength(Attempt->AuthConfigData.CHAP)) {
                    // either chap or reverse chap secret doesn't meet the requirement,
                    // NULL both the secrets so the connection will fail.
                    memset(Attempt->AuthConfigData.CHAP.CHAPSecret,0,ISCSI_CHAP_SECRET_STORAGE);
                    memset(Attempt->AuthConfigData.CHAP.ReverseCHAPSecret,0,ISCSI_CHAP_SECRET_STORAGE);
                }
            }
            break;
        default:
            break;  
        }
    }
    return Status;
}
#endif

/**
  Function to set default Priority when user havent set any priority.

  @retval EFI_OUT_OF_RESOURCES   Does not have sufficient resources to finish this
                                 operation.
  @retval EFI_NOT_FOUND          Cannot find the corresponding variable.
  @retval EFI_SUCCESS            The operation is completed successfully.

**/
EFI_STATUS
IScsiSetDefaultPriority (
)
{
    EFI_STATUS                  Status = EFI_SUCCESS;
    UINT8                       Index;
    UINT8                       NewIndex = 0;
    UINT8                       Type;
    UINTN                       TotalNumber;
    UINT8                       *PriorityOrder;
    UINTN                       PriorityOrderSize;
    UINT8                       *PriorityOrderTmp;
    UINT8                       AttemptConfigIndex;
    ISCSI_ATTEMPT_CONFIG_NVDATA *AttemptConfigData;
    UINT8                       *AttemptConfigOrder;
    UINT8                       *AttemptConfigOrderTmp;
    UINTN                       AttemptConfigOrderSize;
    CHAR16                      AttemptName[ISCSI_NAME_IFR_MAX_SIZE] = {0};
    UINTN                       Size = 0;
    for (Type =1; Type <= ISCSI_MAX_ATTEMPT_TYPE; Type ++) {
        PriorityOrder = IScsiGetVariableAndSize (
                               L"PriorityOrder",
                               &gIScsiConfigGuid,
                               &PriorityOrderSize
                               );
        TotalNumber = PriorityOrderSize / sizeof (UINT8);
        if (TotalNumber == ISCSI_MAX_ATTEMPT_TYPE) {
            break;
        }
        TotalNumber++;

        PriorityOrderTmp = AllocateZeroPool (TotalNumber * sizeof (UINT8));
        if (PriorityOrderTmp == NULL) {
            return EFI_OUT_OF_RESOURCES;
        }
        if (PriorityOrder != NULL) {
            CopyMem (PriorityOrderTmp, PriorityOrder, PriorityOrderSize);
            FreePool (PriorityOrder);
        }
        PriorityOrderTmp[TotalNumber-1] = Type;
        PriorityOrder                   = PriorityOrderTmp;
        PriorityOrderSize               = (TotalNumber) * sizeof (UINT8);
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
    }
    AttemptConfigOrder = IScsiGetVariableAndSize (
                           L"AttemptOrder",
                           &gIScsiConfigGuid,
                           &AttemptConfigOrderSize
                           );
    if (AttemptConfigOrder == NULL) {
        return EFI_SUCCESS;
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
    for (Type = 0; Type < ISCSI_MAX_ATTEMPT_TYPE; Type++) {
        //
        // The real content ends with 0.
        //
        for(Index = 0; Index < AttemptConfigOrderSize; Index ++) {
            AttemptConfigIndex = AttemptConfigOrder[Index];
            UnicodeSPrint (
                    AttemptName,
                    (UINTN) ISCSI_NAME_IFR_MAX_SIZE,
                    L"Attempt %d",
                    (UINTN) AttemptConfigIndex
            );            
            Status = GetVariable2(AttemptName,
                    &gEfiIScsiInitiatorNameProtocolGuid,
                    (void**)&AttemptConfigData, 
                    &Size
            );
            if (AttemptConfigData == NULL) {
                continue;
            }
            if (AttemptConfigData->AttemptType == PriorityOrder[Type] ) {
                //
                // Reorder the Attempt List.
                //

                AttemptConfigOrderTmp[NewIndex] = AttemptConfigIndex;
                NewIndex++;
                //
                // Mark it to be deleted - 0.
                //
                AttemptConfigOrder[Index] = 0;
            }
        }
    }
    //
    // Adjust the attempt order in NVR.
    //
    for (; NewIndex < AttemptConfigOrderSize / sizeof (UINT8); NewIndex++) {
        for (Index = 0; Index < AttemptConfigOrderSize / sizeof (UINT8); Index++) {
            if (AttemptConfigOrder[Index] != 0) {
                AttemptConfigOrderTmp[NewIndex] = AttemptConfigOrder[Index];
                AttemptConfigOrder[Index]   = 0;
                break;
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
  Function to allow multiple targets on a NIC without MPIO enabled

  @param[out] UINTN    Returns the valid Target count.

**/
UINTN
ConfigureMultipleTargets (
)
{
    UINTN                        NumOffset;
    LIST_ENTRY                   *Entry;
    ISCSI_ATTEMPT_CONFIG_NVDATA  *AttemptConfigData;
    UINTN                         ValidTargetsCount = 0;
    
    // Fix to allow configuring multiple targets on a NIC without MPIO enabled
    if (PcdGetBool(PcdAllowMultipleTargetsOnNIC)) {           
        NET_LIST_FOR_EACH (Entry, &mPrivate->AttemptConfigs) {
            AttemptConfigData = NET_LIST_USER_STRUCT (Entry, ISCSI_ATTEMPT_CONFIG_NVDATA, Link);
            if (AttemptConfigData->ValidiBFTPath == TRUE) {
                ValidTargetsCount++;
            }  
        }
        NumOffset = 2 * ValidTargetsCount;
    } else {
        NumOffset = 2 * mPrivate->ValidSinglePathCount;
    }
    return NumOffset;
}

/**
  Get the various configuration data.

  @param[in]  Private   The iSCSI driver data.
  @param[in]  SameNicIndex   Boolean for checking same NIC

  @retval EFI_SUCCESS            The configuration data is retrieved.
  @retval EFI_NOT_FOUND          This iSCSI driver is not configured yet.
  @retval EFI_OUT_OF_RESOURCES   Failed to allocate memory.

**/
EFI_STATUS
AmiIScsiGetConfigData (
    IN ISCSI_DRIVER_DATA  *Private
)
{
    EFI_STATUS Status;
    CHAR16                      MacString[ISCSI_MAX_MAC_STRING_LEN];
    CHAR16                      AttemptMacString[ISCSI_MAX_MAC_STRING_LEN];
    UINTN                       Index;
    ISCSI_NIC_INFO              *NicInfo;
    ISCSI_ATTEMPT_CONFIG_NVDATA *AttemptConfigData;
    ISCSI_ATTEMPT_CONFIG_NVDATA *AttemptTmp;
    UINT8                       *AttemptConfigOrder;
    UINTN                       AttemptConfigOrderSize;
    CHAR16                      IScsiMode[64];
    CHAR16                      IpMode[64];  
    CHAR8                       *AllocatedLanguages = NULL;
    CHAR8                       *Language = NULL;
    CHAR16                      *CharPtr= NULL;
    UINT8                       OldMpioCount = mPrivate->MpioCount;
    UINT8                       OldSinglePathCount = mPrivate->SinglePathCount;
    CHAR16                      MacStringStr[ISCSI_MAX_MAC_STRING_LEN];
#if (RSD_ISCSI_SUPPORT)
    EFI_STATUS                  RsdIScsiStatus = EFI_NOT_FOUND;   
    
    // Initialize RSD configuration Data
    if (!RsdIScsiIsBmcDataAvailable()) { 
        RsdIScsiStatus = RsdIScsiInitializeBmcInformation();
        DEBUG((DEBUG_INFO,"\nRSD_ISCSI_2_2:In %a, RsdIScsiInitializeBmcInformation Status = %r\n", __FUNCTION__, RsdIScsiStatus));
    }
#endif
    // There should be at least one attempt configured.
    //
    AttemptConfigOrder = IScsiGetVariableAndSize (
                             L"AttemptOrder",
                             &gIScsiConfigGuid,
                             &AttemptConfigOrderSize
                             );
    if (AttemptConfigOrder == NULL || AttemptConfigOrderSize == 0) {
        return EFI_NOT_FOUND;
    }

    //
    // Get the iSCSI Initiator Name.
    //
    mPrivate->InitiatorNameLength  = ISCSI_NAME_MAX_SIZE;
    Status = gIScsiInitiatorName.Get (
                                   &gIScsiInitiatorName,
                                   &mPrivate->InitiatorNameLength,
                                   mPrivate->InitiatorName
                                   );

#if (REDFISH_ISCSI_SUPPORT || RSD_ISCSI_SUPPORT)
    if(
        #if (REDFISH_ISCSI_SUPPORT && RSD_ISCSI_SUPPORT)
        (!gRedfishIScsiDataAvailable) && (!RsdIScsiIsBmcDataAvailable())
        #elif (REDFISH_ISCSI_SUPPORT)       
        !gRedfishIScsiDataAvailable 
        #elif (RSD_ISCSI_SUPPORT)     
        !RsdIScsiIsBmcDataAvailable()
        #endif
        ) {         
#endif
        if (EFI_ERROR(Status)) {
            return Status;
        }
#if (REDFISH_ISCSI_SUPPORT || RSD_ISCSI_SUPPORT )
    }
#endif
    // Get the normal configuration.
    //
    for (Index = 0; Index < AttemptConfigOrderSize / sizeof (UINT8); Index++) {

        //
        // Check whether the attempt exists in AttemptConfig.
        //
        AttemptTmp = IScsiConfigGetAttemptByConfigIndex (AttemptConfigOrder[Index]);
        if (AttemptTmp != NULL && AttemptTmp->SessionConfigData.Enabled == ISCSI_DISABLED) {
            continue;
        } else if (AttemptTmp != NULL && AttemptTmp->SessionConfigData.Enabled != ISCSI_DISABLED) {
            //
            // Check the autoconfig path to see whether it should be retried.
            //
            if (AttemptTmp->SessionConfigData.IpMode == IP_MODE_AUTOCONFIG &&
                !AttemptTmp->AutoConfigureSuccess) {
                if (mPrivate->Ipv6Flag &&
                    AttemptTmp->AutoConfigureMode == IP_MODE_AUTOCONFIG_IP6) {
                    //
                    // Autoconfigure for IP6 already attempted but failed. Do not try again.
                    //
                    continue;
                } else if (!mPrivate->Ipv6Flag &&
                    AttemptTmp->AutoConfigureMode == IP_MODE_AUTOCONFIG_IP4) {
                    //
                    // Autoconfigure for IP4  already attempted but failed. Do not try again.
                    //
                    continue;
                  } else {
                      //
                      // Try another approach for this autoconfigure path.
                      //
                      AttemptTmp->AutoConfigureMode =
                      (UINT8) (mPrivate->Ipv6Flag ? IP_MODE_AUTOCONFIG_IP6 : IP_MODE_AUTOCONFIG_IP4);
                      AttemptTmp->SessionConfigData.InitiatorInfoFromDhcp = TRUE;
                      AttemptTmp->SessionConfigData.TargetInfoFromDhcp    = TRUE;
                      AttemptTmp->DhcpSuccess                             = FALSE;

                      //
                      // Get some information from the dhcp server.
                      //
                      if (!mPrivate->Ipv6Flag) {  
                          Status = IScsiDoDhcp (Private->Image, Private->Controller, AttemptTmp);
                          if (!EFI_ERROR (Status)) {
                              AttemptTmp->DhcpSuccess = TRUE;
                          }
                      } else {
                          Status = IScsiDoDhcp6 (Private->Image, Private->Controller, AttemptTmp);
                          if (!EFI_ERROR (Status)) {
                              AttemptTmp->DhcpSuccess = TRUE;
                      }
                  }

                  //
                  // Refresh the state of this attempt to NVR.
                  //
                  UnicodeSPrint (
                  mPrivate->PortString,
                  (UINTN) ISCSI_NAME_IFR_MAX_SIZE,
                  L"Attempt %d",
                  (UINTN) AttemptTmp->AttemptConfigIndex
                  );           
#if(ENABLE_CHAP_SECRET_ENCRYPTION)
                  Status = PrepareToEncrypt(AttemptTmp,EncryptSecrets);
#endif


                  gRT->SetVariable (
                     mPrivate->PortString,
                     &gEfiIScsiInitiatorNameProtocolGuid,
                     ISCSI_CONFIG_VAR_ATTR,
                     sizeof (ISCSI_ATTEMPT_CONFIG_NVDATA),
                     AttemptTmp
                     );
#if(ENABLE_CHAP_SECRET_ENCRYPTION)
                  Status = PrepareToEncrypt(AttemptTmp,DecryptSecrets);
#endif

                  continue;
              }
          } else if (AttemptTmp->SessionConfigData.InitiatorInfoFromDhcp &&
                     !AttemptTmp->ValidPath &&
                     AttemptTmp->NicIndex == mPrivate->CurrentNic) {
              //
              // If the attempt associates with the current NIC, we can
              // get DHCP information for already added, but failed, attempt.
              //
              AttemptTmp->DhcpSuccess = FALSE;
              if (!mPrivate->Ipv6Flag && (AttemptTmp->SessionConfigData.IpMode == IP_MODE_IP4)) {
                  Status = IScsiDoDhcp (Private->Image, Private->Controller, AttemptTmp);
                  if (!EFI_ERROR (Status)) {
                      AttemptTmp->DhcpSuccess = TRUE;
                  }
              } else if (mPrivate->Ipv6Flag && (AttemptTmp->SessionConfigData.IpMode == IP_MODE_IP6)) {
                  Status = IScsiDoDhcp6 (Private->Image, Private->Controller, AttemptTmp);
                  if (!EFI_ERROR (Status)) {
                      AttemptTmp->DhcpSuccess = TRUE;
                  }
              }

              //
              // Refresh the state of this attempt to NVR.
              //
              UnicodeSPrint (
              mPrivate->PortString,
              (UINTN) ISCSI_NAME_IFR_MAX_SIZE,
              L"Attempt %d",
              (UINTN) AttemptTmp->AttemptConfigIndex
              );         
#if(ENABLE_CHAP_SECRET_ENCRYPTION)    
              Status = PrepareToEncrypt(AttemptTmp,EncryptSecrets);
#endif

              gRT->SetVariable (
                   mPrivate->PortString,
                   &gEfiIScsiInitiatorNameProtocolGuid,
                   ISCSI_CONFIG_VAR_ATTR,
                   sizeof (ISCSI_ATTEMPT_CONFIG_NVDATA),
                   AttemptTmp);
        
#if(ENABLE_CHAP_SECRET_ENCRYPTION)
              Status = PrepareToEncrypt(AttemptTmp,DecryptSecrets);
#endif

              continue;

          } else {
              continue;
          }
      }

      //
      // This attempt does not exist in AttemptConfig. Try to add a new one.
      //

      NicInfo = IScsiGetNicInfoByIndex (mPrivate->CurrentNic);
      ASSERT (NicInfo != NULL);
      IScsiMacAddrToStr (&NicInfo->PermanentAddress, NicInfo->HwAddressSize, NicInfo->VlanId, MacString);
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
      if (AttemptConfigData->AttemptType == RSD_ATTEMPT || AttemptConfigData->AttemptType == REDFISH_ATTEMPT) {
          IScsiMacAddrToStr ((EFI_MAC_ADDRESS *)AttemptConfigData->MacString,NET_ETHER_ADDR_LEN,0, MacStringStr);
          if (AttemptConfigData == NULL || StrCmp (MacString, MacStringStr)) {
               continue;
          }  
      } else {
          AsciiStrToUnicodeStrS (AttemptConfigData->MacString, AttemptMacString, sizeof (AttemptMacString) / sizeof (AttemptMacString[0]));
          if ((AttemptConfigData == NULL) || ((AttemptConfigData->AttemptType == SETUP_ATTEMPT) &&
             ((AttemptConfigData->Actived == ISCSI_ACTIVE_DISABLED)|| (StrCmp (MacString, AttemptMacString))))) {
              continue;
          }
      }
      //Mulitple initiator name is supported only in redfish support. In normal iscsi and rsd iscsi, only single initiator name is allowed.
      //Initiator name specific to the current MAC and attempt is saved in Nvram. Extract nvram data and respective initiator name.          
#if (REDFISH_ISCSI_SUPPORT)
      if(gRedfishIScsiDataAvailable) {
          if (AttemptConfigData->AttemptType == REDFISH_ATTEMPT) {
               Status = RedfishMultipleInitiatorName(Private->Controller,AttemptConfigData);
               if (EFI_ERROR (Status)) {
                   return Status;
               }
          }
      }
#endif
#if (RSD_ISCSI_SUPPORT)
      if(RsdIScsiIsBmcDataAvailable()) {
          if(AttemptConfigData->AttemptType == RSD_ATTEMPT) {
              // Use RSD interface to get the InitiatorName
              AsciiStrCpy(mPrivate->InitiatorName, gRsdIScsiInitiatorName);
              Status = EFI_SUCCESS;
          }
      }
#endif 
   
#if(ENABLE_CHAP_SECRET_ENCRYPTION)
      Status = PrepareToEncrypt(AttemptConfigData,DecryptSecrets);
#endif

            
#if( (!RSD_ISCSI_SUPPORT) && (!REDFISH_ISCSI_SUPPORT))  
      ASSERT (AttemptConfigOrder[Index] == AttemptConfigData->AttemptConfigIndex);
#endif
      AttemptConfigData->NicIndex      = NicInfo->NicIndex;
      AttemptConfigData->DhcpSuccess   = FALSE;
        
      if(AttemptConfigData->SessionConfigData.InitiatorInfoFromDhcp) {
          //Initiator IP is displayed if attempt is created for connection-less NIC.
          ZeroMem(&AttemptConfigData->SessionConfigData.LocalIp,sizeof(EFI_IP_ADDRESS));                  
      }
      AttemptConfigData->ValidiBFTPath = (BOOLEAN) (mPrivate->EnableMpio ? TRUE : FALSE);
      AttemptConfigData->ValidPath     = FALSE;

      if (AttemptConfigData->SessionConfigData.IpMode == IP_MODE_AUTOCONFIG) {
          AttemptConfigData->SessionConfigData.InitiatorInfoFromDhcp = TRUE;
          AttemptConfigData->SessionConfigData.TargetInfoFromDhcp    = TRUE;

          AttemptConfigData->AutoConfigureMode =
            (UINT8) (mPrivate->Ipv6Flag ? IP_MODE_AUTOCONFIG_IP6 : IP_MODE_AUTOCONFIG_IP4);
          AttemptConfigData->AutoConfigureSuccess = FALSE;
      }

      //
      // Get some information from dhcp server.
      //
      if (AttemptConfigData->SessionConfigData.Enabled != ISCSI_DISABLED &&
            AttemptConfigData->SessionConfigData.InitiatorInfoFromDhcp) {

          if (!mPrivate->Ipv6Flag &&
              (AttemptConfigData->SessionConfigData.IpMode == IP_MODE_IP4 ||
               AttemptConfigData->AutoConfigureMode == IP_MODE_AUTOCONFIG_IP4)) {
              Status = IScsiDoDhcp (Private->Image, Private->Controller, AttemptConfigData);
              if (!EFI_ERROR (Status)) {
                  AttemptConfigData->DhcpSuccess = TRUE;
              }
          } else if (mPrivate->Ipv6Flag &&
                    (AttemptConfigData->SessionConfigData.IpMode == IP_MODE_IP6 ||
                     AttemptConfigData->AutoConfigureMode == IP_MODE_AUTOCONFIG_IP6)) {
              Status = IScsiDoDhcp6 (Private->Image, Private->Controller, AttemptConfigData);
              if (!EFI_ERROR (Status)) {
                  AttemptConfigData->DhcpSuccess = TRUE;
              }
          }

          //
          // Refresh the state of this attempt to NVR.
          //
          UnicodeSPrint (
            mPrivate->PortString,
            (UINTN) ISCSI_NAME_IFR_MAX_SIZE,
            L"Attempt %d",
            (UINTN) AttemptConfigData->AttemptConfigIndex
            );       
#if(ENABLE_CHAP_SECRET_ENCRYPTION)
          Status = PrepareToEncrypt(AttemptConfigData,EncryptSecrets);
#endif

          gRT->SetVariable (
                       mPrivate->PortString,
                       &gEfiIScsiInitiatorNameProtocolGuid,
                       ISCSI_CONFIG_VAR_ATTR,
                       sizeof (ISCSI_ATTEMPT_CONFIG_NVDATA),
                       AttemptConfigData
                       );


      
#if(ENABLE_CHAP_SECRET_ENCRYPTION)
          Status = PrepareToEncrypt(AttemptConfigData,DecryptSecrets);
#endif
        }

        //
        // Update Attempt Help Info.
        //

        if (AttemptConfigData->SessionConfigData.Enabled == ISCSI_DISABLED) {
            CharPtr=NetLibHiiGetString(mCallbackInfo->RegisteredHandle,STRING_TOKEN(STR_ISCSI_MODE_DISABLED),NULL);
            UnicodeSPrint (IScsiMode, 64, CharPtr);
        } else if (AttemptConfigData->SessionConfigData.Enabled == ISCSI_ENABLED) {
            CharPtr=NetLibHiiGetString(mCallbackInfo->RegisteredHandle,STRING_TOKEN(STR_ISCSI_MODE_ENABLED),NULL);
            UnicodeSPrint (IScsiMode, 64, CharPtr);
        } else if (AttemptConfigData->SessionConfigData.Enabled == ISCSI_ENABLED_FOR_MPIO) {
            CharPtr=NetLibHiiGetString(mCallbackInfo->RegisteredHandle,STRING_TOKEN(STR_ISCSI_MODE_ENABLED_FOR_MPIO),NULL);
            UnicodeSPrint (IScsiMode, 64, CharPtr);
        }    
        //
        // Release the string pointer which is pointed in if or else if blocks.
        // 
        if(CharPtr != NULL) {   
            FreePool (CharPtr);
            CharPtr = NULL;
        }
        if (AttemptConfigData->SessionConfigData.IpMode == IP_MODE_IP4) {
            CharPtr=NetLibHiiGetString(mCallbackInfo->RegisteredHandle,STRING_TOKEN(STR_IP_MODE_IP4),NULL);
            UnicodeSPrint (IpMode, 64, CharPtr);
        } else if (AttemptConfigData->SessionConfigData.IpMode == IP_MODE_IP6) {    
            CharPtr=NetLibHiiGetString(mCallbackInfo->RegisteredHandle,STRING_TOKEN(STR_IP_MODE_IP6),NULL);
            UnicodeSPrint (IpMode, 64, CharPtr);
        } else if (AttemptConfigData->SessionConfigData.IpMode == IP_MODE_AUTOCONFIG) {
            CharPtr=NetLibHiiGetString(mCallbackInfo->RegisteredHandle,STRING_TOKEN(STR_IP_MODE_AUTOCONFIG),NULL);
            UnicodeSPrint (IpMode, 64, CharPtr);
        }    
        //
        // Release the string pointer which is pointed in if or else if blocks.
        // 
         if(CharPtr != NULL) {     
             FreePool (CharPtr);
             CharPtr = NULL;
         }
         AttemptConfigData->AttemptTitleHelpToken = 0;
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
                     
                 AttemptConfigData->AttemptTitleHelpToken = HiiSetString (
                         mCallbackInfo->RegisteredHandle,
                         AttemptConfigData->AttemptTitleHelpToken,
                         mPrivate->PortString,
                         Language
                 );
             }
         } 
         if(CharPtr != NULL) {
             FreePool (CharPtr);
             CharPtr=NULL;
         }
        if (AttemptConfigData->AttemptTitleHelpToken == 0) {
            return EFI_OUT_OF_RESOURCES;
        }

        //
        // Record the attempt in global link list.
        //
#if(ENABLE_CHAP_SECRET_ENCRYPTION)
        // Here the attempt count gets incremented. So, if CMOS is removed, all the attempts get deleted at once.
        // With the following condition, we can trigger the user hook to delete all the entries.
        if(mPrivate->AttemptCount == 0)
            Status = PrepareToEncrypt(NULL,ClearAllEntries);
#endif
        InsertTailList (&mPrivate->AttemptConfigs, &AttemptConfigData->Link);
        mPrivate->AttemptCount++;

        if (AttemptConfigData->SessionConfigData.Enabled == ISCSI_ENABLED_FOR_MPIO) {
            mPrivate->MpioCount++;
            mPrivate->EnableMpio = TRUE;

          if (AttemptConfigData->AuthenticationType == ISCSI_AUTH_TYPE_KRB) {
              mPrivate->Krb5MpioCount++;
          }
        } else if (AttemptConfigData->SessionConfigData.Enabled == ISCSI_ENABLED) {
            mPrivate->SinglePathCount++;
        }
      }
       
      // Either MpioCount or SinglePathCount variable will be changed if attempt is available for this NIC
      // Case-1: These values will not be changed if the attempt is not available for this NIC. - return NOT found
      // Case-2: These values will not be changed when the attempt is available and configured in the first call. - do not return.
      //
      if ( !SameNicIndex && (OldMpioCount == mPrivate->MpioCount) && \
             (OldSinglePathCount == mPrivate->SinglePathCount) ) {
          return EFI_NOT_FOUND;
      }
      //
      // Reorder the AttemptConfig by the configured order.
      //
      for (Index = 0; Index < AttemptConfigOrderSize / sizeof (UINT8); Index++) {
          AttemptConfigData = IScsiConfigGetAttemptByConfigIndex (AttemptConfigOrder[Index]);
          if (AttemptConfigData == NULL) {
              continue;
          }

          RemoveEntryList (&AttemptConfigData->Link);
          InsertTailList (&mPrivate->AttemptConfigs, &AttemptConfigData->Link);
      }  
      //
      // Update the Main Form.
      //
      IScsiConfigUpdateAttempt ();

      FreePool (AttemptConfigOrder);  
      //
      //  There should be at least one attempt configuration.
      //
      if (!mPrivate->EnableMpio) {
          if (mPrivate->SinglePathCount == 0) {
              return EFI_NOT_FOUND;
          }
          mPrivate->ValidSinglePathCount = mPrivate->SinglePathCount;
      }

      return EFI_SUCCESS;
}

/**
  Start to manage the controller. This is the worker function for
  IScsiIp4(6)DriverBindingStart.

  @param[in]  Image                Handle of the image.
  @param[in]  ControllerHandle     Handle of the controller.
  @param[in]  IpVersion            IP_VERSION_4 or IP_VERSION_6.

  @retval EFI_SUCCES            This driver was started.
  @retval EFI_ALREADY_STARTED   This driver is already running on this device.
  @retval EFI_INVALID_PARAMETER Any input parameter is invalid.
  @retval EFI_NOT_FOUND         There is no sufficient information to establish
                                the iScsi session.
  @retval EFI_OUT_OF_RESOURCES  Failed to allocate memory.
  @retval EFI_DEVICE_ERROR      Failed to get TCP connection device path.
  @retval EFI_ACCESS_DENIED     The protocol could not be removed from the Handle
                                because its interfaces are being used.

**/
EFI_STATUS
AmiIScsiStart (
    IN EFI_HANDLE                   Image,
    IN EFI_HANDLE                   ControllerHandle,
    IN UINT8                        IpVersion
)
{
    EFI_STATUS                      Status;
    ISCSI_DRIVER_DATA               *Private;
    LIST_ENTRY                      *Entry;
    LIST_ENTRY                      *NextEntry;
    ISCSI_ATTEMPT_CONFIG_NVDATA     *AttemptConfigData;
    ISCSI_SESSION                   *Session;
    UINT8                           Index;
    EFI_EXT_SCSI_PASS_THRU_PROTOCOL *ExistIScsiExtScsiPassThru;
    ISCSI_DRIVER_DATA               *ExistPrivate;
    UINT8                           *AttemptConfigOrder;
    UINTN                           AttemptConfigOrderSize;
    UINT8                           BootSelected;
    EFI_HANDLE                      *HandleBuffer;
    UINTN                           NumberOfHandles;
    EFI_DEVICE_PATH_PROTOCOL        *DevicePath;
    EFI_GUID                        *IScsiPrivateGuid;
    EFI_GUID                        *TcpServiceBindingGuid;
    BOOLEAN                         NeedUpdate = FALSE;
    VOID                            *Interface;
    EFI_GUID                        *ProtocolGuid;
    UINT8                           NetworkBootPolicy;
    ISCSI_SESSION_CONFIG_NVDATA     *NvData;

    //
    // Test to see if iSCSI driver supports the given controller.
    //

    if (IpVersion == IP_VERSION_4) {
        IScsiPrivateGuid      = &gIScsiV4PrivateGuid;
        TcpServiceBindingGuid = &gEfiTcp4ServiceBindingProtocolGuid;
        ProtocolGuid          = &gEfiTcp4ProtocolGuid;
    } else if (IpVersion == IP_VERSION_6) {
        IScsiPrivateGuid      = &gIScsiV6PrivateGuid;
        TcpServiceBindingGuid = &gEfiTcp6ServiceBindingProtocolGuid;
        ProtocolGuid          = &gEfiTcp6ProtocolGuid;
    } else {
        return EFI_INVALID_PARAMETER;
    }

    Status = gBS->OpenProtocol (
                  ControllerHandle,
                  IScsiPrivateGuid,
                  NULL,
                  Image,
                  ControllerHandle,
                  EFI_OPEN_PROTOCOL_TEST_PROTOCOL
                  );
    if (!EFI_ERROR (Status)) {
        return EFI_ALREADY_STARTED;
    }

    Status = gBS->OpenProtocol (
                  ControllerHandle,
                  TcpServiceBindingGuid,
                  NULL,
                  Image,
                  ControllerHandle,
                  EFI_OPEN_PROTOCOL_TEST_PROTOCOL
                  );
    if (EFI_ERROR (Status)) {
        return EFI_UNSUPPORTED;
    }

    NetworkBootPolicy = PcdGet8 (PcdIScsiAIPNetworkBootPolicy);
    if (NetworkBootPolicy == ALWAYS_USE_ISCSI_HBA_AND_IGNORE_UEFI_ISCSI) {
        return EFI_ABORTED;
    }

    if (NetworkBootPolicy != ALWAYS_USE_UEFI_ISCSI_AND_IGNORE_ISCSI_HBA) {
        //
        // Check existing iSCSI AIP.
        //
        Status = IScsiCheckAip ();
        if (!EFI_ERROR (Status)) {
            //
            // Find iSCSI AIP with specified network boot policy. return EFI_ABORTED.
            //
            return EFI_ABORTED;
        }
    }

    //
    // Record the incoming NIC info.
    //
    if(mPrivate->MaxNic == 0)
    {
        Status = IScsiSetDefaultPriority();
    }
    Status = IScsiAddNic (ControllerHandle, Image);
    if (EFI_ERROR (Status)) {
        return Status;
    }

    //
   // Create the instance private data.
    //
    Private = IScsiCreateDriverData (Image, ControllerHandle);
    if (Private == NULL) {
        return EFI_OUT_OF_RESOURCES;
    }

    //
    // Create a underlayer child instance, but not need to configure it. Just open ChildHandle
    // via BY_DRIVER. That is, establishing the relationship between ControllerHandle and ChildHandle.
    // Therefore, when DisconnectController(), especially VLAN virtual controller handle,
    // IScsiDriverBindingStop() will be called.
    //
    Status = NetLibCreateServiceChild (
             ControllerHandle,
             Image,
             TcpServiceBindingGuid,
             &Private->ChildHandle
             );

    if (EFI_ERROR (Status)) {
        goto ON_ERROR;
    }

    Status = gBS->OpenProtocol (
                  Private->ChildHandle, /// Default Tcp child
                  ProtocolGuid,
                  &Interface,
                  Image,
                  ControllerHandle,
                  EFI_OPEN_PROTOCOL_BY_DRIVER
                  );

    if (EFI_ERROR (Status)) {
        goto ON_ERROR;
    }

    //
    // Always install private protocol no matter what happens later. We need to
    // keep the relationship between ControllerHandle and ChildHandle.
    //
    Status = gBS->InstallProtocolInterface (
                  &ControllerHandle,
                  IScsiPrivateGuid,
                  EFI_NATIVE_INTERFACE,
                  &Private->IScsiIdentifier
                  );
    if (EFI_ERROR (Status)) {
        goto ON_ERROR;
    }

    if (IpVersion == IP_VERSION_4) {
        mPrivate->Ipv6Flag = FALSE;
    } else {
        mPrivate->Ipv6Flag = TRUE;
    }

    //
    // Get the current iSCSI configuration data.
    //
    Status = AmiIScsiGetConfigData(Private);
    if (EFI_ERROR (Status)) {
        goto ON_ERROR;
    }

    //
    // If there is already a successul attempt, check whether this attempt is the
    // first "enabled for MPIO" attempt. If not, still try the first attempt.
    // In single path mode, try all attempts.
    //
    ExistPrivate = NULL;
    Status       = EFI_NOT_FOUND;

    if (mPrivate->OneSessionEstablished && mPrivate->EnableMpio) {
        AttemptConfigData = NULL;
        NET_LIST_FOR_EACH (Entry, &mPrivate->AttemptConfigs) {
            AttemptConfigData = NET_LIST_USER_STRUCT (Entry, ISCSI_ATTEMPT_CONFIG_NVDATA, Link);
            if (AttemptConfigData->SessionConfigData.Enabled == ISCSI_ENABLED_FOR_MPIO) {
                break;
            }
        }

        if (AttemptConfigData == NULL) {
            goto ON_ERROR;
        }

        if (AttemptConfigData->AttemptConfigIndex == mPrivate->BootSelectedIndex) {
            goto ON_EXIT;
        }

        //
        // Uninstall the original ExtScsiPassThru first.
        //

        //
        // Locate all ExtScsiPassThru protocol instances.
        //
        Status = gBS->LocateHandleBuffer (
                    ByProtocol,
                    &gEfiExtScsiPassThruProtocolGuid,
                    NULL,
                    &NumberOfHandles,
                    &HandleBuffer
                    );
        if (EFI_ERROR (Status)) {
            goto ON_ERROR;
        }

        //
        // Find ExtScsiPassThru protocol instance produced by this driver.
        //
        ExistIScsiExtScsiPassThru = NULL;
        for (Index = 0; Index < NumberOfHandles && ExistIScsiExtScsiPassThru == NULL; Index++) {
            Status = gBS->HandleProtocol (
                      HandleBuffer[Index],
                      &gEfiDevicePathProtocolGuid,
                      (VOID **) &DevicePath
                      );
            if (EFI_ERROR (Status)) {
                continue;
            }

            while (!IsDevicePathEnd (DevicePath)) {
                if ((DevicePath->Type == MESSAGING_DEVICE_PATH) && (DevicePath->SubType == MSG_MAC_ADDR_DP)) {
                    //
                    // Get the ExtScsiPassThru protocol instance.
                    //
                    Status = gBS->HandleProtocol (
                          HandleBuffer[Index],
                          &gEfiExtScsiPassThruProtocolGuid,
                          (VOID **) &ExistIScsiExtScsiPassThru
                          );
                    ASSERT_EFI_ERROR (Status);
                    break;
                }

                DevicePath = NextDevicePathNode (DevicePath);
            }
        }

        FreePool (HandleBuffer);

        if (ExistIScsiExtScsiPassThru == NULL) {
            Status = EFI_NOT_FOUND;
            goto ON_ERROR;
        }

        ExistPrivate = ISCSI_DRIVER_DATA_FROM_EXT_SCSI_PASS_THRU (ExistIScsiExtScsiPassThru);

        Status = gBS->UninstallProtocolInterface (
                    ExistPrivate->ExtScsiPassThruHandle,
                    &gEfiExtScsiPassThruProtocolGuid,
                    &ExistPrivate->IScsiExtScsiPassThru
                    );
        if (EFI_ERROR (Status)) {
            goto ON_ERROR;
        }
    }

    //
    // Install the Ext SCSI PASS THRU protocol.
    //
    Status = gBS->InstallProtocolInterface (
                  &Private->ExtScsiPassThruHandle,
                  &gEfiExtScsiPassThruProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  &Private->IScsiExtScsiPassThru
                  );
    if (EFI_ERROR (Status)) {
        goto ON_ERROR;
    }

    BootSelected = 0;

    NET_LIST_FOR_EACH_SAFE (Entry, NextEntry, &mPrivate->AttemptConfigs) {
        AttemptConfigData = NET_LIST_USER_STRUCT (Entry, ISCSI_ATTEMPT_CONFIG_NVDATA, Link);
        //
        // Don't process the attempt that does not associate with the current NIC or
        // this attempt is disabled or established.
        //
        if (AttemptConfigData->NicIndex != mPrivate->CurrentNic ||
            AttemptConfigData->SessionConfigData.Enabled == ISCSI_DISABLED ||
            AttemptConfigData->ValidPath) {
            continue;
        }

        //
        // In multipath mode, don't process attempts configured for single path.
        // In default single path mode, don't process attempts configured for multipath.
        //
        if ((mPrivate->EnableMpio &&
           AttemptConfigData->SessionConfigData.Enabled != ISCSI_ENABLED_FOR_MPIO) ||
           (!mPrivate->EnableMpio &&
           AttemptConfigData->SessionConfigData.Enabled != ISCSI_ENABLED)) {
            continue;
        }

        //
        // Don't process the attempt that fails to get the init/target information from DHCP.
        //
        if (AttemptConfigData->SessionConfigData.InitiatorInfoFromDhcp &&
            !AttemptConfigData->DhcpSuccess) {
            if (!mPrivate->EnableMpio && mPrivate->ValidSinglePathCount > 0) {
            mPrivate->ValidSinglePathCount--;
            }
            continue;
        }

        //
        // Don't process the autoconfigure path if it is already established.
        //
        if (AttemptConfigData->SessionConfigData.IpMode == IP_MODE_AUTOCONFIG &&
            AttemptConfigData->AutoConfigureSuccess) {
            continue;
        }

        //
        // Don't process the attempt if its IP mode is not in the current IP version.
        //
        if (!mPrivate->Ipv6Flag) {
            if (AttemptConfigData->SessionConfigData.IpMode == IP_MODE_IP6) {
                continue;
            }
            if (AttemptConfigData->SessionConfigData.IpMode == IP_MODE_AUTOCONFIG &&
                AttemptConfigData->AutoConfigureMode == IP_MODE_AUTOCONFIG_IP6) {
                continue;
            }
        } else {
            if (AttemptConfigData->SessionConfigData.IpMode == IP_MODE_IP4) {
                continue;
            }
            if (AttemptConfigData->SessionConfigData.IpMode == IP_MODE_AUTOCONFIG &&
                AttemptConfigData->AutoConfigureMode == IP_MODE_AUTOCONFIG_IP4) {
                continue;
            }
        }

        //
        // Fill in the Session and init it.
        //
        Session = (ISCSI_SESSION *) AllocateZeroPool (sizeof (ISCSI_SESSION));
        if (Session == NULL) {
            Status = EFI_OUT_OF_RESOURCES;
            goto ON_ERROR;
        }

        Session->Private    = Private;
        Session->ConfigData = AttemptConfigData;
        Session->AuthType   = AttemptConfigData->AuthenticationType;

        UnicodeSPrint (
             mPrivate->PortString,
             (UINTN) ISCSI_NAME_IFR_MAX_SIZE,
             L"Attempt %d",
             (UINTN) AttemptConfigData->AttemptConfigIndex
        );

        if (Session->AuthType == ISCSI_AUTH_TYPE_CHAP) {
            Session->AuthData.CHAP.AuthConfig = &AttemptConfigData->AuthConfigData.CHAP;
        }

        IScsiSessionInit (Session, FALSE);

        //
        // Try to login and create an iSCSI session according to the configuration.
        //
        Status = IScsiSessionLogin (Session);
        if (Status == EFI_MEDIA_CHANGED) {
            //
            // The specified target is not available, and the redirection information is
            // received. Login the session again with the updated target address.
            //
            Status = IScsiSessionLogin (Session);
        } else if (Status == EFI_NOT_READY) {
            Status = IScsiSessionReLogin (Session);
        }

        //
        // Restore the origial user setting which specifies the proxy/virtual iSCSI target to NV region.
        //
        NvData = &AttemptConfigData->SessionConfigData;
        if (NvData->RedirectFlag) {
            NvData->TargetPort = NvData->OriginalTargetPort;
            CopyMem (&NvData->TargetIp, &NvData->OriginalTargetIp, sizeof (EFI_IP_ADDRESS));
            NvData->RedirectFlag = FALSE;

            gRT->SetVariable (
             mPrivate->PortString,
             &gEfiIScsiInitiatorNameProtocolGuid,
             ISCSI_CONFIG_VAR_ATTR,
             sizeof (ISCSI_ATTEMPT_CONFIG_NVDATA),
             AttemptConfigData
             );
        }

        if (EFI_ERROR (Status)) {
            //
            // In Single path mode, only the successful attempt will be recorded in iBFT;
            // in multi-path mode, all the attempt entries in MPIO will be recorded in iBFT.
            //
            if (!mPrivate->EnableMpio && mPrivate->ValidSinglePathCount > 0) {
                mPrivate->ValidSinglePathCount--;
            }

            FreePool (Session);

        } else {
            AttemptConfigData->ValidPath = TRUE;

            //
            // Do not record the attempt in iBFT if it login with KRB5.
            // TODO: record KRB5 attempt information in the iSCSI device path.
            //
            if (Session->AuthType == ISCSI_AUTH_TYPE_KRB) {
                if (!mPrivate->EnableMpio && mPrivate->ValidSinglePathCount > 0) {
                    mPrivate->ValidSinglePathCount--;
                }

                AttemptConfigData->ValidiBFTPath = FALSE;
            } else {
                AttemptConfigData->ValidiBFTPath = TRUE;
            }

            //
            // IScsi session success. Update the attempt state to NVR.
            //
            if (AttemptConfigData->SessionConfigData.IpMode == IP_MODE_AUTOCONFIG) {
                AttemptConfigData->AutoConfigureSuccess = TRUE;
            }

#if(ENABLE_CHAP_SECRET_ENCRYPTION)
            Status = PrepareToEncrypt(AttemptConfigData,EncryptSecrets);
            if(!(Status == EFI_SUCCESS || Status == EFI_UNSUPPORTED))
                return Status;
#endif

            gRT->SetVariable (
             mPrivate->PortString,
             &gEfiIScsiInitiatorNameProtocolGuid,
             ISCSI_CONFIG_VAR_ATTR,
             sizeof (ISCSI_ATTEMPT_CONFIG_NVDATA),
             AttemptConfigData
             );


#if(ENABLE_CHAP_SECRET_ENCRYPTION)
            Status = PrepareToEncrypt(AttemptConfigData,DecryptSecrets);
#endif
            //
            // Select the first login session. Abort others.
            //
            if (Private->Session == NULL) {
                Private->Session = Session;
                BootSelected     = AttemptConfigData->AttemptConfigIndex;
                //
                // Don't validate other attempt in multipath mode if one is success.
                //
                if (mPrivate->EnableMpio) {
                    break;
                }
            } else {
                IScsiSessionAbort (Session);
                FreePool (Session);        
                // Fix to allow configuring multiple targets on a NIC without MPIO enabled
                if (PcdGetBool(PcdAllowMultipleTargetsOnNIC)){
                    AttemptConfigData->ValidiBFTPath = FALSE;
                }
            }
        }
    }

    //
    // All attempts configured for this driver instance are not valid.
    //
    if (Private->Session == NULL) {
        Status = gBS->UninstallProtocolInterface (
                    Private->ExtScsiPassThruHandle,
                    &gEfiExtScsiPassThruProtocolGuid,
                    &Private->IScsiExtScsiPassThru
                    );
        ASSERT_EFI_ERROR (Status);
        Private->ExtScsiPassThruHandle = NULL;

        //
        // Reinstall the original ExtScsiPassThru back.
        //
        if (mPrivate->OneSessionEstablished && ExistPrivate != NULL) {
            Status = gBS->InstallProtocolInterface (
                      &ExistPrivate->ExtScsiPassThruHandle,
                      &gEfiExtScsiPassThruProtocolGuid,
                      EFI_NATIVE_INTERFACE,
                      &ExistPrivate->IScsiExtScsiPassThru
                      );
            if (EFI_ERROR (Status)) {
                goto ON_ERROR;
            }

        goto ON_EXIT;
    }

    Status = EFI_NOT_FOUND;

    goto ON_ERROR;

// Fix for block device not detected when MPIO attempts' order 
//  is changed and first Attempt is made invalid.
    } else {  //  If current session is valid, then delete prev session
        //  Uninstall only if 1. Session is already established 2. The attempt is enabled for Mpio
        if( mPrivate->OneSessionEstablished && mPrivate->EnableMpio ) {
            Status = gBS->UninstallProtocolInterface (
                              ExistPrivate->ExtScsiPassThruHandle,
                              &gEfiExtScsiPassThruProtocolGuid,
                              &ExistPrivate->IScsiExtScsiPassThru
                              );
            if (EFI_ERROR (Status)) {
                goto ON_ERROR;
            }
        }
    }

    NeedUpdate = TRUE;
    //
    // More than one attempt successes.
    //
    if (Private->Session != NULL && mPrivate->OneSessionEstablished) {

      AttemptConfigOrder = IScsiGetVariableAndSize (
                           L"AttemptOrder",
                           &gIScsiConfigGuid,
                           &AttemptConfigOrderSize
                           );
      if (AttemptConfigOrder == NULL) {
          goto ON_ERROR;
      }
      for (Index = 0; Index < AttemptConfigOrderSize / sizeof (UINT8); Index++) {
          if (AttemptConfigOrder[Index] == mPrivate->BootSelectedIndex ||
              AttemptConfigOrder[Index] == BootSelected) {
              break;
          }
      }

      if (mPrivate->EnableMpio) {
          //
          // Use the attempt in earlier order. Abort the later one in MPIO.
          //
          if (AttemptConfigOrder[Index] == mPrivate->BootSelectedIndex) {
              IScsiSessionAbort (Private->Session);
              FreePool (Private->Session);
              Private->Session = NULL;
              gBS->UninstallProtocolInterface (
               Private->ExtScsiPassThruHandle,
               &gEfiExtScsiPassThruProtocolGuid,
               &Private->IScsiExtScsiPassThru
               );
              Private->ExtScsiPassThruHandle = NULL;

              //
              // Reinstall the original ExtScsiPassThru back.
              //
              Status = gBS->InstallProtocolInterface (
                        &ExistPrivate->ExtScsiPassThruHandle,
                        &gEfiExtScsiPassThruProtocolGuid,
                        EFI_NATIVE_INTERFACE,
                        &ExistPrivate->IScsiExtScsiPassThru
                        );
              if (EFI_ERROR (Status)) {
                  goto ON_ERROR;
              }

              goto ON_EXIT;
          } else {
              if (AttemptConfigOrder[Index] != BootSelected) {
                  goto ON_ERROR;
              }
              mPrivate->BootSelectedIndex = BootSelected;
              //
              // Clear the resource in ExistPrivate.
              //
              gBS->UninstallProtocolInterface (
               ExistPrivate->Controller,
               IScsiPrivateGuid,
               &ExistPrivate->IScsiIdentifier
               ); 
              if (ExistPrivate->Session != NULL) {
                  IScsiSessionAbort (ExistPrivate->Session);
              }

              if (ExistPrivate->DevicePath != NULL) {
                  Status = gBS->UninstallProtocolInterface (
                          ExistPrivate->ExtScsiPassThruHandle,
                          &gEfiDevicePathProtocolGuid,
                          ExistPrivate->DevicePath
                          );
                  if (EFI_ERROR (Status)) {
                      goto ON_ERROR;
                  }

                  FreePool (ExistPrivate->DevicePath);
              }

              gBS->CloseEvent (ExistPrivate->ExitBootServiceEvent);
              FreePool (ExistPrivate);
          }
      } else {
          //
          // Use the attempt in earlier order as boot selected in single path mode.
          //
          if (AttemptConfigOrder[Index] == mPrivate->BootSelectedIndex) {
              NeedUpdate = FALSE;
          }
      }
    }

    if (NeedUpdate) {
        mPrivate->OneSessionEstablished = TRUE;
        mPrivate->BootSelectedIndex     = BootSelected;
    }

    //
    // Duplicate the Session's tcp connection device path. The source port field
    // will be set to zero as one iSCSI session is comprised of several iSCSI
    // connections.
    //
    Private->DevicePath = IScsiGetTcpConnDevicePath (Private->Session);
    if (Private->DevicePath == NULL) {
        Status = EFI_DEVICE_ERROR;
        goto ON_ERROR;
    }
    //
    // Install the updated device path onto the ExtScsiPassThruHandle.
    //
    Status = gBS->InstallProtocolInterface (
                  &Private->ExtScsiPassThruHandle,
                  &gEfiDevicePathProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  Private->DevicePath
                  );
    if (EFI_ERROR (Status)) {
        goto ON_ERROR;
    }

    //
    // ISCSI children should share the default Tcp child, just open the default Tcp child via BY_CHILD_CONTROLLER.
    //
    Status = gBS->OpenProtocol (
                  Private->ChildHandle, /// Default Tcp child
                  ProtocolGuid,
                  &Interface,
                  Image,
                  Private->ExtScsiPassThruHandle,
                  EFI_OPEN_PROTOCOL_BY_CHILD_CONTROLLER
                  );
    if (EFI_ERROR (Status)) {
        gBS->UninstallMultipleProtocolInterfaces (
           Private->ExtScsiPassThruHandle,
           &gEfiExtScsiPassThruProtocolGuid,
           &Private->IScsiExtScsiPassThru,
           &gEfiDevicePathProtocolGuid,
           Private->DevicePath,
           NULL
           );

        goto ON_ERROR;
    }

    ON_EXIT:

    //
    // Update/Publish the iSCSI Boot Firmware Table.
    //
#if (RSD_ISCSI_SUPPORT)
    // If BMC data is available and session login is successful,then publish ibft table
    if (NeedUpdate && RsdIScsiIsBmcDataAvailable())
        IScsiPublishIbft ();
    else if (mPrivate->BootSelectedIndex != 0)
        IScsiPublishIbft ();
#elif (REDFISH_ISCSI_SUPPORT)
    if (NeedUpdate && gRedfishIScsiDataAvailable)
        IScsiPublishIbft ();
    else if(mPrivate->BootSelectedIndex != 0)
        IScsiPublishIbft ();
#else 
    if (mPrivate->BootSelectedIndex != 0) {
        IScsiPublishIbft ();
    }
#endif

    return EFI_SUCCESS;

ON_ERROR:

    if (Private->Session != NULL) {
        IScsiSessionAbort (Private->Session);
    }

    return Status;
}

/**
  Parse IsId in string format and convert it to binary.

  @param[in]        String  The buffer of the string to be parsed.
  @param[in, out]   IsId    The buffer to store IsId.

  @retval EFI_SUCCESS              The operation finished successfully.
  @retval EFI_INVALID_PARAMETER    Any input parameter is invalid.

**/
EFI_STATUS
AmiIScsiParseIsIdFromString (
    IN CONST CHAR16                    *String,
    IN OUT   UINT8                     *IsId
)
{
    UINT8                          Index;
    CHAR16                         *IsIdStr;
    CHAR16                         TempStr[3];     
    UINT8                          IsIdConfigured[3];  
    CHAR16                         *CharPtr = NULL;
    CHAR16                         *PopUpTitle = NULL;
    
    if ((String == NULL) || (IsId == NULL)) {
        return EFI_INVALID_PARAMETER;
    }

    IsIdStr = (CHAR16 *) String;

    if (StrLen (IsIdStr) == 12) {
        IsIdStr += 6;
    }  
    for (Index = 0; Index < 3; Index++) {
        CopyMem (TempStr, IsIdStr, sizeof (TempStr));
        TempStr[2] = L'\0';
          
        //
        // Validate the each unicode character entered in setup.
        // Print error message if the character is a in valid hex number.
        //
        if ( ( ValidateUnicodeChar (TempStr[0]) && \
                 ValidateUnicodeChar ( TempStr[1]) ) != TRUE ) {
            CharPtr=NetLibHiiGetString(mCallbackInfo->RegisteredHandle,STRING_TOKEN(STR_INVALID_ISID),NULL);
            PopUpTitle=NetLibHiiGetString(mCallbackInfo->RegisteredHandle,STRING_TOKEN(STR_MSGBOX_TITLE),NULL);
            AMICreatePopUpEx (PopUpTitle,CharPtr);
            if(CharPtr != NULL) {
                FreePool (CharPtr);
                CharPtr= NULL;
            }
            return EFI_INVALID_PARAMETER;
        }

        //
        // Convert the string to IsId. StrHexToUintn stops at the first character
        // that is not a valid hex character, '\0' here.
        //
        IsIdConfigured[Index] = (UINT8)StrHexToUintn (TempStr);

        IsIdStr = IsIdStr + 2;
    }
    //
    // Control comes here only when input entered in setup is valid
    // So update the newly configured ISID
    //
    CopyMem (&IsId[3], &IsIdConfigured[0], sizeof (IsIdConfigured));
 
    return EFI_SUCCESS;
}

/**
  Update the component name for the iSCSI instance.

  @param[in]  IScsiExtScsiPassThru  A pointer to the EFI_EXT_SCSI_PASS_THRU_PROTOCOL instance.
  @param[in]  Ipv6Flag              TRUE if IP6 network stack is used.

  @retval EFI_SUCCESS               Update the ControllerNameTable of this instance successfully.
  @retval EFI_INVALID_PARAMETER     The input parameter is invalid.
  @retval EFI_UNSUPPORTED           Can't get the corresponding NIC info from the Controller handle.

**/
EFI_STATUS
AmiUpdateName (
    IN   EFI_EXT_SCSI_PASS_THRU_PROTOCOL *IScsiExtScsiPassThru,
    IN   BOOLEAN     Ipv6Flag
)
{
    EFI_STATUS                       Status;
    CHAR16                           HandleName[80];
    ISCSI_DRIVER_DATA                *Private;
    UINT8                            NicIndex;
    CHAR16             *CharPtr;
 
    if (IScsiExtScsiPassThru == NULL) {
        return EFI_INVALID_PARAMETER;
    }

    Private  = ISCSI_DRIVER_DATA_FROM_EXT_SCSI_PASS_THRU (IScsiExtScsiPassThru);
    NicIndex = Private->Session->ConfigData->NicIndex;

    CharPtr=NetLibHiiGetString(mCallbackInfo->RegisteredHandle,STRING_TOKEN(STR_ISCSI_NIC_INDEX),NULL);
    UnicodeSPrint (
        HandleName,
        sizeof (HandleName),
        CharPtr,
        Ipv6Flag ? L"IPv6" : L"IPv4",
        NicIndex
    );
    if(CharPtr != NULL) {
        FreePool (CharPtr);
        CharPtr=NULL;
    }

    if (gIScsiControllerNameTable != NULL) {
        FreeUnicodeStringTable (gIScsiControllerNameTable);
        gIScsiControllerNameTable = NULL;
    }

    Status = AddUnicodeString2 (
             "eng",
             gIScsiComponentName.SupportedLanguages,
             &gIScsiControllerNameTable,
             HandleName,
             TRUE
             );
    if (EFI_ERROR (Status)) {
        return Status;
    }

    return AddUnicodeString2 (
           "en",
           gIScsiComponentName2.SupportedLanguages,
           &gIScsiControllerNameTable,
           HandleName,
           FALSE
           );
}

/**
  This function determines whether the decrypted secret meets the required length.

  @param[in]    MacStringStr    MacString available in attempt data.

  @retval[out]  TRUE            If the MAC address matched.
  @retval[out]  FALSE           If the MAC address not matched.

**/

BOOLEAN CheckMacAvailability(CHAR16 MacStringStr[])
{
    EFI_STATUS                      Status = EFI_SUCCESS;
    EFI_HANDLE                      *Handles = NULL;  
    UINTN                           HandleCount = 0, Index = 0;
    EFI_SIMPLE_NETWORK_PROTOCOL     *Snp = NULL;
    CHAR16                          MacString[ISCSI_MAX_MAC_STRING_LEN];
	BOOLEAN							MacAvailability = FALSE;
	
    Status = gBS->LocateHandleBuffer (ByProtocol, &gEfiSimpleNetworkProtocolGuid, NULL, &HandleCount, &Handles);
    if(EFI_ERROR(Status))
    {
        goto EXIT;
    }
    for (Index = 0; Index < HandleCount; Index ++)
    {   
        Status = gBS->HandleProtocol (Handles [Index], &gEfiSimpleNetworkProtocolGuid, &Snp);
        if (EFI_ERROR (Status))
        {
            continue;
        }
        IScsiMacAddrToStr (&Snp->Mode->PermanentAddress, Snp->Mode->HwAddressSize, 0, MacString);
        if(!StrCmp(MacString, MacStringStr))
        {
            MacAvailability = TRUE;
			break;
        }
    }
    EXIT:
    if(Handles != NULL)
    {
        FreePool(Handles);
    }
    return MacAvailability;
}
