/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2013 - 2018 Intel Corporation. <BR>

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

Scope (\_SB.PC00) {

  Device (GPIO) {
    Name (_HID, "INTC3001")
    Name (_CRS, ResourceTemplate () {
      Memory32Fixed (ReadWrite, 0xFDC20000, 0x2000, COM0)  //GPIOW -> AddressBase: PCH_PCR_ADDRESS(PID_GPIOCOM0, 0)
      Memory32Fixed (ReadWrite, 0xFDC50000, 0x2000, COM1)  //GPIOE -> AddressBase: PCH_PCR_ADDRESS(PID_GPIOCOM1, 0) (#include "Register/PchRegsPcr.h")
      Interrupt (ResourceConsumer, Level, ActiveLow, Shared) {0x15}
    })


    Method (_STA, 0, NotSerialized) {
      Return (0xf)
    }
  }
}
