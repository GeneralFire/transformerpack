//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2020, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************


/** @file RedirElogProtocol.h
    Redir Elog Protocol structure, macros and function declarations.

*/

#ifndef _REDIR_ELOG_PROTOCOL_H_
#define _REDIR_ELOG_PROTOCOL_H_

//---------------------------------------------------------------------------

#define EFI_ELOG_REDIR_PROTOCOL_GUID \
    { \
        0x16d11030, 0x71ba, 0x4e5e, { 0xa9, 0xf9, 0xb4, 0x75, 0xa5, 0x49, 0x4, 0x8a } \
    }

// BMC Elog Instance signature.
#define EFI_SM_ELOG_REDIR_SIGNATURE     SIGNATURE_32('E', 'L', 'O', 'F')

typedef struct _EFI_SM_ELOG_REDIR_PROTOCOL EFI_SM_ELOG_REDIR_PROTOCOL;

/**
    Error logging types.
*/
typedef enum {
    EfiElogRedirSmSMBIOS,             ///< EfiElogRedirSmSMBIOS
    EfiElogRedirSmIPMI,               ///< EfiElogRedirSmIPMI
    EfiElogRedirSmMachineCritical,    ///< EfiElogRedirSmMachineCritical
    EfiElogRedirSmASF,                ///< EfiElogRedirSmASF
    EfiElogRedirSmOEM,                ///< EfiElogRedirSmOEM
    EfiSmElogRedirMax                 ///< EfiSmElogRedirMax
} EFI_SM_ELOG_REDIR_TYPE;

/**
    Sends the Event-Log data to the destination. Destination can be a remote
    target like LAN, ICMB, local targets BMC, IPMB or a Firmware-Volume.

    @param[in]  This        Instance of EFI_SM_ELOG_PROTOCOL.
    @param[in]  ElogData    Pointer to the Event-Log data that needs to be
                            recorded.
    @param[in]  DataType    Type of Elog Data that is being recorded.
                            Elog is redirected based upon this information.
    @param[in]  Size        Size of the data to be logged.
    @param[out] RecordId    This is the array of record IDs sent by the
                            target. This can be used to retrieve the records
                            or erase the records.

    @retval EFI_SUCCESS             Event Log was recorded successfully.
    @retval EFI_NOT_FOUND           Event Log target not found.
    @retval EFI_INVALID_PARAMETER   Invalid Parameter.
    @retval EFI_OUT_OF_RESOURCES    Not enough resources to record data.
    @retval Others                  Status returned from send ipmi command.

**/
typedef
EFI_STATUS
(EFIAPI *EFI_SET_ELOG_REDIR_DATA) (
  IN  EFI_SM_ELOG_REDIR_PROTOCOL        *This,
  IN  UINT8                             *ElogData,
  IN  EFI_SM_ELOG_REDIR_TYPE            DataType,
  IN  BOOLEAN                           AlertEvent,
  IN  UINTN                             DataSize,
  OUT UINT64                            *RecordId
);

/**
    Gets the Event-Log data from the destination. Destination can be a
    remote target like LAN, ICMB, local targets BMC, IPMB or a
    Firmware-Volume.

    @param[in]      This        Instance of EFI_SM_ELOG_PROTOCOL.
    @param[out]     ElogData    Pointer to the Event-Log data that needs to
                                be retrieved.
    @param[in]      DataType    Type of Elog Data. Elog is redirected based
                                upon this information.
    @param[in, out] Size        Size of the data to be retrieved. This size
                                should be equal to or more than the size of
                                data to be retrieved. On return, it contains
                                the actual data size retrieved.
    @param[in, out] RecordId    This is record id to retrieve the specific
                                record.

    @retval EFI_SUCCESS             Event Log was retrieved successfully.
    @retval EFI_NOT_FOUND           Event Log target not found.
    @retval EFI_INVALID_PARAMETER   Invalid Parameter.
    @retval EFI_OUT_OF_RESOURCES    Not enough resources to record data.
    @retval EFI_BUFFER_TOO_SMALL    Target buffer is too small to retrieve
                                    the data.
    @retval Others                  Status returned from send ipmi command.

**/
typedef
EFI_STATUS
(EFIAPI *EFI_GET_ELOG_REDIR_DATA) (
  IN     EFI_SM_ELOG_REDIR_PROTOCOL    *This,
  OUT    UINT8                         *ElogData,
  IN     EFI_SM_ELOG_REDIR_TYPE        DataType,
  IN OUT UINTN                         *DataSize,
  IN OUT UINT64                        *RecordId
);

/**
    Erases the Event-Log data from the destination. Destination can be a
    remote target like LAN, ICMB, local targets BMC, IPMB or a
    Firmware-Volume.

    @param[in]      This        Instance of EFI_SM_ELOG_PROTOCOL.
    @param[in]      DataType    Type of Elog Data that is being Erased. Elog
                                is redirected based upon this information.
    @param[in, out] RecordId    This is the RecordId of the data to be
                                erased. If RecordId is NULL, all the records
                                on the database are erased if permitted by
                                the target. Contains the deleted RecordId on
                                return. Record Id value is assumed as Hexa
                                Decimal with 2 bytes of data.

    @retval EFI_SUCCESS         Event Log was Erased successfully.
    @retval EFI_NOT_FOUND       Event Log target not found.
    @retval EFI_UNSUPPORTED     Delete sel entry command is not supported.
    @retval EFI_NOT_READY       Erasing event log is in progress, so cannot
                                erase.
    @retval Others              Status returned from send ipmi command.

**/
typedef
EFI_STATUS
(EFIAPI *EFI_ERASE_ELOG_REDIR_DATA) (
  IN     EFI_SM_ELOG_REDIR_PROTOCOL     *This,
  IN     EFI_SM_ELOG_REDIR_TYPE         DataType,
  IN OUT UINT64                         *RecordId
);

/**
    This API enables/disables Event Log.

    @param[in]      This        Instance of EFI_SM_ELOG_PROTOCOL.
    @param[in]      DataType    Type of Elog Data that is being Activated.
                                Activation (Enable/Disable) is redirected
                                based upon this information.
    @param[in]      EnableElog  Enables (TRUE)/Disables(FALSE) Event Log.
                                If NULL, just return the Current ElogStatus.
    @param[out]     ElogStatus  Current (New) Status of Event Log.
                                Enabled (TRUE) or Disabled (FALSE).

    @retval EFI_SUCCESS     Event Log is enabled/disabled successfully.
    @retval EFI_NOT_FOUND   Event Log target not found.
    @retval Others          Status returned from send ipmi command.

**/
typedef
EFI_STATUS
(EFIAPI *EFI_ACTIVATE_ELOG_REDIR) (
  IN  EFI_SM_ELOG_REDIR_PROTOCOL    *This,
  IN  EFI_SM_ELOG_REDIR_TYPE        DataType,
  IN  BOOLEAN                       *EnableElog,
  OUT BOOLEAN                       *ElogStatus
);

/**
    Error logging protocol.
*/
struct _EFI_SM_ELOG_REDIR_PROTOCOL {
    /// Set event log data.
    EFI_SET_ELOG_REDIR_DATA     SetEventLogData;
    /// Get event log data.
    EFI_GET_ELOG_REDIR_DATA     GetEventLogData;
    /// Erase event log data.
    EFI_ERASE_ELOG_REDIR_DATA   EraseEventLogData;
    /// Enable/disable event logging.
    EFI_ACTIVATE_ELOG_REDIR     ActivateEventLog;
};

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

extern EFI_GUID gEfiRedirElogProtocolGuid;

//---------------------------------------------------------------------------

#endif  // #ifndef _REDIR_ELOG_PROTOCOL_H_


