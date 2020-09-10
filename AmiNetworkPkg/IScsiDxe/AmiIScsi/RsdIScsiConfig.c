//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************


//<AMI_FHDR_START>
//
// Name:  RsdIscsiConfig.c
//
// Description: This contains RSD setup related functions
//
//<AMI_FHDR_END>
//**********************************************************************
#include "RsdIScsiConfig.h"


/**
  Provides the MAC of all the Attempts.
  @param[in]    Private                      The iSCSI FORM CALLBACK INFO.
**/
VOID 
RsdIscsiAttemptsList(ISCSI_FORM_CALLBACK_INFO    *Private)
{
	VOID                          *StartOpcodeHandle = NULL;
	VOID                          *EndOpcodeHandle = NULL;
	EFI_IFR_GUID_LABEL            *StartLabel = (EFI_IFR_GUID_LABEL *)NULL;
	EFI_IFR_GUID_LABEL            *EndLabel = (EFI_IFR_GUID_LABEL *)NULL;
	UINTN                         AttemptIndex = 0;
	UINT8                         *AttemptConfigOrder;
	ISCSI_ATTEMPT_CONFIG_NVDATA   *IScsiAttemptConfigData = NULL;
	EFI_STATUS                    Status;
	UINTN                         AttemptConfigOrderSize;
	UINTN                         AttemptCount = 0;
	
	StartOpcodeHandle = HiiAllocateOpCodeHandle();
	EndOpcodeHandle = HiiAllocateOpCodeHandle();
	
	StartLabel = (EFI_IFR_GUID_LABEL*) HiiCreateGuidOpCode(StartOpcodeHandle, &gEfiIfrTianoGuid, NULL, sizeof(EFI_IFR_GUID_LABEL));
	StartLabel->ExtendOpCode = EFI_IFR_EXTEND_OP_LABEL;
	StartLabel->Number = RSD_ATTEMPT_START;
	
	EndLabel = (EFI_IFR_GUID_LABEL*) HiiCreateGuidOpCode(EndOpcodeHandle, &gEfiIfrTianoGuid, NULL, sizeof(EFI_IFR_GUID_LABEL));
	EndLabel->ExtendOpCode = EFI_IFR_EXTEND_OP_LABEL;
	EndLabel->Number = RSD_ATTEMPT_END;
	
	AttemptConfigOrderSize = 0;
	AttemptConfigOrder = IScsiGetVariableAndSize (
	        L"InitialAttemptOrder",
	        &gIScsiConfigGuid,
	        &AttemptConfigOrderSize
	);
	AttemptCount = AttemptConfigOrderSize / sizeof(UINT8);
	
	for(AttemptIndex = 0; AttemptIndex < AttemptCount; AttemptIndex++)
	{
	    CHAR16 AttemptName[ISCSI_NAME_IFR_MAX_SIZE] = {0};
	    UINT16 AttemptNameId = 0;
	    UINTN Size = 0;
	    UnicodeSPrint (
	            AttemptName,
	            (UINTN) ISCSI_NAME_IFR_MAX_SIZE,
	            L"Attempt %d",
	            (UINTN) AttemptConfigOrder[AttemptIndex]
	    );
	    
	    Status = GetVariable2(AttemptName,
	            &gEfiIScsiInitiatorNameProtocolGuid,
	            (void**)&IScsiAttemptConfigData, 
	            &Size
	    );    
	    if(IScsiAttemptConfigData->AttemptType == RSD_ATTEMPT)
	    {
	        AttemptNameId = HiiSetString(Private->RegisteredHandle, 0, AttemptName, NULL);
	        HiiCreateGotoOpCode(StartOpcodeHandle, FORMID_RSD_ATTEMPT_FORM, AttemptNameId, AttemptNameId,EFI_IFR_FLAG_CALLBACK,KEY_RSD_UPDATE_ATTEMPT_BASE + (EFI_QUESTION_ID)IScsiAttemptConfigData->AttemptConfigIndex);
	    }
	}
	HiiUpdateForm(Private->RegisteredHandle, &gIScsiConfigGuid, FORMID_RSD_FORM, StartOpcodeHandle, EndOpcodeHandle);         
	FreePool(IScsiAttemptConfigData);
}   
    
/**
  Provides the Attempt corresponding to the MAC.
  @param[in]    Private                      	The iSCSI FORM CALLBACK INFO.
  @param[in]    QuestionId                      QuestionId of the MAC choosen.
**/
VOID RsdIscsiAttemptDetails(
	ISCSI_FORM_CALLBACK_INFO    *Private,
	EFI_QUESTION_ID         QuestionId)
{
	 UINTN                           Size = 0;
	 CHAR16                          AttemptName[ISCSI_NAME_IFR_MAX_SIZE] = {0};
	 ISCSI_ATTEMPT_CONFIG_NVDATA     *IScsiAttemptConfigData;  
	 EFI_STATUS                      Status;
	 
	 UnicodeSPrint (
	         AttemptName,
	         (UINTN) ISCSI_NAME_IFR_MAX_SIZE,
	         L"Attempt %d",
	         (UINTN)(QuestionId - KEY_RSD_UPDATE_ATTEMPT_BASE)
	 );
	 HiiSetString(Private->RegisteredHandle, STR_RSD_ISCSI_ATTEMPT_NAME_VALUE,AttemptName, NULL);
	 
	 Status = GetVariable2 (
	         AttemptName,
	         &gEfiIScsiInitiatorNameProtocolGuid,
	         (void**)&IScsiAttemptConfigData,
	         &Size 
	 );
	 DEBUG((DEBUG_INFO,"\n RSD:: GetVariable2 IScsiAttemptConfigData Status is %r",Status));
	 
	 if(Status == EFI_SUCCESS)
	     RsdIscsiData(IScsiAttemptConfigData,Private);
            
	 //Free the buffer
	 FreePool(IScsiAttemptConfigData);
}   

/**
  Provides the Rsd attempt details.

  @param[in]    Private                      	 The iSCSI FORM CALLBACK INFO.
  @param[in]    RsdIScsiAttemptConfigData        Parsed ISCSI_ATTEMPT_CONFIG_NVDATA attempt data.

**/
VOID RsdIscsiData( ISCSI_ATTEMPT_CONFIG_NVDATA *RsdIScsiAttemptConfigData,ISCSI_FORM_CALLBACK_INFO *Private )
{
    //IscsiMode
    CHAR16          BufString[128] = {L""};
    CHAR16          *ChapStatusStr, *IpModeStatusStr;//  AMI PORTING - Variable declaration to give multilanguage support.
    ChapStatusStr = NULL;//  AMI PORTING - Variable declaration to give multilanguage support.
    IpModeStatusStr = NULL;//  AMI PORTING - Variable declaration to give multilanguage support.
    if(RsdIScsiAttemptConfigData->SessionConfigData.Enabled )
    {
        UnicodeSPrint (BufString, 64, L"Enabled");
    }
    else
    {
        UnicodeSPrint (BufString, 64, L"Disabled");
    }
    HiiSetString(Private->RegisteredHandle, STR_RSD_ISCSI_MODE_VALUE,BufString, NULL);

    //IPMode
    if(RsdIScsiAttemptConfigData->SessionConfigData.IpMode == IP_MODE_IP4)
    {
        UnicodeSPrint (BufString,64 , L"IP4");
    }
    else if(RsdIScsiAttemptConfigData->SessionConfigData.IpMode == IP_MODE_IP6)
    {
        UnicodeSPrint (BufString, 64, L"IP6"); 
    }
    else
    {
        UnicodeSPrint (BufString, 64, L"Autoconfig"); 
    }
    HiiSetString(Private->RegisteredHandle, STR_RSD_IP_MODE_VALUE, BufString, NULL);

    // connection retry
    UnicodeSPrint (BufString, ISCSI_CHAP_NAME_MAX_LEN, L"%d", RsdIScsiAttemptConfigData->SessionConfigData.ConnectRetryCount); 
    HiiSetString(Private->RegisteredHandle, STR_RSD_ISCSI_CONFIG_RETRY_VALUE, BufString , NULL);

    // connection timeout
    UnicodeSPrint (BufString, ISCSI_CHAP_NAME_MAX_LEN, L"%d", RsdIScsiAttemptConfigData->SessionConfigData.ConnectTimeout); 
    HiiSetString(Private->RegisteredHandle, STR_RSD_ISCSI_CONFIG_TIMEOUT_VALUE,BufString, NULL);

    // ISId
    IScsiConvertIsIdToString (BufString, RsdIScsiAttemptConfigData->SessionConfigData.IsId); 
    HiiSetString(Private->RegisteredHandle, STR_RSD_ISCSI_CONFIG_ISID_VALUE, BufString , NULL);

    //Initiator Name
    AsciiStrToUnicodeStrS (gRsdIScsiInitiatorName, BufString, sizeof (BufString) / sizeof (BufString[0]));
    HiiSetString(Private->RegisteredHandle, STR_RSD_ISCSI_CONFIG_INIT_NAME_VALUE, BufString, NULL);
    
    // Initiator IP Configuration
    if(RsdIScsiAttemptConfigData->SessionConfigData.InitiatorInfoFromDhcp == 1) 
    {
        IpModeStatusStr = HiiGetString(Private->RegisteredHandle,STRING_TOKEN(STR_ISCSI_DHCP_IP_CONFIG),NULL);          
    }
    else
    {
        IpModeStatusStr = HiiGetString(Private->RegisteredHandle,STRING_TOKEN(STR_ISCSI_STATIC_IP_CONFIG),NULL);          
    }
    HiiSetString(Private->RegisteredHandle, STR_RSD_ISCSI_INIT_IP_CONFIG_VALUE, IpModeStatusStr , NULL);

    // Local IP
    if(RsdIScsiAttemptConfigData->SessionConfigData.IpMode == IP_MODE_IP4 || RsdIScsiAttemptConfigData->SessionConfigData.IpMode == IP_MODE_AUTOCONFIG )
    {   
        IScsiIpToStr ((VOID*)&RsdIScsiAttemptConfigData->SessionConfigData.LocalIp, FALSE, BufString);
    }
    else
    {
        IScsiIpToStr ((VOID*)&RsdIScsiAttemptConfigData->SessionConfigData.LocalIp, TRUE, BufString);
    }
    HiiSetString(Private->RegisteredHandle, STR_RSD_ISCSI_LOCAL_IP_ADDRESS_VALUE, BufString, NULL);

    // Subnet Mask
    if(RsdIScsiAttemptConfigData->SessionConfigData.IpMode == IP_MODE_IP4 || RsdIScsiAttemptConfigData->SessionConfigData.IpMode == IP_MODE_AUTOCONFIG )
    {
        UnicodeSPrint (
                BufString,
                (UINTN) 2 *IP4_STR_MAX_SIZE,
                L"%d.%d.%d.%d",
                (UINTN) RsdIScsiAttemptConfigData->SessionConfigData.SubnetMask.Addr[0],
                (UINTN) RsdIScsiAttemptConfigData->SessionConfigData.SubnetMask.Addr[1],
                (UINTN) RsdIScsiAttemptConfigData->SessionConfigData.SubnetMask.Addr[2],
                (UINTN) RsdIScsiAttemptConfigData->SessionConfigData.SubnetMask.Addr[3]
        );
        HiiSetString(Private->RegisteredHandle, STR_RSD_ISCSI_LOCAL_MASK_VALUE, BufString, NULL);
    }

    //Gateway
    if(RsdIScsiAttemptConfigData->SessionConfigData.IpMode == IP_MODE_IP4 || RsdIScsiAttemptConfigData->SessionConfigData.IpMode == IP_MODE_AUTOCONFIG )
    {
        IScsiIpToStr ((VOID*)&RsdIScsiAttemptConfigData->SessionConfigData.Gateway, FALSE, BufString);  
        HiiSetString(Private->RegisteredHandle, STR_RSD_ISCSI_LOCAL_GATEWAY_VALUE,BufString, NULL);
    }

    // Target IP Configuration
    if(RsdIScsiAttemptConfigData->SessionConfigData.TargetInfoFromDhcp == 1) 
    {
        IpModeStatusStr = HiiGetString(Private->RegisteredHandle,STR_ISCSI_DHCP_IP_CONFIG,NULL);
    }
    else
    {
        IpModeStatusStr = HiiGetString(Private->RegisteredHandle,STR_ISCSI_STATIC_IP_CONFIG,NULL);   
    }
    HiiSetString(Private->RegisteredHandle, STR_RSD_ISCSI_SERVER_IP_CONFIG_VALUE, IpModeStatusStr , NULL);
    
    //TargetName
    AsciiStrToUnicodeStrS(RsdIScsiAttemptConfigData->SessionConfigData.TargetName, BufString, sizeof (BufString) / sizeof (BufString[0]));
    HiiSetString(Private->RegisteredHandle, STR_RSD_ISCSI_TARGET_NAME_VALUE, BufString , NULL);

    // Target IP
    if(RsdIScsiAttemptConfigData->SessionConfigData.IpMode == IP_MODE_IP4 || RsdIScsiAttemptConfigData->SessionConfigData.IpMode == IP_MODE_AUTOCONFIG )
    {
        IScsiIpToStr (&RsdIScsiAttemptConfigData->SessionConfigData.TargetIp, FALSE, BufString);   
    }
    if(RsdIScsiAttemptConfigData->SessionConfigData.IpMode == IP_MODE_IP6)
    {
        IScsiIpToStr (&RsdIScsiAttemptConfigData->SessionConfigData.TargetIp, TRUE, BufString);    
    }
    HiiSetString(Private->RegisteredHandle, STR_RSD_ISCSI_TARGET_ADDRESS_VALUE, BufString , NULL);

    //Target port
    UnicodeSPrint (BufString, ISCSI_CHAP_NAME_MAX_LEN, L"%d", RsdIScsiAttemptConfigData->SessionConfigData.TargetPort); 
    HiiSetString(Private->RegisteredHandle, STR_RSD_ISCSI_TARGET_PORT_VALUE,BufString, NULL);

    //Boot LUN
    IScsiLunToUnicodeStr(RsdIScsiAttemptConfigData->SessionConfigData.BootLun, BufString); 
    HiiSetString(Private->RegisteredHandle, STR_RSD_ISCSI_BOOT_LUN_VALUE,BufString, NULL);

    // Authentication type
    if(RSD_ISCSI_NO_CHAP == RsdIScsiAttemptConfigData->AuthenticationType) 
    {
        ChapStatusStr = HiiGetString(Private->RegisteredHandle,STR_AUTHEN_TYPE_NONE,NULL); 
        HiiSetString(Private->RegisteredHandle, STR_RSD_AUTHEN_TYPE_VALUE, ChapStatusStr, NULL);
        ChapStatusStr = HiiGetString(Private->RegisteredHandle,STR_AUTHEN_TYPE_NONE,NULL); 
        HiiSetString(Private->RegisteredHandle, STR_RSD_CHAP_TYPE_VALUE, ChapStatusStr, NULL);
        ChapStatusStr = HiiGetString(Private->RegisteredHandle,STR_CHAP_STATUS_NOT_INSTALLED,NULL); 
        HiiSetString(Private->RegisteredHandle, STR_RSD_CHAP_SECRET_STATUS_VALUE, ChapStatusStr, NULL);
        HiiSetString(Private->RegisteredHandle, STR_RSD_REVERSE_CHAP_SECRET_STATUS_VALUE, ChapStatusStr, NULL);
    } 
    else 
    {
        if( ISCSI_CHAP_UNI == RsdIScsiAttemptConfigData->AuthConfigData.CHAP.CHAPType)
        {
            ChapStatusStr = HiiGetString(Private->RegisteredHandle,STR_AUTHEN_TYPE_CHAP,NULL); 
            HiiSetString(Private->RegisteredHandle, STR_RSD_AUTHEN_TYPE_VALUE, ChapStatusStr, NULL);            
            ChapStatusStr = HiiGetString(Private->RegisteredHandle,STR_CHAP_TYPE_UNI,NULL); 
            HiiSetString(Private->RegisteredHandle, STR_RSD_CHAP_TYPE_VALUE, ChapStatusStr, NULL);
            ChapStatusStr = HiiGetString(Private->RegisteredHandle,STR_CHAP_STATUS_INSTALLED,NULL);
            HiiSetString(Private->RegisteredHandle, STR_RSD_CHAP_SECRET_STATUS_VALUE, ChapStatusStr, NULL);    
        }
        else
        {
            ChapStatusStr = HiiGetString(Private->RegisteredHandle,STR_AUTHEN_TYPE_CHAP,NULL); 
            HiiSetString(Private->RegisteredHandle, STR_RSD_AUTHEN_TYPE_VALUE, ChapStatusStr, NULL);
            ChapStatusStr = HiiGetString(Private->RegisteredHandle,STR_CHAP_TYPE_MUTUAL,NULL); 
            HiiSetString(Private->RegisteredHandle, STR_RSD_CHAP_TYPE_VALUE, ChapStatusStr, NULL);
            ChapStatusStr = HiiGetString(Private->RegisteredHandle,STR_CHAP_STATUS_INSTALLED,NULL);
            HiiSetString(Private->RegisteredHandle, STR_RSD_CHAP_SECRET_STATUS_VALUE, ChapStatusStr, NULL);
            HiiSetString(Private->RegisteredHandle, STR_RSD_REVERSE_CHAP_SECRET_STATUS_VALUE, ChapStatusStr, NULL);
        }        
    }
} 
