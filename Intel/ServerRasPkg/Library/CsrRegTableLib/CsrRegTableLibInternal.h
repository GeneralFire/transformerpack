/** @file
  Inernal Interface of CSR register table library.

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

#ifndef __CSR_REG_TABLE_LIB_INTERNAL_H__
#define __CSR_REG_TABLE_LIB_INTERNAL_H__

//
// Element of CSR register table entry
//
typedef struct {
  UINT64 MmioAddr;
  UINT16 ValidBitStart;
  UINT16 ValidBitLength;
  UINT32 Value;
} CSR_REGISTER_TABLE_ENTRY;

//
// CSR Register table definition, including current table length,
// allocated size of this table, and pointer to the list of table entries.
//
typedef struct {
  //
  // The number of valid entries in the RegisterTableEntry buffer
  //
  UINT32 TableLength;
  //
  // The size, in bytes, of the RegisterTableEntry buffer
  //
  UINT32 AllocatedSize;
  //
  // Pointer of CSR_REGISTER_TABLE_ENTRY structures.
  //
  CSR_REGISTER_TABLE_ENTRY *CsrRegisterTableEntry;
} CSR_REGISTER_TABLE;
#endif
