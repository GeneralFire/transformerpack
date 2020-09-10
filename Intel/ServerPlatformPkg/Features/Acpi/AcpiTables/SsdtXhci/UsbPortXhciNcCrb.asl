/** @file
  ACPI SSDT table

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 Intel Corporation. <BR>

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

DefinitionBlock (
    "UsbPortXhciNcCrb.aml",
    "SSDT",
    0x02, // SHAD revision.
        // A Revision field value greater than or equal to 2 signifies that integers 
        // declared within the Definition Block are to be evaluated as 64-bit values
    "INTEL", // OEM ID (6 byte string)
    "xh_nccrb", // OEM table ID  (8 byte string)
    0x0 // OEM version of DSDT table (4 byte Integer)
    )
{

  External(\_SB.PC00.XHCI.RHUB, DeviceObj)

  External(\_SB.PC00.XHCI.RHUB.HS01, DeviceObj)
  External(\_SB.PC00.XHCI.RHUB.HS02, DeviceObj)
  External(\_SB.PC00.XHCI.RHUB.HS03, DeviceObj)
  External(\_SB.PC00.XHCI.RHUB.HS04, DeviceObj)
  External(\_SB.PC00.XHCI.RHUB.HS05, DeviceObj)
  External(\_SB.PC00.XHCI.RHUB.HS06, DeviceObj)
  External(\_SB.PC00.XHCI.RHUB.HS07, DeviceObj)
  External(\_SB.PC00.XHCI.RHUB.HS08, DeviceObj)
  External(\_SB.PC00.XHCI.RHUB.HS09, DeviceObj)
  External(\_SB.PC00.XHCI.RHUB.HS10, DeviceObj)
  External(\_SB.PC00.XHCI.RHUB.HS11, DeviceObj)
  External(\_SB.PC00.XHCI.RHUB.HS12, DeviceObj)
  External(\_SB.PC00.XHCI.RHUB.HS13, DeviceObj)
  External(\_SB.PC00.XHCI.RHUB.HS14, DeviceObj)
  
  External(\_SB.PC00.XHCI.RHUB.SS01, DeviceObj)
  External(\_SB.PC00.XHCI.RHUB.SS02, DeviceObj)
  External(\_SB.PC00.XHCI.RHUB.SS03, DeviceObj)
  External(\_SB.PC00.XHCI.RHUB.SS04, DeviceObj)
  External(\_SB.PC00.XHCI.RHUB.SS05, DeviceObj)
  External(\_SB.PC00.XHCI.RHUB.SS06, DeviceObj)
  External(\_SB.PC00.XHCI.RHUB.SS07, DeviceObj)
  External(\_SB.PC00.XHCI.RHUB.SS08, DeviceObj)
  External(\_SB.PC00.XHCI.RHUB.SS09, DeviceObj)
  External(\_SB.PC00.XHCI.RHUB.SS10, DeviceObj)
  
  External(\_SB.PC00.XHCI.RHUB.USR1, DeviceObj)
  External(\_SB.PC00.XHCI.RHUB.USR2, DeviceObj)

  Scope (\_SB.PC00.XHCI.RHUB) {


    //
    // Method for creating generic _PLD buffers
    // _PLD contains lots of data, but for purpose of internal validation we care only about
    // ports' visibility and pairing (this requires group position)
    // so these are the only 2 configurable parameters (User Visible, Group Position)
    //
    Method(GPLD, 2, Serialized) {
      Name(PCKG, Package() { Buffer(0x10) {} } )
      CreateField(DerefOf(Index(PCKG,0)), 0, 7, REV)
      Store(1,REV)
      CreateField(DerefOf(Index(PCKG,0)), 64, 1, VISI)
      Store(Arg0, VISI)
      CreateField(DerefOf(Index(PCKG,0)), 87, 8, GPOS)
      Store(Arg1, GPOS)


      return (PCKG)
    }

    //
    // Method for creating generic _UPC buffers
    // Similar to _PLD, for internal testing we only care about 1 parameter (port connectable)
    //
    Method(GUPC, 1, Serialized) {
      Name(PCKG, Package(4) { 0, 0xFF, 0, 0 } )
      Store(Arg0,Index(PCKG,0))


      return (PCKG)
    }



  } //end scope RHUB

  Scope (\_SB.PC00.XHCI.RHUB.HS01) {
    Method(_UPC) { Return (GUPC(1)) }
    Method(_PLD) { Return (GPLD(0,1)) } //BMC USB2.0 - internal - BMC USB2B 
  }

  Scope (\_SB.PC00.XHCI.RHUB.HS02) {
    Method(_UPC) { Return (GUPC(1)) }
    Method(_PLD) { Return (GPLD(0,2)) }  //BMC USB1.1 - internal - BMC USB2A 
  }

  Scope (\_SB.PC00.XHCI.RHUB.HS03) {
    Method(_UPC) { Return (GUPC(1)) }
    Method(_PLD) { Return (GPLD(1,3)) } //Back Panel [CONN J3A1] - Upper - USB2 port
  }

  Scope (\_SB.PC00.XHCI.RHUB.HS04) {
    Method(_UPC) { Return (GUPC(1)) }
    Method(_PLD) { Return (GPLD(1,4)) } //Back Panel [CONN J3A1] - Center - USB2 port
  }

  Scope (\_SB.PC00.XHCI.RHUB.HS05) {
    Method(_UPC) { Return (GUPC(1)) }
    Method(_PLD) { Return (GPLD(1,5)) } //Back Panel [CONN J3A1] - Bottom - USB2 port
  }
  
  Scope (\_SB.PC00.XHCI.RHUB.HS06) {
    Method(_UPC) { Return (GUPC(0)) }
    Method(_PLD) { Return (GPLD(0,0)) } //INTERNAL TYPE A USB3.0 [CONN J3B2] - internal - usb3 port (No BOM)
  }

  Scope (\_SB.PC00.XHCI.RHUB.HS07) {
    Method(_UPC) { Return (GUPC(0)) }
    Method(_PLD) { Return (GPLD(0,0)) } 
  }

  Scope (\_SB.PC00.XHCI.RHUB.HS08) {
    Method(_UPC) { Return (GUPC(1)) }
    Method(_PLD) { Return (GPLD(0,8)) } //INTERNAL TYPE A USB3.0 [CONN J3B2] - internal - usb2 port
  }

  Scope (\_SB.PC00.XHCI.RHUB.HS09) {
    Method(_UPC) { Return (GUPC(0)) }
    Method(_PLD) { Return (GPLD(0,0)) } 
  }

  Scope (\_SB.PC00.XHCI.RHUB.HS10) {
    Method(_UPC) { Return (GUPC(0)) }
    Method(_PLD) { Return (GPLD(0,0)) } 
  }

  Scope (\_SB.PC00.XHCI.RHUB.HS11) {
    Method(_UPC) { Return (GUPC(1)) }
    Method(_PLD) { Return (GPLD(1,11)) } //Front Panel [CONN J8E2] - Front - usb2 port
  }

  Scope (\_SB.PC00.XHCI.RHUB.HS12) {
    Method(_UPC) { Return (GUPC(1)) }
    Method(_PLD) { Return (GPLD(1,12)) } //Storage Front Panel Board [CONN J1D1] - Front - usb2 port
  }

  Scope (\_SB.PC00.XHCI.RHUB.HS13) {
    Method(_UPC) { Return (GUPC(1)) }
    Method(_PLD) { Return (GPLD(1,13)) } //Front Panel [CONN J8E2] - Front - usb2 port
  }

  Scope (\_SB.PC00.XHCI.RHUB.HS14) {
    Method(_UPC) { Return (GUPC(1)) }
    Method(_PLD) { Return (GPLD(1,14)) } //Storage Front Panel Board [CONN J1D1] - Front - usb2 port
  }

  Scope (\_SB.PC00.XHCI.RHUB.USR1) {
    Method(_UPC) { Return (GUPC(0)) }
    Method(_PLD) { Return (GPLD(0,0)) }
  }

  Scope (\_SB.PC00.XHCI.RHUB.USR2) {
    Method(_UPC) { Return (GUPC(0)) }
    Method(_PLD) { Return (GPLD(0,0)) }
  }

  Scope (\_SB.PC00.XHCI.RHUB.SS01) {
    Method(_UPC) { Return (GUPC(1)) }
    Method(_PLD) { Return (GPLD(1,11)) } //Front Panel [CONN J8E2] - Front - usb3port
  }

  Scope (\_SB.PC00.XHCI.RHUB.SS02) {
    Method(_UPC) { Return (GUPC(1)) }
    Method(_PLD) { Return (GPLD(1,13)) } //Front Panel [CONN J8E2] - Front - usb3port
  }

  Scope (\_SB.PC00.XHCI.RHUB.SS03) {
    Method(_UPC) { Return (GUPC(1)) }
    Method(_PLD) { Return (GPLD(1,3)) } //Back Panel A [CONN J3A1] - Upper - USB3 port
  }

  Scope (\_SB.PC00.XHCI.RHUB.SS04) {
    Method(_UPC) { Return (GUPC(1)) }
    Method(_PLD) { Return (GPLD(1,4)) } //Back Panel A [CONN J3A1] - Center - USB3 port
  }

  Scope (\_SB.PC00.XHCI.RHUB.SS05) {
    Method(_UPC) { Return (GUPC(1)) }
    Method(_PLD) { Return (GPLD(1,5)) } //Back Panel A [CONN J3A1] - Bottom - USB3 port
  }

  Scope (\_SB.PC00.XHCI.RHUB.SS06) {
    Method(_UPC) { Return (GUPC(0)) }
    Method(_PLD) { Return (GPLD(0,0)) } //INTERNAL TYPE A USB3.0 [CONN J3B2] - internal - usb3 port (No BOM)
  }

  Scope (\_SB.PC00.XHCI.RHUB.SS07) {
    Method(_UPC) { Return (GUPC(0)) }
    Method(_PLD) { Return (GPLD(0,0)) } //N/A
  }

  Scope (\_SB.PC00.XHCI.RHUB.SS08) {
    Method(_UPC) { Return (GUPC(0)) }
    Method(_PLD) { Return (GPLD(0,0)) } //N/A
  }

  Scope (\_SB.PC00.XHCI.RHUB.SS09) {
    Method(_UPC) { Return (GUPC(0)) }
    Method(_PLD) { Return (GPLD(0,0)) } //N/A
  }

  Scope (\_SB.PC00.XHCI.RHUB.SS10) {
    Method(_UPC) { Return (GUPC(0)) }
    Method(_PLD) { Return (GPLD(0,0)) } //N/A
  }
}
