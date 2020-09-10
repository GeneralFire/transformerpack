/** @file
  PCIE controller initialization routines.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2020 Intel Corporation. <BR>

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

#include <RcRegs.h>
#include <IioRegs.h>

#include <Library/IioDataHubLib.h>
#include <Library/IioAccessLib.h>
#include <LibraryPrivate/IioDebug.h>


/**
  Used to set what lanes within the PCIE ports should be masked during link training
  according to setup knob Override Max Link Width.

  @param[in] IioGlobalData            Pointer to IioGlobalData
  @param[in] IioIndex                 IIO index (socket number)
  @param[in] PortIndex                PCIe port index
  @param[in] DefaultMaxLinkWidth      Platform Default Max link width
  @param[in] OverrideMaxLinkWidth     Max link width set by BIOS knob to override default
  @param[in] LaneReversedLink         Port configured as lane reversed
**/
VOID
IioPcieMaxLinkWidth (
  IIO_GLOBALS                               *IioGlobalData,
  UINT8                                     IioIndex,
  UINT8                                     PortIndex,
  UINT8                                     DefaultMaxLinkWidth,
  UINT8                                     OverrideMaxLinkWidth
)
{
  LTSSMTRNCTL_SB_FBLP_STRUCT              LtssmTrnCtl;
  LTSSMTRNCTL_SB_FBLP_STRUCT              LtssmTrnCtlBckup;
  LTSSMTRNCTL_SB_FBLP_STRUCT              LtssmTrnCtlBckup2;

  LtssmTrnCtl.Data = IioReadCpuCsr32 (IioIndex, PortIndex, LTSSMTRNCTL_SB_FBLP_REG);
  LtssmTrnCtlBckup.Data = LtssmTrnCtl.Data;

  switch (OverrideMaxLinkWidth) {
    case PORT_LINK_WIDTH_x1:
      LtssmTrnCtl.Bits.widthmask |= BIT1;
    case PORT_LINK_WIDTH_x2:
      LtssmTrnCtl.Bits.widthmask |= BIT2;
    case PORT_LINK_WIDTH_x4:
      LtssmTrnCtl.Bits.widthmask |= BIT3;
    case PORT_LINK_WIDTH_x8:
      LtssmTrnCtl.Bits.widthmask |= BIT4;
    case PORT_LINK_WIDTH_x16:
    default:
      break;
  }

  LtssmTrnCtlBckup2.Data = LtssmTrnCtl.Data;
  IioWriteCpuCsr32 (IioIndex, PortIndex, LTSSMTRNCTL_SB_FBLP_REG, LtssmTrnCtl.Data);

  LtssmTrnCtl.Data = IioReadCpuCsr32 (IioIndex, PortIndex, LTSSMTRNCTL_SB_FBLP_REG);
  IIO_D_PCIDBG ("Link Width (IIO%d, port%s(%d)) forced to x%d  widthmask 0x%x -> 0x%x -> 0x%x\n",
    IioIndex, IIO_PORT_LABEL (PortIndex), PortIndex, OverrideMaxLinkWidth,
    LtssmTrnCtlBckup.Bits.widthmask, LtssmTrnCtlBckup2.Bits.widthmask, LtssmTrnCtl.Bits.widthmask);
}

