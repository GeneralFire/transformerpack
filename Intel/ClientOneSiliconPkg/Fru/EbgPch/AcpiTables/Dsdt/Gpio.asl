/**@file
 ACPI definition for GPIO controller

 @copyright
  INTEL CONFIDENTIAL
  Copyright 2017 - 2019 Intel Corporation.

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
Include("GpioAcpiDefinesEbg.h")

Scope(\_SB) {
  //----------------------------
  //  GPIO Controller
  //----------------------------
  Device (GPI0) {
    Method (_HID) {
      Return(GPIO_EBG_ACPI_HID)
    }

    Method (_CID) {
      Return(GPIO_EBG_ACPI_CID)
    }

    Method (_CRS, 0x0, Serialized) {
      //
      // In this resource descriptor we only return GPIO memory ranges that are
      // going to be used by GPIO OS driver. All communities that
      // do not contain GPIO pads that can be used in GPIO mode
      // are defined as motherboard restricted resource to prevent
      // OS from accessing them.
      //
      Name (RBUF, ResourceTemplate () {
        Memory32Fixed (ReadWrite, 0x00000000, 0x00010000, RBR0)
        Memory32Fixed (ReadWrite, 0x00000000, 0x00010000, RBR1)
        Memory32Fixed (ReadWrite, 0x00000000, 0x00010000, RBR2)
        Memory32Fixed (ReadWrite, 0x00000000, 0x00010000, RBR3)
        Memory32Fixed (ReadWrite, 0x00000000, 0x00010000, RBR4)
        Interrupt (ResourceConsumer, Level, ActiveLow, Shared, , , IRQ) { 14 } //Interrupt IRQ_EN
      })

      CreateDWordField(RBUF,RBR0._BAS,COM0)
      CreateDWordField(RBUF,RBR1._BAS,COM1)
      CreateDWordField(RBUF,RBR2._BAS,COM3)
      CreateDWordField(RBUF,RBR3._BAS,COM4)
      CreateDWordField(RBUF,RBR4._BAS,COM5)
      CreateDWordField(RBUF,IRQ._INT,IRQN)
      Store( Add(SBRG,PCH_GPIO_COM0), COM0)
      Store( Add(SBRG,PCH_GPIO_COM1), COM1)
      Store( Add(SBRG,PCH_GPIO_COM3), COM3)
      Store( Add(SBRG,PCH_GPIO_COM4), COM4)
      Store( Add(SBRG,PCH_GPIO_COM5), COM5)
      Store(SGIR,IRQN)

      Return (RBUF)
    }

    Method (_STA, 0x0, NotSerialized) {
      If(LEqual(GPHD, 1)) { // Hide GPIO ACPI device
        Return(0x3)
      }
      Return(0xF)
    }
  } // END Device(GPIO)
} // END Scope(\_SB.PCI0)

