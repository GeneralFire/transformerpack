//***********************************************************************
//*                                                                     *
//*   Copyright (c) 1985-2020, American Megatrends International LLC.   *
//*                                                                     *
//*      All rights reserved. Subject to AMI licensing agreement.       *
//*                                                                     *
//***********************************************************************


/** @file IpmiRedirFru.c
    Ipmi Redir Fru driver.

*/

#include "IpmiRedirFru.h"

AMI_IPMI_FRU_GLOBAL     *gIpmiFruGlobal;
EFI_IPMI_TRANSPORT      *gIpmiTransport = NULL;


/**
    Notification function for Virtual address change event.
    Convert all addresses inside IpmiRedirFru Protocol.

    @param[in] Event    Event which caused this handler.
    @param[in] Context  Context passed during Event Handler registration.

    @return VOID    Nothing.

**/
VOID
EFIAPI
IpmiRedirFruVirtualAddressChangeEvent (
  IN EFI_EVENT  Event,
  IN VOID       *Context )
{
    // Change all addresses inside IpmiRedirFruProtocol stored locally.
    gRT->ConvertPointer (
            0,
            (VOID **) &(gIpmiFruGlobal->IpmiRedirFruProtocol.GetFruRedirInfo) );
    gRT->ConvertPointer (
            0,
            (VOID **) &(gIpmiFruGlobal->IpmiRedirFruProtocol.GetFruSlotInfo) );
    gRT->ConvertPointer (
            0,
            (VOID **) &(gIpmiFruGlobal->IpmiRedirFruProtocol.GetFruRedirData) );
    gRT->ConvertPointer (
            0,
            (VOID **) &(gIpmiFruGlobal->IpmiRedirFruProtocol.SetFruRedirData) );
}

/** @internal
    Returns the empty FRU slot to fill the new entry.

    @param VOID     Nothing.

    @return UINT8   Returns empty FRU slot number if found, else returns 0xFF.

**/
UINT8
GetEmptyFruSlot (
  VOID )
{
    UINT8   Index;

    for (Index = 1; Index < gIpmiFruGlobal->MaxFruSlots; Index++) {
        if (!gIpmiFruGlobal->FruDeviceInfo[Index].Valid) {
            return Index;
        }
    }
    return 0xFF;
}

/**
    This API returns FRU Inventory Area information.

    @param[in]  This                    Pointer to the Redir FRU protocol.
    @param[in]  FruSlotNumber           This is the Slot Number of this FRU
                                        Type. This instance represents Slot
                                        Number in case of Memory, CPU etc.
    @param[out] FruFormatGuid           FRU Format GUID. This GUID is a
                                        format of FRU we are dealing with
                                        and it can also be OEM specific.
    @param[out] DataAccessGranularity   Data Access granularity  in bytes.
    @param[out] FruInformationString    This is a human readable string. 
                                        It has a information that can be
                                        useful to a user while reading
                                        through a setup or terminal.

    @retval EFI_SUCCESS             FRU Data info retrieved  successful.
    @retval EFI_INVALID_PARAMETER   Invalid Parameter.
    @retval EFI_NO_MAPPING          Requested FRU slot number is not exists.

**/
EFI_STATUS
EFIAPI
EfiGetFruRedirInfo (
  IN  EFI_SM_FRU_REDIR_PROTOCOL     *This,
  IN  UINTN                         FruSlotNumber,
  OUT EFI_GUID                      *FruFormatGuid,
  OUT UINTN                         *DataAccessGranularity,
  OUT CHAR16                        **FruInformationString 
  )
{
    AMI_IPMI_FRU_GLOBAL     *FruPrivate;

    if (!FruFormatGuid || !DataAccessGranularity || !FruInformationString) {
        return EFI_INVALID_PARAMETER;
    }

    FruPrivate = AMI_IPMI_INSTANCE_FROM_EFI_SM_IPMI_FRU_THIS (This);
    if ((FruSlotNumber + 1) > FruPrivate->NumSlots) {
        return EFI_NO_MAPPING;
    }

    CopyMem (
        FruFormatGuid,
        &gEfiIpmiFormatFruGuid,
        sizeof (EFI_GUID));

    *DataAccessGranularity = 0x1;
    *FruInformationString = NULL;

    return EFI_SUCCESS;
}

/**
    This API returns the slot information in case of FRU. This slot info
    could represent a slot number in case of Memory, CPU or simple instances
    in other cases.

    @param[in]  This                Pointer to the Redir FRU protocol.
    @param[out] FruTypeGuid         This is the TYPE GUID represented by
                                    this protocol.
    @param[out] StartFruSlotNumber  This is the Starting slot number for
                                    this instance of TYPE GUID.
    @param[out] NumSlots            This is the number of slots for this
                                    instance of  TYPE GUID.

    @retval EFI_SUCCESS             FRU Data info is retrieved successfully.
    @retval EFI_INVALID_PARAMETER   Invalid parameter.

**/
EFI_STATUS
EFIAPI
EfiGetFruSlotInfo (
  IN  EFI_SM_FRU_REDIR_PROTOCOL     *This,
  OUT EFI_GUID                      *FruTypeGuid,
  OUT UINTN                         *StartFruSlotNumber,
  OUT UINTN                         *NumSlots 
  )
{
    AMI_IPMI_FRU_GLOBAL     *FruPrivate;

    if (!FruTypeGuid || !StartFruSlotNumber || !NumSlots) {
        return EFI_INVALID_PARAMETER;
    }

    FruPrivate = AMI_IPMI_INSTANCE_FROM_EFI_SM_IPMI_FRU_THIS (This);

    CopyMem (
        FruTypeGuid,
        &gEfiSystemTypeFruGuid,
        sizeof (EFI_GUID));
    
    *StartFruSlotNumber = 0;
    *NumSlots = FruPrivate->NumSlots;
    return EFI_SUCCESS;
}

/**
    This API returns the specified data from the FRU Inventory Info
    area. This is effectively a low level direct interface to a
    non-volatile storage area.

    @param[in]      This            Pointer to the Redir FRU protocol.
    @param[in]      FruSlotNumber   This is the Slot Number of the this FRU
                                    Type. This instance represents Slot
                                    Number in case of Memory, CPU etc.
    @param[in]      FruDataOffset   This is the Data Offset from where FRU
                                    is to be read.
    @param[in, out] FruDataSize     On input, pointer to the FRU Data size
                                    to be read. On output, pointer to the
                                    FRU data size actually read.
    @param[out]     FruData         Pointer to the buffer, where FRU data
                                    will be returned.

    @retval EFI_SUCCESS             FRU Data info retrieved successful.
    @retval EFI_UNSUPPORTED         Data is not available yet.
    @retval EFI_NO_MAPPING          Requested FRU slot number is not exists.
    @retval EFI_INVALID_PARAMETER   Invalid Parameter.
    @retval EFI_TIMEOUT             FRU device is not ready.
    @retval Others                  Return status from Send Ipmi command.

**/
EFI_STATUS
EFIAPI
EfiGetFruRedirData (
  IN     EFI_SM_FRU_REDIR_PROTOCOL  *This,
  IN     UINTN                      FruSlotNumber,
  IN     UINTN                      FruDataOffset,
  IN OUT UINTN                      *FruDataSize,
  OUT    UINT8                      *FruData 
  )
{
    EFI_STATUS                          Status;
    AMI_IPMI_FRU_GLOBAL                 *FruPrivate;
    IPMI_READ_FRU_DATA_REQUEST          FruReadCommandReq;
    IPMI_READ_FRU_DATA_RESPONSE         *FruReadCommandRes;
    UINT8                               DataBuffer[MAX_REQ_RES_DATA_SIZE];
    UINT8                               ResponseDataSize;
    UINT8                               RetryCounter;
    UINTN                               ReadFruDataSize = 0;

    if (!FruDataSize || !FruData || !(*FruDataSize)) {
        return EFI_INVALID_PARAMETER;
    }

    FruPrivate = AMI_IPMI_INSTANCE_FROM_EFI_SM_IPMI_FRU_THIS (This);

    if (FruSlotNumber >= FruPrivate->NumSlots) {
        DEBUG ((DEBUG_ERROR, "%a Error!!!  No FRU Mapping\n", __FUNCTION__)); 
        *FruDataSize = 0x00;
        return EFI_NO_MAPPING;
    }

    // Create the FRU Read Command for the logical FRU Device.
    if (FruPrivate->FruDeviceInfo[FruSlotNumber].FruDevice.Bits.LogicalFruDevice) {
        FruReadCommandReq.DeviceId = FruPrivate->FruDeviceInfo[FruSlotNumber].FruDevice.Bits.FruDeviceId;
        FruReadCommandReq.InventoryOffset = (UINT16) FruDataOffset;

        // Collect the data till it is completely retrieved.
        while (*FruDataSize > 0) {
            /* BMC allows to get CUSTOM_FRU_DATA_SIZE bytes with in a single command.
               Data more than the CUSTOM_FRU_DATA_SIZE size will be received as part by part of size
               mentioned in CUSTOM_FRU_DATA_SIZE token.*/
            if (*FruDataSize > CUSTOM_FRU_DATA_SIZE) {
                FruReadCommandReq.CountToRead = CUSTOM_FRU_DATA_SIZE;
            } else {
                FruReadCommandReq.CountToRead = (UINT8)*FruDataSize;
            }

            FruReadCommandRes = (IPMI_READ_FRU_DATA_RESPONSE *)DataBuffer;
            RetryCounter = IPMI_FRU_COMMAND_RETRY_COUNTER;
            // Read FRU data.
            do {
                ResponseDataSize = sizeof (DataBuffer);
                Status = gIpmiTransport->SendIpmiCommand (
                                                    gIpmiTransport,
                                                    IPMI_NETFN_STORAGE,
                                                    AMI_BMC_LUN,
                                                    IPMI_STORAGE_READ_FRU_DATA,
                                                    (UINT8 *)&FruReadCommandReq,
                                                    sizeof (FruReadCommandReq),
                                                    (UINT8 *)FruReadCommandRes,
                                                    &ResponseDataSize);

                if (EFI_ERROR(Status) || ((FruReadCommandRes->CompletionCode != FRU_CC_CMD_SUCCESS) && (FruReadCommandRes->CompletionCode != FRU_CC_DEVICE_BUSY))) {
                    DEBUG ((DEBUG_ERROR, "%a Status = %r Completion code = %x \n", __FUNCTION__, Status, FruReadCommandRes->CompletionCode)); 
                    *FruDataSize = 0x00;
                    return Status;
                }

                if (FruReadCommandRes->CompletionCode == FRU_CC_DEVICE_BUSY) {
                    MicroSecondDelay (IPMI_FRU_COMMAND_RETRY_DELAY);
                }

            } while ((RetryCounter--) && (FruReadCommandRes->CompletionCode));

            if (RetryCounter == 0) {
                DEBUG ((DEBUG_ERROR, "%a FRU Device busy!!! RetryCounter = %x \n", __FUNCTION__, RetryCounter)); 
                *FruDataSize = 0x00;
                return EFI_TIMEOUT;
            }

            FruReadCommandRes->CountReturned = FruReadCommandRes->CountReturned <= FruReadCommandReq.CountToRead \
                                                ? FruReadCommandRes->CountReturned \
                                                : FruReadCommandReq.CountToRead;

            // Store remaining count in FruDataSize.
            *FruDataSize = (UINT16) (*FruDataSize - FruReadCommandRes->CountReturned);

            // Copy the partial data.
            CopyMem (FruData + ReadFruDataSize, FruReadCommandRes->Data, FruReadCommandRes->CountReturned);

            // Store the FRU data size read till now.
            ReadFruDataSize += FruReadCommandRes->CountReturned;

            // Next Offset to retrieve.
            FruReadCommandReq.InventoryOffset += FruReadCommandRes->CountReturned;

        } //while (*FruDataSize > 0)

        *FruDataSize = ReadFruDataSize;



         return EFI_SUCCESS;
    }

    *FruDataSize = 0x00;
     DEBUG ((DEBUG_ERROR, "%a Error!!!  No FRU device [%x]\n", __FUNCTION__, FruPrivate->FruDeviceInfo[FruSlotNumber].FruDevice.Bits.LogicalFruDevice));
     return  EFI_UNSUPPORTED;

}

/**
    The command writes the specified bytes to the FRU Inventory Info area.
    This is a low level direct interface to a non-volatile storage area.
    Updating the FRU Inventory Data is presumed to be a system level,
    privileged operation. There is no requirement for devices implementing
    this API to provide mechanisms for rolling back the FRU Inventory Area
    in the case of incomplete or incorrect writes

    @param[in]      This            Pointer to the Redir FRU protocol.
    @param[in]      FruSlotNumber   This is the Slot Number of the this FRU
                                    Type. This instance represents Slot
                                    Number in case of Memory, CPU etc.
    @param[in]      FruDataOffset   This is the Data Offset to where FRU is
                                    to be written.
    @param[in, out] FruDataSize     On input, pointer to the FRU Data size
                                    to be written. On output, pointer to the
                                    FRU data size actually written.
    @param[in]      FruData         Pointer to the buffer, from where FRU
                                    data will be read.

    @retval EFI_SUCCESS           FRU Data written successfully.
    @retval EFI_INVALID_PARAMETER Invalid Parameter.
    @retval EFI_UNSUPPORTED       Data Writes are Un-Supported.
    @retval EFI_NOT_READY         FRU device is not ready.
    @retval EFI_WRITE_PROTECTED   User is not allowed to write to the FRU.
    @retval EFI_NO_MAPPING        Requested FRU slot number is not exists.
    @retval Others                Return status from Send Ipmi command.

**/
EFI_STATUS
EFIAPI
EfiSetFruRedirData (
  IN     EFI_SM_FRU_REDIR_PROTOCOL  *This,
  IN     UINTN                      FruSlotNumber,
  IN     UINTN                      FruDataOffset,
  IN OUT UINTN                      *FruDataSize,
  IN     UINT8                      *FruData 
  )
{
    EFI_STATUS                                      Status;
    AMI_IPMI_FRU_GLOBAL                             *FruPrivate;
    IPMI_WRITE_FRU_DATA_REQUEST                     *FruWriteCommandReq;
    IPMI_WRITE_FRU_DATA_RESPONSE                    FruWriteCommandRes;
    UINT8                                           DataBuffer[MAX_REQ_RES_DATA_SIZE];
    UINT8                                           ReqDataSize;
    UINT8                                           ResponseDataSize;
    UINT8                                           RetryCounter;
    UINTN                                           WriteFruDataSize = 0;

    if (!FruDataSize || !FruData) {
        return EFI_INVALID_PARAMETER;
    }

    FruPrivate = AMI_IPMI_INSTANCE_FROM_EFI_SM_IPMI_FRU_THIS (This);

    if (FruSlotNumber >= FruPrivate->NumSlots) {
        *FruDataSize = 0x00;
        DEBUG ((DEBUG_ERROR, "%a Error!!!  No FRU Mapping\n", __FUNCTION__)); 
        return EFI_NO_MAPPING;
    }

    FruWriteCommandReq = (IPMI_WRITE_FRU_DATA_REQUEST *)DataBuffer;

    // Create the FRU Read Command for the logical FRU Device.
    if (FruPrivate->FruDeviceInfo[FruSlotNumber].FruDevice.Bits.LogicalFruDevice) {
        FruWriteCommandReq->DeviceId = FruPrivate->FruDeviceInfo[FruSlotNumber].FruDevice.Bits.FruDeviceId;

        FruWriteCommandReq->InventoryOffset = (UINT16) FruDataOffset;

        // Write the data till it is completely restored.
        while (*FruDataSize > 0) {
            /* BMC allows to set CUSTOM_FRU_DATA_SIZE bytes with in a single command.
               Data more than the size will be written as part by part of size
               mentioned in CUSTOM_FRU_DATA_SIZE token.*/
            if (*FruDataSize > CUSTOM_FRU_DATA_SIZE) {
                ReqDataSize = CUSTOM_FRU_DATA_SIZE;
            } else {
                ReqDataSize = (UINT8)*FruDataSize;
            }

            CopyMem (FruWriteCommandReq->Data, FruData + WriteFruDataSize, ReqDataSize);
            RetryCounter = IPMI_FRU_COMMAND_RETRY_COUNTER;
            // Write FRU data.
            do {
                ResponseDataSize = sizeof (FruWriteCommandRes);
                Status = gIpmiTransport->SendIpmiCommand (
                                                    gIpmiTransport,
                                                    IPMI_NETFN_STORAGE,
                                                    AMI_BMC_LUN,
                                                    IPMI_STORAGE_WRITE_FRU_DATA,
                                                    (UINT8 *)FruWriteCommandReq,
                                                    (sizeof(IPMI_WRITE_FRU_DATA_REQUEST) + ReqDataSize),
                                                    (UINT8 *)&FruWriteCommandRes,
                                                    &ResponseDataSize );

                if (!EFI_ERROR(Status) && (FruWriteCommandRes.CompletionCode == FRU_CC_WRITE_PROTECTED)) {
                    *FruDataSize = 0x00;
                    DEBUG ((DEBUG_ERROR, "%a FRU write protected. Completion code = %x \n", __FUNCTION__, Status, FruWriteCommandRes.CompletionCode)); 
                    return EFI_WRITE_PROTECTED;
                }

                if (EFI_ERROR(Status) || ((FruWriteCommandRes.CompletionCode != FRU_CC_CMD_SUCCESS) && (FruWriteCommandRes.CompletionCode != FRU_CC_DEVICE_BUSY))) {
                    DEBUG ((DEBUG_ERROR, "%a Status = %r Completion code = %x \n", __FUNCTION__, Status, FruWriteCommandRes.CompletionCode)); 
                    *FruDataSize = 0x00;
                    return Status;
                }

                if (FruWriteCommandRes.CompletionCode == FRU_CC_DEVICE_BUSY) {
                    MicroSecondDelay (IPMI_FRU_COMMAND_RETRY_DELAY);
                }

            } while ((RetryCounter--) && (FruWriteCommandRes.CompletionCode));

            if (RetryCounter == 0) {
                DEBUG ((DEBUG_ERROR, "%a FRU Device busy!!! RetryCounter = %x \n", __FUNCTION__, RetryCounter)); 
                *FruDataSize = 0x00;
                return EFI_TIMEOUT;
            }

            // Store remaining count in FruDataSize.
            *FruDataSize = (UINT8) (*FruDataSize - FruWriteCommandRes.CountWritten);

            // Store the FRU data size written till now.
            WriteFruDataSize += FruWriteCommandRes.CountWritten;

            // Next Offset to set.
            FruWriteCommandReq->InventoryOffset += FruWriteCommandRes.CountWritten;

        } //while (*FruDataSize > 0)

        *FruDataSize = WriteFruDataSize;
        return EFI_SUCCESS;
    }

    *FruDataSize = 0x00;
     DEBUG ((DEBUG_ERROR, "%a Error!!!  No FRU device [%x]\n", __FUNCTION__, FruPrivate->FruDeviceInfo[FruSlotNumber].FruDevice.Bits.LogicalFruDevice));
     return  EFI_UNSUPPORTED;
}

/**
    This function is entry point of this driver and it performs:
    1. Installs Redir FRU protocol.
    2. Read the FRU information from authenticated device and publish FRU strings.
    3. If FRU information is not present then publish dummy FRU strings.

    @param[in] ImageHandle  Handle of this driver image.
    @param[in] SystemTable  Table containing standard EFI services.

    @retval EFI_SUCCESS             Successful driver initialization.
    @retval EFI_OUT_OF_RESOURCES    Failed to allocate requested memory.

**/
EFI_STATUS
EFIAPI
InitializeSmRedirFruLayer (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable 
  )
{
    EFI_STATUS                                  Status;
    EFI_HANDLE                                  NewHandle;
    UINT8                                       Index = 0;
    AMI_RESERVE_SDR_REPOSITORY_RESPONSE         ReserveSdrRepoResponse;
    UINT8                                       ReserveSdrRepoResponseSize = sizeof (ReserveSdrRepoResponse);
    IPMI_GET_SDR_REQUEST                        GetSdrCommandData;
    UINT8                                       GetSdrResponseData[0xFF];
    UINT8                                       GetSdrResponseDataSize = sizeof (GetSdrResponseData);
    IPMI_GET_DEVICE_ID_RESPONSE                 GetDeviceIdResponse;
    EFI_EVENT                                   EfiVirtualAddressChangeEvent = NULL;

    DEBUG ((DEBUG_INFO, "%a Entry... \n", __FUNCTION__));

    // Allocate memory for gIpmiFruGlobal.
    gIpmiFruGlobal = (AMI_IPMI_FRU_GLOBAL *)AllocateRuntimeZeroPool (sizeof (AMI_IPMI_FRU_GLOBAL));
    if (gIpmiFruGlobal == NULL) {
        DEBUG ((DEBUG_ERROR, "Failed to allocate memory for gIpmiFruGlobal! \n"));
        return EFI_OUT_OF_RESOURCES;
    }

    // Initialize allocated memory.
    gIpmiFruGlobal->NumSlots = 0;
    gIpmiFruGlobal->IpmiRedirFruProtocol.GetFruRedirInfo  = EfiGetFruRedirInfo;
    gIpmiFruGlobal->IpmiRedirFruProtocol.GetFruSlotInfo   = EfiGetFruSlotInfo;
    gIpmiFruGlobal->IpmiRedirFruProtocol.GetFruRedirData  = EfiGetFruRedirData;
    gIpmiFruGlobal->IpmiRedirFruProtocol.SetFruRedirData  = EfiSetFruRedirData;
    gIpmiFruGlobal->Signature = EFI_SM_FRU_REDIR_SIGNATURE;
    gIpmiFruGlobal->MaxFruSlots = MAX_FRU_SLOT;

    Status = gBS->LocateProtocol (
                            &gEfiDxeIpmiTransportProtocolGuid,
                            NULL,
                            (VOID **)&gIpmiTransport );

    DEBUG ((DEBUG_INFO, " gEfiDxeIpmiTransportProtocolGuid Status: %r \n", Status));

    if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, " Error in locating IPMI transport protocol so publish Dummy records...\n" ));
        UpdateFruSmbiosTables ();
        FreePool (gIpmiFruGlobal);
        return EFI_SUCCESS;
    }

    // Get the device ID information for the BMC.
    Status = AmiIpmiGetDeviceId (
                            gIpmiTransport,
                            &GetDeviceIdResponse );

    if (EFI_ERROR (Status) || 
        !(GetDeviceIdResponse.DeviceSupport.Bits.FruInventorySupport) ||
        ((*((UINT16*) &GetDeviceIdResponse.ManufacturerId[0]) != IANA_PEN_IPMI_FORUM) && \
         (*((UINT16*) &GetDeviceIdResponse.ManufacturerId[0]) != IANA_PEN_INTEL) && \
         (*((UINT16*) &GetDeviceIdResponse.ManufacturerId[0]) != OEM_MANF_ID))) {

        // If Get device id command is not successful then publish Dummy records.
        DEBUG ((DEBUG_ERROR, " Get Device id command is not successful so publish Dummy records...\n" ));
        UpdateFruSmbiosTables ();
        FreePool (gIpmiFruGlobal);
        return EFI_SUCCESS;
    }

    // Add a mandatory FRU Inventory device attached to the controller.
    gIpmiFruGlobal->FruDeviceInfo[0].Valid = TRUE;
    ZeroMem (
        &gIpmiFruGlobal->FruDeviceInfo[0].FruDevice, 
        sizeof (IPMI_FRU_DATA_INFO) );

    gIpmiFruGlobal->FruDeviceInfo[0].FruDevice.Bits.LogicalFruDevice = TRUE;
    gIpmiFruGlobal->NumSlots++;

    DEBUG ((DEBUG_INFO, "*((UINT16*) &GetDeviceIdResponse.ManufacturerId[0]) %X\n", *((UINT16*) &GetDeviceIdResponse.ManufacturerId[0])));

    /* Now we found out a controller that supports FRU Inventory &
    IPMI Compatible, Start searching for Sensor Information.*/
    if (CUSTOM_FRU_ID_SUPPORT) {
        gIpmiFruGlobal->FruDeviceInfo[0].FruDevice.Bits.FruDeviceId = CUSTOM_FRU_ID;
    } else {
        // Get Reservation ID for Get SDR Info command.
        Status = gIpmiTransport->SendIpmiCommand (
                                            gIpmiTransport,
                                            IPMI_NETFN_STORAGE,
                                            AMI_BMC_LUN,
                                            IPMI_STORAGE_RESERVE_SDR_REPOSITORY,
                                            NULL,
                                            0,
                                            (UINT8 *)&ReserveSdrRepoResponse,
                                            &ReserveSdrRepoResponseSize );

        if (EFI_ERROR (Status) || (ReserveSdrRepoResponse.CompletionCode)) {
            DEBUG ((DEBUG_ERROR, " IPMI_STORAGE_RESERVE_SDR_REPOSITORY Status: %r CompletionCode: %x ReservationId: %x\n", 
                    Status, ReserveSdrRepoResponse.CompletionCode, ReserveSdrRepoResponse.ReservationId));
            UpdateFruSmbiosTables ();
            FreePool (gIpmiFruGlobal);
            return EFI_SUCCESS;
        }

        // Fill Get SDR command Data.
        GetSdrCommandData.ReservationId = ReserveSdrRepoResponse.ReservationId; // Reservation ID.
        GetSdrCommandData.RecordId = 0; // Record ID.

        while (GetSdrCommandData.RecordId != 0xFFFF) {

            GetSdrCommandData.RecordOffset = 0;
            GetSdrCommandData.BytesToRead = sizeof(IPMI_SDR_RECORD_STRUCT_HEADER);
            GetSdrResponseDataSize = sizeof (GetSdrResponseData);

            // Clear the GetSdrResponseData.
            ZeroMem (
                &GetSdrResponseData[0],
                GetSdrResponseDataSize );

            DEBUG ((DEBUG_INFO, " GetSdrCommandData.RecordId: %x\n", GetSdrCommandData.RecordId ));

            // Get SDR Info command to Get FRU data.
            Status = gIpmiTransport->SendIpmiCommand (
                                                gIpmiTransport,
                                                IPMI_NETFN_STORAGE,
                                                AMI_BMC_LUN,
                                                IPMI_STORAGE_GET_SDR,
                                                (UINT8 *)&GetSdrCommandData,
                                                sizeof (GetSdrCommandData),
                                                (UINT8 *)&GetSdrResponseData,
                                                &GetSdrResponseDataSize );

            DEBUG ((DEBUG_INFO, " IPMI_STORAGE_GET_SDR Status: %r CompletionCode: %x RecordType: %x RecordId: %x\n", 
                    Status, ((AMI_IPMI_GET_SDR_RESPONSE_WITH_RECORD_HDR*)GetSdrResponseData)->CompletionCode, 
                    ((AMI_IPMI_GET_SDR_RESPONSE_WITH_RECORD_HDR*)GetSdrResponseData)->RecordHeader.RecordType, 
                    ((AMI_IPMI_GET_SDR_RESPONSE_WITH_RECORD_HDR*)GetSdrResponseData)->RecordHeader.RecordId ));

            if ((EFI_ERROR (Status) || ((AMI_IPMI_GET_SDR_RESPONSE_WITH_RECORD_HDR*)GetSdrResponseData)->CompletionCode)) {
                /* FRU data is not available from the BMC.
                   As such we need to publish Dummy records.*/
                DEBUG ((DEBUG_ERROR, "  FRU data is not available from the BMC so publish Dummy records...\n" ));
                UpdateFruSmbiosTables ();
                FreePool (gIpmiFruGlobal);
                return EFI_SUCCESS;
            }

            // We only care about TYPE 11 Records. Discard other Record types.
            if (((AMI_IPMI_GET_SDR_RESPONSE_WITH_RECORD_HDR*)GetSdrResponseData)->RecordHeader.RecordType == FRU_DEVICE_LOCATOR_RECORD) {
                GetSdrCommandData.RecordOffset = 0; // Record Offset.
                GetSdrCommandData.BytesToRead = ((AMI_IPMI_GET_SDR_RESPONSE_WITH_RECORD_HDR*)GetSdrResponseData)->RecordHeader.RecordLength + sizeof(IPMI_SDR_RECORD_STRUCT_HEADER); // Bytes to read
                GetSdrResponseDataSize = sizeof (GetSdrResponseData);
                // Clear the GetSdrResponseData.
                ZeroMem (
                    &GetSdrResponseData[0],
                    GetSdrResponseDataSize );

                // Get SDR Info command to Get FRU data.
                Status = gIpmiTransport->SendIpmiCommand (
                                                gIpmiTransport,
                                                IPMI_NETFN_STORAGE,
                                                AMI_BMC_LUN,
                                                IPMI_STORAGE_GET_SDR,
                                                (UINT8 *)&GetSdrCommandData,
                                                sizeof (GetSdrCommandData),
                                                (UINT8 *)&GetSdrResponseData,
                                                &GetSdrResponseDataSize );
                DEBUG ((DEBUG_INFO, " IPMI_STORAGE_GET_SDR Status: %r RecordType: %x RecordId: %x\n", Status, 
                       ((AMI_IPMI_GET_SDR_RESPONSE_WITH_RECORD_HDR*)GetSdrResponseData)->RecordHeader.RecordType, 
                       ((AMI_IPMI_GET_SDR_RESPONSE_WITH_RECORD_HDR*)GetSdrResponseData)->RecordHeader.RecordId ));

                if (EFI_ERROR (Status)) {
                     // FRU data of record type 11 with particular record ID not found.
                     DEBUG ((DEBUG_ERROR, "  Error with reading the FRU data of record type 11 with record ID %d, %r .... \n", GetSdrCommandData.RecordId, Status));
                     continue;
                }

                Index = GetEmptyFruSlot ();
                if (Index >= MAX_FRU_SLOT) {
                    DEBUG ((DEBUG_INFO, "Increase the number of FRU Slots!!!\n"));
                    break;
                }
                DEBUG ((DEBUG_INFO, "Type 0x11 Record Found: FruDeviceId:  %x\n",  gIpmiFruGlobal->FruDeviceInfo[Index].FruDevice.Bits.FruDeviceId ));

                gIpmiFruGlobal->FruDeviceInfo[Index].Valid = TRUE;
                gIpmiFruGlobal->NumSlots++;

                // Copy the FRU Data to FruDeviceInfo.
                CopyMem (
                    &gIpmiFruGlobal->FruDeviceInfo[Index].FruDevice,
                    (UINT8*)GetSdrResponseData + sizeof (AMI_IPMI_GET_SDR_RESPONSE_WITH_RECORD_HDR),
                    sizeof (IPMI_FRU_DATA_INFO) );
            }

            // Update the RecordId from the Get SDR command response.
            if (GetSdrCommandData.RecordId == ((AMI_IPMI_GET_SDR_RESPONSE_WITH_RECORD_HDR*)GetSdrResponseData)->NextRecordId) {
                break;
            } else {
                GetSdrCommandData.RecordId = ((AMI_IPMI_GET_SDR_RESPONSE_WITH_RECORD_HDR*)GetSdrResponseData)->NextRecordId; // Record ID
            }

        } // while (GetSdrCommandData.RecordId != 0xFFFF)
    }

    // Install the Ipmi Redir Fru protocol.
    NewHandle = NULL;
    Status = gBS->InstallProtocolInterface (
                                &NewHandle,
                                &gEfiRedirFruProtocolGuid,
                                EFI_NATIVE_INTERFACE,
                                &gIpmiFruGlobal->IpmiRedirFruProtocol );
    
    DEBUG ((DEBUG_INFO, " InstallProtocolInterface gEfiRedirFruProtocolGuid Status: %r \n", Status));

    if (!EFI_ERROR(Status)) {
        // Generate FRU SMBIOS Records.
        GenerateFruSmbiosData ((EFI_SM_FRU_REDIR_PROTOCOL *)(UINTN)&gIpmiFruGlobal->IpmiRedirFruProtocol);

        Status = gBS->CreateEventEx (
                            EVT_NOTIFY_SIGNAL,
                            TPL_NOTIFY,
                            IpmiRedirFruVirtualAddressChangeEvent,
                            NULL,
                            (CONST EFI_GUID *)&gEfiEventVirtualAddressChangeGuid,
                            &EfiVirtualAddressChangeEvent );

        DEBUG ((DEBUG_INFO, "Create event for the Virtual Address Change Event  status %r\n", Status));
    }

    // Update SMBIOS tables with FRU data.
    UpdateFruSmbiosTables();

    if (EFI_ERROR(Status))
        FreePool (gIpmiFruGlobal);

    return EFI_SUCCESS;
}


