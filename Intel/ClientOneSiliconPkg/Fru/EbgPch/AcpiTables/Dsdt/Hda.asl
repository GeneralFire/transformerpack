/**@file

 @copyright
  INTEL CONFIDENTIAL
  Copyright 1999 - 2019 Intel Corporation.

  The source code contained or described herein and all documents related to the
  source code ("Material") are owned by Intel Corporation or its suppliers or
  licensors. Title to the Material remains with Intel Corporation or its suppliers
  and licensors. The Material may contain trade secrets and proprietary and
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

  This file contains an 'Intel Peripheral Driver' and is uniquely identified as
  "Intel Reference Module" and is licensed for Intel CPUs and chipsets under
  the terms of your license agreement with Intel or your vendor. This file may
  be modified by the user, subject to additional terms of the license agreement.

@par Specification Reference:
**/

External(PS0X, MethodObj)
External(PS3X, MethodObj)
External(PPMS, MethodObj)
External(HIDW, MethodObj)
External(HIWC, MethodObj)

// Define a Memory Region that will allow access to the HDA PCI Configuration Space
//
OperationRegion(HDAR, PCI_Config, 0x00, 0x100)
Field(HDAR,WordAcc,NoLock,Preserve) {
  VDID,32       // 0x00, VID DID
}

Name(_S0W, 3) // Device can wake itself from D3 in S0

Method(_DSW, 3) {}

Method(_PRW, 0) {
  Return(GPRW(0x6D, 4)) // can wakeup from S4 state
}

// D0 Method for HD-A Controller
Method(_PS0, 0, Serialized)
{
  //
  // Call platform HDAS PS0 method if present
  //
  If(CondRefOf(PS0X)) {
    PS0X()
  }
}

// D3 Method for HD-A Controller
Method(_PS3, 0, Serialized)
{
  //
  // Call platform HDAS PS3 method if present
  //
  If(CondRefOf(PS3X)) {
    PS3X()
  }
}
