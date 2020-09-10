/**@file

  Serial IO SPI Controller ACPI definitions

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


    If(LEqual(SPI_DEVICE_MODE, SERIAL_IO_SPI_HIDDEN)) {
      Name (_HID, "PNP0C02")
      Method (_CRS) { Return (SPIH (SPI_DEVICE_PCI_BASE)) }
      Name (_STA, 0x3)
    }
    If(LEqual(SPI_DEVICE_MODE, SERIAL_IO_SPI_PCI)) {
      Method (_DSM,4,Serialized){if(PCIC(Arg0)) { Return(PCID(Arg0,Arg1,Arg2,Arg3)) }; Return (Buffer(){0})}
      Method (_PS3) { SOD3 (SPI_DEVICE_PCI_BASE, SERIAL_IO_D3, SERIAL_IO_BAR_RESET) }
      Method (_PS0) {  }
    }
    If (LOr (LEqual (SPI_DEVICE_MODE, SERIAL_IO_SPI_PCI), LEqual (SPI_DEVICE_MODE, SERIAL_IO_SPI_DISABLED))) {
      Method (_ADR) { Return (SPI_DEVICE_ADR) }
    }