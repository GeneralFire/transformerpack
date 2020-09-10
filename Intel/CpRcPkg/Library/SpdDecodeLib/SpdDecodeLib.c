/** @file
  The SPD Decode Lib Common file provides common functions for the PEI, DXE, and
  SMM phases of the implmentation of SpdDecodeLib.

  This common file provides functions which are universal between PEI, DXE, and
  SMM.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 - 2020 Intel Corporation. <BR>

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

#include <MemRegs.h>
#include <MaxSocket.h>
#include <MemHostChipCommon.h>
#include <Uefi/UefiBaseType.h>
#include <SysHost.h>
#include <Library/RcDebugLib.h>
#include <Library/SpdAccessLib.h>
#include <Memory/SpdFieldIndex.h>
#include "SpdDecodeCommon.h"
#include <Library/MemoryCoreLib.h>
#include <Library/Ddr5CoreLib.h>
#include <Library/MemoryServicesLib.h>
#include <ReferenceCodeFatalErrors.h>

/**
  Performs SPD field read operations.

  @param[in]  Socket      - Socket ID
  @param[in]  Chan        - Channel on socket
  @param[in]  Dimm        - DIMM on channel
  @param[in]  FieldIndex  - SPD Field Index to be read
  @param[out] Buffer      - Pointer to buffer to store read value
  @param[out] BufLength   - Pointer to the length, in bytes, of Buffer

  @retval RETURN_SUCCESS         The read completed successfully
  @retval EFI_DEVICE_ERROR       Error configuring or during lower level library transaction
  @retval EFI_INVALID_PARAMETER  Input parameter out of bounds
  @retval EFI_NOT_FOUND          Specifed DIMM was not found
  @retval EFI_UNSUPPORTED        This Field Index is not supported
  @retval EFI_BUFFER_TOO_SMALL   Buffer Length not big enough.
**/
EFI_STATUS
EFIAPI
SpdReadField (
  IN     UINT8  Socket,
  IN     UINT8  Chan,
  IN     UINT8  Dimm,
  IN     UINT16 FieldIndex,
  IN OUT VOID   *Buffer,
  IN OUT UINT8  *BufLength
  )
{
  EFI_STATUS         Status;
  UINT8              ByteData;
  UINT8              Index;
  UINT8              StartBit = 0;  // Byte level Start Bit
  UINT8              EndBit = 7;    // Byte level End Bit
  UINT8              DataLength;
  SPD_FIELD_MAPPING  SpdFieldMapping;

  Status = EFI_SUCCESS;
  ByteData = 0xFF;

  //
  // Sanity check of incoming data
  //
  if (Buffer == NULL) {
    RcDebugPrintWithDevice (SDBG_MAX, Socket, Chan, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                            "SpdReadField Error: Invalid Parameter Buffer = NULL\n");
    return EFI_INVALID_PARAMETER;
  }

  if ((Socket >= MAX_SOCKET) || (Chan >= MAX_CH) || (Dimm >= MAX_DIMM) || (FieldIndex >= SPD_FIELD_MAX_INDEX)) {
    RcDebugPrintWithDevice (SDBG_MAX, Socket, Chan, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                            "SpdReadField Error: Invalid Parameter FieldIndex = 0x%x, *(UINT8 *)Buffer = 0x%x\n", FieldIndex, * (UINT8 *)Buffer);
    return EFI_INVALID_PARAMETER;
  }

  Status = GetSpdFieldMappingTable (FieldIndex, &SpdFieldMapping);

  if (!EFI_ERROR (Status)) {

    if (PcdGetBool (PcdMrcDebugSpdDecodeLibTrace) == TRUE) {
      RcDebugPrintWithDevice (SDBG_MAX, Socket, Chan, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                              "SpdReadField: FieldIndex = 0x%x\n", FieldIndex);
      RcDebugPrintWithDevice (SDBG_MAX, Socket, Chan, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                              "SpdReadField: SpdFieldMapping.StartByte = 0x%x; StartByteStartBit = 0x%x\n", SpdFieldMapping.StartByte, SpdFieldMapping.StartByteStartBit);
      RcDebugPrintWithDevice (SDBG_MAX, Socket, Chan, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                              "SpdReadField: SpdFieldMapping.EndByte = 0x%x; EndByteEndBit = 0x%x\n", SpdFieldMapping.EndByte, SpdFieldMapping.EndByteEndBit);
    }

    DataLength = (UINT8) (SpdFieldMapping.EndByte + 1 - SpdFieldMapping.StartByte);

    if ((SpdFieldMapping.FieldIndex == NO_SPD_FIELD_INDEX) || (SpdFieldMapping.FieldIndex != FieldIndex)) {
      RcDebugPrintWithDevice (SDBG_MAX, Socket, Chan, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                              "SpdReadField Error: Unsupported or Mismatched FieldIndex! Input FieldIndex = 0x%x; SpdFieldMapping.FieldIndex = 0x%x\n", FieldIndex, SpdFieldMapping.FieldIndex);
      return EFI_INVALID_PARAMETER;
    } else if ((SpdFieldMapping.StartByte == NO_SPD_BYTE_NUMBER) || (SpdFieldMapping.StartByteStartBit == NO_SPD_START_BIT)
               || (SpdFieldMapping.EndByte == NO_SPD_BYTE_NUMBER) || (SpdFieldMapping.EndByteEndBit == NO_SPD_END_BIT)) {
      RcDebugPrintWithDevice (SDBG_MAX, Socket, Chan, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                              "SpdReadField Error: Unsupported FieldIndex = 0x%x\n", FieldIndex);
      return EFI_UNSUPPORTED;
    } else if (*BufLength < DataLength) {
      // BufLength should not less than the DataLength.
      RcDebugPrintWithDevice (SDBG_MAX, Socket, Chan, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                              "SpdReadField Error: *BufLength = 0x%x should not less than the DataLength in SpdFieldMappingTable = 0x%x\n", *BufLength, DataLength);
      *BufLength = DataLength;
      return EFI_BUFFER_TOO_SMALL;
    }

    *BufLength = DataLength;  // Return the BufLength as the valid data length

    StartBit = SpdFieldMapping.StartByteStartBit;

    for (Index = 0; Index < DataLength; Index++) {

      Status = SpdReadByte (Socket, Chan, Dimm, SpdFieldMapping.StartByte + Index, &ByteData);

      // Merger the bit 0 to StartBit to the previous Byte if the StartBit != 0
      if ((StartBit > 0) && (Index > 0)) {
        *((UINT8 *) Buffer + Index - 1) |= (UINT8) (ByteData << (8 - StartBit));
      }

      // When it is the last Byte to read, the EndBit should be EndByteEndBit, or the EndBit should be 7.
      if (DataLength == Index + 1) {
        EndBit = SpdFieldMapping.EndByteEndBit;
      }

      // Pick up the data from StartBit to EndBit of the ByteData, using Little-Endian.
      *((UINT8 *) Buffer + Index) = (ByteData >> StartBit) & ((UINT8) (0xFF >> (7 + StartBit - EndBit)));
    }

    if (PcdGetBool (PcdMrcDebugSpdDecodeLibTrace) == TRUE) {
      for (Index = 0; Index < DataLength; Index++) {
        Status = SpdReadByte (Socket, Chan, Dimm, SpdFieldMapping.StartByte + Index, &ByteData);
        RcDebugPrintWithDevice (SDBG_MAX, Socket, Chan, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                                "SpdReadByte Byte Data: ByteData[%d] = 0x%x\n", Index, ByteData);
        RcDebugPrintWithDevice (SDBG_MAX, Socket, Chan, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                                "SpdReadField Field Data: FieldData[%d] = 0x%x\n", Index, * ((UINT8 *)Buffer + Index));
      }
    }
  }

  if (PcdGetBool (PcdMrcDebugSpdDecodeLibTrace) == TRUE) {
    RcDebugPrintWithDevice (SDBG_MAX, Socket, Chan, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                            "SpdReadField: return Status = %r\n", Status);
  }

  return Status;
}

/**
  Performs SPD field UINT32 read operations.

  @param[in]  Socket      - Socket ID
  @param[in]  Chan        - Channel on socket
  @param[in]  Dimm        - DIMM on channel
  @param[in]  FieldIndex  - SPD Field Index to be read
  @param[out] Buffer      - Pointer to buffer to store read value

  @retval RETURN_SUCCESS         The read completed successfully
  @retval EFI_DEVICE_ERROR       Error configuring or during lower level library transaction
  @retval EFI_INVALID_PARAMETER  Input parameter out of bounds
  @retval EFI_NOT_FOUND          Specifed DIMM was not found
  @retval EFI_UNSUPPORTED        This Field Index is not supported
  @retval EFI_BUFFER_TOO_SMALL   Buffer Length not big enough
**/
EFI_STATUS
EFIAPI
SpdReadField32 (
  IN     UINT8  Socket,
  IN     UINT8  Chan,
  IN     UINT8  Dimm,
  IN     UINT16 FieldIndex,
  IN OUT UINT32 *Buffer
  )
{
  EFI_STATUS         Status;
  UINT8              Length;

  Status = EFI_SUCCESS;
  Length = sizeof (UINT32);

  Status = SpdReadField (Socket, Chan, Dimm, FieldIndex, (VOID *) Buffer, &Length);

  return Status;
}

/**
  Get the SPD version.

  @param[in]  Socket      - Socket ID
  @param[in]  Chan        - Channel on socket
  @param[in]  Dimm        - DIMM on channel
  @param[out] Version     - SPD Version

  @retval EFI_SUCCESS       Successfully initialized SPD related functionality
  @retval EFI_DEVICE_ERROR  Error initializing devices
**/
EFI_STATUS
EFIAPI
SpdGetVersion (
  IN UINT8      Socket,
  IN UINT8      Chan,
  IN UINT8      Dimm,
  OUT UINT8     *Version
  )
{
  EFI_STATUS        Status = EFI_SUCCESS;
  UINT8             Length;
  UINT8             EncodingVersion;
  UINT8             AdditionsVersion;

  Length = sizeof (UINT8);
  EncodingVersion = 0;
  AdditionsVersion = 0;

  Status = SpdReadField (Socket, Chan, Dimm, SPD_INDEX_ENCODING_LEVEL, (VOID *)&EncodingVersion, &Length);
  Status = SpdReadField (Socket, Chan, Dimm, SPD_INDEX_ADDITIONS_LEVEL, (VOID *)&AdditionsVersion, &Length);

  *Version = (EncodingVersion << 4) | (AdditionsVersion & 0x0F);

  return Status;
}

/**
  Performs SPD field write operations.

  @param[in] Socket      - Socket ID
  @param[in] Chan        - Channel on socket
  @param[in] Dimm        - DIMM on channel
  @param[in] FieldIndex  - SPD Field Index to be written
  @param[in] Buffer      - Pointer to buffer to write
  @param[in] BufLength   - The length, in bytes, of Buffer

  @retval RETURN_SUCCESS         The write completed successfully.
  @retval EFI_DEVICE_ERROR       Error configuring or during lower level library transaction
  @retval EFI_INVALID_PARAMETER  Input parameter out of bounds
  @retval EFI_NOT_FOUND          Specifed DIMM was not found
  @retval EFI_UNSUPPORTED        This Field Index is not supported
**/
EFI_STATUS
EFIAPI
SpdWriteField (
  IN UINT8  Socket,
  IN UINT8  Chan,
  IN UINT8  Dimm,
  IN UINT16 FieldIndex,
  IN VOID   *Buffer,
  IN UINT8  BufLength
  )
{
  EFI_STATUS         Status;
  UINT8              ByteData;
  UINT8              NextByteData;
  UINT8              Index;
  UINT8              StartBit = 0;  // Byte level Start Bit
  UINT8              EndBit = 7;    // Byte level End Bit
  UINT8              DataLength;
  SPD_FIELD_MAPPING  SpdFieldMapping;

  Status = EFI_SUCCESS;
  ByteData = 0xFF;
  NextByteData = 0;

  //
  // Sanity check of incoming data
  //
  if (Buffer == NULL) {
    RcDebugPrintWithDevice (SDBG_MAX, Socket, Chan, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                            "SpdWriteField Error: Invalid Parameter Buffer = NULL");
    return EFI_INVALID_PARAMETER;
  }

  if ((Socket >= MAX_SOCKET) || (Chan >= MAX_CH) || (Dimm >= MAX_DIMM) || (FieldIndex >= SPD_FIELD_MAX_INDEX)) {
    RcDebugPrintWithDevice (SDBG_MAX, Socket, Chan, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                            "SpdWriteField Error: Invalid Parameter FieldIndex = 0x%x, *(UINT8 *)Buffer = 0x%x\n", FieldIndex, * (UINT8 *)Buffer);
    return EFI_INVALID_PARAMETER;
  }

  Status = GetSpdFieldMappingTable (FieldIndex, &SpdFieldMapping);

  if (!EFI_ERROR (Status)) {

    if (PcdGetBool (PcdMrcDebugSpdDecodeLibTrace) == TRUE) {
      RcDebugPrintWithDevice (SDBG_MAX, Socket, Chan, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                              "SpdWriteField: FieldIndex = 0x%x\n", FieldIndex);
      RcDebugPrintWithDevice (SDBG_MAX, Socket, Chan, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                              "SpdWriteField: SpdFieldMapping.StartByte = 0x%x; StartByteStartBit = 0x%x\n", SpdFieldMapping.StartByte, SpdFieldMapping.StartByteStartBit);
      RcDebugPrintWithDevice (SDBG_MAX, Socket, Chan, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                              "SpdWriteField: SpdFieldMapping.EndByte = 0x%x; EndByteEndBit = 0x%x\n", SpdFieldMapping.EndByte, SpdFieldMapping.EndByteEndBit);
    }

    DataLength = (UINT8) (SpdFieldMapping.EndByte + 1 - SpdFieldMapping.StartByte);

    if ((SpdFieldMapping.FieldIndex == NO_SPD_FIELD_INDEX) || (SpdFieldMapping.FieldIndex != FieldIndex)) {
      RcDebugPrintWithDevice (SDBG_MAX, Socket, Chan, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                              "SpdWriteField Error: Unsupported or Mismatched FieldIndex! Input FieldIndex = 0x%x; SpdFieldMapping.FieldIndex = 0x%x\n", FieldIndex, SpdFieldMapping.FieldIndex);
      return EFI_INVALID_PARAMETER;
    } else if ((SpdFieldMapping.StartByte == NO_SPD_BYTE_NUMBER) || (SpdFieldMapping.StartByteStartBit == NO_SPD_START_BIT)
               || (SpdFieldMapping.EndByte == NO_SPD_BYTE_NUMBER) || (SpdFieldMapping.EndByteEndBit == NO_SPD_END_BIT)) {
      RcDebugPrintWithDevice (SDBG_MAX, Socket, Chan, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                              "SpdWriteField Error: Unsupported FieldIndex = 0x%x\n", FieldIndex);
      return EFI_UNSUPPORTED;
    } else if (BufLength != DataLength) {
      // BufLength should equal to the DataLength.
      RcDebugPrintWithDevice (SDBG_MAX, Socket, Chan, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                              "SpdWriteField Error: BufLength = 0x%x should equal to the DataLength in SpdFieldMappingTable = 0x%x\n", BufLength, DataLength);
      return EFI_INVALID_PARAMETER;
    }

    StartBit = SpdFieldMapping.StartByteStartBit;

    for (Index = 0; Index < DataLength; Index++) {
      // Read from SPD only if the field is not from the first bit in first Byte or the end bit of the last Byte
      if (((Index == 0) && (StartBit != 0)) || ((Index == DataLength - 1) && (EndBit != 7))) {
        Status = SpdReadByte (Socket, Chan, Dimm, SpdFieldMapping.StartByte + Index, &ByteData);
      } else {
        ByteData = NextByteData;
        Status = EFI_SUCCESS;
      }

      if (PcdGetBool (PcdMrcDebugSpdDecodeLibTrace) == TRUE) {
        RcDebugPrintWithDevice (SDBG_MAX, Socket, Chan, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                                "SpdWriteByte Byte Data Before: ByteData[%d] = 0x%x\n", Index, ByteData);
      }

      // When it is the last Byte to read, the EndBit should be EndByteEndBit, or the EndBit should be 7.
      if (DataLength == Index + 1) {
        EndBit = SpdFieldMapping.EndByteEndBit;
      }

      if (EFI_ERROR (Status)) {
        RcDebugPrintWithDevice (SDBG_MAX, Socket, Chan, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                                "SpdWriteByte ERROR Status is %r when reading Spd Byte %d\n", Status, SpdFieldMapping.StartByte + Index);
      } else {
        if (((Index == 0) && (StartBit != 0)) || ((Index == DataLength - 1) && (EndBit != 7))) {
          // Clear the Field value from StartBit to EndBit in the ByteData
          ByteData &= (((UINT8) (0xFF << (EndBit + 1))) | ((UINT8) (0xFF >> (8 - StartBit))));
        }
        // Merge the Field StartBit to bit 7 value into ByteData
        ByteData |= ((UINT8) (*((UINT8 *) Buffer + Index) << StartBit));
        // Merger the Field bit 0 to EndBit to the next Byte if the StartBit != 0
        if ((StartBit != 0) && (Index < DataLength - 1)) {
          NextByteData = ((UINT8) (*((UINT8 *) Buffer + Index) >> (8 - StartBit)));
        } else {
          NextByteData = 0;
        }

        // Write the modified Byte Data into secific byte
        Status = SpdWriteByte (Socket, Chan, Dimm, SpdFieldMapping.StartByte + Index, &ByteData);
      }

      if (PcdGetBool (PcdMrcDebugSpdDecodeLibTrace) == TRUE) {
        RcDebugPrintWithDevice (SDBG_MAX, Socket, Chan, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                                "SpdWriteByte Byte Data After: ByteDataToWrite[%d] = 0x%x Status = %r\n", Index, ByteData, Status);
        Status = SpdReadByte (Socket, Chan, Dimm, SpdFieldMapping.StartByte + Index, &ByteData);
        RcDebugPrintWithDevice (SDBG_MAX, Socket, Chan, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                                "SpdWriteByte Byte Data After: ByteData[%d] = 0x%x\n", Index, ByteData);
        RcDebugPrintWithDevice (SDBG_MAX, Socket, Chan, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                                "SpdWriteField Field Data: FieldData[%d] = 0x%x\n", Index, *((UINT8 *)Buffer + Index));
      }
    }
  }

  if (PcdGetBool (PcdMrcDebugSpdDecodeLibTrace) == TRUE) {
    RcDebugPrintWithDevice (SDBG_MAX, Socket, Chan, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
                            "SpdWriteField: return Status = %r\n", Status);
  }

  return Status;
}

/**
  Check whether the target rank has CA[13] supported or not

  @param[in] Host    - Pointer to sysHost
  @param[in] Socket  - Socket to train
  @param[in] Ch      - DDR channel to setup
  @param[in] Dimm    - Dimm number the rank is on
  @param[in] Rank    - Rank number

  @retval TRUE       - Target rank supports CA[13]
          FALSE      - Target rank doesn't support CA[13]
**/
BOOLEAN
EFIAPI
Ca13Enabled (
  IN PSYSHOST  Host,
  IN UINT8     Socket,
  IN UINT8     Ch,
  IN UINT8     Dimm,
  IN UINT8     Rank
  )
{
  struct dimmNvram      (*DimmNvList)[MAX_DIMM];
  UINT8                 RowBits;
  UINT8                 DieCount;
  BOOLEAN               RankMix;
  BOOLEAN               Status;

  DimmNvList  = GetDimmNvList (Host, Socket, Ch);
  RowBits     = (*DimmNvList)[Dimm].numRowBits;    // Get number of row bits for primary SDRAM
  DieCount    = (*DimmNvList)[Dimm].dieCount;      // Get die count of Primary SDRAM
  RankMix     = (*DimmNvList)[Dimm].rankMix;       // RankMix is used to decide whether this DIMM is asymmetric

  if (RankMix &&      // TRUE means asymmetric and FALSE means symmetric, seen in Byte 234 in SPD Spec
    (Rank % 2 != 0))  // If the target DIMM is asymmetric and this rank is odd, then we need to read Secondary SDRAM info to check this rank. Seen in SPD Spec
  {
    RowBits        = (*DimmNvList)[Dimm].numRowBitsSecondary; // Get number of row bits for Secondary SDRAM
    DieCount       = (*DimmNvList)[Dimm].dieCountSecondary;
  }

  //
  // If SDRAM correponding to the target rank supports R17, CID2 or CID3, then CA13 is considered to be enabled
  //
  if (RowBits == 18 ||  // Whether R17 is supported, seen in DDR5 SDRAM Spec and SPD Spec
    DieCount == 8 ||    // Whether CID2 is supported, seen in DDR5 SDRAM Spec and SPD Spec
    DieCount == 16)   // Whether CID3 is supported, seen in DDR5 SDRAM Spec and SPD Spec
  {
    Status = TRUE;
  } else {
    Status = FALSE;
  }

  return Status;
}

/**
  Return tRFC and nRFC from SPD based on refresh mode policy

  @param[in]  Socket      - Socket to train
  @param[in]  Ch          - DDR channel to setup
  @param[in]  Dimm        - Dimm number the rank is on
  @param[in]  RefreshMode - Refresh mode policy setting
  @param[out] tRFC        - tRFC returned in units of ns
  @param[out] nRFC        - nRFC returned in units of clocks (nCK)

  @retval MRC_STATUS_SUCCESS if tRFC and nRFC are valid
  @retval MRC_STATUS_FAILURE otherwise
**/
MRC_STATUS
EFIAPI
SpdGetTrfc (
  IN  UINT8         Socket,
  IN  UINT8         Ch,
  IN  UINT8         Dimm,
  IN  REFRESH_MODE  RefreshMode,
  OUT UINT16        *tRFC,
  OUT UINT16        *nRFC
  )
{
  UINT8               MsbVal;
  UINT8               LsbVal;
  UINT8               Length;
  UINT16              MsbIndex;
  UINT16              LsbIndex;

  Length = 1;

  if (RefreshMode == RefabNormal) {
    //
    // Get tRFC1
    //
    MsbIndex = SPD_INDEX_SDRAM_MIN_REFRESH_RECOVERY_DELAY_TRFC1_SLR_MSB;
    LsbIndex = SPD_INDEX_SDRAM_MIN_REFRESH_RECOVERY_DELAY_TRFC1_SLR_LSB;

  } else if (RefreshMode == RefabFineGrain) {
    //
    // Get tRFC2
    //
    MsbIndex = SPD_INDEX_SDRAM_MIN_REFRESH_RECOVERY_DELAY_TRFC2_SLR_MSB;
    LsbIndex = SPD_INDEX_SDRAM_MIN_REFRESH_RECOVERY_DELAY_TRFC2_SLR_LSB;

  } else if (RefreshMode == RefsbFineGrain) {
    //
    // Get tRFCsb
    //
    MsbIndex = SPD_INDEX_SDRAM_MIN_REFRESH_RECOVERY_DELAY_TRFCSB_MSB;
    LsbIndex = SPD_INDEX_SDRAM_MIN_REFRESH_RECOVERY_DELAY_TRFCSB_LSB;

  } else {

    RC_FATAL_ERROR (FALSE, ERR_SPD_DECODE, SPD_FATAL_ERROR_UNSUPPORTED_MINOR_CODE_104);
    return MRC_STATUS_FAILURE;
  }

  SpdReadField (Socket, Ch, Dimm, MsbIndex, &MsbVal, &Length);
  SpdReadField (Socket, Ch, Dimm, LsbIndex, &LsbVal, &Length);
  //
  // tRFC in ns
  //
  *tRFC = (UINT16) (MsbVal << 8);
  *tRFC = *tRFC | (UINT16) LsbVal;
  //
  // tRFC in clocks (nCK)
  //
  *nRFC = NanosecondToClocks (Socket, *tRFC);

  return MRC_STATUS_SUCCESS;
}
