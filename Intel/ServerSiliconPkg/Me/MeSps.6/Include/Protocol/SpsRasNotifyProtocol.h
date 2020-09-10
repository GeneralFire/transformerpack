/** @file
  Protocol for ME RAS Notification support.
  This protocol is used by RAS.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 Intel Corporation. <BR>

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

#ifndef _SPS_RAS_NOTIFY_PROTOCOL_H_
#define _SPS_RAS_NOTIFY_PROTOCOL_H_

#define HECI_RAS_TIMEOUT                            5    // HECI timeout in ms


typedef enum
{
  RasNotificationStart                    = 0x00,
  RasNotificationDone                     = 0x01,
  RasNotificationSerialPeciQuiescence     = 0x02,
  RasNotificationSerialPeciUnQuiescence   = 0x03,
  RasNotificationDmiQuiescence            = 0x04,
  RasNotificationDmiUnQuiescence          = 0x05
} RAS_NOTIFICATION_TYPE;



extern EFI_GUID gSpsRasNotifyProtocolGuid;

typedef struct _SPS_SMM_HMRFPO_PROTOCOL_ _SPS_SMM_HMRFPO_PROTOCOL;

typedef EFI_STATUS (EFIAPI *RAS_NOTIFY_FUNC)(RAS_NOTIFICATION_TYPE NotificationType);

typedef struct _SPS_RAS_NOTIFY_PROTOCOL {

  RAS_NOTIFY_FUNC     RasNotify;

} SPS_RAS_NOTIFY_PROTOCOL;

#endif // _SPS_RAS_NOTIFY_PROTOCOL_H_
