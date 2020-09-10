/** @file
  Interface of memory access library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2019 Intel Corporation. <BR>

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

#ifndef __MEM_ACCESS_LIB_H__
#define __MEM_ACCESS_LIB_H__


/**
  This API gets the memory controller register flat address.

  @param[in] MemTechType        Memory Technology Type
  @param[in] SocketId           CPU Socket Node number (Socket ID)
  @param[in] McId               Memory controller ID
  @param[in] CsrOffset          Register offset

  @retval                       The flat address
**/
UINTN
EFIAPI
GetMcRegisterAddress (
  IN  MEM_TECH_TYPE       MemTechType,
  IN  UINT8               SocketId,
  IN  UINT8               McId,
  IN  UINT32              CsrOffset
  );

/**
  This API gets the memory channel register flat address .

  @param[in] MemTechType        Memory Technology Type
  @param[in] SocketId           CPU Socket Node number (Socket ID)
  @param[in] ChId               Channel Id within a memory controller
  @param[in] CsrOffset          Register offset

  @retval                       The flat address
**/
UINTN
EFIAPI
GetChRegisterAddress (
  IN  MEM_TECH_TYPE       MemTechType,
  IN  UINT8               SocketId,
  IN  UINT8               ChId,
  IN  UINT32              CsrOffset
  );

/**
  This API gets the memory channel register flat address .

  @param[in] MemTechType        Memory Technology Type
  @param[in] SocketId           CPU Socket Node number (Socket ID)
  @param[in] IoId               HBM/DDR IO Id
  @param[in] CsrOffset          Register offset

  @retval                       The flat address
**/
UINTN
EFIAPI
GetIoRegisterAddress (
  IN  MEM_TECH_TYPE       MemTechType,
  IN  UINT8               SocketId,
  IN  UINT8               IoId,
  IN  UINT32              CsrOffset
  );

/**
  This API performs silicon CSR register read operations for instance number is McId.

  @param[in] MemTechType        Memory Technology Type
  @param[in] SocketId           CPU Socket Node number (Socket ID)
  @param[in] McId               Memory controller ID
  @param[in] CsrOffset          Register offset

  @retval UINT32                The data from CSR register returned.
**/
UINT32
EFIAPI
McRegisterRead (
  IN  MEM_TECH_TYPE      MemTechType,
  IN  UINT8              SocketId,
  IN  UINT8              McId,
  IN  UINT32             CsrOffset
  );

/**
  This API performs silicon CSR register read operations for instance number is ChId.

  @param[in] MemTechType        Memory Technology Type
  @param[in] SocketId           CPU Socket Node number (Socket ID)
  @param[in] ChId               Channel Id within a memory controller
  @param[in] CsrOffset          Register offset

  @retval UINT32                The data from CSR register returned.
**/
UINT32
EFIAPI
ChRegisterRead (
  IN  MEM_TECH_TYPE      MemTechType,
  IN  UINT8              SocketId,
  IN  UINT8              ChId,
  IN  UINT32             CsrOffset
  );

/**
  This API performs silicon CSR register read operations for instance number is IoId.

  @param[in] MemTechType        Memory Technology Type
  @param[in] SocketId           CPU Socket Node number (Socket ID)
  @param[in] IoId               HBM/DDR IO Id
  @param[in] CsrOffset          Register offset

  @retval UINT32                The data from CSR register returned.
**/
UINT32
EFIAPI
IoRegisterRead (
  IN  MEM_TECH_TYPE      MemTechType,
  IN  UINT8              SocketId,
  IN  UINT8              IoId,
  IN  UINT32             CsrOffset
  );

/**
  This API performs silicon CSR register write operations for instance number is McId.

  @param[in] MemTechType        Memory Technology Type
  @param[in] SocketId           CPU Socket Node number (Socket ID)
  @param[in] McId               Memory controller ID
  @param[in] CsrOffset          Register offset
  @param[in] Data32             The data needs to be written to CSR.

**/
VOID
EFIAPI
McRegisterWrite (
  IN  MEM_TECH_TYPE      MemTechType,
  IN  UINT8              SocketId,
  IN  UINT8              McId,
  IN  UINT32             CsrOffset,
  IN  UINT32             Data32
  );

/**
  This API performs silicon CSR register write operations for instance number is ChId.

  @param[in] MemTechType        Memory Technology Type
  @param[in] SocketId           CPU Socket Node number (Socket ID)
  @param[in] ChId               Channel Id within a memory controller
  @param[in] CsrOffset          Register offset
  @param[in] Data32             The data needs to be written to CSR.

**/
VOID
EFIAPI
ChRegisterWrite (
  IN  MEM_TECH_TYPE      MemTechType,
  IN  UINT8              SocketId,
  IN  UINT8              ChId,
  IN  UINT32             CsrOffset,
  IN  UINT32             Data32
  );

/**
  This API performs silicon CSR register write operations for instance number is IoId.

  @param[in] MemTechType        Memory Technology Type
  @param[in] SocketId           CPU Socket Node number (Socket ID)
  @param[in] IoId               HBM/DDR IO Id
  @param[in] CsrOffset          Register offset
  @param[in] Data32             The data needs to be written to CSR.

**/
VOID
EFIAPI
IoRegisterWrite (
  IN  MEM_TECH_TYPE      MemTechType,
  IN  UINT8              SocketId,
  IN  UINT8              IoId,
  IN  UINT32             CsrOffset,
  IN  UINT32             Data32
  );

#endif
