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

/** @file IpmiSelfTestLogProtocol.h
    Ipmi Self test log protocol structure and macros.

*/

#ifndef _IPMI_SELF_TEST_LOG_PROTOCOL_H_
#define _IPMI_SELF_TEST_LOG_PROTOCOL_H_

//---------------------------------------------------------------------------

#include <IndustryStandard/IpmiNetFnApp.h>
#include <Pi/PiStatusCode.h>

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

#define EFI_BMC_SELF_TEST_LOG_PROTOCOL_GUID \
    { \
        0x9fe6a332, 0x1a1f, 0x43e3, { 0xbd, 0x25, 0xf7, 0x89, 0x1c, 0x6f, 0xc7, 0x4a } \
    }

/* Computing Unit Firmware Processor Subclass Error Code definitions.
   Hard fail, Soft fail and Communication errors are already defined in PiStatusCode.h
*/
#define EFI_CU_FP_EC_SEL_NOT_ACCESSIBLE               (EFI_SUBCLASS_SPECIFIC | 0x00000003)
#define EFI_CU_FP_EC_SDR_REPOSITORY_NOT_ACCESSABLE    (EFI_SUBCLASS_SPECIFIC | 0x00000004)
#define EFI_CU_FP_EC_BMC_FRU_NOT_ACCESSIBLE           (EFI_SUBCLASS_SPECIFIC | 0x00000005)
#define EFI_CU_FP_EC_IPMB_NOT_RESPONDING              (EFI_SUBCLASS_SPECIFIC | 0x00000006)
#define EFI_CU_FP_EC_SDR_EMPTY                        (EFI_SUBCLASS_SPECIFIC | 0x00000007)
#define EFI_CU_FP_EC_BMC_FRU_CORRUPTED                (EFI_SUBCLASS_SPECIFIC | 0x00000008)
#define EFI_CU_FP_EC_BB_CORRUPTED                     (EFI_SUBCLASS_SPECIFIC | 0x00000009)
#define EFI_CU_FP_EC_FIRMWARE_CORRUPTED               (EFI_SUBCLASS_SPECIFIC | 0x0000000a)
#define EFI_CU_FP_EC_FORCE_UPDATE_MODE                (EFI_SUBCLASS_SPECIFIC | 0x0000000b)

#define AMI_IPMI_BST_LOG_SIZE     20

/**
    BMC Self test log protocol.
*/
typedef struct {
    /// Self test error count.
    UINT32                         BmcSelfTestErrorCount;
    /// Self test errors.
    UINT32                         BmcSelfTestError[AMI_IPMI_BST_LOG_SIZE + 1];
    /// Represents get device id command passed or not.
    BOOLEAN                        IsGetDeviceIdCommandPassed;
    /// Get device id command response.
    IPMI_GET_DEVICE_ID_RESPONSE    GetDeviceIdResponse;
    /// BMC Status
    UINT32                         BmcStatus;
} EFI_BMC_SELF_TEST_LOG_PROTOCOL;

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

extern EFI_GUID gEfiBmcSelfTestLogProtocolGuid;

//---------------------------------------------------------------------------

#endif  // #ifndef _IPMI_SELF_TEST_LOG_PROTOCOL_H_

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
