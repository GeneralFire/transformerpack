/** @file
  Internal header file for CXL RAS lib.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 Intel Corporation. <BR>

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

#ifndef _CXL_RAS_LIB_INTERNAL_H_
#define _CXL_RAS_LIB_INTERNAL_H_

#define CXL_IO_AER_REG_OFFSET               0x100

#define CXL_CACHE_MEM_CXL_CAP_REG_OFFSET    0
#define CXL_CACHE_MEM_RAS_CAP_REG_OFFSET    0x04

#define CXL_CACHE_MEM_UNC_ERR_STS_REG_OFFSET  0
#define CXL_CACHE_MEM_UNC_ERR_MSK_REG_OFFSET  4
#define CXL_CACHE_MEM_UNC_ERR_SEV_REG_OFFSET  8
#define CXL_CACHE_MEM_COR_ERR_STS_REG_OFFSET  0xc
#define CXL_CACHE_MEM_COR_ERR_MSK_REG_OFFSET  0x10
#define CXL_CACHE_MEM_ERR_CAP_CTL_REG_OFFSET  0x14

#endif
