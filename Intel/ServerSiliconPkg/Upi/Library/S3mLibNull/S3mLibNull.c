/** @file
  S3M library.

  This file implements the S3mLib APIs.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 - 2020 Intel Corporation. <BR>

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

//
// Statements that include other files
//
#include <Library/S3mLib.h>

/**
  Collect CPU strap contents from all sockets.

  @param SocketData        - Pointer to socket specific data
  @param KtiInternalGlobal - KTIRC Internal Global data

  @retval KTI_SUCCESS        Collect S3M CPU Strap contents successfully
  @retval KTI_FAILURE        Failed to collect S3M CPU Strap contents
  @retval KTI_IGNORE         Not supported
**/
KTI_STATUS
EFIAPI
CollectS3mCpuStrapContents (
  KTI_SOCKET_DATA            *SocketData,
  KTI_HOST_INTERNAL_GLOBAL   *KtiInternalGlobal
  )
{
  return KTI_IGNORE;
}

/**
  S3M CPU strap provision flow main entry.

  @param            - VOID

  @retval           KTI_SUCCESS
  @retval           KTI_IGNORE   Feature not supported
**/
KTI_STATUS
EFIAPI
S3mCpuStrapProvision (
  VOID
  )
{
  return KTI_IGNORE;
}

/**
  S3M CFR patch provision flow main entry.

  @param PatchType- S3M_FW_PATCH: S3m Fw CFR
                    P_U_CODE_PATCH: Pcode/Ucode CFR

  @retval           KTI_SUCCESS
  @retval           KTI_IGNORE   Feature not supported
**/
KTI_STATUS
EFIAPI
S3mCfrPatchProvision (
  UINTN           PatchType
  )
{
  return KTI_IGNORE;
}
