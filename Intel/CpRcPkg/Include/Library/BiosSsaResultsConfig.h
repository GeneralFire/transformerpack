/** @file
  BiosSsaResultsConfig.h

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

#ifndef __BIOS_SSA_RESULTS_CONFIG__
#define __BIOS_SSA_RESULTS_CONFIG__

#pragma pack (push, 1)

/**
  Function allocates a buffer to collect results data.
  It is made up of a block of metadata of MetadataSize and a block of results
  elements which is ResultsElementSize * ResultsCapacity in size.

  @param[in, out] This              - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[in]      MetadataSize      - Declares the size of the result buffers metadata block.
  @param[in]      ResultElementSize - Declares the size of an individual result element.
  @param[in]      ResultsCapacity   - Declares the number of elements to store results in.

  @retval NotAvailable - There is insufficient memory to create the result buffer.
  @retval Success - Buffer is valid
**/
SSA_STATUS
EFIAPI
BiosCreateResultBuffer (
  IN OUT  SSA_BIOS_SERVICES_PPI *This,
  IN      UINT32                MetadataSize,
  IN      UINT32                ResultElementSize,
  IN      UINT32                ResultsCapacity
  );

/**
  Function increases the size of a existing result buffer.

  It increases the capacity of the existing result buffer by allocating
  an increased size of result buffer, then copies the previous result back
  to the new buffer.

  @param[in, out] This              - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[in]      AddCapacity       - The number of element space to add to results.

  @retval Success, NotAvailable.
**/
SSA_STATUS
EFIAPI
BiosIncreaseResultBuffer (
  IN OUT  SSA_BIOS_SERVICES_PPI *This,
  IN      INT32                  AddCapacity
  );

/**
  Function sets the type of the metadata.

  @param[in, out] This     - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[in]      Revision - Incremented when a backwards compatible binary change is made to the results format.

  @retval NotAvailable -  A results buffer has not been created.
  @retval Success - The results buffer has been created.
**/
SSA_STATUS
EFIAPI
BiosSetRevision (
  IN OUT  SSA_BIOS_SERVICES_PPI *This,
  IN      UINT32                Revision
  );

/**
  Function gets the type of the metadata.

  @param[in, out] This     - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[out]     Revision - Incremented when a backwards compatible binary change is made to the results format.

  @retval UnsupportedValue - Revision is NULL.
  @retval NotAvailable - A results buffer has not been created.
  @retval Success - metadata is valid.
**/
SSA_STATUS
EFIAPI
BiosGetRevision (
  IN OUT  SSA_BIOS_SERVICES_PPI *This,
  OUT     UINT32                **Revision
  );

/**
  Function sets the GUID associated with the metadata.

  @param[in, out] This         - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[in, out] MetadataType - Address to an EFI_GUID representing a specific metadata type.

  @retval UnsupportedValue -MetadataType is NULL or invalid.
  @retval NotAvailable - A results buffer has not been created.
  @retval Success - GUID set.
**/
SSA_STATUS
EFIAPI
BiosSetMetadataType (
  IN OUT  SSA_BIOS_SERVICES_PPI *This,
  IN OUT  EFI_GUID              *MetadataType
  );

/**
  Function gets the GUID associated with the metadata.

  @param[in, out] This         - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[out]     MetadataType - Address to an EFI_GUID representing a specific metadata type.

  @retval UnsupportedValue - MetadataType is NULL.
  @retval NotAvailable - A results buffer has not been created.
  @retval Success - GUID associated with the metadata is valid.
**/
SSA_STATUS
EFIAPI
BiosGetMetadataType (
  IN OUT  SSA_BIOS_SERVICES_PPI *This,
  OUT     EFI_GUID              **MetadataType
  );

/**
  Function returns the size of the metadata block.

  @param[in, out] This         - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[out]     MetadataSize - The address to the UINT32 in the results data buffer that holds the size.

  @retval UnsupportedValue - MetadataSize is NULL.
  @retval NotAvailable - a results buffer has not been created.
  @retval Success - MetadataSize is valid.
**/
SSA_STATUS
EFIAPI
BiosGetMetadataSize (
  IN OUT  SSA_BIOS_SERVICES_PPI *This,
  OUT     UINT32                **MetadataSize
  );

/**
  Function returns the address to the metadata contained in the results data buffer.

  @param[in, out] This     - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[out]     Metadata - The address to the metadata contained in the results data buffer.

  @retval UnsupportedValue - Metadata is NULL.
  @retval NotAvailable - a results buffer has not been created.
  @retval Success - Metadata is valid.
**/
SSA_STATUS
EFIAPI
BiosGetMetadata (
  IN OUT  SSA_BIOS_SERVICES_PPI *This,
  OUT     VOID                  **Metadata
  );

/**
  Function sets the GUID associated with result elements.

  @param[in, out] This       - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[in]      ResultType - The address to an EFI_GUID representing a specific result type.

  @retval UnsupportedValue - ResultType is NULL or invalid.
  @retval NotAvailable - a results buffer has not been created.
  @retval Success - ResultType is set.
**/
SSA_STATUS
EFIAPI
BiosSetResultElementType (
  IN OUT  SSA_BIOS_SERVICES_PPI *This,
  IN      EFI_GUID              *ResultType
  );

/**
  Function gets the GUID associated with result elements.

  @param[in, out] This       - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[out]     ResultType - The address to an EFI_GUID representing a specific result type.

  @retval UnsupportedValue - ResultType is NULL.
  @retval NotAvailable - a results buffer has not been created.
  @retval Success - ResultType is valid.
**/
SSA_STATUS
EFIAPI
BiosGetResultElementType (
  IN OUT  SSA_BIOS_SERVICES_PPI *This,
  OUT     EFI_GUID              **ResultType
  );

/**
  Function returns the size a result element.

  @param[in, out] This       - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[out]     ResultSize - Returns the address to the UINT32 in the results data buffer that holds the size.

  @retval UnsupportedValue - ResultSize is NULL.
  @retval NotAvailable - a results buffer has not been created.
  @retval Success - ResultSize if valid
**/
SSA_STATUS
EFIAPI
BiosGetResultElementSize (
  IN OUT  SSA_BIOS_SERVICES_PPI *This,
  OUT     UINT32                **ResultSize
  );

/**
  Function returns how many elements can be stored in the results data buffer.

  @param[in, out] This           - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[out]     ResultCapacity - Returns the address to the UINT32 in the results data buffer that holds the capacity.

  @retval UnsupportedValue - ResultCapacity is NULL.
  @retval NotAvailable - a results buffer has not been created.
  @retval Success - ResultCapacity is valid.
**/
SSA_STATUS
EFIAPI
BiosGetResultElementCapacity (
  IN OUT  SSA_BIOS_SERVICES_PPI *This,
  OUT     UINT32                **ResultCapacity
  );

/**
  Function returns how many elements are currently stored in the results data buffer.

  @param[in, out] This        - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[out]     ResultCount - Returns the address to the UINT32 in the results data buffer that holds the current element count.

  @retval UnsupportedValue - ResultCount is NULL.
  @retval NotAvailable - a results buffer has not been created.
  @retval Success - ResultCount is valid.
**/
SSA_STATUS
EFIAPI
BiosGetResultElementCount (
  IN OUT  SSA_BIOS_SERVICES_PPI *This,
  OUT     UINT32                **ResultCount
  );

/**
  Function returns the next available result element in the results data buffer.
  While in automatic transfer mode every time a new element is requested the current element count is checked.
  If the buffer is full the entire block is transferred back to the Host and the buffer count is reset.
  Note: results remaining in the buffer will be finalized (transferred, etc) either by the loader when the test completes
  or by CreateResultsBuffer() before it frees any preexisting buffer.

  @param[in, out] This          - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[out]     ResultElement - Returns the address to next available results element in the results data buffer.

  @retval UnsupportedValue - ResultElement is NULL.
  @retval NotAvailable - a results buffer has not been created or there are no results elements available.
  @retval Success - ResultElement is valid and has data.
**/
SSA_STATUS
EFIAPI
BiosGetNextResultElement (
  IN OUT  SSA_BIOS_SERVICES_PPI *This,
  OUT     VOID                  **ResultElement
  );

/**
  The function toggles the explicit mode either on or off.

  @param[in, out] This - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[in]      Mode - Toggle explicit mode on (TRUE) or off (FALSE).

  @retval NotAvailable - a results buffer has not been created.
  @retval Success - Explicit mode set according to the Mode parameter.
**/
SSA_STATUS
EFIAPI
BiosSetExplicitTransferMode (
  IN OUT  SSA_BIOS_SERVICES_PPI *This,
  IN      BOOLEAN               Mode
  );

/**
  The function returns the current explicit mode value.

  @param[in, out] This - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.
  @param[out]     Mode - Returns the current explicit mode, on (TRUE) or off (FALSE).

  @retval UnsupportedValue - Mode is NULL.
  @retval NotAvailable - a results buffer has not been created.
  @retval Success - Mode is valid.
**/
SSA_STATUS
EFIAPI
BiosGetExplicitTransferMode (
  IN OUT  SSA_BIOS_SERVICES_PPI *This,
  OUT     BOOLEAN               *Mode
  );

/**
  The function flushes the used portion of the results buffer and resets the results data index.
  (Used in explicit transfer mode only.) Note: results remaining in the buffer will be finalized
  (transferred, etc) either by the loader when the test completes or by CreateResultsBuffer()
  before it frees any pre-existing buffer.

  @param[in, out] This - Interface pointer that implements the particular SSA_BIOS_SERVICES_PPI instance.

  @retval NotAvailable - a results buffer has not been created or the explicit transfer mode is FALSE.
  @retval Success - Push completed successfully.
**/
SSA_STATUS
EFIAPI
BiosPushResults (
  IN OUT  SSA_BIOS_SERVICES_PPI *This
  );

#pragma pack (pop)
#endif // __BIOS_SSA_RESULTS_CONFIG__

// end file BiosSsaResultsConfig.h
