/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation. <BR>

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

#include <Library/HbmMemLib.h>
#include <Library/MemMcIpLib.h>

/**
  This function gets the HBM stack information for the specified socket.

  @param[in]  SocketId              Socket index.
  @param[out] HbmStackInfo          HBM stack information.

  @retval EFI_SUCCESS               This function got the register data successfully.
  @retval EFI_UNSUPPORTED           HBM is not supported on this socket.
  @retval EFI_INVALID_PARAMETER     Some of input parameters are invalid.
**/
EFI_STATUS
EFIAPI
GetHbmStackInfo (
  IN  UINT8               SocketId,
  OUT HBM_STACK_INFO      *HbmStackInfo
  )
{
  return EFI_SUCCESS;
}

/**
  Writes HBM Mesh2Mem TAD register.

  @param[in] SocketId              Socket index.
  @param[in] HbmStackId            HBM stack index within the socket.
  @param[in] HbmM2mTadData         Information required to program the TAD_WR CSR.

  @retval EFI_SUCCESS              This stub function always return success.

**/
EFI_STATUS
EFIAPI
WriteHbmM2mTadData (
  IN UINT8                SocketId,
  IN UINT8                HbmStackId,
  IN HBM_M2M_TAD_DATA     *HbmM2mTadData
  )
{
  return EFI_SUCCESS;
}

/**
  Writes HBM Mesh2Mem miscellaneous register(s).

  @param[in] SocketId              Socket index.
  @param[in] HbmStackId            HBM stack index within the socket.
  @param[in] HbmM2mMiscData        Information required to program the M2Mem Mode CSR.

  @retval EFI_SUCCESS              This stub function always return success.

**/
EFI_STATUS
EFIAPI
WriteHbmM2mMiscData (
  IN UINT8                SocketId,
  IN UINT8                HbmStackId,
  IN HBM_M2M_MISC_DATA    *HbmM2mMiscData
  )
{
  return EFI_SUCCESS;
}
