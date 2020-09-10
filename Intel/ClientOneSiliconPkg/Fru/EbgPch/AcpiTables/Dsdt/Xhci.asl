/**@file

 @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 - 2020 Intel Corporation.

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

#include <Register/UsbRegs.h>

//
// Platform hooks definitions for XHCI controller flows.
// Platform can implement them to insert platform specific logic during
// certain XHCI power flows.
//
External(PS0X, MethodObj) // Called at the end of controller's _PS0
External(PS3X, MethodObj) // Called at the end of controller's _PS3
External(RHUB.PS0X, MethodObj) // Called at the end of root hub's _PS0
External(RHUB.PS2X, MethodObj) // Called at the end of the root hub's _PS2
External(RHUB.PS3X, MethodObj) // Called at the end of the root hub's _PS3
External(RHUB.INIR, MethodObj) // Called at the end of the controller _INI

OperationRegion(XPRT,PCI_Config,0x00,0x100)
Field(XPRT,AnyAcc,NoLock,Preserve)
{
  DVID, 16,
  Offset(0x10),
  XADL,  32,
  XADH,  32,
  Offset(R_XHCI_CFG_PCE), // 0xA2
      ,  2,
  D3HE,  1,  // D3_hot_en
}

Method(_S3D, 0, NotSerialized) {
  Return(3)
}

Method(_S4D, 0, NotSerialized) {
  Return(3)
}

Method(_S3W, 0, NotSerialized) {
  Return(3)
}

Method(_S4W, 0, NotSerialized) {
  Return(3)
}

Method(_S0W, 0x0, NotSerialized) {
  Return(Zero)
}

Method(_PRW, 0) {
  Return(GPRW(0x6D, 4)) // can wakeup from S4 state
}

Method(_DSM,4,serialized){
  ADBG("XHCI _DSM")
  If(PCIC(Arg0)) { return(PCID(Arg0,Arg1,Arg2,Arg3)) }
  Return(Buffer() {0})
}

Method(_INI) {
  // _INI for RTD3 run conditionally if implemented in platform specific code
  If(CondRefOf(^RHUB.INIR)) {
    ^RHUB.INIR()
  }
}

Method(_PS0,0,Serialized) {

  If(LEqual(DVID,0xFFFF)) {
    Return()
  }

  //
  // Call platform XHC PS0 method if present
  //
  If(CondRefOf(PS0X)) {
    PS0X()
  }
}

Method(_PS3,0,Serialized) {

  If(LEqual(DVID,0xFFFF)) {
    Return()
  }

  //
  // Call platform XHC PS3 method if present
  //
  If(CondRefOf(PS3X)) {
    PS3X()
  }
}

Device(RHUB) {
  Name(_ADR, Zero)

  Method(_PS0,0,Serialized) {
   If(LEqual(^^DVID,0xFFFF)) {
      Return()
    }
    //
    // Call platform XHC.RHUB PS0 method if present.
    //
    If(CondRefOf(PS0X)) {
      PS0X()
    }
  }

  Method(_PS2,0,Serialized) {
    If(LEqual(^^DVID,0xFFFF)) {
      Return()
    }
    //
    // Call platform XHC.RHUB PS2 method if present.
    //
    If(CondRefOf(PS2X)) {
      PS2X()
    }
  }

  Method(_PS3,0,Serialized) {
    If(LEqual(^^DVID,0xFFFF)) {
      Return()
    }
    //
    // Call platform XHC.RHUB PS3 method if present.
    //
    If(CondRefOf(PS3X)) {
      PS3X()
    }
  }

  //
  // High Speed Ports (without USBR)
  //
  Device(HS01) {
    Name(_ADR, 0x01)
  }

  Device(HS02) {
    Name(_ADR, 0x02)
  }

  Device(HS03) {
    Name(_ADR, 0x03)
  }

  Device(HS04) {
    Name(_ADR, 0x04)
  }

  Device(HS05) {
    Name(_ADR, 0x05)
  }

  Device(HS06) {
    Name(_ADR, 0x06)
  }

  Device(HS07) {
    Name(_ADR, 0x07)
  }

  Device(HS08) {
    Name(_ADR, 0x08)
  }

  Device(HS09) {
    Name(_ADR, 0x09)
  }

  Device(HS10) {
    Name(_ADR, 0x0A)
  }

  Device(HS11) {
    Name(_ADR, 0x0B)
  }

  Device(HS12) {
    Name(_ADR, 0x0C)
  }

  Device(HS13) {
    Name(_ADR, 0x0D)
  }

  Device(HS14) {
    Name(_ADR, 0x0E)
  }

  //
  // USBR ports definitions for supporting PCH
  //
  Device(USR1) {
    Method(_ADR) {Return (USRA())}
  }

  Device(USR2) {
    Method(_ADR) {Return (Add(USRA(),1))}
  }

  //
  // Super Speed Ports
  //
  Device(SS01) {
    Method(_ADR) { Return (Add(SSPA(),0)) }
  }

  Device(SS02) {
    Method(_ADR) { Return (Add(SSPA(),1)) }
  }

  Device(SS03) {
    Method(_ADR) { Return (Add(SSPA(),2)) }
  }

  Device(SS04) {
    Method(_ADR) { Return (Add(SSPA(),3)) }
  }

  Device(SS05) {
    Method(_ADR) { Return (Add(SSPA(),4)) }
  }

  Device(SS06) {
    Method(_ADR) { Return (Add(SSPA(),5)) }
  }

  Device(SS07) {
    Method(_ADR) { Return (Add(SSPA(),6)) }
  }

  Device(SS08) {
    Method(_ADR) { Return (Add(SSPA(),7)) }
  }

  Device(SS09) {
    Method(_ADR) { Return (Add(SSPA(),8)) }
  }

  Device(SS10) {
    Method(_ADR) { Return (Add(SSPA(),9)) }
  }
}

Method(_DSW, 3) {}

