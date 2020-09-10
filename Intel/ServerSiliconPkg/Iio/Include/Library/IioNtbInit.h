/** IioNtbInit library header file.

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

#ifndef _IIO_NTB_INIT_LIB_H_
#define _IIO_NTB_INIT_LIB_H_

#define PPD_REG_SB_INVALID_VALUE      0x0

BOOLEAN
IsNtbSbActive (
  IN  UINT8                   IioIndex,
  IN  UINT8                   PortIndex
  );

VOID
NtbEarlyInit (
  IN  IIO_GLOBALS               *IioGlobalData,
  IN  UINT8                     IioIndex
  );

UINT64
NtbGetMbar01BaseAddress (
  UINT8                       IioIndex,
  UINT8                       PortIndex
  );

VOID
NtbInit (
  IN  IIO_GLOBALS             *IioGlobalData,
  IN  UINT8                   IioIndex
  );

VOID
NtbLateInit (
  IN  IIO_GLOBALS             *IioGlobalData,
  IN  UINT8                   IioIndex,
  IN  IIO_INIT_ENUMERATION    Phase
  );

VOID
NtbMiscCtrlStsInit (
  IN  UINT8                   IioIndex,
  IN  UINT8                   PortIndex
  );

VOID
NtbProblematicPortConfig (
  IN  UINT8                   IioIndex,
  IN  UINT8                   PortIndex
  );

VOID
NtbSetEmBar (
  IN  UINT8                   IioIndex,
  IN  UINT8                   PortIndex,
  IN  UINT8                   EmBarSize
  );

VOID
NtbSetImBar (
  IN  UINT8                   IioIndex,
  IN  UINT8                   PortIndex,
  IN  UINT8                   ImBarSize
  );

VOID
NtbSetMPL (
  IIO_GLOBALS                 *IioGlobalData,
  UINT8                       IioIndex,
  UINT8                       PortIndex
  );

VOID
NtbSetPpd (
  IN  UINT8                   IioIndex,
  IN  UINT8                   PortIndex,
  IN  UINT8                   Ppd,
  IN  UINT8                   CrossLinkOverride
);

VOID
NtbSetSplitBar (
  IN  UINT8                   IioIndex,
  IN  UINT8                   PortIndex,
  IN  UINT8                   SplitBar
  );

VOID
NtbSetSplitEmBar (
  IN  UINT8                   IioIndex,
  IN  UINT8                   PortIndex,
  IN  BOOLEAN                 SplitEnable,
  IN  UINT8                   Embar23Size,
  IN  UINT8                   Embar45Size
  );

VOID
NtbSetSplitImBar (
  IN  UINT8                   IioIndex,
  IN  UINT8                   PortIndex,
  IN  BOOLEAN                 SplitEnable,
  IN  UINT8                   Imbar23Size,
  IN  UINT8                   Imbar45Size
  );

VOID
NtbSetPowerState (
  UINT8                       IioIndex,
  UINT8                       PortIndex,
  UINT8                       PowerState
  );

VOID
PreNtbInitPortConfiguration (
  IIO_GLOBALS                 *IioGlobalData,
  UINT8                       IioIndex,
  UINT8                       PortIndex
  );

VOID
NtbLateWorkarounds (
  IIO_GLOBALS                 *IioGlobalData
);

VOID
ApplyWAS1506869315 (
  IIO_GLOBALS                 *IioGlobalData,
  UINT8                       IioIndex,
  UINT8                       PortIndex
);

VOID
NtbPersonalityLockEnable (
  IN  UINT8                   IioIndex,
  IN  UINT8                   PortIndex
  );

VOID
IioNtbMaxLinkWidth (
  IIO_GLOBALS                               *IioGlobalData,
  UINT8                                     IioIndex,
  UINT8                                     PortIndex,
  UINT8                                     DefaultMaxLinkWidth,
  UINT8                                     OverrideMaxLinkWidth
);

#endif // _IIO_NTB_INIT_LIB_H_
