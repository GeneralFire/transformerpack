/** @file
  Interface header for Bios to VCU Mailbox library

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2020 Intel Corporation. <BR>

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

#ifndef _VCU_MAIL_BOX_LIB_H_
#define _VCU_MAIL_BOX_LIB_H_

#include <ReferenceCodeDataTypes.h>

//
// -----------------------------------------------------------------------------
// BIOS to VCode Mailbox Commands
//

#define VCODE_API_OPCODE_NOP                   0x0
#define VCODE_API_OPCODE_OPEN_SEQUENCE         0x3
#define VCODE_API_OPCODE_SET_PARAMETER         0x1
#define VCODE_API_OPCODE_CLOSE_SEQUENCE        0x4
#define VCODE_API_OPCODE_IOT_LLC_SETUP         0x1007
#define VCODE_API_OPCODE_IOT_REG_READ          0x1003
#define VCODE_API_OPCODE_IOT_REG_WRITE         0x1006
#define VCODE_API_OPCODE_READ_LOCAL_CSR        0x0010
#define VCODE_API_OPCODE_READ_DATA             0x0002
#define VCODE_API_OPCODE_WRITE_LOCAL_CSR       0x0011
#define VCODE_API_OPCODE_WRITE_DATA            0x0006

#define VCODE_API_SEQ_ID_IOT_LLC_SETUP 0x1000e
#define VCODE_API_SEQ_ID_OPEN_SEQUENCE 0x0

#define VCODE_API_SEQ_ID_IOT_TRACE_DUMP_SETUP 0x1000c
#define VCODE_API_OPCODE_IOT_TRACE_DUMP_SETUP 0x1005

#define VCODE_API_SEQ_ID_READ_LOCAL_CSR       0x0001
#define VCODE_API_SEQ_ID_WRITE_LOCAL_CSR      0x0002


// command return code
#define VCODE_MAILBOX_CC_SUCCESS            0x40     // Successful completion
#define VCODE_MAILBOX_CC_TIMEOUT            0x80     // Time-out.
#define VCODE_MAILBOX_CC_THREAD_UNAVAILABLE 0x82     // Thread Unavailable.
#define VCODE_MAILBOX_CC_ILLEGAL            0x90     // Illegal.

#define VCODE_MAILBOX_PECI_CC_SUCCESS 0x40     // Successful completion
#define VCODE_MAILBOX_PECI_CC_ILLEGAL 0x90     // Illegal

#define BIOS_VCU_MAILBOX_TIMEOUT_RETRY 3      // Retry count used by BIOS

/**
  First  writes the given 32-bit data to VCU MAILBOX_DATA CSR,
  Then writes the given 32-bit Command to VCU MAILBOX_INTERFACE CSR,

  @param[in] socket       - CPU Socket number (Socket ID)
  @param[in] dwordCommand - VCU mailbox command to be written
  @param[in] dworddata    - VCU mailbox data to be written

  @retval 64-bit return code from the VCU mailbox:
  @retval Lo Dword
  @retval [15:0] = Command Response Code (success, timeout, etc)
  @retval [31:16] = rsvd
  @retval Hi Dword
  @retval [31:0] = Return data if applicable
**/

UINT64_STRUCT
EFIAPI
WriteBios2VcuMailboxCommand (
  IN UINT8  socket,
  IN UINT32 dwordCommand,
  IN UINT32 dworddata
  );

#endif  // _VCU_MAIL_BOX_LIB_H_
