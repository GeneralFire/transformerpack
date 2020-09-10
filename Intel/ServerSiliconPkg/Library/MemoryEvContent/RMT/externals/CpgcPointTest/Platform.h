/** @file
  Platform.h

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2013 - 2020 Intel Corporation. <BR>

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

#ifndef __PLATFORM_H__
#define __PLATFORM_H__
#include <SysHost.h>

#define MAX_SOCKET_CNT   MAX_SOCKET  // maximum number of sockets per system
#define MAX_CONTROLLER_CNT        MAX_IMC  // maximum number of controllers per socket
#define MAX_CHANNEL_CNT           MAX_MC_CH  // maximum number of channels per controller
#define MAX_DIMM_CNT              MAX_DIMM  // maximum number of DIMMs per channel
#define MAX_RANK_CNT              MAX_RANK_DIMM  // maximum number of ranks per DIMM (4 for RDIMM, 8 for LRDIMM)
#define MAX_DQDB_UNISEQ_CNT       (3)  // maximum number of DqDB unisequencers
#define DQDB_PATTERN_WIDTH        (64) // width of DqDB pattern
#define MAX_DQDB_LMN_UNISEQ_CNT   (1)  // maximum number of DqDB unisequencers that support LMN
#define MAX_DQDB_LMN_VALUES_CNT   (3)  // maximum number of DqDB LMN values, e.g., L, M, and N
#define CADB_PATTERN_ROW_CNT      (16) // maximum number of CADB pattern rows
#define CADB_PATTERN_WIDTH        (64) // width of CADB pattern
#define MAX_CADB_UNISEQ_CNT       (4)  // maximum number of CADB unisequencers
#define MAX_CADB_LMN_UNISEQ_CNT   (1)  // maximum number of CADB unisequencers that support LMN
#define MAX_CADB_LMN_VALUES_CNT   (3)  // maximum number of CADB LMN values, e.g., L, M, and N
#define MAX_BUS_WIDTH             MAX_BITS // maximum bus width
#define CHUNKS_PER_CACHELINE      (8)  // number of chunks per cacheline
#define MAX_ERID_SIGNAL_CNT       (2)  // maximum number of ERID signals

#define MALLOC_HEADER_SIZE        (9)  // number of bytes in Malloc header
#define MALLOC_BUFFER_GRANULARITY (8)  // number of bytes in Malloc quantum buffer size

#endif // __PLATFORM_H__

// end file Platform.h
