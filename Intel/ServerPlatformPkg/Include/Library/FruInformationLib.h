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
#ifndef __FRU_INFORMATION_LIB_H__
#define __FRU_INFORMATION_LIB_H__

#define FRUMAXSTRING 128

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
  );


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
  );

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
  );

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
  );

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
  );


#endif
