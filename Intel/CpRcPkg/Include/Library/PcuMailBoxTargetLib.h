/** @file
  Interface header for Bios to Pcode Mailbox abstraction library class

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

#ifndef _BIOS_TO_PCODE_MAILBOX_TARGET_LIB_H_
#define _BIOS_TO_PCODE_MAILBOX_TARGET_LIB_H_

/**
  Abstraction function to delay for Pcode mailbox Busy flag to get cleared.

  @param - None

  @retval EFI_SUCCESS           Delayed successfully.
  @retval !EFI_SUCCESS          Error.
**/

EFI_STATUS
EFIAPI
ClearRunBusyFlagDelayTarget (
  VOID
  );

/**
  Abstraction function to clear and check Pcode Mailbox Busy flag

  @param[in]  Socket            Socket number
  @param[in]  RegOffset         Register offset
  @param[out] *MailBoxStatus    Ptr to PCU Mailbox status

  @retval EFI_SUCCESS           Clearned RUN BUSY flag in PCU successfully
  @retval EFI_TIMEOUT           Timeout reached when trying to clear RUN_BUSY flag in PCU.
**/

EFI_STATUS
EFIAPI
ClearRunBusyFlagTarget (
  IN UINT8 Socket,
  IN UINT32 RegOffset,
  OUT UINT32 *MailBoxStatus
  );

/**
  Abstraction function to check the Pcode mailbox status bit and log error if necessary

  @param[in]  MailBoxStatus     Pcode mailbox status bit to check

  @retval none
**/

EFI_STATUS
EFIAPI
CheckMailboxStatusTarget (
  IN UINT32 MailBoxStatus
  );

#endif  // _BIOS_TO_PCODE_MAILBOX_TARGET_LIB_H_
