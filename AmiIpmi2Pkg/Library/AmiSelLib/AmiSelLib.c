//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2020, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************


/** @file AmiSelLib.c
    This file contains library functions related to SEL status codes.

*/

//---------------------------------------------------------------------------

#include <Token.h>
#include <Uefi/UefiBaseType.h>
#include <IndustryStandard/IpmiNetFnSensorEvent.h>
#include <Include/AmiIpmiNetFnStorageDefinitions.h>
#include <Library/PcdLib.h>
#include <Library/DebugLib.h>
#include <Library/AmiSelLib.h>

/* OEM Status Code to SEL Conversion table. Entries should be added to this
   table using SDL tokens by OEM to convert to SEL entry.*/
#include <AmiOemStatusCodeTable.h>

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

#define AMI_IPMI_CLEAR_SEL_RETRY_COUNT  0x200

//---------------------------------------------------------------------------

/** @internal
    This function walks through the table based upon Value field of
    Status Code and extract EFI status code to SEL conversion entry.

    @param[in] EfiStatusCode    EFI status code value.
    @param[in] StatusCodeType   EFI status code type.

    @return AMI_STATUS_CODE_TO_SEL_ENTRY*   Pointer to conversion entry if
                                            found. Else return NULL.

**/
AMI_STATUS_CODE_TO_SEL_ENTRY*
AmiIpmiConvertEfiStatusCodeToSelEntry (
  IN EFI_STATUS_CODE_VALUE  EfiStatusCode,
  IN EFI_STATUS_CODE_TYPE   StatusCodeType )
{
    UINTN   TableSize;
    UINTN   Index;

    if (OEM_SEL_SUPPORT) {
        // OEM to Sel Status code list.
        TableSize = (sizeof (gOemStatusCodeToSelTable) / sizeof (AMI_STATUS_CODE_TO_SEL_ENTRY));
    
        for (Index = 0; Index < TableSize - 1; Index++) {
            if (EfiStatusCode == gOemStatusCodeToSelTable[Index].StatusCode && \
                StatusCodeType == gOemStatusCodeToSelTable[Index].StatusCodeType) {
                return &gOemStatusCodeToSelTable[Index];
            }
        }
    }

    // EFI to Sel Status code list.
    TableSize = (sizeof (gAmiStatusCodeToSelTable) / sizeof (AMI_STATUS_CODE_TO_SEL_ENTRY));

    for (Index = 0; Index < TableSize; Index++) {
        if (EfiStatusCode == gAmiStatusCodeToSelTable[Index].StatusCode && \
            StatusCodeType == gAmiStatusCodeToSelTable[Index].StatusCodeType) {
            return &gAmiStatusCodeToSelTable[Index];
        }
    }

    return NULL;
}

/** @internal
    Get Reservation Id to access Sel area. 

    @param[in] IpmiTransport        Pointer to Ipmi transport instance.
    @param[out] ReserveSelResponse  Contains Sel reservation id.

    @retval EFI_INVALID_PARAMETER   Invalid parameter.
    @retval Others                  Return status of function call used.

**/
EFI_STATUS
AmiIpmiGetReservationId (
  IN  AMI_IPMI_TRANSPORT_INSTANCE   *IpmiTransport,
  OUT AMI_RESERVE_SEL_RESPONSE      *ReserveSelResponse )
{
    EFI_STATUS  Status;
    UINT8       ReserveSelResponseSize;

    if (!IpmiTransport || !ReserveSelResponse) {
        return EFI_INVALID_PARAMETER;
    }
    
    ReserveSelResponseSize = sizeof (*ReserveSelResponse);
    Status = IpmiTransport->SendIpmiCommand (
                              IpmiTransport,
                              IPMI_NETFN_STORAGE,
                              AMI_BMC_LUN,
                              IPMI_STORAGE_RESERVE_SEL,
                              NULL,
                              0,
                              (UINT8 *)ReserveSelResponse,
                              &ReserveSelResponseSize );
    DEBUG ((DEBUG_INFO, "DXE: IPMI_STORAGE_RESERVE_SEL Status: %r ReserveId: %4x \n", Status, *((UINT16*)ReserveSelResponse->ReservationId)));
    return Status;
}

/** @internal
    This function checks for the erasure status.

    @param[in] IpmiTransport    Pointer to Ipmi transport instance.
    @param[in, out] ReserveSelResponse  Contains Sel reservation id.

    @retval EFI_SUCCESS             Sel is erased successfully.
    @retval EFI_NO_RESPONSE         If time out occurred and still Sel is not erased
                                    successfully.
    @retval EFI_INVALID_PARAMETER   Invalid parameter.
    @retval Others                  Error status returned from send ipmi command.

**/
EFI_STATUS
AmiIpmiWaitTillErased (
  IN     AMI_IPMI_TRANSPORT_INSTANCE    *IpmiTransport,
  IN OUT AMI_RESERVE_SEL_RESPONSE       *ReserveSelResponse )
{
    EFI_STATUS                Status;
    UINT16                    Counter;
    IPMI_CLEAR_SEL_REQUEST    ClearSelData;
    AMI_CLEAR_SEL_RESPONSE    ClearSelResponse;
    UINT8                     ClearSelResponseDataSize;

    if (!IpmiTransport || !ReserveSelResponse) {
        return EFI_INVALID_PARAMETER;
    }
    
    for (Counter = 0; Counter < AMI_IPMI_CLEAR_SEL_RETRY_COUNT; Counter++) {
        ClearSelData.Reserve[0]     = ReserveSelResponse->ReservationId[0];
        ClearSelData.Reserve[1]     = ReserveSelResponse->ReservationId[1];
        ClearSelData.AscC           = AMI_IPMI_C_CHAR_ASCII_VALUE;
        ClearSelData.AscL           = AMI_IPMI_L_CHAR_ASCII_VALUE;
        ClearSelData.AscR           = AMI_IPMI_R_CHAR_ASCII_VALUE;
        ClearSelData.Erase          = AMI_IPMI_GET_ERASE_STATUS;
        ClearSelResponseDataSize    = sizeof (ClearSelResponse);

        Status = IpmiTransport->SendIpmiCommand (
                                    IpmiTransport,
                                    IPMI_NETFN_STORAGE,
                                    AMI_BMC_LUN,
                                    IPMI_STORAGE_CLEAR_SEL,
                                    (UINT8 *)&ClearSelData,
                                    sizeof (ClearSelData),
                                    (UINT8 *)&ClearSelResponse,
                                    &ClearSelResponseDataSize );
        
        DEBUG ((DEBUG_INFO, " IPMI_STORAGE_CLEAR_SEL Status: %r ClearSelResponse: %x Counter: %x\n", Status, ClearSelResponse.ErasureProgress, Counter));
        
        if ((!EFI_ERROR(Status)) && \
            ((ClearSelResponse.ErasureProgress & 0x0F) == AMI_IPMI_ERASURE_COMPLETED)) {
            return EFI_SUCCESS;
        } else if ((EFI_ERROR(Status)) && \
            (ClearSelResponse.CompletionCode == AMI_IPMI_INVALID_RESERVATION_ID)) {
            // Get new Reservation id and process further.
            Status = AmiIpmiGetReservationId (
                        IpmiTransport,
                        ReserveSelResponse );
            if (EFI_ERROR(Status)) {
                return Status;
            }
        }
    }
    // If there is no response from the BMC controller, then return.
    return EFI_NO_RESPONSE;
}

/** @internal
    This function verifies if the BMC SEL is full and provide delete SEL support information.

    @param IpmiTransport    Pointer to Ipmi transport instance.
    @param Delsupport       Delete support.

    @retval TRUE    If Sel is full.
    @retval FALSE   If Sel is not full or Invalid parameter.

**/ 
BOOLEAN
AmiIpmiIsSelFull (
  IN AMI_IPMI_TRANSPORT_INSTANCE    *IpmiTransport,
  IN OUT BOOLEAN                    *Delsupport )
{
    EFI_STATUS                    Status;
    IPMI_GET_SEL_INFO_RESPONSE    SelInfo;
    UINT8                         SelInfoSize = sizeof (SelInfo);

    if (IpmiTransport) {
        Status = IpmiTransport->SendIpmiCommand (
                                    IpmiTransport,
                                    IPMI_NETFN_STORAGE,
                                    AMI_BMC_LUN,
                                    IPMI_STORAGE_GET_SEL_INFO,
                                    NULL,
                                    0,
                                    (UINT8 *)&SelInfo,
                                    &SelInfoSize );

        DEBUG ((DEBUG_INFO, " IPMI_STORAGE_GET_SEL_INFO Status: %r (SelInfo.OperationSupport & AMI_IPMI_OVERFLOW_FLAG_MASK): %x\n", Status, (SelInfo.OperationSupport & AMI_IPMI_OVERFLOW_FLAG_MASK)));

        if (!EFI_ERROR (Status)) {

            if (Delsupport != NULL) {
                *Delsupport = (SelInfo.OperationSupport & AMI_IPMI_DELETE_SEL_ENTRY_COMMAND_SUPPORTED_MASK);
            }

            // If overflow flag is set, then report Sel is full.
            if ((SelInfo.OperationSupport & AMI_IPMI_OVERFLOW_FLAG_MASK) == \
                AMI_IPMI_SEL_OVERFLOW_FLAG_SET) {
                return TRUE;
            }
        } 
    }
    return FALSE;
}
 
/** @internal
    This function erases/Delete the SEL entries.

    @param[in] IpmiTransport    Pointer to Ipmi transport instance.
    @param     Eraseoption      Erase(1) or delete(2) sel entry

    @retval EFI_SUCCESS             SEL is erased successfully.
    @retval EFI_INVALID_PARAMETER   Invalid parameter.
    @retval EFI_NO_RESPONSE         If time out occurred or still SEL is not erased
                                    successfully.

**/
EFI_STATUS
AmiIpmiEraseSelElogData (
  IN AMI_IPMI_TRANSPORT_INSTANCE    *IpmiTransport,
  IN UINT8                          Eraseoption )
{
    EFI_STATUS                       Status;
    IPMI_GET_SEL_INFO_RESPONSE       SelInfo;
    UINT8                            SelInfoSize;
    AMI_RESERVE_SEL_RESPONSE         ReserveSelResponse = {0};
    IPMI_CLEAR_SEL_REQUEST           ClearSelData;
    IPMI_DELETE_SEL_ENTRY_REQUEST    DeleteSelRec;
    AMI_CLEAR_SEL_RESPONSE           ClearSelResponse;
    UINT8                            ClearSelResponseDataSize;
    AMI_DELETE_SEL_ENTRY_RESPONSE    DeleteSelRecResponse;
    UINT8                            DeleteSelRecResponseSize = sizeof (DeleteSelRecResponse);

    if (!IpmiTransport) {
        return EFI_INVALID_PARAMETER;
    }
    
    SelInfoSize = sizeof (SelInfo);
    if ((Eraseoption != AMI_IPMI_ERASE_WHEN_SEL_FULL) && (Eraseoption != AMI_IPMI_DEL_WHEN_SEL_FULL))
        return EFI_INVALID_PARAMETER;
    /* Before issuing Sel reservation id, check whether this command is
       supported or not by issuing the GetSelInfo Command. If it does not
       supported, ReserveId should be 0000h.*/
    Status = IpmiTransport->SendIpmiCommand (
                                IpmiTransport,
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

    /* If SelReserveId command is not supported, do not issue the RESERVE_SEL
       command and set the ReserveId value to 0000h else Get the Sel
       reservation id.*/
    if (SelInfo.OperationSupport & AMI_IPMI_RESERVE_SEL_COMMAND_SUPPORTED_MASK) {
        // Get Reservation Id value for accessing Sel area.
        Status = AmiIpmiGetReservationId (
                    IpmiTransport,
                    &ReserveSelResponse );
        if (EFI_ERROR (Status)) {
            return Status;
        }
    } else {
        ReserveSelResponse.ReservationId[0] = 0x00;
        ReserveSelResponse.ReservationId[1] = 0x00;
    }

    // Clear the Sel.
    if (Eraseoption == AMI_IPMI_ERASE_WHEN_SEL_FULL) {
        ClearSelData.Reserve[0]     = ReserveSelResponse.ReservationId[0];
        ClearSelData.Reserve[1]     = ReserveSelResponse.ReservationId[1];
        ClearSelData.AscC           = AMI_IPMI_C_CHAR_ASCII_VALUE;
        ClearSelData.AscL           = AMI_IPMI_L_CHAR_ASCII_VALUE;
        ClearSelData.AscR           = AMI_IPMI_R_CHAR_ASCII_VALUE;
        ClearSelData.Erase          = AMI_IPMI_INITIALIZE_ERASE;
        ClearSelResponseDataSize    = sizeof (ClearSelResponse);
        Status = IpmiTransport->SendIpmiCommand (
                                    IpmiTransport,
                                    IPMI_NETFN_STORAGE,
                                    AMI_BMC_LUN,
                                    IPMI_STORAGE_CLEAR_SEL,
                                    (UINT8 *)&ClearSelData,
                                    sizeof (ClearSelData),
                                    (UINT8 *)&ClearSelResponse,
                                    &ClearSelResponseDataSize );
    
        DEBUG ((DEBUG_INFO, " IPMI_STORAGE_CLEAR_SEL Status: %r ClearSelResponse: %x\n", Status, ClearSelResponse.ErasureProgress));
    
        if ((!EFI_ERROR(Status)) && \
            ((ClearSelResponse.ErasureProgress&0x0F) == AMI_IPMI_ERASURE_COMPLETED)) {
            DEBUG ((DEBUG_INFO, "DXE: SEL is cleared\n"));
            return EFI_SUCCESS;
        }
        Status =  AmiIpmiWaitTillErased (
                      IpmiTransport,
                      &ReserveSelResponse );
        DEBUG ((DEBUG_INFO, "DXE: AmiIpmiWaitTillErased Status : %r\n",Status));
    } else {
        if ((SelInfo.OperationSupport & AMI_IPMI_DELETE_SEL_ENTRY_COMMAND_SUPPORTED_MASK) == FALSE) {
             DEBUG ((DEBUG_ERROR, " Deleting Single record is not supported. Status: EFI_UNSUPPORTED\n"));
             return EFI_UNSUPPORTED;
        }
        DeleteSelRec.ReserveId[0] = ReserveSelResponse.ReservationId[0];
        DeleteSelRec.ReserveId[1] = ReserveSelResponse.ReservationId[1];
        DeleteSelRec.RecordToDelete[0] = 0x00 ;
        DeleteSelRec.RecordToDelete[1] = 0x00;
        Status = IpmiTransport->SendIpmiCommand (
                                    IpmiTransport,
                                    IPMI_NETFN_STORAGE,
                                    AMI_BMC_LUN,
                                    IPMI_STORAGE_DELETE_SEL_ENTRY,
                                    (UINT8 *) &DeleteSelRec,
                                    sizeof (DeleteSelRec),
                                    (UINT8 *) &DeleteSelRecResponse,
                                    (UINT8 *) &DeleteSelRecResponseSize );
       
       
       DEBUG ((DEBUG_INFO, " IPMI_STORAGE_DELETE_SEL_ENTRY Status: %r Completion code: %x\n", Status,DeleteSelRecResponse.CompletionCode ));
       if (EFI_ERROR(Status)) {
           return Status;
       }
       if (DeleteSelRecResponse.CompletionCode == AMI_IPMI_SEL_RECORD_TYPE_INVALID_TO_DELETE) {
           return EFI_UNSUPPORTED;
       }
       if (DeleteSelRecResponse.CompletionCode == AMI_IPMI_SEL_ERASE_IN_PROGRESS) {
           return EFI_NOT_READY;
       }
       DEBUG ((DEBUG_INFO, " Deleted SEL entry Record Id : %x \n",DeleteSelRecResponse.RecordId));
    }
    return Status;
}

/** @internal
    This function fills the event format and logs the event if the status
    code is found in table.

    @param[in]  IpmiTransport   Pointer to Ipmi transport instance.
    @param[in]  Value           EFI status code Value.
    @param[in]  CodeType        EFI status code type.
    @param[out] EventData       Pointer to event data.

    @retval EFI_INVALID_PARAMETER   Invalid parameter.
    @retval EFI_DEVICE_ERROR        If status code is not found in the
                                    conversion table.
    @retval Others                  Status returned by function calls used.

**/
EFI_STATUS
AmiIpmiFillAndLogEventRecord (
  IN  AMI_IPMI_TRANSPORT_INSTANCE   *IpmiTransport,
  IN  EFI_STATUS_CODE_VALUE         Value,
  IN  EFI_STATUS_CODE_TYPE          CodeType,
  OUT VOID                          *EventData )
{
    AMI_STATUS_CODE_TO_SEL_ENTRY                *SelEntry;
    AMI_ADD_SEL_ENTRY_RESPONSE                  AddSelEntryResponse = {0};
    IPMI_PLATFORM_EVENT_MESSAGE_DATA_REQUEST    *EventMessage;
    IPMI_SEL_EVENT_RECORD_DATA                  *SelRecordData;
    UINT8                                       ResponseDataSize;
    UINT8                                       CompletionCode;
    EFI_STATUS                                  Status;
    BOOLEAN                                     Delsupport;
    
    if (!IpmiTransport || !EventData) {
        return EFI_INVALID_PARAMETER;
    }
    
    SelEntry = AmiIpmiConvertEfiStatusCodeToSelEntry (
                    Value,
                    CodeType );

    if (SelEntry != NULL) {
        /* Check if SEL is full before logging any entries. If Sel is full and user opts to erase
           then erase it, report error and log, else return without logging.*/
        if (AmiIpmiIsSelFull (
                IpmiTransport, 
                &Delsupport ) == TRUE) {
            if (PcdGet8 (AmiPcdSelFullBehaviour) == AMI_IPMI_ERASE_WHEN_SEL_FULL) {
                AmiIpmiEraseSelElogData (
                    IpmiTransport,
                    AMI_IPMI_ERASE_WHEN_SEL_FULL );
            } else if (Delsupport && (PcdGet8(AmiPcdSelFullBehaviour) == AMI_IPMI_DEL_WHEN_SEL_FULL)) {
                AmiIpmiEraseSelElogData (
                    IpmiTransport, 
                    AMI_IPMI_DEL_WHEN_SEL_FULL );
            } else {
                return EFI_DEVICE_ERROR;
            }
        }
        if (USE_PLATFORM_EVENT_MESSAGE) {
            EventMessage                = (IPMI_PLATFORM_EVENT_MESSAGE_DATA_REQUEST *)EventData;
            EventMessage->GeneratorId   = AMI_IPMI_GENERATOR_ID (AMI_IPMI_BIOS_SOFTWARE_ID);
            EventMessage->EvMRevision   = IPMI_EVM_REVISION;
            EventMessage->SensorType    = SelEntry->SensorTypeCode;
            EventMessage->SensorNumber  = AMI_IPMI_DEFAULT_SENSOR_NUMBER;
            EventMessage->EventDirType  = IPMI_SENSOR_TYPE_EVENT_CODE_DISCRETE;
            EventMessage->OEMEvData1    = SelEntry->EventData1;
            EventMessage->OEMEvData2    = SelEntry->EventData2;
            EventMessage->OEMEvData3    = SelEntry->EventData3;
            ResponseDataSize            = sizeof (CompletionCode);
            
            Status = IpmiTransport->SendIpmiCommand (
                                        IpmiTransport,
                                        IPMI_NETFN_SENSOR_EVENT,
                                        AMI_BMC_LUN,
                                        IPMI_SENSOR_PLATFORM_EVENT_MESSAGE,
                                        (UINT8 *)EventMessage,
                                        sizeof (IPMI_PLATFORM_EVENT_MESSAGE_DATA_REQUEST),
                                        &CompletionCode,
                                        &ResponseDataSize );
        } else {
            SelRecordData               = (IPMI_SEL_EVENT_RECORD_DATA *)EventData;
            SelRecordData->RecordId     = 0x00;
            SelRecordData->RecordType   = IPMI_SEL_SYSTEM_RECORD;   // Discrete.
            SelRecordData->TimeStamp    = 0x00;
            SelRecordData->GeneratorId  = (UINT16) AMI_IPMI_GENERATOR_ID (AMI_IPMI_BIOS_SOFTWARE_ID);
            SelRecordData->EvMRevision  = IPMI_EVM_REVISION;
            SelRecordData->SensorType   = SelEntry->SensorTypeCode;
            SelRecordData->SensorNumber = AMI_IPMI_DEFAULT_SENSOR_NUMBER;
            SelRecordData->EventDirType = IPMI_SENSOR_TYPE_EVENT_CODE_DISCRETE;
            SelRecordData->OEMEvData1   = SelEntry->EventData1;
            SelRecordData->OEMEvData2   = SelEntry->EventData2;
            SelRecordData->OEMEvData3   = SelEntry->EventData3;
            ResponseDataSize            = sizeof (AddSelEntryResponse);
            
            Status = IpmiTransport->SendIpmiCommand (
                                        IpmiTransport,
                                        IPMI_NETFN_STORAGE,
                                        AMI_BMC_LUN,
                                        IPMI_STORAGE_ADD_SEL_ENTRY,
                                        (UINT8 *)SelRecordData,
                                        sizeof (IPMI_SEL_EVENT_RECORD_DATA),
                                        (UINT8 *)&AddSelEntryResponse,
                                        &ResponseDataSize );
        }
        DEBUG ((DEBUG_INFO, " gIpmiTransport-> SendIpmiCommand Status: %r \n", Status));
        return Status;
    }
    return EFI_DEVICE_ERROR;
}


