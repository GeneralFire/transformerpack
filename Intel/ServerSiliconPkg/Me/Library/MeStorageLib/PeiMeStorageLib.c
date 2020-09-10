/** @file
  Implements ME Storage HECI Interface Library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2019 Intel Corporation.

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
#include <Uefi.h>
#include <PiDxe.h>
#include <Sps.h>

#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MeTypeLib.h>
#include <Library/MeStorageLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/PeiServicesTablePointerLib.h>

#include "Private/MeStorageLibInternal.h"
#include <Ppi/HeciPpi.h>
#include <Ppi/HeciControlPpi.h>

static BOOLEAN gSupported = FALSE;

#if defined(ME_SPS_SUPPORT) && ME_SPS_SUPPORT
#include <Guid/SpsInfoHobGuid.h>
#endif // ME_SPS_SUPPORT

/**
  The constructor function initializes the ME Storage library

  @param  ImageHandle   The firmware allocated handle for the EFI image.
  @param  PeiServices   A pointer to Pei Services Table.

  @retval EFI_SUCCESS   The constructor always returns EFI_SUCCESS.

**/
EFI_STATUS
EFIAPI
MeStorageLibConstructor (
  IN EFI_PEI_FILE_HANDLE    FileHandle,
  IN CONST EFI_PEI_SERVICES **PeiServices
  )
{
#if defined(ME_SPS_SUPPORT) && ME_SPS_SUPPORT
  EFI_HOB_GUID_TYPE    *SpsHob;
  SPS_INFO_HOB         *SpsHobData;

  if (MeTypeIsSps ()) {
    SpsHob = GetFirstGuidHob (&gSpsInfoHobGuid);
    if (SpsHob != NULL) {
      SpsHobData = GET_GUID_HOB_DATA (SpsHob);
      if (SpsHobData->FeatureSet.Bits.MeStorageServices) {
        gSupported = TRUE;
      }
    }
    DEBUG ((DEBUG_INFO, "[SPS] ME Storage Service %aavailable\n", (gSupported) ? "" : "NOT "));
  }
#endif // ME_SPS_SUPPORT

  return EFI_SUCCESS;
}

/**
  Checks whether ME Storage is supported or not.

  @returns True is returned if ME is functional and supports storage
           functionality, otherwise false.
 **/
BOOLEAN EFIAPI
MeStorageIsSupported (
  VOID
)
{
  return gSupported;
}

/**
  Read entry from ME storage.

  @param[in]  Index    Index of the entry to read
  @param[out] pStatus  Operation status provided by ME in HECI message
  @param[out] pBuffer  Buffer for the entry data
  @param[io]  pBufLen  On input length of buffer, on exit length of entry data

  @retval EFI_SUCCESS            Command succeeded
  @retval EFI_INVALID_PARAMETER  Index out of range or null pointer passed to the function
  @retval EFI_BAD_BUFFER_SIZE    Buffer size out of range
  @retval EFI_PROTOCOL_ERROR     ME reports error, detailed status in pStatus
  @retval EFI_BUFFER_TOO_SMALL   Buffer too small, entry data truncated
  @retval EFI_DEVICE_ERROR       HECI communication error
  @retval EFI_UNSUPPORTED        Implementation not supported
 **/
EFI_STATUS EFIAPI
MeStorageEntryRead (
  IN     UINT8   Index,
     OUT UINT8  *pStatus,
     OUT VOID   *pBuffer,
  IN OUT UINT32 *pBufLen
  )
{
#if ME_SUPPORT_FLAG
  EFI_STATUS                 Status;
  HECI_PPI                   *Heci;
  HECI_CONTROL               *HeciControl;
  UINT32                     ReqLen;
  UINT32                     RspLen;
  UINT32                     RspHeaderSize;
  ME_READ_STORAGE_SERVICES   *HeciMsg;

  DEBUG ((DEBUG_INFO, "[SPS] MeStorageEntryRead ()\n"));

  if (Index > ME_STORAGE_MAX_INDEX ||
      pStatus == NULL ||
      pBuffer == NULL ||
      pBufLen == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (*pBufLen < ME_STORAGE_MIN_BUFFER ||
      *pBufLen > ME_STORAGE_MAX_BUFFER) {
    return EFI_BAD_BUFFER_SIZE;
  }

  if (!MeTypeIsSps ()) {
    return EFI_UNSUPPORTED;
  }

  Heci = NULL;
  HeciControl = NULL;
  Status = PeiServicesLocatePpi (&gHeciPpiGuid, 0, NULL, &Heci);
  if (EFI_ERROR (Status)) {
    Status = PeiServicesLocatePpi (&gHeciControlPpiGuid, 0, NULL, &HeciControl);
  }
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[SPS] ERROR: Cannot locate HECI protocol (status: %r)\n", Status));
    return EFI_DEVICE_ERROR;
  }
  HeciMsg = (ME_READ_STORAGE_SERVICES *) AllocateZeroPool (sizeof (ME_READ_STORAGE_SERVICES));
  if (HeciMsg == NULL) {
    ASSERT_EFI_ERROR (Status);
    DEBUG ((DEBUG_ERROR, "[SPS] ERROR: Cannot allocate memory (status: %r)\n", Status));
    return Status;
  }

  *pStatus = 0;

  ReqLen = sizeof (ME_READ_STORAGE_SERVICES_REQ);

  HeciMsg->MeStorageSrvReq.ApiFunctionSelector = SPS_ME_STORAGE_READ_STORAGE_ENTRY_CMD;
  HeciMsg->MeStorageSrvReq.Nonce = 0;
  HeciMsg->MeStorageSrvReq.StorageEntryIndex = Index;
  HeciMsg->MeStorageSrvReq.SeqNumber = 0;

  DEBUG ((DEBUG_INFO, "[SPS] MeStorageServices request: \n"));
  DEBUG ((DEBUG_INFO, "[SPS]  ApiFunctionSelector: %d\n", HeciMsg->MeStorageSrvReq.ApiFunctionSelector));
  DEBUG ((DEBUG_INFO, "[SPS]  SeqNumber: %d\n", HeciMsg->MeStorageSrvReq.SeqNumber));
  DEBUG ((DEBUG_INFO, "[SPS]  Nonce: %d\n", HeciMsg->MeStorageSrvReq.Nonce));
  DEBUG ((DEBUG_INFO, "[SPS]  StorageEntryIndex: %d\n", HeciMsg->MeStorageSrvReq.StorageEntryIndex));

  RspHeaderSize = sizeof (ME_READ_STORAGE_SERVICES_RSP) - ME_STORAGE_MAX_BUFFER;
  RspLen = RspHeaderSize + *pBufLen;

  if (Heci != NULL) {
    Status = Heci->SendwAck (
                     HECI1_DEVICE,
                     (UINT32*)&HeciMsg->MeStorageSrvReq,
                     ReqLen,
                     &RspLen,
                     SPS_CLIENTID_BIOS,
                     SPS_CLIENTID_ME_STORAGE_SERVICES
                     );
  } else {
    if (HeciControl == NULL) {
      ASSERT (HeciControl == NULL);
      Status = EFI_DEVICE_ERROR;
    } else {
      Status = HeciControl->HeciSendAndReceive (
                              HeciControl,
                              HECI_DEFAULT_PCH_ID,
                              NULL,
                              HECI_DEFAULT_RETRY_NUMBER,
                              SPS_CLIENTID_BIOS,
                              SPS_CLIENTID_ME_STORAGE_SERVICES,
                              (UINT32*) &HeciMsg->MeStorageSrvReq,
                              ReqLen,
                              (UINT32*) &HeciMsg->MeStorageSrvReq,
                              &RspLen,
                              NULL,
                              NULL
                              );
    }
  }
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[SPS] ERROR: Cannot send ME Storage Service request (%r)\n", Status));
    Status = EFI_DEVICE_ERROR;
    goto ExitMeStorageEntryRead;
  } else if (RspHeaderSize > RspLen ||
    HeciMsg->MeStorageSrvRsp.ApiFunctionSelector != SPS_ME_STORAGE_READ_STORAGE_ENTRY_CMD ||
    HeciMsg->MeStorageSrvRsp.StorageEntryIndex != Index ||
    (HeciMsg->MeStorageSrvRsp.SeqNumber & ~SPS_ME_STORAGE_SEQ_FLAG) != 0 ||
    (HeciMsg->MeStorageSrvRsp.SeqNumber & SPS_ME_STORAGE_SEQ_FLAG) != SPS_ME_STORAGE_SEQ_FLAG) {
    DEBUG ((DEBUG_ERROR, "[SPS] ERROR: Sending ME Storage Service failed (wrong response)\n"));
    Status = EFI_INVALID_PARAMETER;
    goto ExitMeStorageEntryRead;
  } else if (HeciMsg->MeStorageSrvRsp.OperationStatus != 0) {
    DEBUG ((DEBUG_ERROR, "[SPS] ERROR: ME Storage Service operation status: %d\n",
                        HeciMsg->MeStorageSrvRsp.OperationStatus));
    *pStatus = HeciMsg->MeStorageSrvRsp.OperationStatus;
    Status = EFI_PROTOCOL_ERROR;
    goto ExitMeStorageEntryRead;
  } else {
     DEBUG ((DEBUG_INFO, "[SPS] MeStorageServices response: Success.\n"));
  }

  RspLen -= RspHeaderSize;

  if (*pBufLen < RspLen) {
    Status = EFI_BUFFER_TOO_SMALL;
    goto ExitMeStorageEntryRead;
  }

  CopyMem (pBuffer, &HeciMsg->MeStorageSrvRsp.Data, (UINTN)RspLen);
  *pBufLen = RspLen;

ExitMeStorageEntryRead:
  FreePool (HeciMsg);

  DEBUG ((DEBUG_INFO, "[SPS] MeStorageEntryRead exit: %r\n", Status));

  return Status;
#else // ME_SUPPORT_FLAG
  return EFI_UNSUPPORTED;
#endif // ME_SUPPORT_FLAG
}

/**
  Write entry to ME storage.

  @param[in]  Index    Index of the entry to read
  @param[out] pStatus  Operation status provided by ME in HECI message
  @param[in]  pBuffer  Buffer with data to store
  @param[in]  BufLen   Length of buffer data to store

  @retval EFI_SUCCESS            Command succeeded
  @retval EFI_INVALID_PARAMETER  Index out of range or null pointer passed to the function
  @retval EFI_PROTOCOL_ERROR     ME reports error, detailed status in pStatus
  @retval EFI_BAD_BUFFER_SIZE    Buffer size out of range
  @retval EFI_DEVICE_ERROR       HECI communication error
  @retval EFI_UNSUPPORTED        Implementation not supported
 **/
EFI_STATUS EFIAPI
MeStorageEntryWrite(
  IN     UINT8   Index,
  OUT    UINT8  *pStatus,
  IN     VOID   *pBuffer,
  IN     UINT32  BufLen
  )
{
#if ME_SUPPORT_FLAG
  EFI_STATUS                  Status;
  HECI_PPI                    *Heci;
  HECI_CONTROL                *HeciControl;
  UINT32                      ReqLen;
  UINT32                      RspLen;
  ME_WRITE_STORAGE_SERVICES   *HeciMsg;

  DEBUG ((DEBUG_INFO, "[SPS] MeStorageEntryWrite ()\n"));

  if (Index > ME_STORAGE_MAX_INDEX ||
      pStatus == NULL ||
      pBuffer == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  if (BufLen < ME_STORAGE_MIN_BUFFER ||
      BufLen > ME_STORAGE_MAX_BUFFER) {
    return EFI_BAD_BUFFER_SIZE;
  }

  if (!MeTypeIsSps ()) {
    return EFI_UNSUPPORTED;
  }

  Heci = NULL;
  HeciControl = NULL;
  Status = PeiServicesLocatePpi (&gHeciPpiGuid, 0, NULL, &Heci);
  if (EFI_ERROR (Status)) {
    Status = PeiServicesLocatePpi (&gHeciControlPpiGuid, 0, NULL, &HeciControl);
  }
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[SPS] ERROR: Cannot locate HECI protocol (status: %r)\n", Status));
    return EFI_DEVICE_ERROR;
  }

  HeciMsg = (ME_WRITE_STORAGE_SERVICES *) AllocateZeroPool (sizeof (ME_WRITE_STORAGE_SERVICES));
  if (HeciMsg == NULL) {
    ASSERT_EFI_ERROR (Status);
    DEBUG ((DEBUG_ERROR, "[SPS] ERROR: Cannot allocate memory (status: %r)\n", Status));
    return Status;
  }

  *pStatus = 0;

  ReqLen = (sizeof (ME_WRITE_STORAGE_SERVICES_REQ) - ME_STORAGE_MAX_BUFFER) + BufLen;
  ASSERT (ReqLen <= sizeof (ME_WRITE_STORAGE_SERVICES_REQ));

  CopyMem (&HeciMsg->MeStorageSrvReq.Data, pBuffer, (UINTN)BufLen);

  HeciMsg->MeStorageSrvReq.ApiFunctionSelector = SPS_ME_STORAGE_WRITE_STORAGE_ENTRY_CMD;
  HeciMsg->MeStorageSrvReq.Nonce = 0;
  HeciMsg->MeStorageSrvReq.StorageEntryIndex = Index;
  HeciMsg->MeStorageSrvReq.SeqNumber = 0;

  DEBUG ((DEBUG_INFO, "[SPS] MeStorageServices request: \n"));
  DEBUG ((DEBUG_INFO, "[SPS]  ApiFunctionSelector: %d\n", HeciMsg->MeStorageSrvReq.ApiFunctionSelector));
  DEBUG ((DEBUG_INFO, "[SPS]  SeqNumber: %d\n", HeciMsg->MeStorageSrvReq.SeqNumber));
  DEBUG ((DEBUG_INFO, "[SPS]  Nonce: %d\n", HeciMsg->MeStorageSrvReq.Nonce));
  DEBUG ((DEBUG_INFO, "[SPS]  StorageEntryIndex: %d\n", HeciMsg->MeStorageSrvReq.StorageEntryIndex));

  RspLen = sizeof (ME_WRITE_STORAGE_SERVICES_RSP);
  if (Heci != NULL) {
    Status = Heci->SendwAck (
                     HECI1_DEVICE,
                     (UINT32*)&HeciMsg->MeStorageSrvReq,
                     ReqLen,
                     &RspLen,
                     SPS_CLIENTID_BIOS,
                     SPS_CLIENTID_ME_STORAGE_SERVICES
                     );
  } else {
    if (HeciControl == NULL) {
      ASSERT (HeciControl == NULL);
      Status = EFI_DEVICE_ERROR;
    } else {
      Status = HeciControl->HeciSendAndReceive (
                              HeciControl,
                              HECI_DEFAULT_PCH_ID,
                              NULL,
                              HECI_DEFAULT_RETRY_NUMBER,
                              SPS_CLIENTID_BIOS,
                              SPS_CLIENTID_ME_STORAGE_SERVICES,
                              (UINT32*) &HeciMsg->MeStorageSrvReq,
                              ReqLen,
                              (UINT32*) &HeciMsg->MeStorageSrvReq,
                              &RspLen,
                              NULL,
                              NULL
                              );
    }
  }
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[SPS] ERROR: Cannot send ME Storage Service request (%r)\n", Status));
    Status = EFI_DEVICE_ERROR;
    goto ExitMeStorageEntryWrite;
  } else if (RspLen < sizeof (ME_WRITE_STORAGE_SERVICES_RSP) ||
    HeciMsg->MeStorageSrvRsp.ApiFunctionSelector != SPS_ME_STORAGE_WRITE_STORAGE_ENTRY_CMD ||
    HeciMsg->MeStorageSrvRsp.StorageEntryIndex != Index ||
    (HeciMsg->MeStorageSrvRsp.SeqNumber & ~SPS_ME_STORAGE_SEQ_FLAG) != 0 ||
    (HeciMsg->MeStorageSrvRsp.SeqNumber & SPS_ME_STORAGE_SEQ_FLAG) != SPS_ME_STORAGE_SEQ_FLAG) {
    DEBUG ((DEBUG_ERROR, "[SPS] ERROR: Sending ME Storage Service failed (wrong response)\n"));
    Status = EFI_INVALID_PARAMETER;
    goto ExitMeStorageEntryWrite;
  } else if (HeciMsg->MeStorageSrvRsp.OperationStatus != 0) {
    DEBUG ((DEBUG_ERROR, "[SPS] ERROR: ME Storage Service operation status: %d\n",
                        HeciMsg->MeStorageSrvRsp.OperationStatus));
    *pStatus = HeciMsg->MeStorageSrvRsp.OperationStatus;
    Status = EFI_PROTOCOL_ERROR;
    goto ExitMeStorageEntryWrite;
  } else {
    DEBUG ((DEBUG_INFO, "[SPS] MeStorageServices response: Success.\n"));
  }

ExitMeStorageEntryWrite:
  FreePool (HeciMsg);

  DEBUG ((DEBUG_INFO, "[SPS] MeStorageEntryWrite exit: %r\n", Status));

  return Status;
#else // ME_SUPPORT_FLAG
  return EFI_UNSUPPORTED;
#endif // ME_SUPPORT_FLAG
}

