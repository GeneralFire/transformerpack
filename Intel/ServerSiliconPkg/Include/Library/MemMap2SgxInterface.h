/** @file
  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 Intel Corporation. <BR>

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


#ifndef __MEM_MAP_2_SGX_INTERFACE__
#define __MEM_MAP_2_SGX_INTERFACE__

#include <Uefi/UefiBaseType.h>

typedef UINT32 MEMORY_MODE_ATTRIBUTE;

#define MEMORY_MODE_ATTRIBUTE_1LM                BIT0
#define MEMORY_MODE_ATTRIBUTE_2LM                BIT1
#define MEMORY_MODE_ATTRIBUTE_2LM_XTILE          BIT2

/**
  Get current memory mode for a given socket.

  This API is invoked to get the memory mode for a socket and the bitmap of memory mode attribute is returned.

  If the socket contains 1LM region, MEMORY_MODE_ATTRIBUTE_1LM is set in the output bitmap in MemMode.

  If the socket contains 2LM region, MEMORY_MODE_ATTRIBUTE_2LM is set in the output bitmap in MemMode.

  If the socket is XTile 2LM configure, MEMORY_MODE_ATTRIBUTE_2LM_XTILE is set in the output bitmap in MemMode.


  @param[in]  SocketId           The socket index id

  @param[out] MemMode            The output bitmap for the memory mode attribute.`

  @retval EFI_SUCCESS            Successfully get the memory mode attribute in the output bitmap in MemMode.

  @retval EFI_NOT_READY          This function is invoked before memory map flow.

  @retval EFI_INVALID_PARAMETER  MemMode is NULL as input parameter.

  @retval EFI_INVALID_PARAMETER  Socket ID is invalid.
**/
EFI_STATUS
EFIAPI
GetMemMode (
  IN  UINT8                   SocketId,
  OUT MEMORY_MODE_ATTRIBUTE   *MemMode
  );

#endif // __MEM_MAP_2_SGX_INTERFACE__
