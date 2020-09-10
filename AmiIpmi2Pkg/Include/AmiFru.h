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

/** @file AmiFru.h
    Header file for Fru Area usage.

*/

#ifndef _AMI_FRU_H_
#define _AMI_FRU_H_

//---------------------------------------------------------------------------

#include <IndustryStandard/IpmiNetFnStorage.h>
#include <Protocol/RedirFruProtocol.h>

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

#define MAX_FRU_SLOT    20

#define AMI_IPMI_INSTANCE_FROM_EFI_SM_IPMI_FRU_THIS(a) CR ( \
                                                            a, \
                                                            AMI_IPMI_FRU_GLOBAL, \
                                                            IpmiRedirFruProtocol, \
                                                            EFI_SM_FRU_REDIR_SIGNATURE \
                                                          )

#pragma pack(1)
/**
    FRU device info.
*/
typedef struct {
    /// Valid FRU bit.
    BOOLEAN               Valid;
    IPMI_FRU_DATA_INFO    FruDevice;    ///< FRU data info.
} AMI_FRU_DEVICE_INFO;

/**
    All FRU slot info.
*/
typedef struct {
    /// FRU Signature.
    UINTN                       Signature;
    /// Maximum FRU slots.
    UINT8                       MaxFruSlots;
    /// Number of FRU slots present.
    UINT8                       NumSlots;
    /// FRU device info.
    AMI_FRU_DEVICE_INFO         FruDeviceInfo[MAX_FRU_SLOT];
    /// Redir fru protocol.
    EFI_SM_FRU_REDIR_PROTOCOL   IpmiRedirFruProtocol;
} AMI_IPMI_FRU_GLOBAL;

/**
    FRU common header.
*/
typedef struct {
    /// Format version.
    UINT8   FormatVersionNumber:4;
    UINT8   Reserved:4;                 ///< Reserved.
    UINT8   InternalUseStartingOffset;  ///< Starting Offset of internal use area.
    UINT8   ChassisInfoStartingOffset;  ///< Starting Offset of Chassis info area.
    UINT8   BoardAreaStartingOffset;    ///< Starting Offset of Board area.
    UINT8   ProductInfoStartingOffset;  ///< Starting Offset of Product info area.
    UINT8   MultiRecInfoStartingOffset; ///< Starting offset of Multi record info.
    UINT8   Pad;                        ///< Padding.
    UINT8   Checksum;                   ///< Checksum.
} AMI_IPMI_FRU_COMMON_HEADER;

/**
    FRU multi record header.
*/
typedef struct {
    /// Record type id.
    UINT8   RecordTypeId;
    UINT8   RecordFormatVersion:4;  ///< Format version.
    UINT8   Reserved:3;             ///< Reserved.
    UINT8   EndofList:1;            ///< End of FRU multi record.
    UINT8   RecordLength;           ///< Multi record length.
    UINT8   RecordChecksum;         ///< FRU record checksum.
    UINT8   HeaderChecksum;         ///< FRU header checksum.
} AMI_IPMI_FRU_MULTI_RECORD_HEADER;

/**
    System UUID record.
*/
typedef struct {
    /// FRU record checksum.
    UINT8       RecordChecksum;
    UINT8       SubRecordId;    ///< Sub-record id in FRU multi record.
    EFI_GUID    Uuid;           ///< System UUID.
} AMI_IPMI_SYSTEM_UUID_SUB_RECORD_WITH_CHECKSUM;
#pragma pack()

//---------------------------------------------------------------------------

#endif  // #ifndef _AMI_FRU_H_

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
