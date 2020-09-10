/** @file
  This file initialize VMD

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 Intel Corporation. <BR>

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

#ifndef _IIOVMDCONF_H_
#define _IIOVMDCONF_H_

/**
  Return TRUE if VMD is present on given stack (VMAssign register is readable)

  @param IioIndex   - index of the socket
  @param StackIndex - index of the stack

  @return True if VMD device is present, FALSE otherwise
 */
BOOLEAN
IsVmdDevicePresent (
  IN  UINT8  IioIndex,
  IN  UINT8  StackIndex
  );

/**
  Clear all VMD assigment - all devices are visible

  @param IioIndex   - index of the socket
  @param StackIndex - index of the stack
 */
VOID
ClearVmdAssigning (
    IN  UINT8  IioIndex,
    IN  UINT8  StackIndex
  );

/**
  Program VMD VmAssign register of current stack for given Device/Function

  @param IioIndex   - index of the socket
  @param StackIndex - index of the stack
  @param Device     - device number of the rootport
  @param Function   - function number of the rootport
  @param AssignToVmd - if TRUE assign device to VMD, unassigns otherwise
 */
VOID
AssigningDeviceToVmd (
  IN  UINT8    IioIndex,
  IN  UINT8    StackIndex,
  IN  UINT8    Device,
  IN  UINT8    Function,
  IN  BOOLEAN  AssignToVmd
  );

/**
  Configure VMD BARs for given stack

  @param IioIndex   - index of the socket
  @param StackIndex - index of the stack
 */
VOID
ConfigureVmdBars(
  IN  UINT8  IioIndex,
  IN  UINT8  StackIndex
  );

/**
  Program the VMD VmConfig register

  @param IioIndex   - index of the socket
  @param StackIndex - index of the stack
 */
VOID
SetVmdConfig (
  IN  UINT8  IioIndex,
  IN  UINT8  StackIndex
  );

/**
  Apply VMD workarounds for given stack

  @param IioIndex   - index of the socket
  @param StackIndex - index of the stack
 */
VOID
ApplyVmdWorkarounds (
  IN  UINT8  IioIndex,
  IN  UINT8  StackIndex
  );

#endif /* _IIOVMDCONF_H_ */
