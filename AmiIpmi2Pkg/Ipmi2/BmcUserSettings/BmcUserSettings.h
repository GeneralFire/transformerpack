//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2020, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************


/** @file BmcUserSettings.h
    Includes for BMC User Settings.

*/

#ifndef _BMC_USER_SETTINGS_H_
#define _BMC_USER_SETTINGS_H_

//---------------------------------------------------------------------------

#include <Token.h>
#include <ServerMgmtSetup.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/AMIPostMgr.h>
#include <Protocol/IpmiTransportProtocol.h>
#include <Library/AmiIpmiLib.h>

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

#define MAX_BMC_USER_COUNT                              64
#define MAX_BMC_USER_NAME_LENGTH                        16
#define MAX_BMC_USER_NAME_LENGTH_WITH_NULL_CHARACTER    17

#define LAN_CHANNEL_TYPE                                4

#define PASSWORD_DOES_NOT_MATCH_CC                      0x80
#define CMD_NOT_SUPPORTED_CC                            0x82
#define COMPLETION_CODE_SUCCESS                         0
#define USER_PRIVILEGE_NO_ACCESS                        0x0F

#define MAX_BMC_USER_PASSWORD_LENGTH                        BMC_USER_PASSWORD_MAX_SIZE
#define MAX_BMC_USER_PASSWORD_LENGTH_WITH_NULL_CHARACTER    BMC_USER_PASSWORD_MAX_SIZE_WITH_NULL

#pragma pack(1)

/** @internal
    Bmc users details.
*/
typedef struct {
    /// User id.
    UINT8       UserId;
    /// User name.
    CHAR8       UserName[MAX_BMC_USER_NAME_LENGTH_WITH_NULL_CHARACTER];
} BMC_USER_DETAILS_LIST;

/** @internal
    Bmc users details with password.
*/
typedef struct {
    /// User id.
    UINT8       UserId;
    ///< User name.
    CHAR8       UserName[MAX_BMC_USER_NAME_LENGTH_WITH_NULL_CHARACTER];
    ///< User password.
    CHAR8       UserPassword[MAX_BMC_USER_PASSWORD_LENGTH_WITH_NULL_CHARACTER];
} BMC_USER_DETAILS;

#pragma pack()

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

static EFI_IPMI_TRANSPORT   *gIpmiTransport;
BMC_USER_DETAILS_LIST       gBmcUserDetailsList[MAX_BMC_USER_COUNT];
AMI_POST_MANAGER_PROTOCOL   *gAmiPostMgr = NULL;
UINT8                       gAllowableUser = 0;
BMC_USER_DETAILS            gBmcAddUserDetails;
BMC_USER_DETAILS            gBmcDeleteUserDetails;
BMC_USER_DETAILS            gBmcChangeUserDetails;
UINT8                       *gPcdLanChannelBufferPtr;
UINT8                       gPcdSize;
//---------------------------------------------------------------------------

#endif

