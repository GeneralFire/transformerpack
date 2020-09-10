/** @file
  PCH Platform Reset policy protocol produced by a platform driver providing
  interface to return extended PCH reset types. This protocol is consumed
  by the PCH drivers.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2010 - 2012 Intel Corporation. <BR>

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

#ifndef _PCH_PLATFORM_RESET_POLICY_H_
#define _PCH_PLATFORM_RESET_POLICY_H_

extern EFI_GUID                                   gDxePlatformResetPolicyProtocolGuid;

//
// Forward reference for ANSI C compatibility
//
typedef struct _DXE_PLATFORM_RESET_POLICY_PROTOCOL  DXE_PLATFORM_RESET_POLICY_PROTOCOL;

///
/// SPEICAL_RESET_TYPES must be aligned with PCH_EXTENDED_RESET_TYPES
///
typedef enum {
  PowerCycleResetReq  = 3,
  GlobalResetReq,
  GlobalResetWithEcReq,
  MaxRestReq
} PLT_SPECIAL_RESET_TYPES;


typedef
PLT_SPECIAL_RESET_TYPES
(EFIAPI *PLTFORM_RESET)(
  IN EFI_RESET_TYPE  Types
  )
/**

  Do the platform special reset.
  It doesn't return if platform decides to do special reset,
  otherwise the Types contains the PCH_EXTENDED_RESET TYPES for PCH to reset system.

  @param Type                  Pointer to PCH_EXTENDED_RESET_TYPES

**/
;


//
// ------------ General Platform Reset Policiy protocol definition ------------
//
struct _DXE_PLATFORM_RESET_POLICY_PROTOCOL {
  PLTFORM_RESET     PlatformReset;
};

#endif
