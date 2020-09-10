/** @file
  MeSmmLib.h

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

#ifndef _ME_SMM_LIB_H_
#define _ME_SMM_LIB_H_

#include <MkhiMsgs.h>

/**
  Send Get Firmware Version Request to ME

  @param[in][out] MsgGenGetFwVersionAckData   Return the message of FW version

  @exception EFI_UNSUPPORTED        Current ME mode doesn't support this function
  @retval    EFI_SUCCESS            Command succeeded
  @retval    EFI_DEVICE_ERROR       HECI Device error, command aborts abnormally
  @retval    EFI_TIMEOUT            HECI does not return the buffer before timeout
  @retval    EFI_BUFFER_TOO_SMALL   Message Buffer is too small for the Acknowledge
  @retval    EFI_INVALID_PARAMETER  Passed NULL pointer to function
**/
EFI_STATUS
GetMeFwVersionSmm (
  IN OUT GET_FW_VER_ACK_DATA  *MsgGenGetFwVersionAckData
  );

#endif // _ME_SMM_LIB_H_
