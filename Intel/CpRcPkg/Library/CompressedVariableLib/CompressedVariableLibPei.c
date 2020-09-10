/** @file

  Compressed Variable Library PEI

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

#include <PiPei.h>

#include <Library/PeiServicesLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/RcMemAllocLib.h>
#include <Library/DebugLib.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <Library/UefiDecompressLib.h>
#include <Library/PrintLib.h>
#include <Library/CompressedVariableLib.h>

#include <Ppi/ReadOnlyVariable2.h>

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

  DEBUG ((EFI_D_ERROR, "DumpDecompressedVariable PEI:\n\n"));

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

  @param[in] VariablePpi  - Ptr to the Variable PPI

  @retval None

**/

VOID
DumpVariableNames (
  IN EFI_PEI_READ_ONLY_VARIABLE2_PPI *VariablePpi
  )
{
  EFI_STATUS Status;
  CHAR16 *VariableName = NULL;
  UINTN VariableNameSize = 1024;
  EFI_GUID Guid;

  VariableName = RcAllocatePool (VariableNameSize);
  if (VariableName == NULL) {
    DEBUG ((EFI_D_ERROR,"DumpVariableNames: RcAllocatePool failed on CompressedVariableSize\n"));
    ASSERT (FALSE);
    return;
  }

  ZeroMem (VariableName, 1024);

  while (TRUE) {

    VariableNameSize = 1024;

    Status = VariablePpi->NextVariableName (
              VariablePpi,
              &VariableNameSize,
              VariableName,
              &Guid
              );
    if (EFI_ERROR (Status)) {
      DEBUG ((EFI_D_ERROR, "DumpVariableNames: NextVariableName failed, Status = %r\n", Status));
      break;
    }

    DEBUG ((EFI_D_ERROR,"DumpVariableNames: This variable = %s\n", VariableName));

  }

  return;

} // DumpVariableNames

/**
  Allocate memory for the internal variable name, initialize it
  with the caller's variable name and append 0.

  Note that is is the caller's responsibility to free InternalVariableName
  when done with it.

  @param[in]  VariableName          Name of the variable.
  @param[out] InternalVariableName  Ptr to the internal variable name.
  @param[out] VariableNameLength    Ptr to the length of the variable name.

  @retval EFI_SUCCESS           The variable was loaded and decompressed successfully.
  @retval EFI_OUT_OF_RESOURCES  Could not allocate memory.

**/

EFI_STATUS
SetupInternalVariableName (
  IN CHAR16 *VariableName,
  OUT CHAR16 **InternalVariableName,
  OUT UINT32 *VariableNameLength
  )
{
  EFI_STATUS Status;
  CHAR16 *LocalBuffer = NULL;
  UINT32 NameLength = 0;

  LocalBuffer = RcAllocatePool (MAX_VARIABLE_NAME_LENGTH);
  if (LocalBuffer == NULL) {
    COMPRESSED_VAR_ERROR ("RcAllocatePool failed on CompressedVariableSize\n");
    ASSERT (FALSE);
    Status = EFI_OUT_OF_RESOURCES;
    return Status;
  }

  NameLength = StrnLenS (VariableName, MAX_VARIABLE_NAME_LENGTH);

  ZeroMem (LocalBuffer, MAX_VARIABLE_NAME_LENGTH);

  Status = StrnCpyS (LocalBuffer, MAX_VARIABLE_NAME_LENGTH, VariableName, NameLength);
  if (EFI_ERROR(Status)) {
    COMPRESSED_VAR_ERROR ("StrnCpyS failed, Status = %r\n", Status);
    RcFreePool (LocalBuffer);
    return Status;
  }

  LocalBuffer[NameLength] = L'0';

  COMPRESSED_VAR_INFO ("LocalBuffer = %s\n", LocalBuffer);

  *VariableNameLength = NameLength;
  *InternalVariableName = LocalBuffer;

  COMPRESSED_VAR_INFO ("*InternalVariableName = %s\n", *InternalVariableName);

  return EFI_SUCCESS;

} // SetupInternalVariableName

/**
  Get the size of the data for a variable.

  Note that is is the caller's responsibility to free InternalVariableName
  when done with it.

  @param[in] VariablePpi  - Ptr to the Variable PPI
  @param[in]  VariableName          Name of the variable.
  @param[out] InternalVariableName  Ptr to the internal variable name.
  @param[out] VariableNameLength    Ptr to the length of the variable name.

  @retval EFI_SUCCESS           The variable was loaded and decompressed successfully.
  @retval EFI_OUT_OF_RESOURCES  Could not allocate memory.

**/

EFI_STATUS
GetVariableDataSize (
  IN EFI_PEI_READ_ONLY_VARIABLE2_PPI *VariablePpi,
  IN CHAR16 *VariableName,
  IN EFI_GUID VariableGuid,
  OUT UINT32 *VariableDataSize
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  UINTN VariableAttributes = 0;
  CHAR16 *InternalVariableName = NULL;
  UINT32 VariableNameLength = 0;
  UINT8 *ThisVariableBuffer = NULL;
  BOOLEAN FoundVariable = FALSE;

  COMPRESSED_VAR_INFO ("Entered\n");

  Status = SetupInternalVariableName (VariableName, &InternalVariableName, &VariableNameLength);
  if (EFI_ERROR (Status)) {
    COMPRESSED_VAR_ERROR ("SetupInternalVariableName failed, Status = %r\n", Status);
    return Status;
  }

  ThisVariableBuffer = RcAllocatePool (VARIABLE_SPLIT_SIZE);
  if (ThisVariableBuffer == NULL) {
    COMPRESSED_VAR_ERROR ("RcAllocatePool ThisVariableBuffer failed\n");
    Status = EFI_OUT_OF_RESOURCES;
    return Status;
  }

  *VariableDataSize = 0;

  VariableAttributes = EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS;

  while (TRUE) {

    UINT32 ThisVariableSize = VARIABLE_SPLIT_SIZE;

    Status = VariablePpi->GetVariable (
              VariablePpi,
              InternalVariableName,
              &VariableGuid,
              (UINT32 *)&VariableAttributes,
              &ThisVariableSize,
              ThisVariableBuffer
              );
    if (EFI_ERROR (Status)) {
      COMPRESSED_VAR_ERROR ("GetVariable failed, Status = %r\n", Status);
      goto End;
    }

    //
    // This loop will eventually fail no matter what. If it fails on the
    // first iteration, we need to return failure, but not if it fails on
    // subsequent interations, so track if we found any variable or not.
    //

    FoundVariable = TRUE;

    //
    // Update the total size
    //

    *VariableDataSize += ThisVariableSize;

    //
    // Increment variable name. If the variable is very large, it is possible
    // to go past VariableName9, but this still works since both save and load
    // still set the next variable name to the same thing. Unlikely that there
    // would be a variable large enough to hit this condition, but not really a
    // problem anyway.
    //

    InternalVariableName[VariableNameLength]++;

  }

End:

  if (ThisVariableBuffer != NULL) {
    RcFreePool (ThisVariableBuffer);
  }

  if (InternalVariableName != NULL) {
    RcFreePool (InternalVariableName);
  }

  if (FoundVariable == FALSE) {
    COMPRESSED_VAR_ERROR ("Did not find requested variable\n");
    return Status;
  }

  return EFI_SUCCESS;

} // GetVariableDataSize

/**
  Load a variable from Nvram.

  Variables have a maximum size that is smaller than many common
  use cases. Because of that this function and the corresponding
  save function split the data into chunks smaller than the maximum
  variable size, and save those chunks individually.

  We use VARIABLE_SPLIT_SIZE as the value to split data at. This value
  has been empirically determined to work over time, since we cannot reliably
  determine the actual maximum variable size at build time.

  Each chunk will be given a name VariableNameX, where X is an integer
  starting at 0. Caller does not need to comprehend this behavior, as long
  as the corresponding save function in this library is used.

  @param[in] VariablePpi          Ptr to the Variable PPI
  @param[in]  VariableName        Name of the variable to load.
  @param[in]  VariableGuid        GUID of the variable to load.
  @param[out] VariableData        Address of a ptr that contains the variable data.
  @param[out] VariableDataSize    Ptr to the size of the variable data in bytes.

  @retval EFI_SUCCESS           The variable was loaded successfully.
  @retval EFI_NOT_FOUND         The variable was not found.

**/

EFI_STATUS
LoadVariable (
  IN EFI_PEI_READ_ONLY_VARIABLE2_PPI *VariablePpi,
  IN CHAR16 *VariableName,
  IN EFI_GUID VariableGuid,
  OUT UINT8 **VariableData,
  OUT UINT32 *VariableDataSize
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  UINTN VariableAttributes = 0;
  UINT32 BytesRemaining = 0;
  CHAR16 *InternalVariableName;
  UINTN VariableNameLength = 0;
  UINT8 *ThisVariableBuffer = NULL;
  UINT8 *OffsetPtr = NULL;

  COMPRESSED_VAR_INFO ("Entered\n");

  *VariableDataSize = 0;

  Status = GetVariableDataSize (VariablePpi, VariableName, VariableGuid, VariableDataSize);
  if (EFI_ERROR (Status)) {
    COMPRESSED_VAR_ERROR ("GetVariableDataSize failed, Status = %r\n", Status);
    return Status;
  }

  *VariableData = RcAllocatePool (*VariableDataSize);
  if (*VariableData == NULL) {
    COMPRESSED_VAR_ERROR ("RcAllocatePool *VariableData failed\n");
    Status = EFI_OUT_OF_RESOURCES;
    goto End;
  }

  Status = SetupInternalVariableName (VariableName, &InternalVariableName, &VariableNameLength);
  if (EFI_ERROR (Status)) {
    COMPRESSED_VAR_ERROR ("SetupInternalVariableName failed, Status = %r\n", Status);
    return Status;
  }

  //
  // Initialize our pointer and set the bytes remaining to
  // the size of the caller's buffer.
  //

  OffsetPtr = *VariableData;
  BytesRemaining = *VariableDataSize;

  VariableAttributes = EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS;

  ThisVariableBuffer = RcAllocatePool (VARIABLE_SPLIT_SIZE);
  if (ThisVariableBuffer == NULL) {
    COMPRESSED_VAR_ERROR ("RcAllocatePool ThisVariableBuffer failed\n");
    Status = EFI_OUT_OF_RESOURCES;
    goto End;
  }

  while (BytesRemaining > 0) {

    UINT32 VariableSize = VARIABLE_SPLIT_SIZE;

    ZeroMem (ThisVariableBuffer, VARIABLE_SPLIT_SIZE);

    Status = VariablePpi->GetVariable (
              VariablePpi,
              InternalVariableName,
              &VariableGuid,
              (UINT32 *)&VariableAttributes,
              &VariableSize,
              ThisVariableBuffer
              );
    if (EFI_ERROR (Status)) {
      COMPRESSED_VAR_ERROR ("GetVariable failed, Status = %r\n", Status);
      goto End;
    }

    if (VariableSize < BytesRemaining) {

      CopyMem (OffsetPtr, ThisVariableBuffer, VariableSize);

      BytesRemaining -= VariableSize;
      OffsetPtr += VariableSize;

    } else {

      CopyMem (OffsetPtr, ThisVariableBuffer, BytesRemaining);

      BytesRemaining = 0;

      break;
    }

    //
    // Increment variable name. If the variable is very large, it is possible
    // to go past VariableName9, but this still works since both save and load
    // still set the next variable name to the same thing. Unlikely that there
    // would be a variable large enough to hit this condition, but not really a
    // problem anyway.
    //

    InternalVariableName[VariableNameLength]++;

  }

End:

  if (ThisVariableBuffer != NULL) {
    RcFreePool (ThisVariableBuffer);
  }

  if (InternalVariableName != NULL) {
    RcFreePool (InternalVariableName);
  }

  COMPRESSED_VAR_INFO ("Exiting with Status = %r\n", Status);

  return Status;

} // LoadVariable

/**
  Compress and save data to Nvram.

  @param[in] VariableName        Name of the variable to load.
  @param[in] VariableGuid        GUID of the variable to load.
  @param[in] VariableBuffer      Ptr to the buffer to load the variable to.
  @param[in] VariableBufferSize  Size of the buffer in bytes.

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
  //
  // Cannot save variables in PEI, so this should not be called
  //

  ASSERT (FALSE);

  return EFI_UNSUPPORTED;

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
  //
  // Cannot save variables in PEI, so this should not be called
  //

  ASSERT (FALSE);

  return EFI_UNSUPPORTED;

}

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
  EFI_STATUS Status;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI *VariablePpi = NULL;
  UINT8 *CompressedData = NULL;
  UINT32 CompressedDataSize = 0;
  UINT8 *DecompressedData = NULL;
  UINT32 DecompressedDataSize = 0;
  UINT8  *ScratchBuffer = NULL;
  UINT32 RequiredScratchBufferSize = 0;

  COMPRESSED_VAR_INFO ("Entered\n");

  Status = PeiServicesLocatePpi (
                &gEfiPeiReadOnlyVariable2PpiGuid,
                0,
                NULL,
                &VariablePpi
                );
  if (EFI_ERROR (Status)) {
    COMPRESSED_VAR_ERROR ("PeiServicesLocatePpi failed, Status = %r\n", Status);
    return Status;
  }

  Status = LoadVariable (VariablePpi, VariableName, VariableGuid, &CompressedData, &CompressedDataSize);
  if (EFI_ERROR (Status)) {
    COMPRESSED_VAR_ERROR ("LoadVariable failed, Status = %r\n", Status);
    return Status;
  }

  //
  // We now have all the compressed data in CompressedData. Get the
  // size of the data after it's decompressed.
  //

  Status = UefiDecompressGetInfo (CompressedData, CompressedDataSize, &DecompressedDataSize, &RequiredScratchBufferSize);
  if (EFI_ERROR (Status)) {
    COMPRESSED_VAR_ERROR ("UefiDecompressGetInfo failed, Status = %r\n", Status);
    ASSERT_EFI_ERROR (Status);
    goto End;
  }

  //
  // When HOBs are created via BuildGuidDataHob, the size of the data will always be in
  // multiples of 8 bytes (64bits), and the extra data is padded with zeros. This will only be
  // a problem on the last HOB, the previous HOB's will always split on the correct boundary. The
  // rounded up size is preserved in the compressed variable. We can assume that if the caller
  // provided buffer is smaller by less than 8 bytes that the rest is padding and
  // can be dropped without issue.
  //

  if (VariableBufferSize + 7 < DecompressedDataSize) {
    COMPRESSED_VAR_ERROR ("VariableBufferSize too small, caller size = %d, required size = %d\n", VariableBufferSize, DecompressedDataSize);
    ASSERT (FALSE);
    Status = EFI_BUFFER_TOO_SMALL;
    goto End;
  }

  ScratchBuffer = RcAllocatePool (RequiredScratchBufferSize);
  if (ScratchBuffer == NULL) {
    COMPRESSED_VAR_ERROR ("RcAllocatePool failed on ScratchBuffer\n");
    ASSERT (FALSE);
    Status = EFI_OUT_OF_RESOURCES;
    goto End;
  }

  //
  // We decompress to a temporary buffer since the caller buffer may be slightly smaller
  // than the decompressed variable due to the padding.
  //

  DecompressedData = RcAllocatePool (DecompressedDataSize);
  if (DecompressedData == NULL) {
    COMPRESSED_VAR_ERROR ("RcAllocatePool failed on DecompressedData\n");
    Status = EFI_OUT_OF_RESOURCES;
    goto End;
  }

  ZeroMem (DecompressedData, DecompressedDataSize);

  Status = UefiDecompress (CompressedData, DecompressedData, ScratchBuffer);
  if (EFI_ERROR (Status)) {
    COMPRESSED_VAR_ERROR ("UefiDecompress failed, Status = %r\n", Status);
    goto End;
  }

  //
  // Only copy up to the size of the provided variable, not the size
  // of the padded DecompressedData.
  //

  CopyMem (VariableBuffer, DecompressedData, VariableBufferSize);

End:

  if (ScratchBuffer != NULL) {
    RcFreePool (ScratchBuffer);
  }

  if (CompressedData != NULL) {
    RcFreePool (CompressedData);
  }

  if (DecompressedData != NULL) {
    RcFreePool (DecompressedData);
  }

  COMPRESSED_VAR_INFO ("Exiting with Status = %r\n", Status);

  return Status;

} // LoadCompressedVariable

/**
  Load an uncompressed variable from Nvram.

  @param[in]  VariableName        Name of the variable to load.
  @param[in]  VariableGuid        GUID of the variable to load.
  @param[out] VariableBuffer      Ptr to the buffer to load the variable to.
  @param[in]  VariableBufferSize  Size of the buffer in bytes.

  @retval EFI_SUCCESS           The variable was loaded and successfully.
  @retval EFI_NOT_FOUND         The variable was not found.

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
  EFI_STATUS Status;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI *VariablePpi = NULL;
  UINT8 *VariableData = NULL;
  UINT32 VariableDataSize = 0;

  COMPRESSED_VAR_INFO ("Entered\n");

  Status = PeiServicesLocatePpi (
                &gEfiPeiReadOnlyVariable2PpiGuid,
                0,
                NULL,
                &VariablePpi
                );
  if (EFI_ERROR (Status)) {
    COMPRESSED_VAR_ERROR ("PeiServicesLocatePpi failed, Status = %r\n", Status);
    return Status;
  }

  Status = LoadVariable (VariablePpi, VariableName, VariableGuid, &VariableData, &VariableDataSize);
  if (EFI_ERROR (Status)) {
    COMPRESSED_VAR_ERROR ("LoadVariable failed, Status = %r\n", Status);
    return Status;
  }

  //
  // When HOBs are created via BuildGuidDataHob, the size of the data will always be in
  // multiples of 8 bytes (64bits), and the extra data is padded with zeros. This will only be
  // a problem on the last HOB, the previous HOB's will always split on the correct boundary. The
  // rounded up size is preserved in the saved variable. We can assume that if the caller
  // provided buffer is smaller by less than 8 bytes that the rest is padding and
  // can be dropped without issue.
  //

  if (VariableBufferSize + 7 < VariableDataSize) {
    COMPRESSED_VAR_ERROR ("VariableBufferSize too small, caller size = %d, required size = %d\n", VariableBufferSize, VariableDataSize);
    Status = EFI_BUFFER_TOO_SMALL;
    goto End;
  }

  //
  // Only copy up to the size of the provided buffer, not the size
  // of the saved variable.
  //

  CopyMem (VariableBuffer, VariableData, VariableBufferSize);

End:

  if (VariableData != NULL) {
    RcFreePool (VariableData);
  }

  COMPRESSED_VAR_INFO ("Exiting with Status = %r\n", Status);

  return Status;

} // LoadUncompressedVariable






