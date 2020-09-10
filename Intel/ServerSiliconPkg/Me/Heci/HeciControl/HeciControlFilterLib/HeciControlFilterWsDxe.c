/** @file
  HECI Control protocol driver filtering functionality for Workstation/Client ME (Dxe)

 @copyright
  INTEL CONFIDENTIAL
  Copyright (c) 2019 - 2020 Intel Corporation.

  The source code contained or described herein and all documents related to the
  source code ("Material") are owned by Intel Corporation or its suppliers or
  licensors. Title to the Material remains with Intel Corporation or its suppliers
  and licensors. The Material may contain trade secrets and proprietary and
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

  This file contains an 'Intel Peripheral Driver' and is uniquely identified as
  "Intel Reference Module" and is licensed for Intel CPUs and chipsets under
  the terms of your license agreement with Intel or your vendor. This file may
  be modified by the user, subject to additional terms of the license agreement.

**/

#include <PiDxe.h>
#include <Base.h>
#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Register/HeciRegs.h>
#include <MeAccess.h>
#include <BupMsgs.h>
#include <CoreBiosMsg.h>
#include <MkhiMsgs.h>

/**
  HECI Control filter procedure specific to WS ME provided by User

  This function returns HECI function number that message has been provided to

  @param[in]     This          HeciControl instance
  @param[in]     HeciPchId     PchId we want to communicate with 1..N
                               0 Id is default and
                               HeciControl decide where to route communication
  @param[in,out] HeciIn        Pointer to default HECI provided by HeciControl that will be filtered and returned
  @param[out]    HostAddress   Host address
  @param[out]    EngineAddress Engine (ME/IE) address
  @param[out]    MsgBody       Body of the message to send
  @param[in,out] MsgLen        On input buffer size, on exit message body size in bytes

  @retval EFI_SUCCESS           Success, send message
  @retval EFI_INVALID_PARAMETER Failure, invalid input parameters
  @retval EFI_DEVICE_ERROR      Failure, deny sending
**/
EFI_STATUS
WsFilterHeciMessage (
  IN     UINT32               HeciPchId,
  IN     UINTN                *DefaultHeci,
  IN     UINT8                HostAddress,
  IN     UINT8                EngineAddress,
  IN     UINT32               *MessageBody,
  IN     UINT32               MessageLength
  )
{
  MKHI_MESSAGE_HEADER         *MkhiHeader;
  HECI_FWS_REGISTER           MeHfs;

  MeHfs.ul = HeciPciRead32 (R_ME_HFS);

  if (MeHfs.r.CurrentState == ME_STATE_DISABLED) {
    MkhiHeader = (MKHI_MESSAGE_HEADER*) MessageBody;

    if (MkhiHeader != NULL) {
      if ((MkhiHeader->Fields.IsResponse == 0) &&
          (MkhiHeader->Fields.GroupId == BUP_COMMON_GROUP_ID) &&
          (MkhiHeader->Fields.Command == ME_ENABLE_CMD) &&
          (HostAddress == BIOS_FIXED_HOST_ADDR) &&
          (EngineAddress == HECI_MKHI_MESSAGE_ADDR)) {
        return EFI_SUCCESS;
      } else {
        return EFI_DEVICE_ERROR;
      }
    }
  }

  return EFI_SUCCESS;
}
