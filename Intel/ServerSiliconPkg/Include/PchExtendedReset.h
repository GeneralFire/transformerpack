/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2011 - 2017 Intel Corporation. <BR>

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

#ifndef _EFI_PCH_EXTENDED_RESET_H_
#define _EFI_PCH_EXTENDED_RESET_H_

//BUGBUG This file was added in order to get LewisburgPkg to build with Purley. This needs to be looked at to make sure it's necessary.

//
// Extern the GUID for protocol users.
//
extern EFI_GUID                                 gEfiPchExtendedResetProtocolGuid;

//
// Forward reference for ANSI C compatibility
//
typedef struct _EFI_PCH_EXTENDED_RESET_PROTOCOL EFI_PCH_EXTENDED_RESET_PROTOCOL;

//
// Related Definitions
//
//
// PCH Extended Reset Types
//
typedef struct {
  UINT8 PowerCycleReset : 1;   // 0: Disabled*; 1: Enabled
  UINT8 GlobalReset : 1;  // 0: Disabled*; 1: Enabled
  UINT8 GlobalResetWithEc : 1;  // 0: Do Nothing;
                          // 1: GPIO[30](SUS_PWR_DN_ACK) level is set low prior to Global Reset(for systems with an embedded controller)
  UINT8 MaxRestReq : 5;     // Reserved fields for future expansion w/o protocol change
} PCH_EXTENDED_RESET_TYPES;

//
// Member functions
//
typedef
EFI_STATUS
(EFIAPI *EFI_PCH_EXTENDED_RESET) (
  IN     EFI_PCH_EXTENDED_RESET_PROTOCOL   * This,
  IN     PCH_EXTENDED_RESET_TYPES          PchExtendedResetTypes
  );

/**

  Execute Pch Extended Reset from the Host controller.

  @param This                    - Pointer to the EFI_PCH_EXTENDED_RESET_PROTOCOL instance.
  @param PchExtendedResetTypes   - Pch Extended Reset Types which includes PowerCycle, Globalreset.

  @retval Does not return if the reset takes place.
  @retval EFI_INVALID_PARAMETER   - If ResetType is invalid.

**/

//
// Interface structure for the Pch Extended Reset Protocol
//
struct _EFI_PCH_EXTENDED_RESET_PROTOCOL {
  EFI_PCH_EXTENDED_RESET  Reset;
};

#endif
