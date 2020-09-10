/** @file
 Implements functions to send HECI messages in context of cosigning feature.

 @copyright
 INTEL CONFIDENTIAL
 Copyright 2018 - 2020 Intel Corporation.

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

#include <Library/UefiBootServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/MeTypeLib.h>
#include <Library/DebugLib.h>

#include <Library/MeCosignLib.h>
#include <Protocol/HeciControlProtocol.h>
#include <Library/MeUtilsLib.h>
#include "MeCosignLibInternal.h"
// TODO: Remove Simics environment check IsSimicsEnvironment() once Simics model support new logic
#include <Library/PreSiliconEnvDetectLib.h>

/**
  This is interface which allow sending SB_VERIFY_MANIFEST HECI command.
  Function allows cosign manifest verification.

  @param[in]  Manifest           Manifest data buffer
  @param[in]  ManifestSize       Manifest size in bytes
  @param[in]  Metadata           Manigest metadata buffer
  @param[in]  MetadataSize       Manifest metadata size in bytes

  @retval EFI_SUCCESS            Command succeeded (manifest verified)
  @retval EFI_INVALID_PARAMETER  Error in manifest verification process
  @retval EFI_DEVICE_ERROR       HECI communication error
 **/

EFI_STATUS
EFIAPI
MeCoSignVerifyManifest (
    IN VOID     *Manifest,
    IN UINT32   ManifestSize,
    IN VOID     *MetaData,
    IN UINT32   MetaDataSize,
    IN UINT32   UsageIndex)
{
  EFI_STATUS    Status = EFI_UNSUPPORTED;
#if ME_SUPPORT_FLAG
  HECI_CONTROL          *pHeci;

  UINT32 ReqLen;
  UINT32 RspLen;
  UINT32 BufLen;

  SB_VERIFY_MANIFEST_REQ *MsgReq;
  SB_VERIFY_MANIFEST_RSP *MsgRsp;

  VOID *HeciMsg;
  VOID *MsgDataBuf;

  VOID  *Chunk;
  UINT8  Chunks;
  UINT8  ChunkNumer;
  UINT16 ChunkSize;

  if (!MeTypeIsSps ()) {
    return EFI_UNSUPPORTED;
  }

  if (ManifestSize > MAX_MANIFEST_SIZE) {
    DEBUG ((DEBUG_ERROR, "[SPS] ERROR: MeCoSignVerifyManifest: Manifest size too big (%d)\n", ManifestSize));
    return EFI_INVALID_PARAMETER;
  }

  if (MetaDataSize > MAX_METADATA_SIZE) {
    DEBUG ((DEBUG_ERROR, "[SPS] ERROR: MeCoSignVerifyManifest: Metadata size too big (%d)\n", MetaDataSize));
    return EFI_INVALID_PARAMETER;
  }

  Status = gBS->LocateProtocol (&gHeciControlProtocolGuid, NULL, &pHeci);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[SPS] ERROR: Cannot locate HECI protocol (status: %r)\n", Status));
    return EFI_DEVICE_ERROR;
  }
  // TODO: Remove Simics environment check and make ChunkSize equal to MAX_CHUNK_SIZE once Simics model support new logic
  ChunkSize = (UINT16)((IsSimicsEnvironment() == FALSE) ? MAX_CHUNK_SIZE : ManifestSize);
  Chunks = (UINT8)(ManifestSize / ChunkSize);
  if (ManifestSize % ChunkSize) {
    Chunks++;
  }
  Chunk = Manifest;

  // send manifest data to ME
  ReqLen = sizeof (SB_VERIFY_MANIFEST_REQ) + ChunkSize;
  RspLen = sizeof (SB_VERIFY_MANIFEST_RSP);

  BufLen = (ReqLen > RspLen) ? ReqLen : RspLen;

  Status = gBS->AllocatePool (EfiBootServicesData, BufLen, &HeciMsg);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[SPS] ERROR: Cannot allocate memory (status: %r)\n", Status));
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  for (ChunkNumer = 1; ChunkNumer <= Chunks; ChunkNumer++) {
    MsgReq = (SB_VERIFY_MANIFEST_REQ*) HeciMsg;
    MsgReq->MkhiHeader.Data = 0;
    MsgReq->MkhiHeader.Fields.GroupId = MKHI_SECURE_BOOT_GROUP_ID;
    MsgReq->MkhiHeader.Fields.Command = MKHI_SB_VERIFY_MANIFEST_CMD_ID;

    MsgReq->UsageIndex = UsageIndex;
    MsgReq->ManifestSize = ManifestSize/sizeof(UINT32);
    MsgReq->MetaDataSize = MetaDataSize;
    MsgReq->Flags.Data = 0;
    MsgReq->Flags.Flags.StartOverVerify = (ChunkNumer == 1) ? 1 : 0;
    MsgReq->Flags.Flags.ManifestDataInChunk = 1;
    MsgReq->ChunkSize = (UINT16)((ManifestSize >= (UINT32)(ChunkNumer * ChunkSize)) ? (ChunkSize) : (ManifestSize - ((ChunkNumer - 1) * ChunkSize)));
    MsgReq->Reserved = 0;

    ReqLen = sizeof (SB_VERIFY_MANIFEST_REQ) + MsgReq->ChunkSize;
    MsgDataBuf = (UINT8*)HeciMsg + sizeof (SB_VERIFY_MANIFEST_REQ);
    gBS->CopyMem (MsgDataBuf, Chunk, MsgReq->ChunkSize);

    DEBUG ((DEBUG_INFO, "[SPS] SB_MANIFEST_VERIFY request (part #1 chunk #%d: manifest)\n", ChunkNumer));

    Status = pHeci->HeciSendAndReceive (
                      pHeci,
                      HECI_DEFAULT_PCH_ID,
                      NULL,
                      HECI_DEFAULT_RETRY_NUMBER,
                      SPS_CLIENTID_BIOS,
                      HECI_MKHI_MESSAGE_ADDR,
                      (UINT32*) HeciMsg,
                      ReqLen,
                      (UINT32*) HeciMsg,
                      &RspLen,
                      NULL,
                      NULL
                      );

    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "[SPS] ERROR: Cannot send SB_MANIFEST_VERIFY request (part #1 chunk #%d: manifest) (%r)\n", ChunkNumer, Status));
      Status = EFI_DEVICE_ERROR;
      goto ExitMeCoSignVerifyManifest;
    }
    MsgRsp = (SB_VERIFY_MANIFEST_RSP*) HeciMsg;
    if (MsgRsp->MkhiHeader.Fields.GroupId != MKHI_SECURE_BOOT_GROUP_ID ||
        MsgRsp->MkhiHeader.Fields.Command != MKHI_SB_VERIFY_MANIFEST_CMD_ID ||
        MsgRsp->MkhiHeader.Fields.IsResponse == 0 ||
        MsgRsp->MkhiHeader.Fields.Result != SB_VERIFY_MANIFEST_RSP_OK_CHUNK) {

      DEBUG ((
        DEBUG_ERROR,
        "[SPS] ERROR: Error of chunk processing in SB_MANIFEST_VERIFY request (part #1: manifest) (%X)\n",
        MsgRsp->MkhiHeader.Fields.Result
        ));
      Status = EFI_INVALID_PARAMETER;
      goto ExitMeCoSignVerifyManifest;
    }
    Chunk = (UINT8*) Chunk + MsgReq->ChunkSize;
  }

  gBS->FreePool (HeciMsg);

  // manifest is sent, proceed sending metadata

  ReqLen = sizeof (SB_VERIFY_MANIFEST_REQ) + MetaDataSize;
  RspLen = sizeof (SB_VERIFY_MANIFEST_RSP);

  BufLen = (ReqLen > RspLen) ? ReqLen : RspLen;

  Status = gBS->AllocatePool (EfiBootServicesData, BufLen, &HeciMsg);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[SPS] ERROR: Cannot allocate memory (status: %r)\n", Status));
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  MsgReq = (SB_VERIFY_MANIFEST_REQ*) HeciMsg;
  MsgReq->MkhiHeader.Data = 0;
  MsgReq->MkhiHeader.Fields.GroupId = MKHI_SECURE_BOOT_GROUP_ID;
  MsgReq->MkhiHeader.Fields.Command = MKHI_SB_VERIFY_MANIFEST_CMD_ID;

  MsgReq->UsageIndex = UsageIndex;
  MsgReq->ManifestSize = ManifestSize/sizeof(UINT32);
  MsgReq->MetaDataSize = MetaDataSize;
  MsgReq->Flags.Data = 0;
  MsgReq->Flags.Flags.MetadataDataInChunk = 1;
  MsgReq->Flags.Flags.LastChunk = 1;
  MsgReq->ChunkSize = (UINT16) MetaDataSize; // safe: max size of metadata is 88 assuming SHA512 was supported.
  MsgReq->Reserved = 0;

  MsgDataBuf = (UINT8*)HeciMsg + sizeof (SB_VERIFY_MANIFEST_REQ);
  gBS->CopyMem (MsgDataBuf, MetaData, MetaDataSize);

  DEBUG ((DEBUG_INFO, "[SPS] SB_MANIFEST_VERIFY request (part #2: metadata)\n"));

  Status = pHeci->HeciSendAndReceive (
                    pHeci,
                    HECI_DEFAULT_PCH_ID,
                    NULL,
                    HECI_DEFAULT_RETRY_NUMBER,
                    SPS_CLIENTID_BIOS,
                    HECI_MKHI_MESSAGE_ADDR,
                    (UINT32*) HeciMsg,
                    ReqLen,
                    (UINT32*) HeciMsg,
                    &RspLen,
                    NULL,
                    NULL
                    );

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR,"[SPS] ERROR: Cannot send SB_MANIFEST_VERIFY request (part #2: metadata) (Status=%r)\n", Status));
    Status = EFI_DEVICE_ERROR;
    goto ExitMeCoSignVerifyManifest;
  }

  MsgRsp = (SB_VERIFY_MANIFEST_RSP*) HeciMsg;
  if (MsgRsp->MkhiHeader.Fields.GroupId != MKHI_SECURE_BOOT_GROUP_ID ||
      MsgRsp->MkhiHeader.Fields.Command != MKHI_SB_VERIFY_MANIFEST_CMD_ID ||
      MsgRsp->MkhiHeader.Fields.IsResponse == 0 ||
      MsgRsp->MkhiHeader.Fields.Result != MkhiStatusSuccess) {

    DEBUG (( DEBUG_ERROR, "[SPS] ERROR: Error in SB_MANIFEST_VERIFY request (part #2: metadata) (Status=%X)\n", MsgRsp->MkhiHeader.Fields.Result));

    Status = EFI_INVALID_PARAMETER;
    goto ExitMeCoSignVerifyManifest;
  }

ExitMeCoSignVerifyManifest:

  gBS->FreePool (HeciMsg);
  DEBUG ((DEBUG_INFO, "[SPS] MeCoSignVerifyManifest exit: %r\n", Status));
#endif // ME_SUPPORT_FLAG

  return Status;
}

/**
  This is an interface which allow sending ARBH_SVN_COMMIT_NONCE HECI command.
  Function instructs ME to commit SVNs into FPFs.

  @retval EFI_SUCCESS             Command succeeded (SVNs were or are going to be committed)
  @retval EFI_INVALID_PARAMETER   Error while executing ARBH_SVN_COMMIT_NONCE command
  @retval EFI_DEVICE_ERROR        HECI communication error
 **/
EFI_STATUS
EFIAPI
MeCoSignArbSvnCommit (
  VOID
  )
{
  EFI_STATUS                   Status = EFI_UNSUPPORTED;
#if ME_SUPPORT_FLAG
  HECI_CONTROL                 *pHeci;
  UINT32                       ReqLen;
  UINT32                       RspLen;
  SB_ARBH_SVN_COMMIT_NONCE_REQ MsgReq;
  SB_ARBH_SVN_COMMIT_NONCE_RSP MsgRsp;

  if (!MeTypeIsSps ()) {
    return EFI_UNSUPPORTED;
  }

  Status = gBS->LocateProtocol (&gHeciControlProtocolGuid, NULL, &pHeci);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[SPS] ERROR: Cannot locate HECI protocol (status: %r)\n", Status));
    return EFI_DEVICE_ERROR;
  }

  // Send ARB SVN commit nonce command to ME
  ReqLen = sizeof (SB_ARBH_SVN_COMMIT_NONCE_REQ);
  RspLen = sizeof (SB_ARBH_SVN_COMMIT_NONCE_RSP);

  MsgReq.MkhiHeader.Data = 0;
  MsgReq.MkhiHeader.Fields.GroupId = MKHI_MCA_GROUP_ID;
  MsgReq.MkhiHeader.Fields.Command = MKHI_ARBH_SVN_COMMIT_NONCE_CMD_ID;

  MsgReq.Nonce = 0;
  MsgReq.UsageId = ARBH_SVN_COMMIT_ALL;
  MsgReq.Reserved1 = 0;
  MsgReq.Reserved2 = 0;

  DEBUG ((DEBUG_INFO, "[SPS] SB_ARBH_SVN_COMMIT_NONCE request\n"));

  Status = pHeci->HeciSendAndReceive (
                    pHeci,
                    HECI_DEFAULT_PCH_ID,
                    NULL,
                    HECI_DEFAULT_RETRY_NUMBER,
                    SPS_CLIENTID_BIOS,
                    HECI_MKHI_MESSAGE_ADDR,
                    (UINT32*) &MsgReq,
                    ReqLen,
                    (UINT32*) &MsgRsp,
                    &RspLen,
                    NULL,
                    NULL
                    );

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[SPS] ERROR: Cannot send SB_ARBH_SVN_COMMIT_NONCE request (%r)\n", Status));
    Status = EFI_DEVICE_ERROR;
  } else if (MsgRsp.MkhiHeader.Fields.GroupId != MKHI_MCA_GROUP_ID ||
             MsgRsp.MkhiHeader.Fields.Command != MKHI_ARBH_SVN_COMMIT_NONCE_CMD_ID ||
             MsgRsp.MkhiHeader.Fields.IsResponse == 0 ||
             MsgRsp.MkhiHeader.Fields.Result != MkhiStatusSuccess) {
    DEBUG ((
      DEBUG_ERROR,
      "[SPS] ERROR: Error in SB_ARBH_SVN_COMMIT_NONCE request (0x%X)\n",
      MsgRsp.MkhiHeader.Fields.Result
      ));
    Status = EFI_INVALID_PARAMETER;
  }

  DEBUG ((DEBUG_INFO, "[SPS] %a Exit: %r\n", __FUNCTION__, Status));
#endif // ME_SUPPORT_FLAG

  return Status;
}
