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

/** @file AmiIpmiNetFnStorageDefinitions.h
    Ipmi NetFn Storage additional command and its structures.

*/

#ifndef _AMI_IPMI_NETFN_STORAGE_DEFINITIONS_H_
#define _AMI_IPMI_NETFN_STORAGE_DEFINITIONS_H_

//---------------------------------------------------------------------------

#include <IndustryStandard/IpmiNetFnStorage.h>

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

#define AMI_IPMI_GET_SEL_ALLOCATION_INFO_COMMAND_SUPPORTED_MASK     BIT0
#define AMI_IPMI_RESERVE_SEL_COMMAND_SUPPORTED_MASK                 BIT1
#define AMI_IPMI_PARTIAL_ADD_SEL_ENTRY_COMMAND_SUPPORTED_MASK       BIT2
#define AMI_IPMI_DELETE_SEL_ENTRY_COMMAND_SUPPORTED_MASK            BIT3
#define AMI_IPMI_OVERFLOW_FLAG_MASK                                 BIT7
#define AMI_IPMI_SEL_OVERFLOW_FLAG_SET                              0x80

#define AMI_IPMI_BIOS_SOFTWARE_ID                                   0x00
#define AMI_IPMI_GENERATOR_ID(SOFTWARE_ID)                          ((SOFTWARE_ID << 1) | (IPMI_SOFTWARE_ID))

#define AMI_IPMI_SEL_OEM_TIME_STAMP_RECORD_START                    0xC0
#define AMI_IPMI_SEL_OEM_TIME_STAMP_RECORD_END                      0xDF
#define AMI_IPMI_SEL_OEM_NO_TIME_STAMP_RECORD_START                 0xE0
#define AMI_IPMI_SEL_OEM_NO_TIME_STAMP_RECORD_END                   0xFF

#define AMI_IPMI_SEL_EVENT_DIR(EventDirType)                        (EventDirType >> 7)
#define AMI_IPMI_SEL_EVENT_DIR_ASSERTION_EVENT                      0x00
#define AMI_IPMI_SEL_EVENT_DIR_DEASSERTION_EVENT                    0x01

#define AMI_IPMI_SEL_EVENT_TYPE(EventDirType)                       (EventDirType & 0x7F)
#define AMI_IPMI_SEL_EVENT_TYPE_UNSPECIFIED                         0x00
#define AMI_IPMI_SEL_EVENT_TYPE_THRESHOLD                           0x01
#define AMI_IPMI_SEL_EVENT_TYPE_GENERIC_START                       0x02
#define AMI_IPMI_SEL_EVENT_TYPE_GENERIC_END                         0x0C
#define AMI_IPMI_SEL_EVENT_TYPE_SENSOR_SPECIFIC                     0x6F
#define AMI_IPMI_SEL_EVENT_TYPE_OEM_START                           0x70
#define AMI_IPMI_SEL_EVENT_TYPE_OEM_END                             0x7F

#define AMI_IPMI_SOFTWARE_ID_FROM_GENERATOR_ID(GeneratorId)         ((GeneratorId & 0xFF) >> 1)
#define AMI_IPMI_SLAVE_ADDRESS_FROM_GENERATOR_ID(GeneratorId)       ((GeneratorId & 0xFF) >> 1)
#define AMI_IPMI_LUN_FROM_GENERATOR_ID(GeneratorId)                 ((GeneratorId >> 8) & 0x03)
#define AMI_IPMI_CHANNEL_NUMBER_FROM_GENERATOR_ID(GeneratorId)      ((GeneratorId >> 12) & 0x0F)

//
// System Software IDs defined in IPMI 2.0 Revision-1 specification.
//
#define AMI_IPMI_BIOS_ID_RANGE_START                                0x00
#define AMI_IPMI_BIOS_ID_RANGE_END                                  0x0F
#define AMI_IPMI_BIOS_SOFTWARE_TYPE_STRING                          L"BIOS"

#define AMI_IPMI_SMI_HANDLER_ID_RANGE_START                         0x10
#define AMI_IPMI_SMI_HANDLER_ID_RANGE_END                           0x1F
#define AMI_IPMI_SMI_HANDLER_SOFTWARE_TYPE_STRING                   L"SMI Handler"

#define AMI_IPMI_SMS_ID_RANGE_START                                 0x20
#define AMI_IPMI_SMS_ID_RANGE_END                                   0x2F
#define AMI_IPMI_SMS_SOFTWARE_TYPE_STRING                           L"System Management Software"

#define AMI_IPMI_OEM_ID_RANGE_START                                 0x30
#define AMI_IPMI_OEM_ID_RANGE_END                                   0x3F
#define AMI_IPMI_OEM_SOFTWARE_TYPE_STRING                           L"OEM"

#define AMI_IPMI_REMOTE_CONSOLE_ID_RANGE_START                      0x40
#define AMI_IPMI_REMOTE_CONSOLE_ID_RANGE_END                        0x46
#define AMI_IPMI_REMOTE_CONSOLE_SOFTWARE_TYPE_STRING                L"Remote Console software"

#define AMI_IPMI_TERMINAL_CONSOLE_ID                                0x47
#define AMI_IPMI_TERMINAL_CONSOLE_SOFTWARE_TYPE_STRING              L"Terminal Mode Remote Console software"

#define AMI_IPMI_RESERVED_SOFTWARE_TYPE_STRING                      L"Reserved"

#define AMI_IPMI_C_CHAR_ASCII_VALUE                                 0x43
#define AMI_IPMI_L_CHAR_ASCII_VALUE                                 0x4C
#define AMI_IPMI_R_CHAR_ASCII_VALUE                                 0x52

#define AMI_IPMI_INITIALIZE_ERASE                                   0xAA
#define AMI_IPMI_GET_ERASE_STATUS                                   0x00

#define AMI_IPMI_ERASURE_IN_PROGRESS                                0x00
#define AMI_IPMI_ERASURE_COMPLETED                                  0x01

#define AMI_IPMI_INVALID_RESERVATION_ID                             0xC5

#define AMI_IPMI_SEL_RECORD_TYPE_INVALID_TO_DELETE                  0x80
#define AMI_IPMI_SEL_ERASE_IN_PROGRESS                              0x81
#define AMI_IPMI_MASK_LOWER_NIBBLE                                  0x0F
#define AMI_IPMI_MASK_UPPER_NIBBLE                                  0xF0

// UTC offset range macros.
#define MAX_UTC_OFFSET                                              1440
#define MIN_UTC_OFFSET                                              0x00

#pragma pack(1)

/**
    SDR record header.
*/
typedef struct {
    /* Commenting out the NextRecordId from header structure.
       Since it is not part of SDR Record Header in IPMI spec.*/
    //  UINT16  NextRecordId;       //1
    /// 1 - Record Id.
    UINT16  RecordId;
    UINT8   Version;        ///< 3 - SDR version.
    UINT8   RecordType;     ///< 4 - Record type.
    UINT8   RecordLength;   ///< 5 - Record length.
} AMI_IPMI_SDR_RECORD_HEADER;

/**
    Get SDR command response with SDR Record Header.
*/
typedef struct {
    /// Completion code.
    UINT8                       CompletionCode;
    UINT16                      NextRecordId;   ///< Next SDR Record Id.
    AMI_IPMI_SDR_RECORD_HEADER  RecordHeader;   ///< SDR record Header.
} AMI_IPMI_GET_SDR_RESPONSE_WITH_RECORD_HDR;

/**
    Sel OEM time stamp record structure.
*/
typedef struct {
    ///< Record ID of SEL event.
    UINT16  RecordId;
    UINT8   RecordType;         ///< Record Type of SEL event.
    UINT32  TimeStamp;          ///< Event time stamp.
    UINT8   ManufacturerId[3];  ///< Manufacturer ID.
    UINT8   OEMDefined[6];      ///< OEM defined data.
} AMI_SEL_OEM_TIMESTAMP_RECORD;

/**
    Get sel entry command response structure.
*/
typedef struct {
    /// Completion code.
    UINT8                       CompletionCode;
    UINT16                      NextSelRecordId;    /// Next SEL Record ID.
    IPMI_SEL_EVENT_RECORD_DATA  RecordData;         ///< SEL Record Data.
} AMI_GET_SEL_ENTRY_RESPONSE;

/**
    Definitions for Get Sel Time UTC offset command
*/
#define AMI_STORAGE_GET_SEL_TIME_UTC_OFFSET         0x5C

/**
    Get sel time UTC offset command response structure.
*/
typedef struct {
    /// Completion code.
    UINT8   CompletionCode;
    INT16   UtcOffset; ///< UTC offset, LS-byte first. (ranges from -1440 to 1440).
} AMI_GET_SEL_TIME_UTC_OFFSET_RESPONSE;

/**
    Definitions for Set Sel Time UTC offset command
*/
#define AMI_STORAGE_SET_SEL_TIME_UTC_OFFSET         0x5D

/**
    Get sel time command response structure.
*/
typedef struct {
    /// Completion code.
    UINT8   CompletionCode;
    UINT32  Timestamp; ///< Present Timestamp clock reading. LS byte first.
} AMI_GET_SEL_TIME_RESPONSE;

/**
    Add sel entry command response structure.
*/
typedef struct {
    /// Completion code.
    UINT8   CompletionCode;
    UINT16  RecordId; ///< Record ID added. LS byte first.
} AMI_ADD_SEL_ENTRY_RESPONSE;

/**
    Delete sel entry command response structure.
*/
typedef struct {
    /// Completion code.
    UINT8   CompletionCode;
    UINT16  RecordId; ///< Record ID deleted. LS byte first.
} AMI_DELETE_SEL_ENTRY_RESPONSE;

/**
    Clear sel command response structure.
*/
typedef struct {
    /// Completion code.
    UINT8   CompletionCode;
    UINT8   ErasureProgress; ///< Erasure progress.
} AMI_CLEAR_SEL_RESPONSE;

/**
    Reserve sel command response structure.
*/
typedef struct {
    /// Completion code.
    UINT8   CompletionCode;
    UINT8   ReservationId[2]; ///< Reservation ID. LS byte first.
} AMI_RESERVE_SEL_RESPONSE;

/**
    Write Fru data command response structure.
*/
typedef struct {
    /// Completion code.
    UINT8   CompletionCode;
    UINT8   Count; ///< Count written --- count is 1 based.
} AMI_WRITE_FRU_DATA_RESPONSE;

/**
    Reserve sdr repository command response structure.
*/
typedef struct {
    /// Completion code.
    UINT8   CompletionCode;
    UINT16  ReservationId; ///< Reservation ID. LS byte first.
} AMI_RESERVE_SDR_REPOSITORY_RESPONSE;

#pragma pack()

//---------------------------------------------------------------------------

#endif  // #ifndef _AMI_IPMI_NETFN_STORAGE_DEFINITIONS_H_

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
