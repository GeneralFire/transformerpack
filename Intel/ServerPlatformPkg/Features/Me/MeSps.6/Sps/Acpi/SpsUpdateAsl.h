/** @file
  This code updates ASL code that supports SPS firmware in ME.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2017 Intel Corporation. <BR>

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

#ifndef _SPS_UPDATE_ASL_H_
#define _SPS_UPDATE_ASL_H_

/*****************************************************************************
 * Definitions
 *****************************************************************************/
#define AML_STA_PRESENT    0x01
#define AML_STA_ENABLED    0x02
#define AML_STA_VISIBLE    0x04
#define AML_STA_FUNCTIONAL 0x08
#define AML_STA_BATTERY    0x10

#define AML_REGTYPE_SYSMEM 0
#define AML_REGTYPE_SYSIO  1
#define AML_REGTYPE_PCICFG 2
#define AML_REGTYPE_EMBEDD 3
#define AML_REGTYPE_SMBUS  4
#define AML_REGTYPE_CMOS   5
#define AML_REGTYPE_PCIBAR 6
#define AML_REGTYPE_IPMI   7
#define AML_REGTYPE_GPIO   8
#define AML_REGTYPE_GSBUS  9

#define AML_SIGNATURE_16(A, B)        ((A) | (B << 8))
#define AML_SIGNATURE_32(A, B, C, D)  (AML_SIGNATURE_16(A, B) | (AML_SIGNATURE_16(C, D) << 16))
#define AML_SIGNATURE_64(A, B, C, D, E, F, G, H) \
       (AML_SIGNATURE_32(A, B, C, D) | ((UINT64)(AML_SIGNATURE_32(E, F, G, H)) << 32))

#define SPS_ASL_SIG_NVS  AML_SIGNATURE_64('S', 'p', 's', 'N', 'v', 's', ' ', ' ')
#define SPS_ASL_SIG_NM   AML_SIGNATURE_64('S', 'p', 's', 'N', 'm', ' ', ' ', ' ')

#ifndef EFI_ACPI_CREATOR_ID
#define EFI_ACPI_CREATOR_ID       0x4C544E49          // "INTL"
#define EFI_ACPI_CREATOR_REVISION 0x20091013          // Oct 13 2009
#endif

#define GetHeci1Bar() ((PciSegmentRead32 ( PCI_SEGMENT_LIB_ADDRESS (ME_SEGMENT, ME_BUS, ME_DEVICE_NUMBER, HECI_FUNCTION_NUMBER, 0x10)) & ~0xF) |\
                        (UINT64)PciSegmentRead32 (PCI_SEGMENT_LIB_ADDRESS (ME_SEGMENT, ME_BUS, ME_DEVICE_NUMBER, HECI_FUNCTION_NUMBER, 0x14)) << 32)
#define GetHeci2Bar() ((PciSegmentRead32 (PCI_SEGMENT_LIB_ADDRESS (ME_SEGMENT, ME_BUS, ME_DEVICE_NUMBER, HECI2_FUNCTION_NUMBER, 0x10)) & ~0xF) |\
                        (UINT64)PciSegmentRead32 (PCI_SEGMENT_LIB_ADDRESS (ME_SEGMENT, ME_BUS, ME_DEVICE_NUMBER, HECI2_FUNCTION_NUMBER, 0x14)) << 32)

#endif // _SPS_UPDATE_ASL_H_
