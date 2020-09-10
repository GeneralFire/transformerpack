//*************************************************************************
//*************************************************************************
//**                                                                     **
//**        (C)Copyright 1985-2018, American Megatrends, Inc.            **
//**                                                                     **
//**                       All Rights Reserved.                          **
//**                                                                     **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093           **
//**                                                                     **
//**                       Phone: (770)-246-8600                         **
//**                                                                     **
//*************************************************************************
//*************************************************************************

/** @file BmcSelfTestLog.h
    Header file for BmcSeltTestLog module.

*/

#ifndef _BMC_SELF_TEST_LOG_H_
#define _BMC_SELF_TEST_LOG_H_

//---------------------------------------------------------------------------

#include <Token.h>
#include <ServerMgmtSetup.h>
#include <Library/DebugLib.h>
#include <Library/PrintLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Protocol/IpmiSelfTestLogProtocol.h>
#include <AmiIpmiBmcCommon.h>

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------


#define VAR_FLAG_ATTRIBUTES     EFI_VARIABLE_BOOTSERVICE_ACCESS | \
                                EFI_VARIABLE_NON_VOLATILE

// Default values for setup data.
#define ERASE_ON_EVERY_RESET    0x01
#define CLEAR_LOG               0x01
#define DONOT_LOG_ANY_MORE      0x00
#define STRING_BUFFER_LENGTH    100
#define MASK_LOWER_NIBBLE       0x0F

#pragma pack(1)

/**
    Bmc self test log data structure.
*/
typedef struct {
    /// Log size.
    UINT8           LogSize;
    UINT8           Log[AMI_IPMI_BST_LOG_SIZE];      ///< List of logs.
    EFI_HII_DATE    Date[AMI_IPMI_BST_LOG_SIZE];     ///< Date of each log.
    EFI_HII_TIME    Time[AMI_IPMI_BST_LOG_SIZE];     ///< Time of each log.
} BMC_SELF_TEST_LOG_DATA;

#pragma pack()

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

BMC_SELF_TEST_LOG_DATA  gBstLog;

//---------------------------------------------------------------------------

#endif  // #ifndef _BMC_SELF_TEST_LOG_H_

//*************************************************************************
//*************************************************************************
//**                                                                     **
//**        (C)Copyright 1985-2018, American Megatrends, Inc.            **
//**                                                                     **
//**                       All Rights Reserved.                          **
//**                                                                     **
//**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093           **
//**                                                                     **
//**                       Phone: (770)-246-8600                         **
//**                                                                     **
//*************************************************************************
//*************************************************************************
