/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2015 - 2020 Intel Corporation. <BR>

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


//
// Lookup table for increment values based on transfer widths
//
UINT8 mInStride[UsraWidthMaximum] = {
  1, // UsraWidth8
  2, // UsraWidth16
  4, // UsraWidth32
  8, // UsraWidth64
  0, // UsraWidthFifo8
  0, // UsraWidthFifo16
  0, // UsraWidthFifo32
  0, // UsraWidthFifo64
  1, // UsraWidthFill8
  2, // UsraWidthFill16
  4, // UsraWidthFill32
  8  // UsraWidthFill64
};

//
// Lookup table for increment values based on transfer widths
//
UINT8 mOutStride[UsraWidthMaximum] = {
  1, // UsraWidth8
  2, // UsraWidth16
  4, // UsraWidth32
  8, // UsraWidth64
  1, // UsraWidthFifo8
  2, // UsraWidthFifo16
  4, // UsraWidthFifo32
  8, // UsraWidthFifo64
  0, // UsraWidthFill8
  0, // UsraWidthFill16
  0, // UsraWidthFill32
  0  // UsraWidthFill64
};

/**
  This API gets the Pcie address from the given USRA Address.

  @param[in] Address              A pointer of the address of the USRA Address Structure
  @param[out] AlignedAddress      A pointer of aligned address converted from USRA address

**/
VOID
EFIAPI
GetPcieAccessAddress (
  IN USRA_ADDRESS             *Address,
  OUT UINTN                   *AlignedAddress
  )
{
  INTN                        MmCfgBase;

  MmCfgBase = GetPcieSegMmcfgBaseAddress (Address);
  *AlignedAddress = MmCfgBase + (UINTN) (Address->Attribute.RawData32[0] & 0x0fffffff);
}

/**
  Check parameters for PcieBlk Register Read request.

  The I/O operations are carried out exactly as requested. The caller is responsible
  for satisfying any alignment and I/O width restrictions that a PI System on a
  platform might require. For example on some platforms, width requests of
  UsraWidth64 do not work. Misaligned buffers, on the other hand, will
  be handled by the driver.

  @param[in] Width          Signifies the width of the I/O or Memory operation.
  @param[in] Address        The base address of the I/O operation.
  @param[in] Count          The number of I/O operations to perform. The number of
                            bytes moved is Width size * Count, starting at Address.
  @param[in] Buffer         For read operations, the destination buffer to store the results.
                            For write operations, the source buffer from which to write data.

  @retval EFI_SUCCESS            The parameters for this request pass the checks.
  @retval EFI_INVALID_PARAMETER  Buffer is NULL.
  @retval EFI_INVALID_PARAMETER  Width is invalid for this PI system.
  @retval EFI_UNSUPPORTED        The Buffer is not aligned for the given Width.
  @retval EFI_UNSUPPORTED        The address range specified by Address, Width,
                                 and Count is not valid for this PI system.

**/
RETURN_STATUS
CpuIoCheckParameter (
  IN USRA_ACCESS_WIDTH          Width,
  IN UINT64                     Address,
  IN UINTN                      Count,
  IN VOID                       *Buffer
  )
{
  UINT64  MaxCount;
  UINT64  Limit;

  //
  // Check to see if Buffer is NULL
  //
  if (Buffer == NULL) {
    return RETURN_INVALID_PARAMETER;
  }

  //
  // Check to see if Width is in the valid range
  //
  if ((UINT32) Width >= UsraWidthMaximum) {
    return RETURN_INVALID_PARAMETER;
  }

  //
  // For FIFO type, the target address won't increase during the access,
  // so treat Count as 1
  //
  if (Width >= UsraWidthFifo8 && Width <= UsraWidthFifo64) {
    Count = 1;
  }

  //
  // Check to see if Width is in the valid range for I/O Port operations
  //
  Width = (USRA_ACCESS_WIDTH) (Width & 0x03);

  //
  // Check to see if Address is aligned
  //
  if ((Address & (UINT64) (mInStride[Width] - 1)) != 0) {
    return RETURN_UNSUPPORTED;
  }

  //
  // Check to see if any address associated with this transfer exceeds the maximum
  // allowed address.  The maximum address implied by the parameters passed in is
  // Address + Size * Count.  If the following condition is met, then the transfer
  // is not supported.
  //
  //    Address + Size * Count > MAX_ADDRESS + 1
  //
  // Since MAX_ADDRESS can be the maximum integer value supported by the CPU and Count
  // can also be the maximum integer value supported by the CPU, this range
  // check must be adjusted to avoid all oveflow conditions.
  //
  // The following form of the range check is equivalent but assumes that
  // MAX_ADDRESS and MAX_IO_PORT_ADDRESS are of the form (2^n - 1).
  //
  Limit = MAX_ADDRESS;
  if (Count == 0) {
    if (Address > Limit) {
      return RETURN_UNSUPPORTED;
    }
  } else {
    MaxCount = RShiftU64 (Limit, Width);
    if (MaxCount < (Count - 1)) {
      return RETURN_UNSUPPORTED;
    }
    if (Address > LShiftU64 (MaxCount - Count + 1, Width)) {
      return RETURN_UNSUPPORTED;
    }
  }

  //
  // Check to see if Buffer is aligned
  // (IA-32 allows UINT64 and INT64 data types to be 32-bit aligned.)
  //
  if (((UINTN) Buffer & ((MIN (sizeof (UINTN), mInStride[Width])  - 1))) != 0) {
    return RETURN_UNSUPPORTED;
  }

  return RETURN_SUCCESS;
}
