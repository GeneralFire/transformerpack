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

/** @file RedirFruProtocol.h
    Redir Fru Protocol structure, macros and function declarations.

*/

#ifndef _REDIR_FRU_PROTOCOL_H_
#define _REDIR_FRU_PROTOCOL_H_

//---------------------------------------------------------------------------

#define EFI_SM_FRU_REDIR_PROTOCOL_GUID \
    { \
        0x28638cfa, 0xea88, 0x456c, { 0x92, 0xa5, 0xf2, 0x49, 0xca, 0x48, 0x85, 0x35 } \
    }

#define EFI_SYSTEM_TYPE_FRU_GUID \
    { \
        0xaab16018, 0x679d, 0x4461, { 0xba, 0x20, 0xe7, 0xc, 0xf7, 0x86, 0x6a, 0x9b } \
    }

#define EFI_IPMI_FORMAT_FRU_GUID \
    { \
        0x3531fdc6, 0xeae, 0x4cd2, { 0xb0, 0xa6, 0x5f, 0x48, 0xa0, 0xdf, 0xe3, 0x8 } \
    }

#define EFI_SM_FRU_REDIR_SIGNATURE  SIGNATURE_32 ('F', 'R', 'R', 'X')

typedef struct _EFI_SM_FRU_REDIR_PROTOCOL EFI_SM_FRU_REDIR_PROTOCOL;

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
    @param[out] FruInformationString    This is a human readable string that
                                        can has information that can be
                                        useful to a user while reading
                                        through a setup or terminal.

    @retval EFI_SUCCESS             FRU Data info retrieved  successful.
    @retval EFI_INVALID_PARAMETER   Invalid Parameter.
    @retval EFI_NO_MAPPING          Requested FRU slot number is not exists.

**/
typedef
EFI_STATUS
(EFIAPI *EFI_GET_FRU_REDIR_INFO) (
  IN  EFI_SM_FRU_REDIR_PROTOCOL     *This,
  IN  UINTN                         FruSlotNumber,
  OUT EFI_GUID                      *FruFormatGuid,
  OUT UINTN                         *DataAccessGranularity,
  OUT CHAR16                        **FruInformationString
);

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
typedef
EFI_STATUS
(EFIAPI *EFI_GET_FRU_SLOT_INFO) (
  IN  EFI_SM_FRU_REDIR_PROTOCOL           *This,
  OUT EFI_GUID                            *FruTypeGuid,
  OUT UINTN                               *StartFruSlotNumber,
  OUT UINTN                               *NumSlots
);

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
    @retval EFI_NOT_READY           FRU device is not ready.
    @retval Others                  Return status from Send Ipmi command.

**/
typedef
EFI_STATUS
(EFIAPI *EFI_GET_FRU_REDIR_DATA) (
  IN     EFI_SM_FRU_REDIR_PROTOCOL  *This,
  IN     UINTN                      FruSlotNumber,
  IN     UINTN                      FruDataOffset,
  IN OUT UINTN                      *FruDataSize,
  OUT    UINT8                      *FruData
);

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
typedef
EFI_STATUS
(EFIAPI *EFI_SET_FRU_REDIR_DATA) (
  IN     EFI_SM_FRU_REDIR_PROTOCOL  *This,
  IN     UINTN                      FruSlotNumber,
  IN     UINTN                      FruDataOffset,
  IN OUT UINTN                      *FruDataSize,
  IN     UINT8                      *FruData
);

/**
    FRU redir protocol.
*/
struct _EFI_SM_FRU_REDIR_PROTOCOL {
    /// Gets FRU Inventory Area info.
    EFI_GET_FRU_REDIR_INFO  GetFruRedirInfo;

    /// Gets FRU slot info.
    EFI_GET_FRU_SLOT_INFO   GetFruSlotInfo;

    /// Gets the specific data FRU inventory.
    EFI_GET_FRU_REDIR_DATA  GetFruRedirData;

    /// Sets the specific data FRU inventory.
    EFI_SET_FRU_REDIR_DATA  SetFruRedirData;
};

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

extern EFI_GUID gEfiRedirFruProtocolGuid;

//---------------------------------------------------------------------------

#endif  // #ifndef _REDIR_FRU_PROTOCOL_H_

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
