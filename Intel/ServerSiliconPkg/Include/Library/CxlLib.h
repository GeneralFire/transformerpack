/** @file
  This is the library class header file for CxlLib.

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

#ifndef __CXL_LIB_H__
#define __CXL_LIB_H__

#include <Upi/KtiHost.h>

/**
  Routine to initialize the Compute Express Link(CXL).

  @param[in]  SocketData         - Pointer to socket specific data
  @param[in]  KtiInternalGlobal  - Pointer to the KTI RC internal global structure

  @retval KTI_SUCCESS
**/
KTI_STATUS
EFIAPI
InitializeComputeExpressLink (
  IN KTI_SOCKET_DATA            *SocketData,
  IN KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  );

/**
  Program SNC registers on CXL

  @param[in] KtiInternalGlobal - KTIRC Internal Global data
  @param[in] SocketId          - Socket

  @retval KTI_SUCCESS
**/
KTI_STATUS
EFIAPI
ProgramSncRegistersOnCxl (
  IN KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal,
  IN UINT8                      SocketId
  );

#endif // __CXL_LIB_H__
