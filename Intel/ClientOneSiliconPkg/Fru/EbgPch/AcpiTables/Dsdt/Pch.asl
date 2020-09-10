/**@file

 @copyright
  INTEL CONFIDENTIAL
  Copyright 1999 - 2020 Intel Corporation.

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

#include <Library/PchInfoLib.h>
#include <Register/PchPcrRegs.h>
#include <Register/PchRegsLpc.h>
#include <Register/PmcRegs.h>
#include <Register/PchDmiRegs.h>
#include <PchBdfAssignment.h>

#define ACPI_PCI_ADDR(Device, Function)  Or(ShiftLeft(Device, 16), Function)

Name (PNVB, 0xFFFF0000)  // PCH NVS Base address
Name (PNVL, 0xAA55)      // PCH NVS Length
Include ("Pch/AcpiTables/Dsdt/PchNvs.asl")


//
// Include GPIO macros and access library
//
Include("HsioDefine.asl")
Include("GpioAcpiDefinesEbg.h")
Include("GpioLib.asl")
Include("HsioLib.asl")
Include("Gpio.asl")


//
// Trace Hub debug library
// Include it earlier so the debug function can be used as soon as possible
//
Include ("TraceHubDebug.asl")
Name(PCHH,PCH_H) // PCH H
Name(PCHL,PCH_LP) // PCH LP

//
// PME event (PCH's GPE 6Dh) is received when any PCH internal device with
// PCI Power Management capabilities on bus 0 asserts the equivalent of the PME# signal.
// Dedicated _L6D method is not needed if its only purpose is to notify the device.
// Instead to properly handle this GPE event below requirements need to be fulfilled
// - _PRW for GPE 6Dh in devices capable of generating PME (XHCI, XDCI, GbE, HDA)
// - _S0W(3) and _DSW is present within those devices

Scope (\_SB.PC00) {
  Name(TEMP,0)                          // scratch variable that does not cause compiler warnings as writes to Localx variables do.

  //
  // PCH reserved resource
  //
  Device(PRRE) {
    Name(_HID,EISAID("PNP0C02")) // motherboard resource
    Name(_UID,"PCHRESV")
    Name(_STA,0x3) // device present and decodes its resources, but not to be displayed in OSPM

    Method(_CRS,0,Serialized)
    {
      Name(BUF0,ResourceTemplate(){
        //
        // PCH RESERVED MMIO RANGE
        // Refer to PchReservedResources.h for ranges used by PCH
        //
        // 0xFC800000 to 0xFE7FFFFF
        // skip over address range that might be claimed by the GPIO, Intel Serial IO, and TraceHub
        // The GPIO COMMx and SerialIO ranges will be handled by SIRC device.
        //
        // skip 0xFC800000 - 0xFCFFFFFF for PCH Trace Hub : SW BAR space
        Memory32Fixed(ReadWrite,0xFD000000,0x006A0000) // 0xFD000000 - 0xFD69FFFF
        //
        // On EBG GPIO COM2 contains pads that are used by PMC and can't
        // be used by GPIO OS driver.
        //
        Memory32Fixed(ReadWrite, 0x00000000, 0x10000, RBR0)
        Memory32Fixed(ReadWrite,0xFD6F0000,0x00910000) // 0xFD6F0000 - 0xFDFFFFFF
        Memory32Fixed(ReadWrite,0xFE000000,0x00020000) // 0xFE000000 - 0xFE01FFFF
        // Skip 0xFE020000 - 0xFE035FFF for Serial IO
        // skip 0xFE100000 - 0xFE1FFFFF for PCH Trace Hub : MTB BAR space
        Memory32Fixed(ReadWrite,0xFE200000,0x00600000) // 0xFE200000 - 0xFE7FFFFF

        // BIOS decoding range. Assume it's always 16MB.
        Memory32Fixed(ReadOnly,0xFF000000,0x1000000)
        // ACPI Base
        IO(Decode16,0,0,0x1,0xFF,ABAR)
      })
      // Update ACPI IO region
      CreateWordField(BUF0,ABAR._MIN,AMIN)
      CreateWordField(BUF0,ABAR._MAX,AMAX)
      Store(PMBS, AMIN)
      Store(PMBS, AMAX)

      CreateDWordField(BUF0, RBR0._BAS, COM2)
      Store(Add(SBRG, PCH_GPIO_COM2), COM2)

      Return(BUF0)
    }
  }
  Device(IOTR) {
    //
    // This device claims IO range reserved for IO traps
    // to prevent OS from reusing it for other purposes
    //
    Name(_HID,EISAID("PNP0C02"))
    Name(_UID,"IoTraps")
    Name(BUF0,ResourceTemplate(){
      Io(Decode16,0x0,0x0,0x1,0xFF,TAG0)
      Io(Decode16,0x0,0x0,0x1,0xFF,TAG1)
      Io(Decode16,0x0,0x0,0x1,0xFF,TAG2)
      Io(Decode16,0x0,0x0,0x1,0xFF,TAG3)
    })
    CreateWordField(BUF0,TAG0._MIN,AMI0)
    CreateWordField(BUF0,TAG0._MAX,AMA0)
    CreateByteField(BUF0,TAG0._LEN,LEN0)
    CreateWordField(BUF0,TAG1._MIN,AMI1)
    CreateWordField(BUF0,TAG1._MAX,AMA1)
    CreateByteField(BUF0,TAG1._LEN,LEN1)
    CreateWordField(BUF0,TAG2._MIN,AMI2)
    CreateWordField(BUF0,TAG2._MAX,AMA2)
    CreateByteField(BUF0,TAG2._LEN,LEN2)
    CreateWordField(BUF0,TAG3._MIN,AMI3)
    CreateWordField(BUF0,TAG3._MAX,AMA3)
    CreateByteField(BUF0,TAG3._LEN,LEN3)
    Method(_CRS) {
      Store(ITA0,AMI0);Store(ITA0,AMA0)
      Store(ITA1,AMI1);Store(ITA1,AMA1)
      Store(ITA2,AMI2);Store(ITA2,AMA2)
      Store(ITA3,AMI3);Store(ITA3,AMA3)
      if(LNotEqual(ITS0,1)) { Store(0, LEN0) }
      if(LNotEqual(ITS1,1)) { Store(0, LEN1) }
      if(LNotEqual(ITS2,1)) { Store(0, LEN2) }
      if(LNotEqual(ITS3,1)) { Store(0, LEN3) }
      return (BUF0)
    }
  }


  //
  // LPC Bridge - Device 31, Function 0, this is only for PCH register Memory Region declare,
  // it's better to be declared as early as possible since it's widely used in whole ACPI name space.
  // Please add any code which needs to reference any register of it after this
  //
// APTIOV_SERVER_OVERRIDE_START: Device Instance and _ADR Method will be declared through SDL in PCI_TREE.asl  
  Scope (\_SB.PC00.LPC0) {
  //Device(LPC0) {
    //Name(_ADR, 0x001F0000)
// APTIOV_SERVER_OVERRIDE_END: Device Instance and _ADR Method will be declared through SDL in PCI_TREE.asl
    Method(_DSM,4,serialized){if(PCIC(Arg0)) { return(PCID(Arg0,Arg1,Arg2,Arg3)) }; Return(Buffer() {0})}

    OperationRegion(LPC, PCI_Config, 0x00, 0x100)
    Field(LPC, AnyAcc, NoLock, Preserve)
    {
      Offset(0x02),
      CDID, 16,
      Offset(0x08),
      CRID,  8,
      Offset(R_LPC_CFG_IOD), // 0x80
      IOD0,  8,
      IOD1,  8,
      Offset(0xA0),
      , 9,
      PRBL,  1,
      Offset(R_LPC_CFG_BC),  // 0xDC
      ,      2,
      ESPI,  1,
    }
  }

  //
  // SMBus Controller - Device 31, Function 4
  //
  Device(SBUS) {
    Name(_ADR,0x001F0004)
    Method(_DSM,4,serialized){if(PCIC(Arg0)) { return(PCID(Arg0,Arg1,Arg2,Arg3)) }; Return(Buffer() {0})}
  }
}

Include ("Pcr.asl")
Include ("Pmc.asl")
Include ("PchSerialIo.asl")

If (LNotEqual(GBES,0)) {
  //
  // LAN Controller
  //
  // APTIOV_SERVER_OVERRIDE_RC_START :Device Instance and _ADR Method will be declared through SDL in PCI_TREE.asl
  Scope(\_SB.PC00.GLAN) {
   // Device(GLAN) { // GbE Controller
     // Name(_ADR, 0x001F0006)
      Method(_DSM,4,serialized){if(PCIC(Arg0)) { return(PCID(Arg0,Arg1,Arg2,Arg3)) }; Return(Buffer() {0})}

      Name(_S0W, 3) // Device can wake itself from D3 in S0

      Method(_PRW, 0) {
        Return(GPRW(0x6D, 4)) // can wakeup from S4 state
      }

      Method(_DSW, 3) {}
  //  } // end "GbE Controller"
  // APTIOV_SERVER_OVERRIDE_RC_END
  } //scope
}

// APTIOV_SERVER_OVERRIDE_RC_START :Device Instance and _ADR Method will be declared through SDL in PCI_TREE.asl
//Scope(\_SB_.PC00) {
 Scope(\_SB.PC00.RP01) {  
 // Device(RP01) {
    Name(LTRZ, 0)
    Name(LMSL, 0)
    Name(LNSL, 0)
  /**
    Method (_ADR, 0) {
      Return (ACPI_PCI_ADDR (PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_1, PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_1))
    }
  **/
// APTIOV_SERVER_OVERRIDE_RC_END :Device Instance and _ADR Method will be declared through SDL in PCI_TREE.asl
    Name(SLOT,1)
    Method(_INI)
    {
      Store (LTR1, LTRZ)
      Store (PML1, LMSL)
      Store (PNL1, LNSL)
    }

    Include("EnterprisePcie.asl")
  }

// APTIOV_SERVER_OVERRIDE_RC_START :Device Instance and _ADR Method will be declared through SDL in PCI_TREE.asl
  Scope(\_SB.PC00.RP02) {  
 // Device(RP02) {
    Name(LTRZ, 0)
    Name(LMSL, 0)
    Name(LNSL, 0)
    /**
    Method (_ADR, 0) {
      Return (ACPI_PCI_ADDR (PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_2, PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_2))
    }
    **/
// APTIOV_SERVER_OVERRIDE_RC_END :Device Instance and _ADR Method will be declared through SDL in PCI_TREE.asl
    Name(SLOT,2)
    Method(_INI)
    {
      Store (LTR2, LTRZ)
      Store (PML2, LMSL)
      Store (PNL2, LNSL)
    }

    Include("EnterprisePcie.asl")
  }

// APTIOV_SERVER_OVERRIDE_RC_START :Device Instance and _ADR Method will be declared through SDL in PCI_TREE.asl
  Scope(\_SB.PC00.RP03) {
  //Device(RP03) {
    Name(LTRZ, 0)
    Name(LMSL, 0)
    Name(LNSL, 0)
    /**
    Method (_ADR, 0) {
      Return (ACPI_PCI_ADDR (PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_3, PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_3))
    }
    **/
// APTIOV_SERVER_OVERRIDE_RC_END :Device Instance and _ADR Method will be declared through SDL in PCI_TREE.asl
    Name(SLOT,3)
    Method(_INI)
    {
      Store (LTR3, LTRZ)
      Store (PML3, LMSL)
      Store (PNL3, LNSL)
    }

    Include("EnterprisePcie.asl")
  }

// APTIOV_SERVER_OVERRIDE_RC_START :Device Instance and _ADR Method will be declared through SDL in PCI_TREE.asl
  Scope(\_SB.PC00.RP04) {
  //Device(RP04) {
    Name(LTRZ, 0)
    Name(LMSL, 0)
    Name(LNSL, 0)
    /**
    Method (_ADR, 0) {
      Return (ACPI_PCI_ADDR (PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_4, PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_4))
    }
    **/
// APTIOV_SERVER_OVERRIDE_RC_END :Device Instance and _ADR Method will be declared through SDL in PCI_TREE.asl
    Name(SLOT,4)
    Method(_INI)
    {
      Store (LTR4, LTRZ)
      Store (PML4, LMSL)
      Store (PNL4, LNSL)
    }

    Include("EnterprisePcie.asl")
  }

// APTIOV_SERVER_OVERRIDE_RC_START :Device Instance and _ADR Method will be declared through SDL in PCI_TREE.asl
  Scope(\_SB.PC00.RP05) {
  //Device(RP05) {
    Name(LTRZ, 0)
    Name(LMSL, 0)
    Name(LNSL, 0)
    /**
    Method (_ADR, 0) {
      Return (ACPI_PCI_ADDR (PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_5, PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_5))
    }
    **/
// APTIOV_SERVER_OVERRIDE_RC_END :Device Instance and _ADR Method will be declared through SDL in PCI_TREE.asl
    Name(SLOT,5)
    Method(_INI)
    {
      Store (LTR5, LTRZ)
      Store (PML5, LMSL)
      Store (PNL5, LNSL)
    }

    Include("EnterprisePcie.asl")
  }

// APTIOV_SERVER_OVERRIDE_RC_START :Device Instance and _ADR Method will be declared through SDL in PCI_TREE.asl
  Scope(\_SB.PC00.RP06) {
  //Device(RP06) {
    Name(LTRZ, 0)
    Name(LMSL, 0)
    Name(LNSL, 0)
    /**
    Method (_ADR, 0) {
      Return (ACPI_PCI_ADDR (PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_6, PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_6))
    }
    **/
// APTIOV_SERVER_OVERRIDE_RC_END :Device Instance and _ADR Method will be declared through SDL in PCI_TREE.asl
    Name(SLOT,6)
    Method(_INI)
    {
      Store (LTR6, LTRZ)
      Store (PML6, LMSL)
      Store (PNL6, LNSL)
    }

    Include("EnterprisePcie.asl")
  }

// APTIOV_SERVER_OVERRIDE_RC_START :Device Instance and _ADR Method will be declared through SDL in PCI_TREE.asl
  Scope(\_SB.PC00.RP07) {
  //Device(RP07) {
    Name(LTRZ, 0)
    Name(LMSL, 0)
    Name(LNSL, 0)
    /**
    Method (_ADR, 0) {
      Return (ACPI_PCI_ADDR (PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_7, PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_7))
    }
    **/
  // APTIOV_SERVER_OVERRIDE_RC_END :Device Instance and _ADR Method will be declared through SDL in PCI_TREE.asl
    Name(SLOT,7)
    Method(_INI)
    {
      Store (LTR7, LTRZ)
      Store (PML7, LMSL)
      Store (PNL7, LNSL)
    }

    Include("EnterprisePcie.asl")
  }

// APTIOV_SERVER_OVERRIDE_RC_START :Device Instance and _ADR Method will be declared through SDL in PCI_TREE.asl
  Scope(\_SB.PC00.RP08) {
  //Device(RP08) {
    Name(LTRZ, 0)
    Name(LMSL, 0)
    Name(LNSL, 0)
    /**
    Method (_ADR, 0) {
      Return (ACPI_PCI_ADDR (PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_8, PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_8))
    }
    **/
// APTIOV_SERVER_OVERRIDE_RC_END :Device Instance and _ADR Method will be declared through SDL in PCI_TREE.asl
    Name(SLOT,8)
    Method(_INI)
    {
      Store (LTR8, LTRZ)
      Store (PML8, LMSL)
      Store (PNL8, LNSL)
    }

    Include("EnterprisePcie.asl")
  }

// APTIOV_SERVER_OVERRIDE_RC_START :Device Instance and _ADR Method will be declared through SDL in PCI_TREE.asl
  Scope(\_SB.PC00.RP09) {
//  Device(RP09) {
    Name(LTRZ, 0)
    Name(LMSL, 0)
    Name(LNSL, 0)
    /**
    Method (_ADR, 0) {
      Return (ACPI_PCI_ADDR (PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_9, PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_9))
    }
    **/
// APTIOV_SERVER_OVERRIDE_RC_END :Device Instance and _ADR Method will be declared through SDL in PCI_TREE.asl
    Name(SLOT,9)
    Method(_INI)
    {
      Store (LTR9, LTRZ)
      Store (PML9, LMSL)
      Store (PNL9, LNSL)
    }

    Include("EnterprisePcie.asl")
  }

// APTIOV_SERVER_OVERRIDE_RC_START :Device Instance and _ADR Method will be declared through SDL in PCI_TREE.asl
  Scope(\_SB.PC00.RP10) {
  //Device(RP10) {
    Name(LTRZ, 0)
    Name(LMSL, 0)
    Name(LNSL, 0)
    /**
    Method (_ADR, 0) {
      Return (ACPI_PCI_ADDR (PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_10, PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_10))
    }
    **/
// APTIOV_SERVER_OVERRIDE_RC_END :Device Instance and _ADR Method will be declared through SDL in PCI_TREE.asl
    Name(SLOT,10)
    Method(_INI)
    {
      Store (LTRA, LTRZ)
      Store (PMLA, LMSL)
      Store (PNLA, LNSL)
    }

    Include("EnterprisePcie.asl")
  }

// APTIOV_SERVER_OVERRIDE_RC_START :Device Instance and _ADR Method will be declared through SDL in PCI_TREE.asl
  Scope(\_SB.PC00.RP11) {
  //Device(RP11) {
    Name(LTRZ, 0)
    Name(LMSL, 0)
    Name(LNSL, 0)
    /**
    Method (_ADR, 0) {
      Return (ACPI_PCI_ADDR (PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_11, PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_11))
    }
    **/
// APTIOV_SERVER_OVERRIDE_RC_END :Device Instance and _ADR Method will be declared through SDL in PCI_TREE.asl
    Name(SLOT,11)
    Method(_INI)
    {
      Store (LTRB, LTRZ)
      Store (PMLB, LMSL)
      Store (PNLB, LNSL)
    }

    Include("EnterprisePcie.asl")
  }

// APTIOV_SERVER_OVERRIDE_RC_START :Device Instance and _ADR Method will be declared through SDL in PCI_TREE.asl
  Scope(\_SB.PC00.RP12) {
  //Device(RP12) {
    Name(LTRZ, 0)
    Name(LMSL, 0)
    Name(LNSL, 0)
    /**
    Method (_ADR, 0) {
      Return (ACPI_PCI_ADDR (PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_12, PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_12))
    }
    **/
// APTIOV_SERVER_OVERRIDE_RC_END :Device Instance and _ADR Method will be declared through SDL in PCI_TREE.asl
    Name(SLOT,12)
    Method(_INI)
    {
      Store (LTRC, LTRZ)
      Store (PMLC, LMSL)
      Store (PNLC, LNSL)
    }

    Include("EnterprisePcie.asl")
  }

// APTIOV_SERVER_OVERRIDE_RC_START :Device Instance and _ADR Method will be declared through SDL in PCI_TREE.asl
  Scope(\_SB.PC00.RP13) {
  //Device(RP13) {
    Name(LTRZ, 0)
    Name(LMSL, 0)
    Name(LNSL, 0)
    /**
    Method (_ADR, 0) {
      Return (ACPI_PCI_ADDR (PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_13, PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_13))
    }
    **/
// APTIOV_SERVER_OVERRIDE_RC_END :Device Instance and _ADR Method will be declared through SDL in PCI_TREE.asl
    Name(SLOT,13)
    Method(_INI)
    {
      Store (LTRC, LTRZ)
      Store (PMLC, LMSL)
      Store (PNLC, LNSL)
    }

    Include("EnterprisePcie.asl")
  }

// APTIOV_SERVER_OVERRIDE_RC_START :Device Instance and _ADR Method will be declared through SDL in PCI_TREE.asl
  Scope(\_SB.PC00.RP14) {
  //Device(RP14) {
    Name(LTRZ, 0)
    Name(LMSL, 0)
    Name(LNSL, 0)
    /**
    Method (_ADR, 0) {
      Return (ACPI_PCI_ADDR (PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_14, PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_14))
    }
    **/
// APTIOV_SERVER_OVERRIDE_RC_END :Device Instance and _ADR Method will be declared through SDL in PCI_TREE.asl
    Name(SLOT,14)
    Method(_INI)
    {
      Store (LTRC, LTRZ)
      Store (PMLC, LMSL)
      Store (PNLC, LNSL)
    }

    Include("EnterprisePcie.asl")
  }

// APTIOV_SERVER_OVERRIDE_RC_START
  Scope(\_SB.PC00.RP15) {
  //Device(RP15) {
    Name(LTRZ, 0)
    Name(LMSL, 0)
    Name(LNSL, 0)
    /**
    Method (_ADR, 0) {
      Return (ACPI_PCI_ADDR (PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_15, PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_15))
    }
    **/
// APTIOV_SERVER_OVERRIDE_RC_END
    Name(SLOT,15)
    Method(_INI)
    {
      Store (LTRC, LTRZ)
      Store (PMLC, LMSL)
      Store (PNLC, LNSL)
    }

    Include("EnterprisePcie.asl")
  }

  // APTIOV_SERVER_OVERRIDE_RC_START
  Scope(\_SB.PC00.RP16) {
  //Device(RP16) {
    Name(LTRZ, 0)
    Name(LMSL, 0)
    Name(LNSL, 0)
    /**
    Method (_ADR, 0) {
      Return (ACPI_PCI_ADDR (PCI_DEVICE_NUMBER_PCH_PCIE_ROOT_PORT_15, PCI_FUNCTION_NUMBER_PCH_PCIE_ROOT_PORT_15))
    }
    **/
    // APTIOV_SERVER_OVERRIDE_RC_END
    Name(SLOT,16)
    Method(_INI)
    {
      Store (LTRC, LTRZ)
      Store (PMLC, LMSL)
      Store (PNLC, LNSL)
    }

    Include("EnterprisePcie.asl")
  }
// APTIOV_SERVER_OVERRIDE_RC_START :
Scope(\_SB_.PC00) {
  Device(SAT0) {
    Name(_ADR, ACPI_PCI_ADDR(PCI_DEVICE_NUMBER_PCH_SATA_1, PCI_FUNCTION_NUMBER_PCH_SATA_1))
    Include("Sata.asl")
  }

  Device(SAT1) {
    Name(_ADR, ACPI_PCI_ADDR(PCI_DEVICE_NUMBER_PCH_SATA_2, PCI_FUNCTION_NUMBER_PCH_SATA_2))
    Include("Sata.asl")
  }

  Device(SAT2) {
    Name(_ADR, ACPI_PCI_ADDR(PCI_DEVICE_NUMBER_PCH_SATA_3, PCI_FUNCTION_NUMBER_PCH_SATA_3))
    Include("Sata.asl")
  }

  Device (XHCI) {
    Name (_ADR, ACPI_PCI_ADDR(PCI_DEVICE_NUMBER_PCH_XHCI, PCI_FUNCTION_NUMBER_PCH_XHCI))

    //
    // USRA (USbR port Address), method for calculating address of first USBR port in XHCI controller
    //
    Method(USRA,0,Serialized) {
      Return (15)
    }

    //
    // SSPA (SuperSpeed Port Address), method for calculating address of first SS port in XHCI controller
    //
    Method(SSPA,0,Serialized) {
      Return (17)
    }

    Include("Xhci.asl")
  }

  Device (HEC1) {
    Name (_ADR, ACPI_PCI_ADDR(PCI_DEVICE_NUMBER_PCH_HECI1, PCI_FUNCTION_NUMBER_PCH_HECI1))
  }

  Device (HEC2) {
    Name (_ADR, ACPI_PCI_ADDR(PCI_DEVICE_NUMBER_PCH_HECI2, PCI_FUNCTION_NUMBER_PCH_HECI2))
  }
  //
  // High Definition Audio Controller - Device 31, Function 3
  //
  Device(HDAS)
  {
    Name(_ADR, ACPI_PCI_ADDR(PCI_DEVICE_NUMBER_PCH_HDA, PCI_FUNCTION_NUMBER_PCH_HDA))

    Include ("Hda.asl")
  }
}
// APTIOV_SERVER_OVERRIDE_RC_END:
Include ("PchTraceHub.asl")

