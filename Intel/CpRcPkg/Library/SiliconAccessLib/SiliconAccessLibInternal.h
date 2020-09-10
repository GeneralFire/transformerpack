/** @file

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

#ifndef __SILICON_ACCESS_LIB_INTERNAL_H__
#define __SILICON_ACCESS_LIB_INTERNAL_H__

#include <Base.h>
#include <Uefi.h>
#include <Library/SiliconAccessLib.h>
#include <Library/CsrAddressTranslationLib.h>
#include <Library/PcieMmcfgLib.h>
#include <Library/BaseLib.h>
#include <Library/IoLib.h>
#include <Library/S3BootScriptLib.h>
#include <Library/USRATrace.h>
#include <Library/UsraRegisterLogLib.h>
#include <Library/UsraRegisterFilterLib.h>
#include <Library/CsrSbPortIdLib.h>
#include <Library/SystemInfoLib.h>


#define MAX_IO_PORT_ADDRESS   0xFFFF

//
// Get Aligned Access Address
//
typedef
VOID
(EFIAPI *GET_ALIGNED_ACCESS_ADDRESS) (USRA_ADDRESS*, UINTN*);

//
// Register Read
//
typedef
RETURN_STATUS
(EFIAPI *REGISTER_READ) (UINT32, UINTN, VOID *);

//
// Register Write
//
typedef
RETURN_STATUS
(EFIAPI *REGISTER_WRITE) (UINT32, UINTN, VOID *);

/**
  This API get the Pcie address from the given USRA Address.

  @param[in] Address              A pointer of the address of the USRA Address Structure to be read out
  @param[out] AlignedAddress      To be added.

**/
VOID
EFIAPI
GetPcieAccessAddress (
  IN USRA_ADDRESS             *Address,
  OUT UINTN                   *AlignedAddress
  );

/**
  This API gets the CSR address from the given USRA Address.

  @param[in] Address              A pointer of the address of the USRA Address Structure to be read out
  @param[out] AlignedAddress      To be added.

**/
VOID
EFIAPI
GetCsrAccessAddress (
  IN USRA_ADDRESS             *Address,
  OUT UINTN                   *AlignedAddress
  );

/**
  This API gets the CSR Mem address from the given USRA Address.

  @param[in] Address              A pointer of the address of the USRA Address Structure
  @param[out] AlignedAddress      A pointer of aligned address converted from USRA address

**/
VOID
EFIAPI
GetCsrMemAccessAddress (
  IN USRA_ADDRESS             *Address,
  OUT UINTN                   *AlignedAddress
  );

/**
  This API gets the CSR Cfg address from the given USRA Address.

  @param[in] Address              A pointer of the address of the USRA Address Structure
  @param[out] AlignedAddress      A pointer of aligned address converted from USRA address

**/
VOID
EFIAPI
GetCsrCfgAccessAddress (
  IN USRA_ADDRESS             *Address,
  OUT UINTN                   *AlignedAddress
  );

/**
  This API gets the PciIo address from the given USRA Address.

  @param[in] Address              A pointer of the address of the USRA Address Structure
  @param[out] AlignedAddress      A pointer of aligned address converted from USRA address

**/
VOID
EFIAPI
GetPciIoAccessAddress (
  IN USRA_ADDRESS             *Address,
  OUT UINTN                   *AlignedAddress
  );

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
  );

/**
  Perform MMIO write

  @param[in] AccessWidth          Access Width
  @param[in] AlignedAddress       An address to be written
  @param[out] Buffer               A pointer of buffer contains the data to be written

  @retval RETURN_SUCCESS          The function completed successfully.
**/
RETURN_STATUS
EFIAPI
UsraRegAlignedWrite (
  IN UINT32                   AccessWidth,
  IN UINTN                    AlignedAddress,
  OUT VOID                    *Buffer
  );

/**
  Perform PCIIO read

  @param[in] AccessWidth          Access Width
  @param[in] AlignedAddress       An address to be read out
  @param[in] Buffer               A pointer of buffer contains the data to be read out

  @retval RETURN_SUCCESS          The function completed successfully.
**/
RETURN_STATUS
UsraPciIoAlignedRead (
  IN UINT32                   AccessWidth,
  IN UINTN                    AlignedAddress,
  OUT VOID                    *Buffer
  );

/**
  Perform PCIIO write

  @param[in] AccessWidth          Access Width
  @param[in] AlignedAddress       An address to be read out
  @param[in] Buffer               A pointer of buffer contains the data to be read out

  @retval RETURN_SUCCESS          The function completed successfully.
**/
RETURN_STATUS
UsraPciIoAlignedWrite (
  IN UINT32                   AccessWidth,
  IN UINTN                    AlignedAddress,
  OUT VOID                    *Buffer
  );

/**
  Perform AND then OR operations for a input data

  @param[in] Data             A pointer of the address of the register to be modified
  @param[in] AndBuffer            A pointer of buffer for the value used for AND operation
                                  A NULL pointer means no AND operation. RegisterModify() equivalents to RegisterOr()
  @param[in] OrBuffer             A pointer of buffer for the value used for OR operation
                                  A NULL pointer means no OR operation. RegisterModify() equivalents to RegisterAnd()
  @param[in] NumOfByte            NumOfByte Count of byte data to be performed

**/
VOID
EFIAPI
DataAndOr (
  IN  UINT64                  *Data,
  IN  VOID                    *AndBuffer,
  IN  VOID                    *OrBuffer,
  IN  UINT8                   NumOfByte
  );

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
  );

#endif //#ifndef __SILICON_ACCESS_LIB_INTERNAL_H__
