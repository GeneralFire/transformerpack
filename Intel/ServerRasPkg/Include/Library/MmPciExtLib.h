/** @file
  Provides services to access PCI Configuration Space on a platform with multiple PCI segments.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2006 - 2019 Intel Corporation. <BR>

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

#ifndef __PCI_SEGMENT_LIB_EXT__
#define __PCI_SEGMENT_LIB_EXT__

#define  PCI_EXT_MM_TYPE_VMD                  0x01
#define  PCI_EXT_MM_TYPE_RCRB                 0x02
#define  PCI_EXT_MM_SUB_TYPE_RCRB_DMI         0x01
#define  PCI_EXT_MM_SUB_TYPE_RCRB_CXLDP       0x02
#define  PCI_EXT_MM_SUB_TYPE_RCRB_CXLUP       0x03
#define  PCI_EXT_MM_SUB_TYPE_RCRB_CXLIO_DP    0x04
#define  PCI_EXT_MM_SUB_TYPE_RCRB_CXLIO_UP    0x05
#define  PCI_EXT_MM_SUB_TYPE_RCRB_CXLCM_DP    0x06
#define  PCI_EXT_MM_SUB_TYPE_RCRB_CXLCM_UP    0x07

#define CXL_DP_UP_RCRB_MEM_SIZE               0x1000
#define CXL_DP_UP_MEMBAR0_IO_OFFSET           0x0
#define CXL_DP_UP_MEMBAR0_CM_OFFSET           0x1000

typedef struct{
  UINT32  PciExtMmType : 4;
  UINT32  PciExtMmSubType : 4;
  UINT32  Stack : 8;
  UINT32  Reserved : 16;
} PCI_EXT_MM_INFO;

/**
  This procedure will get PCIE address

  @param[in] Socket               Socket Number
  @param[in] Bus                  Pcie Bus Number
  @param[in] Device               Pcie Device Number
  @param[in] Function             Pcie Function Number
  @param[in] MmInfo               Memory Map information of target device

  @retval PCIE address
**/
UINTN
EFIAPI
MmPciAddressExt (
  IN UINT32                       Socket,
  IN UINT32                       Bus,
  IN UINT32                       Device,
  IN UINT32                       Function,
  IN UINT32                       Register,
  IN PCI_EXT_MM_INFO              *MmInfo
  );

#define ONLY_REGISTER_OFFSET(x)  (x & 0xFFF)

#define PCI_DEVICE_MM_OFFSET(Bus,Device,Function,Register)   \
    ( ((Register) & 0xfff)        |                          \
      (((Function) & 0x07) << 12) |                          \
      (((Device) & 0x1f) << 15)   |                          \
      (((Bus) & 0xff) << 20)                                 \
    )
#endif
