/** @file
  Data structures for DDR4 ODT Activation Table

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
  Input Source: icx_odt_act_with_backside.xml 2019-08-13 17:17:42 
**/

#ifndef _DDR4_ODT_ACTIVATION_TABLE_H_
#define _DDR4_ODT_ACTIVATION_TABLE_H_

struct ddrOdtActivationTableEntry Ddr4OdtActTable[] = {

  // Config index
  { (DIMM0 << 0)              | // target dimm
    (RANK0 << 2)              | // target rank
    (SR_DIMM << 7)            | // slot 0
    (EMPTY_DIMM << 10)        | // slot 1
    (EMPTY_DIMM << 13),         // slot 2

    // ODT activation matrix
    {
      // read
      MCODT_ACT << 15  |
      0,
      // write
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)    |
      0,
    }
  },
  // Config index
  { (DIMM0 << 0)              | // target dimm
    (RANK0 << 2)              | // target rank
    (LR_DIMM << 7)            | // slot 0
    (EMPTY_DIMM << 10)        | // slot 1
    (EMPTY_DIMM << 13),         // slot 2

    // ODT activation matrix
    {
      // read
      MCODT_ACT << 15  |
      0,
      // write
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)    |
      0,
    }
  },
  // Config index
  { (DIMM0 << 0)              | // target dimm
    (RANK1 << 2)              | // target rank
    (LR_DIMM << 7)            | // slot 0
    (EMPTY_DIMM << 10)        | // slot 1
    (EMPTY_DIMM << 13),         // slot 2

    // ODT activation matrix
    {
      // read
      MCODT_ACT << 15  |
      0,
      // write
      0,
    }
  },
  // Config index
  { (DIMM0 << 0)              | // target dimm
    (RANK2 << 2)              | // target rank
    (LR_DIMM << 7)            | // slot 0
    (EMPTY_DIMM << 10)        | // slot 1
    (EMPTY_DIMM << 13),         // slot 2

    // ODT activation matrix
    {
      // read
      MCODT_ACT << 15  |
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)    |
      0,
      // write
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)    |
      0,
    }
  },
  // Config index
  { (DIMM0 << 0)              | // target dimm
    (RANK3 << 2)              | // target rank
    (LR_DIMM << 7)            | // slot 0
    (EMPTY_DIMM << 10)        | // slot 1
    (EMPTY_DIMM << 13),         // slot 2

    // ODT activation matrix
    {
      // read
      MCODT_ACT << 15  |
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK1)    |
      0,
      // write
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK1)    |
      0,
    }
  },
  // Config index
  { (DIMM0 << 0)              | // target dimm
    (RANK0 << 2)              | // target rank
    (DR_DIMM << 7)            | // slot 0
    (EMPTY_DIMM << 10)        | // slot 1
    (EMPTY_DIMM << 13),         // slot 2

    // ODT activation matrix
    {
      // read
      MCODT_ACT << 15  |
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK1)    |
      0,
      // write
      0,
    }
  },
  // Config index
  { (DIMM0 << 0)              | // target dimm
    (RANK1 << 2)              | // target rank
    (DR_DIMM << 7)            | // slot 0
    (EMPTY_DIMM << 10)        | // slot 1
    (EMPTY_DIMM << 13),         // slot 2

    // ODT activation matrix
    {
      // read
      MCODT_ACT << 15  |
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)    |
      0,
      // write
      0,
    }
  },
  // Config index
  { (DIMM0 << 0)              | // target dimm
    (RANK0 << 2)              | // target rank
    (SR_DIMM << 7)            | // slot 0
    (SR_DIMM << 10)           | // slot 1
    (EMPTY_DIMM << 13),         // slot 2

    // ODT activation matrix
    {
      // read
      MCODT_ACT << 15  |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)    |
      0,
      // write
      0,
    }
  },
  // Config index
  { (DIMM1 << 0)              | // target dimm
    (RANK0 << 2)              | // target rank
    (SR_DIMM << 7)            | // slot 0
    (SR_DIMM << 10)           | // slot 1
    (EMPTY_DIMM << 13),         // slot 2

    // ODT activation matrix
    {
      // read
      MCODT_ACT << 15  |
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)    |
      0,
      // write
      0,
    }
  },
  // Config index
  { (DIMM0 << 0)              | // target dimm
    (RANK0 << 2)              | // target rank
    (DR_DIMM << 7)            | // slot 0
    (DR_DIMM << 10)           | // slot 1
    (EMPTY_DIMM << 13),         // slot 2

    // ODT activation matrix
    {
      // read
      MCODT_ACT << 15  |
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK1)    |
      0,
      // write
      0,
    }
  },
  // Config index
  { (DIMM0 << 0)              | // target dimm
    (RANK1 << 2)              | // target rank
    (DR_DIMM << 7)            | // slot 0
    (DR_DIMM << 10)           | // slot 1
    (EMPTY_DIMM << 13),         // slot 2

    // ODT activation matrix
    {
      // read
      MCODT_ACT << 15  |
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)    |
      0,
      // write
      0,
    }
  },
  // Config index
  { (DIMM1 << 0)              | // target dimm
    (RANK0 << 2)              | // target rank
    (DR_DIMM << 7)            | // slot 0
    (DR_DIMM << 10)           | // slot 1
    (EMPTY_DIMM << 13),         // slot 2

    // ODT activation matrix
    {
      // read
      MCODT_ACT << 15  |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK1)    |
      0,
      // write
      0,
    }
  },
  // Config index
  { (DIMM1 << 0)              | // target dimm
    (RANK1 << 2)              | // target rank
    (DR_DIMM << 7)            | // slot 0
    (DR_DIMM << 10)           | // slot 1
    (EMPTY_DIMM << 13),         // slot 2

    // ODT activation matrix
    {
      // read
      MCODT_ACT << 15  |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)    |
      0,
      // write
      0,
    }
  },
  // Config index
  { (DIMM0 << 0)              | // target dimm
    (RANK0 << 2)              | // target rank
    (DR_DIMM << 7)            | // slot 0
    (SR_DIMM << 10)           | // slot 1
    (EMPTY_DIMM << 13),         // slot 2

    // ODT activation matrix
    {
      // read
      MCODT_ACT << 15  |
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK1)    |
      0,
      // write
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)    |
      0,
    }
  },
  // Config index
  { (DIMM0 << 0)              | // target dimm
    (RANK1 << 2)              | // target rank
    (DR_DIMM << 7)            | // slot 0
    (SR_DIMM << 10)           | // slot 1
    (EMPTY_DIMM << 13),         // slot 2

    // ODT activation matrix
    {
      // read
      MCODT_ACT << 15  |
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)    |
      0,
      // write
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)    |
      0,
    }
  },
  // Config index
  { (DIMM1 << 0)              | // target dimm
    (RANK0 << 2)              | // target rank
    (DR_DIMM << 7)            | // slot 0
    (SR_DIMM << 10)           | // slot 1
    (EMPTY_DIMM << 13),         // slot 2

    // ODT activation matrix
    {
      // read
      MCODT_ACT << 15  |
      0,
      // write
      0,
    }
  },
  // Config index
  { (DIMM0 << 0)              | // target dimm
    (RANK0 << 2)              | // target rank
    (SR_DIMM << 7)            | // slot 0
    (DR_DIMM << 10)           | // slot 1
    (EMPTY_DIMM << 13),         // slot 2

    // ODT activation matrix
    {
      // read
      MCODT_ACT << 15  |
      0,
      // write
      0,
    }
  },
  // Config index
  { (DIMM1 << 0)              | // target dimm
    (RANK0 << 2)              | // target rank
    (SR_DIMM << 7)            | // slot 0
    (DR_DIMM << 10)           | // slot 1
    (EMPTY_DIMM << 13),         // slot 2

    // ODT activation matrix
    {
      // read
      MCODT_ACT << 15  |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK1)    |
      0,
      // write
      0,
    }
  },
  // Config index
  { (DIMM1 << 0)              | // target dimm
    (RANK1 << 2)              | // target rank
    (SR_DIMM << 7)            | // slot 0
    (DR_DIMM << 10)           | // slot 1
    (EMPTY_DIMM << 13),         // slot 2

    // ODT activation matrix
    {
      // read
      MCODT_ACT << 15  |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)    |
      0,
      // write
      0,
    }
  },
  // Config index
  { (DIMM0 << 0)              | // target dimm
    (RANK0 << 2)              | // target rank
    (LR_DIMM << 7)            | // slot 0
    (LR_DIMM << 10)           | // slot 1
    (EMPTY_DIMM << 13),         // slot 2

    // ODT activation matrix
    {
      // read
      MCODT_ACT << 15  |
      0,
      // write
      0,
    }
  },
  // Config index
  { (DIMM0 << 0)              | // target dimm
    (RANK1 << 2)              | // target rank
    (LR_DIMM << 7)            | // slot 0
    (LR_DIMM << 10)           | // slot 1
    (EMPTY_DIMM << 13),         // slot 2

    // ODT activation matrix
    {
      // read
      MCODT_ACT << 15  |
      0,
      // write
      0,
    }
  },
  // Config index
  { (DIMM0 << 0)              | // target dimm
    (RANK2 << 2)              | // target rank
    (LR_DIMM << 7)            | // slot 0
    (LR_DIMM << 10)           | // slot 1
    (EMPTY_DIMM << 13),         // slot 2

    // ODT activation matrix
    {
      // read
      MCODT_ACT << 15  |
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)    |
      0,
      // write
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)    |
      0,
    }
  },
  // Config index
  { (DIMM0 << 0)              | // target dimm
    (RANK3 << 2)              | // target rank
    (LR_DIMM << 7)            | // slot 0
    (LR_DIMM << 10)           | // slot 1
    (EMPTY_DIMM << 13),         // slot 2

    // ODT activation matrix
    {
      // read
      MCODT_ACT << 15  |
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK1)    |
      0,
      // write
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK1)    |
      0,
    }
  },
  // Config index
  { (DIMM1 << 0)              | // target dimm
    (RANK0 << 2)              | // target rank
    (LR_DIMM << 7)            | // slot 0
    (LR_DIMM << 10)           | // slot 1
    (EMPTY_DIMM << 13),         // slot 2

    // ODT activation matrix
    {
      // read
      MCODT_ACT << 15  |
      0,
      // write
      0,
    }
  },
  // Config index
  { (DIMM1 << 0)              | // target dimm
    (RANK1 << 2)              | // target rank
    (LR_DIMM << 7)            | // slot 0
    (LR_DIMM << 10)           | // slot 1
    (EMPTY_DIMM << 13),         // slot 2

    // ODT activation matrix
    {
      // read
      MCODT_ACT << 15  |
      0,
      // write
      0,
    }
  },
  // Config index
  { (DIMM1 << 0)              | // target dimm
    (RANK2 << 2)              | // target rank
    (LR_DIMM << 7)            | // slot 0
    (LR_DIMM << 10)           | // slot 1
    (EMPTY_DIMM << 13),         // slot 2

    // ODT activation matrix
    {
      // read
      MCODT_ACT << 15  |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)    |
      0,
      // write
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)    |
      0,
    }
  },
  // Config index
  { (DIMM1 << 0)              | // target dimm
    (RANK3 << 2)              | // target rank
    (LR_DIMM << 7)            | // slot 0
    (LR_DIMM << 10)           | // slot 1
    (EMPTY_DIMM << 13),         // slot 2

    // ODT activation matrix
    {
      // read
      MCODT_ACT << 15  |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK1)    |
      0,
      // write
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK1)    |
      0,
    }
  },
  // Config index
  { (DIMM0 << 0)              | // target dimm
    (RANK0 << 2)              | // target rank
    (DR_DIMM << 7)            | // slot 0
    (LR_DIMM << 10)           | // slot 1
    (EMPTY_DIMM << 13),         // slot 2

    // ODT activation matrix
    {
      // read
      MCODT_ACT << 15  |
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK1)    |
      0,
      // write
      0,
    }
  },
  // Config index
  { (DIMM0 << 0)              | // target dimm
    (RANK1 << 2)              | // target rank
    (DR_DIMM << 7)            | // slot 0
    (LR_DIMM << 10)           | // slot 1
    (EMPTY_DIMM << 13),         // slot 2

    // ODT activation matrix
    {
      // read
      MCODT_ACT << 15  |
      ODT_ACT << (DIMM0 * MAX_RANK_DIMM + RANK0)    |
      0,
      // write
      0,
    }
  },
  // Config index
  { (DIMM1 << 0)              | // target dimm
    (RANK0 << 2)              | // target rank
    (DR_DIMM << 7)            | // slot 0
    (LR_DIMM << 10)           | // slot 1
    (EMPTY_DIMM << 13),         // slot 2

    // ODT activation matrix
    {
      // read
      MCODT_ACT << 15  |
      0,
      // write
      0,
    }
  },
  // Config index
  { (DIMM0 << 0)              | // target dimm
    (RANK0 << 2)              | // target rank
    (LR_DIMM << 7)            | // slot 0
    (DR_DIMM << 10)           | // slot 1
    (EMPTY_DIMM << 13),         // slot 2

    // ODT activation matrix
    {
      // read
      MCODT_ACT << 15  |
      0,
      // write
      0,
    }
  },
  // Config index
  { (DIMM1 << 0)              | // target dimm
    (RANK0 << 2)              | // target rank
    (LR_DIMM << 7)            | // slot 0
    (DR_DIMM << 10)           | // slot 1
    (EMPTY_DIMM << 13),         // slot 2

    // ODT activation matrix
    {
      // read
      MCODT_ACT << 15  |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK1)    |
      0,
      // write
      0,
    }
  },
  // Config index
  { (DIMM1 << 0)              | // target dimm
    (RANK1 << 2)              | // target rank
    (LR_DIMM << 7)            | // slot 0
    (DR_DIMM << 10)           | // slot 1
    (EMPTY_DIMM << 13),         // slot 2

    // ODT activation matrix
    {
      // read
      MCODT_ACT << 15  |
      ODT_ACT << (DIMM1 * MAX_RANK_DIMM + RANK0)    |
      0,
      // write
      0,
    }
  },
  // Config index
  { (DIMM0 << 0)              | // target dimm
    (RANK0 << 2)              | // target rank
    (SR_DIMM << 7)            | // slot 0
    (LR_DIMM << 10)           | // slot 1
    (EMPTY_DIMM << 13),         // slot 2

    // ODT activation matrix
    {
      // read
      MCODT_ACT << 15  |
      0,
      // write
      0,
    }
  },
  // Config index
  { (DIMM1 << 0)              | // target dimm
    (RANK0 << 2)              | // target rank
    (SR_DIMM << 7)            | // slot 0
    (LR_DIMM << 10)           | // slot 1
    (EMPTY_DIMM << 13),         // slot 2

    // ODT activation matrix
    {
      // read
      MCODT_ACT << 15  |
      0,
      // write
      0,
    }
  },
  // Config index
  { (DIMM0 << 0)              | // target dimm
    (RANK0 << 2)              | // target rank
    (LR_DIMM << 7)            | // slot 0
    (SR_DIMM << 10)           | // slot 1
    (EMPTY_DIMM << 13),         // slot 2

    // ODT activation matrix
    {
      // read
      MCODT_ACT << 15  |
      0,
      // write
      0,
    }
  },
  // Config index
  { (DIMM1 << 0)              | // target dimm
    (RANK0 << 2)              | // target rank
    (LR_DIMM << 7)            | // slot 0
    (SR_DIMM << 10)           | // slot 1
    (EMPTY_DIMM << 13),         // slot 2

    // ODT activation matrix
    {
      // read
      MCODT_ACT << 15  |
      0,
      // write
      0,
    }
  },
};

#endif //ifndef _DDR4_ODT_ACTIVATION_TABLE_H_
