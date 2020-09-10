/** @file
  HECI Control protocol driver filtering functionality for SPS ME (Smm)

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
#include <Library/DebugLib.h>
#include <Protocol/HeciControlProtocol.h>
#include <Library/HeciCommonLib.h>
#include <Library/MeUtilsLib.h>
#include <Register/MeRegs.h>
#include <Sps.h>


#define HECI_CONTROL_DEBUG   "[HECI Control %s]"

//
// Global driver data
//

extern CHAR16                *mHeciModuleDebugPrefix;

BOOLEAN
GetMeHmrfpoViaHeci3 (
  VOID
  );

/**
  HECI Control filter procedure specific to SPS provided by User

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

  @retval EFI_SUCCESS            Success, send message
  @retval EFI_INVALID_PARAMETER  Failure, invalid input parameters
  @retval EFI_DEVICE_ERROR       Failure, deny sending
**/
EFI_STATUS
SpsFilterHeciMessage (
  IN     UINT32               HeciPchId,
  IN     UINTN                *DefaultHeci,
  IN     UINT8                HostAddress,
  IN     UINT8                EngineAddress,
  IN     UINT32               *MessageBody,
  IN     UINT32               MessageLength
  )
{
  MKHI_MESSAGE_HEADER           *MkhiHeader;
  HECI_RAS_NOTIFY_REQ           *RasRequest;
  EFI_STATUS                    Status;

  if (DefaultHeci == NULL) {
    ASSERT (DefaultHeci != NULL);
    return EFI_INVALID_PARAMETER;
  }

  if (IsInSmm (NULL)) {
    DEBUG ((DEBUG_INFO, HECI_CONTROL_DEBUG, mHeciModuleDebugPrefix));
    DEBUG ((
      DEBUG_INFO,
      " %a() (default HECI function %d)\n",
      __FUNCTION__,
      *DefaultHeci
      ));

    MkhiHeader = (MKHI_MESSAGE_HEADER*)MessageBody;
    RasRequest = (HECI_RAS_NOTIFY_REQ*)MessageBody;
    if (MkhiHeader != NULL) {
      if ((MkhiHeader->Fields.IsResponse == 0) &&
          (MkhiHeader->Fields.GroupId == MKHI_HMRFPO_GROUP_ID) &&
          (HostAddress == SPS_CLIENTID_BIOS) &&
          (EngineAddress == SPS_CLIENTID_ME_MKHI)) {
        DEBUG ((DEBUG_INFO, HECI_CONTROL_DEBUG, mHeciModuleDebugPrefix));
        DEBUG ((DEBUG_INFO, " Filtering command %d\n", MkhiHeader->Fields.Command));
        if (GetMeHmrfpoViaHeci3 ()) {
          *DefaultHeci = HECI3_DEVICE;
        } else {
          *DefaultHeci = HECI1_DEVICE;
        }
      } else if ((RasRequest->Cmd == RAS_NOTIFY_REQ) &&
                 (HostAddress == SPS_CLIENTID_BIOS) &&
                 (EngineAddress == SPS_CLIENTID_ME_RAS)) {
        *DefaultHeci = HECI3_DEVICE;
      }
    }
    Status = EFI_SUCCESS;
    DEBUG ((DEBUG_INFO, HECI_CONTROL_DEBUG, mHeciModuleDebugPrefix));
    DEBUG ((
      DEBUG_INFO,
      " %a() exit (selected HECI function %d)\n",
      __FUNCTION__,
      *DefaultHeci
      ));
  } else {
    *DefaultHeci = MAX_UINTN;
    Status = EFI_DEVICE_ERROR;
  }

  return Status;
}
