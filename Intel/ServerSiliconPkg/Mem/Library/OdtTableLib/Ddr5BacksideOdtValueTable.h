/** @file
  ODT table of "backside" ODT settings for DDR5 RDIMMs and LRDIMMs

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation. <BR>

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

#ifndef _DDR5_BACKSIDE_ODT_VALUE_TABLE_H_
#define _DDR5_BACKSIDE_ODT_VALUE_TABLE_H_

#include <MemRegs.h>
#include <MemCommon.h>
#include "DdrOdtPrivate.h"
#include "Include/Memory/JedecDefinitions.h"

DDR_BACKSIDE_ODT_VALUE_TABLE_ENTRY Ddr5BacksideOdtValueTable[] = {
  {    // Config Index
    (RDIMM << 0)       | // type,
    (RAW_CARD_A << 5)  | // slot0,
    (0 << 10)          | // revision,
    (DDR_4800 << 13)   | // frequency,
    (QCA_VREF << 18),    // Vref Type,
    // Position-based Data
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 0)  |  // P1,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 3)  |  // P2,
    (DDR5_RTT_CK_CS_CA_RTT_480_BITS << 6)  |  // P3,
    (DDR5_RTT_CK_CS_CA_RTT_480_BITS << 9)  |  // P4,
    (DDR5_RTT_CK_CS_CA_RTT_80_BITS << 12),  // P5,
    132,    // Percentage * 2,
    DDR5_RCD_OUTPUT_DRIVE_STRENGTH_14_OHMS    // RCD Output Drive Strength
  },
  {    // Config Index
    (RDIMM << 0)       | // type,
    (RAW_CARD_A << 5)  | // slot0,
    (0 << 10)          | // revision,
    (DDR_4800 << 13)   | // frequency,
    (QCS_VREF << 18),    // Vref Type,
    // Position-based Data
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 0)  |  // P1,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 3)  |  // P2,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 6)  |  // P3,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 9)  |  // P4,
    (DDR5_RTT_CK_CS_CA_RTT_40_BITS << 12),  // P5,
    133,    // Percentage * 2,
    DDR5_RCD_OUTPUT_DRIVE_STRENGTH_20_OHMS    // RCD Output Drive Strength
  },
  {    // Config Index
    (RDIMM << 0)       | // type,
    (RAW_CARD_A << 5)  | // slot0,
    (0 << 10)          | // revision,
    (DDR_4800 << 13)   | // frequency,
    (QCK_VREF << 18),    // Vref Type,
    // Position-based Data
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 0)  |  // P1,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 3)  |  // P2,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 6)  |  // P3,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 9)  |  // P4,
    (DDR5_RTT_CK_CS_CA_RTT_60_BITS << 12),  // P5,
    0,    // Percentage * 2,
    DDR5_RCD_OUTPUT_DRIVE_STRENGTH_14_OHMS    // RCD Output Drive Strength
  },
  {    // Config Index
    (RDIMM << 0)       | // type,
    (RAW_CARD_B << 5)  | // slot0,
    (0 << 10)          | // revision,
    (DDR_4800 << 13)   | // frequency,
    (QCA_VREF << 18),    // Vref Type,
    // Position-based Data
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 0)  |  // P1,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 3)  |  // P2,
    (DDR5_RTT_CK_CS_CA_RTT_480_BITS << 6)  |  // P3,
    (DDR5_RTT_CK_CS_CA_RTT_480_BITS << 9)  |  // P4,
    (DDR5_RTT_CK_CS_CA_RTT_80_BITS << 12),  // P5,
    132,    // Percentage * 2,
    DDR5_RCD_OUTPUT_DRIVE_STRENGTH_14_OHMS    // RCD Output Drive Strength
  },
  {    // Config Index
    (RDIMM << 0)       | // type,
    (RAW_CARD_B << 5)  | // slot0,
    (0 << 10)          | // revision,
    (DDR_4800 << 13)   | // frequency,
    (QCS_VREF << 18),    // Vref Type,
    // Position-based Data
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 0)  |  // P1,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 3)  |  // P2,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 6)  |  // P3,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 9)  |  // P4,
    (DDR5_RTT_CK_CS_CA_RTT_40_BITS << 12),  // P5,
    133,    // Percentage * 2,
    DDR5_RCD_OUTPUT_DRIVE_STRENGTH_20_OHMS    // RCD Output Drive Strength
  },
  {    // Config Index
    (RDIMM << 0)       | // type,
    (RAW_CARD_B << 5)  | // slot0,
    (0 << 10)          | // revision,
    (DDR_4800 << 13)   | // frequency,
    (QCK_VREF << 18),    // Vref Type,
    // Position-based Data
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 0)  |  // P1,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 3)  |  // P2,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 6)  |  // P3,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 9)  |  // P4,
    (DDR5_RTT_CK_CS_CA_RTT_60_BITS << 12),  // P5,
    0,    // Percentage * 2,
    DDR5_RCD_OUTPUT_DRIVE_STRENGTH_14_OHMS    // RCD Output Drive Strength
  },
  {    // Config Index
    (RDIMM << 0)       | // type,
    (RAW_CARD_C << 5)  | // slot0,
    (0 << 10)          | // revision,
    (DDR_4800 << 13)   | // frequency,
    (QCA_VREF << 18),    // Vref Type,
    // Position-based Data
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 0)  |  // P1,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 3)  |  // P2,
    (DDR5_RTT_CK_CS_CA_RTT_480_BITS << 6)  |  // P3,
    (DDR5_RTT_CK_CS_CA_RTT_480_BITS << 9)  |  // P4,
    (DDR5_RTT_CK_CS_CA_RTT_80_BITS << 12),  // P5,
    132,    // Percentage * 2,
    DDR5_RCD_OUTPUT_DRIVE_STRENGTH_14_OHMS    // RCD Output Drive Strength
  },
  {    // Config Index
    (RDIMM << 0)       | // type,
    (RAW_CARD_C << 5)  | // slot0,
    (0 << 10)          | // revision,
    (DDR_4800 << 13)   | // frequency,
    (QCS_VREF << 18),    // Vref Type,
    // Position-based Data
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 0)  |  // P1,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 3)  |  // P2,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 6)  |  // P3,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 9)  |  // P4,
    (DDR5_RTT_CK_CS_CA_RTT_40_BITS << 12),  // P5,
    133,    // Percentage * 2,
    DDR5_RCD_OUTPUT_DRIVE_STRENGTH_20_OHMS    // RCD Output Drive Strength
  },
  {    // Config Index
    (RDIMM << 0)       | // type,
    (RAW_CARD_C << 5)  | // slot0,
    (0 << 10)          | // revision,
    (DDR_4800 << 13)   | // frequency,
    (QCK_VREF << 18),    // Vref Type,
    // Position-based Data
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 0)  |  // P1,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 3)  |  // P2,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 6)  |  // P3,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 9)  |  // P4,
    (DDR5_RTT_CK_CS_CA_RTT_60_BITS << 12),  // P5,
    0,    // Percentage * 2,
    DDR5_RCD_OUTPUT_DRIVE_STRENGTH_14_OHMS    // RCD Output Drive Strength
  },
  {    // Config Index
    (RDIMM << 0)       | // type,
    (RAW_CARD_D << 5)  | // slot0,
    (0 << 10)          | // revision,
    (DDR_4800 << 13)   | // frequency,
    (QCA_VREF << 18),    // Vref Type,
    // Position-based Data
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 0)  |  // P1,
    (DDR5_RTT_CK_CS_CA_RTT_480_BITS << 3)  |  // P2,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 6)  |  // P3,
    (DDR5_RTT_CK_CS_CA_RTT_480_BITS << 9)  |  // P4,
    (DDR5_RTT_CK_CS_CA_RTT_40_BITS << 12),  // P5,
    137,    // Percentage * 2,
    DDR5_RCD_OUTPUT_DRIVE_STRENGTH_20_OHMS    // RCD Output Drive Strength
  },
  {    // Config Index
    (RDIMM << 0)       | // type,
    (RAW_CARD_D << 5)  | // slot0,
    (0 << 10)          | // revision,
    (DDR_4800 << 13)   | // frequency,
    (QCS_VREF << 18),    // Vref Type,
    // Position-based Data
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 0)  |  // P1,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 3)  |  // P2,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 6)  |  // P3,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 9)  |  // P4,
    (DDR5_RTT_CK_CS_CA_RTT_40_BITS << 12),  // P5,
    133,    // Percentage * 2,
    DDR5_RCD_OUTPUT_DRIVE_STRENGTH_20_OHMS    // RCD Output Drive Strength
  },
  {    // Config Index
    (RDIMM << 0)       | // type,
    (RAW_CARD_D << 5)  | // slot0,
    (0 << 10)          | // revision,
    (DDR_4800 << 13)   | // frequency,
    (QCK_VREF << 18),    // Vref Type,
    // Position-based Data
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 0)  |  // P1,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 3)  |  // P2,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 6)  |  // P3,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 9)  |  // P4,
    (DDR5_RTT_CK_CS_CA_RTT_60_BITS << 12),  // P5,
    0,    // Percentage * 2,
    DDR5_RCD_OUTPUT_DRIVE_STRENGTH_14_OHMS    // RCD Output Drive Strength
  },
  {    // Config Index
    (RDIMM << 0)       | // type,
    (RAW_CARD_E << 5)  | // slot0,
    (0 << 10)          | // revision,
    (DDR_4800 << 13)   | // frequency,
    (QCA_VREF << 18),    // Vref Type,
    // Position-based Data
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 0)  |  // P1,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 3)  |  // P2,
    (DDR5_RTT_CK_CS_CA_RTT_480_BITS << 6)  |  // P3,
    (DDR5_RTT_CK_CS_CA_RTT_480_BITS << 9)  |  // P4,
    (DDR5_RTT_CK_CS_CA_RTT_80_BITS << 12),  // P5,
    132,    // Percentage * 2,
    DDR5_RCD_OUTPUT_DRIVE_STRENGTH_14_OHMS    // RCD Output Drive Strength
  },
  {    // Config Index
    (RDIMM << 0)       | // type,
    (RAW_CARD_E << 5)  | // slot0,
    (0 << 10)          | // revision,
    (DDR_4800 << 13)   | // frequency,
    (QCS_VREF << 18),    // Vref Type,
    // Position-based Data
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 0)  |  // P1,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 3)  |  // P2,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 6)  |  // P3,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 9)  |  // P4,
    (DDR5_RTT_CK_CS_CA_RTT_40_BITS << 12),  // P5,
    133,    // Percentage * 2,
    DDR5_RCD_OUTPUT_DRIVE_STRENGTH_20_OHMS    // RCD Output Drive Strength
  },
  {    // Config Index
    (RDIMM << 0)       | // type,
    (RAW_CARD_E << 5)  | // slot0,
    (0 << 10)          | // revision,
    (DDR_4800 << 13)   | // frequency,
    (QCK_VREF << 18),    // Vref Type,
    // Position-based Data
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 0)  |  // P1,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 3)  |  // P2,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 6)  |  // P3,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 9)  |  // P4,
    (DDR5_RTT_CK_CS_CA_RTT_60_BITS << 12),  // P5,
    0,    // Percentage * 2,
    DDR5_RCD_OUTPUT_DRIVE_STRENGTH_14_OHMS    // RCD Output Drive Strength
  },
  {    // Config Index
    (LRDIMM << 0)      | // type,
    (RAW_CARD_A << 5)  | // slot0,
    (0 << 10)          | // revision,
    (DDR_4800 << 13)   | // frequency,
    (QCA_VREF << 18),    // Vref Type,
    // Position-based Data
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 0)  |  // P1,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 3)  |  // P2,
    (DDR5_RTT_CK_CS_CA_RTT_480_BITS << 6)  |  // P3,
    (DDR5_RTT_CK_CS_CA_RTT_480_BITS << 9)  |  // P4,
    (DDR5_RTT_CK_CS_CA_RTT_80_BITS << 12),  // P5,
    132,    // Percentage * 2,
    DDR5_RCD_OUTPUT_DRIVE_STRENGTH_14_OHMS    // RCD Output Drive Strength
  },
  {    // Config Index
    (LRDIMM << 0)      | // type,
    (RAW_CARD_B << 5)  | // slot0,
    (0 << 10)          | // revision,
    (DDR_4800 << 13)   | // frequency,
    (QCA_VREF << 18),    // Vref Type,
    // Position-based Data
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 0)  |  // P1,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 3)  |  // P2,
    (DDR5_RTT_CK_CS_CA_RTT_480_BITS << 6)  |  // P3,
    (DDR5_RTT_CK_CS_CA_RTT_480_BITS << 9)  |  // P4,
    (DDR5_RTT_CK_CS_CA_RTT_80_BITS << 12),  // P5,
    132,    // Percentage * 2,
    DDR5_RCD_OUTPUT_DRIVE_STRENGTH_14_OHMS    // RCD Output Drive Strength
  },
  {    // Config Index
    (LRDIMM << 0)      | // type,
    (RAW_CARD_A << 5)  | // slot0,
    (0 << 10)          | // revision,
    (DDR_4800 << 13)   | // frequency,
    (QCS_VREF << 18),    // Vref Type,
    // Position-based Data
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 0)  |  // P1,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 3)  |  // P2,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 6)  |  // P3,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 9)  |  // P4,
    (DDR5_RTT_CK_CS_CA_RTT_40_BITS << 12),  // P5,
    133,    // Percentage * 2,
    DDR5_RCD_OUTPUT_DRIVE_STRENGTH_20_OHMS    // RCD Output Drive Strength
  },
  {    // Config Index
    (LRDIMM << 0)      | // type,
    (RAW_CARD_B << 5)  | // slot0,
    (0 << 10)          | // revision,
    (DDR_4800 << 13)   | // frequency,
    (QCS_VREF << 18),    // Vref Type,
    // Position-based Data
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 0)  |  // P1,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 3)  |  // P2,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 6)  |  // P3,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 9)  |  // P4,
    (DDR5_RTT_CK_CS_CA_RTT_40_BITS << 12),  // P5,
    133,    // Percentage * 2,
    DDR5_RCD_OUTPUT_DRIVE_STRENGTH_20_OHMS    // RCD Output Drive Strength
  },
  {    // Config Index
    (LRDIMM << 0)      | // type,
    (RAW_CARD_A << 5)  | // slot0,
    (0 << 10)          | // revision,
    (DDR_4800 << 13)   | // frequency,
    (QCK_VREF << 18),    // Vref Type,
    // Position-based Data
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 0)  |  // P1,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 3)  |  // P2,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 6)  |  // P3,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 9)  |  // P4,
    (DDR5_RTT_CK_CS_CA_RTT_60_BITS << 12),  // P5,
    0,    // Percentage * 2,
    DDR5_RCD_OUTPUT_DRIVE_STRENGTH_14_OHMS    // RCD Output Drive Strength
  },
  {    // Config Index
    (LRDIMM << 0)      | // type,
    (RAW_CARD_B << 5)  | // slot0,
    (0 << 10)          | // revision,
    (DDR_4800 << 13)   | // frequency,
    (QCK_VREF << 18),    // Vref Type,
    // Position-based Data
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 0)  |  // P1,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 3)  |  // P2,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 6)  |  // P3,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 9)  |  // P4,
    (DDR5_RTT_CK_CS_CA_RTT_60_BITS << 12),  // P5,
    0,    // Percentage * 2,
    DDR5_RCD_OUTPUT_DRIVE_STRENGTH_14_OHMS    // RCD Output Drive Strength
  },
  {    // Config Index
    (RDIMM << 0)       | // type,
    (RAW_CARD_A << 5)  | // slot0,
    (0 << 10)          | // revision,
    (DDR_4400 << 13)   | // frequency,
    (QCA_VREF << 18),    // Vref Type,
    // Position-based Data
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 0)  |  // P1,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 3)  |  // P2,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 6)  |  // P3,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 9)  |  // P4,
    (DDR5_RTT_CK_CS_CA_RTT_80_BITS << 12),  // P5,
    126,    // Percentage * 2,
    DDR5_RCD_OUTPUT_DRIVE_STRENGTH_14_OHMS    // RCD Output Drive Strength
  },
  {    // Config Index
    (RDIMM << 0)       | // type,
    (RAW_CARD_A << 5)  | // slot0,
    (0 << 10)          | // revision,
    (DDR_4400 << 13)   | // frequency,
    (QCS_VREF << 18),    // Vref Type,
    // Position-based Data
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 0)  |  // P1,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 3)  |  // P2,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 6)  |  // P3,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 9)  |  // P4,
    (DDR5_RTT_CK_CS_CA_RTT_40_BITS << 12),  // P5,
    133,    // Percentage * 2,
    DDR5_RCD_OUTPUT_DRIVE_STRENGTH_20_OHMS    // RCD Output Drive Strength
  },
  {    // Config Index
    (RDIMM << 0)       | // type,
    (RAW_CARD_A << 5)  | // slot0,
    (0 << 10)          | // revision,
    (DDR_4400 << 13)   | // frequency,
    (QCK_VREF << 18),    // Vref Type,
    // Position-based Data
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 0)  |  // P1,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 3)  |  // P2,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 6)  |  // P3,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 9)  |  // P4,
    (DDR5_RTT_CK_CS_CA_RTT_60_BITS << 12),  // P5,
    0,    // Percentage * 2,
    DDR5_RCD_OUTPUT_DRIVE_STRENGTH_14_OHMS    // RCD Output Drive Strength
  },
  {    // Config Index
    (RDIMM << 0)       | // type,
    (RAW_CARD_B << 5)  | // slot0,
    (0 << 10)          | // revision,
    (DDR_4400 << 13)   | // frequency,
    (QCA_VREF << 18),    // Vref Type,
    // Position-based Data
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 0)  |  // P1,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 3)  |  // P2,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 6)  |  // P3,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 9)  |  // P4,
    (DDR5_RTT_CK_CS_CA_RTT_80_BITS << 12),  // P5,
    126,    // Percentage * 2,
    DDR5_RCD_OUTPUT_DRIVE_STRENGTH_14_OHMS    // RCD Output Drive Strength
  },
  {    // Config Index
    (RDIMM << 0)       | // type,
    (RAW_CARD_B << 5)  | // slot0,
    (0 << 10)          | // revision,
    (DDR_4400 << 13)   | // frequency,
    (QCS_VREF << 18),    // Vref Type,
    // Position-based Data
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 0)  |  // P1,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 3)  |  // P2,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 6)  |  // P3,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 9)  |  // P4,
    (DDR5_RTT_CK_CS_CA_RTT_40_BITS << 12),  // P5,
    133,    // Percentage * 2,
    DDR5_RCD_OUTPUT_DRIVE_STRENGTH_20_OHMS    // RCD Output Drive Strength
  },
  {    // Config Index
    (RDIMM << 0)       | // type,
    (RAW_CARD_B << 5)  | // slot0,
    (0 << 10)          | // revision,
    (DDR_4400 << 13)   | // frequency,
    (QCK_VREF << 18),    // Vref Type,
    // Position-based Data
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 0)  |  // P1,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 3)  |  // P2,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 6)  |  // P3,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 9)  |  // P4,
    (DDR5_RTT_CK_CS_CA_RTT_60_BITS << 12),  // P5,
    0,    // Percentage * 2,
    DDR5_RCD_OUTPUT_DRIVE_STRENGTH_14_OHMS    // RCD Output Drive Strength
  },
  {    // Config Index
    (RDIMM << 0)       | // type,
    (RAW_CARD_C << 5)  | // slot0,
    (0 << 10)          | // revision,
    (DDR_4400 << 13)   | // frequency,
    (QCA_VREF << 18),    // Vref Type,
    // Position-based Data
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 0)  |  // P1,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 3)  |  // P2,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 6)  |  // P3,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 9)  |  // P4,
    (DDR5_RTT_CK_CS_CA_RTT_80_BITS << 12),  // P5,
    126,    // Percentage * 2,
    DDR5_RCD_OUTPUT_DRIVE_STRENGTH_14_OHMS    // RCD Output Drive Strength
  },
  {    // Config Index
    (RDIMM << 0)       | // type,
    (RAW_CARD_C << 5)  | // slot0,
    (0 << 10)          | // revision,
    (DDR_4400 << 13)   | // frequency,
    (QCS_VREF << 18),    // Vref Type,
    // Position-based Data
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 0)  |  // P1,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 3)  |  // P2,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 6)  |  // P3,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 9)  |  // P4,
    (DDR5_RTT_CK_CS_CA_RTT_40_BITS << 12),  // P5,
    133,    // Percentage * 2,
    DDR5_RCD_OUTPUT_DRIVE_STRENGTH_20_OHMS    // RCD Output Drive Strength
  },
  {    // Config Index
    (RDIMM << 0)       | // type,
    (RAW_CARD_C << 5)  | // slot0,
    (0 << 10)          | // revision,
    (DDR_4400 << 13)   | // frequency,
    (QCK_VREF << 18),    // Vref Type,
    // Position-based Data
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 0)  |  // P1,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 3)  |  // P2,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 6)  |  // P3,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 9)  |  // P4,
    (DDR5_RTT_CK_CS_CA_RTT_60_BITS << 12),  // P5,
    0,    // Percentage * 2,
    DDR5_RCD_OUTPUT_DRIVE_STRENGTH_14_OHMS    // RCD Output Drive Strength
  },
  {    // Config Index
    (RDIMM << 0)       | // type,
    (RAW_CARD_D << 5)  | // slot0,
    (0 << 10)          | // revision,
    (DDR_4400 << 13)   | // frequency,
    (QCA_VREF << 18),    // Vref Type,
    // Position-based Data
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 0)  |  // P1,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 3)  |  // P2,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 6)  |  // P3,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 9)  |  // P4,
    (DDR5_RTT_CK_CS_CA_RTT_40_BITS << 12),  // P5,
    133,    // Percentage * 2,
    DDR5_RCD_OUTPUT_DRIVE_STRENGTH_20_OHMS    // RCD Output Drive Strength
  },
  {    // Config Index
    (RDIMM << 0)       | // type,
    (RAW_CARD_D << 5)  | // slot0,
    (0 << 10)          | // revision,
    (DDR_4400 << 13)   | // frequency,
    (QCS_VREF << 18),    // Vref Type,
    // Position-based Data
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 0)  |  // P1,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 3)  |  // P2,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 6)  |  // P3,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 9)  |  // P4,
    (DDR5_RTT_CK_CS_CA_RTT_40_BITS << 12),  // P5,
    133,    // Percentage * 2,
    DDR5_RCD_OUTPUT_DRIVE_STRENGTH_20_OHMS    // RCD Output Drive Strength
  },
  {    // Config Index
    (RDIMM << 0)       | // type,
    (RAW_CARD_D << 5)  | // slot0,
    (0 << 10)          | // revision,
    (DDR_4400 << 13)   | // frequency,
    (QCK_VREF << 18),    // Vref Type,
    // Position-based Data
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 0)  |  // P1,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 3)  |  // P2,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 6)  |  // P3,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 9)  |  // P4,
    (DDR5_RTT_CK_CS_CA_RTT_60_BITS << 12),  // P5,
    0,    // Percentage * 2,
    DDR5_RCD_OUTPUT_DRIVE_STRENGTH_14_OHMS    // RCD Output Drive Strength
  },
  {    // Config Index
    (RDIMM << 0)       | // type,
    (RAW_CARD_E << 5)  | // slot0,
    (0 << 10)          | // revision,
    (DDR_4400 << 13)   | // frequency,
    (QCA_VREF << 18),    // Vref Type,
    // Position-based Data
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 0)  |  // P1,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 3)  |  // P2,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 6)  |  // P3,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 9)  |  // P4,
    (DDR5_RTT_CK_CS_CA_RTT_80_BITS << 12),  // P5,
    126,    // Percentage * 2,
    DDR5_RCD_OUTPUT_DRIVE_STRENGTH_14_OHMS    // RCD Output Drive Strength
  },
  {    // Config Index
    (RDIMM << 0)       | // type,
    (RAW_CARD_E << 5)  | // slot0,
    (0 << 10)          | // revision,
    (DDR_4400 << 13)   | // frequency,
    (QCS_VREF << 18),    // Vref Type,
    // Position-based Data
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 0)  |  // P1,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 3)  |  // P2,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 6)  |  // P3,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 9)  |  // P4,
    (DDR5_RTT_CK_CS_CA_RTT_40_BITS << 12),  // P5,
    133,    // Percentage * 2,
    DDR5_RCD_OUTPUT_DRIVE_STRENGTH_20_OHMS    // RCD Output Drive Strength
  },
  {    // Config Index
    (RDIMM << 0)       | // type,
    (RAW_CARD_E << 5)  | // slot0,
    (0 << 10)          | // revision,
    (DDR_4400 << 13)   | // frequency,
    (QCK_VREF << 18),    // Vref Type,
    // Position-based Data
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 0)  |  // P1,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 3)  |  // P2,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 6)  |  // P3,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 9)  |  // P4,
    (DDR5_RTT_CK_CS_CA_RTT_60_BITS << 12),  // P5,
    0,    // Percentage * 2,
    DDR5_RCD_OUTPUT_DRIVE_STRENGTH_14_OHMS    // RCD Output Drive Strength
  },
  {    // Config Index
    (LRDIMM << 0)      | // type,
    (RAW_CARD_A << 5)  | // slot0,
    (0 << 10)          | // revision,
    (DDR_4400 << 13)   | // frequency,
    (QCA_VREF << 18),    // Vref Type,
    // Position-based Data
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 0)  |  // P1,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 3)  |  // P2,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 6)  |  // P3,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 9)  |  // P4,
    (DDR5_RTT_CK_CS_CA_RTT_80_BITS << 12),  // P5,
    126,    // Percentage * 2,
    DDR5_RCD_OUTPUT_DRIVE_STRENGTH_14_OHMS    // RCD Output Drive Strength
  },
  {    // Config Index
    (LRDIMM << 0)      | // type,
    (RAW_CARD_B << 5)  | // slot0,
    (0 << 10)          | // revision,
    (DDR_4400 << 13)   | // frequency,
    (QCA_VREF << 18),    // Vref Type,
    // Position-based Data
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 0)  |  // P1,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 3)  |  // P2,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 6)  |  // P3,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 9)  |  // P4,
    (DDR5_RTT_CK_CS_CA_RTT_80_BITS << 12),  // P5,
    126,    // Percentage * 2,
    DDR5_RCD_OUTPUT_DRIVE_STRENGTH_14_OHMS    // RCD Output Drive Strength
  },
  {    // Config Index
    (LRDIMM << 0)      | // type,
    (RAW_CARD_A << 5)  | // slot0,
    (0 << 10)          | // revision,
    (DDR_4400 << 13)   | // frequency,
    (QCS_VREF << 18),    // Vref Type,
    // Position-based Data
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 0)  |  // P1,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 3)  |  // P2,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 6)  |  // P3,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 9)  |  // P4,
    (DDR5_RTT_CK_CS_CA_RTT_40_BITS << 12),  // P5,
    133,    // Percentage * 2,
    DDR5_RCD_OUTPUT_DRIVE_STRENGTH_20_OHMS    // RCD Output Drive Strength
  },
  {    // Config Index
    (LRDIMM << 0)      | // type,
    (RAW_CARD_B << 5)  | // slot0,
    (0 << 10)          | // revision,
    (DDR_4400 << 13)   | // frequency,
    (QCS_VREF << 18),    // Vref Type,
    // Position-based Data
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 0)  |  // P1,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 3)  |  // P2,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 6)  |  // P3,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 9)  |  // P4,
    (DDR5_RTT_CK_CS_CA_RTT_40_BITS << 12),  // P5,
    133,    // Percentage * 2,
    DDR5_RCD_OUTPUT_DRIVE_STRENGTH_20_OHMS    // RCD Output Drive Strength
  },
  {    // Config Index
    (LRDIMM << 0)      | // type,
    (RAW_CARD_A << 5)  | // slot0,
    (0 << 10)          | // revision,
    (DDR_4400 << 13)   | // frequency,
    (QCK_VREF << 18),    // Vref Type,
    // Position-based Data
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 0)  |  // P1,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 3)  |  // P2,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 6)  |  // P3,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 9)  |  // P4,
    (DDR5_RTT_CK_CS_CA_RTT_60_BITS << 12),  // P5,
    0,    // Percentage * 2,
    DDR5_RCD_OUTPUT_DRIVE_STRENGTH_14_OHMS    // RCD Output Drive Strength
  },
  {    // Config Index
    (LRDIMM << 0)      | // type,
    (RAW_CARD_B << 5)  | // slot0,
    (0 << 10)          | // revision,
    (DDR_4400 << 13)   | // frequency,
    (QCK_VREF << 18),    // Vref Type,
    // Position-based Data
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 0)  |  // P1,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 3)  |  // P2,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 6)  |  // P3,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 9)  |  // P4,
    (DDR5_RTT_CK_CS_CA_RTT_60_BITS << 12),  // P5,
    0,    // Percentage * 2,
    DDR5_RCD_OUTPUT_DRIVE_STRENGTH_14_OHMS    // RCD Output Drive Strength
  },
  {    // Config Index
    (RDIMM << 0)       | // type,
    (RAW_CARD_A << 5)  | // slot0,
    (0 << 10)          | // revision,
    (DDR_4000 << 13)   | // frequency,
    (QCA_VREF << 18),    // Vref Type,
    // Position-based Data
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 0)  |  // P1,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 3)  |  // P2,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 6)  |  // P3,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 9)  |  // P4,
    (DDR5_RTT_CK_CS_CA_RTT_80_BITS << 12),  // P5,
    126,    // Percentage * 2,
    DDR5_RCD_OUTPUT_DRIVE_STRENGTH_14_OHMS    // RCD Output Drive Strength
  },
  {    // Config Index
    (RDIMM << 0)       | // type,
    (RAW_CARD_A << 5)  | // slot0,
    (0 << 10)          | // revision,
    (DDR_4000 << 13)   | // frequency,
    (QCS_VREF << 18),    // Vref Type,
    // Position-based Data
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 0)  |  // P1,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 3)  |  // P2,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 6)  |  // P3,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 9)  |  // P4,
    (DDR5_RTT_CK_CS_CA_RTT_40_BITS << 12),  // P5,
    133,    // Percentage * 2,
    DDR5_RCD_OUTPUT_DRIVE_STRENGTH_20_OHMS    // RCD Output Drive Strength
  },
  {    // Config Index
    (RDIMM << 0)       | // type,
    (RAW_CARD_A << 5)  | // slot0,
    (0 << 10)          | // revision,
    (DDR_4000 << 13)   | // frequency,
    (QCK_VREF << 18),    // Vref Type,
    // Position-based Data
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 0)  |  // P1,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 3)  |  // P2,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 6)  |  // P3,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 9)  |  // P4,
    (DDR5_RTT_CK_CS_CA_RTT_60_BITS << 12),  // P5,
    0,    // Percentage * 2,
    DDR5_RCD_OUTPUT_DRIVE_STRENGTH_14_OHMS    // RCD Output Drive Strength
  },
  {    // Config Index
    (RDIMM << 0)       | // type,
    (RAW_CARD_B << 5)  | // slot0,
    (0 << 10)          | // revision,
    (DDR_4000 << 13)   | // frequency,
    (QCA_VREF << 18),    // Vref Type,
    // Position-based Data
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 0)  |  // P1,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 3)  |  // P2,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 6)  |  // P3,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 9)  |  // P4,
    (DDR5_RTT_CK_CS_CA_RTT_80_BITS << 12),  // P5,
    126,    // Percentage * 2,
    DDR5_RCD_OUTPUT_DRIVE_STRENGTH_14_OHMS    // RCD Output Drive Strength
  },
  {    // Config Index
    (RDIMM << 0)       | // type,
    (RAW_CARD_B << 5)  | // slot0,
    (0 << 10)          | // revision,
    (DDR_4000 << 13)   | // frequency,
    (QCS_VREF << 18),    // Vref Type,
    // Position-based Data
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 0)  |  // P1,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 3)  |  // P2,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 6)  |  // P3,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 9)  |  // P4,
    (DDR5_RTT_CK_CS_CA_RTT_40_BITS << 12),  // P5,
    133,    // Percentage * 2,
    DDR5_RCD_OUTPUT_DRIVE_STRENGTH_20_OHMS    // RCD Output Drive Strength
  },
  {    // Config Index
    (RDIMM << 0)       | // type,
    (RAW_CARD_B << 5)  | // slot0,
    (0 << 10)          | // revision,
    (DDR_4000 << 13)   | // frequency,
    (QCK_VREF << 18),    // Vref Type,
    // Position-based Data
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 0)  |  // P1,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 3)  |  // P2,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 6)  |  // P3,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 9)  |  // P4,
    (DDR5_RTT_CK_CS_CA_RTT_60_BITS << 12),  // P5,
    0,    // Percentage * 2,
    DDR5_RCD_OUTPUT_DRIVE_STRENGTH_14_OHMS    // RCD Output Drive Strength
  },
  {    // Config Index
    (RDIMM << 0)       | // type,
    (RAW_CARD_C << 5)  | // slot0,
    (0 << 10)          | // revision,
    (DDR_4000 << 13)   | // frequency,
    (QCA_VREF << 18),    // Vref Type,
    // Position-based Data
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 0)  |  // P1,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 3)  |  // P2,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 6)  |  // P3,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 9)  |  // P4,
    (DDR5_RTT_CK_CS_CA_RTT_80_BITS << 12),  // P5,
    126,    // Percentage * 2,
    DDR5_RCD_OUTPUT_DRIVE_STRENGTH_14_OHMS    // RCD Output Drive Strength
  },
  {    // Config Index
    (RDIMM << 0)       | // type,
    (RAW_CARD_C << 5)  | // slot0,
    (0 << 10)          | // revision,
    (DDR_4000 << 13)   | // frequency,
    (QCS_VREF << 18),    // Vref Type,
    // Position-based Data
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 0)  |  // P1,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 3)  |  // P2,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 6)  |  // P3,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 9)  |  // P4,
    (DDR5_RTT_CK_CS_CA_RTT_40_BITS << 12),  // P5,
    133,    // Percentage * 2,
    DDR5_RCD_OUTPUT_DRIVE_STRENGTH_20_OHMS    // RCD Output Drive Strength
  },
  {    // Config Index
    (RDIMM << 0)       | // type,
    (RAW_CARD_C << 5)  | // slot0,
    (0 << 10)          | // revision,
    (DDR_4000 << 13)   | // frequency,
    (QCK_VREF << 18),    // Vref Type,
    // Position-based Data
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 0)  |  // P1,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 3)  |  // P2,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 6)  |  // P3,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 9)  |  // P4,
    (DDR5_RTT_CK_CS_CA_RTT_60_BITS << 12),  // P5,
    0,    // Percentage * 2,
    DDR5_RCD_OUTPUT_DRIVE_STRENGTH_14_OHMS    // RCD Output Drive Strength
  },
  {    // Config Index
    (RDIMM << 0)       | // type,
    (RAW_CARD_D << 5)  | // slot0,
    (0 << 10)          | // revision,
    (DDR_4000 << 13)   | // frequency,
    (QCA_VREF << 18),    // Vref Type,
    // Position-based Data
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 0)  |  // P1,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 3)  |  // P2,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 6)  |  // P3,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 9)  |  // P4,
    (DDR5_RTT_CK_CS_CA_RTT_40_BITS << 12),  // P5,
    133,    // Percentage * 2,
    DDR5_RCD_OUTPUT_DRIVE_STRENGTH_20_OHMS    // RCD Output Drive Strength
  },
  {    // Config Index
    (RDIMM << 0)       | // type,
    (RAW_CARD_D << 5)  | // slot0,
    (0 << 10)          | // revision,
    (DDR_4000 << 13)   | // frequency,
    (QCS_VREF << 18),    // Vref Type,
    // Position-based Data
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 0)  |  // P1,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 3)  |  // P2,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 6)  |  // P3,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 9)  |  // P4,
    (DDR5_RTT_CK_CS_CA_RTT_40_BITS << 12),  // P5,
    133,    // Percentage * 2,
    DDR5_RCD_OUTPUT_DRIVE_STRENGTH_20_OHMS    // RCD Output Drive Strength
  },
  {    // Config Index
    (RDIMM << 0)       | // type,
    (RAW_CARD_D << 5)  | // slot0,
    (0 << 10)          | // revision,
    (DDR_4000 << 13)   | // frequency,
    (QCK_VREF << 18),    // Vref Type,
    // Position-based Data
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 0)  |  // P1,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 3)  |  // P2,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 6)  |  // P3,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 9)  |  // P4,
    (DDR5_RTT_CK_CS_CA_RTT_60_BITS << 12),  // P5,
    0,    // Percentage * 2,
    DDR5_RCD_OUTPUT_DRIVE_STRENGTH_14_OHMS    // RCD Output Drive Strength
  },
  {    // Config Index
    (RDIMM << 0)       | // type,
    (RAW_CARD_E << 5)  | // slot0,
    (0 << 10)          | // revision,
    (DDR_4000 << 13)   | // frequency,
    (QCA_VREF << 18),    // Vref Type,
    // Position-based Data
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 0)  |  // P1,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 3)  |  // P2,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 6)  |  // P3,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 9)  |  // P4,
    (DDR5_RTT_CK_CS_CA_RTT_80_BITS << 12),  // P5,
    126,    // Percentage * 2,
    DDR5_RCD_OUTPUT_DRIVE_STRENGTH_14_OHMS    // RCD Output Drive Strength
  },
  {    // Config Index
    (RDIMM << 0)       | // type,
    (RAW_CARD_E << 5)  | // slot0,
    (0 << 10)          | // revision,
    (DDR_4000 << 13)   | // frequency,
    (QCS_VREF << 18),    // Vref Type,
    // Position-based Data
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 0)  |  // P1,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 3)  |  // P2,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 6)  |  // P3,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 9)  |  // P4,
    (DDR5_RTT_CK_CS_CA_RTT_40_BITS << 12),  // P5,
    133,    // Percentage * 2,
    DDR5_RCD_OUTPUT_DRIVE_STRENGTH_20_OHMS    // RCD Output Drive Strength
  },
  {    // Config Index
    (RDIMM << 0)       | // type,
    (RAW_CARD_E << 5)  | // slot0,
    (0 << 10)          | // revision,
    (DDR_4000 << 13)   | // frequency,
    (QCK_VREF << 18),    // Vref Type,
    // Position-based Data
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 0)  |  // P1,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 3)  |  // P2,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 6)  |  // P3,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 9)  |  // P4,
    (DDR5_RTT_CK_CS_CA_RTT_60_BITS << 12),  // P5,
    0,    // Percentage * 2,
    DDR5_RCD_OUTPUT_DRIVE_STRENGTH_14_OHMS    // RCD Output Drive Strength
  },
  {    // Config Index
    (LRDIMM << 0)      | // type,
    (RAW_CARD_A << 5)  | // slot0,
    (0 << 10)          | // revision,
    (DDR_4000 << 13)   | // frequency,
    (QCA_VREF << 18),    // Vref Type,
    // Position-based Data
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 0)  |  // P1,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 3)  |  // P2,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 6)  |  // P3,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 9)  |  // P4,
    (DDR5_RTT_CK_CS_CA_RTT_80_BITS << 12),  // P5,
    126,    // Percentage * 2,
    DDR5_RCD_OUTPUT_DRIVE_STRENGTH_14_OHMS    // RCD Output Drive Strength
  },
  {    // Config Index
    (LRDIMM << 0)      | // type,
    (RAW_CARD_B << 5)  | // slot0,
    (0 << 10)          | // revision,
    (DDR_4000 << 13)   | // frequency,
    (QCA_VREF << 18),    // Vref Type,
    // Position-based Data
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 0)  |  // P1,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 3)  |  // P2,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 6)  |  // P3,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 9)  |  // P4,
    (DDR5_RTT_CK_CS_CA_RTT_80_BITS << 12),  // P5,
    126,    // Percentage * 2,
    DDR5_RCD_OUTPUT_DRIVE_STRENGTH_14_OHMS    // RCD Output Drive Strength
  },
  {    // Config Index
    (LRDIMM << 0)      | // type,
    (RAW_CARD_A << 5)  | // slot0,
    (0 << 10)          | // revision,
    (DDR_4000 << 13)   | // frequency,
    (QCS_VREF << 18),    // Vref Type,
    // Position-based Data
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 0)  |  // P1,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 3)  |  // P2,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 6)  |  // P3,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 9)  |  // P4,
    (DDR5_RTT_CK_CS_CA_RTT_40_BITS << 12),  // P5,
    133,    // Percentage * 2,
    DDR5_RCD_OUTPUT_DRIVE_STRENGTH_20_OHMS    // RCD Output Drive Strength
  },
  {    // Config Index
    (LRDIMM << 0)      | // type,
    (RAW_CARD_B << 5)  | // slot0,
    (0 << 10)          | // revision,
    (DDR_4000 << 13)   | // frequency,
    (QCS_VREF << 18),    // Vref Type,
    // Position-based Data
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 0)  |  // P1,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 3)  |  // P2,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 6)  |  // P3,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 9)  |  // P4,
    (DDR5_RTT_CK_CS_CA_RTT_40_BITS << 12),  // P5,
    133,    // Percentage * 2,
    DDR5_RCD_OUTPUT_DRIVE_STRENGTH_20_OHMS    // RCD Output Drive Strength
  },
  {    // Config Index
    (LRDIMM << 0)      | // type,
    (RAW_CARD_A << 5)  | // slot0,
    (0 << 10)          | // revision,
    (DDR_4000 << 13)   | // frequency,
    (QCK_VREF << 18),    // Vref Type,
    // Position-based Data
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 0)  |  // P1,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 3)  |  // P2,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 6)  |  // P3,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 9)  |  // P4,
    (DDR5_RTT_CK_CS_CA_RTT_60_BITS << 12),  // P5,
    0,    // Percentage * 2,
    DDR5_RCD_OUTPUT_DRIVE_STRENGTH_14_OHMS    // RCD Output Drive Strength
  },
  {    // Config Index
    (LRDIMM << 0)      | // type,
    (RAW_CARD_B << 5)  | // slot0,
    (0 << 10)          | // revision,
    (DDR_4000 << 13)   | // frequency,
    (QCK_VREF << 18),    // Vref Type,
    // Position-based Data
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 0)  |  // P1,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 3)  |  // P2,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 6)  |  // P3,
    (DDR5_RTT_CK_CS_CA_RTT_OFF_BITS << 9)  |  // P4,
    (DDR5_RTT_CK_CS_CA_RTT_60_BITS << 12),  // P5,
    0,    // Percentage * 2,
    DDR5_RCD_OUTPUT_DRIVE_STRENGTH_14_OHMS    // RCD Output Drive Strength
  },
};

#endif //_DDR5_BACKSIDE_ODT_VALUE_TABLE_H_
