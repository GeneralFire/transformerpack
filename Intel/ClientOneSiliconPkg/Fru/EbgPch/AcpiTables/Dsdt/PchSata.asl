/**@file
  Integrated SATA controller ACPI methods

 @copyright
  INTEL CONFIDENTIAL
  Copyright 2013 - 2019 Intel Corporation.

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

// Check include file prerequisites
#  ifndef SATA_CTRL
#    error SATA_CTRL macro not defined in the includer, the macro have to represent the actual name of the SATA controller
#  endif // SATA_CTRL
#  ifndef SATA_PCI_ADDR
#    error SATA_PCI_ADDR macro not defined in the includer, the macro have to represent the actual name of the SATA controlles BDF
#  endif // SATA_PCI_ADDR


External(\_SB.SATA_CTRL.SATA_CTRL.SATC, MethodObj)
External(\_SB.SATA_CTRL.SATA_CTRL.SATD, MethodObj)

// the 'SATA_CTRL' a macro to instatiate actual server SATA controller name
// the 'SATA_PCI_ADDR' a macro to instatiate actual server SATA controller PCI addr

// APTIOV_SERVER_OVERRIDE_RC_START 
Scope (\_SB.PC00.SATA_CTRL) {
/**
Device (SATA_CTRL) {
  Method (_ADR, 0) {
    return (SATA_PCI_ADDR)
  }
**/
// APTIOV_SERVER_OVERRIDE_RC_END
  Method(_PS0,0,Serialized) {
    ADBG("SATA0 Ctrlr D0")
    //
    // Call CSD0 only if ST_FDIS_PMC == 0
    //
    If(LEqual(PCHS, PCHL)) {
      If(LNot(SCFD)){
        \_SB.CSD0(MODPHY_SPD_GATING_SATA)
      }
    }
  }

  Method(_PS3,0,Serialized) {
    ADBG("SATA0 Ctrlr D3")
    If(LEqual(PCHS, PCHL)) {
      \_SB.CSD3(MODPHY_SPD_GATING_SATA)
    }
  }

  Method(_S0W) {
    Return(3)
  }

  Device(PRT0)
  {
    Name(_ADR,0x0000FFFF)
  }

  Device(PRT1)
  {
    Name(_ADR,0x0001FFFF)
  }

  Device(PRT2)
  {
    Name(_ADR,0x0002FFFF)
  }

  Device(PRT3)
  {
    Name(_ADR,0x0003FFFF)
  }

  Device(PRT4)
  {
    Name(_ADR,0x0004FFFF)
  }

  Device(PRT5)
  {
    Name(_ADR,0x0005FFFF)
  }
}
