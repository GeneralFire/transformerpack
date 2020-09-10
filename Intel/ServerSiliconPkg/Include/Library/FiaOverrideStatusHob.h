/** @file
  FIA MUX OVERRIDE STATUS HOB definition.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation. <BR>

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

#ifndef _FIA_OVERRIDE_STATUS_HOB_H_
#define _FIA_OVERRIDE_STATUS_HOB_H_

extern EFI_GUID gFiaOverrideStatusHobGuid;

/**
  Definition of FIA OVERRIDE STATUS HOB
  Get/Set Status are defined by their respective Get/Set MeFiaMuxConfig functions called in PeiFiaMuxConfigInitLib.

  FIA MUX configuration is based on platform design and generally set by soft straps.
  FIA MUX configuration can change the generic lane assignment through the BIOS setup menu to increase flexibility.
  FIA_OVERRIDE_STATUS_HOB allows the FSP to provide the required feedback
    on the update of the FIA MUX configuration to the customer's bootloader.
**/
typedef struct {
  UINT32    FiaMuxConfigGetStatus;     // Status returned by MeFiaMuxConfigGet (MeFiaMuxLib)
  UINT32    FiaMuxConfigSetStatus;     // Status returned by MeFiaMuxConfigSet (MeFiaMuxLib)
  BOOLEAN   FiaMuxConfigSetRequired;   // Indicates that a set was required in the FIA lane assignment.
                                       //   FALSE = the requested config matched the existing one.
                                       //   TRUE  = the requested config did not match the existing one.
} FIA_OVERRIDE_STATUS_HOB;

#endif // _FIA_OVERRIDE_STATUS_HOB_H_

