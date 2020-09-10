/** @file
  MemXoverCalibOffsets.h

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 Intel Corporation. <BR>

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

#ifndef  __MEM_XOVER_CALIB_OFFSETS_H__
#define  __MEM_XOVER_CALIB_OFFSETS_H__

#include "SysHost.h"
#include "MemXoverCalib.h"


//
// Supported frequencies in this file
//
// DDR_2133
// DDR_2400
// DDR_2666
// DDR_2933
// DDR_3200

#define   XOVER_OFFSET_NUM_FREQ_SUPPORTED   5

typedef struct {
  UINT8                   DdrFreq;
  XOVER_OFFSETS           Offset;
} XOVER_OFFSET_FREQ_SET;

typedef struct {
  XOVER_OFFSET_FREQ_SET   OffsetFreqSet[XOVER_OFFSET_NUM_FREQ_SUPPORTED];
} XOVER_OFFSETS_CHANNEL_SET;

//
// These tables include only a subset of DDR frequencies that are possible.
// The algorithm that searches the tables for a frequency match will pick the
// the entry with an exact frequency match, if found. Otherwise, the entry
// with the next highest frequency will be chosen. If the current frequency
// is greater than the highest frequency in the table, the last table entry
// is used.
//
// The tables below must be sorted in ascending order based on "DdrFreq".
//
//                          DdrFreq    DATA    CLK   CMD   CMDPI0
#define XOVER_OFFSETS_CH0                                           \
                           DDR_2133, {  0,     0,    32,     0  },  \
                           DDR_2400, {  0,     0,    32,     0  },  \
                           DDR_2666, {  0,     0,    32,     0  },  \
                           DDR_2933, {  0,     0,    32,     0  },  \
                           DDR_3200, {  0,     0,    32,     0  }

//                          DdrFreq    DATA    CLK   CMD   CMDPI0
#define XOVER_OFFSETS_CH1                                           \
                           DDR_2133, {  0,     0,    32,     0  },  \
                           DDR_2400, {  0,     0,    32,     0  },  \
                           DDR_2666, {  0,     0,    32,     0  },  \
                           DDR_2933, {  0,     0,    32,     0  },  \
                           DDR_3200, {  0,     0,    32,     0  }

//                          DdrFreq    DATA    CLK   CMD   CMDPI0
#define XOVER_OFFSETS_CH2                                           \
                           DDR_2133, {  0,     0,    32,     0  },  \
                           DDR_2400, {  0,     0,    32,     0  },  \
                           DDR_2666, {  0,     0,    32,     0  },  \
                           DDR_2933, {  0,     0,    32,     0  },  \
                           DDR_3200, {  0,     0,    32,     0  }

//                          DdrFreq    DATA    CLK   CMD   CMDPI0
#define XOVER_OFFSETS_CH3                                           \
                           DDR_2133, {  0,     0,    32,     0  },  \
                           DDR_2400, {  0,     0,    32,     0  },  \
                           DDR_2666, {  0,     0,    32,     0  },  \
                           DDR_2933, {  0,     0,    32,     0  },  \
                           DDR_3200, {  0,     0,    32,     0  }

//                          DdrFreq    DATA    CLK   CMD   CMDPI0
#define XOVER_OFFSETS_CH4                                           \
                           DDR_2133, {  0,     0,    32,     0  },  \
                           DDR_2400, {  0,     0,    32,     0  },  \
                           DDR_2666, {  0,     0,    32,     0  },  \
                           DDR_2933, {  0,     0,    32,     0  },  \
                           DDR_3200, {  0,     0,    32,     0  }

//                          DdrFreq    DATA    CLK   CMD   CMDPI0
#define XOVER_OFFSETS_CH5                                           \
                           DDR_2133, {  0,     0,    32,     0  },  \
                           DDR_2400, {  0,     0,    32,     0  },  \
                           DDR_2666, {  0,     0,    32,     0  },  \
                           DDR_2933, {  0,     0,    32,     0  },  \
                           DDR_3200, {  0,     0,    32,     0  }

//                          DdrFreq    DATA    CLK   CMD   CMDPI0
#define XOVER_OFFSETS_CH6                                           \
                           DDR_2133, {  0,     0,    32,     0  },  \
                           DDR_2400, {  0,     0,    32,     0  },  \
                           DDR_2666, {  0,     0,    32,     0  },  \
                           DDR_2933, {  0,     0,    32,     0  },  \
                           DDR_3200, {  0,     0,    32,     0  }

//                          DdrFreq    DATA    CLK   CMD   CMDPI0
#define XOVER_OFFSETS_CH7                                           \
                           DDR_2133, {  0,     0,    32,     0  },  \
                           DDR_2400, {  0,     0,    32,     0  },  \
                           DDR_2666, {  0,     0,    32,     0  },  \
                           DDR_2933, {  0,     0,    32,     0  },  \
                           DDR_3200, {  0,     0,    32,     0  }


#endif   // __MEM_XOVER_CALIB_OFFSETS_H__
