/** @file
  PC BIOS only OEM SMBIOS structure.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2020 Intel Corporation. <BR>

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
**/

#ifndef __OEM_SMBIOS_H__
#define __OEM_SMBIOS_H__

#include <IndustryStandard/SmBios.h>

//
// Type 132 is for SPD raw data
//
#define OEM_SMBIOS_TYPE_SPD_DATA  132

typedef struct {
  SMBIOS_STRUCTURE              Hdr;
  UINT32                        Address;
  UINT32                        Size;
  UINT8                         Socket;
  UINT8                         Channel;
  UINT8                         Dimm;
} OEM_SMBIOS_TABLE_TYPE_132;


#define OEM_SMBIOS_TYPE_UTILITY_COMMUNICATION 133

typedef struct {
  SMBIOS_STRUCTURE              Hdr;
  UINT32                        Address;
  UINT32                        Size;
} OEM_SMBIOS_TABLE_TYPE_133;


#endif
