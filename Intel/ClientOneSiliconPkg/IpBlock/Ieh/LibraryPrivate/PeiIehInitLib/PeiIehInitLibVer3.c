/** @file
  Initializes IEH Device in PEI

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

#include <Register/IehRegs.h>

#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/PciSegmentLib.h>
#include <IndustryStandard/Pci30.h>

/**
  Programs severity register bits based on ITSS UEV register

  @param[in]  IehPciBase    IEH Controller PCI Base address
**/
VOID
IehProgramSeverity (
  IN  UINT64         IehPciBase
  )
{
  UINT64         Data64;

  //
  // Programming serverity values (LERRUNCSEV)
  //
  Data64  = BIT5 | BIT4 | BIT3 | BIT2 | BIT1 | BIT0;
  //
  // UR Severity related bits
  //
  Data64 |= BIT36 | BIT32 | BIT28 | BIT24 | BIT20 | BIT16 | BIT12 | BIT8;
  //
  // UC Severity related bits
  //
  Data64 |= BIT35 | BIT31 | BIT27 | BIT23 | BIT19 | BIT15 | BIT11 | BIT7;
  //
  // CMD Parity severity bits
  //
  Data64 |= BIT34 | BIT30 | BIT26 | BIT22 | BIT18 | BIT14 | BIT10 | BIT6;

  PciSegmentWrite32 (IehPciBase + R_IEH_CFG_LERRUNCSEV, (UINT32) Data64);
  PciSegmentWrite32 (IehPciBase + R_IEH_CFG_LERRUNCSEV + 4, (UINT32) RShiftU64 (Data64, 32));
}

/**
  Programs IEH to Enable Mode

  @param[in]  IehPciBase    IEH Controller PCI Base address
**/
VOID
IehEnableMode (
  IN  UINT64         IehPciBase
  )
{
  PciSegmentWrite32 (IehPciBase + R_IEH_CFG_GCOERRMSK, 0x0);
  PciSegmentWrite32 (IehPciBase + R_IEH_CFG_GNFERRMSK, 0x0);
  PciSegmentWrite32 (IehPciBase + R_IEH_CFG_GFAERRMSK, 0x0);
  PciSegmentWrite32 (IehPciBase + R_IEH_CFG_GSYSEVTMSK, 0x0);
  PciSegmentAndThenOr32 (IehPciBase + R_IEH_CFG_GSYSEVTMAP, 0x7FFFFFC0, 0x3F);
  PciSegmentAnd32 (IehPciBase + R_IEH_CFG_IEHTYPEVER, (UINT32)~(B_IEH_CFG_IEHTYPEVER_IEH_BUSNUM));
  PciSegmentAnd32 (IehPciBase + R_IEH_CFG_ERRPINCTRL, 0xFFFFFFC0);
  PciSegmentAnd32 (IehPciBase + R_IEH_CFG_ERRPINDATA, 0xFFFFFFF8);
  PciSegmentAnd32 (IehPciBase + R_IEH_CFG_BITMAP, 0xFFFFFFE0);
  PciSegmentWrite32 (IehPciBase + R_IEH_CFG_LERRUNCMSK, 0x0);
  PciSegmentWrite32 (IehPciBase + R_IEH_CFG_LERRUNCMSK + 4, 0xFFFFFF00);
  PciSegmentWrite32 (IehPciBase + R_IEH_CFG_LERRCORMSK, 0xC0000000);
  PciSegmentWrite32 (IehPciBase + R_IEH_CFG_LERRCORMSK + 4, 0xFFFFFFFF);
  PciSegmentWrite32 (IehPciBase + R_IEH_CFG_LCOERRCNTSEL, 0x0);
}