/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2014 - 2019 Intel Corporation.

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

#ifndef _ME_TYPE_DEFS_H_
#define _ME_TYPE_DEFS_H_

// definitions for use in hfr files
#define METYPE_SPS      0x01
#define METYPE_SPS_EPO  0x02
#define METYPE_CLIENT   0x03
#define METYPE_DFX      0x0F
#define METYPE_UNDEF    0xEE
#define METYPE_DISABLED 0xFF


// if METYPE == METYPE_SPS
#define MESUBTYPE_SPS_TYPE_E5          0x01
#define MESUBTYPE_SPS_TYPE_E3          0x02
#define MESUBTYPE_SPS_TYPE_DFX         0x03

// if METYPE == METYPE_SPS_EPO
#define MESUBTYPE_SPS_TYPE_EPO         0x01

// if METYPE == METYPE_CLIENT
#define MESUBTYPE_WS_TYPE_CLIENT       0x01
#define MESUBTYPE_WS_TYPE_HEDT         0x02
#define MESUBTYPE_WS_TYPE_WS           0x03


#endif // _ME_TYPE_DEFS_H_
