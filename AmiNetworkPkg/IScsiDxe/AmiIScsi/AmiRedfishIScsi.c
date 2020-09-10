//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2020, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************
/** @file AmiRedfishIScsi.c
  Provides the AMI Override functions for the RedFish-IScsi feature

**/
#include <AmiRedfishIScsi.h>

extern CHAR8  IScsiHexString[];

/**
  Extract nvram data and respective initiator name specific to the current MAC and attempt.
    
  @param[in]  Controller        Controller Handle
  @param[in]  AttemptConfigData ISCSI_ATTEMPT_CONFIG_NVDATA attempt data.

**/
EFI_STATUS
RedfishMultipleInitiatorName (
    EFI_HANDLE                  Controller,
    ISCSI_ATTEMPT_CONFIG_NVDATA *AttemptConfigData
)
{
    EFI_STATUS                  Status;
    EFI_MAC_ADDRESS             MacAddress;
    UINTN                       HwAddressSize; 
    CHAR16                      TempMacStr16[ISCSI_MAX_MAC_STRING_LEN];
    CHAR16                      MacPortString[ISCSI_NAME_IFR_MAX_SIZE];
    REDFISH_ISCSI_ATTEMPT_CONFIG_DATA   RedfishIScsiAttemptData;
    UINTN                       Datasize;
    
    Status = NetLibGetMacAddress (Controller, &MacAddress, &HwAddressSize);
    if(EFI_ERROR (Status)) {
        return Status;
    }
        
    IScsiMacAddrToStr(&MacAddress,(UINT32)HwAddressSize,0,TempMacStr16);
    if('\0' == TempMacStr16[0])
        DEBUG((DEBUG_INFO,"\n MacString is NULL"));
    else           
        DEBUG((DEBUG_INFO,"\n MacString :: %s", TempMacStr16));   
    UnicodeSPrint (
            MacPortString,
            (UINTN) 128,
            L"%s%d",
            TempMacStr16,
            (UINTN) AttemptConfigData->AttemptConfigIndex
    );
    
    DEBUG((DEBUG_INFO,"\nMacPortString :: %s", MacPortString));
    Datasize = sizeof(REDFISH_ISCSI_ATTEMPT_CONFIG_DATA);
    Status =  gRT->GetVariable(
            MacPortString,
            &gEfiRedfishIscsiDataGuid,
            NULL,             
            &Datasize,
            &RedfishIScsiAttemptData
    );
    
    DEBUG((DEBUG_INFO,"\nIn %a, GetVariable Status :: %r , Datasize = %d", __FUNCTION__, Status, Datasize));        
    if(!EFI_ERROR(Status)) {
        DEBUG((DEBUG_INFO,"\nIn initiator name =  %a", RedfishIScsiAttemptData.IScsiInitiatorName));
        AsciiStrCpy(gRedfishIScsiInitiatorName, RedfishIScsiAttemptData.IScsiInitiatorName);
    }
    AsciiStrCpy(mPrivate->InitiatorName, gRedfishIScsiInitiatorName);
 
    return EFI_SUCCESS;
}   
/**
  Convert Hex decimal MAC address to Ascii String
    
  @param[in]  MacAddr     The mac address  Ex: {0x00 , 0x50, 0x56, 0x0C0, 0X00, 0X02}
  @param[in]  Len         Length in bytes of the mac address Ex: 6 bytes.
  @param[in]  MacString8  The Mac String. Ex: "00:50:56:C0:00:02"

**/
VOID
EFIAPI
RedfishIscsiConvertMacAddrToAsciiStr (
    IN CHAR8 *MacAddr,
    IN UINT8 Len, 
    OUT CHAR8 *MacString8
)
{
    UINT8 Index, Pos;

    if(NULL == MacAddr || 0 == Len) {
        MacString8[0] = '\0';
        return;
    }

    for(Pos = 0, Index = 0 ; Pos < Len ; Pos++) {
        MacString8[Index++] = IScsiHexString[ (MacAddr[Pos] >> 4) & 0x0F ] ;
        MacString8[Index++] = IScsiHexString[ (MacAddr[Pos]) & 0x0F ];   
        MacString8[Index++] = ':';
    }
    MacString8[--Index] = '\0';     // Remove the ':' at the end and place NULL character
}

/**
  Constructs one copy of ISCSI_ATTEMPT_CONFIG_NVDATA from Redfish attempt details.

  @param[in]    RedfishAttemptData                      The iSCSI attempt parameters from Redfish structure.
  @param[OUT]    IScsiAttemptData        Parsed ISCSI_ATTEMPT_CONFIG_NVDATA attempt data.

  @retval EFI_SUCCESS           ISCSI_ATTEMPT_CONFIG_NVDATA has been framed from the input arguments.

**/
EFI_STATUS
EFIAPI
RedfishIScsiFrameAttemptConfigData (
    IN REDFISH_ISCSI_ATTEMPT_CONFIG_DATA   *RedfishAttemptData,
    OUT ISCSI_ATTEMPT_CONFIG_NVDATA        *IScsiAttemptData
)
{
    EFI_STATUS  Status = EFI_SUCCESS;
    CHAR16 		*MacString1 ;
    CHAR16 		MacString2[20] = {0};
    UINTN 		i=0, j=0, IsIdNum= 0;
    CHAR16 		TempStr[3];
	UINTN 		BufferSize;
    DEBUG((DEBUG_INFO,"\nIn %a ENTRY", __FUNCTION__));
    //
    // INITIATOR DATA
    //

    // Copy Initiator Name.
    ZeroMem(gRedfishIScsiInitiatorName, ISCSI_NAME_MAX_SIZE);
    CopyMem(gRedfishIScsiInitiatorName, &RedfishAttemptData->IScsiInitiatorName, AsciiStrLen(RedfishAttemptData->IScsiInitiatorName));
    BufferSize  = AsciiStrLen(gRedfishIScsiInitiatorName);
     Status = gIScsiInitiatorName.Set (&gIScsiInitiatorName, &BufferSize, gRedfishIScsiInitiatorName);
     
    Status = IScsiNormalizeName (gRedfishIScsiInitiatorName, AsciiStrLen (gRedfishIScsiInitiatorName));
    if(EFI_ERROR(Status)) {
        DEBUG((EFI_D_ERROR,"\nBMC's Initiator name is invalid.\n"));
        return Status;
    }

    DEBUG((DEBUG_INFO,"\nIScsiNormalizeName  Status :: %r", Status));
    
    // Copy IpMode
    IScsiAttemptData->SessionConfigData.IpMode = RedfishAttemptData->IpMode;

    // Copy InitiatorInfoFromDhcp
    IScsiAttemptData->SessionConfigData.InitiatorInfoFromDhcp = RedfishAttemptData->InitiatorInfoFromDhcp;

    if ( (IScsiAttemptData->SessionConfigData.IpMode == IP_MODE_AUTOCONFIG )) {
        IScsiAttemptData->SessionConfigData.TargetInfoFromDhcp = 1;
        IScsiAttemptData->SessionConfigData.InitiatorInfoFromDhcp = 1;
    }


    // Copy LocalIp
    CopyMem(&IScsiAttemptData->SessionConfigData.LocalIp, &RedfishAttemptData->LocalIp, sizeof(EFI_IP_ADDRESS));

    // Copy SubnetMask
    CopyMem(&IScsiAttemptData->SessionConfigData.SubnetMask, &RedfishAttemptData->SubnetMask, sizeof(EFI_IPv4_ADDRESS));

    // Copy Gateway
    CopyMem(&IScsiAttemptData->SessionConfigData.Gateway, &RedfishAttemptData->Gateway, sizeof(EFI_IP_ADDRESS));

    // Copy PrimaryDns
    CopyMem(&IScsiAttemptData->PrimaryDns, &RedfishAttemptData->PrimaryDns, sizeof(EFI_IP_ADDRESS));

    // Copy SecondaryDns
    CopyMem(&IScsiAttemptData->SecondaryDns, &RedfishAttemptData->SecondaryDns, sizeof(EFI_IP_ADDRESS));

    // Copy MacString and ISID
    AsciiStrCpy(IScsiAttemptData->MacString, RedfishAttemptData->MacString);
    MacString1 = AllocateZeroPool(sizeof(CHAR16) * (AsciiStrLen(IScsiAttemptData->MacString) + 1));
    AsciiStrToUnicodeStr(IScsiAttemptData->MacString, MacString1);
    // Removing ':'
    for(i =0; i < StrLen(MacString1); i++)
    {
        if(MacString1[i] != ':')
        {
            MacString2[j]= MacString1[i];
            j++;
        }
    }
    MacString2[j]= L'\0';
    for (i = 0,j=0; i < 6; i++) {
        CopyMem (TempStr, MacString2+j, sizeof (TempStr));
        TempStr[2] = L'\0';
        IsIdNum = StrHexToUintn (TempStr);
        IScsiAttemptData->SessionConfigData.IsId[i] = (UINT8) IsIdNum;
        j += 2;
      }
    IScsiAttemptData->SessionConfigData.IsId[0] = (UINT8) (IScsiAttemptData->SessionConfigData.IsId[0] & 0x3F);

    //
    // TARGET DATA
    //

    // Copy TargetName
    CopyMem(IScsiAttemptData->SessionConfigData.TargetName, &RedfishAttemptData->TargetName, AsciiStrLen(RedfishAttemptData->TargetName));
    {
      CHAR16       TargetName16[ISCSI_MAX_MAC_STRING_LEN]; 
      AsciiStrToUnicodeStrS (IScsiAttemptData->SessionConfigData.TargetName, TargetName16, sizeof (TargetName16) / sizeof (TargetName16[0]));
      DEBUG((DEBUG_INFO,"\nTargetName :: %s", TargetName16));
    }
    // Copy TargetInfoFromDhcp
    IScsiAttemptData->SessionConfigData.TargetInfoFromDhcp = (BOOLEAN)RedfishAttemptData->TargetInfoFromDhcp;

    // Copy TargetIp
    CopyMem(&IScsiAttemptData->SessionConfigData.TargetIp, &RedfishAttemptData->TargetIp, sizeof(RedfishAttemptData->TargetIp));

    // Copy TargetPort
    CopyMem(&IScsiAttemptData->SessionConfigData.TargetPort, &RedfishAttemptData->TargetPort, sizeof(RedfishAttemptData->TargetPort));

    // Copy BootLun
    CopyMem(IScsiAttemptData->SessionConfigData.BootLun, &RedfishAttemptData->BootLun, sizeof(RedfishAttemptData->BootLun));

    // Copy CHAP Details.
    IScsiAttemptData->AuthenticationType = RedfishAttemptData->AuthenticationType;
    IScsiAttemptData->AuthConfigData.CHAP.CHAPType = RedfishAttemptData->AuthConfigData.CHAP.CHAPType;

    DEBUG((DEBUG_INFO,"\n AuthType = %d, ChapType = %d", IScsiAttemptData->AuthenticationType,     IScsiAttemptData->AuthConfigData.CHAP.CHAPType));
    if (ISCSI_AUTH_TYPE_CHAP == IScsiAttemptData->AuthenticationType) {
        if(IScsiAttemptData->AuthConfigData.CHAP.CHAPType == ISCSI_CHAP_UNI) {
            DEBUG((DEBUG_INFO,"\n ChapName = %s,  %a, Strlen = %d", RedfishAttemptData->AuthConfigData.CHAP.CHAPName, RedfishAttemptData->AuthConfigData.CHAP.CHAPName, AsciiStrLen(RedfishAttemptData->AuthConfigData.CHAP.CHAPName)));
            DEBUG((DEBUG_INFO,"\n ChapSecret = %s,  %a, Strlen = %d", RedfishAttemptData->AuthConfigData.CHAP.CHAPSecret, RedfishAttemptData->AuthConfigData.CHAP.CHAPSecret, AsciiStrLen(RedfishAttemptData->AuthConfigData.CHAP.CHAPSecret)));
            CopyMem(IScsiAttemptData->AuthConfigData.CHAP.CHAPName, &RedfishAttemptData->AuthConfigData.CHAP.CHAPName, AsciiStrLen(RedfishAttemptData->AuthConfigData.CHAP.CHAPName));
            CopyMem(IScsiAttemptData->AuthConfigData.CHAP.CHAPSecret, &RedfishAttemptData->AuthConfigData.CHAP.CHAPSecret, AsciiStrLen(RedfishAttemptData->AuthConfigData.CHAP.CHAPSecret));
        }
        else if(IScsiAttemptData->AuthConfigData.CHAP.CHAPType == ISCSI_CHAP_MUTUAL) {
            CopyMem(IScsiAttemptData->AuthConfigData.CHAP.CHAPName, &RedfishAttemptData->AuthConfigData.CHAP.CHAPName, AsciiStrLen(RedfishAttemptData->AuthConfigData.CHAP.CHAPName));
            CopyMem(IScsiAttemptData->AuthConfigData.CHAP.CHAPSecret, &RedfishAttemptData->AuthConfigData.CHAP.CHAPSecret, AsciiStrLen(RedfishAttemptData->AuthConfigData.CHAP.CHAPSecret));
            CopyMem(IScsiAttemptData->AuthConfigData.CHAP.ReverseCHAPName, &RedfishAttemptData->AuthConfigData.CHAP.ReverseCHAPName, AsciiStrLen(RedfishAttemptData->AuthConfigData.CHAP.ReverseCHAPName));
            CopyMem(IScsiAttemptData->AuthConfigData.CHAP.ReverseCHAPSecret, &RedfishAttemptData->AuthConfigData.CHAP.ReverseCHAPSecret, AsciiStrLen(RedfishAttemptData->AuthConfigData.CHAP.ReverseCHAPSecret));
        }
    }

    //
    // ATTEMPT DATA
    //

    // Copy ConnectTimeout
    IScsiAttemptData->SessionConfigData.ConnectTimeout = 0x03E8;

    // Copy ConnectRetryCount
    IScsiAttemptData->SessionConfigData.ConnectRetryCount = 0x00;

    // Other default values.
    IScsiAttemptData->AutoConfigureSuccess         = FALSE;
    IScsiAttemptData->SessionConfigData.Enabled    = TRUE;     // Always treat as enabled
    IScsiAttemptData->AttemptType                  = SETUP_ATTEMPT;
    IScsiAttemptData->Actived                      = ISCSI_ACTIVE_ENABLED;

    return Status;
}

/**
  Constructs one copy of REDFISH_ISCSI_ATTEMPT_CONFIG_DATA from ISCSI_ATTEMPT_CONFIG_NVDATA details.

  @param[in]    ISCSI_ATTEMPT_CONFIG_NVDATA              The ISCSI attempt parameters from Redfish structure.
  @param[OUT]   REDFISH_ISCSI_ATTEMPT_CONFIG_DATA        Parsed ISCSI_ATTEMPT_CONFIG_NVDATA attempt data.

  @retval EFI_SUCCESS           ISCSI_ATTEMPT_CONFIG_NVDATA has been framed from the input arguments.

**/
EFI_STATUS
EFIAPI
IScsitoRedfishFrameAttemptConfigData (
    IN  ISCSI_ATTEMPT_CONFIG_NVDATA   *IScsiAttemptData,
    OUT REDFISH_ISCSI_ATTEMPT_CONFIG_DATA        *RedfishAttemptData
)
{
    EFI_STATUS  Status = EFI_SUCCESS;
    UINTN i =0;
    UINTN BufferSize = ISCSI_NAME_MAX_SIZE;
    DEBUG((DEBUG_INFO,"\nIn %a ENTRY", __FUNCTION__));

    //
    // INITIATOR DATA
    //

    // Copy Initiator Name.
    Status = gIScsiInitiatorName.Get (&gIScsiInitiatorName, &BufferSize, gRedfishIScsiInitiatorName);
    if(EFI_ERROR(Status))
        return Status;
    CopyMem(&RedfishAttemptData->IScsiInitiatorName, gRedfishIScsiInitiatorName, AsciiStrLen(gRedfishIScsiInitiatorName));
    
    // Copy IpMode
    RedfishAttemptData->IpMode = IScsiAttemptData->SessionConfigData.IpMode;

    // Copy InitiatorInfoFromDhcp
    RedfishAttemptData->InitiatorInfoFromDhcp = IScsiAttemptData->SessionConfigData.InitiatorInfoFromDhcp;

    // Copy LocalIp
    CopyMem(&RedfishAttemptData->LocalIp,&IScsiAttemptData->SessionConfigData.LocalIp, sizeof(EFI_IP_ADDRESS));

    // Copy SubnetMask
    CopyMem( &RedfishAttemptData->SubnetMask, &IScsiAttemptData->SessionConfigData.SubnetMask,  sizeof(EFI_IPv4_ADDRESS));

    // Copy Gateway
    CopyMem(&RedfishAttemptData->Gateway, &IScsiAttemptData->SessionConfigData.Gateway, sizeof(EFI_IP_ADDRESS));

    // Copy PrimaryDns
    CopyMem( &RedfishAttemptData->PrimaryDns, &IScsiAttemptData->PrimaryDns, sizeof(EFI_IP_ADDRESS));

    // Copy SecondaryDns
    CopyMem( &RedfishAttemptData->SecondaryDns, &IScsiAttemptData->SecondaryDns, sizeof(EFI_IP_ADDRESS));

    // Copy MacString and ISID
    AsciiStrCpy( RedfishAttemptData->MacString, IScsiAttemptData->MacString);
    //
    // TARGET DATA
    //

    // Copy TargetName
    CopyMem(&RedfishAttemptData->TargetName,IScsiAttemptData->SessionConfigData.TargetName, AsciiStrLen(IScsiAttemptData->SessionConfigData.TargetName));

    // Copy TargetInfoFromDhcp
    RedfishAttemptData->TargetInfoFromDhcp = IScsiAttemptData->SessionConfigData.TargetInfoFromDhcp;

    // Copy TargetIp
    CopyMem( &RedfishAttemptData->TargetIp, &IScsiAttemptData->SessionConfigData.TargetIp, sizeof(RedfishAttemptData->TargetIp));

    // Copy TargetPort
    CopyMem( &RedfishAttemptData->TargetPort, &IScsiAttemptData->SessionConfigData.TargetPort, sizeof(RedfishAttemptData->TargetPort));

    // Copy BootLun
    CopyMem( &RedfishAttemptData->BootLun, IScsiAttemptData->SessionConfigData.BootLun, sizeof(RedfishAttemptData->BootLun));

    // Copy CHAP Details.
    RedfishAttemptData->AuthenticationType = IScsiAttemptData->AuthenticationType;
    RedfishAttemptData->AuthConfigData.CHAP.CHAPType = IScsiAttemptData->AuthConfigData.CHAP.CHAPType;
    DEBUG((DEBUG_INFO,"\n AuthType = %d, ChapType = %d", RedfishAttemptData->AuthenticationType,     RedfishAttemptData->AuthConfigData.CHAP.CHAPType));

    if (ISCSI_AUTH_TYPE_CHAP == RedfishAttemptData->AuthenticationType ) {
        if(RedfishAttemptData->AuthConfigData.CHAP.CHAPType  == ISCSI_CHAP_UNI) {
            DEBUG((DEBUG_INFO,"\n ChapName = %s,  %a, Strlen = %d", IScsiAttemptData->AuthConfigData.CHAP.CHAPName, IScsiAttemptData->AuthConfigData.CHAP.CHAPName, AsciiStrLen(IScsiAttemptData->AuthConfigData.CHAP.CHAPName)));
            DEBUG((DEBUG_INFO,"\n ChapSecret = %s,  %a, Strlen = %d", IScsiAttemptData->AuthConfigData.CHAP.CHAPSecret, IScsiAttemptData->AuthConfigData.CHAP.CHAPSecret, AsciiStrLen(IScsiAttemptData->AuthConfigData.CHAP.CHAPSecret)));

            CopyMem( &RedfishAttemptData->AuthConfigData.CHAP.CHAPName, IScsiAttemptData->AuthConfigData.CHAP.CHAPName, AsciiStrLen(IScsiAttemptData->AuthConfigData.CHAP.CHAPName));
            CopyMem( &RedfishAttemptData->AuthConfigData.CHAP.CHAPSecret, IScsiAttemptData->AuthConfigData.CHAP.CHAPSecret, AsciiStrLen(IScsiAttemptData->AuthConfigData.CHAP.CHAPSecret));
        }
        else if(IScsiAttemptData->AuthConfigData.CHAP.CHAPType == ISCSI_CHAP_MUTUAL) {
            CopyMem( &RedfishAttemptData->AuthConfigData.CHAP.CHAPName, IScsiAttemptData->AuthConfigData.CHAP.CHAPName, AsciiStrLen(IScsiAttemptData->AuthConfigData.CHAP.CHAPName));
            CopyMem( &RedfishAttemptData->AuthConfigData.CHAP.CHAPSecret, IScsiAttemptData->AuthConfigData.CHAP.CHAPSecret, AsciiStrLen(IScsiAttemptData->AuthConfigData.CHAP.CHAPSecret));
            CopyMem( &RedfishAttemptData->AuthConfigData.CHAP.ReverseCHAPName, IScsiAttemptData->AuthConfigData.CHAP.ReverseCHAPName, AsciiStrLen(IScsiAttemptData->AuthConfigData.CHAP.ReverseCHAPName));
            CopyMem( &RedfishAttemptData->AuthConfigData.CHAP.ReverseCHAPSecret, IScsiAttemptData->AuthConfigData.CHAP.ReverseCHAPSecret, AsciiStrLen(IScsiAttemptData->AuthConfigData.CHAP.ReverseCHAPSecret));
        }
    }

    return Status;
}

/**

  Set the IScsi configuration retrieved from redfish server to NVRAM

  @param[in]     This           The pointer to the EFI_IP4_CONFIG2_PROTOCOL instance.
  @param[in]     DataSize       Size of the buffer pointed to by Data in bytes.
  @param[in]     Data           The data buffer to set. The type of the data buffer is
                                associated with the DataType.

  @retval EFI_SUCCESS           The specified configuration data for the EFI IPv6
                                network stack was set successfully.
  @retval EFI_INVALID_PARAMETER One or more of the following are TRUE:
                                - This is NULL.
                                - Data is NULL.
                                - One or more fields in Data do not match the requirement of the
                                  data type indicated by DataType.
  @retval EFI_WRITE_PROTECTED   The specified configuration data is read-only or the specified
                                configuration data cannot be set under the current policy.
  @retval EFI_ACCESS_DENIED     Another set operation on the specified configuration
                                data is already in process.
  @retval EFI_NOT_READY         An asynchronous process was invoked to set the specified
                                configuration data, and the process is not finished yet.
  @retval EFI_BAD_BUFFER_SIZE   The DataSize does not match the size of the type
                                indicated by DataType.
  @retval EFI_UNSUPPORTED       This DataType is not supported.
  @retval EFI_OUT_OF_RESOURCES  Required system resources could not be allocated.
  @retval EFI_DEVICE_ERROR      An unexpected system error or network error occurred.

**/
EFI_STATUS
EFIAPI
EfiRedfishConfigSetData (
    IN EFI_REDFISH_ISCSI_CONFIG_PROTOCOL   *This,
    IN REDFISH_ISCSI_ATTEMPT_CONFIG_DATA   *RedfishAttemptData,
    IN UINTN                               DataSize
)
{
    EFI_STATUS                  Status;
    CHAR16                      MacString16[ISCSI_MAX_MAC_STRING_LEN];
    CHAR8                       MacString8[ISCSI_MAX_MAC_STRING_LEN];
    CHAR16                      MacPortString[ISCSI_NAME_IFR_MAX_SIZE];
    UINT8                       *AttemptConfigOrder;
    UINTN                       AttemptConfigOrderSize;
    ISCSI_ATTEMPT_CONFIG_NVDATA *AttemptConfigData = NULL;
    UINT8                       *AttemptOrderTmp;
    UINTN                       TotalNumber;
    UINTN                       Index;   
    ISCSI_ATTEMPT_CONFIG_NVDATA IScsiAttemptData= {0} ;
    
    DEBUG((DEBUG_INFO,"\nIn %a ENTRY", __FUNCTION__));
                
    Status = RedfishIScsiFrameAttemptConfigData(RedfishAttemptData, &IScsiAttemptData); 
    if(EFI_ERROR(Status))
        return Status;  
    /*
     *  Only 1 attempt per Nic is supported.
     *  If attempt comes with same Nic, the previous data is overridden.
     */ 
    Status = IScsiRedfishDeleteAttempts(&IScsiAttemptData);
    if (EFI_ERROR (Status)) {
        return Status;
    }
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
        
        if (AttemptConfigData == NULL || ((AttemptConfigData->AttemptType == REDFISH_ATTEMPT)&& (AttemptConfigData->SessionConfigData.Enabled)) ||
            ((AttemptConfigData->AttemptType == SETUP_ATTEMPT)&& (AttemptConfigData->Actived == ISCSI_ACTIVE_ENABLED)) ){
            continue;
        }
        break;
    }
    IScsiAttemptData.AttemptConfigIndex = AttemptConfigData->AttemptConfigIndex;

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

    AttemptOrderTmp[TotalNumber - 1] = IScsiAttemptData.AttemptConfigIndex;
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
        DEBUG((EFI_D_ERROR,"\nIn %a, AttemptConfigOrder SetVariable failed. Status :: %r", __FUNCTION__, Status));
        return Status;
    }
    DEBUG((DEBUG_INFO,"\nIn %a, AttemptConfigOrder SetVariable Status :: %r", __FUNCTION__, Status));
    
    UnicodeSPrint (
             mPrivate->PortString,
             (UINTN) ISCSI_NAME_IFR_MAX_SIZE,
             L"Attempt %d",
             (UINTN) IScsiAttemptData.AttemptConfigIndex
             );
    UnicodeStrToAsciiStrS (mPrivate->PortString, IScsiAttemptData.AttemptName, ATTEMPT_NAME_SIZE);
    Status = gRT->SetVariable (
                           mPrivate->PortString,
                           &gEfiIScsiInitiatorNameProtocolGuid,
                           ISCSI_CONFIG_VAR_ATTR,
                           sizeof (ISCSI_ATTEMPT_CONFIG_NVDATA),
                           &IScsiAttemptData
                           );
     
    if (EFI_ERROR (Status)) {
        DEBUG((EFI_D_ERROR,"\nSetVariable  IScsiAttemptConfigData Status :: %r", Status));
        return Status;
    }
    DEBUG((DEBUG_INFO,"\nSetVariable  IScsiAttemptConfigData Status :: %r", Status));
     
    FreePool (AttemptConfigData);      
    // Convert Hex decimal MAC address to Ascii String 
    RedfishIscsiConvertMacAddrToAsciiStr(IScsiAttemptData.MacString, REDFISH_NIC_SIZE, MacString8);
    AsciiStrToUnicodeStrS (MacString8, MacString16, sizeof (MacString16) / sizeof (MacString16[0]));
   
    DEBUG((DEBUG_INFO,"\nMacString16 :: %s", MacString16));
    
    UnicodeSPrint (
            MacPortString,
            (UINTN) 128,
            L"%s%d",
            MacString16,
            (UINTN) IScsiAttemptData.AttemptConfigIndex
            );
    DEBUG((DEBUG_INFO,"\nMacPortString :: %s", MacPortString));
  
    Status =  gRT->SetVariable (
                MacPortString,
                &gEfiRedfishIscsiDataGuid,
                EFI_VARIABLE_BOOTSERVICE_ACCESS,             
                sizeof (REDFISH_ISCSI_ATTEMPT_CONFIG_DATA),
                RedfishAttemptData
                );
    if (EFI_ERROR (Status)) {
        DEBUG((EFI_D_ERROR,"\nSetVariable  RedfishIScsiAttemptConfigData Status :: %r", Status));
        return Status;
    }
    DEBUG((DEBUG_INFO,"\nSetVariable  RedfishIScsiAttemptConfigData Status :: %r", Status));    
    
    if(Status == EFI_SUCCESS)
        gRedfishIScsiDataAvailable = TRUE;
    return EFI_SUCCESS;
}

/**
  Get the IScsi configuration data retrieved from NVRAM

  @param[in]   EFI_REDFISH_ISCSI_CONFIG_PROTOCOL*   This
  @param[out]   UINTN                               IScsi configuration data size
  @param[out]  REDFISH_ISCSI_ATTEMPT_CONFIG_DATA    IScsi configuration data

  @retval EFI_SUCCESS             The specified configuration data is retrieved successfully.
  @retval EFI_NOT_FOUND           The specified configuration data is not set.
**/
EFI_STATUS
EFIAPI
EfiRedfishConfigGetData (
    IN EFI_REDFISH_ISCSI_CONFIG_PROTOCOL    *This,
    OUT UINTN                               *RedfishDataSize,
    OUT REDFISH_ISCSI_ATTEMPT_CONFIG_DATA   **IScsiAttemptData
)
{
    EFI_STATUS   Status;
    CHAR16       AttemptStr[25];
    UINTN        AttemptConfigOrderSize = 0;
    UINT8        *AttemptConfigOrder;
    UINTN        NumberofAttempt = 0;
    UINTN        Index;
    UINTN        AttempDataIndex = 0;
    ISCSI_ATTEMPT_CONFIG_NVDATA *AttemptTmp;
    REDFISH_ISCSI_ATTEMPT_CONFIG_DATA   *IScsiAttemptDataTemp = (REDFISH_ISCSI_ATTEMPT_CONFIG_DATA   *)NULL;

    DEBUG((DEBUG_INFO,"\nIn %a ENTRY", __FUNCTION__));

    AttemptConfigOrder = IScsiGetVariableAndSize (
                                                 L"AttemptOrder",
                                                 &gIScsiConfigGuid,
                                                 &AttemptConfigOrderSize
                                                 );
    if ((AttemptConfigOrder == NULL) || (AttemptConfigOrderSize == 0))
        return EFI_NOT_FOUND;

    NumberofAttempt = AttemptConfigOrderSize / sizeof (UINT8);
    IScsiAttemptDataTemp = AllocateZeroPool( NumberofAttempt * sizeof(REDFISH_ISCSI_ATTEMPT_CONFIG_DATA) );

    for (Index = 0; Index < NumberofAttempt; Index++) {
        UnicodeSPrint ( AttemptStr, 25, L"Attempt %d", (UINTN) AttemptConfigOrder[Index] );
        Status = GetVariable2 (AttemptStr, &gEfiIScsiInitiatorNameProtocolGuid, (VOID**)&AttemptTmp, NULL );
        if(AttemptTmp == NULL || EFI_ERROR (Status)) {
              continue;
        }
        // Change ISCSI_ATTEMPT_CONFIG_NVDATA  to ISCSI_ATTEMPT_CONFIG_NVDATA
        Status = IScsitoRedfishFrameAttemptConfigData(AttemptTmp, &IScsiAttemptDataTemp[AttempDataIndex]);
        if(EFI_ERROR(Status)){
            FreePool(IScsiAttemptDataTemp);
            return Status;
        }
        AttempDataIndex++;
    }
    *RedfishDataSize = (AttempDataIndex * sizeof(REDFISH_ISCSI_ATTEMPT_CONFIG_DATA));
    *IScsiAttemptData =  IScsiAttemptDataTemp;

    return Status;
}


/**
  Delete the IScsi configuration data from NVRAM

  @param[in]   EFI_REDFISH_ISCSI_CONFIG_PROTOCOL   *This

  @retval EFI_SUCCESS             IScsi configuration data from NVRAM deleted successfully.
  @retval EFI_NOT_FOUND           IScsi configuration NVRAM variable not found
**/
EFI_STATUS
EFIAPI
EfiRedfishConfigDeleteAttempts (
    IN EFI_REDFISH_ISCSI_CONFIG_PROTOCOL    *This
)
{
    EFI_STATUS   Status;
    CHAR16       AttemptStr[25];
    UINTN        AttemptConfigOrderSize = 0;
    UINTN        AttemptBufferSize = 0;
    UINT8        *AttemptConfigOrder;
    UINTN        NumberofAttempt = 0;
    UINTN        Index;
    ISCSI_ATTEMPT_CONFIG_NVDATA        *AttemptBuffer = (VOID *)NULL;

    DEBUG((DEBUG_INFO,"\nIn %a ENTRY", __FUNCTION__));

    AttemptConfigOrder = IScsiGetVariableAndSize (
                                                 L"AttemptOrder",
                                                 &gIScsiConfigGuid,
                                                 &AttemptConfigOrderSize
                                                 );
    if ((AttemptConfigOrder == NULL) || (AttemptConfigOrderSize == 0))
        return EFI_NOT_FOUND;

    NumberofAttempt = AttemptConfigOrderSize / sizeof (UINT8);

 	for (Index = 0; Index < NumberofAttempt; Index++) {
        UnicodeSPrint ( AttemptStr, 25, L"Attempt %d", (UINTN) AttemptConfigOrder[Index] );
        Status = GetVariable2 (AttemptStr, &gEfiIScsiInitiatorNameProtocolGuid, (VOID**)&AttemptBuffer,&AttemptBufferSize);
        if(!EFI_ERROR(Status)){
            ZeroMem (AttemptBuffer,AttemptBufferSize);
            UnicodeStrToAsciiStr (AttemptStr, AttemptBuffer->AttemptName);
            AttemptBuffer->AttemptConfigIndex = AttemptConfigOrder[Index];
            AttemptBuffer->SessionConfigData.TargetPort        = ISCSI_WELL_KNOWN_PORT;
            AttemptBuffer->SessionConfigData.ConnectTimeout    = CONNECT_DEFAULT_TIMEOUT;
            AttemptBuffer->SessionConfigData.ConnectRetryCount = CONNECT_MIN_RETRY;
            AttemptBuffer->AuthenticationType           = ISCSI_AUTH_TYPE_NONE;
            AttemptBuffer->AuthConfigData.CHAP.CHAPType = ISCSI_CHAP_MUTUAL;

            Status = gRT->SetVariable ( AttemptStr, &gEfiIScsiInitiatorNameProtocolGuid, ISCSI_CONFIG_VAR_ATTR, AttemptBufferSize, AttemptBuffer );
        }
    }
    Status = gRT->SetVariable ( L"AttemptOrder", &gIScsiConfigGuid, ISCSI_CONFIG_VAR_ATTR, 0, NULL );
    DEBUG((DEBUG_INFO,"\nEfiRedfishConfigDeleteData AttemptOrder SetVariable Status: %r \n", Status));
    return Status;
}


/**
  Initialize the iSCSI configuration form.

  @param[in]  DriverBindingHandle The iSCSI driverbinding handle.

  @retval EFI_SUCCESS             The iSCSI configuration form is initialized.
  @retval EFI_OUT_OF_RESOURCES    Failed to allocate memory.

**/
EFI_STATUS
RedfishIScsiInit (
    IN EFI_HANDLE  ImageHandle
)
{
    EFI_STATUS Status = EFI_SUCCESS;
    EFI_REDFISH_ISCSI_CONFIG_PROTOCOL *RedfishIScsi;
    
    DEBUG((DEBUG_INFO,"\nIn %a ENTRY", __FUNCTION__));
    
    RedfishIScsi = AllocateZeroPool( sizeof(EFI_REDFISH_ISCSI_CONFIG_PROTOCOL) );

    RedfishIScsi->Version = REDFISH_ISCSI_CONFIG_PROTOCOL_VERSION;
    RedfishIScsi->SetData = EfiRedfishConfigSetData;
    RedfishIScsi->GetData = EfiRedfishConfigGetData;
    RedfishIScsi->DeleteAttempt = EfiRedfishConfigDeleteAttempts;

    Status = gBS->InstallMultipleProtocolInterfaces (
                      &ImageHandle,
                      &gEfiRedfishIScsiConfigProtocolGuid,
                      RedfishIScsi,
                      NULL
                      );

    DEBUG((DEBUG_INFO,"\nIn %a, InstallMultipleProtocolInterfaces Status :: %r", __FUNCTION__, Status));

    return Status;
}

/**
  Overrides the Redfish Attempt with same MAC.

  @param[in]  IScsiAttemptData ISCSI_ATTEMPT_CONFIG_NVDATA attempt data.

  @retval EFI_SUCCESS             The Redfish Attempt is overrided.
  @retval EFI_OUT_OF_RESOURCES    Failed to allocate memory.

**/
EFI_STATUS
IScsiRedfishDeleteAttempts ( 
    IN ISCSI_ATTEMPT_CONFIG_NVDATA *IScsiAttemptData
) 
{
    EFI_STATUS                    Status = EFI_SUCCESS;
    UINTN                         Index;
    UINTN                         NewIndex;
    UINT8                         *AttemptConfigOrder;
    UINTN                         AttemptConfigOrderSize;
    UINT8                         *AttemptNewOrder;
    UINT8                         AttemptConfigIndex;
    UINT32                        Attribute;
    UINTN                         Total;
    UINTN                         NewTotal;
    ISCSI_ATTEMPT_CONFIG_NVDATA   *AttemptConfigData;  
    CHAR16                        MacString[ISCSI_MAX_MAC_STRING_LEN];
    CHAR8                         MacString8[ISCSI_MAX_MAC_STRING_LEN];
    CHAR16                        AttemptMacString[ISCSI_MAX_MAC_STRING_LEN];
    CHAR8                         AttemptMacString8[ISCSI_MAX_MAC_STRING_LEN];
    ISCSI_SESSION_CONFIG_NVDATA   *ConfigData;

    AttemptConfigOrder = IScsiGetVariableAndSize (
                                             L"AttemptOrder",
                                             &gIScsiConfigGuid,
                                             &AttemptConfigOrderSize
                                             );
    if ((AttemptConfigOrder == NULL) || (AttemptConfigOrderSize == 0)) {
        return Status;
    }
    RedfishIscsiConvertMacAddrToAsciiStr(IScsiAttemptData->MacString, REDFISH_NIC_SIZE, MacString8);
    AsciiStrToUnicodeStrS (MacString8, MacString, sizeof (MacString) / sizeof (MacString[0])); 
    
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
        RedfishIscsiConvertMacAddrToAsciiStr(AttemptConfigData->MacString, REDFISH_NIC_SIZE, AttemptMacString8);
        AsciiStrToUnicodeStrS (AttemptMacString8, AttemptMacString, sizeof (AttemptMacString) / sizeof (AttemptMacString[0]));
                       
        if((AttemptConfigData->AttemptType == REDFISH_ATTEMPT) && (!StrCmp(MacString, AttemptMacString))) {
            AttemptNewOrder = AllocateZeroPool (AttemptConfigOrderSize);
            if (AttemptNewOrder == NULL) {
                Status = EFI_OUT_OF_RESOURCES;
                goto RedfishError;
            }

            Total    = AttemptConfigOrderSize / sizeof (UINT8);
            NewTotal = Total;
            
            //
            // Delete the attempt.
            //

// Trigger the hook to clear an attempt.
#if(ENABLE_CHAP_SECRET_ENCRYPTION)
            Status = PrepareToEncrypt(AttemptConfigData,ClearAnEntry);
#endif

            AttemptConfigIndex = AttemptConfigData->AttemptConfigIndex;
            FreePool (AttemptConfigData); 
            //
            // Create a new Attempt
            //
            AttemptConfigData = AllocateZeroPool (sizeof (ISCSI_ATTEMPT_CONFIG_NVDATA)); 
            if (AttemptConfigData == NULL) {
                return EFI_OUT_OF_RESOURCES;
            }
            ConfigData                    = &AttemptConfigData->SessionConfigData;
            ConfigData->TargetPort        = ISCSI_WELL_KNOWN_PORT;
            ConfigData->ConnectTimeout    = CONNECT_DEFAULT_TIMEOUT;
            ConfigData->ConnectRetryCount = CONNECT_MIN_RETRY;
            //
            // Filling ChapType and AuthenticationType with default values matching vfr
            //
            AttemptConfigData->AuthenticationType           = ISCSI_AUTH_TYPE_NONE;
            AttemptConfigData->AuthConfigData.CHAP.CHAPType = ISCSI_CHAP_MUTUAL;
        
            //
            // Configure the Attempt index and set variable.
            //
            AttemptConfigData->AttemptConfigIndex = AttemptConfigIndex;

            //
            // Set the attempt name to default.
            //
            UnicodeSPrint (
                mPrivate->PortString,
                (UINTN) ISCSI_NAME_IFR_MAX_SIZE,
                L"Attempt %d",
                (UINTN) AttemptConfigData->AttemptConfigIndex
                );
            UnicodeStrToAsciiStrS (mPrivate->PortString, AttemptConfigData->AttemptName, ATTEMPT_NAME_SIZE);
            gRT->SetVariable (
                mPrivate->PortString,
                &gEfiIScsiInitiatorNameProtocolGuid,
                ISCSI_CONFIG_VAR_ATTR,
                sizeof (ISCSI_ATTEMPT_CONFIG_NVDATA),
                AttemptConfigData
                );
            
            //
            // Mark the attempt order in NVR to be deleted - 0.
            //
            for (NewIndex = 0; NewIndex < Total; NewIndex++) {
                if (AttemptConfigOrder[NewIndex] == AttemptConfigData->AttemptConfigIndex) {
                    AttemptConfigOrder[NewIndex] = 0;
                    break;
                }
            }

            NewTotal--;
            if (mCallbackInfo->Current == AttemptConfigData) {
                mCallbackInfo->Current = NULL;
            }
            FreePool (AttemptConfigData);

            //
            // Construct AttemptNewOrder.
            //
            for (Index = 0, NewIndex = 0; Index < Total; Index++) {
                if (AttemptConfigOrder[Index] != 0) {
                    AttemptNewOrder[NewIndex] = AttemptConfigOrder[Index];
                    NewIndex++;
                }
            }

            Attribute = EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_NON_VOLATILE;
            
            //
            // Update AttemptOrder in NVR.
            //
            Status = gRT->SetVariable (
                      L"AttemptOrder",
                      &gIScsiConfigGuid,
                      Attribute,
                      NewTotal * sizeof (UINT8),
                      AttemptNewOrder
                      );
            RedfishError:
                 if (AttemptConfigOrder != NULL) {
                     FreePool (AttemptConfigOrder);
                 }

                 if (AttemptNewOrder != NULL) {
                     FreePool (AttemptNewOrder);
                 }
            }
        }
         
  return Status;          
}

