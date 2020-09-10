/** @file
  Internal header file for IIO RAS lib.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2019 Intel Corporation. <BR>

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

#ifndef _IEH_RAS_SILICON_LIB_H_
#define _IEH_RAS_SILICON_LIB_H_

#define NORTH_SAT_IEH_DEV     0
#define NORTH_SAT_IEH_FUN     4
#define GLOBAL_IEH_DEV        0
#define GLOBAL_IEH_FUN        3

#define VTD_DID_VID     0x09A28086
#define M2PCIE_DID_VID  0x09A48086
#define RAS_DID_VID     0x09A38086
#define CBDMA_DID_VID   0x0B008086
#define SPD_DID_VID     0x34488086
#define MCDDR_DID_VID   0x28808086
#define VPP_DID_VID     0x344b8086

#define ASSERT_ERROR_PIN    0x02

#define IEH_ERRPINSTS_MASK  0x07

#define GENERATE_SMI          0x01
#define NO_INBANDMSG          0x0

#define PCIE_ERROR_MSG_MC_2LM_REG_BITMAP_START  16
#define PCIE_ERROR_MSG_MC_2LM_REG_BITMAP_END  20

#define INBAND_ENABLED      0x1
#define INBAND_DISABLED     0x0

#define PCH_IEH_GCOERRSTS_REG   0x200
#define PCH_IEH_GNFERRSTS_REG   0x210
#define PCH_IEH_GFAERRSTS_REG   0x220
#define PCH_IEH_LERRUNCSTS_REG   0x280
#define PCH_IEH_LERRUNCMSK_REG   0x298
#define PCH_IEH_LERRCORSTS_REG   0x2A8
#define PCH_IEH_LERRCORMSK_REG   0x2C0

typedef enum {
  LERRUNCSTS = 0,
  LERRUNCMSK,
  LERRCORSTS,
  LERRCORMSK,
  LERRUNCFEP,
} LERR_REG_TYPE;

#endif
