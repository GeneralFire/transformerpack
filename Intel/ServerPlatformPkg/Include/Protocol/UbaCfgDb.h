/** @file
  uba central config database Protocol

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2012 - 2017 Intel Corporation. <BR>

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

#ifndef _UBA_CONFIG_DATABASE_H_
#define _UBA_CONFIG_DATABASE_H_

// {E03E0D46-5263-4845-B0A4-58D57B3177E2}
#define UBA_CONFIG_DATABASE_PROTOCOL_GUID \
  { 0xe03e0d46, 0x5263, 0x4845, { 0xb0, 0xa4, 0x58, 0xd5, 0x7b, 0x31, 0x77, 0xe2 } }


typedef struct _UBA_CONFIG_DATABASE_PROTOCOL UBA_CONFIG_DATABASE_PROTOCOL;

#define UBA_CONFIG_PROTOCOL_SIGNATURE  SIGNATURE_32('M', 'S', 'K', 'P')
#define UBA_CONFIG_PROTOCOL_VERSION    0x01

/**
  Get platform's GUID and user friendly name by PlatformType.

  This is used when you need a PlatformId to Add/Get platform data

  Core will create a new platform for you if the PlatformType is not
  recorded in database, and assgin a unique GUID for this platform.

  @param This                   uba Protocol instance.
  @param PlatformType           The platform type, same define as Platform.h.
  @param PlatformId             The GUID for this platform.
  @param PlatformName           The user friendly name for this platform.

  @retval EFI_ALREADY_STARTED   Create new for an exist platform.
  @retval EFI_OUT_OF_RESOURCES  Resource not enough.
  @retval EFI_NOT_FOUND         Platform not found.
  @retval EFI_SUCCESS           Operation success.

**/
typedef
EFI_STATUS
(EFIAPI *UBA_CONFIG_GET_PLATFORM) (
  IN  UBA_CONFIG_DATABASE_PROTOCOL          *This,
  OUT UINT32                                *PlatformType,
  OUT EFI_GUID                              *PlatformId,
  OUT CHAR8                                 *PlatformName
  );

/**
  Add configuration data to uba configuration database.

  @param This                   uba Protocol instance.
  @param PlatformId             The GUID for this platform.
  @param ResId                  The configuration data resource id.
  @param Data                   The data buffer pointer.
  @param DataSize               Size of data want to add into database.

  @retval EFI_INVALID_PARAMETER Required parameters not correct.
  @retval EFI_OUT_OF_RESOURCES  Resource not enough.
  @retval EFI_NOT_FOUND         Platform not found.
  @retval EFI_SUCCESS           Operation success.

**/
typedef
EFI_STATUS
(EFIAPI *UBA_CONFIG_ADD_DATA) (
  IN  UBA_CONFIG_DATABASE_PROTOCOL          *This,
  IN  EFI_GUID                              *ResId,
  IN  VOID                                  *Data,
  IN  UINTN                                 DataSize
  );

/**
  Get configuration data from uba configuration database.

  @param This                   uba Protocol instance.
  @param ResId                  The configuration data resource id.
  @param Data                   The data buffer pointer.
  @param DataSize               IN:Size of data want to get, OUT: Size of data in database.

  @retval EFI_INVALID_PARAMETER Required parameters not correct.
  @retval EFI_BUFFER_TOO_SMALL  The DataSize of Data buffer is too small to get this configuration data
  @retval EFI_OUT_OF_RESOURCES  Resource not enough.
  @retval EFI_NOT_FOUND         Platform or data not found.
  @retval EFI_SUCCESS           Operation success.

**/
typedef
EFI_STATUS
(EFIAPI *UBA_CONFIG_GET_DATA) (
  IN  UBA_CONFIG_DATABASE_PROTOCOL          *This,
  IN  EFI_GUID                              *ResId,
  OUT VOID                                  *Data,
  OUT UINTN                                 *DataSize
  );


//
// UbaConfigDatabaseProtocol
//
struct _UBA_CONFIG_DATABASE_PROTOCOL {
  UINT32                                Signature;
  UINT32                                Version;

  UBA_CONFIG_GET_PLATFORM               GetSku;
  UBA_CONFIG_ADD_DATA                   AddData;
  UBA_CONFIG_GET_DATA                   GetData;
};

extern EFI_GUID gUbaConfigDatabaseProtocolGuid;

#endif // __UBA_CONFIG_DATABASE_H_
