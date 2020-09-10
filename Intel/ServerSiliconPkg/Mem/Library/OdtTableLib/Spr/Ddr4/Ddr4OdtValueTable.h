/** @file
  Data structures for DDR4 ODT Value Table

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 - 2020 Intel Corporation. <BR>

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

/**
  ODT Parser Version is 0.91
  Input Source: icx_odt_value_ww20_19.xml 2019-08-13 16:49:06 
**/

#ifndef _DDR4_ODT_VALUE_TABLE_H_
#define _DDR4_ODT_VALUE_TABLE_H_

#include <Memory/JedecDefinitions.h>

struct ddrOdtValueTableEntry Ddr4OdtValueTable[] = {

  {
    // Config Index
    (DDR_1866 << 0)   |  // freq
    (SR_DIMM << 7)    |  // slot0
    (EMPTY_DIMM << 10)|  // slot1
    (EMPTY_DIMM << 13),  // slot2
    50,                  // MC ODT value
    30,                  // CPU RON value
    75,                  // MC Vref Percent
    {
      74,                // DRAM0 Vref percent
      na                 // DRAM1 Vref percent
    },
    {
      {
          // dimm0
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_48 << 3)  |  DDR4_RTT_NOM_48,                  // rank0
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS                  // rank1
      },
      {
          // dimm1
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS,                 // rank0
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS                  // rank1
      }
    }
  },
  {
    // Config Index
    (DDR_2133 << 0)   |  // freq
    (SR_DIMM << 7)    |  // slot0
    (EMPTY_DIMM << 10)|  // slot1
    (EMPTY_DIMM << 13),  // slot2
    50,                  // MC ODT value
    30,                  // CPU RON value
    75,                  // MC Vref Percent
    {
      74,                // DRAM0 Vref percent
      na                 // DRAM1 Vref percent
    },
    {
      {
          // dimm0
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_48 << 3)  |  DDR4_RTT_NOM_48,                  // rank0
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS                  // rank1
      },
      {
          // dimm1
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS,                 // rank0
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS                  // rank1
      }
    }
  },
  {
    // Config Index
    (DDR_2400 << 0)   |  // freq
    (SR_DIMM << 7)    |  // slot0
    (EMPTY_DIMM << 10)|  // slot1
    (EMPTY_DIMM << 13),  // slot2
    50,                  // MC ODT value
    30,                  // CPU RON value
    75,                  // MC Vref Percent
    {
      74,                // DRAM0 Vref percent
      na                 // DRAM1 Vref percent
    },
    {
      {
          // dimm0
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_48 << 3)  |  DDR4_RTT_NOM_48,                  // rank0
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS                  // rank1
      },
      {
          // dimm1
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS,                 // rank0
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS                  // rank1
      }
    }
  },
  {
    // Config Index
    (DDR_2666 << 0)   |  // freq
    (SR_DIMM << 7)    |  // slot0
    (EMPTY_DIMM << 10)|  // slot1
    (EMPTY_DIMM << 13),  // slot2
    50,                  // MC ODT value
    30,                  // CPU RON value
    75,                  // MC Vref Percent
    {
      74,                // DRAM0 Vref percent
      na                 // DRAM1 Vref percent
    },
    {
      {
          // dimm0
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_48 << 3)  |  DDR4_RTT_NOM_48,                  // rank0
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS                  // rank1
      },
      {
          // dimm1
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS,                 // rank0
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS                  // rank1
      }
    }
  },
  {
    // Config Index
    (DDR_2933 << 0)   |  // freq
    (SR_DIMM << 7)    |  // slot0
    (EMPTY_DIMM << 10)|  // slot1
    (EMPTY_DIMM << 13),  // slot2
    50,                  // MC ODT value
    30,                  // CPU RON value
    75,                  // MC Vref Percent
    {
      74,                // DRAM0 Vref percent
      na                 // DRAM1 Vref percent
    },
    {
      {
          // dimm0
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_48 << 3)  |  DDR4_RTT_NOM_48,                  // rank0
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS                  // rank1
      },
      {
          // dimm1
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS,                 // rank0
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS                  // rank1
      }
    }
  },
  {
    // Config Index
    (DDR_3200 << 0)   |  // freq
    (SR_DIMM << 7)    |  // slot0
    (EMPTY_DIMM << 10)|  // slot1
    (EMPTY_DIMM << 13),  // slot2
    50,                  // MC ODT value
    30,                  // CPU RON value
    75,                  // MC Vref Percent
    {
      74,                // DRAM0 Vref percent
      na                 // DRAM1 Vref percent
    },
    {
      {
          // dimm0
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_48 << 3)  |  DDR4_RTT_NOM_48,                  // rank0
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS                  // rank1
      },
      {
          // dimm1
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS,                 // rank0
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS                  // rank1
      }
    }
  },
  {
    // Config Index
    (DDR_1866 << 0)   |  // freq
    (LR_DIMM << 7)    |  // slot0
    (EMPTY_DIMM << 10)|  // slot1
    (EMPTY_DIMM << 13),  // slot2
    50,                  // MC ODT value
    30,                  // CPU RON value
    75,                  // MC Vref Percent
    {
      71,                // DRAM0 Vref percent
      na                 // DRAM1 Vref percent
    },
    {
      {
          // dimm0
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_60 << 6)    |  (DDR4_RTT_PARK_60 << 3)  |  DDR4_RTT_NOM_60,                  // rank0
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS                  // rank1
      },
      {
          // dimm1
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS,                 // rank0
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS                  // rank1
      }
    }
  },
  {
    // Config Index
    (DDR_2133 << 0)   |  // freq
    (LR_DIMM << 7)    |  // slot0
    (EMPTY_DIMM << 10)|  // slot1
    (EMPTY_DIMM << 13),  // slot2
    50,                  // MC ODT value
    30,                  // CPU RON value
    75,                  // MC Vref Percent
    {
      71,                // DRAM0 Vref percent
      na                 // DRAM1 Vref percent
    },
    {
      {
          // dimm0
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_60 << 6)    |  (DDR4_RTT_PARK_60 << 3)  |  DDR4_RTT_NOM_60,                  // rank0
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS                  // rank1
      },
      {
          // dimm1
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS,                 // rank0
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS                  // rank1
      }
    }
  },
  {
    // Config Index
    (DDR_2400 << 0)   |  // freq
    (LR_DIMM << 7)    |  // slot0
    (EMPTY_DIMM << 10)|  // slot1
    (EMPTY_DIMM << 13),  // slot2
    50,                  // MC ODT value
    30,                  // CPU RON value
    75,                  // MC Vref Percent
    {
      71,                // DRAM0 Vref percent
      na                 // DRAM1 Vref percent
    },
    {
      {
          // dimm0
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_60 << 6)    |  (DDR4_RTT_PARK_60 << 3)  |  DDR4_RTT_NOM_60,                  // rank0
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS                  // rank1
      },
      {
          // dimm1
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS,                 // rank0
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS                  // rank1
      }
    }
  },
  {
    // Config Index
    (DDR_2666 << 0)   |  // freq
    (LR_DIMM << 7)    |  // slot0
    (EMPTY_DIMM << 10)|  // slot1
    (EMPTY_DIMM << 13),  // slot2
    50,                  // MC ODT value
    30,                  // CPU RON value
    75,                  // MC Vref Percent
    {
      71,                // DRAM0 Vref percent
      na                 // DRAM1 Vref percent
    },
    {
      {
          // dimm0
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_60 << 6)    |  (DDR4_RTT_PARK_60 << 3)  |  DDR4_RTT_NOM_60,                  // rank0
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS                  // rank1
      },
      {
          // dimm1
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS,                 // rank0
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS                  // rank1
      }
    }
  },
  {
    // Config Index
    (DDR_2933 << 0)   |  // freq
    (LR_DIMM << 7)    |  // slot0
    (EMPTY_DIMM << 10)|  // slot1
    (EMPTY_DIMM << 13),  // slot2
    50,                  // MC ODT value
    30,                  // CPU RON value
    75,                  // MC Vref Percent
    {
      71,                // DRAM0 Vref percent
      na                 // DRAM1 Vref percent
    },
    {
      {
          // dimm0
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_60 << 6)    |  (DDR4_RTT_PARK_60 << 3)  |  DDR4_RTT_NOM_60,                  // rank0
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS                  // rank1
      },
      {
          // dimm1
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS,                 // rank0
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS                  // rank1
      }
    }
  },
  {
    // Config Index
    (DDR_3200 << 0)   |  // freq
    (LR_DIMM << 7)    |  // slot0
    (EMPTY_DIMM << 10)|  // slot1
    (EMPTY_DIMM << 13),  // slot2
    50,                  // MC ODT value
    30,                  // CPU RON value
    75,                  // MC Vref Percent
    {
      71,                // DRAM0 Vref percent
      na                 // DRAM1 Vref percent
    },
    {
      {
          // dimm0
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_60 << 6)    |  (DDR4_RTT_PARK_60 << 3)  |  DDR4_RTT_NOM_60,                  // rank0
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS                  // rank1
      },
      {
          // dimm1
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS,                 // rank0
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS                  // rank1
      }
    }
  },
  {
    // Config Index
    (DDR_1866 << 0)   |  // freq
    (DR_DIMM << 7)    |  // slot0
    (EMPTY_DIMM << 10)|  // slot1
    (EMPTY_DIMM << 13),  // slot2
    50,                  // MC ODT value
    30,                  // CPU RON value
    77,                  // MC Vref Percent
    {
      78,                // DRAM0 Vref percent
      na                 // DRAM1 Vref percent
    },
    {
      {
          // dimm0
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_240 << 6)   |  (DDR4_RTT_PARK_40 << 3)  |  DDR4_RTT_NOM_240,                 // rank0
          (DDR4_RTT_WR_240 << 6)   |  (DDR4_RTT_PARK_40 << 3)  |  DDR4_RTT_NOM_240                  // rank1
      },
      {
          // dimm1
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS,                 // rank0
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS                  // rank1
      }
    }
  },
  {
    // Config Index
    (DDR_2133 << 0)   |  // freq
    (DR_DIMM << 7)    |  // slot0
    (EMPTY_DIMM << 10)|  // slot1
    (EMPTY_DIMM << 13),  // slot2
    50,                  // MC ODT value
    30,                  // CPU RON value
    77,                  // MC Vref Percent
    {
      78,                // DRAM0 Vref percent
      na                 // DRAM1 Vref percent
    },
    {
      {
          // dimm0
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_240 << 6)   |  (DDR4_RTT_PARK_40 << 3)  |  DDR4_RTT_NOM_240,                 // rank0
          (DDR4_RTT_WR_240 << 6)   |  (DDR4_RTT_PARK_40 << 3)  |  DDR4_RTT_NOM_240                  // rank1
      },
      {
          // dimm1
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS,                 // rank0
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS                  // rank1
      }
    }
  },
  {
    // Config Index
    (DDR_2400 << 0)   |  // freq
    (DR_DIMM << 7)    |  // slot0
    (EMPTY_DIMM << 10)|  // slot1
    (EMPTY_DIMM << 13),  // slot2
    50,                  // MC ODT value
    30,                  // CPU RON value
    77,                  // MC Vref Percent
    {
      78,                // DRAM0 Vref percent
      na                 // DRAM1 Vref percent
    },
    {
      {
          // dimm0
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_240 << 6)   |  (DDR4_RTT_PARK_40 << 3)  |  DDR4_RTT_NOM_240,                 // rank0
          (DDR4_RTT_WR_240 << 6)   |  (DDR4_RTT_PARK_40 << 3)  |  DDR4_RTT_NOM_240                  // rank1
      },
      {
          // dimm1
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS,                 // rank0
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS                  // rank1
      }
    }
  },
  {
    // Config Index
    (DDR_2666 << 0)   |  // freq
    (DR_DIMM << 7)    |  // slot0
    (EMPTY_DIMM << 10)|  // slot1
    (EMPTY_DIMM << 13),  // slot2
    50,                  // MC ODT value
    30,                  // CPU RON value
    77,                  // MC Vref Percent
    {
      78,                // DRAM0 Vref percent
      na                 // DRAM1 Vref percent
    },
    {
      {
          // dimm0
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_240 << 6)   |  (DDR4_RTT_PARK_40 << 3)  |  DDR4_RTT_NOM_240,                 // rank0
          (DDR4_RTT_WR_240 << 6)   |  (DDR4_RTT_PARK_40 << 3)  |  DDR4_RTT_NOM_240                  // rank1
      },
      {
          // dimm1
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS,                 // rank0
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS                  // rank1
      }
    }
  },
  {
    // Config Index
    (DDR_2933 << 0)   |  // freq
    (DR_DIMM << 7)    |  // slot0
    (EMPTY_DIMM << 10)|  // slot1
    (EMPTY_DIMM << 13),  // slot2
    50,                  // MC ODT value
    30,                  // CPU RON value
    77,                  // MC Vref Percent
    {
      78,                // DRAM0 Vref percent
      na                 // DRAM1 Vref percent
    },
    {
      {
          // dimm0
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_240 << 6)   |  (DDR4_RTT_PARK_40 << 3)  |  DDR4_RTT_NOM_240,                 // rank0
          (DDR4_RTT_WR_240 << 6)   |  (DDR4_RTT_PARK_40 << 3)  |  DDR4_RTT_NOM_240                  // rank1
      },
      {
          // dimm1
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS,                 // rank0
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS                  // rank1
      }
    }
  },
  {
    // Config Index
    (DDR_3200 << 0)   |  // freq
    (DR_DIMM << 7)    |  // slot0
    (EMPTY_DIMM << 10)|  // slot1
    (EMPTY_DIMM << 13),  // slot2
    50,                  // MC ODT value
    30,                  // CPU RON value
    77,                  // MC Vref Percent
    {
      78,                // DRAM0 Vref percent
      na                 // DRAM1 Vref percent
    },
    {
      {
          // dimm0
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_240 << 6)   |  (DDR4_RTT_PARK_40 << 3)  |  DDR4_RTT_NOM_240,                 // rank0
          (DDR4_RTT_WR_240 << 6)   |  (DDR4_RTT_PARK_40 << 3)  |  DDR4_RTT_NOM_240                  // rank1
      },
      {
          // dimm1
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS,                 // rank0
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS                  // rank1
      }
    }
  },
  {
    // Config Index
    (DDR_1866 << 0)   |  // freq
    (SR_DIMM << 7)    |  // slot0
    (SR_DIMM << 10)   |  // slot1
    (EMPTY_DIMM << 13),  // slot2
    50,                  // MC ODT value
    30,                  // CPU RON value
    83,                  // MC Vref Percent
    {
      77,                // DRAM0 Vref percent
      77                 // DRAM1 Vref percent
    },
    {
      {
          // dimm0
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_80 << 6)    |  (DDR4_RTT_PARK_48 << 3)  |  DDR4_RTT_NOM_34,                  // rank0
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS                  // rank1
      },
      {
          // dimm1
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_80 << 6)    |  (DDR4_RTT_PARK_48 << 3)  |  DDR4_RTT_NOM_34,                  // rank0
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS                  // rank1
      }
    }
  },
  {
    // Config Index
    (DDR_2133 << 0)   |  // freq
    (SR_DIMM << 7)    |  // slot0
    (SR_DIMM << 10)   |  // slot1
    (EMPTY_DIMM << 13),  // slot2
    50,                  // MC ODT value
    30,                  // CPU RON value
    83,                  // MC Vref Percent
    {
      77,                // DRAM0 Vref percent
      77                 // DRAM1 Vref percent
    },
    {
      {
          // dimm0
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_80 << 6)    |  (DDR4_RTT_PARK_48 << 3)  |  DDR4_RTT_NOM_34,                  // rank0
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS                  // rank1
      },
      {
          // dimm1
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_80 << 6)    |  (DDR4_RTT_PARK_48 << 3)  |  DDR4_RTT_NOM_34,                  // rank0
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS                  // rank1
      }
    }
  },
  {
    // Config Index
    (DDR_2400 << 0)   |  // freq
    (SR_DIMM << 7)    |  // slot0
    (SR_DIMM << 10)   |  // slot1
    (EMPTY_DIMM << 13),  // slot2
    50,                  // MC ODT value
    30,                  // CPU RON value
    83,                  // MC Vref Percent
    {
      77,                // DRAM0 Vref percent
      77                 // DRAM1 Vref percent
    },
    {
      {
          // dimm0
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_80 << 6)    |  (DDR4_RTT_PARK_48 << 3)  |  DDR4_RTT_NOM_34,                  // rank0
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS                  // rank1
      },
      {
          // dimm1
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_80 << 6)    |  (DDR4_RTT_PARK_48 << 3)  |  DDR4_RTT_NOM_34,                  // rank0
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS                  // rank1
      }
    }
  },
  {
    // Config Index
    (DDR_2666 << 0)   |  // freq
    (SR_DIMM << 7)    |  // slot0
    (SR_DIMM << 10)   |  // slot1
    (EMPTY_DIMM << 13),  // slot2
    50,                  // MC ODT value
    30,                  // CPU RON value
    83,                  // MC Vref Percent
    {
      77,                // DRAM0 Vref percent
      77                 // DRAM1 Vref percent
    },
    {
      {
          // dimm0
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_80 << 6)    |  (DDR4_RTT_PARK_48 << 3)  |  DDR4_RTT_NOM_34,                  // rank0
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS                  // rank1
      },
      {
          // dimm1
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_80 << 6)    |  (DDR4_RTT_PARK_48 << 3)  |  DDR4_RTT_NOM_34,                  // rank0
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS                  // rank1
      }
    }
  },
  {
    // Config Index
    (DDR_2933 << 0)   |  // freq
    (SR_DIMM << 7)    |  // slot0
    (SR_DIMM << 10)   |  // slot1
    (EMPTY_DIMM << 13),  // slot2
    50,                  // MC ODT value
    30,                  // CPU RON value
    83,                  // MC Vref Percent
    {
      77,                // DRAM0 Vref percent
      77                 // DRAM1 Vref percent
    },
    {
      {
          // dimm0
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_80 << 6)    |  (DDR4_RTT_PARK_48 << 3)  |  DDR4_RTT_NOM_34,                  // rank0
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS                  // rank1
      },
      {
          // dimm1
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_80 << 6)    |  (DDR4_RTT_PARK_48 << 3)  |  DDR4_RTT_NOM_34,                  // rank0
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS                  // rank1
      }
    }
  },
  {
    // Config Index
    (DDR_3200 << 0)   |  // freq
    (SR_DIMM << 7)    |  // slot0
    (SR_DIMM << 10)   |  // slot1
    (EMPTY_DIMM << 13),  // slot2
    50,                  // MC ODT value
    30,                  // CPU RON value
    83,                  // MC Vref Percent
    {
      77,                // DRAM0 Vref percent
      77                 // DRAM1 Vref percent
    },
    {
      {
          // dimm0
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_80 << 6)    |  (DDR4_RTT_PARK_48 << 3)  |  DDR4_RTT_NOM_34,                  // rank0
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS                  // rank1
      },
      {
          // dimm1
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_80 << 6)    |  (DDR4_RTT_PARK_48 << 3)  |  DDR4_RTT_NOM_34,                  // rank0
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS                  // rank1
      }
    }
  },
  {
    // Config Index
    (DDR_1866 << 0)   |  // freq
    (DR_DIMM << 7)    |  // slot0
    (DR_DIMM << 10)   |  // slot1
    (EMPTY_DIMM << 13),  // slot2
    50,                  // MC ODT value
    30,                  // CPU RON value
    85,                  // MC Vref Percent
    {
      82,                // DRAM0 Vref percent
      82                 // DRAM1 Vref percent
    },
    {
      {
          // dimm0
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_240 << 6)   |  (DDR4_RTT_PARK_60 << 3)  |  DDR4_RTT_NOM_240,                 // rank0
          (DDR4_RTT_WR_240 << 6)   |  (DDR4_RTT_PARK_60 << 3)  |  DDR4_RTT_NOM_240                  // rank1
      },
      {
          // dimm1
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_240 << 6)   |  (DDR4_RTT_PARK_60 << 3)  |  DDR4_RTT_NOM_240,                 // rank0
          (DDR4_RTT_WR_240 << 6)   |  (DDR4_RTT_PARK_60 << 3)  |  DDR4_RTT_NOM_240                  // rank1
      }
    }
  },
  {
    // Config Index
    (DDR_2133 << 0)   |  // freq
    (DR_DIMM << 7)    |  // slot0
    (DR_DIMM << 10)   |  // slot1
    (EMPTY_DIMM << 13),  // slot2
    50,                  // MC ODT value
    30,                  // CPU RON value
    85,                  // MC Vref Percent
    {
      82,                // DRAM0 Vref percent
      82                 // DRAM1 Vref percent
    },
    {
      {
          // dimm0
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_240 << 6)   |  (DDR4_RTT_PARK_60 << 3)  |  DDR4_RTT_NOM_240,                 // rank0
          (DDR4_RTT_WR_240 << 6)   |  (DDR4_RTT_PARK_60 << 3)  |  DDR4_RTT_NOM_240                  // rank1
      },
      {
          // dimm1
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_240 << 6)   |  (DDR4_RTT_PARK_60 << 3)  |  DDR4_RTT_NOM_240,                 // rank0
          (DDR4_RTT_WR_240 << 6)   |  (DDR4_RTT_PARK_60 << 3)  |  DDR4_RTT_NOM_240                  // rank1
      }
    }
  },
  {
    // Config Index
    (DDR_2400 << 0)   |  // freq
    (DR_DIMM << 7)    |  // slot0
    (DR_DIMM << 10)   |  // slot1
    (EMPTY_DIMM << 13),  // slot2
    50,                  // MC ODT value
    30,                  // CPU RON value
    85,                  // MC Vref Percent
    {
      82,                // DRAM0 Vref percent
      82                 // DRAM1 Vref percent
    },
    {
      {
          // dimm0
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_240 << 6)   |  (DDR4_RTT_PARK_60 << 3)  |  DDR4_RTT_NOM_240,                 // rank0
          (DDR4_RTT_WR_240 << 6)   |  (DDR4_RTT_PARK_60 << 3)  |  DDR4_RTT_NOM_240                  // rank1
      },
      {
          // dimm1
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_240 << 6)   |  (DDR4_RTT_PARK_60 << 3)  |  DDR4_RTT_NOM_240,                 // rank0
          (DDR4_RTT_WR_240 << 6)   |  (DDR4_RTT_PARK_60 << 3)  |  DDR4_RTT_NOM_240                  // rank1
      }
    }
  },
  {
    // Config Index
    (DDR_2666 << 0)   |  // freq
    (DR_DIMM << 7)    |  // slot0
    (DR_DIMM << 10)   |  // slot1
    (EMPTY_DIMM << 13),  // slot2
    50,                  // MC ODT value
    30,                  // CPU RON value
    85,                  // MC Vref Percent
    {
      82,                // DRAM0 Vref percent
      82                 // DRAM1 Vref percent
    },
    {
      {
          // dimm0
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_240 << 6)   |  (DDR4_RTT_PARK_60 << 3)  |  DDR4_RTT_NOM_240,                 // rank0
          (DDR4_RTT_WR_240 << 6)   |  (DDR4_RTT_PARK_60 << 3)  |  DDR4_RTT_NOM_240                  // rank1
      },
      {
          // dimm1
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_240 << 6)   |  (DDR4_RTT_PARK_60 << 3)  |  DDR4_RTT_NOM_240,                 // rank0
          (DDR4_RTT_WR_240 << 6)   |  (DDR4_RTT_PARK_60 << 3)  |  DDR4_RTT_NOM_240                  // rank1
      }
    }
  },
  {
    // Config Index
    (DDR_2933 << 0)   |  // freq
    (DR_DIMM << 7)    |  // slot0
    (DR_DIMM << 10)   |  // slot1
    (EMPTY_DIMM << 13),  // slot2
    50,                  // MC ODT value
    30,                  // CPU RON value
    85,                  // MC Vref Percent
    {
      82,                // DRAM0 Vref percent
      82                 // DRAM1 Vref percent
    },
    {
      {
          // dimm0
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_240 << 6)   |  (DDR4_RTT_PARK_60 << 3)  |  DDR4_RTT_NOM_240,                 // rank0
          (DDR4_RTT_WR_240 << 6)   |  (DDR4_RTT_PARK_60 << 3)  |  DDR4_RTT_NOM_240                  // rank1
      },
      {
          // dimm1
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_240 << 6)   |  (DDR4_RTT_PARK_60 << 3)  |  DDR4_RTT_NOM_240,                 // rank0
          (DDR4_RTT_WR_240 << 6)   |  (DDR4_RTT_PARK_60 << 3)  |  DDR4_RTT_NOM_240                  // rank1
      }
    }
  },
  {
    // Config Index
    (DDR_3200 << 0)   |  // freq
    (DR_DIMM << 7)    |  // slot0
    (DR_DIMM << 10)   |  // slot1
    (EMPTY_DIMM << 13),  // slot2
    50,                  // MC ODT value
    30,                  // CPU RON value
    85,                  // MC Vref Percent
    {
      82,                // DRAM0 Vref percent
      82                 // DRAM1 Vref percent
    },
    {
      {
          // dimm0
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_240 << 6)   |  (DDR4_RTT_PARK_60 << 3)  |  DDR4_RTT_NOM_240,                 // rank0
          (DDR4_RTT_WR_240 << 6)   |  (DDR4_RTT_PARK_60 << 3)  |  DDR4_RTT_NOM_240                  // rank1
      },
      {
          // dimm1
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_240 << 6)   |  (DDR4_RTT_PARK_60 << 3)  |  DDR4_RTT_NOM_240,                 // rank0
          (DDR4_RTT_WR_240 << 6)   |  (DDR4_RTT_PARK_60 << 3)  |  DDR4_RTT_NOM_240                  // rank1
      }
    }
  },
  {
    // Config Index
    (DDR_1866 << 0)   |  // freq
    (DR_DIMM << 7)    |  // slot0
    (SR_DIMM << 10)   |  // slot1
    (EMPTY_DIMM << 13),  // slot2
    50,                  // MC ODT value
    30,                  // CPU RON value
    84,                  // MC Vref Percent
    {
      82,                // DRAM0 Vref percent
      76                 // DRAM1 Vref percent
    },
    {
      {
          // dimm0
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_240 << 6)   |  (DDR4_RTT_PARK_60 << 3)  |  DDR4_RTT_NOM_240,                 // rank0
          (DDR4_RTT_WR_240 << 6)   |  (DDR4_RTT_PARK_60 << 3)  |  DDR4_RTT_NOM_240                  // rank1
      },
      {
          // dimm1
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_120 << 6)   |  (DDR4_RTT_PARK_34 << 3)  |  DDR4_RTT_NOM_34,                  // rank0
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS                  // rank1
      }
    }
  },
  {
    // Config Index
    (DDR_2133 << 0)   |  // freq
    (DR_DIMM << 7)    |  // slot0
    (SR_DIMM << 10)   |  // slot1
    (EMPTY_DIMM << 13),  // slot2
    50,                  // MC ODT value
    30,                  // CPU RON value
    84,                  // MC Vref Percent
    {
      82,                // DRAM0 Vref percent
      76                 // DRAM1 Vref percent
    },
    {
      {
          // dimm0
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_240 << 6)   |  (DDR4_RTT_PARK_60 << 3)  |  DDR4_RTT_NOM_240,                 // rank0
          (DDR4_RTT_WR_240 << 6)   |  (DDR4_RTT_PARK_60 << 3)  |  DDR4_RTT_NOM_240                  // rank1
      },
      {
          // dimm1
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_120 << 6)   |  (DDR4_RTT_PARK_34 << 3)  |  DDR4_RTT_NOM_34,                  // rank0
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS                  // rank1
      }
    }
  },
  {
    // Config Index
    (DDR_2400 << 0)   |  // freq
    (DR_DIMM << 7)    |  // slot0
    (SR_DIMM << 10)   |  // slot1
    (EMPTY_DIMM << 13),  // slot2
    50,                  // MC ODT value
    30,                  // CPU RON value
    84,                  // MC Vref Percent
    {
      82,                // DRAM0 Vref percent
      76                 // DRAM1 Vref percent
    },
    {
      {
          // dimm0
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_240 << 6)   |  (DDR4_RTT_PARK_60 << 3)  |  DDR4_RTT_NOM_240,                 // rank0
          (DDR4_RTT_WR_240 << 6)   |  (DDR4_RTT_PARK_60 << 3)  |  DDR4_RTT_NOM_240                  // rank1
      },
      {
          // dimm1
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_120 << 6)   |  (DDR4_RTT_PARK_34 << 3)  |  DDR4_RTT_NOM_34,                  // rank0
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS                  // rank1
      }
    }
  },
  {
    // Config Index
    (DDR_2666 << 0)   |  // freq
    (DR_DIMM << 7)    |  // slot0
    (SR_DIMM << 10)   |  // slot1
    (EMPTY_DIMM << 13),  // slot2
    50,                  // MC ODT value
    30,                  // CPU RON value
    84,                  // MC Vref Percent
    {
      82,                // DRAM0 Vref percent
      76                 // DRAM1 Vref percent
    },
    {
      {
          // dimm0
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_240 << 6)   |  (DDR4_RTT_PARK_60 << 3)  |  DDR4_RTT_NOM_240,                 // rank0
          (DDR4_RTT_WR_240 << 6)   |  (DDR4_RTT_PARK_60 << 3)  |  DDR4_RTT_NOM_240                  // rank1
      },
      {
          // dimm1
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_120 << 6)   |  (DDR4_RTT_PARK_34 << 3)  |  DDR4_RTT_NOM_34,                  // rank0
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS                  // rank1
      }
    }
  },
  {
    // Config Index
    (DDR_2933 << 0)   |  // freq
    (DR_DIMM << 7)    |  // slot0
    (SR_DIMM << 10)   |  // slot1
    (EMPTY_DIMM << 13),  // slot2
    50,                  // MC ODT value
    30,                  // CPU RON value
    84,                  // MC Vref Percent
    {
      82,                // DRAM0 Vref percent
      76                 // DRAM1 Vref percent
    },
    {
      {
          // dimm0
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_240 << 6)   |  (DDR4_RTT_PARK_60 << 3)  |  DDR4_RTT_NOM_240,                 // rank0
          (DDR4_RTT_WR_240 << 6)   |  (DDR4_RTT_PARK_60 << 3)  |  DDR4_RTT_NOM_240                  // rank1
      },
      {
          // dimm1
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_120 << 6)   |  (DDR4_RTT_PARK_34 << 3)  |  DDR4_RTT_NOM_34,                  // rank0
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS                  // rank1
      }
    }
  },
  {
    // Config Index
    (DDR_3200 << 0)   |  // freq
    (DR_DIMM << 7)    |  // slot0
    (SR_DIMM << 10)   |  // slot1
    (EMPTY_DIMM << 13),  // slot2
    50,                  // MC ODT value
    30,                  // CPU RON value
    84,                  // MC Vref Percent
    {
      82,                // DRAM0 Vref percent
      76                 // DRAM1 Vref percent
    },
    {
      {
          // dimm0
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_240 << 6)   |  (DDR4_RTT_PARK_60 << 3)  |  DDR4_RTT_NOM_240,                 // rank0
          (DDR4_RTT_WR_240 << 6)   |  (DDR4_RTT_PARK_60 << 3)  |  DDR4_RTT_NOM_240                  // rank1
      },
      {
          // dimm1
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_120 << 6)   |  (DDR4_RTT_PARK_34 << 3)  |  DDR4_RTT_NOM_34,                  // rank0
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS                  // rank1
      }
    }
  },
  {
    // Config Index
    (DDR_1866 << 0)   |  // freq
    (SR_DIMM << 7)    |  // slot0
    (DR_DIMM << 10)   |  // slot1
    (EMPTY_DIMM << 13),  // slot2
    50,                  // MC ODT value
    30,                  // CPU RON value
    84,                  // MC Vref Percent
    {
      76,                // DRAM0 Vref percent
      82                 // DRAM1 Vref percent
    },
    {
      {
          // dimm0
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_120 << 6)   |  (DDR4_RTT_PARK_34 << 3)  |  DDR4_RTT_NOM_34,                  // rank0
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS                  // rank1
      },
      {
          // dimm1
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_240 << 6)   |  (DDR4_RTT_PARK_60 << 3)  |  DDR4_RTT_NOM_240,                 // rank0
          (DDR4_RTT_WR_240 << 6)   |  (DDR4_RTT_PARK_60 << 3)  |  DDR4_RTT_NOM_240                  // rank1
      }
    }
  },
  {
    // Config Index
    (DDR_2133 << 0)   |  // freq
    (SR_DIMM << 7)    |  // slot0
    (DR_DIMM << 10)   |  // slot1
    (EMPTY_DIMM << 13),  // slot2
    50,                  // MC ODT value
    30,                  // CPU RON value
    84,                  // MC Vref Percent
    {
      76,                // DRAM0 Vref percent
      82                 // DRAM1 Vref percent
    },
    {
      {
          // dimm0
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_120 << 6)   |  (DDR4_RTT_PARK_34 << 3)  |  DDR4_RTT_NOM_34,                  // rank0
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS                  // rank1
      },
      {
          // dimm1
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_240 << 6)   |  (DDR4_RTT_PARK_60 << 3)  |  DDR4_RTT_NOM_240,                 // rank0
          (DDR4_RTT_WR_240 << 6)   |  (DDR4_RTT_PARK_60 << 3)  |  DDR4_RTT_NOM_240                  // rank1
      }
    }
  },
  {
    // Config Index
    (DDR_2400 << 0)   |  // freq
    (SR_DIMM << 7)    |  // slot0
    (DR_DIMM << 10)   |  // slot1
    (EMPTY_DIMM << 13),  // slot2
    50,                  // MC ODT value
    30,                  // CPU RON value
    84,                  // MC Vref Percent
    {
      76,                // DRAM0 Vref percent
      82                 // DRAM1 Vref percent
    },
    {
      {
          // dimm0
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_120 << 6)   |  (DDR4_RTT_PARK_34 << 3)  |  DDR4_RTT_NOM_34,                  // rank0
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS                  // rank1
      },
      {
          // dimm1
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_240 << 6)   |  (DDR4_RTT_PARK_60 << 3)  |  DDR4_RTT_NOM_240,                 // rank0
          (DDR4_RTT_WR_240 << 6)   |  (DDR4_RTT_PARK_60 << 3)  |  DDR4_RTT_NOM_240                  // rank1
      }
    }
  },
  {
    // Config Index
    (DDR_2666 << 0)   |  // freq
    (SR_DIMM << 7)    |  // slot0
    (DR_DIMM << 10)   |  // slot1
    (EMPTY_DIMM << 13),  // slot2
    50,                  // MC ODT value
    30,                  // CPU RON value
    84,                  // MC Vref Percent
    {
      76,                // DRAM0 Vref percent
      82                 // DRAM1 Vref percent
    },
    {
      {
          // dimm0
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_120 << 6)   |  (DDR4_RTT_PARK_34 << 3)  |  DDR4_RTT_NOM_34,                  // rank0
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS                  // rank1
      },
      {
          // dimm1
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_240 << 6)   |  (DDR4_RTT_PARK_60 << 3)  |  DDR4_RTT_NOM_240,                 // rank0
          (DDR4_RTT_WR_240 << 6)   |  (DDR4_RTT_PARK_60 << 3)  |  DDR4_RTT_NOM_240                  // rank1
      }
    }
  },
  {
    // Config Index
    (DDR_2933 << 0)   |  // freq
    (SR_DIMM << 7)    |  // slot0
    (DR_DIMM << 10)   |  // slot1
    (EMPTY_DIMM << 13),  // slot2
    50,                  // MC ODT value
    30,                  // CPU RON value
    84,                  // MC Vref Percent
    {
      76,                // DRAM0 Vref percent
      82                 // DRAM1 Vref percent
    },
    {
      {
          // dimm0
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_120 << 6)   |  (DDR4_RTT_PARK_34 << 3)  |  DDR4_RTT_NOM_34,                  // rank0
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS                  // rank1
      },
      {
          // dimm1
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_240 << 6)   |  (DDR4_RTT_PARK_60 << 3)  |  DDR4_RTT_NOM_240,                 // rank0
          (DDR4_RTT_WR_240 << 6)   |  (DDR4_RTT_PARK_60 << 3)  |  DDR4_RTT_NOM_240                  // rank1
      }
    }
  },
  {
    // Config Index
    (DDR_3200 << 0)   |  // freq
    (SR_DIMM << 7)    |  // slot0
    (DR_DIMM << 10)   |  // slot1
    (EMPTY_DIMM << 13),  // slot2
    50,                  // MC ODT value
    30,                  // CPU RON value
    84,                  // MC Vref Percent
    {
      76,                // DRAM0 Vref percent
      82                 // DRAM1 Vref percent
    },
    {
      {
          // dimm0
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_120 << 6)   |  (DDR4_RTT_PARK_34 << 3)  |  DDR4_RTT_NOM_34,                  // rank0
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS                  // rank1
      },
      {
          // dimm1
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_240 << 6)   |  (DDR4_RTT_PARK_60 << 3)  |  DDR4_RTT_NOM_240,                 // rank0
          (DDR4_RTT_WR_240 << 6)   |  (DDR4_RTT_PARK_60 << 3)  |  DDR4_RTT_NOM_240                  // rank1
      }
    }
  },
  {
    // Config Index
    (DDR_1866 << 0)   |  // freq
    (LR_DIMM << 7)    |  // slot0
    (LR_DIMM << 10)   |  // slot1
    (EMPTY_DIMM << 13),  // slot2
    50,                  // MC ODT value
    30,                  // CPU RON value
    83,                  // MC Vref Percent
    {
      78,                // DRAM0 Vref percent
      78                 // DRAM1 Vref percent
    },
    {
      {
          // dimm0
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_80 << 6)    |  (DDR4_RTT_PARK_34 << 3)  |  DDR4_RTT_NOM_DIS,                 // rank0
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS                  // rank1
      },
      {
          // dimm1
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_80 << 6)    |  (DDR4_RTT_PARK_34 << 3)  |  DDR4_RTT_NOM_DIS,                 // rank0
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS                  // rank1
      }
    }
  },
  {
    // Config Index
    (DDR_2133 << 0)   |  // freq
    (LR_DIMM << 7)    |  // slot0
    (LR_DIMM << 10)   |  // slot1
    (EMPTY_DIMM << 13),  // slot2
    50,                  // MC ODT value
    30,                  // CPU RON value
    83,                  // MC Vref Percent
    {
      78,                // DRAM0 Vref percent
      78                 // DRAM1 Vref percent
    },
    {
      {
          // dimm0
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_80 << 6)    |  (DDR4_RTT_PARK_34 << 3)  |  DDR4_RTT_NOM_DIS,                 // rank0
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS                  // rank1
      },
      {
          // dimm1
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_80 << 6)    |  (DDR4_RTT_PARK_34 << 3)  |  DDR4_RTT_NOM_DIS,                 // rank0
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS                  // rank1
      }
    }
  },
  {
    // Config Index
    (DDR_2400 << 0)   |  // freq
    (LR_DIMM << 7)    |  // slot0
    (LR_DIMM << 10)   |  // slot1
    (EMPTY_DIMM << 13),  // slot2
    50,                  // MC ODT value
    30,                  // CPU RON value
    83,                  // MC Vref Percent
    {
      78,                // DRAM0 Vref percent
      78                 // DRAM1 Vref percent
    },
    {
      {
          // dimm0
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_80 << 6)    |  (DDR4_RTT_PARK_34 << 3)  |  DDR4_RTT_NOM_DIS,                 // rank0
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS                  // rank1
      },
      {
          // dimm1
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_80 << 6)    |  (DDR4_RTT_PARK_34 << 3)  |  DDR4_RTT_NOM_DIS,                 // rank0
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS                  // rank1
      }
    }
  },
  {
    // Config Index
    (DDR_2666 << 0)   |  // freq
    (LR_DIMM << 7)    |  // slot0
    (LR_DIMM << 10)   |  // slot1
    (EMPTY_DIMM << 13),  // slot2
    50,                  // MC ODT value
    30,                  // CPU RON value
    83,                  // MC Vref Percent
    {
      78,                // DRAM0 Vref percent
      78                 // DRAM1 Vref percent
    },
    {
      {
          // dimm0
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_80 << 6)    |  (DDR4_RTT_PARK_34 << 3)  |  DDR4_RTT_NOM_DIS,                 // rank0
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS                  // rank1
      },
      {
          // dimm1
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_80 << 6)    |  (DDR4_RTT_PARK_34 << 3)  |  DDR4_RTT_NOM_DIS,                 // rank0
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS                  // rank1
      }
    }
  },
  {
    // Config Index
    (DDR_2933 << 0)   |  // freq
    (LR_DIMM << 7)    |  // slot0
    (LR_DIMM << 10)   |  // slot1
    (EMPTY_DIMM << 13),  // slot2
    50,                  // MC ODT value
    30,                  // CPU RON value
    83,                  // MC Vref Percent
    {
      78,                // DRAM0 Vref percent
      78                 // DRAM1 Vref percent
    },
    {
      {
          // dimm0
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_80 << 6)    |  (DDR4_RTT_PARK_34 << 3)  |  DDR4_RTT_NOM_DIS,                 // rank0
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS                  // rank1
      },
      {
          // dimm1
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_80 << 6)    |  (DDR4_RTT_PARK_34 << 3)  |  DDR4_RTT_NOM_DIS,                 // rank0
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS                  // rank1
      }
    }
  },
  {
    // Config Index
    (DDR_3200 << 0)   |  // freq
    (LR_DIMM << 7)    |  // slot0
    (LR_DIMM << 10)   |  // slot1
    (EMPTY_DIMM << 13),  // slot2
    50,                  // MC ODT value
    30,                  // CPU RON value
    83,                  // MC Vref Percent
    {
      78,                // DRAM0 Vref percent
      78                 // DRAM1 Vref percent
    },
    {
      {
          // dimm0
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_80 << 6)    |  (DDR4_RTT_PARK_34 << 3)  |  DDR4_RTT_NOM_DIS,                 // rank0
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS                  // rank1
      },
      {
          // dimm1
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_80 << 6)    |  (DDR4_RTT_PARK_34 << 3)  |  DDR4_RTT_NOM_DIS,                 // rank0
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS                  // rank1
      }
    }
  },
  {
    // Config Index
    (DDR_1866 << 0)   |  // freq
    (DR_DIMM << 7)    |  // slot0
    (LR_DIMM << 10)   |  // slot1
    (EMPTY_DIMM << 13),  // slot2
    50,                  // MC ODT value
    30,                  // CPU RON value
    84,                  // MC Vref Percent
    {
      82,                // DRAM0 Vref percent
      81                 // DRAM1 Vref percent
    },
    {
      {
          // dimm0
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_240 << 6)   |  (DDR4_RTT_PARK_60 << 3)  |  DDR4_RTT_NOM_240,                 // rank0
          (DDR4_RTT_WR_240 << 6)   |  (DDR4_RTT_PARK_60 << 3)  |  DDR4_RTT_NOM_240                  // rank1
      },
      {
          // dimm1
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_60 << 6)    |  (DDR4_RTT_PARK_34 << 3)  |  DDR4_RTT_NOM_DIS,                 // rank0
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS                  // rank1
      }
    }
  },
  {
    // Config Index
    (DDR_2133 << 0)   |  // freq
    (DR_DIMM << 7)    |  // slot0
    (LR_DIMM << 10)   |  // slot1
    (EMPTY_DIMM << 13),  // slot2
    50,                  // MC ODT value
    30,                  // CPU RON value
    84,                  // MC Vref Percent
    {
      82,                // DRAM0 Vref percent
      81                 // DRAM1 Vref percent
    },
    {
      {
          // dimm0
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_240 << 6)   |  (DDR4_RTT_PARK_60 << 3)  |  DDR4_RTT_NOM_240,                 // rank0
          (DDR4_RTT_WR_240 << 6)   |  (DDR4_RTT_PARK_60 << 3)  |  DDR4_RTT_NOM_240                  // rank1
      },
      {
          // dimm1
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_60 << 6)    |  (DDR4_RTT_PARK_34 << 3)  |  DDR4_RTT_NOM_DIS,                 // rank0
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS                  // rank1
      }
    }
  },
  {
    // Config Index
    (DDR_2400 << 0)   |  // freq
    (DR_DIMM << 7)    |  // slot0
    (LR_DIMM << 10)   |  // slot1
    (EMPTY_DIMM << 13),  // slot2
    50,                  // MC ODT value
    30,                  // CPU RON value
    84,                  // MC Vref Percent
    {
      82,                // DRAM0 Vref percent
      81                 // DRAM1 Vref percent
    },
    {
      {
          // dimm0
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_240 << 6)   |  (DDR4_RTT_PARK_60 << 3)  |  DDR4_RTT_NOM_240,                 // rank0
          (DDR4_RTT_WR_240 << 6)   |  (DDR4_RTT_PARK_60 << 3)  |  DDR4_RTT_NOM_240                  // rank1
      },
      {
          // dimm1
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_60 << 6)    |  (DDR4_RTT_PARK_34 << 3)  |  DDR4_RTT_NOM_DIS,                 // rank0
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS                  // rank1
      }
    }
  },
  {
    // Config Index
    (DDR_2666 << 0)   |  // freq
    (DR_DIMM << 7)    |  // slot0
    (LR_DIMM << 10)   |  // slot1
    (EMPTY_DIMM << 13),  // slot2
    50,                  // MC ODT value
    30,                  // CPU RON value
    84,                  // MC Vref Percent
    {
      82,                // DRAM0 Vref percent
      81                 // DRAM1 Vref percent
    },
    {
      {
          // dimm0
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_240 << 6)   |  (DDR4_RTT_PARK_60 << 3)  |  DDR4_RTT_NOM_240,                 // rank0
          (DDR4_RTT_WR_240 << 6)   |  (DDR4_RTT_PARK_60 << 3)  |  DDR4_RTT_NOM_240                  // rank1
      },
      {
          // dimm1
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_60 << 6)    |  (DDR4_RTT_PARK_34 << 3)  |  DDR4_RTT_NOM_DIS,                 // rank0
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS                  // rank1
      }
    }
  },
  {
    // Config Index
    (DDR_2933 << 0)   |  // freq
    (DR_DIMM << 7)    |  // slot0
    (LR_DIMM << 10)   |  // slot1
    (EMPTY_DIMM << 13),  // slot2
    50,                  // MC ODT value
    30,                  // CPU RON value
    84,                  // MC Vref Percent
    {
      82,                // DRAM0 Vref percent
      81                 // DRAM1 Vref percent
    },
    {
      {
          // dimm0
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_240 << 6)   |  (DDR4_RTT_PARK_60 << 3)  |  DDR4_RTT_NOM_240,                 // rank0
          (DDR4_RTT_WR_240 << 6)   |  (DDR4_RTT_PARK_60 << 3)  |  DDR4_RTT_NOM_240                  // rank1
      },
      {
          // dimm1
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_60 << 6)    |  (DDR4_RTT_PARK_34 << 3)  |  DDR4_RTT_NOM_DIS,                 // rank0
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS                  // rank1
      }
    }
  },
  {
    // Config Index
    (DDR_3200 << 0)   |  // freq
    (DR_DIMM << 7)    |  // slot0
    (LR_DIMM << 10)   |  // slot1
    (EMPTY_DIMM << 13),  // slot2
    50,                  // MC ODT value
    30,                  // CPU RON value
    84,                  // MC Vref Percent
    {
      82,                // DRAM0 Vref percent
      81                 // DRAM1 Vref percent
    },
    {
      {
          // dimm0
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_240 << 6)   |  (DDR4_RTT_PARK_60 << 3)  |  DDR4_RTT_NOM_240,                 // rank0
          (DDR4_RTT_WR_240 << 6)   |  (DDR4_RTT_PARK_60 << 3)  |  DDR4_RTT_NOM_240                  // rank1
      },
      {
          // dimm1
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_60 << 6)    |  (DDR4_RTT_PARK_34 << 3)  |  DDR4_RTT_NOM_DIS,                 // rank0
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS                  // rank1
      }
    }
  },
  {
    // Config Index
    (DDR_1866 << 0)   |  // freq
    (LR_DIMM << 7)    |  // slot0
    (DR_DIMM << 10)   |  // slot1
    (EMPTY_DIMM << 13),  // slot2
    50,                  // MC ODT value
    30,                  // CPU RON value
    84,                  // MC Vref Percent
    {
      81,                // DRAM0 Vref percent
      82                 // DRAM1 Vref percent
    },
    {
      {
          // dimm0
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_60 << 6)    |  (DDR4_RTT_PARK_34 << 3)  |  DDR4_RTT_NOM_DIS,                 // rank0
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS                  // rank1
      },
      {
          // dimm1
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_240 << 6)   |  (DDR4_RTT_PARK_60 << 3)  |  DDR4_RTT_NOM_240,                 // rank0
          (DDR4_RTT_WR_240 << 6)   |  (DDR4_RTT_PARK_60 << 3)  |  DDR4_RTT_NOM_240                  // rank1
      }
    }
  },
  {
    // Config Index
    (DDR_2133 << 0)   |  // freq
    (LR_DIMM << 7)    |  // slot0
    (DR_DIMM << 10)   |  // slot1
    (EMPTY_DIMM << 13),  // slot2
    50,                  // MC ODT value
    30,                  // CPU RON value
    84,                  // MC Vref Percent
    {
      81,                // DRAM0 Vref percent
      82                 // DRAM1 Vref percent
    },
    {
      {
          // dimm0
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_60 << 6)    |  (DDR4_RTT_PARK_34 << 3)  |  DDR4_RTT_NOM_DIS,                 // rank0
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS                  // rank1
      },
      {
          // dimm1
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_240 << 6)   |  (DDR4_RTT_PARK_60 << 3)  |  DDR4_RTT_NOM_240,                 // rank0
          (DDR4_RTT_WR_240 << 6)   |  (DDR4_RTT_PARK_60 << 3)  |  DDR4_RTT_NOM_240                  // rank1
      }
    }
  },
  {
    // Config Index
    (DDR_2400 << 0)   |  // freq
    (LR_DIMM << 7)    |  // slot0
    (DR_DIMM << 10)   |  // slot1
    (EMPTY_DIMM << 13),  // slot2
    50,                  // MC ODT value
    30,                  // CPU RON value
    84,                  // MC Vref Percent
    {
      81,                // DRAM0 Vref percent
      82                 // DRAM1 Vref percent
    },
    {
      {
          // dimm0
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_60 << 6)    |  (DDR4_RTT_PARK_34 << 3)  |  DDR4_RTT_NOM_DIS,                 // rank0
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS                  // rank1
      },
      {
          // dimm1
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_240 << 6)   |  (DDR4_RTT_PARK_60 << 3)  |  DDR4_RTT_NOM_240,                 // rank0
          (DDR4_RTT_WR_240 << 6)   |  (DDR4_RTT_PARK_60 << 3)  |  DDR4_RTT_NOM_240                  // rank1
      }
    }
  },
  {
    // Config Index
    (DDR_2666 << 0)   |  // freq
    (LR_DIMM << 7)    |  // slot0
    (DR_DIMM << 10)   |  // slot1
    (EMPTY_DIMM << 13),  // slot2
    50,                  // MC ODT value
    30,                  // CPU RON value
    84,                  // MC Vref Percent
    {
      81,                // DRAM0 Vref percent
      82                 // DRAM1 Vref percent
    },
    {
      {
          // dimm0
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_60 << 6)    |  (DDR4_RTT_PARK_34 << 3)  |  DDR4_RTT_NOM_DIS,                 // rank0
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS                  // rank1
      },
      {
          // dimm1
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_240 << 6)   |  (DDR4_RTT_PARK_60 << 3)  |  DDR4_RTT_NOM_240,                 // rank0
          (DDR4_RTT_WR_240 << 6)   |  (DDR4_RTT_PARK_60 << 3)  |  DDR4_RTT_NOM_240                  // rank1
      }
    }
  },
  {
    // Config Index
    (DDR_2933 << 0)   |  // freq
    (LR_DIMM << 7)    |  // slot0
    (DR_DIMM << 10)   |  // slot1
    (EMPTY_DIMM << 13),  // slot2
    50,                  // MC ODT value
    30,                  // CPU RON value
    84,                  // MC Vref Percent
    {
      81,                // DRAM0 Vref percent
      82                 // DRAM1 Vref percent
    },
    {
      {
          // dimm0
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_60 << 6)    |  (DDR4_RTT_PARK_34 << 3)  |  DDR4_RTT_NOM_DIS,                 // rank0
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS                  // rank1
      },
      {
          // dimm1
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_240 << 6)   |  (DDR4_RTT_PARK_60 << 3)  |  DDR4_RTT_NOM_240,                 // rank0
          (DDR4_RTT_WR_240 << 6)   |  (DDR4_RTT_PARK_60 << 3)  |  DDR4_RTT_NOM_240                  // rank1
      }
    }
  },
  {
    // Config Index
    (DDR_3200 << 0)   |  // freq
    (LR_DIMM << 7)    |  // slot0
    (DR_DIMM << 10)   |  // slot1
    (EMPTY_DIMM << 13),  // slot2
    50,                  // MC ODT value
    30,                  // CPU RON value
    84,                  // MC Vref Percent
    {
      81,                // DRAM0 Vref percent
      82                 // DRAM1 Vref percent
    },
    {
      {
          // dimm0
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_60 << 6)    |  (DDR4_RTT_PARK_34 << 3)  |  DDR4_RTT_NOM_DIS,                 // rank0
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS                  // rank1
      },
      {
          // dimm1
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_240 << 6)   |  (DDR4_RTT_PARK_60 << 3)  |  DDR4_RTT_NOM_240,                 // rank0
          (DDR4_RTT_WR_240 << 6)   |  (DDR4_RTT_PARK_60 << 3)  |  DDR4_RTT_NOM_240                  // rank1
      }
    }
  },
  {
    // Config Index
    (DDR_1866 << 0)   |  // freq
    (SR_DIMM << 7)    |  // slot0
    (LR_DIMM << 10)   |  // slot1
    (EMPTY_DIMM << 13),  // slot2
    50,                  // MC ODT value
    30,                  // CPU RON value
    83,                  // MC Vref Percent
    {
      78,                // DRAM0 Vref percent
      78                 // DRAM1 Vref percent
    },
    {
      {
          // dimm0
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_80 << 6)    |  (DDR4_RTT_PARK_34 << 3)  |  DDR4_RTT_NOM_DIS,                 // rank0
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS                  // rank1
      },
      {
          // dimm1
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_80 << 6)    |  (DDR4_RTT_PARK_34 << 3)  |  DDR4_RTT_NOM_DIS,                 // rank0
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS                  // rank1
      }
    }
  },
  {
    // Config Index
    (DDR_2133 << 0)   |  // freq
    (SR_DIMM << 7)    |  // slot0
    (LR_DIMM << 10)   |  // slot1
    (EMPTY_DIMM << 13),  // slot2
    50,                  // MC ODT value
    30,                  // CPU RON value
    83,                  // MC Vref Percent
    {
      78,                // DRAM0 Vref percent
      78                 // DRAM1 Vref percent
    },
    {
      {
          // dimm0
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_80 << 6)    |  (DDR4_RTT_PARK_34 << 3)  |  DDR4_RTT_NOM_DIS,                 // rank0
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS                  // rank1
      },
      {
          // dimm1
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_80 << 6)    |  (DDR4_RTT_PARK_34 << 3)  |  DDR4_RTT_NOM_DIS,                 // rank0
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS                  // rank1
      }
    }
  },
  {
    // Config Index
    (DDR_2400 << 0)   |  // freq
    (SR_DIMM << 7)    |  // slot0
    (LR_DIMM << 10)   |  // slot1
    (EMPTY_DIMM << 13),  // slot2
    50,                  // MC ODT value
    30,                  // CPU RON value
    83,                  // MC Vref Percent
    {
      78,                // DRAM0 Vref percent
      78                 // DRAM1 Vref percent
    },
    {
      {
          // dimm0
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_80 << 6)    |  (DDR4_RTT_PARK_34 << 3)  |  DDR4_RTT_NOM_DIS,                 // rank0
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS                  // rank1
      },
      {
          // dimm1
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_80 << 6)    |  (DDR4_RTT_PARK_34 << 3)  |  DDR4_RTT_NOM_DIS,                 // rank0
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS                  // rank1
      }
    }
  },
  {
    // Config Index
    (DDR_2666 << 0)   |  // freq
    (SR_DIMM << 7)    |  // slot0
    (LR_DIMM << 10)   |  // slot1
    (EMPTY_DIMM << 13),  // slot2
    50,                  // MC ODT value
    30,                  // CPU RON value
    83,                  // MC Vref Percent
    {
      78,                // DRAM0 Vref percent
      78                 // DRAM1 Vref percent
    },
    {
      {
          // dimm0
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_80 << 6)    |  (DDR4_RTT_PARK_34 << 3)  |  DDR4_RTT_NOM_DIS,                 // rank0
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS                  // rank1
      },
      {
          // dimm1
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_80 << 6)    |  (DDR4_RTT_PARK_34 << 3)  |  DDR4_RTT_NOM_DIS,                 // rank0
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS                  // rank1
      }
    }
  },
  {
    // Config Index
    (DDR_2933 << 0)   |  // freq
    (SR_DIMM << 7)    |  // slot0
    (LR_DIMM << 10)   |  // slot1
    (EMPTY_DIMM << 13),  // slot2
    50,                  // MC ODT value
    30,                  // CPU RON value
    83,                  // MC Vref Percent
    {
      78,                // DRAM0 Vref percent
      78                 // DRAM1 Vref percent
    },
    {
      {
          // dimm0
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_80 << 6)    |  (DDR4_RTT_PARK_34 << 3)  |  DDR4_RTT_NOM_DIS,                 // rank0
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS                  // rank1
      },
      {
          // dimm1
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_80 << 6)    |  (DDR4_RTT_PARK_34 << 3)  |  DDR4_RTT_NOM_DIS,                 // rank0
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS                  // rank1
      }
    }
  },
  {
    // Config Index
    (DDR_3200 << 0)   |  // freq
    (SR_DIMM << 7)    |  // slot0
    (LR_DIMM << 10)   |  // slot1
    (EMPTY_DIMM << 13),  // slot2
    50,                  // MC ODT value
    30,                  // CPU RON value
    83,                  // MC Vref Percent
    {
      78,                // DRAM0 Vref percent
      78                 // DRAM1 Vref percent
    },
    {
      {
          // dimm0
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_80 << 6)    |  (DDR4_RTT_PARK_34 << 3)  |  DDR4_RTT_NOM_DIS,                 // rank0
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS                  // rank1
      },
      {
          // dimm1
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_80 << 6)    |  (DDR4_RTT_PARK_34 << 3)  |  DDR4_RTT_NOM_DIS,                 // rank0
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS                  // rank1
      }
    }
  },
  {
    // Config Index
    (DDR_1866 << 0)   |  // freq
    (LR_DIMM << 7)    |  // slot0
    (SR_DIMM << 10)   |  // slot1
    (EMPTY_DIMM << 13),  // slot2
    50,                  // MC ODT value
    30,                  // CPU RON value
    83,                  // MC Vref Percent
    {
      78,                // DRAM0 Vref percent
      78                 // DRAM1 Vref percent
    },
    {
      {
          // dimm0
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_80 << 6)    |  (DDR4_RTT_PARK_34 << 3)  |  DDR4_RTT_NOM_DIS,                 // rank0
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS                  // rank1
      },
      {
          // dimm1
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_80 << 6)    |  (DDR4_RTT_PARK_34 << 3)  |  DDR4_RTT_NOM_DIS,                 // rank0
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS                  // rank1
      }
    }
  },
  {
    // Config Index
    (DDR_2133 << 0)   |  // freq
    (LR_DIMM << 7)    |  // slot0
    (SR_DIMM << 10)   |  // slot1
    (EMPTY_DIMM << 13),  // slot2
    50,                  // MC ODT value
    30,                  // CPU RON value
    83,                  // MC Vref Percent
    {
      78,                // DRAM0 Vref percent
      78                 // DRAM1 Vref percent
    },
    {
      {
          // dimm0
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_80 << 6)    |  (DDR4_RTT_PARK_34 << 3)  |  DDR4_RTT_NOM_DIS,                 // rank0
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS                  // rank1
      },
      {
          // dimm1
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_80 << 6)    |  (DDR4_RTT_PARK_34 << 3)  |  DDR4_RTT_NOM_DIS,                 // rank0
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS                  // rank1
      }
    }
  },
  {
    // Config Index
    (DDR_2400 << 0)   |  // freq
    (LR_DIMM << 7)    |  // slot0
    (SR_DIMM << 10)   |  // slot1
    (EMPTY_DIMM << 13),  // slot2
    50,                  // MC ODT value
    30,                  // CPU RON value
    83,                  // MC Vref Percent
    {
      78,                // DRAM0 Vref percent
      78                 // DRAM1 Vref percent
    },
    {
      {
          // dimm0
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_80 << 6)    |  (DDR4_RTT_PARK_34 << 3)  |  DDR4_RTT_NOM_DIS,                 // rank0
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS                  // rank1
      },
      {
          // dimm1
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_80 << 6)    |  (DDR4_RTT_PARK_34 << 3)  |  DDR4_RTT_NOM_DIS,                 // rank0
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS                  // rank1
      }
    }
  },
  {
    // Config Index
    (DDR_2666 << 0)   |  // freq
    (LR_DIMM << 7)    |  // slot0
    (SR_DIMM << 10)   |  // slot1
    (EMPTY_DIMM << 13),  // slot2
    50,                  // MC ODT value
    30,                  // CPU RON value
    84,                  // MC Vref Percent
    {
      78,                // DRAM0 Vref percent
      78                 // DRAM1 Vref percent
    },
    {
      {
          // dimm0
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_80 << 6)    |  (DDR4_RTT_PARK_34 << 3)  |  DDR4_RTT_NOM_DIS,                 // rank0
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS                  // rank1
      },
      {
          // dimm1
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_80 << 6)    |  (DDR4_RTT_PARK_34 << 3)  |  DDR4_RTT_NOM_DIS,                 // rank0
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS                  // rank1
      }
    }
  },
  {
    // Config Index
    (DDR_2933 << 0)   |  // freq
    (LR_DIMM << 7)    |  // slot0
    (SR_DIMM << 10)   |  // slot1
    (EMPTY_DIMM << 13),  // slot2
    50,                  // MC ODT value
    30,                  // CPU RON value
    84,                  // MC Vref Percent
    {
      78,                // DRAM0 Vref percent
      78                 // DRAM1 Vref percent
    },
    {
      {
          // dimm0
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_80 << 6)    |  (DDR4_RTT_PARK_34 << 3)  |  DDR4_RTT_NOM_DIS,                 // rank0
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS                  // rank1
      },
      {
          // dimm1
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_80 << 6)    |  (DDR4_RTT_PARK_34 << 3)  |  DDR4_RTT_NOM_DIS,                 // rank0
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS                  // rank1
      }
    }
  },
  {
    // Config Index
    (DDR_3200 << 0)   |  // freq
    (LR_DIMM << 7)    |  // slot0
    (SR_DIMM << 10)   |  // slot1
    (EMPTY_DIMM << 13),  // slot2
    50,                  // MC ODT value
    30,                  // CPU RON value
    84,                  // MC Vref Percent
    {
      78,                // DRAM0 Vref percent
      78                 // DRAM1 Vref percent
    },
    {
      {
          // dimm0
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_80 << 6)    |  (DDR4_RTT_PARK_34 << 3)  |  DDR4_RTT_NOM_DIS,                 // rank0
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS                  // rank1
      },
      {
          // dimm1
          // Rtt_wr                    Rtt_park                   Rtt_nom
          (DDR4_RTT_WR_80 << 6)    |  (DDR4_RTT_PARK_34 << 3)  |  DDR4_RTT_NOM_DIS,                 // rank0
          (DDR4_RTT_WR_DIS << 6)   |  (DDR4_RTT_PARK_DIS << 3) |  DDR4_RTT_NOM_DIS                  // rank1
      }
    }
  },
};

#endif //ifndef _DDR4_ODT_VALUE_TABLE_H_
