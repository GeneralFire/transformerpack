/** @file

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


#ifndef _fmcoptime_h
#define _fmcoptime_h

// NVMCTLR FIS 1.15
#define FMC_OPFREQ_1600MTS             0x00
#define FMC_OPFREQ_1866MTS             0x01
#define FMC_OPFREQ_2133MTS             0x02
#define FMC_OPFREQ_2400MTS             0x03
#define FMC_OPFREQ_2666MTS             0x04
#define FMC_OPFREQ_2933MTS             0x05
#define FMC_OPFREQ_3200MTS             0x06
//CSP FIS Rev: 3.0-pre1
#define FMC_OPFREQ_3600MTS             0x07
#define FMC_OPFREQ_4000MTS             0x08
#define FMC_OPFREQ_4400MTS             0x09
#define FMC_OPFREQ_4800MTS             0x0a

//
// FMC Frequency index definitions
//
typedef enum {
  DDRT_FREQUENCY_INDEX0 = 0,
  DDRT_FREQUENCY_INDEX1,
  DDRT_FREQUENCY_INDEX2,
  DDRT_FREQUENCY_INDEX3,
  DDRT_FREQUENCY_INDEX4,
  DDRT_FREQUENCY_INDEX5,
  DDRT_FREQUENCY_INDEX6,
  DDRT_FREQUENCY_INDEX7
} DDRT_SUPPORTED_FREQUENCIES_INDEX;

#endif // _fmcoptime_h
