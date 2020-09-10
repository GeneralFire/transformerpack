/** @file
  Interface definition details between ME and platform drivers during DXE phase.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2006 - 2020 Intel Corporation. <BR>

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

#ifndef _ME_POLICY_H_
#define _ME_POLICY_H_

#include <Config/MeConfig.h>

///
/// ME policy provided by platform for DXE phase
/// This protocol provides an interface to get Intel ME Configuration information
///

/**
  ME Policy Protocol \n
  All ME Policy Protocol change history listed here \n

  -<b> Revision 1</b>:
   - Initial version \n
  -<b> Revision 2</b>
   - Added MeJhiSupport \n
  -<b> Revision 3</b>
   - Added CoreBiosDoneEnabled \n
   - Deprecated NffCapabilityState \n
  -<b> Revision 1</b>:
   - Initial version Server side \n
  -<b> Revision 2</b>:
   - Remove AMT \n
**/
#define SRV_ME_POLICY_PROTOCOL_REVISION  2

extern EFI_GUID gDxeMePolicyGuid;

///
/// ME ERROR Message ID
///
typedef enum {
  MSG_EOP_ERROR             = 0,
  MSG_ME_FW_UPDATE_FAILED,
  MSG_ASF_BOOT_DISK_MISSING,
  MSG_KVM_TIMES_UP,
  MSG_KVM_REJECTED,
  MSG_HMRFPO_LOCK_FAILURE,
  MSG_HMRFPO_UNLOCK_FAILURE,
  MSG_ME_FW_UPDATE_WAIT,
  MSG_ILLEGAL_CPU_PLUGGED_IN,
  MSG_KVM_WAIT,
  MSG_PLAT_DISABLE_WAIT,
  MAX_ERROR_ENUM
} ME_ERROR_MSG_ID;

/**
  Show ME Error message. This is to display localized message in
  the console. This is used to display message strings in local
  language. To display the message, the routine will check the
  message ID and ConOut the message strings. For example, the
  End of Post error displayed in English will be:
  gST->ConOut->OutputString (gST->ConOut, L"Error sending End Of
  Post message to ME\n"); It is recommended to clear the screen
  before displaying the error message and keep the message on
  the screen for several seconds.
  A sample is provided, see ShowSrvMeReportError () to retrieve
  details.

  @param[in] MsgId                ME error message ID for displaying on screen message

**/
typedef
VOID
(EFIAPI *ME_REPORT_ERROR) (
  IN ME_ERROR_MSG_ID              MsgId
  );

///
/// ME DXE Policy
/// This protocol provides information of the current Intel ME feature selection. Information is
/// passed from the platform code to the Intel ME Reference code using this structure. There are
/// 2 types of information, BIOS setup option and ME status information.
///
typedef struct _SRV_ME_POLICY_PROTOCOL {
  ///
  /// Revision for the protocol structure
  ///
  UINT32                          Revision;

  ME_CONFIG                       MeConfig;
  ///
  /// Runtime configuration
  ///
  UINT32                          EndOfPostDone : 1;
  UINT32                          DelayedAuthenticationModeOverride      : 1; // DAM override option
  UINT32                          DelayedAuthenticationMode              : 8; // DAM option

  ///
  /// Support Localization for displaying on screen message
  ///
  ME_REPORT_ERROR                 MeReportError;

} SRV_ME_POLICY_PROTOCOL;

#endif // _ME_POLICY_H_
