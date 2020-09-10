/**@file

  Serial IO UART Hidden Controllers ACPI definitions

 @copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation.

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

Scope(\_SB) {
  If (LOr (LEqual(UART_DEVICE_MODE, SERIAL_IO_UART_HIDDEN), LEqual (UART_DEVICE_MODE, SERIAL_IO_UART_COM))) {
    Device (UART_DEVICE_HIDDEN) {
      Name (_DDN, UART_DEVICE_DDN)
      Name (_UID, UART_DEVICE_DDN)
      Method (_HID) { Return (UHID (UART_DEVICE_MODE)) }
      Method (_CRS) { Return (UARH (UART_DEVICE_PCI_BASE, UART_DEVICE_IRQ)) }
      Method (_STA) {
        If (LEqual (UART_DEVICE_MODE, SERIAL_IO_UART_COM)) {
          If (LEqual (UART_DEVICE_PG, UART_PG_AUTO)) {
            Store (UAPG (UART_DEVICE_MODE, UART_DEVICE_PG, UART_DEVICE_PCI_BASE), UART_DEVICE_PG)
          }
          Return (0xF)
        }
        Return (0x3)
      }
      If (LEqual (UART_DEVICE_MODE, SERIAL_IO_UART_COM)) {
        Method (_PSC) { Return (UPSC (UART_DEVICE_PCI_BASE)) }
        Method (_PS3) { UPS3 (UART_DEVICE_PG, UART_DEVICE_PCI_BASE) }
        Method (_PS0) { UPS0 (UART_DEVICE_PG, UART_DEVICE_PCI_BASE) }
      }
    }
  }

} //End Scope (\_SB)
