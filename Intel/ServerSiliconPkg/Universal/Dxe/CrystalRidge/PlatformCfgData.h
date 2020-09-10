/** @file

  @copyright
  INTEL CONFIDENTIAL
Copyright (c) 2014-2018, Intel Corporation.

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

#ifndef _PLATFORMCFGDATA_H_
#define _PLATFORMCFGDATA_H_

#define NGN_LARGE_PAYLOAD_IN    0
#define NGN_LARGE_PAYLOAD_OUT   1
#define NGN_INT_BUFFER_SIZE     1024

#define NGN_NOT_POPULATED       0xFF

#define NGN_PCD_MAX_CFG         3

#define NGN_PCD_TOTAL_SIZE      0x20000
#define NGN_PCD_OEM_START       0x10000
#define NGN_PCD_OEM_SIZE        (NGN_PCD_TOTAL_SIZE - NGN_PCD_OEM_START)
#define NGN_PCD_GEN_SIZE        (NGN_PCD_TOTAL_SIZE - NGN_PCD_OEM_SIZE)

/*++

  Update the platform configuration data for each NGN DIMM

  @param None

  @retval EFI_SUCCESS - Operation completed successfully,
                        error code otherwise

++*/
EFI_STATUS
UpdatePcd (
  VOID
  );

/*++

  Updates the platform configuration data stored in the BIOS partition for each NGN DIMM

  @param[in] Skt              - Socket number
  @param[in] Ch               - Channel number
  @param[in] Dimm             - DIMM number
  @param[in] CfgCurBuffer     - CfgCur record buffer
  @param[in] CfgCurRecLength  - CfgCur record length

  @retval EFI_SUCCESS         - Operation completed successfully,
                                error code otherwise

++*/
EFI_STATUS
UpdatePcdBiosPartition (
  IN UINT8  Skt,
  IN UINT8  Ch,
  IN UINT8  Dimm,
  IN UINT8  *CfgCurRecBuffer,
  IN UINT32 CfgCurRecLength
  );

/*++

  Update the platform configuration data stored in the OS partiton for each NGN DIMM

  @param[in] Skt                - Socket number
  @param[in] Ch                 - Channel number
  @param[in] Dimm               - DIMM number
  @param[in] OutputPayloadValid - Specifies whether Output Payload has valid data
  @param[in] CfgCurRecBuffer    - CfgCur record buffer
  @param[in] CfgCurRecLength    - CfgCur record length
  @param[in] CfgOutRecBuffer    - CfgOut record buffer
  @param[in] CfgOutRecLength    - CfgOut record length

  @retval EFI_SUCCESS           - Operation completed successfully,
                                  error code otherwise

++*/
EFI_STATUS
UpdatePcdOsPartition (
  IN UINT8   Skt,
  IN UINT8   Ch,
  IN UINT8   Dimm,
  IN BOOLEAN OutputPayloadValid,
  IN UINT8   *CfgCurRecBuffer,
  IN UINT32  CfgCurRecLength,
  IN UINT8   *CfgOutRecBuffer,
  IN UINT32  CfgOutRecLength
  );

/*++

  Writes the data in the large payload region to the specified partiton on the NGN DIMM

  @param[in] Skt         - Socket number
  @param[in] Ch          - Channel number
  @param[in] Dimm        - DIMM number
  @param[in] PartitionId - Specified partition

  @retval EFI_SUCCESS    - Operation completed successfully,
                           error code otherwise

++*/
EFI_STATUS
SetPCD (
  IN UINT8 Skt,
  IN UINT8 Ch,
  IN UINT8 Dimm,
  IN UINT8 PartitionId
  );

/*++

  Reads the data from the specified partition on the NGN DIMM into the large payload region

  @param[in]  Skt                 - Socket number
  @param[in]  Ch                  - Channel number
  @param[in]  Dimm                - DIMM number
  @param[in]  PartitionId         - Specified partition
  @param[in]  Types               - PCD configuration types map (CCUR/COUT/CIN/OEM)
  @param[out] Valid               - Specifies whether Output Payload contains valid data from the PCD

  @retval EFI_SUCCESS             - Operation completed successfully,
                                    error code otherwise

++*/
EFI_STATUS
GetPCD (
  IN  UINT8   Skt,
  IN  UINT8   Ch,
  IN  UINT8   Dimm,
  IN  UINT8   PartitionId,
  IN  UINT8   Types,
  OUT BOOLEAN *Valid
  );

/*++

  Checks whether PCD partition is healthy or not

  @param[in] Skt  - Socket number
  @param[in] Ch   - Channel number
  @param[in] Dimm - DIMM number
  @param[in] Type - PCD partition ID

  @retval TRUE    - PCD partition is healthy,
  @retval FALSE   - PCD partition is not healthy

++*/
BOOLEAN
IsPcdPartitonHealthy (
  IN UINT8 Skt,
  IN UINT8 Ch,
  IN UINT8 Dimm,
  IN UINT8 PartitionId
  );

/**
  This routine is converts BIOS Partition Failure Reason to CCUR configuration status code

  @param[in] BiosPartitonFailureReason   - BIOS Partition Failure Reason

  @retval CCUR configuration status code

**/
UINT16
GetCfgCurStatusForBiosPartitonFailure (
  IN UINT8 BiosPartitonFailureReason
  );

/*++

  Creates the CCUR record for OS partition update when BIOS partition failure was detected.

  @param[in]  Index           - Index to CfgCurRecord record containing the CfgCur data
  @param[out] CfgRecord       - Buffer allocated by function to store the CfgCur record
  @param[out] CfgRecordLength - Length of allocated buffer

  @retval None

++*/
VOID
CreateCfgCurRecordForBiosPartitonFailure (
  IN  UINT8   Index,
  OUT UINT8   **CfgRecord,
  OUT UINT32  *CfgRecordLength
  );

/**

  Updates the CCUR record for OS Partition update.

  @param[in] CfgRecord       - Buffer with CfgCur record data

  @retval None

++*/
VOID
UpdateCfgCurRecordForOsPartitionUpdate (
  IN UINT8   *CfgRecord
  );

/**

  Creates the CCUR record from the MemMapHost data.

  @param[in]  Index           - Index to CfgCurRecord record containing the CfgCur data
  @param[out] CfgRecord       - Buffer allocated by function to store the CfgCur record
  @param[out] CfgRecordLength - Length of allocated buffer

  @retval None

++*/
VOID
CreateCfgCurRecordFromHostData (
  IN  UINT8   Index,
  OUT UINT8   **CfgRecord,
  OUT UINT32  *CfgRecordLength
  );

/**

  Creates the COUT record from the MemMapHost data.

  @param[in] Index            - Index to CfgOutRecord record containing the CfgOut data
  @param[out] CfgRecord       - Buffer allocated by function to store the CfgOut record
  @param[out] CfgRecordLength - Length of allocated buffer

  @retval None

**/
VOID
CreateCfgOutRecordFromHostData (
  IN  UINT8   Index,
  OUT UINT8   **CfgRecord,
  OUT UINT32  *CfgRecordLength
  );

/**

  Loads Configuration Records for updating the platform configuration data for each NGN DIMM and populate the record indices.

  @param None

  @retval EFI_SUCCESS - Operation completed successfully, error code otherwise

**/
EFI_STATUS
LoadCfgRecordsForPCDUpdates (
  VOID
  );

/*++

  Copy data from the SMM large payload output region to a local buffer

  @param[in]  SrcSkt         - Source socket
  @param[in]  SrcCh          - Source channel
  @param[in]  SrcDimm        - Source dimm
  @param[in]  SrcOffset      - Source offset in the large payload region
  @param[out] DstBuffer      - Pointer to destination buffer.
  @param[in]  DstOffset      - Destination offset in the local buffer
  @param[in]  Length         - Number of bytes to copy
  @param[in]  LargePayloadRegion - If is = LARGE_PAYLOAD_INPUT, copies from Input Payload Region, if is = LARGE_PAYLOAD_OUTPUT copies from Output Payload Region

  @retval EFI_INVALID_PARAMETER - Destination Buffer is null
  @retval EFI_NOT_FOUND         - DIMM not found
  @retval EFI_SUCCESS           - Success

++*/
EFI_STATUS
CopyFromLargePayloadToBuffer (
  IN  UINT8   SrcSkt,
  IN  UINT8   SrcCh,
  IN  UINT8   SrcDimm,
  IN  UINT32  SrcOffset,
  OUT UINT8  *DstBuffer,
  IN  UINT32  DstOffset,
  IN  UINT32  Length,
  IN  BOOLEAN LargePayloadRegion
  );

/*++

  Copy data from a local buffer to SMM large payload region

  @param[in] srcBuffer            - Source pointer to local buffer
  @param[in] srcOffset            - Source offset in the local buffer
  @param[in] dstSkt               - Destination socket
  @param[in] dstCh                - Destination channel
  @param[in] dstDimm              - Destination dimm
  @param[in] dstOffset            - Destination offset in the large payload region
  @param[in] length               - Number of bytes to copy
  @param[in] largePayloadRegion   - If it is = LARGE_PAYLOAD_INPUT, copies to the Large Input Payload, else if = LARGE_PAYLOAD_OUTPUT, copies buffer
                                    to the Large Output Payload

  @retval EFI_NOT_FOUND           - DIMM not found
  @retval EFI_SUCCESS             - Success

++*/
EFI_STATUS
CopyFromBufferToLargePayload (
  IN UINT8   *srcBuffer,
  IN UINT32  srcOffset,
  IN UINT8   dstSkt,
  IN UINT8   dstCh,
  IN UINT8   dstDimm,
  IN UINT32  dstOffset,
  IN UINT32  length,
  IN BOOLEAN largePayloadRegion
  );

/*++

  Copy data from SMM large payload region to SMM large payload region

  @param[in] srcSkt       - Source socket
  @param[in] srcCh        - Source channel
  @param[in] srcDimm      - Source dimm
  @param[in] srcOffset    - Source offset in the large payload region
  @param[in] dstSkt       - Destination socket
  @param[in] dstCh        - Destination channel
  @param[in] dstDimm      - Destination dimm
  @param[in] dstOffset    - Destination offset in the large payload region
  @param[in] length       - Number of bytes to copy

  @retval EFI_NOT_FOUND   - DIMM not found
  @retval EFI_SUCCESS     - Success

++*/
EFI_STATUS
CopyLPOutputToLPInput (
  IN UINT8   SrcSkt,
  IN UINT8   SrcCh,
  IN UINT8   SrcDimm,
  IN UINT32  SrcOffset,
  IN UINT8   DstSkt,
  IN UINT8   DstCh,
  IN UINT8   DstDimm,
  IN UINT32  DstOffset,
  IN UINT32  Length
  );

/*++

  Compute byte checksum of a buffer

  @param[in]  srcBuffer         - Pointer to local buffer
  @param[in]  length            - Number of bytes to checksum
  @param[out] chksum            - Computed checksum

  @retval EFI_INVALID_PARAMETER - Invalid parameter
  @retval EFI_SUCCESS           - Success

++*/
EFI_STATUS
ComputeChecksum (
  IN  UINT8   *srcBuffer,
  IN  UINT32  length,
  OUT UINT8   *chksum
  );

/*++

  Displays a buffer of a specified length

  @param[in] DisplayType  - Display Type, DEBUG_INFO or DEBUG_ERROR
  @param[in] Buffer       - Pointer to local buffer
  @param[in] Length       - Number of bytes to display

  @retval EFI_SUCCESS - Success

++*/
EFI_STATUS
DisplayBuffer (
  IN UINT32 DisplayType,
  IN UINT8  *Buffer,
  IN UINT32 Length
  );
#endif // _PLATFORMCFGDATA_H_
