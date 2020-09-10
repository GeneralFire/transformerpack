/** @file
  IPU policy definitions

@copyright
  INTEL CONFIDENTIAL
  Copyright 2017 - 2020 Intel Corporation.

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
#ifndef _IPU_CONFIG_PREMEM_H_
#define _IPU_CONFIG_PREMEM_H_

#pragma pack(push, 1)
#include <ConfigBlock.h>

#define IPU_PREMEM_CONFIG_REVISION 4

#define IMR_IPU_CAMERA   0
#define IMR_IPU_GEN      1

//
// GPIO native features pins data
//
#define GPIO_IMGUCLK_NUMBER_OF_PINS 6

//
// Maximum CSI (Camera Serial Interface) Port
//
#define MAX_CSI_PORT  8

// @todo file name will be changed to IpuConfig.h.

extern EFI_GUID gIpuPreMemConfigGuid;

/**
 IPU PreMem configuration\n
  <b>Revision 1</b>:
  - Initial version.
  <b>Revision 2</b>:
  - Change Bit-wise to Byte-wise.
  <b>Revision 3</b>:
  - Add LaneUsed and Speed of each Port configuration for Mcd10 support.
  <b>Revision 4</b>:
  - Change GPIO_IMGUCLK_NUMBER_OF_PINS to 6
  <b>Revision 5</b>:
  - Deprecate IpuImrConfiguration.
**/
typedef struct {
  CONFIG_BLOCK_HEADER  Header;           /// Config Block Header
  /**
  <b>(Test)</b> It enables the SA IPU Device if supported and not fused off.
  If FALSE, all other policies in this config block will be ignored.
  <b>1=TRUE</b>;
  0=FALSE.
  **/
  UINT8    IpuEnable;
  /**
  Deprecate
  **/
  UINT8    IpuImrConfiguration;
  /**
  It enable the IMGU CLKOUT.
  <b>TRUE</b>
  FALSE
  **/
  UINT8    ImguClkOutEn[GPIO_IMGUCLK_NUMBER_OF_PINS];
  UINT8    LaneUsed[MAX_CSI_PORT];       ///< Indicate laneUsed of each CSI port <b>0: Not configured</b>; 1: x1; 2:x2; 3:x3; 4:x4
  UINT8    CsiSpeed[MAX_CSI_PORT];       ///< Indicate speed of each CSI port <b>0: Sensor default</b>; 1: <416Mbps; 2:<1.5Gbps; 3:<2Gbps; 4:<2.5Gbps; 5:<4Gbps; 6>4Gbps
} IPU_PREMEM_CONFIG;
#pragma pack(pop)

#endif // _IPU_PREMEM_CONFIG_H_
