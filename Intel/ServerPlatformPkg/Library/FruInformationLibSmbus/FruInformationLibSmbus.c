/** @file
  Basic FRU(Field Replaceable Unit) information for board data

  The FRU Information is used to primarily to provide 'inventory' information about the boards that the FRU
  Information Device is located on. It is a goal that all major field replaceable units (FRUs) have an
  EEPROM or FRU Information Device from which, at a minimum, the part number or version number can
  be read through software. It is also a goal that the FRUs serial number be software readable.

  https://www.intel.com/content/dam/www/public/us/en/documents/product-briefs/platform-management-fru-document-rev-1-2-feb-2013.pdf

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 Intel Corporation. <BR>

  The source code contained or described herein and all documents related to the
  source code ("Material") are owned by Intel Corporation or its suppliers or
  licensors. Title to the Material remains with Intel Corporation or its suppliers
  and licensors. The Material may contain trade secrets and proprietary    and
  confidential information of Intel Corporation and its suppliers and licensors,
  and is protected by worldwide copyright and trade secret laws and treaty
  provisions. No part of the Material may be used, copied, reproduced, modified,
  published, uploaded, posted, transmitted, distributed, or disclosed in any way
  without Intel's prior express written permission.

  No license under any patent, copyright, trade secret or other intellectual
  property right is granted to or conferred upon you by disclosure or delivery
  of the Materials, either expressly, by implication, inducement, estoppel or
  otherwise. Any license under such intellectual property rights must be
  express and approved by Intel in writing.

  Unless otherwise agreed by Intel in writing, you may not remove or alter
  this notice or any other notice embedded in Materials by Intel or
  Intel's suppliers or licensors in any way.
**/
#include <Base.h>
#include <PiPei.h>
#include <Library/FruInformationLib.h>
#include <Ppi/Smbus2.h>
#include <Library/PcdLib.h>
#include <Library/DebugLib.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <Private/Library/PchSmbusCommonLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>



#define NUM_OF_RETRIES 3

typedef struct {
  UINT8 FormatVersionNumber : 4;
  UINT8 Reserved : 4;
  UINT8 InternalUseStartingOffset;
  UINT8 ChassisInfoStartingOffset;
  UINT8 BoardAreaStartingOffset;
  UINT8 ProductInfoStartingOffset;
  UINT8 MultiRecInfoStartingOffset;
  UINT8 Pad;
  UINT8 Checksum;
} IPMI_FRU_COMMON_HEADER;

EFI_STATUS
GetFruData(
IN  UINTN                               FruSlotNumber,
IN  UINTN                               FruDataOffset,
IN  UINTN                               FruDataSize,
IN  UINT8                               *FruData) 
{
  EFI_SMBUS_DEVICE_ADDRESS  SmbusDeviceAddress;
  EFI_SMBUS_DEVICE_COMMAND  SmbusCommand;
  EFI_STATUS                Status;
  UINT8                    SmbusData = 0;
  UINTN                     SmbusLength = 1;
  UINTN i;
  
  if(FruData == NULL || FruSlotNumber != 0) {
    return EFI_INVALID_PARAMETER;
  }
  SmbusDeviceAddress.SmbusDeviceAddress = (FixedPcdGet8 (PcdFruInventoryDeviceSmbusAddress) >> 1);
  for(i=0; i < FruDataSize; i++) {    
    // Read FRU SMBUS Controller 
    
    SmbusCommand = FruDataOffset + i;
    Status = SmbusExec( SmbusDeviceAddress,
               SmbusCommand,
               EfiSmbusReadByte,
               FALSE,
               &SmbusLength,
               &SmbusData
               );
    if (EFI_ERROR (Status)) {
      return Status;
    }

    FruData[i] = SmbusData;
  }    
  
  return EFI_SUCCESS;
}

VOID
GetStringFromFru (
  IN OUT  UINTN           * Offset,
  IN      UINT8           * TempPtr,
  IN OUT  UINT8           *StrPtr
)
/*++

Routine Description:
  This routine attempts to get a string out of the FRU at the designated offset in the
  buffer pointed to by TempPtr.  String type is ASCII

Arguments:
  Offset    - Offset of string in buffer pointed to by TempPtr, this is updated to the next
              offset.
  TempPtr   - Pointer to a buffer containing the FRU
  StrPtr    - the pointer to a buffer for retrive the string get from FRU.

Returns:
  STRING_REF with reference to string retrieved from the FRU or NULL if none is found.

--*/
{
  UINTN       Length;
  UINT8       *SrcStrPtr;
  
  if(Offset == NULL || StrPtr == NULL || TempPtr == NULL) {
        return;
  }
  Length     = 0x3F & TempPtr[*Offset];
  SrcStrPtr  = &TempPtr[*Offset + 1];

  ASSERT(Length < FRUMAXSTRING);

  if (Length > 0) {
    CopyMem(StrPtr, SrcStrPtr, Length);
    StrPtr[Length] = '\0';
  } else {
    StrPtr[0] = '\0';
  }
  *Offset = *Offset + Length + 1;
  return;
}

UINT8 *
GetFruInfoArea ( 
  IN  UINTN                     Offset
)
/*++

Routine Description:
  This routine gets the FRU info area specified by the offset and returns it in
  an allocated buffer.  It is the caller's responsibility to free the buffer.

Arguments:
  RedirFru  - SM Fru Redir protocol
  Offset    - Info Area starting offset in multiples of 8 bytes.

Returns:
  Buffer with FruInfo data or NULL if not found.

--*/
{
  EFI_STATUS  Status;
  UINT8       *TempPtr;
  UINTN       Length;

  TempPtr = NULL;

  Offset  = Offset * 8;
  if (Offset > 0) {
    //
    // Get Info area length, which is in multiples of 8 bytes
    //
    Length = 0;
    Status = GetFruData( 0, (Offset + 1), 1, (UINT8 *) &Length);
    if (Status == EFI_NOT_FOUND) {
      return NULL;
    }

    Length = Length * 8;

    if (Length > 0) {
      TempPtr = AllocatePool (Length);
      if (TempPtr == NULL) {
        return NULL;
      }

      Status = GetFruData(0, Offset, Length, TempPtr);
      if (Status == EFI_NOT_FOUND) {
        return NULL;
      }
    }
  }

  return TempPtr;
}

/**
  Reads the Board name from the FRU device specified in the PCD
  gPlatformTokenSpaceGuid.PcdFixedPlatformFruAddress
  Located in the board info area.
  This function is in charge of malloc the area for the buffer.

  @param[out] Buffer               Points to the Null terminated string with values from the FRU.
  @param[out] Size                 Size of the string returned in Buffer.

  @return EFI_SUCCESS              If the data was read from FRU
  @return EFI_UNSUPPORTED          If the function is not implemented
  @return EFI_INVALID_PARAMETER    If PcdFixedPlatformFruAddress has 0xFF or address is invalid
  @return EFI_STATUS               If smblib fails. Status from the library will be returned

 **/
EFI_STATUS
EFIAPI
FruGetBoardProductName (
  OUT CHAR8     **Buffer,
  OUT UINT8     *Size
  )
{
  EFI_STATUS                Status; 
  IPMI_FRU_COMMON_HEADER    FruCommonHeader;
  UINT8                     *FruHdrPtr;
  UINTN                     i;
  UINT8                     FruHdrChksum;
  UINT8                     *TempPtr;
  UINTN                     Offset;
  UINT8                     TempStr[FRUMAXSTRING];
  
  Status = EFI_UNSUPPORTED;
  SetMem((UINT8*)&FruCommonHeader, sizeof(IPMI_FRU_COMMON_HEADER), 0);
  Status = GetFruData(0, 0, sizeof (IPMI_FRU_COMMON_HEADER), (UINT8 *) &FruCommonHeader);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  //
  // Do a validity check on the FRU header, since it may be all 0xFF(s) in
  // case there of no FRU programmed on the system.
  //
  FruHdrPtr = (UINT8 *) &FruCommonHeader;
  for (i = 0, FruHdrChksum = 0; i < sizeof (FruCommonHeader); i++) {
    FruHdrChksum = (UINT8) (FruHdrChksum + *FruHdrPtr++);
  }

  if (FruHdrChksum != 0) {
    DEBUG ((EFI_D_ERROR, "FRU header invalid.\n"));
    Status = EFI_NOT_READY;
    //
    //  The FRU information is bad so we need to create dummy records and return.
    //
    return Status;
  }

    //
  // Get Base Board data, GetStringFromFru updates Offset to the next Field in the FRU.  The fields must
  // be retrieved in order.
  //
  TempPtr = NULL;
  TempPtr = GetFruInfoArea (FruCommonHeader.BoardAreaStartingOffset);
  if (TempPtr != NULL) {
    Offset  = 6;
    GetStringFromFru (&Offset, TempPtr, TempStr);                          // Board Manufacturer Name
    GetStringFromFru (&Offset, TempPtr, TempStr);                          // Board Product Name
    
    *Buffer = AllocatePool (FRUMAXSTRING);
    *Size = FRUMAXSTRING;
    AsciiStrCpyS(*Buffer, FRUMAXSTRING, TempStr);
    Status = EFI_SUCCESS;
    FreePool (TempPtr);
  }
  return Status;
}


/**
  Reads the Board Manufacturer name from the FRU device specified in the PCD
  gPlatformTokenSpaceGuid.PcdFixedPlatformFruAddress
  Located in the board info area.
  This function is in charge of malloc the area for the buffer.

  @param[out] Buffer               Points to the Null terminated string with values from the FRU.
  @param[out] Size                 Size of the string returned in Buffer.

  @return EFI_SUCCESS              If the data was read from FRU
  @return EFI_UNSUPPORTED          If the function is not implemented
  @return EFI_INVALID_PARAMETER    If PcdFixedPlatformFruAddress has 0xFF or address is invalid
  @return EFI_STATUS               If smblib fails. Status from the library will be returned

 **/
EFI_STATUS
EFIAPI
FruGetManufacturerName (
  OUT UINT8     **Buffer,
  OUT UINT8     *Size
  )
{
  EFI_STATUS                Status; 
  IPMI_FRU_COMMON_HEADER    FruCommonHeader;
  UINT8                     *FruHdrPtr;
  UINTN                     i;
  UINT8                     FruHdrChksum;
  UINT8                     *TempPtr;
  UINTN                     Offset;
  UINT8                     TempStr[FRUMAXSTRING];
  
  Status = EFI_UNSUPPORTED;
  SetMem((UINT8*)&FruCommonHeader, sizeof(IPMI_FRU_COMMON_HEADER), 0);
  Status = GetFruData(0, 0, sizeof (IPMI_FRU_COMMON_HEADER), (UINT8 *) &FruCommonHeader);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  //
  // Do a validity check on the FRU header, since it may be all 0xFF(s) in
  // case there of no FRU programmed on the system.
  //
  FruHdrPtr = (UINT8 *) &FruCommonHeader;
  for (i = 0, FruHdrChksum = 0; i < sizeof (FruCommonHeader); i++) {
    FruHdrChksum = (UINT8) (FruHdrChksum + *FruHdrPtr++);
  }

  if (FruHdrChksum != 0) {
    DEBUG ((EFI_D_ERROR, "FRU header invalid.\n"));
    Status = EFI_NOT_READY;
    //
    //  The FRU information is bad so we need to create dummy records and return.
    //
    return Status;
  }

    //
  // Get Base Board data, GetStringFromFru updates Offset to the next Field in the FRU.  The fields must
  // be retrieved in order.
  //
  TempPtr = NULL;
  TempPtr = GetFruInfoArea (FruCommonHeader.BoardAreaStartingOffset);
  if (TempPtr != NULL) {
    Offset  = 6;
    GetStringFromFru (&Offset, TempPtr, TempStr);                          // Board Manufacturer Name
    
    *Buffer = AllocatePool (FRUMAXSTRING);
    *Size = FRUMAXSTRING;
    AsciiStrCpyS(*Buffer, FRUMAXSTRING, TempStr);
    Status = EFI_SUCCESS;
    FreePool (TempPtr);
  }
  return Status;
}

/**
  Reads the board part number from the FRU device specified in the PCD
  gPlatformTokenSpaceGuid.PcdFixedPlatformFruAddress
  Located in the board info area.
  This function is in charge of malloc the area for the buffer.

  @param[out] Buffer               Points to the Null terminated string with values from the FRU.
  @param[out] Size                 Size of the string returned in Buffer.

  @return EFI_SUCCESS              If the data was read from FRU
  @return EFI_UNSUPPORTED          If the function is not implemented
  @return EFI_INVALID_PARAMETER    If PcdFixedPlatformFruAddress has 0xFF or address is invalid
  @return EFI_STATUS               If smblib fails. Status from the library will be returned

 **/
EFI_STATUS
EFIAPI
FruGetPartNumber (
  OUT UINT8     **Buffer,
  OUT UINT8     *Size
  )
{
  EFI_STATUS                Status; 
  IPMI_FRU_COMMON_HEADER    FruCommonHeader;
  UINT8                     *FruHdrPtr;
  UINTN                     i;
  UINT8                     FruHdrChksum;
  UINT8                     *TempPtr;
  UINTN                     Offset;
  UINT8                     TempStr[FRUMAXSTRING];
  
  Status = EFI_UNSUPPORTED;
  SetMem((UINT8*)&FruCommonHeader, sizeof(IPMI_FRU_COMMON_HEADER), 0);
  Status = GetFruData(0, 0, sizeof (IPMI_FRU_COMMON_HEADER), (UINT8 *) &FruCommonHeader);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  //
  // Do a validity check on the FRU header, since it may be all 0xFF(s) in
  // case there of no FRU programmed on the system.
  //
  FruHdrPtr = (UINT8 *) &FruCommonHeader;
  for (i = 0, FruHdrChksum = 0; i < sizeof (FruCommonHeader); i++) {
    FruHdrChksum = (UINT8) (FruHdrChksum + *FruHdrPtr++);
  }

  if (FruHdrChksum != 0) {
    DEBUG ((EFI_D_ERROR, "FRU header invalid.\n"));
    Status = EFI_NOT_READY;
    //
    //  The FRU information is bad so we need to create dummy records and return.
    //
    return Status;
  }

    //
  // Get Base Board data, GetStringFromFru updates Offset to the next Field in the FRU.  The fields must
  // be retrieved in order.
  //
  TempPtr = NULL;
  TempPtr = GetFruInfoArea (FruCommonHeader.BoardAreaStartingOffset);
  if (TempPtr != NULL) {
    Offset  = 6;
    GetStringFromFru (&Offset, TempPtr, TempStr);                          // Board Manufacturer Name
    GetStringFromFru (&Offset, TempPtr, TempStr);                          // Board Product Name
    GetStringFromFru (&Offset, TempPtr, TempStr);                          // Board Serial Number
    GetStringFromFru (&Offset, TempPtr, TempStr);                          // Board Part Number
    
    *Buffer = AllocatePool (FRUMAXSTRING);
    *Size = FRUMAXSTRING;
    AsciiStrCpyS(*Buffer, FRUMAXSTRING, TempStr);
    Status = EFI_SUCCESS;
    FreePool (TempPtr);
  }
  return Status;
}

/**
  Reads the board version from the FRU device specified in the PCD
  gPlatformTokenSpaceGuid.PcdFixedPlatformFruAddress
  Located in the board info area.
  This function is in charge of malloc the area for the buffer.

  @param[out] Buffer               Points to the Null terminated string with values from the FRU.
  @param[out] Size                 Size of the string returned in Buffer.

  @return EFI_SUCCESS              If the data was read from FRU
  @return EFI_UNSUPPORTED          If the function is not implemented
  @return EFI_INVALID_PARAMETER    If PcdFixedPlatformFruAddress has 0xFF or address is invalid
  @return EFI_STATUS               If smblib fails. Status from the library will be returned

 **/
EFI_STATUS
EFIAPI
FruGetVersionType (
  OUT UINT8     **Buffer,
  OUT UINT8     *Size
  )
{
  EFI_STATUS                Status; 
  IPMI_FRU_COMMON_HEADER    FruCommonHeader;
  UINT8                     *FruHdrPtr;
  UINTN                     i;
  UINT8                     FruHdrChksum;
  UINT8                     *TempPtr;
  UINTN                     Offset;
  UINT8                     TempStr[FRUMAXSTRING];
  
  Status = EFI_UNSUPPORTED;
  SetMem((UINT8*)&FruCommonHeader, sizeof(IPMI_FRU_COMMON_HEADER), 0);
  Status = GetFruData(0, 0, sizeof (IPMI_FRU_COMMON_HEADER), (UINT8 *) &FruCommonHeader);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  //
  // Do a validity check on the FRU header, since it may be all 0xFF(s) in
  // case there of no FRU programmed on the system.
  //
  FruHdrPtr = (UINT8 *) &FruCommonHeader;
  for (i = 0, FruHdrChksum = 0; i < sizeof (FruCommonHeader); i++) {
    FruHdrChksum = (UINT8) (FruHdrChksum + *FruHdrPtr++);
  }

  if (FruHdrChksum != 0) {
    DEBUG ((EFI_D_ERROR, "FRU header invalid.\n"));
    Status = EFI_NOT_READY;
    //
    //  The FRU information is bad so we need to create dummy records and return.
    //
    return Status;
  }

    //
  // Get Base Board data, GetStringFromFru updates Offset to the next Field in the FRU.  The fields must
  // be retrieved in order.
  //
  TempPtr = NULL;
  TempPtr = GetFruInfoArea (FruCommonHeader.BoardAreaStartingOffset);
  if (TempPtr != NULL) {
    Offset  = 6;
    GetStringFromFru (&Offset, TempPtr, TempStr);                          // Board Manufacturer Name
    GetStringFromFru (&Offset, TempPtr, TempStr);                          // Board Product Name
    GetStringFromFru (&Offset, TempPtr, NULL);                          // Board Serial Number
    GetStringFromFru (&Offset, TempPtr, NULL);                          // Board Part Number
    GetStringFromFru (&Offset, TempPtr, TempStr);                       // FRU File Version ID string
    
    *Buffer = AllocatePool (FRUMAXSTRING);
    *Size = FRUMAXSTRING;
    AsciiStrCpyS(*Buffer, FRUMAXSTRING, TempStr);
    Status = EFI_SUCCESS;
    FreePool (TempPtr);
  }
  return Status;
}

/**
  Reads the board serial number from the FRU device specified in the PCD
  gPlatformTokenSpaceGuid.PcdFixedPlatformFruAddress
  Located in the board info area.
  This function is in charge of malloc the area for the buffer.

  @param[out] Buffer               Points to the Null terminated string with values from the FRU.
  @param[out] Size                 Size of the string returned in Buffer.

  @return EFI_SUCCESS              If the data was read from FRU
  @return EFI_UNSUPPORTED          If the function is not implemented
  @return EFI_INVALID_PARAMETER    If PcdFixedPlatformFruAddress has 0xFF or address is invalid
  @return EFI_STATUS               If smblib fails. Status from the library will be returned

 **/
EFI_STATUS
EFIAPI
FruGetSerialNumber (
  OUT UINT8     **Buffer,
  OUT UINT8     *Size
  )
{
  EFI_STATUS                Status; 
  IPMI_FRU_COMMON_HEADER    FruCommonHeader;
  UINT8                     *FruHdrPtr;
  UINTN                     i;
  UINT8                     FruHdrChksum;
  UINT8                     *TempPtr;
  UINTN                     Offset;
  UINT8                     TempStr[FRUMAXSTRING];
  
  Status = EFI_UNSUPPORTED;
  SetMem((UINT8*)&FruCommonHeader, sizeof(IPMI_FRU_COMMON_HEADER), 0);
  Status = GetFruData(0, 0, sizeof (IPMI_FRU_COMMON_HEADER), (UINT8 *) &FruCommonHeader);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  //
  // Do a validity check on the FRU header, since it may be all 0xFF(s) in
  // case there of no FRU programmed on the system.
  //
  FruHdrPtr = (UINT8 *) &FruCommonHeader;
  for (i = 0, FruHdrChksum = 0; i < sizeof (FruCommonHeader); i++) {
    FruHdrChksum = (UINT8) (FruHdrChksum + *FruHdrPtr++);
  }

  if (FruHdrChksum != 0) {
    DEBUG ((EFI_D_ERROR, "FRU header invalid.\n"));
    Status = EFI_NOT_READY;
    //
    //  The FRU information is bad so we need to create dummy records and return.
    //
    return Status;
  }

    //
  // Get Base Board data, GetStringFromFru updates Offset to the next Field in the FRU.  The fields must
  // be retrieved in order.
  //
  TempPtr = NULL;
  TempPtr = GetFruInfoArea (FruCommonHeader.BoardAreaStartingOffset);
  if (TempPtr != NULL) {
    Offset  = 6;
    GetStringFromFru (&Offset, TempPtr, TempStr);                          // Board Manufacturer Name
    GetStringFromFru (&Offset, TempPtr, TempStr);                          // Board Product Name
    GetStringFromFru (&Offset, TempPtr, TempStr);                          // Board Serial Number
    
    *Buffer = AllocatePool (FRUMAXSTRING);
    *Size = FRUMAXSTRING;
    AsciiStrCpyS(*Buffer, FRUMAXSTRING, TempStr);
    Status = EFI_SUCCESS;
    FreePool (TempPtr);
  }
  return Status;
}



