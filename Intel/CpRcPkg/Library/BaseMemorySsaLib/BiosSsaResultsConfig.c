/** @file
  This file contains the BIOS implementation of the BIOS-SSA Results Configuration API.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2014 - 2020 Intel Corporation. <BR>

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

#include <Library/MemoryCoreLib.h>
#include <Ppi/SsaBiosServicesPpi.h>
#include <Library/BaseMemoryLib.h>

#ifndef __GNUC__
#pragma warning (disable : 4366)
#endif  //__GNUC__


/**
Function checks the GUID.

@param[in, out] This        - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
@param[in]      Guid        - The address to the given EFI_GUID.

@retval Success, UnsupportedValue.
**/
static
SSA_STATUS
EfiGuidCheck (
  IN OUT  SSA_BIOS_SERVICES_PPI *This,
  IN      EFI_GUID              *Guid
  )
{
  SSA_STATUS            Status;
  UINT8                 Check;
  UINT8                 Index;

  Status = UnsupportedValue;
  if (Guid != NULL) {
    for (Index = 0, Check = 0; Index < sizeof (EFI_GUID); Index++) {
      Check |= ((UINT8 *) Guid)[Index];
    }

    if (Check != 0) {
      Status = Success;
    }
  }

  return Status;
}

/**
Function checks and sets the GUID.

@param[in, out] This        - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
@param[in]      Source      - The address to the given EFI_GUID.
@param[in, out] Destination - Where to save the given EFI_GUID if it is valid.

@retval Success, UnsupportedValue.
**/
static
SSA_STATUS
EfiGuidCheckAndSave (
  IN OUT  SSA_BIOS_SERVICES_PPI *This,
  IN      EFI_GUID              *Source,
  IN OUT  EFI_GUID              *Destination
  )
{
  SSA_STATUS            Status;

  Status = UnsupportedValue;
  if (Destination != NULL) {
    Status = EfiGuidCheck (This, Source);
    if (Status == Success) {
      CopyMem ((UINT8 *) Destination, (UINT8 *) Source, sizeof (EFI_GUID));
    }
  }

  return Status;
}

/**
  Function allocates a buffer to collect results data.
  It is made up of a block of metadata of MetadataSize and a block of results
  elements which is ResultsElementSize * ResultsCapacity in size.

  @param[in, out] This              - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[in]      MetadataSize      - Declares the size of the result buffers metadata block.
  @param[in]      ResultElementSize - Declares the size of an individual result element.
  @param[in]      ResultsCapacity   - Declares the number of elements to store results in.

  @retval Success, UnsupportedValue, NotAvailable.
**/
SSA_STATUS
EFIAPI
BiosCreateResultBuffer (
  IN OUT  SSA_BIOS_SERVICES_PPI *This,
  IN      UINT32                MetadataSize,
  IN      UINT32                ResultElementSize,
  IN      UINT32                ResultsCapacity
  )
{
  SSA_STATUS        Status = UnsupportedValue;
  PSYSHOST          Host;
  RESULTS_DATA_HDR  *ResultsDataHdr;
  UINT32            ResultsDataSize;

  Host = NULL;

  if ((MetadataSize != 0) || (ResultElementSize != 0) || (ResultsCapacity != 0)) {
    if (This->SsaResultsConfig->ResultsData != NULL) {
      This->SsaCommonConfig->Free (This, This->SsaResultsConfig->ResultsData);
    }
    ResultsDataSize                    = sizeof (RESULTS_DATA_HDR) + MetadataSize + (ResultElementSize * ResultsCapacity);

  //If results for one BSSA test run are more than the max size of a HOB, we don't save to BDAT
  if (ResultsDataSize > MAX_HOB_ENTRY_SIZE) {
      This->SsaResultsConfig->ResultsData = NULL;
  } else {
      This->SsaResultsConfig->ResultsData = This->SsaCommonConfig->Malloc (This, ResultsDataSize);
  }

    if (This->SsaResultsConfig->ResultsData != NULL) {

      ZeroMem ((UINT8*)This->SsaResultsConfig->ResultsData, ResultsDataSize);

      ResultsDataHdr                            = (RESULTS_DATA_HDR *) This->SsaResultsConfig->ResultsData;
      ResultsDataHdr->MdBlock.MetadataStart     = (VOID *) ((UINTN) ResultsDataHdr + sizeof (RESULTS_DATA_HDR));
      ResultsDataHdr->MdBlock.MetadataSize      = MetadataSize;
      ResultsDataHdr->RsBlock.ResultStart       = (VOID *) ((UINTN) (ResultsDataHdr->MdBlock.MetadataStart) + MetadataSize);
      ResultsDataHdr->RsBlock.ResultElementSize = ResultElementSize;
      ResultsDataHdr->RsBlock.ResultCapacity    = ResultsCapacity;
      Status = Success;
    } else {
      Status = NotAvailable;
    }
  }

  return Status;
}


/**
  Function increases the size of a existing result buffer.

  It increases the capacity of the existing result buffer by allocating
  an increased size of result buffer, then copy the previous result back
  to the new buffer.

  If the exsiting buffer has leftover space that are more than the requested add capacity,
  then don't create a new one.

  If not able to create the requested increased size buffer, return NotAvailable but keep 
  the original buffer.

  @param[in, out] This              - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[in]      AddCapacity       - The number of element space to need to add to results.

  @retval Success, NotAvailable.
**/
SSA_STATUS
EFIAPI
BiosIncreaseResultBuffer (
  IN OUT  SSA_BIOS_SERVICES_PPI *This,
  IN      INT32                  AddCapacity
  )
{
  SSA_STATUS        Status = UnsupportedValue;
  RESULTS_DATA_HDR  *ResultsDataHdr;
  UINT32            ResultsDataSize;
  UINT8             *PreviousResultsData;
  UINT32            MetadataSize;
  UINT32            ResultElementSize;
  UINT32            PreviousResultsDataSize;
  INT32             PreviousResultsCapacity;
  INT32             PreviousResultsElementCount;
  INT32             ResultsCapacity;

  if (This->SsaResultsConfig->ResultsData == NULL) {
    return NotAvailable;
  }

  ResultsDataHdr = (RESULTS_DATA_HDR *)This->SsaResultsConfig->ResultsData;
  PreviousResultsCapacity = ResultsDataHdr->RsBlock.ResultCapacity;
  PreviousResultsElementCount = ResultsDataHdr->RsBlock.ResultElementCount;

  //
  // If previous result buffer still has enough space for the "AddCapacity", then no need to expand it and just returns.
  //
  if ((PreviousResultsCapacity - PreviousResultsElementCount) >= AddCapacity) {
    return Success;
  }

  PreviousResultsData = ((UINT8 *)This->SsaResultsConfig->ResultsData);
  ResultElementSize = ResultsDataHdr->RsBlock.ResultElementSize;
  MetadataSize = ResultsDataHdr->MdBlock.MetadataSize;
  PreviousResultsDataSize = sizeof (RESULTS_DATA_HDR) + MetadataSize + (ResultElementSize * PreviousResultsCapacity);
  ResultsCapacity = PreviousResultsCapacity + AddCapacity;
  ResultsDataSize = sizeof (RESULTS_DATA_HDR) + MetadataSize + (ResultElementSize * ResultsCapacity);

  if (ResultsDataSize > MAX_HOB_ENTRY_SIZE) {
    //
    // The new requested data buffer is too big. Do not create it. Just return. The old buffer is kept.
    //
    return NotAvailable;
  } else {
    This->SsaResultsConfig->ResultsData = This->SsaCommonConfig->Malloc (This, ResultsDataSize);
  }

  if (This->SsaResultsConfig->ResultsData != NULL) {

    ZeroMem ((UINT8*)This->SsaResultsConfig->ResultsData, ResultsDataSize);

    //
    // Copy previous result data to new buffer.
    //
    CopyMem ((UINT8*)This->SsaResultsConfig->ResultsData, PreviousResultsData, PreviousResultsDataSize);

    //
    // Update a couple fields: MetaData and Result element starting address, new capacity.
    //
    ResultsDataHdr = (RESULTS_DATA_HDR *)This->SsaResultsConfig->ResultsData;
    ResultsDataHdr->MdBlock.MetadataStart = (VOID *)((UINTN)ResultsDataHdr + sizeof (RESULTS_DATA_HDR));
    ResultsDataHdr->RsBlock.ResultStart = (VOID *)((UINTN)(ResultsDataHdr->MdBlock.MetadataStart) + MetadataSize);
    ResultsDataHdr->RsBlock.ResultCapacity = ResultsCapacity;

    //
    // Free previous results
    //
    This->SsaCommonConfig->Free (This, PreviousResultsData);

    Status = Success;
  } else {
    //
    // Fail to allocate new requested data buffer. The old buffer is kept. 
    //
    This->SsaResultsConfig->ResultsData = PreviousResultsData;
    Status = NotAvailable;
  }

  return Status;
}

/**
  Function sets the type of the metadata.

  @param[in, out] This     - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[in]      Revision - Incremented when a backwards compatible binary change is made to the results format.

  @retval Success, NotYetAvailable.
**/
SSA_STATUS
EFIAPI
BiosSetRevision (
  IN OUT  SSA_BIOS_SERVICES_PPI *This,
  IN      UINT32                Revision
  )
{
  RESULTS_DATA_HDR *ResultsDataHdr = NULL;

  if (This->SsaResultsConfig->ResultsData != NULL) {
    ResultsDataHdr = (RESULTS_DATA_HDR *) This->SsaResultsConfig->ResultsData;
    ResultsDataHdr->Revision = Revision;
    return (Success);
  }

  return NotAvailable;
}

/**
  Function gets the type of the metadata.

  @param[in, out] This     - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[in]      Revision - Incremented when a backwards compatible binary change is made to the results format.

  @retval Success, NotYetAvailable.
**/
SSA_STATUS
EFIAPI
BiosGetRevision (
  IN OUT  SSA_BIOS_SERVICES_PPI *This,
  OUT     UINT32                **Revision
  )
{
  RESULTS_DATA_HDR *ResultsDataHdr = NULL;

  if (This->SsaResultsConfig->ResultsData != NULL) {
   ResultsDataHdr = (RESULTS_DATA_HDR *) This->SsaResultsConfig->ResultsData;
   *Revision = &ResultsDataHdr->Revision;
   return (Success);
  }

  return NotAvailable;
}

/**
  Function sets the GUID associated with the metadata.

  @param[in, out] This         - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[in]      MetadataType - Address to an EFI_GUID representing a specific metadata type.

  @retval Success, UnsupportedValue, NotAvailable.
**/
SSA_STATUS
EFIAPI
BiosSetMetadataType (
  IN OUT  SSA_BIOS_SERVICES_PPI *This,
  IN OUT  EFI_GUID              *MetadataType
  )
{
  RESULTS_DATA_HDR *ResultsDataHdr = NULL;

  if (This->SsaResultsConfig->ResultsData != NULL) {
   ResultsDataHdr = (RESULTS_DATA_HDR *) This->SsaResultsConfig->ResultsData;
   return (EfiGuidCheckAndSave (This, MetadataType, &ResultsDataHdr->MdBlock.MetadataType));
  }

  return NotAvailable;
}

/**
  Function gets the GUID associated with the metadata.

  @param[in, out] This         - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[out]     MetadataType - Address to an EFI_GUID representing a specific metadata type.

  @retval Success, NotAvailable.
**/
SSA_STATUS
EFIAPI
BiosGetMetadataType (
  IN OUT  SSA_BIOS_SERVICES_PPI *This,
  OUT     EFI_GUID              **MetadataType
  )
{
  if (This->SsaResultsConfig->ResultsData != NULL) {
    *MetadataType = &(((RESULTS_DATA_HDR *) This->SsaResultsConfig->ResultsData)->MdBlock.MetadataType);
   return (Success);
  }

  return NotAvailable;
}

/**
  Function returns the size of the metadata block.

  @param[in, out] This         - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[out]     MetadataSize - The address to the UINT32 in the results data buffer that holds the size.

  @retval Success, NotAvailable.
**/
SSA_STATUS
EFIAPI
BiosGetMetadataSize (
  IN OUT  SSA_BIOS_SERVICES_PPI *This,
  OUT     UINT32                **MetadataSize
  )
{
  if (This->SsaResultsConfig->ResultsData != NULL) {
    *MetadataSize = &(((RESULTS_DATA_HDR *) This->SsaResultsConfig->ResultsData)->MdBlock.MetadataSize);
    return (Success);
  }

  return NotAvailable;
}

/**
  Function returns the address to the metadata contained in the results data buffer.

  @param[in, out] This        - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[out]     Metadata - The address to the metadata contained in the results data buffer.

  @retval Success, NotAvailable.
**/
SSA_STATUS
EFIAPI
BiosGetMetadata (
  IN OUT  SSA_BIOS_SERVICES_PPI *This,
  OUT     VOID                  **Metadata
  )
{
  if (This->SsaResultsConfig->ResultsData != NULL) {
    *Metadata = ((RESULTS_DATA_HDR *) (This->SsaResultsConfig->ResultsData))->MdBlock.MetadataStart;
    return (Success);
  }

  return NotAvailable;
}

/**
  Function sets the GUID associated with result elements.

  @param[in, out] This       - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[in]      ResultType - The address to an EFI_GUID representing a specific result type.

  @retval Success, UnsupportedValue, NotAvailable.
**/
SSA_STATUS
EFIAPI
BiosSetResultElementType (
  IN OUT  SSA_BIOS_SERVICES_PPI *This,
  IN      EFI_GUID              *ResultType
  )
{
  RESULTS_DATA_HDR *ResultsDataHdr;

  if (This->SsaResultsConfig->ResultsData != NULL) {
    ResultsDataHdr = (RESULTS_DATA_HDR *) This->SsaResultsConfig->ResultsData;
    return (EfiGuidCheckAndSave (This, ResultType, &ResultsDataHdr->RsBlock.ResultType));
  }

  return NotAvailable;
}

/**
  Function gets the GUID associated with result elements.

  @param[in, out] This       - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[out]     ResultType - The address to an EFI_GUID representing a specific result type.

  @retval Success, NotAvailable.
**/
SSA_STATUS
EFIAPI
BiosGetResultElementType (
  IN OUT  SSA_BIOS_SERVICES_PPI *This,
  OUT     EFI_GUID              **ResultType
  )
{
  if (This->SsaResultsConfig->ResultsData != NULL) {
    *ResultType = &(((RESULTS_DATA_HDR *) This->SsaResultsConfig->ResultsData)->RsBlock.ResultType);
    return (Success);
  }

  return NotAvailable;
}

/**
  Function returns the size a result element.

  @param[in, out] This       - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[out]     ResultSize - Returns the address to the UINT32 in the results data buffer that holds the size.

  @retval Success, NotAvailable.
**/
SSA_STATUS
EFIAPI
BiosGetResultElementSize (
  IN OUT  SSA_BIOS_SERVICES_PPI *This,
  OUT     UINT32                **ResultSize
  )
{
  if (This->SsaResultsConfig->ResultsData != NULL) {
   *ResultSize = &(((RESULTS_DATA_HDR *) This->SsaResultsConfig->ResultsData)->RsBlock.ResultElementSize);
   return (Success);
  }

  return NotAvailable;
}

/**
  Function returns how many elements can be stored in the results data buffer.

  @param[in, out] This           - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[out]     ResultCapacity - Returns the address to the UINT32 in the results data buffer that holds the capacity.

  @retval Success, NotAvailable.
**/
SSA_STATUS
EFIAPI
BiosGetResultElementCapacity (
  IN OUT  SSA_BIOS_SERVICES_PPI *This,
  OUT     UINT32                **ResultCapacity
  )
{
  if (This->SsaResultsConfig->ResultsData != NULL) {
    *ResultCapacity = (UINT32 *) &(((RESULTS_DATA_HDR *) This->SsaResultsConfig->ResultsData)->RsBlock.ResultCapacity);
    return (Success);
  }

  return NotAvailable;
}

/**
  Function returns how many elements are currently stored in the results data buffer.

  @param[in, out] This        - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[out]     ResultCount - Returns the address to the UINT32 in the results data buffer that holds the current element count.

  @retval Success, NotAvailable.
**/
SSA_STATUS
EFIAPI
BiosGetResultElementCount (
  IN OUT  SSA_BIOS_SERVICES_PPI *This,
  OUT     UINT32                **ResultCount
  )
{
  if (This->SsaResultsConfig->ResultsData != NULL) {
    *ResultCount = (UINT32 *) &(((RESULTS_DATA_HDR *) This->SsaResultsConfig->ResultsData)->RsBlock.ResultElementCount);
    return (Success);
  }

  return NotAvailable;
}

/**
  Function returns the next available result element in the results data buffer.
  While in automatic transfer mode every time a new element is requested the current element count is checked.
  If the buffer is full the entire block is transferred back to the Host and the buffer count is reset.
  Note: results remaining in the buffer will be finalized (transferred, etc) either by the loader when the test completes
  or by CreateResultsBuffer() before it frees any preexisting buffer.

  @param[in, out] This          - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[out]     ResultElement - Returns the address to next available results element in the results data buffer.

  @retval Success, NotAvailable.
**/
SSA_STATUS
EFIAPI
BiosGetNextResultElement (
  IN OUT  SSA_BIOS_SERVICES_PPI *This,
  OUT     VOID                  **ResultElement
  )
{
  UINT32                           ResultsStart;
  UINT32                           CurrentResult;
  RESULTS_DATA_HDR *ResultsDataHdr = NULL;

  if (This->SsaResultsConfig->ResultsData != NULL) {
    ResultsDataHdr = (RESULTS_DATA_HDR *) This->SsaResultsConfig->ResultsData;
    if (ResultsDataHdr->RsBlock.ResultElementCount < ResultsDataHdr->RsBlock.ResultCapacity) {
      ResultsStart   = (UINT32) (UINTN) (ResultsDataHdr->RsBlock.ResultStart);
      CurrentResult  = ResultsDataHdr->RsBlock.ResultElementCount * ResultsDataHdr->RsBlock.ResultElementSize;
      *ResultElement = (VOID *) (UINTN) (ResultsStart + CurrentResult);
      ResultsDataHdr->RsBlock.ResultElementCount++;
      return (Success);
    } else {
      return (NotAvailable);
    }
  }

  return NotAvailable;
}

/**
  The function toggles the explicit mode either on or off.

  @param[in, out] This - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[in]      Mode - Toggle explicit mode on (TRUE) or off (FALSE).

  @retval Success, NotAvailable.
**/
SSA_STATUS
EFIAPI
BiosSetExplicitTransferMode (
  IN OUT  SSA_BIOS_SERVICES_PPI *This,
  IN      BOOLEAN               Mode
  )
{
  if (This->SsaResultsConfig->ResultsData != NULL) {
    (((RESULTS_DATA_HDR *) This->SsaResultsConfig->ResultsData)->TransferMode) = Mode ? 1 : 0;
    return (Success);
  }

  return NotAvailable;
}

/**
  The function returns the current explicit mode value.

  @param[in, out] This - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[out]     Mode - Returns the current explicit mode, on (TRUE) or off (FALSE).

  @retval Success, NotAvailable.
**/
SSA_STATUS
EFIAPI
BiosGetExplicitTransferMode (
  IN OUT  SSA_BIOS_SERVICES_PPI *This,
  OUT     BOOLEAN               *Mode
  )
{
  if (This->SsaResultsConfig->ResultsData != NULL) {
    *Mode = ((RESULTS_DATA_HDR *) (This->SsaResultsConfig->ResultsData))->TransferMode;
    return (Success);
  }

  return NotAvailable;
}

/**
  The function flushes the used portion of the results buffer and resets the results data index.
  (Used in explicit transfer mode only.) Note: results remaining in the buffer will be finalized
  (transferred, etc) either by the loader when the test completes or by CreateResultsBuffer()
  before it frees any pre-existing buffer.

  @param[in, out] This - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.

  @retval Success, NotAvailable.
**/
SSA_STATUS
EFIAPI
BiosPushResults (
  IN OUT  SSA_BIOS_SERVICES_PPI *This
  )
{
  RESULTS_DATA_HDR *ResultsDataHdr = NULL;

  if (This->SsaResultsConfig->ResultsData != NULL) {
    ResultsDataHdr = (RESULTS_DATA_HDR *) This->SsaResultsConfig->ResultsData;
    if (ResultsDataHdr->TransferMode) {
      ResultsDataHdr->RsBlock.ResultElementCount = 0;
      return (Success);
    }
  }

  return NotAvailable;
}

// End file BiosSsaResultsConfig.c
