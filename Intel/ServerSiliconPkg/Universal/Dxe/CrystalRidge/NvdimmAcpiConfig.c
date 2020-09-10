/** @file NvdimmAcpiConfig.c
  Implementation of NVDIMM ACPI config protocol.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2019 Intel Corporation. <BR>

  The source code contained or described herein and all documents related to the
  source code ("Material") are owned by Intel Corporation or its suppliers or
  licensors. Title to the Material remains with Intel Corporation or its suppliers
  and licensors. The Material may contain trade secrets and proprietary and
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
#include <Library/DebugLib.h>
#include "CrystalRidge.h"
#include <IndustryStandard/AcpiAml.h>
#include <Protocol/NvdimmAcpiConfigProtocol.h>
#include "NvdimmAcpiConfig.h"

//#define NVDIMM_ACPI_CONFIG_DEBUG 1

#ifdef NVDIMM_ACPI_CONFIG_DEBUG
#define COND_DEBUG(_X) CRDEBUG(_X)
#else
#define COND_DEBUG(_X)
#endif

#define REG_SOURCE_MAILBOX 0
#define REG_SOURCE_DIMM    1

extern CR_INFO mCrInfo;
EFI_NVDIMM_ACPI_CONFIG_PROTOCOL mNvdimmAcpiConfigProtocol;

/**
  Populates Control Region Operation Region's offset and length properties (CCT#)

  Extracts region index from the template and populates offset and length properties accordingly
  Note: the function expects that both offset and length properties are encoded as quad-word,
        otherwise triggers assertion failure

  @param[in,out]  TablePointer  Pointer to size prefix of Operation Region's offset

**/
VOID
EFIAPI
PopulateControlRegion (
  IN OUT UINT8 *TablePointer
  )
{
  UINT64 *Offset;
  UINT64 *Length;
  UINT64 RegionBase;
  UINT64 RegionSize;
  UINT32 RegionDetails;
  UINT8  RegionType;
  UINT8  RegionIndex;
  UINT8  LengthPrefix;

  ASSERT (*TablePointer == AML_QWORD_PREFIX);
  Offset = (UINT64 *)(TablePointer + 1);
  LengthPrefix = *(TablePointer + 9);
  Length = (UINT64 *)(TablePointer + 10);
  RegionDetails = *Offset >> 32;
  RegionType = RegionDetails & 0xFF;
  RegionIndex = (RegionDetails >> 8) & 0xFF;
  COND_DEBUG ((DEBUG_INFO, "PopulateControlRegion: Offset: 0x%llx, LengthPrefix: 0x%x, Length: 0x%llx, "
                           "RegionType: 0x%x, RegionIndex: 0x%x\n",
               *Offset, LengthPrefix, *Length, RegionType, RegionIndex));
  ASSERT (LengthPrefix == AML_QWORD_PREFIX);
  ASSERT (RegionType == REGION_TYPE_CONTROL_REGION);
  if (RegionIndex > MAX_CONTROL_REGIONS) {
    ASSERT (FALSE);
    return;
  }

  RegionBase = mCrInfo.ControlRegion[RegionIndex].Base;
  if (mCrInfo.CrbdAddrLimit > mCrInfo.CrbdAddrBase && mCrInfo.ControlRegion[RegionIndex].Base != (UINT64) &mCrInfo.ControlRegionDummy)
  {
    RegionSize = CR_CRBD_MAX_ASL_REGION_LENGTH;
  }
  else
  {
    RegionSize = mCrInfo.ControlRegion[RegionIndex].Size;
  }
  COND_DEBUG ((DEBUG_INFO, "PopulateControlRegion: Base: 0x%llx, Size: 0x%llx\n", RegionBase, RegionSize));
  *Offset = RegionBase;
  *Length = RegionSize;
}

/**
  Populates Flush Region Operation Region's offset and length properties (CFH#)

  Extracts region index from the template and populates offset and length properties accordingly
  Note: the function expects that both offset and length properties are encoded as quad-word,
        otherwise triggers assertion failure

  @param[in,out]  TablePointer  Pointer to size prefix of Operation Region's offset

**/
VOID
EFIAPI
PopulateFlushRegion (
  IN OUT UINT8 *TablePointer
  )
{
  UINT64 *Offset;
  UINT64 *Length;
  UINT64 RegionBase;
  UINT64 RegionSize;
  UINT32 RegionDetails;
  UINT8  RegionType;
  UINT8  RegionIndex;
  UINT8  LengthPrefix;

  ASSERT (*TablePointer == AML_QWORD_PREFIX);
  Offset = (UINT64 *)(TablePointer + 1);
  LengthPrefix = *(TablePointer + 9);
  Length = (UINT64 *)(TablePointer + 10);
  RegionDetails = *Offset >> 32;
  RegionType = RegionDetails & 0xFF;
  RegionIndex = (RegionDetails >> 8) & 0xFF;
  COND_DEBUG ((DEBUG_INFO, "PopulateFlushRegion: Offset: 0x%llx, LengthPrefix: 0x%x, Length: 0x%llx, "
                           "RegionType: 0x%x, RegionIndex: 0x%x\n",
               *Offset, LengthPrefix, *Length, RegionType, RegionIndex));
  ASSERT (LengthPrefix == AML_QWORD_PREFIX);
  ASSERT (RegionType == REGION_TYPE_FLUSH_REGION);
  if (RegionIndex > MAX_CONTROL_REGIONS) {
    ASSERT (FALSE);
    return;
  }
  RegionBase = mCrInfo.FlushRegion[RegionIndex].Base;
  RegionSize = mCrInfo.FlushRegion[RegionIndex].End - mCrInfo.FlushRegion[RegionIndex].Base;
  COND_DEBUG ((DEBUG_INFO, "PopulateFlushRegion: Base: 0x%llx, Size: 0x%llx\n", RegionBase, RegionSize));
  *Offset = RegionBase;
  *Length = RegionSize;
}

/**
  Parses AML Package Length from specified location and advances the pointer accordingly

  Note: the function expects valid AML, otherwise triggers assertion failure

  @param[in,out]  TablePointer    Address of the Package Length pointer.
  @param[in]      FullLengthOnly  TRUE:  trigger assertion failure if encoding length < 4
                                  FALSE: allow any valid encoding length

**/
STATIC
UINT32
AcpiParsePackageLength (
  IN OUT UINT8   **TablePointer,
  IN     BOOLEAN FullLengthOnly
  )
{
  UINT8  ExtraBytes;
  UINT32 Result = 0;

  ExtraBytes = (*TablePointer)[0] >> 6;
  if (ExtraBytes != 0) {
    ASSERT (((*TablePointer)[0] & 0x30) == 0);
  }

  if (FullLengthOnly) {
    ASSERT (ExtraBytes == 3);
  }
  switch (ExtraBytes) {
    case 3:
      Result |= (UINT32)(*TablePointer)[3];
      Result <<= 8;
    case 2:
      Result |= (UINT32)(*TablePointer)[2];
      Result <<= 8;
    case 1:
      Result |= (UINT32)(*TablePointer)[1];
      Result <<= 4;
      Result |= (*TablePointer)[0] & 0x0F;
      break;
    case 0:
      ASSERT ((*TablePointer)[0] < 0x40);
      Result = (*TablePointer)[0];
      break;
  }
  (*TablePointer) += ExtraBytes + 1;
  return Result;
}

/**
  Encodes specified value as AML Package Length and stores it at specified location.

  Note: always uses 4-byte-long encoding!
  Validation of the value belongs to the caller. If the value can't be encoded, the function triggers asserton failure

  @param[in]   PackageLength     Package length to write
  @param[in]   WriteEightOffsets Workaround to write extended package length
  @param[out]  TablePointer      Location to write encoded value to

**/
STATIC
VOID
AcpiWritePackageLength (
  IN  UINT64  PackageLength,
  IN  BOOLEAN WriteEightOffsets,
  OUT UINT8  *TablePointer
  )
{
  UINT8  Index, ByteIndex = 0, LengthNum = 1;
  UINT64 PackageLengthTemp;

  ASSERT (PackageLength < (1U << 28));

  if (WriteEightOffsets)
  {
    LengthNum = 8;
  }
  else
  {
    ASSERT (PackageLength < (1U << 25));
    PackageLength <<= 3;
  }

  for (Index = 0; Index < LengthNum; Index++)
  {
    PackageLengthTemp = PackageLength;

    TablePointer[ByteIndex++] = 0xC0 | (PackageLengthTemp & 0xF);
    PackageLengthTemp >>= 4;
    TablePointer[ByteIndex++] = PackageLengthTemp & 0xFF;
    PackageLengthTemp >>= 8;
    TablePointer[ByteIndex++] = PackageLengthTemp & 0xFF;
    PackageLengthTemp >>= 8;
    TablePointer[ByteIndex++] = PackageLengthTemp & 0xFF;
    if (Index < LengthNum-1)
    {
      TablePointer[ByteIndex++] = 0x00;
    }
  }
}

/**
  Converts a digit to ASCII hexadecimal digit

  @param[in]  Digit  Digit to convert

  @returns  ASCII hexadecimal digit ([0-9,A-F])

**/
STATIC
UINT8
ToAsciiHexDigit (
  IN  UINT8  Digit
  )
{
  UINT8 Result;

  ASSERT (Digit < 0x10);

  Result = Digit + '0';
  if (Digit > 9) {
    Result += 'A' - ('9' + 1);
  }
  return Result;
}

/**
  Common register population function.

  Populates register value(s) according to input parameters.

  @param[in,out]  TablePointer       Pointer to field opcode.
  @param[in]      Magic              Magic number to cross check the template against
  @param[in]      RegSource          Register offset source
                                     REG_SOURCE_MAILBOX: CR_MAILBOX structure (OS mailbox)
                                     REG_SOURCE_DIMM:    NVDIMM structure
  @param[in]      RegOffset          Offset within the structure indicated by RegSource
                                     where the address of first register to populate is located
  @param[in]      RegNum             Number of registers to populate
  @param[in]      RegName            Register name (in network byte order) to cross check the template against
  @param[in]      RegNameMask        Register name mask (in network byte order) to use in the cross check;
                                     only masked bits are used in the comparison
  @param[in]      RegOrderLutOffset  Offset of register order look-up table (UINT8 *)
                                     within the structure indicated by RegSource;
                                     this is to accommodate scenarios in which register addresses
                                     are not ordered in SPA space

**/
STATIC
VOID
PopulateNvmCtrlRegister (
  IN OUT UINT8  *TablePointer,
  IN     UINT32 Magic,
  IN     UINT8  RegSource,
  IN     UINT32 RegOffset,
  IN     UINT32 RegNum,
  IN     UINT32 RegName,
  IN     UINT32 RegNameMask,
  IN     UINT32 RegOrderLutOffset
  )
{
  UINT8  *OriginalTablePointer;
  UINT8  *RegionNamePointer;
  UINT8  *OffsetDeltaPointer;
  UINT32 OffsetDelta;
  UINT8  Socket;
  UINT8  Ch;
  UINT8  Dimm;
  NVDIMM *DimmPtr;
  CR_MAILBOX *Mailbox;
  UINT8  Index;
  UINT64 *RegPtr;
  UINT32 *NamePtr;
  UINT8  *RegOrderLut = NULL;

  OriginalTablePointer = TablePointer;
  COND_DEBUG ((DEBUG_INFO, "PopulateNvmCtrlRegister: TablePointer: 0x%llx, Magic: 0x%x\n", TablePointer, Magic));
  ASSERT (TablePointer[0] == AML_EXT_OP && TablePointer[1] == AML_EXT_FIELD_OP);
  TablePointer += 2;
  AcpiParsePackageLength (&TablePointer, FALSE);
  RegionNamePointer = TablePointer;
  ASSERT (RegionNamePointer[0] == 'C' &&
          (RegionNamePointer[1] == 'C' || RegionNamePointer[1] == 'F') &&
          (RegionNamePointer[2] == 'T' || RegionNamePointer[2] == 'H') &&
          RegionNamePointer[3] == ToAsciiHexDigit(MAX_CONTROL_REGIONS));
  TablePointer += 5;
  ASSERT (TablePointer[0] == 0x00);
  TablePointer++;
  OffsetDeltaPointer = TablePointer;
  OffsetDelta = AcpiParsePackageLength (&TablePointer, TRUE);
  OffsetDelta >>= 3;
  ASSERT ((OffsetDelta >> 12) == Magic);

  Socket = (OffsetDelta >> 8) & 0xF;
  Ch = (OffsetDelta >> 4) & 0xF;
  Dimm = OffsetDelta & 0xF;

  if (Magic != MAGIC_FLUSH_ADDRESS) {
    for (Index = 0; Index < 7; Index++) {
      ASSERT (TablePointer[0] == 0x00);
      TablePointer++;
      OffsetDelta = AcpiParsePackageLength (&TablePointer, TRUE);
      OffsetDelta >>= 3;
      ASSERT ((OffsetDelta >> 12) == MAGIC_NEXT_OFFSETS);
    }
  }

  COND_DEBUG ((DEBUG_INFO, "PopulateNvmCtrlRegister: Socket Channel Dimm: %d %d %d\n", Socket, Ch, Dimm));
  DimmPtr = GetDimm (&mCrInfo.NvdimmInfo, Socket, Ch, Dimm, NVDIMM_FLAG_ENABLED | NVDIMM_FLAG_DUMMY);
  ASSERT (DimmPtr);
  Mailbox = GetMailbox (DimmPtr, TRUE);
  ASSERT (Mailbox);
  switch (RegSource) {
    case REG_SOURCE_MAILBOX:
      RegPtr = (UINT64 *)((UINT64)Mailbox + (UINT64)RegOffset);
      RegOrderLut = (UINT8 *)((UINT64)Mailbox + (UINT64)RegOrderLutOffset);
      break;
    case REG_SOURCE_DIMM:
      RegPtr = (UINT64 *)((UINT64)DimmPtr + (UINT64)RegOffset);
      RegOrderLut = (UINT8 *)((UINT64)DimmPtr + (UINT64)RegOrderLutOffset);
      break;
    default:
      ASSERT (FALSE);
      return;
  }
  COND_DEBUG ((DEBUG_INFO, "PopulateNvmCtrlRegister: Register: 0x%llx, Base: 0x%llx\n", *RegPtr, DimmPtr->SADSpaBase));
  if (Magic != MAGIC_FLUSH_ADDRESS) {
    AcpiWritePackageLength (*RegPtr - DimmPtr->SADSpaBase, TRUE, OffsetDeltaPointer);
    ASSERT (DimmPtr->ControlRegionIndex < 16);
    RegionNamePointer[3] = ToAsciiHexDigit ((UINT8) DimmPtr->ControlRegionIndex);
  } else {
    AcpiWritePackageLength (*RegPtr - mCrInfo.FlushRegion[DimmPtr->FlushRegionIndex].Base, FALSE, OffsetDeltaPointer);
    ASSERT (DimmPtr->FlushRegionIndex < 16);
    RegionNamePointer[3] = ToAsciiHexDigit ((UINT8) DimmPtr->FlushRegionIndex);
  }

  for (Index = 1; Index < RegNum; Index++) {
    RegPtr++;
    NamePtr = (UINT32 *) TablePointer;
	//COND_DEBUG ((DEBUG_INFO, "PopulateNvmCtrlRegister: *NamePtr: 0x%x, RegNameMask: 0x%x, RegName: 0x%x, TablePointer[4]: 0x%x \n", *NamePtr, RegNameMask, RegName, TablePointer[4]));

    ASSERT ((*NamePtr & RegNameMask) == (RegName & RegNameMask) && TablePointer[4] == 0x40 && TablePointer[5] == 0x04);
    TablePointer += 6;
    ASSERT (TablePointer[0] == 0x00);
    TablePointer++;
    OffsetDeltaPointer = TablePointer;
    OffsetDelta = AcpiParsePackageLength (&TablePointer, TRUE);
    OffsetDelta >>= 3;
    ASSERT (OffsetDelta == 0xFFFF8);
    COND_DEBUG ((DEBUG_INFO, "PopulateNvmCtrlRegister: Register: 0x%llx, Previous: 0x%llx\n", RegPtr[0], RegPtr[-1]));
    AcpiWritePackageLength (RegPtr[0] - RegPtr[-1] - 8, FALSE, OffsetDeltaPointer);
    *NamePtr &= SIGNATURE_32(0xFF, 0xFF, 0, 0);
    *NamePtr |= SIGNATURE_32(0, 0,
                             ToAsciiHexDigit (((RegOrderLut[Index - 1] + 1) >> 4) & 0xF),
                             ToAsciiHexDigit ((RegOrderLut[Index - 1] + 1) & 0xF));
    COND_DEBUG ((DEBUG_INFO, "PopulateNvmCtrlRegister: *NamePtr: 0x%x\n", *NamePtr));
  }
  if (RegNum > 1) {
    NamePtr = (UINT32 *) TablePointer;
    ASSERT ((*NamePtr & RegNameMask) == (RegName & RegNameMask) && TablePointer[4] == 0x40 && TablePointer[5] == 0x04);
    *NamePtr &= SIGNATURE_32(0xFF, 0xFF, 0, 0);
    *NamePtr |= SIGNATURE_32(0, 0,
                             ToAsciiHexDigit (((RegOrderLut[Index - 1] + 1) >> 4) & 0xF),
                             ToAsciiHexDigit ((RegOrderLut[Index - 1] + 1) & 0xF));
    COND_DEBUG ((DEBUG_INFO, "PopulateNvmCtrlRegister: *NamePtr: 0x%x\n", *NamePtr));
  }
  return;
}

/**
  Populates Mailbox Command Register Field (N###.XCMD)

  Extracts DIMM location from the template and populates object's offset accordingly.

  @param[in,out]  TablePointer  Pointer to XCMD Field Opcode

**/
VOID
EFIAPI
PopulateCommandRegister (
  IN OUT UINT8 *TablePointer
  )
{
  PopulateNvmCtrlRegister (TablePointer, MAGIC_COMMAND_REG, REG_SOURCE_MAILBOX, OFFSET_OF(CR_MAILBOX, pCommand),
                           1, 0, 0, 0);
}

/**
  Populates Mailbox Status Register Field (N###.XSTA)

  Extracts DIMM location from the template and populates object's offset accordingly.

  @param[in,out]  TablePointer  Pointer to XSTA Field Opcode

**/
VOID
EFIAPI
PopulateStatusRegister (
  IN OUT UINT8 *TablePointer
  )
{
  PopulateNvmCtrlRegister (TablePointer, MAGIC_STATUS_REG, REG_SOURCE_MAILBOX, OFFSET_OF(CR_MAILBOX, pStatus),
                           1, 0, 0, 0);
}

/**
  Populates Input Payload Register Fields (N###.IN##)

  Extracts DIMM location and register numbers from templates
  and populates objects' offset and name properties accordingly.

  @param[in,out]  TablePointer  Pointer to IN01 field opcode.

**/
VOID
EFIAPI
PopulateInputPayloadRegister (
  IN OUT UINT8 *TablePointer
  )
{
  PopulateNvmCtrlRegister (TablePointer, MAGIC_INPUT_PAYLOAD_REG, REG_SOURCE_MAILBOX,
                           OFFSET_OF(CR_MAILBOX, pInPayloadOrder), NUM_PAYLOAD_REG / 2,
                           SIGNATURE_32('I', 'N', 'X', 'X'), 0xFFFF, OFFSET_OF(CR_MAILBOX, InPayloadOrderLut));
}

/**
  Populates Output Payload Register Fields (N###.OU##)

  Extracts DIMM location and register numbers from templates
  and populates objects' offset and name properties accordingly.

  @param[in,out]  TablePointer  Pointer to OU01 field opcode.

**/
VOID
EFIAPI
PopulateOutputPayloadRegister (
  IN OUT UINT8 *TablePointer
  )
{
  PopulateNvmCtrlRegister (TablePointer, MAGIC_OUTPUT_PAYLOAD_REG, REG_SOURCE_MAILBOX,
                           OFFSET_OF(CR_MAILBOX, pOutPayloadOrder), NUM_PAYLOAD_REG / 2,
                           SIGNATURE_32('O', 'U', 'X', 'X'), 0xFFFF, OFFSET_OF(CR_MAILBOX, OutPayloadOrderLut));
}

/**
  Populates Boot Status Register Field (N###.XBSR)

  Extracts DIMM location from the template and populates object's offset property accordingly.

  @param[in,out]  TablePointer  Pointer to XBSR Field Opcode

**/
VOID
EFIAPI
PopulateBootStatusRegister (
  IN OUT UINT8 *TablePointer
  )
{
  PopulateNvmCtrlRegister (TablePointer, MAGIC_BOOT_STATUS_REG, REG_SOURCE_DIMM, OFFSET_OF(NVDIMM, BsrSpa),
                           1, 0, 0, 0);
}

/**
  Populates DIMM Enable Status object (N###.CENA)

  Extracts DIMM location from the template and checks if the DIMM is enabled,
  then populates object's value accordingly.
  Note: the function expects object's value to be encoded as quad-word, otherwise triggers assertion failure

  @param[in,out]  TablePointer  Pointer to size prefix of object value

**/
VOID
EFIAPI
PopulateDimmEnableStatus (
  IN OUT UINT8 *TablePointer
  )
{
  UINT64 *DimmEnableStatus;
  UINT64 Temp;
  UINT8  Socket;
  UINT8  Ch;
  UINT8  Dimm;

  ASSERT (*TablePointer == AML_QWORD_PREFIX);
  DimmEnableStatus = (UINT64 *)(TablePointer + 1);
  Temp = *DimmEnableStatus;
  Temp >>= 32;
  Dimm = Temp & 0x0F;
  Ch = (Temp >> 4) & 0x0F;
  Socket = (Temp >> 8) & 0x0F;
  *DimmEnableStatus = IsNvmDimmEnabled (Socket, Ch, Dimm);
}

/**
  Populates FIS version object (N###.CFIS)

  Note: the function expects object's value to be encoded as quad-word, otherwise triggers assertion failure

  @param[in,out]  TablePointer  Pointer to size prefix of object value

**/
VOID
EFIAPI
PopulateFisVersion (
  IN OUT UINT8 *TablePointer
  )
{
  UINT64   *FisVersion;
  UINT64   Temp;
  NVDIMM   *DimmPtr;
  UINT8    Socket;
  UINT8    Ch;
  UINT8    Dimm;

  ASSERT (*TablePointer == AML_QWORD_PREFIX);
  FisVersion = (UINT64 *)(TablePointer + 1);
  Temp = *FisVersion;
  Temp >>= 32;
  Dimm = Temp & 0x0F;
  Ch   = (Temp >> 4) & 0x0F;
  Socket = (Temp >> 8) & 0x0F;

  DimmPtr = GetDimm (&mCrInfo.NvdimmInfo, Socket, Ch, Dimm, NVDIMM_FLAG_ENABLED);
  if (DimmPtr) {
    *FisVersion = DimmPtr->FisVersion;
  } else {
    *FisVersion = 0;
  }
}

/**
  Populates Flush Address Fields (N###.FL##)

  Extracts DIMM location and register numbers from templates
  and populates objects' offset and name properties accordingly.

  @param[in,out]  TablePointer  Pointer to FL01 field opcode.
**/
VOID
EFIAPI
PopulateFlushAddress (
  IN OUT UINT8 *TablePointer
  )
{
  PopulateNvmCtrlRegister (TablePointer, MAGIC_FLUSH_ADDRESS, REG_SOURCE_DIMM,
                           OFFSET_OF(NVDIMM, FlushAddressOrder), 32,
                           SIGNATURE_32('F', 'L', 'X', 'X'), 0xFFFF, OFFSET_OF(NVDIMM, FlushAddressOrderLut));
}


/**
  Installs NVDIMM ACPI Config Protocol

  @param[in]  Handle  Handle to install the protocol on.

  @retval     EFI_SUCCESS         Protocol installed.
  @retval     <EFI error status>  An error occurred. Protocol not installed.

**/
EFI_STATUS
InstallNvdimmAcpiConfigProtocol (
  IN EFI_HANDLE  *Handle
  )
{
  EFI_STATUS Status;

  mNvdimmAcpiConfigProtocol.PopulateControlRegion         = PopulateControlRegion;
  mNvdimmAcpiConfigProtocol.PopulateFlushRegion           = PopulateFlushRegion;
  mNvdimmAcpiConfigProtocol.PopulateCommandRegister       = PopulateCommandRegister;
  mNvdimmAcpiConfigProtocol.PopulateStatusRegister        = PopulateStatusRegister;
  mNvdimmAcpiConfigProtocol.PopulateInputPayloadRegister  = PopulateInputPayloadRegister;
  mNvdimmAcpiConfigProtocol.PopulateOutputPayloadRegister = PopulateOutputPayloadRegister;
  mNvdimmAcpiConfigProtocol.PopulateBootStatusRegister    = PopulateBootStatusRegister;
  mNvdimmAcpiConfigProtocol.PopulateDimmEnableStatus      = PopulateDimmEnableStatus;
  mNvdimmAcpiConfigProtocol.PopulateFisVersion            = PopulateFisVersion;
  mNvdimmAcpiConfigProtocol.PopulateFlushAddress          = PopulateFlushAddress;

  Status = gBS->InstallProtocolInterface (
           Handle,
           &gEfiNvdimmAcpiConfigProtocolGuid,
           EFI_NATIVE_INTERFACE,
           &mNvdimmAcpiConfigProtocol
           );

  return Status;
}
