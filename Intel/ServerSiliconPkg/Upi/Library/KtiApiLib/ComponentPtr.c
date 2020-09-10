/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 Intel Corporation. <BR>

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

#include "SysHost.h"
#include <Library/SystemInfoLib.h>
#include <Library/UpiHostDataLib.h>
#include <Library/KtiApi.h>
#include <Library/SysHostPointerLib.h>

/**

  Get Kti Port Count
  @retval    current KtiPortCnt

**/
UINT8
EFIAPI
GetKtiPortCnt (
  VOID
  )
{
  return KTI_HOST_OUT_PTR->KtiPortCnt;
}

/**

  Set Kti Port Count
  @param[in] KtiPortCnt     - KtiPortCnt to be set

**/
VOID
EFIAPI
SetKtiPortCnt (
  UINT8 KtiPortCnt
  )
{
  KTI_HOST_OUT_PTR->KtiPortCnt = KtiPortCnt;
}

/**

  Get Kti Port Bitmap
  @param[in] SocId     - Socket ID
  @retval    current KtiPortCnt

**/
UINT8
EFIAPI
GetKtiPortPresentBitmap (
  UINT8 SocId
  )
{
  return KTI_HOST_OUT_PTR->KtiPortPresentBitmap[SocId];
}

/**

  Return if the stack indexed is present or not
  @param[in] SocId    - Socket Index
  @param[in] StackId    - Stack Index
  @retval    TRUE       Stack present
  @retval    TRUE       Stack present
  @retval    FALSE      Stack not present
**/
BOOLEAN
EFIAPI
StackPresent (
  IN UINT8     SocId,
  IN UINT8     StackId
  )
{
  return ((KTI_HOST_OUT_PTR->stackPresentBitmap[SocId] & (BIT0 << StackId)) != 0) ? TRUE : FALSE;
}

/**

  Get current stackPresentBitMap
  @param[in] SocId    - Socket Index
  @retval    current stackPresentBitMap
**/
UINT16
EFIAPI
GetStackPresentBitMap (
  IN UINT8     SocId
  )
{
  return KTI_HOST_OUT_PTR->stackPresentBitmap[SocId];
}

/**

  Set stackPresentBitMap for requested Stack in requested Socket
  @param[in] SocId    - Socket Index
  @param[in] StackId    - Stack Index
**/
VOID
EFIAPI
SetStackPresent (
  IN UINT8     SocId,
  IN UINT8     StackId
  )
{
  KTI_HOST_OUT_PTR->stackPresentBitmap[SocId] |= (BIT0 << StackId);
}

/**

  Clear stackPresentBitMap for requested Stack in requested Socket
  @param[in] SocId    - Socket Index
  @param[in] StackId    - Stack Index
**/
VOID
EFIAPI
ClearStackPresent (
  IN UINT8     SocId,
  IN UINT8     StackId
  )
{
  KTI_HOST_OUT_PTR->stackPresentBitmap[SocId] &= ~(BIT0 << StackId);
}

/**

  Get StackBus for requested Stack in requested Socket
  @param[in] SocId    - Socket Index
  @param[in] StackId  - Stack Index

@retval                 BusNum
**/
UINT8
EFIAPI
GetStackBus (
  IN UINT8     SocId,
  IN UINT8     StackId
  )
{
  return KTI_HOST_OUT_PTR->StackBus[SocId][StackId];
}

/**

  Set StackBus for requested Stack in requested Socket
  @param[in] SocId    - Socket Index
  @param[in] StackId  - Stack Index

**/
VOID
EFIAPI
SetStackBus (
  IN UINT8     SocId,
  IN UINT8     StackId,
  IN UINT8     BusNum
  )
{
  KTI_HOST_OUT_PTR->StackBus[SocId][StackId] = BusNum;
}

/**

  Get Last Bus Number in requested Socket
  @param[in] SocId    - Socket Index

@retval                 BusNum
**/
UINT8
EFIAPI
GetSocketLastBus (
  IN UINT8     SocId
  )
{
  return KTI_HOST_OUT_PTR->SocketLastBus[SocId];
}

/**

  Set Last Bus Number in requested Socket
  @param[in] SocId    - Socket Index

**/
VOID
EFIAPI
SetSocketLastBus (
  IN UINT8     SocId,
  IN UINT8     BusNum
  )
{
  KTI_HOST_OUT_PTR->SocketLastBus[SocId] = BusNum;
}

/**

  Get First Bus Number in requested Socket
  @param[in] SocId    - Socket Index

@retval                 BusNum
**/
UINT8
EFIAPI
GetSocketFirstBus (
  IN UINT8     SocId
  )
{
  return KTI_HOST_OUT_PTR->SocketFirstBus[SocId];
}

/**

  Set First Bus Number in requested Socket
  @param[in] SocId    - Socket Index

**/
VOID
EFIAPI
SetSocketFirstBus (
  IN UINT8     SocId,
  IN UINT8     BusNum
  )
{
  KTI_HOST_OUT_PTR->SocketFirstBus[SocId] = BusNum;
}

