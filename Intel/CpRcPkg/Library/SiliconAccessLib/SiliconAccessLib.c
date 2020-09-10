/** @file
  Unified Silicon Register Access (USRA) Lib

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2011 - 2019 Intel Corporation. <BR>

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

#include "SiliconAccessLibInternal.h"


extern UINT8 mInStride[UsraWidthMaximum];
extern UINT8 mOutStride[UsraWidthMaximum];

GET_ALIGNED_ACCESS_ADDRESS mAccessAddrPtr[AddrTypeMaximum] =
{
  &GetPcieAccessAddress,     // AddrTypePCIE
  &GetPcieAccessAddress,     // AddrTypePCIEBLK
  &GetCsrAccessAddress,      // AddrTypeCSR
  &GetPciIoAccessAddress,    // AddrTypePCIIO
  &GetCsrMemAccessAddress,   // AddrTypeCSRMEM
  &GetCsrCfgAccessAddress    // AddrTypeCSRCFG
};

REGISTER_READ mRegisterReadPtr[AddrTypeMaximum] =
{
  &UsraRegAlignedRead,      // AddrTypePCIE
  &UsraRegAlignedRead,      // AddrTypePCIEBLK
  &UsraRegAlignedRead,      // AddrTypeCSR
  &UsraPciIoAlignedRead,     // AddrTypePCIIO
  &UsraRegAlignedRead,      // AddrTypeCSRMEM
  &UsraRegAlignedRead       // AddrTypeCSRCFG
};

REGISTER_WRITE mRegisterWritePtr[AddrTypeMaximum] =
{
  &UsraRegAlignedWrite,      // AddrTypePCIE
  &UsraRegAlignedWrite,      // AddrTypePCIEBLK
  &UsraRegAlignedWrite,      // AddrTypeCSR
  &UsraPciIoAlignedWrite,    // AddrTypePCIIO
  &UsraRegAlignedWrite,      // AddrTypeCSRMEM
  &UsraRegAlignedWrite       // AddrTypeCSRCFG
};

/**
  Perform MMIO read

  @param[in] AccessWidth          Access Width
  @param[in] AlignedAddress       An address to be read out
  @param[out] Buffer               A pointer of buffer contains the data to be read out

  @retval RETURN_SUCCESS          The function completed successfully.
**/
RETURN_STATUS
EFIAPI
UsraRegAlignedRead (
  IN UINT32                   AccessWidth,
  IN UINTN                    AlignedAddress,
  OUT VOID                    *Buffer
  )
{
  switch (AccessWidth) {
    case  UsraWidth8:
      *((UINT8*) Buffer) = MmioRead8 (AlignedAddress);
      break;
    case  UsraWidth16:
      *((UINT16*) Buffer) = MmioRead16 (AlignedAddress);
      break;
    case  UsraWidth32:
      *((UINT32*) Buffer) = MmioRead32 (AlignedAddress);
      break;
    default:
      *((UINT64*) Buffer) = MmioRead64 (AlignedAddress);
      break;
  }

  return RETURN_SUCCESS;
};

/**
  Perform MMIO write

  @param[in] AccessWidth          Access Width
  @param[in] AlignedAddress       An address to be written
  @param[in] Buffer               A pointer of buffer contains the data to be written

  @retval RETURN_SUCCESS          The function completed successfully.
**/
RETURN_STATUS
EFIAPI
UsraRegAlignedWrite (
  IN UINT32                   AccessWidth,
  IN UINTN                    AlignedAddress,
  OUT VOID                    *Buffer
  )
{
  switch (AccessWidth) {
    case  UsraWidth8:
      MmioWrite8 (AlignedAddress,*((UINT8*) Buffer));
      break;
    case  UsraWidth16:
      MmioWrite16 (AlignedAddress,*((UINT16*) Buffer));
      break;
    case  UsraWidth32:
      MmioWrite32 (AlignedAddress,*((UINT32*) Buffer));
      break;
    default:
      MmioWrite64 (AlignedAddress, *((UINT64*) Buffer));
      break;
  }
  return RETURN_SUCCESS;
}

/**
  Perform AND then OR operations for a input data

  @param[in out] Data             A pointer of the address of the register to be modified
  @param[in] AndBuffer            A pointer of buffer for the value used for AND operation
                                  A NULL pointer means no AND operation. RegisterModify() equivalents to RegisterOr()
  @param[in] OrBuffer             A pointer of buffer for the value used for OR operation
                                  A NULL pointer means no OR operation. RegisterModify() equivalents to RegisterAnd()
  @param[in] NumOfByte            NumOfByte Count of byte data to be performed

  @retval NONE
**/
VOID
EFIAPI
DataAndOr (
  IN  UINT64                  *Data,
  IN  VOID                    *AndBuffer,
  IN  VOID                    *OrBuffer,
  IN  UINT8                   NumOfByte
  )
{
  union {
    UINT64      QW;
    UINT8       Byte[8];
  } Buffer;

  UINT8 AndData[8];
  UINT8 OrData[8];
  UINT8 i;

  Buffer.QW = *Data;
  for (i = 0;i<NumOfByte;i++) {
    if (AndBuffer == NULL) {
      ((UINT8*) AndData) [i] = 0xff;
    } else {
      AndData[i] = ((UINT8*) AndBuffer) [i];
    }
    if (OrBuffer == NULL) {
      ((UINT8*) OrData) [i] = 0;
    } else {
      OrData[i] = ((UINT8*) OrBuffer) [i];
    }
    Buffer.Byte[i] = (Buffer.Byte[i] & AndData[i]) | OrData[i];
  }

  *Data = Buffer.QW;
}

//////////////////////////////////////////////////////////////////////////
//
// USRA Hardware Access Library
//
//////////////////////////////////////////////////////////////////////////

/**
  This API gets the flat address from the given USRA Address.

  @param[in] Address              A pointer of the address of the USRA Address Structure to be read out

  @retval                         The flat address
**/
UINTN
EFIAPI
InternalGetRegisterAddress (
  IN USRA_ADDRESS            *Address
  )
{
  UINTN AlignedAddress;

  mAccessAddrPtr[Address->Attribute.AddrType] (Address, &AlignedAddress);

  if (FeaturePcdGet (PcdUsraTraceEnable)) {
    USRATrace (Address, AlignedAddress, TraceGetAddr, NULL, NULL);
  }

  return AlignedAddress;
};

/**
  This API performs 8-bit, 16-bit, 32-bit or 64-bit silicon register read operations.
  It transfers data from a register into a naturally aligned data buffer.

  @param[in] Address              A pointer of the address of the USRA Address Structure to be read out
  @param[in] Buffer               A pointer of buffer for the value read from the register

  @retval RETURN_SUCCESS          The function completed successfully.
**/
RETURN_STATUS
EFIAPI
InternalRegisterRead (
  IN USRA_ADDRESS             *Address,
  IN VOID                     *Buffer
  )
{
  UINT8                       InStride;
  UINT8                       OutStride;
  RETURN_STATUS               Status;
  UINT32                      ReadCount;
  UINT8                       *StrideBuffer;

  UINTN AlignedAddress = 0;
  BOOLEAN FilterCatch = FALSE;

  mAccessAddrPtr[Address->Attribute.AddrType] (Address, &AlignedAddress);

  // PcieBlk Access Only
  if (Address->Attribute.AddrType == AddrTypePCIEBLK) {
    ReadCount = Address->PcieBlk.Count;
    Status = CpuIoCheckParameter (Address->Attribute.AccessWidth, AlignedAddress, ReadCount, Buffer);
    if (RETURN_ERROR (Status)) {
      return Status;
    }
    InStride = mInStride[Address->Attribute.AccessWidth];
    OutStride = mOutStride[Address->Attribute.AccessWidth];
  }
  else {
    // All other accesses
    ReadCount = 1;
    InStride = 0;
    OutStride = 0;
  }

  for (StrideBuffer = Buffer; ReadCount > 0; AlignedAddress += InStride, StrideBuffer += OutStride, ReadCount--) {
    // CSR (MEM|CFG) Access Only
    if (Address->Attribute.AddrType == AddrTypeCSR || Address->Attribute.AddrType == AddrTypeCSRMEM || Address->Attribute.AddrType == AddrTypeCSRCFG) {
      if (AlignedAddress == 0) {
        switch (Address->Attribute.AccessWidth) {
          case  UsraWidth8:
            *((UINT8*) (VOID *) StrideBuffer) = 0;
            break;
          case  UsraWidth16:
            *((UINT16*) (VOID *) StrideBuffer) = 0;
            break;
          case  UsraWidth32:
            *((UINT32*) (VOID *) StrideBuffer) = 0;
            break;
          default:
            *((UINT64*) (VOID *) StrideBuffer) = 0;
            break;
        }
        return RETURN_SUCCESS;
      }
    }

    if (FeaturePcdGet (PcdUsraRegisterFilterEnable)) {
      if (UsraRegisterFilterRead (Address, AlignedAddress, (VOID *) StrideBuffer) == RETURN_SUCCESS) {
        FilterCatch = TRUE;
      }
    }

    if (!FilterCatch) {
      mRegisterReadPtr[Address->Attribute.AddrType] ((USRA_ACCESS_WIDTH) (Address->Attribute.AccessWidth & 0x03), AlignedAddress, (VOID *) StrideBuffer);
    }

    if (FeaturePcdGet (PcdUsraRegisterLogEnable)) {
      UsraRegisterLogRead (Address, AlignedAddress, (VOID *) StrideBuffer);
    }

    if (FeaturePcdGet (PcdUsraTraceEnable)) {
      USRATrace (Address, AlignedAddress, TraceRead, (VOID *) StrideBuffer, NULL);
    }
  }

  return RETURN_SUCCESS;
};

/**
  This API performs 8-bit, 16-bit, 32-bit or 64-bit silicon register write operations.
  It transfers data from a naturally aligned data buffer into a silicon register.

  @param[in] Address              A pointer of the address of the USRA Address Structure to be written
  @param[in] Buffer               A pointer of buffer for the value write to the register

  @retval RETURN_SUCCESS          The function completed successfully.
**/
RETURN_STATUS
EFIAPI
InternalRegisterWrite (
  IN USRA_ADDRESS             *Address,
  IN VOID                     *Buffer
  )
{
  UINT8                       InStride;
  UINT8                       OutStride;
  RETURN_STATUS               Status;
  UINT32                      WriteCount;
  UINT8                       *StrideBuffer;

  UINTN AlignedAddress = 0;
  BOOLEAN FilterCatch = FALSE;

  mAccessAddrPtr[Address->Attribute.AddrType] (Address, &AlignedAddress);

  // PcieBlk Access Only
  if (Address->Attribute.AddrType == AddrTypePCIEBLK) {
    WriteCount = Address->PcieBlk.Count;
    Status = CpuIoCheckParameter (Address->Attribute.AccessWidth, AlignedAddress, WriteCount, Buffer);
    if (RETURN_ERROR (Status)) {
      return Status;
    }
    InStride = mInStride[Address->Attribute.AccessWidth];
    OutStride = mOutStride[Address->Attribute.AccessWidth];
  }
  else {
    // All other accesses
    WriteCount = 1;
    InStride = 0;
    OutStride = 0;
  }

  for (StrideBuffer = Buffer; WriteCount > 0; AlignedAddress += InStride, StrideBuffer += OutStride, WriteCount--) {

    // CSR (MEM|CFG) Access only
    if (Address->Attribute.AddrType == AddrTypeCSR || Address->Attribute.AddrType == AddrTypeCSRMEM || Address->Attribute.AddrType == AddrTypeCSRCFG) {
      if (AlignedAddress == 0) {
        return RETURN_ABORTED;
      }
    }

    if (FeaturePcdGet (PcdUsraRegisterFilterEnable)) {
      if (UsraRegisterFilterWrite (Address, AlignedAddress, (VOID *) StrideBuffer) == RETURN_SUCCESS) {
        FilterCatch = TRUE;
      }
    }

    if (!FilterCatch) {
      mRegisterWritePtr[Address->Attribute.AddrType] ((USRA_ACCESS_WIDTH) (Address->Attribute.AccessWidth & 0x03), AlignedAddress, (VOID *) StrideBuffer);
    }

    if (FeaturePcdGet (PcdUsraRegisterLogEnable)) {
      UsraRegisterLogWrite (Address, AlignedAddress, (VOID *) StrideBuffer);
    }

    // CSR (MEM|CFG), PCIEBLK, and PCIE Access Only
    if (FeaturePcdGet (PcdUsraSupportS3)) {
      if (Address->Attribute.AddrType != AddrTypePCIIO && Address->Attribute.S3Enable) {
        S3BootScriptSaveMemWrite ((S3_BOOT_SCRIPT_LIB_WIDTH) (Address->Attribute.AccessWidth & 0x03), (UINT64) AlignedAddress, 1, (VOID *) StrideBuffer);
      }
    }

    if (FeaturePcdGet (PcdUsraTraceEnable)) {
      USRATrace (Address, AlignedAddress, TraceWrite, (VOID *) StrideBuffer, NULL);
    }

  }

  return RETURN_SUCCESS;
};

/**
  This API performs 8-bit, 16-bit, 32-bit or 64-bit silicon register AND then OR operations. It read data from a
  register, And it with the AndBuffer, then Or it with the OrBuffer, and write the result back to the register

  @param[in] Address              A pointer of the address of the silicon register to be written
  @param[in] AndBuffer            A pointer of buffer for the value used for AND operation
                                  A NULL pointer means no AND operation. RegisterModify() equivalents to RegisterOr()
  @param[in] OrBuffer             A pointer of buffer for the value used for OR operation
                                  A NULL pointer means no OR operation. RegisterModify() equivalents to RegisterAnd()

  @retval RETURN_SUCCESS          The function completed successfully.
**/
RETURN_STATUS
EFIAPI
InternalRegisterModify (
  IN USRA_ADDRESS             *Address,
  IN VOID                     *AndBuffer,
  IN VOID                     *OrBuffer
  )
{
  UINT64 Data;
  UINT8 WidthTable[4];
  UINTN AlignedAddress;

  WidthTable[0]  = 1;
  WidthTable[1]  = 2;
  WidthTable[2]  = 4;
  WidthTable[3]  = 8;
  AlignedAddress = 0;

  if (Address->Attribute.AddrType == AddrTypePCIIO) {
    return RETURN_UNSUPPORTED;
  }

  mAccessAddrPtr[Address->Attribute.AddrType] (Address, &AlignedAddress);

  // CSR (MEM|CFG) Access only
  if (Address->Attribute.AddrType == AddrTypeCSR || Address->Attribute.AddrType == AddrTypeCSRMEM || Address->Attribute.AddrType == AddrTypeCSRCFG) {
    if (AlignedAddress == 0) {
      return RETURN_ABORTED;
    }
  }

  mRegisterReadPtr[Address->Attribute.AddrType] (Address->Attribute.AccessWidth, AlignedAddress, &Data);
  DataAndOr (&Data, AndBuffer, OrBuffer, WidthTable[ (UINT8) Address->Attribute.AccessWidth]);
  mRegisterWritePtr[Address->Attribute.AddrType] (Address->Attribute.AccessWidth, AlignedAddress, &Data);

  if (FeaturePcdGet (PcdUsraTraceEnable)) {
    USRATrace (Address, 0, TraceModify, AndBuffer, OrBuffer);
  }

  return RETURN_SUCCESS;
};
