/** @file
  This file defines the IPU CONFIG HOB

@copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2020 Intel Corporation.

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
#ifndef _IPU_CONFIG_HOB_H_
#define _IPU_CONFIG_HOB_H_

#include <Base.h>

//
// Maximum CSI (Camera Serial Interface) Port
//
#define MAX_CSI_PORT  8

extern EFI_GUID gIpuConfigHobGuid;
#pragma pack (push,1)
///
/// This HOB is used to pass IPU related Private information to DXE phase
///
typedef struct {
  EFI_HOB_GUID_TYPE        EfiHobGuidType;                    ///< GUID Hob type structure for gIpuConfigHobGuid
  /**
  IpuAcpi - Indicates IPU ACPI mode
  0 - Disabled
  1 - IGF Child device
  2 - ACPI device
  **/
  UINT8                    IpuAcpiMode;
  UINT8                    LaneUsed[MAX_CSI_PORT];  ///< Indicate laneUsed of each CSI port <b>0: Not Configured</b>; 1: x1; 2:x2; 3:x3; 4:x4
  UINT8                    CsiSpeed[MAX_CSI_PORT];  ///< Indicate speed of each CSI port <b>0: sensor default</b>; 1: <416Mbps; 2:<1.5Gbps; 3:<2Gbps; 4:<2.5Gbps; 5:<4Gbps; 6>4Gbps
} IPU_CONFIG_HOB;
#pragma pack (pop)

#endif  // _IPU_CONFIG_HOB_H_
