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

#include <Library/UsraCsrLib.h>
#include "Include/Mesh2MemRegs.h"

/**
  This function sets the DisCritChunk bit field of DEFEATURES0_M2MEM_MAIN_REG register.

  @param[in] SocketId         Socket index.
  @param[in] McId             Memory controller index on socket.
  @param[in] DisCritChunk     Flag indicates if critical chunk needs to be disabled.

  @retval N/A
**/
VOID
EFIAPI
SetM2mDefeatures0DiscritChunk (
  IN UINT8       SocketId,
  IN UINT8       McId,
  IN BOOLEAN     DisCritChunk
  )
{
  DEFEATURES0_M2MEM_MAIN_STRUCT   M2MemDefeatures0;

  M2MemDefeatures0.Data = UsraCsrRead (SocketId, McId, DEFEATURES0_M2MEM_MAIN_REG);
  M2MemDefeatures0.Bits.discritchunk = DisCritChunk;
  UsraCsrWrite (SocketId, McId, DEFEATURES0_M2MEM_MAIN_REG, M2MemDefeatures0.Data);
}
