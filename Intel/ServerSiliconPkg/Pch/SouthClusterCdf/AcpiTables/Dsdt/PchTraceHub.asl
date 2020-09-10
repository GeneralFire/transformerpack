/** @file
  PCH Trace Hub Device

@copyright
  INTEL CONFIDENTIAL
  Copyright 2017 - 2020 Intel Corporation.

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

  This file contains a 'Sample Driver' and is licensed as such under the terms
  of your license agreement with Intel or your vendor. This file may be modified
  by the user, subject to the additional terms of the license agreement.

@par Specification Reference:
**/

#include <PchReservedResources.h>
External(PTHM, FieldUnitObj)
#define R_TRACE_HUB_MEM_MTB_SCRPD2     0xE8

Scope (\_SB.PC00)
{
  //
  // Declare PCH TH device for target debugger mode
  //
  Device(PTHT)
  {
    Name(_ADR, 0x001F0007)

    Method(_STA, 0, Serialized)
    {
       If (LEqual (PTHM, 1)) {
         Return (0xF)
       } Else {
         Return (0x0)
       }
    }

    OperationRegion (PTHR, PCI_Config, 0x00, 0x20)
    Field(PTHR, AnyAcc, NoLock, Preserve)
    {
      Offset (0x10),
      MTBL,  32,
      MTBH,  32,
    }

    Method (THEN, 0, Serialized, 0, UnknownObj)
    {
      OperationRegion(RMTB, SystemMemory, Add (ShiftLeft (MTBH, 32), And (MTBL, 0xFFF00000)), 0x100)
      Field(RMTB, AnyAcc, NoLock, Preserve) {
        Offset (R_TRACE_HUB_MEM_MTB_SCRPD2),
        BTTH,  1,
      }
      //
      // Check SCRPD2[0] if BIOS trace to TraceHub is enabled
      //
      If (LEqual (BTTH, 1)) {
        Return (1)
      } Else {
        Return (0)
      }
    }
  }
}

Scope (\_SB) {
  //
  // Declare PCH TH device for host debugger mode
  //
  Device(PTHH)
  {
    Name(_HID, "INTC1001")

    Method(_STA,0,Serialized)
    {
      If (LEqual (PTHM, 2)) {
        Return(0xF)
      } Else {
        Return (0x0)
      }
    }

    Name(_CRS, ResourceTemplate() {
        Memory32Fixed(ReadWrite, PCH_TRACE_HUB_MTB_BASE_ADDRESS, PCH_TRACE_HUB_MTB_MMIO_SIZE) // MTB BAR
        Memory32Fixed(ReadWrite, PCH_TRACE_HUB_SW_BASE_ADDRESS, PCH_TRACE_HUB_SW_MMIO_SIZE)   // SW BAR
        })

    OperationRegion(MTBR, SystemMemory, PCH_TRACE_HUB_MTB_BASE_ADDRESS, 0x100)
    Field(MTBR, AnyAcc, NoLock, Preserve) {
      Offset (R_TRACE_HUB_MEM_MTB_SCRPD2),
      BHTH,  1,
    }

    Method (THEN, 0, Serialized, 0, UnknownObj)
    {
      //
      // Check SCRPD2[0] if BIOS trace to TraceHub is enabled
      //
      If (LEqual (BHTH, 1)) {
        Return (1)
      } Else {
        Return (0)
      }
    }
  }
}