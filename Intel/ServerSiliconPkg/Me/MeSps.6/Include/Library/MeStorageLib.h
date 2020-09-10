/** @file
  ME Storage Library

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2017 Intel Corporation.

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

#ifndef _ME_STORAGE_LIB_H_
#define _ME_STORAGE_LIB_H_

#define ME_STORAGE_MAX_INDEX      255 // Max storage entry index
#define ME_STORAGE_MIN_BUFFER       1 // Min storage entry buffer length
#define ME_STORAGE_MAX_BUFFER 1024*33 // Max storage entry buffer length

/**
  Checks whether ME Storage is supported or not.

  @returns True is returned if ME is functional and supports stoarage
           functionality, otherwise false.
 **/
BOOLEAN EFIAPI
MeStorageIsSupported(
  VOID
);

/**
  Read entry from ME storage.

  @param[in]  Index    Index of the entry to read
  @param[out] pBuffer  Buffer for the entry data
  @param[io]  pBufLen  On input length of buffer, on exit length of entry data

  @retval EFI_SUCCESS            Command succeeded
  @retval EFI_INVALID_PARAMETER  Index out of range or null pointer passed to the function
  @retval EFI_PROTOCOL_ERROR     ME reports error, detailed status in pStatus
  @retval EFI_BUFFER_TOO_SMALL   Buffer too small, entry data truncated
  @retval EFI_DEVICE_ERROR       HECI communication error
 **/
EFI_STATUS EFIAPI
MeStorageEntryRead(
  IN     UINT8   Index,
     OUT UINT8  *pStatus,
     OUT VOID   *pBuffer,
  IN OUT UINT32 *pBufLen);

/**
  Write entry to ME storage.

  @param[in]  Index    Index of the entry to read
  @param[in]  pBuffer  Buffer with data to store
  @param[in]  BufLen   Length of buffer data to store

  @retval EFI_SUCCESS            Command succeeded
  @retval EFI_INVALID_PARAMETER  Index out of range or null pointer passed to the function
  @retval EFI_PROTOCOL_ERROR     ME reports error, detailed status in pStatus
  @retval EFI_BAD_BUFFER_SIZE    Buffer size out of range
  @retval EFI_DEVICE_ERROR       HECI communication error
 **/
EFI_STATUS EFIAPI
MeStorageEntryWrite(
  IN     UINT8   Index,
     OUT UINT8  *pStatus,
  IN     VOID   *pBuffer,
  IN     UINT32  BufLen);

#endif // _ME_STORAGE_LIB_H_

