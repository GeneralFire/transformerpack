/** @file
  HeciMsgDxeLib.c

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2006 - 2020 Intel Corporation. <BR>

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

#include <Uefi.h>
#include <PiDxe.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/IoLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/MeTypeLib.h>
#include <Library/MeUefiFwHealthStatusLib.h>
#include <Library/MeUtilsLib.h>
#include <Library/DxeMeLib.h>
#include <Library/HobLib.h>
#include <Protocol/HeciControlProtocol.h>
#include <Register/HeciRegs.h>
#include <CoreBiosMsg.h>
#include <MeState.h>
#include <Sps.h>

#define ME_DAM_FILE "/home/hotham/dbg_dam_req"

//
// Interface functions of HeciMsgLib
//

/**
  This message is sent by the BIOS when it wants to notify that the BIOS is starting
  to run 3rd party code.

  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function or EOP was sent
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
**/
EFI_STATUS
ServerHeciCoreBiosDoneMsg (
  VOID
  )
{
  EFI_STATUS                                Status = EFI_UNSUPPORTED;
#if ME_SUPPORT_FLAG
  UINT32                                    Length;
  UINT32                                    RespLength;
  CORE_BIOS_DONE                            CoreBiosDone;
  CORE_BIOS_DONE_ACK                        CoreBiosDoneAck;
  UINT32                                    MeMode;
  HECI_CONTROL                              *Heci;
  UINT8                                     GroupId;
  UINT8                                     MeClientAddress;

  DEBUG ((DEBUG_INFO, "[ME] HeciCoreBiosDoneMsg.\n"));

  Status = gBS->LocateProtocol (
                  &gHeciControlProtocolGuid,
                  NULL,
                  (VOID**) &Heci
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[ME] Unable to Locate Heci Protocol.- %r\n", Status));
    return Status;
  }

  Length = sizeof (CORE_BIOS_DONE);
  RespLength = sizeof (CORE_BIOS_DONE_ACK);

  Status = MeBiosGetMeMode (&MeMode);
  if (EFI_ERROR (Status) || (MeMode != ME_MODE_NORMAL)) {
    DEBUG ((DEBUG_ERROR, "[ME] HeciCoreBiosDoneMsg: unsupported! (status: %r - ME mode: %d)\n", Status, MeMode));
    return EFI_UNSUPPORTED;
  }

  if (MeIsAfterEndOfPost ()) {
    DEBUG ((DEBUG_ERROR, "[ME] HeciCoreBiosDoneMsg: unsupported after EndOfPost\n"));
    return EFI_UNSUPPORTED;
  }

  GetMcaMsgParams (MCA_CORE_BIOS_DONE_CMD, &GroupId, &MeClientAddress);

  CoreBiosDone.MkhiHeader.Data              = 0;
  CoreBiosDone.MkhiHeader.Fields.GroupId    = GroupId;
  CoreBiosDone.MkhiHeader.Fields.Command    = MCA_CORE_BIOS_DONE_CMD;
  CoreBiosDone.MkhiHeader.Fields.IsResponse = 0;

  Status = Heci->HeciSendAndReceive (
                   Heci,
                   HECI_DEFAULT_PCH_ID,
                   NULL,
                   HECI_DEFAULT_RETRY_NUMBER,
                   BIOS_FIXED_HOST_ADDR,
                   MeClientAddress,
                   (UINT32*) &CoreBiosDone,
                   Length,
                   (UINT32*) &CoreBiosDoneAck,
                   &RespLength,
                   NULL,
                   NULL
                   );
  if (EFI_ERROR (Status)) {
    SetMeUefiFwHealthStatus (CoreBiosDoneSendingStatus, StatusError);
    DEBUG ((DEBUG_ERROR, "[ME] Core Bios Done Message: message failed! - %r\n", Status));
  } else if ((CoreBiosDoneAck.MkhiHeader.Fields.Command != MCA_CORE_BIOS_DONE_CMD) ||
      (CoreBiosDoneAck.MkhiHeader.Fields.GroupId != MKHI_MCA_GROUP_ID) ||
      (CoreBiosDoneAck.MkhiHeader.Fields.IsResponse != 1) ||
      (CoreBiosDoneAck.MkhiHeader.Fields.Result != 0)
      ) {
    SetMeUefiFwHealthStatus (CoreBiosDoneSendingStatus, StatusOther);
    DEBUG ((
      DEBUG_ERROR,
      "[ME] ERROR: Invalid response received for CORE_BIOS_DONE (MKHI: %08X)\n",
      CoreBiosDoneAck.MkhiHeader.Data
      ));
    return EFI_DEVICE_ERROR;
  } else {
    SetMeUefiFwHealthStatus (CoreBiosDoneSendingStatus, StatusOk);
  }

  DEBUG ((DEBUG_INFO, "[ME] HeciCoreBiosDoneMsg: exit (%r)\n", Status));
#endif // ME_SUPPORT_FLAG

  return Status;
}

/**
  This message is sent by the BIOS to retrieve file stored in ME firmware NVM.

  @param[in]      FilePath        File path of file to read
  @param[in]      Offset          File offset
  @param[in, out] DataSize        On input - size of data to read, on output - size of read data
  @param[in]      Flags           Flags
  @param[out]     *Buffer         Pointer to the data buffer

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
  @retval EFI_ABORTED             Cannot allocate memory
  @retval EFI_BUFFER_TOO_SMALL    Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
ServerHeciReadFileMsg (
  IN CONST CHAR8 *FilePath,
  IN UINT32      Offset,
  IN OUT UINT32  *DataSize,
  IN UINT8       Flags,
  OUT UINT8      *DataBuffer
  )
{
  EFI_STATUS              Status = EFI_UNSUPPORTED;
#if ME_SUPPORT_FLAG
  UINT32                  Length;
  UINT32                  RecvLength;
  READ_FILE_BUFFER        *ReadFile;
  UINT32                  MeMode;
  HECI_CONTROL            *Heci;
  UINT8                   GroupId;
  UINT8                   MeClientAddress;

  Status = gBS->LocateProtocol (
                  &gHeciControlProtocolGuid,
                  NULL,
                  (VOID**) &Heci
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = MeBiosGetMeMode (&MeMode);
  if (EFI_ERROR (Status) || (MeMode != ME_MODE_NORMAL)) {
    return EFI_UNSUPPORTED;
  }


  Length     = sizeof (READ_FILE);
  RecvLength = sizeof (READ_FILE_ACK) + *DataSize;

  ReadFile = AllocateZeroPool (MAX (RecvLength,Length));

  if (ReadFile == NULL) {
    DEBUG ((DEBUG_ERROR, "HeciReadFileMsg Error: Could not allocate Memory\n"));
    return EFI_ABORTED;
  }

  GetMcaMsgParams (MCA_READ_FILE_CMD, &GroupId, &MeClientAddress);

  ReadFile->Request.MkhiHeader.Fields.GroupId = GroupId;
  ReadFile->Request.MkhiHeader.Fields.Command = MCA_READ_FILE_CMD;
  ReadFile->Request.Offset                    = Offset;
  ReadFile->Request.DataSize                  = *DataSize;
  ReadFile->Request.Flags                     = Flags;

  ASSERT (AsciiStrLen (FilePath) <= (sizeof (ReadFile->Request.FilePath)));
  CopyMem (ReadFile->Request.FilePath, FilePath, AsciiStrLen (FilePath));

  Status = Heci->HeciSendAndReceive (
                   Heci,
                   HECI_DEFAULT_PCH_ID,
                   NULL,
                   HECI_DEFAULT_RETRY_NUMBER,
                   BIOS_FIXED_HOST_ADDR,
                   MeClientAddress,
                   (UINT32*) &ReadFile->Request,
                   Length,
                   (UINT32*) &ReadFile->Response,
                   &RecvLength,
                   NULL,
                   NULL
                   );
  if (!EFI_ERROR (Status)) {
    if (ReadFile->Response.MkhiHeader.Fields.Command == MCA_READ_FILE_CMD &&
       (ReadFile->Response.MkhiHeader.Fields.IsResponse == 1) &&
       (ReadFile->Response.MkhiHeader.Fields.Result == MkhiStatusSuccess)) {
      CopyMem (DataBuffer, ReadFile->Response.Data, ReadFile->Response.DataSize);
    } else {
      Status = EFI_DEVICE_ERROR;
    }
  }

  FreePool (ReadFile);
#endif // ME_SUPPORT_FLAG

  return Status;
}

/**
  Read Delayed Authentication Mode from ME NVM

  @param[out] DamState            Delayed Authentication Mode State

  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
  @retval EFI_ABORTED             Cannot allocate memory
  @retval EFI_BUFFER_TOO_SMALL    Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
ServerHeciGetDamState (
  OUT UINT8  *DamState
  )
{
  EFI_STATUS Status = EFI_UNSUPPORTED;
#if ME_SUPPORT_FLAG
  UINT32 DataSize;

  DEBUG ((DEBUG_INFO, "[ME] HeciGetDamState()\n"));
  if (DamState == NULL) {
    DEBUG ((DEBUG_ERROR, "[ME] ERROR: HeciGetDamState() invalid input parameters\n"));
    return EFI_INVALID_PARAMETER;
  }

  *DamState = 0;

  if (MeTypeIsDfx () || MeIsInRecovery ()) {
    DEBUG ((DEBUG_ERROR, "[ME] WARNING: Exit - Me is Dfx or in recovery\n"));
    return EFI_UNSUPPORTED;
  }

  DataSize = MCA_DAM_FILE_SIZE;
  Status = ServerHeciReadFileMsg (ME_DAM_FILE, 0, &DataSize, 0, DamState);

  DEBUG ((DEBUG_INFO, "[ME] HeciGetDamState(): %r\n", Status));
#endif // ME_SUPPORT_FLAG

  return Status;
}

/**
  This message is sent by the BIOS to save data in a file stored in ME firmware NVM.

  @param[in]   FilePath           File path of file to write
  @param[in]   Offset             File offset
  @param[in]   DataSize           Size of data to write
  @param[in]   Flags              Flags
  @param[in]   Buffer             Pointer to the data buffer

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
  @retval EFI_ABORTED             Cannot allocate memory
  @retval EFI_BUFFER_TOO_SMALL    Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
ServerHeciSetFileMsg (
  IN CONST CHAR8  *FilePath,
  IN UINT32       Offset,
  IN UINT32       DataSize,
  IN UINT8        Flags,
  IN UINT8        *DataBuffer
  )
{
  EFI_STATUS               Status = EFI_UNSUPPORTED;
#if ME_SUPPORT_FLAG
  UINT32                   Length;
  UINT32                   RecvLength;
  SET_FILE_BUFFER          *SetFile;
  UINT32                   MeMode;
  HECI_CONTROL             *Heci;
  UINT8                    GroupId;
  UINT8                    MeClientAddress;

  Status = gBS->LocateProtocol (
                  &gHeciControlProtocolGuid,
                  NULL,
                  (VOID**) &Heci
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = MeBiosGetMeMode (&MeMode);
  if (EFI_ERROR (Status) || (MeMode != ME_MODE_NORMAL)) {
    return EFI_UNSUPPORTED;
  }

  Length     = sizeof (SET_FILE) + DataSize;
  RecvLength = sizeof (SET_FILE_ACK);

  SetFile = AllocateZeroPool (MAX (RecvLength,Length));
  if (SetFile == NULL) {
    DEBUG ((DEBUG_ERROR, "HeciSetFileMsg Error: Could not allocate Memory\n"));
    return EFI_ABORTED;
  }

  GetMcaMsgParams (MCA_SET_FILE_CMD, &GroupId, &MeClientAddress);

  SetFile->Request.MkhiHeader.Fields.GroupId = GroupId;
  SetFile->Request.MkhiHeader.Fields.Command = MCA_SET_FILE_CMD;
  SetFile->Request.Offset                    = Offset;
  SetFile->Request.DataSize                  = DataSize;
  SetFile->Request.Flags                     = Flags;

  CopyMem (SetFile->Request.Data, DataBuffer, DataSize);

  ASSERT (AsciiStrLen (FilePath) <= (sizeof (SetFile->Request.FilePath)));
  CopyMem (SetFile->Request.FilePath, FilePath, AsciiStrLen (FilePath));

  Status = Heci->HeciSendAndReceive (
                   Heci,
                   HECI_DEFAULT_PCH_ID,
                   NULL,
                   HECI_DEFAULT_RETRY_NUMBER,
                   BIOS_FIXED_HOST_ADDR,
                   MeClientAddress,
                   (UINT32*) &SetFile->Request,
                   Length,
                   (UINT32*) &SetFile->Response,
                   &RecvLength,
                   NULL,
                   NULL
                   );
  if (SetFile->Response.MkhiHeader.Fields.Result != MkhiStatusSuccess) {
    Status = EFI_DEVICE_ERROR;
  }

  FreePool (SetFile);
#endif // ME_SUPPORT_FLAG

  return Status;
}

/**
  This message is sent by the BIOS after HeciSetFileMsg to notify ME firmware that it needs to
  commit previously set files. Caller shold invoke global reset afterwards.

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL    Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
ServerHeciCommitFilesMsg (
  VOID
  )
{
  EFI_STATUS             Status = EFI_UNSUPPORTED;
#if ME_SUPPORT_FLAG
  UINT32                 Length;
  UINT32                 RecvLength;
  COMMIT_FILES           CommitFiles;
  COMMIT_FILES           CommitFilesAck;
  UINT32                 MeMode;
  HECI_CONTROL           *Heci;
  UINT8                  GroupId;
  UINT8                  MeClientAddress;

  Status = gBS->LocateProtocol (
                  &gHeciControlProtocolGuid,
                  NULL,
                  (VOID**) &Heci
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = MeBiosGetMeMode (&MeMode);
  if (EFI_ERROR (Status) || (MeMode != ME_MODE_NORMAL)) {
    return EFI_UNSUPPORTED;
  }

  GetMcaMsgParams (MCA_COMMIT_FILES_CMD, &GroupId, &MeClientAddress);

  CommitFiles.MkhiHeader.Data           = 0;
  CommitFiles.MkhiHeader.Fields.GroupId = GroupId;
  CommitFiles.MkhiHeader.Fields.Command = MCA_COMMIT_FILES_CMD;
  Length                                = sizeof (COMMIT_FILES);
  RecvLength                            = sizeof (COMMIT_FILES);

  Status = Heci->HeciSendAndReceive (
                   Heci,
                   HECI_DEFAULT_PCH_ID,
                   NULL,
                   HECI_DEFAULT_RETRY_NUMBER,
                   BIOS_FIXED_HOST_ADDR,
                   MeClientAddress,
                   (UINT32*) &CommitFiles,
                   Length,
                   (UINT32*) &CommitFilesAck,
                   &RecvLength,
                   NULL,
                   NULL
                   );
  if (CommitFilesAck.MkhiHeader.Fields.Result != MkhiStatusSuccess) {
    Status = EFI_DEVICE_ERROR;
  }
#endif // ME_SUPPORT_FLAG

  return Status;
}

/**
  Set Delayed Authentication Mode in ME NVM

  @param[in] DamState             New Delayed Authentication Mode State:
                                    0 - DAM Disable
                                    1 - DAM Enable

  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
  @retval EFI_ABORTED             Cannot allocate memory
  @retval EFI_BUFFER_TOO_SMALL    Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
ServerHeciSetDamState (
  IN UINT8 DamState
  )
{
  EFI_STATUS Status = EFI_UNSUPPORTED;

#if ME_SUPPORT_FLAG
  DEBUG ((DEBUG_INFO, "[ME] HeciSetDamState(%d)\n", DamState));
  if (MeTypeIsDfx () || MeIsInRecovery ()) {
    DEBUG ((DEBUG_ERROR, "[ME] WARNING: Exit - Me is Dfx or in recovery\n"));
    return EFI_UNSUPPORTED;
  }

  Status = ServerHeciSetFileMsg (ME_DAM_FILE, 0, MCA_DAM_FILE_SIZE, 0, &DamState);
  if (!EFI_ERROR (Status)) {
    // CommitFiles
    Status = ServerHeciCommitFilesMsg ();
  }

  DEBUG ((DEBUG_INFO, "[ME] HeciSetDamState(): %r\n", Status));
#endif // ME_SUPPORT_FLAG

  return Status;
}

