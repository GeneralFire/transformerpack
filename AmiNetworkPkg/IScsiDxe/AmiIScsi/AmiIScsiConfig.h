//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************
/** @file AmiIScsiConfig.h
    Header file for AMI Override functions for the IScsi EDKII driver 

**/
#ifndef __AMI_ISCSI_CONFIG__H__
#define __AMI_ISCSI_CONFIG__H__
#include <Token.h>
#include <AMIVfr.h>

#define FORMID_RSD_FORM                  6  
#define FORMID_RSD_ATTEMPT_FORM          7
#define FORMID_REDFISH_FORM              8 
#define FORMID_REDFISH_ATTEMPT_LIST_FORM 9
#define FORMID_REDFISH_ATTEMPT_DATA_FORM 10
#define FORMID_LOCAL_ISCSI_FORM          11
#define FORMID_PRIORITY_FORM             12
  
#define KEY_REDFISH_CONFIGURATION  0x117 
#define KEY_SAVE_PRIORITY_CHANGES  0x118
#define KEY_PRIORITY_CHANGE_CONFIG 0x119
#define PRIORITY_ENTRY_LABEL       0xc000

#define RSD_ATTEMPT_START   0x250
#define RSD_ATTEMPT_END     0x251
#define LOCAL_ATTEMPT_START 0x252
#define LOCAL_ATTEMPT_END   0x253

#define KEY_RSD_ISCSI_FORM          0x200
#define KEY_RSD_UPDATE_ATTEMPT_LIST 0x203
#define KEY_RSD_UPDATE_ATTEMPT_BASE 0x400

#define RSD_ISCSI_NO_CHAP           0x00
#define RSD_ISCSI_UNI_CHAP          0x01
#define RSD_ISCSI_MUTUAL_CHAP       0x02

#define REDFISH_MAC_START   0x255
#define REDFISH_MAC_END     0x256

#define KEY_REDFISH_ISCSI_FORM          0x300
#define KEY_REDFISH_UPDATE_ATTEMPT_LIST 0x303
#define KEY_REDFISH_UPDATE_ATTEMPT_BASE 0x500

#define REDFISH_ISCSI_NO_CHAP               0x00
#define REDFISH_ISCSI_UNI_CHAP              0x01
#define REDFISH_ISCSI_MUTUAL_CHAP           0x02

#endif
