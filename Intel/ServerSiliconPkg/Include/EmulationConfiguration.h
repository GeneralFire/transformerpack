/** @file

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

#ifndef _EMULATION_CONFIGURATION_H_
#define _EMULATION_CONFIGURATION_H_

#define EMULATION_CLASS_ID                        45
#define EMULATION_SUBCLASS_ID                     0x00
#define VFR_FORMID_EMULATION_SETUP                0x48D0
#define VFR_FORMLABLE_EMULATION_SETUP_TOP         0x8b10
#define VFR_FORMLABLE_EMULATION_SETUP_BOTTOM      0x8b11

// {52B3B56E-E716-455f-A5E3-B314F18E6C5D}
#define EMULATION_FORMSET_GUID \
  { \
  0x52b3b56e, 0xe716, 0x455f, 0xa5, 0xe3, 0xb3, 0x14, 0xf1, 0x8e, 0x6c, 0x5d \
  }

// {41266EAD-701C-461f-A6B1-42CD387D1A6E}
#define EMULATION_CONFIGURATION_GUID \
  { \
  0x41266ead, 0x701c, 0x461f, 0xa6, 0xb1, 0x42, 0xcd, 0x38, 0x7d, 0x1a, 0x6e \
  }

//
// Emulation Setting Values
//
#define EMULATION_AUTO           0
#define EMULATION_DISABLE        1
#define EMULATION_ENABLE         2

extern EFI_GUID gEmulationHobGuid;

#endif // _EMULATION_CONFIGURATION_H_



