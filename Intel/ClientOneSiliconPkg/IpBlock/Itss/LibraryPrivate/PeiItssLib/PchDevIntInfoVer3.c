/** @file
  PCH Generation Specific interrupts.

@copyright
  INTEL CONFIDENTIAL
  Copyright 2019 Intel Corporation.

  The source code contained or described herein and all documents related to the
  source code ("Material") are owned by Intel Corporation or its suppliers or
  licensors. Title to the Material remains with Intel Corporation or its suppliers
  and licensors. The Material may contain trade secrets and proprietary and
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

  This file contains an 'Intel Peripheral Driver' and is uniquely identified as
  "Intel Reference Module" and is licensed for Intel CPUs and chipsets under
  the terms of your license agreement with Intel or your vendor. This file may
  be modified by the user, subject to additional terms of the license agreement.

@par Specification Reference:
**/

#include "ItssLibInternal.h"

//
// mPchTypeDeviceInterruptInfo consist of information for whole range of devices related to PCH. This table must cover
// all possible device and function number (even if they are not a valid device or do not generate an interrupt).
// Data in table must be arranged in ascending order to be correctly consumed by functions which use it.
// @todo: VER3 ITSS: Check and update for Ver3
//
GLOBAL_REMOVE_IF_UNREFERENCED PCH_DEV_INT_INFO mPchDeviceInterruptInfo[] = {
    {8, 0, VALID_DEV_YES, INTERRUPT_YES, USES_PIRQ_YES, INTP_NO_REQ  , IRQ_UNIQUE_NO  }, // PCIE0
    {8, 1, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {8, 2, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {8, 3, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {8, 4, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {8, 5, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {8, 6, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {8, 7, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {9, 0, VALID_DEV_YES, INTERRUPT_YES, USES_PIRQ_YES, INTP_NO_REQ  , IRQ_UNIQUE_NO  }, // PCIE1
    {9, 1, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {9, 2, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {9, 3, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {9, 4, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {9, 5, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {9, 6, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {9, 7, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {10, 0, VALID_DEV_YES, INTERRUPT_YES, USES_PIRQ_YES, INTP_NO_REQ  , IRQ_UNIQUE_NO  }, // PCIE2
    {10, 1, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {10, 2, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {10, 3, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {10, 4, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {10, 5, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {10, 6, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {10, 7, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {11, 0, VALID_DEV_YES, INTERRUPT_YES, USES_PIRQ_YES, INTP_NO_REQ  , IRQ_UNIQUE_NO  }, // PCIE3
    {11, 1, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {11, 2, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {11, 3, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {11, 4, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {11, 5, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {11, 6, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {11, 7, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {12, 0, VALID_DEV_YES, INTERRUPT_YES, USES_PIRQ_YES, INTP_NO_REQ  , IRQ_UNIQUE_NO  }, // PCIE4
    {12, 1, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {12, 2, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {12, 3, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {12, 4, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {12, 5, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {12, 6, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {12, 7, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {13, 0, VALID_DEV_YES, INTERRUPT_YES, USES_PIRQ_YES, INTP_NO_REQ  , IRQ_UNIQUE_NO  }, // PCIE5
    {13, 1, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {13, 2, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {13, 3, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {13, 4, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {13, 5, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {13, 6, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {13, 7, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {14, 0, VALID_DEV_YES, INTERRUPT_YES, USES_PIRQ_YES, INTP_NO_REQ  , IRQ_UNIQUE_NO  }, // PCIE6
    {14, 1, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {14, 2, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {14, 3, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {14, 4, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {14, 5, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {14, 6, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {14, 7, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {15, 0, VALID_DEV_YES, INTERRUPT_YES, USES_PIRQ_YES, INTP_NO_REQ  , IRQ_UNIQUE_NO  }, // PCIE7
    {15, 1, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {15, 2, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {15, 3, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {15, 4, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {15, 5, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {15, 6, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {15, 7, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {16, 0, VALID_DEV_YES, INTERRUPT_YES, USES_PIRQ_YES, INTP_NO_REQ  , IRQ_UNIQUE_NO  }, // PCIE8
    {16, 1, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {16, 2, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {16, 3, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {16, 4, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {16, 5, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {16, 6, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {16, 7, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {17, 0, VALID_DEV_YES, INTERRUPT_YES, USES_PIRQ_YES, INTP_NO_REQ  , IRQ_UNIQUE_NO  }, // PCIE9
    {17, 1, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {17, 2, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {17, 3, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {17, 4, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {17, 5, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {17, 6, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {17, 7, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {18, 0, VALID_DEV_YES, INTERRUPT_YES, USES_PIRQ_YES, INTP_NO_REQ  , IRQ_UNIQUE_NO  }, // PCIE10
    {18, 1, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {18, 2, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {18, 3, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {18, 4, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {18, 5, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {18, 6, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {18, 7, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {19, 0, VALID_DEV_YES, INTERRUPT_YES, USES_PIRQ_YES, INTP_NO_REQ  , IRQ_UNIQUE_NO  }, // PCIE11
    {19, 1, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {19, 2, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {19, 3, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {19, 4, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {19, 5, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {19, 6, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {19, 7, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {20, 0, VALID_DEV_YES, INTERRUPT_YES, USES_PIRQ_YES, INTP_IS_INTA , IRQ_UNIQUE_NO  }, // USB 3.0 xHCI Controller
    {20, 1, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {20, 2, VALID_DEV_YES, INTERRUPT_NO , 0            , 0            , 0              }, // Share SRAM
    {20, 3, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {20, 4, VALID_DEV_YES, INTERRUPT_NO , USES_PIRQ_NO , INTP_NO_REQ  , IRQ_UNIQUE_NO  }, // sIEH
    {20, 5, VALID_DEV_YES, INTERRUPT_NO , USES_PIRQ_NO , INTP_NO_REQ  , 0              }, // ME: fTPM DMA
    {20, 6, VALID_DEV_YES, INTERRUPT_NO , 0            , 0            , 0              }, // ME: PMT,  doesn't use interrupts
    {20, 7, VALID_DEV_YES, INTERRUPT_NO , USES_PIRQ_NO , INTP_NO_REQ  , IRQ_UNIQUE_NO  }, // NPK (ACPI)
    {21, 0, VALID_DEV_YES ,INTERRUPT_YES, USES_PIRQ_YES, INTP_NO_REQ  , IRQ_UNIQUE_NO  }, // SMbus DMA
    {21, 1, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {21, 2, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {21, 3, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {21, 4, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {21, 5, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {21, 6, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {21, 7, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {22, 0, VALID_DEV_YES, INTERRUPT_YES, USES_PIRQ_YES, INTP_NO_REQ  , IRQ_UNIQUE_NO  }, // ME: HECI #1
    {22, 1, VALID_DEV_YES, INTERRUPT_YES, USES_PIRQ_YES, INTP_NO_REQ  , IRQ_UNIQUE_NO  }, // ME: HECI #2
    {22, 2, VALID_DEV_YES, INTERRUPT_YES, USES_PIRQ_YES, INTP_NO_REQ  , IRQ_UNIQUE_NO  }, // ME: PTIO IDE-Redirection (IDE-R)
    {22, 3, VALID_DEV_YES, INTERRUPT_YES, USES_PIRQ_YES, INTP_NO_REQ  , IRQ_UNIQUE_NO  }, // ME: PTIO Keyboard and Text (KT) Redirection
    {22, 4, VALID_DEV_YES, INTERRUPT_YES, USES_PIRQ_YES, INTP_NO_REQ  , IRQ_UNIQUE_NO  }, // ME: HECI #3
    {22, 5, VALID_DEV_YES, INTERRUPT_YES, USES_PIRQ_YES, INTP_NO_REQ  , IRQ_UNIQUE_NO  }, // ME: HECI #4
    {22, 6, VALID_DEV_YES, INTERRUPT_NO , USES_PIRQ_NO , INTP_NO_REQ  , IRQ_UNIQUE_NO  }, // ME: MROM
    {22, 7, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {23, 0, VALID_DEV_YES, INTERRUPT_YES, USES_PIRQ_YES, INTP_MUST_SET, IRQ_UNIQUE_NO  }, // SATA Controller 0
    {23, 1, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {23, 2, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {23, 3, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {23, 4, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {23, 5, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {23, 6, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {23, 7, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {24, 0, VALID_DEV_YES, INTERRUPT_YES, USES_PIRQ_YES, INTP_MUST_SET, IRQ_UNIQUE_NO  }, // SATA Controller 1
    {24, 1, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {24, 2, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {24, 3, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {24, 4, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {24, 5, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {24, 6, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {24, 7, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {25, 0, VALID_DEV_YES, INTERRUPT_YES, USES_PIRQ_YES, INTP_MUST_SET, IRQ_UNIQUE_NO  }, // SATA Controller 2
    {25, 1, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {25, 2, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {25, 3, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {25, 4, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {25, 5, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {25, 6, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {25, 7, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {26, 0, VALID_DEV_YES, INTERRUPT_YES, USES_PIRQ_YES, INTP_NO_REQ  , IRQ_UNIQUE_NO  }, // PCIE12
    {26, 1, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {26, 2, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {26, 3, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {26, 4, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {26, 5, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {26, 6, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {26, 7, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {27, 0, VALID_DEV_YES, INTERRUPT_YES, USES_PIRQ_YES, INTP_NO_REQ  , IRQ_UNIQUE_NO  }, // PCIE13
    {27, 1, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {27, 2, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {27, 3, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {27, 4, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {27, 5, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {27, 6, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {27, 7, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {28, 0, VALID_DEV_YES, INTERRUPT_YES, USES_PIRQ_YES, INTP_NO_REQ  , IRQ_UNIQUE_NO  }, // PCIE14
    {28, 1, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {28, 2, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {28, 3, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {28, 4, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {28, 5, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {28, 6, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {28, 7, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {29, 0, VALID_DEV_YES, INTERRUPT_YES, USES_PIRQ_YES, INTP_NO_REQ  , IRQ_UNIQUE_NO  }, // PCIE15
    {29, 1, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {29, 2, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {29, 3, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {29, 4, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {29, 5, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {29, 6, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {29, 7, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {30, 0, VALID_DEV_YES, INTERRUPT_YES, USES_PIRQ_YES, INTP_NO_REQ  , IRQ_UNIQUE_NO  }, // HSUART0
    {30, 1, VALID_DEV_YES, INTERRUPT_YES, USES_PIRQ_YES, INTP_NO_REQ  , IRQ_UNIQUE_NO  }, // HSUART1
    {30, 2, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {30, 3, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {30, 4, VALID_DEV_YES, INTERRUPT_YES, USES_PIRQ_YES, INTP_IS_INTA , IRQ_UNIQUE_NO  }, // SMBbus DMA
    {30, 5, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {30, 6, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {30, 7, VALID_DEV_NO , 0            , 0            , 0            , 0              }, // Not a valid PCH device
    {31, 0, VALID_DEV_YES, INTERRUPT_NO , 0            , 0            , 0              }, // LPC/eSPI Interface, doesn't use interrupts
    {31, 1, VALID_DEV_YES, INTERRUPT_NO , 0            , 0            , 0              }, // P2SB, doesn't use interrupts
    {31, 2, VALID_DEV_YES, INTERRUPT_NO , 0            , 0            , 0              }, // PMC , doesn't use interrupts
    {31, 3, VALID_DEV_YES, INTERRUPT_YES, USES_PIRQ_YES, INTP_NO_REQ  , IRQ_UNIQUE_NO  }, // cAVS
    {31, 4, VALID_DEV_YES, INTERRUPT_YES, USES_PIRQ_YES, INTP_MUST_SET, IRQ_UNIQUE_NO  }, // SMBus Legacy
    {31, 5, VALID_DEV_YES, INTERRUPT_NO , 0            , 0            , 0              }, // SPI , doesn't use interrupts
    {31, 6, VALID_DEV_YES, INTERRUPT_YES, USES_PIRQ_YES, INTP_MUST_SET, IRQ_UNIQUE_NO  }, // GbE Controller
    {31, 7, VALID_DEV_YES, INTERRUPT_YES, USES_PIRQ_YES, INTP_IS_INTA , IRQ_UNIQUE_NO  }  // NPK
};

GLOBAL_REMOVE_IF_UNREFERENCED UINT32 mPchDeviceInterruptInfoSize = ARRAY_SIZE (mPchDeviceInterruptInfo);
