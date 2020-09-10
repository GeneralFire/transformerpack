//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2020, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************


/** @file BmcElogCommon.c
    This file contains common functions for Dxe and Smm Elog drivers.

*/

//---------------------------------------------------------------------------

#include "BmcElogCommon.h"

//---------------------------------------------------------------------------

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
    @retval Others                  Status returned from function calls used.

**/
EFI_STATUS
EFIAPI
EfiSetBmcElogData (
  IN  EFI_SM_ELOG_REDIR_PROTOCOL    *This,
  IN  UINT8                         *ElogData,
  IN  EFI_SM_ELOG_REDIR_TYPE        DataType,
  IN  BOOLEAN                       AlertEvent, 
  IN  UINTN                         Size,
  OUT UINT64                        *RecordId )
{
    BMC_ELOG_INSTANCE_DATA                      *BmcElogPrivateData;
    UINT8                                       ResponseDataSize;
    AMI_ADD_SEL_ENTRY_RESPONSE                  ResponseData;
    EFI_STATUS                                  Status;
    IPMI_PLATFORM_EVENT_MESSAGE_DATA_REQUEST    EventMessage;
    UINT8                                       CompletionCode;
    IPMI_SEL_EVENT_RECORD_DATA                  SelRecordData;
    BOOLEAN                                     Delsupport;

    if (!RecordId || !ElogData) {
        return EFI_INVALID_PARAMETER;
    }
    BmcElogPrivateData = INSTANCE_FROM_EFI_SM_ELOG_REDIR_THIS (This);

    if (BmcElogPrivateData->DataType != DataType) {
        return EFI_NOT_FOUND;
    }

    /* Check if SEL is full before logging any entries.
       If SEL is full, then return EFI_OUT_OF_RESOURCES.*/
    if (AmiIpmiIsSelFull (
             gIpmiTransport,
             &Delsupport )) {
        return EFI_OUT_OF_RESOURCES;
    }

    if (USE_PLATFORM_EVENT_MESSAGE) {
        if (Size > sizeof (EventMessage)) {
            DEBUG ((DEBUG_ERROR, " Request data field length limit exceeded. Status: EFI_UNSUPPORTED \n"));
            return EFI_UNSUPPORTED;
        }
        CopyMem (
            (UINT8 *) &EventMessage,
            ElogData,
            sizeof (EventMessage) );
    
        ResponseDataSize = sizeof (CompletionCode);
        Status = gIpmiTransport->SendIpmiCommand (
                                    gIpmiTransport,
                                    IPMI_NETFN_SENSOR_EVENT,
                                    AMI_BMC_LUN,
                                    IPMI_SENSOR_PLATFORM_EVENT_MESSAGE,
                                    (UINT8 *)&EventMessage,
                                    sizeof (EventMessage),
                                    &CompletionCode,
                                    &ResponseDataSize );
        DEBUG ((DEBUG_INFO, " IPMI_SENSOR_PLATFORM_EVENT_MESSAGE Status: %r \n", Status));
    } else {
        if (Size > sizeof (SelRecordData)) {
            DEBUG ((DEBUG_ERROR, " Request data field length limit exceeded. Status: EFI_UNSUPPORTED \n"));
            return EFI_UNSUPPORTED;
        }
        CopyMem (
            (UINT8 *) &SelRecordData,
            ElogData,
            sizeof (SelRecordData) );
    
        ResponseDataSize = sizeof (ResponseData);
        Status = gIpmiTransport->SendIpmiCommand (
                                    gIpmiTransport,
                                    IPMI_NETFN_STORAGE,
                                    AMI_BMC_LUN,
                                    IPMI_STORAGE_ADD_SEL_ENTRY,
                                    (UINT8 *)&SelRecordData,
                                    sizeof (SelRecordData),
                                    (UINT8 *)&ResponseData,
                                    &ResponseDataSize );
        
        DEBUG ((DEBUG_INFO, " IPMI_STORAGE_ADD_SEL_ENTRY Status: %r RecordId: %x \n", Status, ResponseData.RecordId));
        
        if (!EFI_ERROR (Status)) {
            *RecordId = *((UINT16*) (&ResponseData.RecordId));
        }
    }

    return Status;
}

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
    @retval Others                  Status returned from function calls used.

**/
EFI_STATUS
EFIAPI
EfiGetBmcElogData (
  IN     EFI_SM_ELOG_REDIR_PROTOCOL     *This,
  OUT    UINT8                          *ElogData,
  IN     EFI_SM_ELOG_REDIR_TYPE         DataType,
  IN OUT UINTN                          *Size,
  IN OUT UINT64                         *RecordId )
{
    BMC_ELOG_INSTANCE_DATA      *BmcElogPrivateData;
    IPMI_GET_SEL_ENTRY_REQUEST  SelEntry;
    AMI_GET_SEL_ENTRY_RESPONSE  SelResponseData;
    UINT8                       SelResponseSize = sizeof (SelResponseData);
    EFI_STATUS                  Status;

    if (!ElogData || !Size || !RecordId) {
        return EFI_INVALID_PARAMETER;
    }
    BmcElogPrivateData = INSTANCE_FROM_EFI_SM_ELOG_REDIR_THIS (This);

    if (BmcElogPrivateData->DataType != DataType) {
        return EFI_NOT_FOUND;
    }

    if (*Size < sizeof (IPMI_SEL_EVENT_RECORD_DATA)) {
        DEBUG ((DEBUG_ERROR, "Response data field length is small. Status: EFI_BUFFER_TOO_SMALL \n"));
        return EFI_BUFFER_TOO_SMALL;
    }

    // Reserve ID only required for partial get. Use 0000h otherwise.
    SelEntry.ReserveId[0]   = 0x00;
    SelEntry.ReserveId[1]   = 0x00;
    SelEntry.SelRecID[0]    = *((UINT8*)RecordId);
    SelEntry.SelRecID[1]    = *(UINT8*)((UINT8*)RecordId + 1);
    SelEntry.Offset         = 0x00;
    SelEntry.BytesToRead    = IPMI_COMPLETE_SEL_RECORD;

    Status = gIpmiTransport->SendIpmiCommand (
                                gIpmiTransport,
                                IPMI_NETFN_STORAGE,
                                AMI_BMC_LUN,
                                IPMI_STORAGE_GET_SEL_ENTRY,
                                (UINT8 *)&SelEntry,
                                sizeof (SelEntry),
                                (UINT8 *)&SelResponseData,
                                &SelResponseSize );
    
    DEBUG ((DEBUG_INFO, "IPMI_STORAGE_GET_SEL_ENTRY Status: %r Next RecordId: %x \n", Status, SelResponseData.NextSelRecordId ));
    
    if (EFI_ERROR (Status)) {
        return Status;
    }

    // Copy the response data and Record Id.
    *RecordId = SelResponseData.NextSelRecordId;
    *Size = sizeof (SelResponseData.RecordData);
    CopyMem (
        ElogData,
        (UINT8 *) &(SelResponseData.RecordData),
        *Size );
    return EFI_SUCCESS;
}

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
    @retval Others              Status returned from function calls used.

**/
EFI_STATUS
EFIAPI
EfiEraseBmcElogData (
  IN     EFI_SM_ELOG_REDIR_PROTOCOL     *This,
  IN     EFI_SM_ELOG_REDIR_TYPE         DataType,
  IN OUT UINT64                         *RecordId )
{
    BMC_ELOG_INSTANCE_DATA          *BmcElogPrivateData;
    EFI_STATUS                      Status = EFI_SUCCESS;
    IPMI_GET_SEL_INFO_RESPONSE      SelInfo;
    UINT8                           SelInfoSize = sizeof (SelInfo);
    AMI_RESERVE_SEL_RESPONSE        ReserveSelResponse;
    IPMI_CLEAR_SEL_REQUEST          ClearSelData;
    IPMI_DELETE_SEL_ENTRY_REQUEST   DeleteSelRec;
    AMI_CLEAR_SEL_RESPONSE          ClearSelResponse;
    UINT8                           ClearSelResponseDataSize = sizeof (ClearSelResponse);
    AMI_DELETE_SEL_ENTRY_RESPONSE   DeleteSelRecResponse;
    UINT8                           DeleteSelRecResponseSize = sizeof (DeleteSelRecResponse);

    BmcElogPrivateData = INSTANCE_FROM_EFI_SM_ELOG_REDIR_THIS (This);

    if (BmcElogPrivateData->DataType != DataType) {
        return EFI_NOT_FOUND;
    }

    /* Before issuing  SEL reservation ID, Check whether this command is
       supported or not by issuing the GetSelInfoCommand. If it does not
       supported ResvId should be 0000h.*/
    Status = gIpmiTransport->SendIpmiCommand (
                                gIpmiTransport,
                                IPMI_NETFN_STORAGE,
                                AMI_BMC_LUN,
                                IPMI_STORAGE_GET_SEL_INFO,
                                NULL,
                                0,
                                (UINT8 *)&SelInfo,
                                &SelInfoSize );
    
    DEBUG ((DEBUG_INFO, " IPMI_STORAGE_GET_SEL_INFO Status: %r\n", Status));
    
    if (EFI_ERROR (Status)) {
        return Status;
    }

    /* Check Delete Sel Entry command support and return EFI_UNSUPPORTED
       if it is not supported.*/
    if ((RecordId != NULL) && \
        ((SelInfo.OperationSupport & AMI_IPMI_DELETE_SEL_ENTRY_COMMAND_SUPPORTED_MASK) == FALSE)) {
        DEBUG ((DEBUG_ERROR, " Deleting Single record is not supported. Status: EFI_UNSUPPORTED\n"));
        return EFI_UNSUPPORTED;
    }

    /* If SelReserveIdcommand not supported do not issue the
       RESERVE_SEL_ENTRY command, and set the ResvId value to 0000h
       else Get the SEL reservation ID.*/
    if (SelInfo.OperationSupport & AMI_IPMI_RESERVE_SEL_COMMAND_SUPPORTED_MASK) {
        // Get Reservation Id value for accessing SEL area.
        Status = AmiIpmiGetReservationId (
                    gIpmiTransport,
                    &ReserveSelResponse );

        if (EFI_ERROR (Status)) {
            return Status;
        }
    } else {
        ReserveSelResponse.ReservationId[0] = 0x00;
        ReserveSelResponse.ReservationId[1] = 0x00;
    }

    if (RecordId == NULL) {
        // Clear the SEL.
        ClearSelData.Reserve[0] = ReserveSelResponse.ReservationId[0];
        ClearSelData.Reserve[1] = ReserveSelResponse.ReservationId[1];
        ClearSelData.AscC = AMI_IPMI_C_CHAR_ASCII_VALUE;
        ClearSelData.AscL = AMI_IPMI_L_CHAR_ASCII_VALUE;
        ClearSelData.AscR = AMI_IPMI_R_CHAR_ASCII_VALUE;
        ClearSelData.Erase = AMI_IPMI_INITIALIZE_ERASE;

        Status = gIpmiTransport->SendIpmiCommand (
                                    gIpmiTransport,
                                    IPMI_NETFN_STORAGE,
                                    AMI_BMC_LUN,
                                    IPMI_STORAGE_CLEAR_SEL,
                                    (UINT8 *)&ClearSelData,
                                    sizeof (ClearSelData),
                                    (UINT8 *)&ClearSelResponse,
                                    &ClearSelResponseDataSize );

        DEBUG ((DEBUG_INFO, " IPMI_STORAGE_CLEAR_SEL Status: %r ClearSelResponse: %x\n", Status, ClearSelResponse.ErasureProgress));
        Status = AmiIpmiWaitTillErased (
                    gIpmiTransport, 
                    &ReserveSelResponse );
    } else {
        // Delete SEL Record.
        DeleteSelRec.ReserveId[0] = ReserveSelResponse.ReservationId[0];
        DeleteSelRec.ReserveId[1] = ReserveSelResponse.ReservationId[1];
        DeleteSelRec.RecordToDelete[0] = (UINT8)*RecordId;
        DeleteSelRec.RecordToDelete[1] = (UINT8)(((UINT16)*RecordId) / (0x100));

        Status = gIpmiTransport->SendIpmiCommand (
                                    gIpmiTransport,
                                    IPMI_NETFN_STORAGE,
                                    AMI_BMC_LUN,
                                    IPMI_STORAGE_DELETE_SEL_ENTRY,
                                    (UINT8 *)&DeleteSelRec,
                                    sizeof (DeleteSelRec),
                                    (UINT8 *)&DeleteSelRecResponse,
                                    &DeleteSelRecResponseSize );

        DEBUG ((DEBUG_INFO, " IPMI_STORAGE_DELETE_SEL_ENTRY Status: %r Completion code: %x\n", Status, DeleteSelRecResponse.CompletionCode));
        
        if (EFI_ERROR(Status)) {
            return Status;
        } else if (DeleteSelRecResponse.CompletionCode == AMI_IPMI_SEL_RECORD_TYPE_INVALID_TO_DELETE) {
            return EFI_UNSUPPORTED;
        } else if (DeleteSelRecResponse.CompletionCode == AMI_IPMI_SEL_ERASE_IN_PROGRESS) {
            return EFI_NOT_READY;
        }
        DEBUG ((DEBUG_INFO, " Deleted SEL entry Record Id : %x \n", *((UINT16*)(&DeleteSelRecResponse))));
        *RecordId = (UINT64) *((UINT16*)(&DeleteSelRecResponse));
    }

    return Status;
}

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
EFI_STATUS
EFIAPI
EfiActivateBmcElog (
  IN  EFI_SM_ELOG_REDIR_PROTOCOL    *This,
  IN  EFI_SM_ELOG_REDIR_TYPE        DataType,
  IN  BOOLEAN                       *EnableElog,
  OUT BOOLEAN                       *ElogStatus )
{
    BMC_ELOG_INSTANCE_DATA              *BmcElogPrivateData;
    AMI_GET_BMC_GLOBAL_ENABLES_RESPONSE BmcGlobalEnables;
    UINT8                               CompletionCode = 0;
    UINT8                               ResponseDataSize = sizeof (BmcGlobalEnables);
    EFI_STATUS                          Status = EFI_SUCCESS;

    BmcElogPrivateData = INSTANCE_FROM_EFI_SM_ELOG_REDIR_THIS (This);

    if (BmcElogPrivateData->DataType != DataType) {
        return EFI_NOT_FOUND;
    }

    Status = gIpmiTransport->SendIpmiCommand (
                                gIpmiTransport,
                                IPMI_NETFN_APP,
                                AMI_BMC_LUN,
                                IPMI_APP_GET_BMC_GLOBAL_ENABLES,
                                NULL,
                                0,
                                (UINT8 *)&BmcGlobalEnables,
                                &ResponseDataSize );
    
    DEBUG ((DEBUG_INFO, " IPMI_APP_GET_BMC_GLOBAL_ENABLES Status: %r BmcGlobalEnables.SystemEventLogging: %x \n", Status, BmcGlobalEnables.SystemEventLogging ));
    
    if (EFI_ERROR (Status)) {
        return Status;
    }

    /* EnableElog: Enables (TRUE)/Disables(FALSE) Event Log. If NULL, just
       returns the Current ElogStatus.*/
    if (EnableElog == NULL) {
        *ElogStatus = BmcGlobalEnables.SystemEventLogging;
    } else if (BmcGlobalEnables.SystemEventLogging != *EnableElog) {

        BmcGlobalEnables.SystemEventLogging = *EnableElog;
        ResponseDataSize = sizeof (CompletionCode);
        Status = gIpmiTransport->SendIpmiCommand (
                                    gIpmiTransport,
                                    IPMI_NETFN_APP,
                                    AMI_BMC_LUN,
                                    IPMI_APP_SET_BMC_GLOBAL_ENABLES,
                                    (UINT8 *)(&BmcGlobalEnables),
                                    sizeof (BmcGlobalEnables),
                                    &CompletionCode,
                                    &ResponseDataSize );
        
        DEBUG ((DEBUG_INFO, " IPMI_APP_SET_BMC_GLOBAL_ENABLES Status: %r CompletionCode: %x \n", Status, CompletionCode ));
    }
    DEBUG ((DEBUG_INFO, " BmcGlobalEnables.SystemEventLogging: %x \n", BmcGlobalEnables.SystemEventLogging ));
    return Status;
}


