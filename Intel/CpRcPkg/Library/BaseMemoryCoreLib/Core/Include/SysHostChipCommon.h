/** @file
  SysHostChipCommon.h

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2020 Intel Corporation. <BR>

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

#ifndef _SYSHOST_CHIP_COMMON_H_
#define _SYSHOST_CHIP_COMMON_H_

#include <RcVersion.h>

//
// -----------------------------------------------------------------------------
// Nvram structures
//
#pragma pack(1)

typedef union {
  struct {
    UINT32 Bit0:1;
    UINT32 Bit1:1;
    UINT32 Bit2:1;
    UINT32 Bit3:1;
    UINT32 Bit4:1;
    UINT32 Bit5:1;
    UINT32 Bit6:1;
    UINT32 Bit7:1;
    UINT32 Bit8:1;
    UINT32 Bit9:1;
    UINT32 Bit10:1;
    UINT32 Bit11:1;
    UINT32 Bit12:1;
    UINT32 Bit13:1;
    UINT32 Bit14:1;
    UINT32 Bit15:1;
    UINT32 Bit16:1;
    UINT32 Bit17:1;
    UINT32 Bit18:1;
    UINT32 Bit19:1;
    UINT32 Bit20:1;
    UINT32 Bit21:1;
    UINT32 Bit22:1;
    UINT32 Bit23:1;
    UINT32 Bit24:1;
    UINT32 Bit25:1;
    UINT32 Bit26:1;
    UINT32 Bit27:1;
    UINT32 Bit28:1;
    UINT32 Bit29:1;
    UINT32 Bit30:1;
    UINT32 Bit31:1;
  } Bits;
  UINT32 Data;
} DUMMY_REG;

//
// -----------------------------------------------------------------------------
// Nvram structures
//

struct commonNvram {
  RC_VERSION            RcVersion;      // Version of the reference code last executed
  UINT8                 platformType;
  DUMMY_REG             TsegBase;
  DUMMY_REG             TsegLimit;
  DUMMY_REG             MeMemLowBaseAddr;
  DUMMY_REG             MeMemHighBaseAddr;
  DUMMY_REG             MeMemLowLimit;
  DUMMY_REG             MeMemHighLimit;
  DUMMY_REG             MeNcMemLowBaseAddr;
  DUMMY_REG             MeNcMemHighBaseAddr;
  DUMMY_REG             MeNcMemLowLimit;
  DUMMY_REG             MeNcMemHighLimit;
  DUMMY_REG             MeNcMemLowRac;
  DUMMY_REG             MeNcMemLowWac;
  UINT32                IeRequestedSize;
  DUMMY_REG             IeNcMemLowBaseAddr;
  DUMMY_REG             IeNcMemHighBaseAddr;
  DUMMY_REG             IeNcMemLowLimit;
  DUMMY_REG             IeNcMemHighLimit;
  DUMMY_REG             IeNcMemHighRac;
  DUMMY_REG             IeNcMemHighWac;
};
#pragma pack()

#endif // _SYSHOST_CHIP_COMMON_H_
