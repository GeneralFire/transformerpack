/** @file
  Provides data structure information used by SiliconIp MK-TME

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

//
// MK-TME
//
// NAK - Not a knob, used just for indication
UINT8  TmeCapability; // TME Capable
UINT8  TmeCrSupport; // Flag used to check if Crystal Ridge is supported in UEFI
UINT8  MktmeCapability; // MK-TME Capable
UINT16 MktmeMaxKeys; // Max number of keys used for encryption
UINT8  MkTmeKeyIdBits; // Used to suppress setup menu key-splits