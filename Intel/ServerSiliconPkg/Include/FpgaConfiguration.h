/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 Intel Corporation. <BR>

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

#define FPGA_CONFIGURATION_KEY    0xF0E0


#define FPGA_CLASS_ID          44
#define FPGA_SUBCLASS_ID       0x00

#define FPGA_SOCKET_CONFIGURATION_VARID    0x9a00


// {1EB3BC41-1492-40B8-AC9E-43A56C14C7A1}
#define FPGA_SETUP_GUID { 0x1eb3bc41, 0x1492, 0x40b8, { 0xac, 0x9e, 0x43, 0xa5, 0x6c, 0x14, 0xc7, 0xa1 } }


#define VFR_FORMID_FPGA_SETUP               0x47D0

#define KEY_FPGA_TEST                       0x8a00
#define KEY_FPGA_SOCKET                     0x8b00
#define KEY_FPGA_ENABLE                     0x8c00

#define VFR_FORMLABLE_FPGA_SOCKET_TOP        0x8a10
#define VFR_FORMLABLE_FPGA_SOCKET_BOTTOM     0x8a11

#define FPGA_SETUP_ENABLE_DEFAULT_VALUE      0x3
#define FPGA_GUID_INDEX_AUTO_VALUE           0x00
#define FPGA_GUID_INDEX_NONE_VALUE           0xff


#define FPGA_HII_GUID_STRING_ID_BASE    0x2200
#define FPGA_HII_GUID_STRING_ID(GuidIndex)   ((EFI_STRING_ID )(FPGA_HII_GUID_STRING_ID_BASE + (GuidIndex)))

#define FPGA_HII_SOCKET_STRING_ID_BASE    0x2300
#define FPGA_HII_SOCKET_STRING_ID(SocketIndex)   ((EFI_STRING_ID )(FPGA_HII_SOCKET_STRING_ID_BASE + (SocketIndex)))


// {22819110-7F6F-4852-B4BB-13A770149B0C}
#define FPGA_FORMSET_GUID \
  { \
  0x22819110, 0x7f6f, 0x4852, { 0xb4, 0xbb, 0x13, 0xa7, 0x70, 0x14, 0x9b, 0xc } \
  }

//  {75839B0B-0A99-4233-8AA4-3866F6CEF4B3}
#define FPGA_CONFIGURATION_GUID \
  { \
  0x75839b0b, 0x0a99, 0x4233, 0x8a, 0xa4, 0x38, 0x66, 0xf6, 0xce, 0xf4, 0xb3 \
  }



