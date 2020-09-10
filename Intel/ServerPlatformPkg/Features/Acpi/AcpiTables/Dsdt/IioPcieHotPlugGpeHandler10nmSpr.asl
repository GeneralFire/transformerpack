/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2020 Intel Corporation. <BR>

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

  Module Name:

    IioPcieHotPlugGpeHandler10nmSpr.asi

**/

#define PM_PME_EVENT_CLEAR_INTS_PORTS(socket, stack, port, slot)                    \
  If ( LEqual(\_SB.PC##socket##stack.BRP##port.PMEP,1) ) {                          \
    Store (\_SB.PC##socket##stack.BRP##port.PMEH((stack - 1) * 8 + slot), Local0)   \
  } else {                                                                          \
    Store (\_SB.PC##socket##stack.BRP##port.HPEH((stack - 1) * 8 + slot), Local0)   \
  }                                                                                 \
  If (Lnot(LEqual(Local0,0xFF))) {                                                  \
    Store ((stack - 1) * 8 + slot, Local1)                                          \
    Notify (\_SB.PC##socket##stack.BRP##port, Local0)                               \
  }

#define PM_PME_EVENT_CLEAR_INTS_STACKS(socket, stack)                               \
  PM_PME_EVENT_CLEAR_INTS_PORTS(socket, stack, A, 1)                                \
  PM_PME_EVENT_CLEAR_INTS_PORTS(socket, stack, B, 2)                                \
  PM_PME_EVENT_CLEAR_INTS_PORTS(socket, stack, C, 3)                                \
  PM_PME_EVENT_CLEAR_INTS_PORTS(socket, stack, D, 4)                                \
  PM_PME_EVENT_CLEAR_INTS_PORTS(socket, stack, E, 5)                                \
  PM_PME_EVENT_CLEAR_INTS_PORTS(socket, stack, F, 6)                                \
  PM_PME_EVENT_CLEAR_INTS_PORTS(socket, stack, G, 7)                                \
  PM_PME_EVENT_CLEAR_INTS_PORTS(socket, stack, H, 8)

#define PM_PME_EVENT_CLEAR_INTS_SOCKET(socket)                                      \
  PM_PME_EVENT_CLEAR_INTS_STACKS(socket, 1)                                         \
  PM_PME_EVENT_CLEAR_INTS_STACKS(socket, 2)                                         \
  PM_PME_EVENT_CLEAR_INTS_STACKS(socket, 3)                                         \
  PM_PME_EVENT_CLEAR_INTS_STACKS(socket, 4)                                         \
  PM_PME_EVENT_CLEAR_INTS_STACKS(socket, 5)

#define PM_PME_EVENT_CLEAR_PENDING_PORTS(socket, stack, port, slot)                 \
  If (LEqual(Local1, (stack - 1) * 8 + slot)) {                                     \
    Store (1, \_SB.PC##socket##stack.BRP##port.PMES)                                \
    Store (1, \_SB.PC##socket##stack.BRP##port.PMEP)                                \
  }

#define PM_PME_EVENT_CLEAR_PENDING_STACKS(socket, stack)                            \
  PM_PME_EVENT_CLEAR_PENDING_PORTS(socket, stack, A, 1)                             \
  PM_PME_EVENT_CLEAR_PENDING_PORTS(socket, stack, B, 2)                             \
  PM_PME_EVENT_CLEAR_PENDING_PORTS(socket, stack, C, 3)                             \
  PM_PME_EVENT_CLEAR_PENDING_PORTS(socket, stack, D, 4)                             \
  PM_PME_EVENT_CLEAR_PENDING_PORTS(socket, stack, E, 5)                             \
  PM_PME_EVENT_CLEAR_PENDING_PORTS(socket, stack, F, 6)                             \
  PM_PME_EVENT_CLEAR_PENDING_PORTS(socket, stack, G, 7)                             \
  PM_PME_EVENT_CLEAR_PENDING_PORTS(socket, stack, H, 8)

#define PM_PME_EVENT_CLEAR_PENDING_SOCKET(socket)                                   \
  PM_PME_EVENT_CLEAR_PENDING_STACKS(socket, 1)                                      \
  PM_PME_EVENT_CLEAR_PENDING_STACKS(socket, 2)                                      \
  PM_PME_EVENT_CLEAR_PENDING_STACKS(socket, 3)                                      \
  PM_PME_EVENT_CLEAR_PENDING_STACKS(socket, 4)                                      \
  PM_PME_EVENT_CLEAR_PENDING_STACKS(socket, 5)

  //
  // Delay introduced as initial delay after entering ACPI hotplug method
  //
  Sleep (200)
  Store (0x01, IO80)
  Sleep (10)
  Store (0,Local1)

    PM_PME_EVENT_CLEAR_INTS_SOCKET(0)

#if MAX_SOCKET > 1
    PM_PME_EVENT_CLEAR_INTS_SOCKET(1)
#endif

#if MAX_SOCKET > 2
    PM_PME_EVENT_CLEAR_INTS_SOCKET(2)
#endif

#if MAX_SOCKET > 3
    PM_PME_EVENT_CLEAR_INTS_SOCKET(3)
#endif

#if MAX_SOCKET > 4
    PM_PME_EVENT_CLEAR_INTS_SOCKET(4)
#endif

#if MAX_SOCKET > 5
    PM_PME_EVENT_CLEAR_INTS_SOCKET(5)
#endif

#if MAX_SOCKET > 6
    PM_PME_EVENT_CLEAR_INTS_SOCKET(6)
#endif

#if MAX_SOCKET > 7
    PM_PME_EVENT_CLEAR_INTS_SOCKET(7)
#endif


  //If a hotplug event was serviced check if this was generated by PM_PME
  If (Lnot (LEqual(Local0, 0))) {
    //Clear the status bit 16 of PMEStatus
    //Clear the PME Pending bit 17 of PMEStatus

    PM_PME_EVENT_CLEAR_PENDING_SOCKET(0)

#if MAX_SOCKET > 1
    PM_PME_EVENT_CLEAR_PENDING_SOCKET(1)
#endif

#if MAX_SOCKET > 2
    PM_PME_EVENT_CLEAR_PENDING_SOCKET(2)
#endif

#if MAX_SOCKET > 3
    PM_PME_EVENT_CLEAR_PENDING_SOCKET(3)
#endif

#if MAX_SOCKET > 4
    PM_PME_EVENT_CLEAR_PENDING_SOCKET(4)
#endif

#if MAX_SOCKET > 5
    PM_PME_EVENT_CLEAR_PENDING_SOCKET(5)
#endif

#if MAX_SOCKET > 6
    PM_PME_EVENT_CLEAR_PENDING_SOCKET(6)
#endif

#if MAX_SOCKET > 7
    PM_PME_EVENT_CLEAR_PENDING_SOCKET(7)
#endif

    Store(0x01,PEES)               //Clear bit 9 of Status
    Store(0x00,PMEE)               //Clear bit 9 of GPE0_EN
  }
