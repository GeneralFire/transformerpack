/** @file

  Compressed Variable Library DXE

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation. <BR>

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

#include <PiDxe.h>

#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/PrintLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/HobLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/CompressLib.h>
#include <Library/CompressedVariableLib.h>

#include <Protocol/VariableLock.h>

#include "CompressedVariableLibInternal.h"

/**
  Dump variable contents to the serial log for debugging.
  May drastically increase the size of the serial log depending
  on the size of the variable, use with caution.

  @param[in] Variable        The data to dump the log.
  @param[in] VariableSize    The size of the data.

  @retval None

**/

VOID
DumpDecompressedVariable (
  IN VOID *Variable,
  IN UINTN VariableSize
  )
{
  UINTN i;
  UINT8 *Ptr;

  DEBUG ((EFI_D_ERROR, "DumpDecompressedVariable DXE:\n\n"));

  Ptr = (UINT8 *)Variable;

  for (i = 0; i < VariableSize; i++) {
    DEBUG ((EFI_D_ERROR, "0x%2x ", *(Ptr + i)));

    if (i % 16 == 0) {
      DEBUG ((EFI_D_ERROR, "\n"));
    }
  }

  DEBUG ((EFI_D_ERROR, "\n\n"));

  return;

} // DumpDecompressedVariable

/**
  Dump variable names to the serial log for debugging.
  May drastically increase the size of the serial log depending
  on the size of the variable, use with caution.

  @param  None

  @retval None

**/

VOID
DumpVariableNames (
  VOID
  )
{
  EFI_STATUS Status;
  CHAR16 *VariableName = NULL;
  UINTN VariableNameSize = 1024;
  EFI_GUID Guid;

  VariableName = AllocatePool (VariableNameSize);
  if (VariableName == NULL) {
    DEBUG ((EFI_D_ERROR,"DumpVariableNames: AllocatePool failed on CompressedVariableSize\n"));
    ASSERT (FALSE);
    return;
  }

  ZeroMem (VariableName, 1024);

  while (TRUE) {

    VariableNameSize = 1024;

    Status = gRT->GetNextVariableName (&VariableNameSize, VariableName, &Guid);
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "DumpVariableNames: GetNextVariableName failed, Status = %r\n", Status));
      break;
    }

    DEBUG ((EFI_D_ERROR,"DumpVariableNames: This variable = %s\n", VariableName));

  }

  return;

} // DumpVariableNames

/**
  Save data to Nvram.

  @param[in] VariableName        Name of the variable to load.
  @param[in] VariableGuid        GUID of the variable to load.
  @param[in] VariableBuffer      Ptr to the buffer to load the variable to.
  @param[in] VariableBufferSize  Size of the buffer in bytes.

  @retval EFI_SUCCESS           The variable was saved successfully.
  @retval !EFI_SUCCESS          Failure.

**/

EFI_STATUS
SaveToVariable (
  IN CHAR16 *VariableName,
  IN EFI_GUID VariableGuid,
  IN VOID *VariableBuffer,
  IN UINT32 VariableBufferSize
  )
{
  EFI_STATUS Status;
  CHAR16 PaddedVariableName[MAX_VARIABLE_NAME_LENGTH];
  UINTN VariableNameLength = 0;
  UINT8 *OffsetPtr = NULL;
  UINT32 BytesRemaining = 0;
  EDKII_VARIABLE_LOCK_PROTOCOL *VariableLock = NULL;

  COMPRESSED_VAR_INFO ("Saving variable with size = %d\n", VariableBufferSize);

  Status = gBS->LocateProtocol (&gEdkiiVariableLockProtocolGuid, NULL, (VOID **)&VariableLock);
  if (EFI_ERROR (Status)) {
    COMPRESSED_VAR_ERROR ("LocateProtocol gEdkiiVariableLockProtocolGuid failed, Status = %r\n", Status);
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  VariableNameLength = StrnLenS (VariableName, MAX_VARIABLE_NAME_LENGTH);

  ZeroMem (PaddedVariableName, MAX_VARIABLE_NAME_LENGTH);

  Status = StrnCpyS (PaddedVariableName, MAX_VARIABLE_NAME_LENGTH, VariableName, VariableNameLength);

  PaddedVariableName[VariableNameLength] = L'0';

  OffsetPtr = VariableBuffer;
  BytesRemaining = VariableBufferSize;

  while (BytesRemaining > 0) {

    UINT32 SizeToSave = 0;

    if (BytesRemaining >  VARIABLE_SPLIT_SIZE) {
      SizeToSave = VARIABLE_SPLIT_SIZE;
    } else {
      SizeToSave = BytesRemaining;
    }

    COMPRESSED_VAR_INFO ("Top of loop, Saving %s, guid = %g, size of this var = %d\n", PaddedVariableName, VariableGuid, SizeToSave);

    Status = EFI_NOT_FOUND;
    Status = gRT->SetVariable (
                    PaddedVariableName,
                    &VariableGuid,
                    EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
                    SizeToSave,
                    OffsetPtr
                    );
    if (EFI_ERROR (Status)) {
      COMPRESSED_VAR_ERROR ("SetVariable failed, Status = %r\n", Status);
      ASSERT (FALSE);
      return Status;
    }

    //
    // Lock the variable
    //

    Status = VariableLock->RequestToLock (VariableLock, PaddedVariableName, &VariableGuid);
    if (EFI_ERROR (Status)) {
      COMPRESSED_VAR_ERROR ("RequestToLock failed, Status = %r\n", Status);
      ASSERT (FALSE);
      return Status;
    }

    //
    // Increment variable name
    //

    PaddedVariableName[VariableNameLength]++;

    BytesRemaining -= SizeToSave;
    OffsetPtr += SizeToSave;

  }

  COMPRESSED_VAR_INFO ("Exiting successfully\n");

  return EFI_SUCCESS;

} // SaveToVariable

/**
  Compress and save data to Nvram.

  @param[in]  VariableName        Name of the variable to load.
  @param[in]  VariableGuid        GUID of the variable to load.
  @param[in]  VariableBuffer      Ptr to the buffer to load the variable to.
  @param[in]  VariableBufferSize  Size of the buffer in bytes.

  @retval EFI_SUCCESS           The variable was saved successfully.
  @retval !EFI_SUCCESS          Failure.

**/

EFI_STATUS
EFIAPI
CompressAndSaveToVariable (
  IN CHAR16 *VariableName,
  IN EFI_GUID VariableGuid,
  IN VOID *VariableBuffer,
  IN UINT32 VariableBufferSize
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  VOID  *CompressedVariableData = NULL;
  UINTN CompressedBufferSize = 0;

  COMPRESSED_VAR_INFO ("Entered, saving data of size %d\n", VariableBufferSize);

  Status = Compress (VariableBuffer, VariableBufferSize, CompressedVariableData, &CompressedBufferSize);
  if (Status != EFI_BUFFER_TOO_SMALL) {

    //
    // We call compress with size == 0 in order to get the required size. Any other error is a problem.
    //

    COMPRESSED_VAR_ERROR ("Compress failed unexpectedly, Status = %r\n", Status);
    return Status;
  }

  //
  // We're in DXE, so we can use AllocatePool because FreePool
  // works fine.
  //

  CompressedVariableData = AllocatePool (CompressedBufferSize);
  if (CompressedVariableData == NULL) {
    COMPRESSED_VAR_ERROR ("AllocatePool failed\n");
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // Now do the real compress.
  //

  Status = Compress (VariableBuffer, VariableBufferSize, CompressedVariableData, &CompressedBufferSize);
  if (EFI_ERROR (Status)) {
    COMPRESSED_VAR_ERROR ("Compress failed, Status = %r\n", Status);
    FreePool (CompressedVariableData);
    return Status;
  }

  COMPRESSED_VAR_INFO ("Saving variable with size = %d\n", CompressedBufferSize);

  Status = SaveToVariable (VariableName, VariableGuid, CompressedVariableData, (UINT32)CompressedBufferSize);
  if (EFI_ERROR (Status)) {
    COMPRESSED_VAR_ERROR ("SaveToVariable failed, Status = %r\n", Status);
    FreePool (CompressedVariableData);
    return Status;
  }

  COMPRESSED_VAR_INFO ("Exiting\n");

  return Status;

} // CompressAndSaveToVariable

/**
  Retrieve data from a HOB(s), then compress and save the data.

  @param[in]  HobGuid        GUID of the HOB to save.
  @param[in]  VariableName   Name of the variable to save as.
  @param[in]  VariableGuid   GUID of the variable to save as.

  @retval EFI_SUCCESS           The variable was saved successfully.
  @retval !EFI_SUCCESS          Failure.

**/

EFI_STATUS
EFIAPI
SaveVariableFromHob (
  IN EFI_GUID HobGuid,
  IN CHAR16 *VariableName,
  IN EFI_GUID VariableGuid
  )
{
  EFI_STATUS Status;
  UINT32 TotalSize = 0;
  VOID *DataPtr = NULL;
  UINTN CurrentOffset = 0;
  UINT8 *CurrentOffsetPtr = NULL;
  EFI_HOB_GUID_TYPE *CurrentHob = NULL;
  UINT16 CurrentHobSize = 0;
  VOID *CurrentHobData = NULL;

  COMPRESSED_VAR_INFO ("Entered, trying to save %s\n", VariableName);

  //
  // First get the total size of all the HOBs with this GUID
  //

  CurrentHob = GetFirstGuidHob (&HobGuid);
  if (CurrentHob == NULL) {
    COMPRESSED_VAR_ERROR ("GetFirstGuidHob failed on guid %g\n", HobGuid);
    return EFI_NOT_FOUND;
  }

  while (TRUE) {

    COMPRESSED_VAR_INFO ("SaveVariableFromHob: size of this HOB = %d\n", (UINT32)GET_GUID_HOB_DATA_SIZE (CurrentHob));

    TotalSize += (UINT32)GET_GUID_HOB_DATA_SIZE (CurrentHob);

    //
    // First increment past the current HOB in the list of all HOBs. Then find the next
    // HOB of type HobGuid, this works even if we are already on the next HOB of
    // type HobGuid.
    //

    CurrentHob = GET_NEXT_HOB (CurrentHob);
    CurrentHob = GetNextGuidHob (&HobGuid, CurrentHob);

    //
    // If the HOB is NULL, we're at the end of the list of HOBs
    // with this GUID.
    //

    if (CurrentHob == NULL) {
      break;
    }

  }

  COMPRESSED_VAR_INFO ("SaveVariableFromHob: TotalSize = %d\n", TotalSize);

  //
  // Allocate a buffer big enough to hold all the HOB data.
  //

  DataPtr = AllocatePool (TotalSize);
  if (DataPtr == NULL) {
    COMPRESSED_VAR_ERROR ("SaveVariableFromHob: AllocatePool failed\n");
    ASSERT (FALSE);
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // Set our offset to the beginning of the allocated buffer.
  //

  CurrentOffset = 0;
  CurrentOffsetPtr = (UINT8 *)DataPtr;

  CurrentHob = GetFirstGuidHob (&HobGuid);
  ASSERT (CurrentHob != NULL);

  //
  // Copy all the HOB data into our allocated buffer.
  //

  while (TRUE) {

    CurrentHobData = GET_GUID_HOB_DATA (CurrentHob);
    CurrentHobSize = GET_GUID_HOB_DATA_SIZE (CurrentHob);

    CopyMem (CurrentOffsetPtr + CurrentOffset, CurrentHobData, CurrentHobSize);

    //
    // Update our offset
    //

    CurrentOffset += CurrentHobSize;

    //
    // Get the next HOB with this GUID.
    //

    CurrentHob = GET_NEXT_HOB (CurrentHob);
    CurrentHob = GetNextGuidHob (&HobGuid, CurrentHob);
    if (CurrentHob == NULL) {
      break;
    }

  }

  //
  // Now compress the data and save it.
  //

  Status = CompressAndSaveToVariable (VariableName,
              VariableGuid,
              DataPtr,
              TotalSize
              );
  if (EFI_ERROR (Status)) {
    COMPRESSED_VAR_ERROR ("CompressAndSaveToVariable failed, status = %r\n", Status);
    ASSERT (FALSE);
    FreePool (DataPtr);
    return Status;
  }

  FreePool (DataPtr);

  COMPRESSED_VAR_INFO ("Exiting\n");

  return Status;

} // SaveVariableFromHob

/**
  Load and decompress a variable from Nvram.

  @param[in]  VariableName        Name of the variable to load.
  @param[in]  VariableGuid        GUID of the variable to load.
  @param[out] VariableBuffer      Ptr to the buffer to load the variable to.
  @param[in]  VariableBufferSize  Size of the buffer in bytes.

  @retval EFI_SUCCESS           The variable was loaded and decompressed successfully.
  @retval EFI_NOT_FOUND         The variable was not found.
  @retval EFI_BUFFER_TOO_SMALL  The buffer was not large enough to hold the decompressed variable.

**/

EFI_STATUS
EFIAPI
LoadCompressedVariable (
  IN CHAR16 *VariableName,
  IN EFI_GUID VariableGuid,
  IN OUT VOID *VariableBuffer,
  IN UINT32 VariableBufferSize
  )
{

  //
  // Not implemented in DXE. Could be, but we don't have
  // a use case for it at the moment.
  //

  ASSERT (FALSE);

  return EFI_NOT_FOUND;

} // LoadCompressedVariable

/**
  Load a variable from Nvram.

  @param[in]  VariableName        Name of the variable to load.
  @param[in]  VariableGuid        GUID of the variable to load.
  @param[out] VariableBuffer      Ptr to the buffer to load the variable to.
  @param[in]  VariableBufferSize  Size of the buffer in bytes.

  @retval EFI_SUCCESS           The variable was loaded and successfully.
  @retval EFI_NOT_FOUND         The variable was not found.
  @retval EFI_BUFFER_TOO_SMALL  The buffer was not large enough to hold the variable.

**/

EFI_STATUS
EFIAPI
LoadUncompressedVariable (
  IN CHAR16 *VariableName,
  IN EFI_GUID VariableGuid,
  IN OUT VOID *VariableBuffer,
  IN UINT32 VariableBufferSize
  )
{

  //
  // Not implemented in DXE. Could be, but we don't have
  // a use case for it at the moment.
  //

  ASSERT (FALSE);

  return EFI_NOT_FOUND;

} // LoadUncompressedVariable

