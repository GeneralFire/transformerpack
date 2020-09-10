/** @file
  Back Compatiable temp header file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2007 Intel Corporation. <BR>

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

#ifndef __BACK_COMPATIABLE_H__
#define __BACK_COMPATIABLE_H__

//
// ACPI and legacy I/O register offsets from PMBASE
//
#define R_ACPI_PM1_STS                      0x00
#define R_ACPI_PM1_EN                       0x02
#define R_ACPI_PM1_CNT                      0x04
#define R_ACPI_PM1_TMR                      0x08
#define R_ACPI_PROC_CNT                     0x10
#define R_ACPI_PM2_CNT                      0x50
#define R_ACPI_GPE0_STS                     0x20
#define R_ACPI_GPE0_EN                      0x28
#define R_ACPI_SMI_EN                       0x30
#define R_ACPI_SMI_STS                      0x34
#define R_ACPI_ALT_GP_SMI_EN                0x38
#define R_ACPI_ALT_GP_SMI_STS               0x3A

#define R_ACPI_LV2                          0x14

#define R_IOPORT_CMOS_STANDARD_INDEX            0x70
#define R_IOPORT_CMOS_STANDARD_DATA             0x71

#define R_IOPORT_CMOS_UPPER_INDEX               0x72
#define R_IOPORT_CMOS_UPPER_DATA                0x73

#define R_IOPORT_CMOS_IDX_DIAGNOSTIC_STATUS     0x0E

//
// Misc PCI register offsets and sizes
//
#define R_EFI_PCI_SVID                              0x2C
#define   V_EFI_PCI_SVID_SIZE                         0x2
#define R_EFI_PCI_SID                               0x2E
#define   V_EFI_PCI_SID_SIZE                          0x2

#endif

