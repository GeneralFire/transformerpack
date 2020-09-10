/** @file
  Interface of CSR register table library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 Intel Corporation. <BR>

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

#ifndef __CSR_REG_TABLE_LIB_H__
#define __CSR_REG_TABLE_LIB_H__

/**
  Add an entry in CSR register table.

  This function adds an entry in CSR register table, with register MmioAddr, bit section and value.
  Note: WriteCsrRegTable() only can be used before End of DXE.
        WriteCsrRegTable() only accepts MMIO address, user needs to call GetCpuCsrAddress() to covert CSR crif address to MMIO address.

  @param  MmioAddr           MMIO address of the register to program
  @param  ValidBitStart      Start of the bit section
  @param  ValidBitLength     Length of the bit section
  @param  Value              Value to write

  @retval EFI_SUCCESS            add an entry in CSR register table successfully.
  @retval EFI_OUT_OF_RESOURCES   The memory pages could not be allocated.
  @retval EFI_UNSUPPORTED        WriteCsrRegTable () is unsupported.

**/
EFI_STATUS
EFIAPI
WriteCsrRegTableBitField (
  IN UINT64               MmioAddr,
  IN UINT16               ValidBitStart,
  IN UINT16               ValidBitLength,
  IN UINT32               Value
  );

/**
  Add an entry in CSR register table.

  This function adds an entry in CSR register table, with register MmioAddr and value.
  Note: WriteCsrRegTable() only can be used before End of DXE.
        WriteCsrRegTable() only accepts MMIO address, user needs to call GetCpuCsrAddress() to covert CSR crif address to MMIO address.

  @param  MmioAddr           MMIO address of the register to program
  @param  ValidBitStart      Start of the bit section
  @param  ValidBitLength     Length of the bit section
  @param  Value              Value to write

  @retval EFI_SUCCESS            add an entry in CSR register table successfully.
  @retval EFI_OUT_OF_RESOURCES   The memory pages could not be allocated.
  @retval EFI_UNSUPPORTED        WriteCsrRegTable () is unsupported.

**/
EFI_STATUS
EFIAPI
WriteCsrRegTable (
  IN UINT64               MmioAddr,
  IN UINT32               Value
  );

/**
  Programs registers according to register table.

  This function Programs CSR according to CSR register table.

**/
VOID
EFIAPI
SaiCsrInBatchWrite (
  VOID
  );
#endif
