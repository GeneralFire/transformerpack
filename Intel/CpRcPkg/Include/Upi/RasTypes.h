/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2018 Intel Corporation. <BR>

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

#ifndef _RAS_TYPES_H_
#define _RAS_TYPES_H_


//
// Ras types defined in ascending order of feature
//
#define HEDT_RAS              0
#define BASIC_RAS_AX          1
#define S1WS_RAS              2
#define CORE_RAS              3
#define STANDARD_RAS          4
#define FPGA_RAS              5
#define ADVANCED_RAS          6
#define ICXP_RAS              7
#define ICXD_RAS              8
#define ICXG_RAS              9
#define KNH_RAS               10
#define SNR_RAS               11
#define HPC_RAS               12
#define CIRRUS_RAS            13
#define STANDARD_PLUS_RAS     14
#define UNDEFINED_RAS         15

#endif // _RAS_TYPES_H_
