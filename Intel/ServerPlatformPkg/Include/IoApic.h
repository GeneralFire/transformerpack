/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1999 - 2002 Intel Corporation. <BR>

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

#ifndef _IOAPIC_H_
#define _IOAPIC_H_

#define EFI_IO_APIC_INDEX_OFFSET          0x00
#define EFI_IO_APIC_DATA_OFFSET           0x10
#define EFI_IO_APIC_IRQ_ASSERTION_OFFSET  0x20
#define EFI_IO_APIC_EOI_OFFSET            0x40

#define EFI_IO_APIC_ID_REGISTER           0x0
#define EFI_IO_APIC_ID_BITSHIFT           24
#define EFI_IO_APIC_VER_REGISTER          0x1
#define EFI_IO_APIC_BOOT_CONFIG_REGISTER  0x3
#define EFI_IO_APIC_FSB_INT_DELIVERY      0x1

#endif
