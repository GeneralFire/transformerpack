//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************
/** @file RsdIScsiConfig.h
    Header file that provides the declarations of 
    AMI Override functions for the Rsd-IScsi Setup feature 

**/
#ifndef _RSD_ISCSI_CONFIG_H_
#define _RSD_ISCSI_CONFIG_H_

#include <Token.h>
#include "IScsiImpl.h"
#include "AmiIScsiConfig.h"

extern  CHAR8   gRsdIScsiInitiatorName[ISCSI_NAME_MAX_SIZE];

/**
  Provides the MAC of all the Attempts.

  @param[in]    Private                      The iSCSI FORM CALLBACK INFO.

**/
VOID 
RsdIscsiAttemptsList (
	ISCSI_FORM_CALLBACK_INFO    *Private
);


/**
  Provides the Attempt corresponding to the MAC.
  @param[in]    Private                      	The iSCSI FORM CALLBACK INFO.
  @param[in]    QuestionId                      QuestionId of the MAC choosen.
**/
VOID RsdIscsiAttemptDetails (
	ISCSI_FORM_CALLBACK_INFO  *Private,
	EFI_QUESTION_ID           QuestionId
);

/**
  Provides the Rsd attempt details.

  @param[in]    Private                      	 The iSCSI FORM CALLBACK INFO.
  @param[in]    RsdIScsiAttemptConfigData        Parsed ISCSI_ATTEMPT_CONFIG_NVDATA attempt data.

**/
VOID 
RsdIscsiData ( 
    ISCSI_ATTEMPT_CONFIG_NVDATA *RsdIScsiAttemptConfigData,
    ISCSI_FORM_CALLBACK_INFO    *Private 
);

#endif
