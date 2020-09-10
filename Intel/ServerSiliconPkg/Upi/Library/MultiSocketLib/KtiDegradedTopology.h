/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2019 Intel Corporation. <BR>

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

#ifndef _KTI_DEGRADED_TOPOLOGY_H_
#define _KTI_DEGRADED_TOPOLOGY_H_

typedef union {
  struct {
    UINT8  Is4S2LFullyConnect : 1;
    UINT8  Is4S1LFullyConnect : 1;
    UINT8  Is4S2LRing : 1;
    UINT8  Is4S1LRing : 1;
    UINT8  Rsvd : 4;
  } Bits;
  UINT8    Data8;
} TOPOLOGY_BITMAP_4S;
#define TOPOLOGY_BITMAP_4S_MASK              0x0F
#define TOPOLOGY_BITMAP_4S_FULLYCONNECT_MASK 0x03
#define TOPOLOGY_BITMAP_4S_RING_MASK         0x0C

typedef union {
  struct {
    UINT8  Is3S2LRing : 1;
    UINT8  Is3S1LRing : 1;
    UINT8  Is3S2LChain : 1;
    UINT8  Is3S1LChain : 1;
    UINT8  Is3SChainStartWithLegacySocket : 1;
    UINT8  Rsvd : 3;
  } Bits;
  UINT8    Data8;
} TOPOLOGY_BITMAP_3S;
#define TOPOLOGY_BITMAP_3S_MASK              0x1F

typedef union {
  struct {
    UINT8  Is2S2L : 1;
    UINT8  Is2S1L : 1;
    UINT8  Rsvd : 6;
  } Bits;
  UINT8    Data8;
} TOPOLOGY_BITMAP_2S;
#define TOPOLOGY_BITMAP_2S_MASK              0x03

#endif  //_KTI_DEGRADED_TOPOLOGY_H_
