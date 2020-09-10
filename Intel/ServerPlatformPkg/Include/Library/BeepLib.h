/** @file
  Provides services to send progress/error codes to Beep device.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2014 - 2017 Intel Corporation. <BR>

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

#ifndef __BEEP_LIB_H__
#define __BEEP_LIB_H__

/**
  Sends a 32-bit value to Beep device.

  Sends the 32-bit value specified by Value to Beep device, and returns Value.
  Some implementations of this library function may perform I/O operations
  directly to Beep device.  Other implementations may send Value to
  ReportStatusCode(), and the status code reporting mechanism will eventually
  display the 32-bit value on the status reporting device.

  Beep() must actively prevent recursion.  If Beep() is called while
  processing another Post Code Library function, then
  Beep() must return Value immediately.

  @param  Value  Beep count.
**/
VOID
EFIAPI
Beep (
  IN UINT32  Value
  );

/**
  Sends a 32-bit value to Beep device.

  Sends the 32-bit value specified by Value to Beep device, and returns Value.
  Some implementations of this library function may perform I/O operations
  directly to Beep device.  Other implementations may send Value to
  ReportStatusCode(), and the status code reporting mechanism will eventually
  display the 32-bit value on the status reporting device.

  LongBeep() must actively prevent recursion.  If LongBeep() is called while
  processing another Post Code Library function, then
  LongBeep() must return Value immediately.

  @param  Value  Beep count.
**/
VOID
EFIAPI
LongBeep (
  IN UINT32  Value
  );

#endif  // __BEEP_LIB_H__
