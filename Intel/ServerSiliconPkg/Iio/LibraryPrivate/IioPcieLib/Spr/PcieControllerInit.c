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

#include <Library/IioAccessLib.h>
#include <Library/IioDataHubLib.h>
#include <RcRegs.h>
#include <Library/IioDmiLib.h>
#include <Chip/Include/CpuPciAccess.h>
#include <Library/IioSysInfoLib.h>
#include <IioSetupDefinitions.h>
#include <Library/PciMmio32AssignLib.h>
#include <Library/KtiApi.h>
#include <Library/IioPcieInit.h>
#include <Library/IioNtbInit.h>
#include <Library/SiliconWorkaroundLib.h>

#include <LibraryPrivate/IioDebug.h>


typedef struct {
  UINT8   Gen4;
  UINT8   Gen5;
  UINT8   Pcs;
  BOOLEAN Valid;
} PI5_CONTROLLER_CONFIG;

CONST PI5_CONTROLLER_CONFIG ModesConfigTable[][PI5_MODE_MAX] = {
/*  {           DMI           }, {           IAL           }, {        Gen4 only        }, {        Gen5 only        }, {       Gen5 + Gen4      }, {        NTB Gen4         }, {     NTB Gen4 + Gen5     }, */
/*  { Gen4  Gen5  Pcs   Valid }, { Gen4  Gen5  Pcs   Valid }, { Gen4  Gen5  Pcs   Valid }, { Gen4  Gen5  Pcs   Valid }, { Gen4  Gen5  Pcs   Valid}, { Gen4  Gen5  Pcs   Valid }, { Gen4  Gen5  Pcs   Valid }, */ //   port    0   1    2    3    4    5    6    7   in gen5
  { { 0x03, 0x00, 0x00, FALSE }, { 0x00, 0x04, 0xFF, FALSE }, { 0x00, 0x00, 0x00, TRUE  }, { 0x00, 0x00, 0xFF, TRUE  }, { 0x00, 0x00, 0xFF, TRUE }, { 0x00, 0x00, 0x00, TRUE  }, { 0x00, 0x00, 0x3F, TRUE  } }, //   00 = |  x4 | -- | x4 | -- | x4 | -- | x4 | -- | IIO_BIFURCATE_x4x4x4x4
  { { 0x03, 0x00, 0x00, FALSE }, { 0x00, 0x04, 0xFF, FALSE }, { 0x02, 0x00, 0x00, TRUE  }, { 0x00, 0x01, 0xFF, TRUE  }, { 0x00, 0x01, 0xFF, TRUE }, { 0x02, 0x00, 0x00, TRUE  }, { 0x02, 0x01, 0x3F, TRUE  } }, //   01 = |  x8 | -- | -- | -- | x4 | -- | x4 | -- | IIO_BIFURCATE_x4x4xxx8
  { { 0x03, 0x00, 0x00, FALSE }, { 0x00, 0x04, 0xFF, FALSE }, { 0x01, 0x00, 0x00, TRUE  }, { 0x00, 0x02, 0xFF, TRUE  }, { 0x00, 0x02, 0xFF, TRUE }, { 0x01, 0x00, 0x00, TRUE  }, { 0x01, 0x02, 0x0F, TRUE  } }, //   02 = |  x4 | -- | x4 | -- | x8 | -- | -- | -- | IIO_BIFURCATE_xxx8x4x4
  { { 0x03, 0x00, 0x00, TRUE  }, { 0x00, 0x04, 0xFF, FALSE }, { 0x03, 0x00, 0x00, TRUE  }, { 0x00, 0x03, 0xFF, TRUE  }, { 0x00, 0x03, 0xFF, TRUE }, { 0x03, 0x00, 0x00, TRUE  }, { 0x03, 0x03, 0x0F, TRUE  } }, //   03 = |  x8 | -- | -- | -- | x8 | -- | -- | -- | IIO_BIFURCATE_xxx8xxx8
  { { 0x03, 0x00, 0x00, FALSE }, { 0x00, 0x04, 0xFF, TRUE  }, { 0x04, 0x00, 0x00, TRUE  }, { 0x00, 0x04, 0xFF, TRUE  }, { 0x00, 0x04, 0xFF, TRUE }, { 0x04, 0x00, 0x00, TRUE  }, { 0x04, 0x00, 0x00, TRUE  } }, //   04 = | x16 | -- | -- | -- | -- | -- | -- | -- | IIO_BIFURCATE_xxxxxx16
  { { 0x03, 0x00, 0x00, FALSE }, { 0x00, 0x04, 0xFF, FALSE }, { 0x00, 0x00, 0x00, FALSE }, { 0x00, 0x00, 0xFF, FALSE }, { 0x02, 0x01, 0x7F, TRUE }, { 0x00, 0x00, 0x00, FALSE }, { 0x02, 0x01, 0x70, FALSE  } },//   05 = |  x8 | -- | -- | -- | x4 | -- | x2 | x2 | IIO_BIFURCATE_x2x2x4x8
  { { 0x03, 0x00, 0x00, FALSE }, { 0x00, 0x04, 0xFF, FALSE }, { 0x00, 0x00, 0x00, FALSE }, { 0x00, 0x00, 0xFF, FALSE }, { 0x02, 0x01, 0xDF, TRUE }, { 0x00, 0x00, 0x00, FALSE }, { 0x02, 0x01, 0x1F, TRUE  } }, //   06 = |  x8 | -- | -- | -- | x2 | x2 | x4 | -- | IIO_BIFURCATE_x4x2x2x8
  { { 0x03, 0x00, 0x00, FALSE }, { 0x00, 0x04, 0xFF, FALSE }, { 0x00, 0x00, 0x00, FALSE }, { 0x00, 0x00, 0xFF, FALSE }, { 0x01, 0x02, 0xF7, TRUE }, { 0x00, 0x00, 0x00, FALSE }, { 0x01, 0x02, 0x07, TRUE  } }, //   07 = |  x4 | -- | x2 | x2 | x8 | -- | -- | -- | IIO_BIFURCATE_x8x2x2x4
  { { 0x03, 0x00, 0x00, FALSE }, { 0x00, 0x04, 0xFF, FALSE }, { 0x00, 0x00, 0x00, FALSE }, { 0x00, 0x00, 0xFF, FALSE }, { 0x01, 0x02, 0xFD, TRUE }, { 0x00, 0x00, 0x00, FALSE }, { 0x01, 0x02, 0x0D, TRUE  } }, //   08 = |  x2 | x2 | x4 | -- | x8 | -- | -- | -- | IIO_BIFURCATE_x8x4x2x2
  { { 0x03, 0x00, 0x00, FALSE }, { 0x00, 0x04, 0xFF, FALSE }, { 0x00, 0x00, 0x00, FALSE }, { 0x00, 0x00, 0xFF, FALSE }, { 0x00, 0x00, 0x7F, TRUE }, { 0x00, 0x00, 0x00, FALSE }, { 0x00, 0x00, 0x7F, FALSE  } },//   09 = |  x4 | -- | x4 | -- | x4 | -- | x2 | x2 | IIO_BIFURCATE_x2x2x4x4x4
  { { 0x03, 0x00, 0x00, FALSE }, { 0x00, 0x04, 0xFF, FALSE }, { 0x00, 0x00, 0x00, FALSE }, { 0x00, 0x00, 0xFF, FALSE }, { 0x00, 0x00, 0xDF, TRUE }, { 0x00, 0x00, 0x00, FALSE }, { 0x00, 0x00, 0x1F, TRUE  } }, //   0A = |  x4 | -- | x4 | -- | x2 | x2 | x4 | -- | IIO_BIFURCATE_x4x2x2x4x4
  { { 0x03, 0x00, 0x00, FALSE }, { 0x00, 0x04, 0xFF, FALSE }, { 0x00, 0x00, 0x00, FALSE }, { 0x00, 0x00, 0xFF, FALSE }, { 0x00, 0x00, 0xF7, TRUE }, { 0x00, 0x00, 0x00, FALSE }, { 0x00, 0x00, 0x37, TRUE  } }, //   0B = |  x4 | -- | x2 | x2 | x4 | -- | x4 | -- | IIO_BIFURCATE_x4x4x2x2x4
  { { 0x03, 0x00, 0x00, FALSE }, { 0x00, 0x04, 0xFF, FALSE }, { 0x00, 0x00, 0x00, FALSE }, { 0x00, 0x00, 0xFF, FALSE }, { 0x00, 0x00, 0xFD, TRUE }, { 0x00, 0x00, 0x00, FALSE }, { 0x04, 0x00, 0x3D, TRUE } },  //   0C = |  x2 | x2 | x4 | -- | x4 | -- | x4 | -- | IIO_BIFURCATE_x4x4x4x2x2
  { { 0x03, 0x00, 0x00, FALSE }, { 0x00, 0x04, 0xFF, FALSE }, { 0x00, 0x00, 0x00, FALSE }, { 0x00, 0x00, 0xFF, FALSE }, { 0x02, 0x01, 0x5F, TRUE }, { 0x00, 0x00, 0x00, FALSE }, { 0x02, 0x01, 0x50, FALSE  } },//   0D = |  x8 | -- | -- | -- | x2 | x2 | x2 | x2 | IIO_BIFURCATE_x2x2x2x2x8
  { { 0x03, 0x00, 0x00, FALSE }, { 0x00, 0x04, 0xFF, FALSE }, { 0x00, 0x00, 0x00, FALSE }, { 0x00, 0x00, 0xFF, FALSE }, { 0x01, 0x02, 0xF5, TRUE }, { 0x00, 0x00, 0x00, FALSE }, { 0x01, 0x02, 0x05, TRUE } },  //   0E = |  x2 | x2 | x2 | x2 | x8 | -- | -- | -- | IIO_BIFURCATE_x8x2x2x2x2
  { { 0x03, 0x00, 0x00, FALSE }, { 0x00, 0x04, 0xFF, FALSE }, { 0x00, 0x00, 0x00, FALSE }, { 0x00, 0x00, 0xFF, FALSE }, { 0x00, 0x00, 0x5F, TRUE }, { 0x00, 0x00, 0x00, FALSE }, { 0x00, 0x00, 0x5C, FALSE  } },//   0F = |  x4 | -- | x4 | -- | x2 | x2 | x2 | x2 | IIO_BIFURCATE_x2x2x2x2x4x4
  { { 0x03, 0x00, 0x00, FALSE }, { 0x00, 0x04, 0xFF, FALSE }, { 0x00, 0x00, 0x00, FALSE }, { 0x00, 0x00, 0xFF, FALSE }, { 0x00, 0x00, 0x77, TRUE }, { 0x00, 0x00, 0x00, FALSE }, { 0x00, 0x00, 0x74, FALSE  } },//   10 = |  x4 | -- | x2 | x2 | x4 | -- | x2 | x2 | IIO_BIFURCATE_x2x2x4x2x2x4
  { { 0x03, 0x00, 0x00, FALSE }, { 0x00, 0x04, 0xFF, FALSE }, { 0x00, 0x00, 0x00, FALSE }, { 0x00, 0x00, 0xFF, FALSE }, { 0x00, 0x00, 0x7D, TRUE }, { 0x00, 0x00, 0x00, FALSE }, { 0x04, 0x00, 0x00, FALSE } }, //   11 = |  x2 | x2 | x4 | -- | x4 | -- | x2 | x2 | IIO_BIFURCATE_x2x2x4x4x2x2
  { { 0x03, 0x00, 0x00, FALSE }, { 0x00, 0x04, 0xFF, FALSE }, { 0x00, 0x00, 0x00, FALSE }, { 0x00, 0x00, 0xFF, FALSE }, { 0x00, 0x00, 0xD7, TRUE }, { 0x00, 0x00, 0x00, FALSE }, { 0x00, 0x00, 0x17, TRUE  } }, //   12 = |  x4 | -- | x2 | x2 | x2 | x2 | x4 | -- | IIO_BIFURCATE_x4x2x2x2x2x4
  { { 0x03, 0x00, 0x00, FALSE }, { 0x00, 0x04, 0xFF, FALSE }, { 0x00, 0x00, 0x00, FALSE }, { 0x00, 0x00, 0xFF, FALSE }, { 0x00, 0x00, 0xDD, TRUE }, { 0x00, 0x00, 0x00, FALSE }, { 0x00, 0x00, 0x1D, TRUE } },  //   13 = |  x2 | x2 | x4 | -- | x2 | x2 | x4 | -- | IIO_BIFURCATE_x4x2x2x4x2x2
  { { 0x03, 0x00, 0x00, FALSE }, { 0x00, 0x04, 0xFF, FALSE }, { 0x00, 0x00, 0x00, FALSE }, { 0x00, 0x00, 0xFF, FALSE }, { 0x00, 0x00, 0xF5, TRUE }, { 0x00, 0x00, 0x00, FALSE }, { 0x00, 0x00, 0x35, TRUE } },  //   14 = |  x2 | x2 | x2 | x2 | x4 | -- | x4 | -- | IIO_BIFURCATE_x4x4x2x2x2x2
  { { 0x03, 0x00, 0x00, FALSE }, { 0x00, 0x04, 0xFF, FALSE }, { 0x00, 0x00, 0x00, FALSE }, { 0x00, 0x00, 0xFF, FALSE }, { 0x00, 0x00, 0x57, TRUE }, { 0x00, 0x00, 0x00, FALSE }, { 0x00, 0x00, 0x54, FALSE } }, //   15 = |  x4 | -- | x2 | x2 | x2 | x2 | x2 | x2 | IIO_BIFURCATE_x2x2x2x2x2x2x4
  { { 0x03, 0x00, 0x00, FALSE }, { 0x00, 0x04, 0xFF, FALSE }, { 0x00, 0x00, 0x00, FALSE }, { 0x00, 0x00, 0xFF, FALSE }, { 0x00, 0x00, 0x5D, TRUE }, { 0x00, 0x00, 0x00, FALSE }, { 0x04, 0x00, 0x00, FALSE } }, //   16 = |  x2 | x2 | x4 | -- | x2 | x2 | x2 | x2 | IIO_BIFURCATE_x2x2x2x2x4x2x2
  { { 0x03, 0x00, 0x00, FALSE }, { 0x00, 0x04, 0xFF, FALSE }, { 0x00, 0x00, 0x00, FALSE }, { 0x00, 0x00, 0xFF, FALSE }, { 0x00, 0x00, 0x75, TRUE }, { 0x00, 0x00, 0x00, FALSE }, { 0x04, 0x00, 0x00, FALSE } }, //   17 = |  x2 | x2 | x2 | x2 | x4 | -- | x2 | x2 | IIO_BIFURCATE_x2x2x4x2x2x2x2
  { { 0x03, 0x00, 0x00, FALSE }, { 0x00, 0x04, 0xFF, FALSE }, { 0x00, 0x00, 0x00, FALSE }, { 0x00, 0x00, 0xFF, FALSE }, { 0x00, 0x00, 0xD5, TRUE }, { 0x00, 0x00, 0x00, FALSE }, { 0x00, 0x00, 0x15, TRUE  } }, //   18 = |  x2 | x2 | x2 | x2 | x2 | x2 | x4 | -- | IIO_BIFURCATE_x4x2x2x2x2x2x2
  { { 0x03, 0x00, 0x00, FALSE }, { 0x00, 0x04, 0xFF, FALSE }, { 0x00, 0x00, 0x00, FALSE }, { 0x00, 0x00, 0xFF, FALSE }, { 0x00, 0x00, 0x55, TRUE }, { 0x00, 0x00, 0x00, FALSE }, { 0x04, 0x00, 0x00, FALSE } }, //   19 = |  x2 | x2 | x2 | x2 | x2 | x2 | x2 | x2 | IIO_BIFURCATE_x2x2x2x2x2x2x2x2
};


/**
 @brief Convert stack mode to text name for debug purpose.

 @param[in] Mode - Stack mode enum to convert to text

 @return Text name of the mode is returned.
**/
CHAR8*
IioDbgStackModeName (
  PI5_MODES Mode
  )
{
  static CHAR8 *Enum2Name[] = {"DMI", "IAL", "GEN4_ONLY", "GEN5_ONLY", "GEN5_AND_GEN4", "NTB_GEN4", "NTB", "UNKNOWN"};

  if (Mode >= NELEMENTS(Enum2Name)) {
    Mode = NELEMENTS(Enum2Name) - 1;
  }
  return Enum2Name[Mode];
}


/**
 @brief Convert bifurcation to text name for debug purpose.

 @param[in] Bifurcation - Bifurcation enum to convert to text.

 @return Text name of the mode is returned.
**/
CHAR8*
IioDbgBifurcationName (
  IN  UINT8  Bifurcation
  )
{
  static CHAR8 *Enum2Name[] = {"x4x4x4x4",
                               "x4x4xxx8",
                               "xxx8x4x4",
                               "xxx8xxx8",
                               "xxxxxx16",
                               "x2x2x4x8",
                               "x4x2x2x8",
                               "x8x2x2x4",
                               "x8x4x2x2",
                               "x2x2x4x4x4",
                               "x4x2x2x4x4",
                               "x4x4x2x2x4",
                               "x4x4x4x2x2",
                               "x2x2x2x2x8",
                               "x8x2x2x2x2",
                               "x2x2x2x2x4x4",
                               "x2x2x4x2x2x4",
                               "x2x2x4x4x2x2",
                               "x4x2x2x2x2x4",
                               "x4x2x2x4x2x2",
                               "x4x4x2x2x2x2",
                               "x2x2x2x2x2x2x4",
                               "x2x2x2x2x4x2x2",
                               "x2x2x4x2x2x2x2",
                               "x4x2x2x2x2x2x2",
                               "x2x2x2x2x2x2x2x2",
                               "UNKNOWN"};

  if (Bifurcation >= NELEMENTS(Enum2Name)) {
    Bifurcation = NELEMENTS(Enum2Name) - 1;
  }
  return Enum2Name[Bifurcation];
}


/**
   Gets the Iou config for given stack from setup

 @param[in] IioGlobalData - Pointer to IioGlobalData
 @param[in] IioIndex      - IIO instance index (usually socket)
 @param[in] Stack         - IIO stack index

 @return UINT8    Iou config
**/
UINT8
GetIouConfig (
  IIO_GLOBALS             *IioGlobalData,
  UINT8                   IioIndex,
  UINT8                   Stack
  )
{
  UINT8 MaxPortNumPerIou;
  UINT8 IouConfig;

  MaxPortNumPerIou = GetMaxPortNumPerIou();

  if (IIO_STACK0 == Stack) {
    IouConfig = CheckBifurcationMapPort (IioGlobalData, IioIndex, PORT_0_INDEX);
  } else {
    IouConfig = CheckBifurcationMapPort (IioGlobalData, IioIndex, (Stack - 1) * MaxPortNumPerIou + 1);
    if (IouConfig >= ARRAY_SIZE (ModesConfigTable)) {
      IIO_D_ERR ("[%d][%d]: %a: setup bifurcation data 0x%x not correct\n",
                 IioIndex, Stack, __FUNCTION__, IouConfig);
      ASSERT (IouConfig < ARRAY_SIZE (ModesConfigTable));
      return IIO_BIFURCATE_xxxxxxxx;
    }
  }
  return IouConfig;
}


/**
  The purpose of this routine is to do the necessary work
  required to set the PCS MUX register in orde to choice
  proper IP lanes.

 @param[in] IioGlobalData - Pointer to IioGlobalData
 @param[in] IioIndex      - Index to Iio
 @param[in] Stack         - Stack index
 @param[in] IouConfig     - Iou config from setup
 @param[in] StackMode     - Current stack mode

 @return Void.
**/
VOID
SetPcsMux (
  IN IIO_GLOBALS *IioGlobalData,
  IN UINT8        IioIndex,
  IN UINT8        Stack,
  IN UINT8        IouConfig,
  IN PI5_MODES    StackMode
  )
{
  CSR_PCS_MUX_SEL_SB_FBLP_STRUCT    PcsMuxData;
  CSR_PCS_MUX_SEL_SB_FBLP_STRUCT    PcsMuxBkp;
  UINT8                             PortIndex;

  if (Stack >= GetMaxIouNumPerSocket () + 1) { // if (Stack > PCIE + DMI)
    ASSERT (Stack < GetMaxIouNumPerSocket () + 1);
    return;
  }

  if (IouConfig >= NELEMENTS (ModesConfigTable) ||
      StackMode >= NELEMENTS (ModesConfigTable[IouConfig]) ||
      !ModesConfigTable[IouConfig][StackMode].Valid) {

    IIO_D_ERR ("[%d.%d] Bifurcation %a (%d) is not valid for stack mode %a (%d) - ignoring\n",
               IioIndex, Stack, IioDbgBifurcationName (IouConfig), IouConfig,
               IioDbgStackModeName (StackMode), StackMode);
    return;
  }

  if (IIO_STACK0 == Stack) {
    PortIndex = 0;
  } else {
    PortIndex = PORT_A_INDEX + ((Stack - 1) * GetMaxPortNumPerIou ());
  }

  PcsMuxData.Data = IioReadCpuCsr16 (IioIndex, PortIndex, CSR_PCS_MUX_SEL_SB_FBLP_REG);
  PcsMuxBkp.Data = PcsMuxData.Data;
  PcsMuxData.Bits.lane_sel = ModesConfigTable[IouConfig][StackMode].Pcs;
  PcsMuxData.Bits.lane_sel_val = 1;
  IioWriteCpuCsr16 (IioIndex, PortIndex, CSR_PCS_MUX_SEL_SB_FBLP_REG, PcsMuxData.Data);
  IIO_D_DBG ("[%d.%d] %a: CSR_PCS_MUX_SEL_SB_FBLP_REG 0x%04X -> 0x%04X -> 0x%04X\n",
             IioIndex, PortIndex, __FUNCTION__, PcsMuxBkp.Data, PcsMuxData.Data,
             IioReadCpuCsr16 (IioIndex, PortIndex, CSR_PCS_MUX_SEL_SB_FBLP_REG));
} // SetPcsMux()

/**
  Set or clear RXTERMENCTL register to enable proper controller configuration.

  @param[in] IioGlobalData   Pointer to IioGlobalData
  @param[in] IioIndex        IIO index (socket number)
  @param[in] EnableOverride  Enable the register settings

**/
VOID
PcieRxControlOverride (
  IIO_GLOBALS    *IioGlobalData,
  UINT8          IioIndex,
  BOOLEAN        EnableOverride
)
{
  UINT8                              PortIndex;
  UINT8                              StackIndex;
  RXTERMENCTL_SB_FBLP_STRUCT         RxTermEnCtl;
  ACRDPR5_N0_SB_PRVT_FBLP_STRUCT     AcRdPr5;
  ACWRPR5_N0_SB_PRVT_FBLP_STRUCT     AcWrPr5;
  ACRDPR5_N0_SB_PRVT_FBLP_STRUCT     OrgAcRdPr5;
  ACWRPR5_N0_SB_PRVT_FBLP_STRUCT     OrgAcWrPr5;

  if (IsSiliconWorkaroundEnabled ("S14010687317")) {
    for (StackIndex = 0; StackIndex < MAX_IIO_STACK_SPR; StackIndex++) {
      if (IioGlobalData->IioVar.IioVData.StackPersonality[IioIndex][StackIndex] != TYPE_UBOX_IIO) {
        continue;
      }

      PortIndex = GetPortIndexbyStack (StackIndex, IioPortA);
      if (PortIndex == PORT_0_INDEX) {
        continue;
      }

      OrgAcRdPr5.Data = IioReadCpuCsr32 (IioIndex, StackIndex, ACRDPR5_N0_SB_PRVT_FBLP_REG);
      OrgAcWrPr5.Data = IioReadCpuCsr32 (IioIndex, StackIndex, ACWRPR5_N0_SB_PRVT_FBLP_REG);
      AcRdPr5.Data = OrgAcWrPr5.Data;
      AcWrPr5.Data = OrgAcWrPr5.Data;
      //
      // Add HOSTIA_BOOT_SAI to FBLP DFX1
      //
      AcRdPr5.Bits.acpv |= HOSTIA_BOOT_SAI;
      AcWrPr5.Bits.acpv |= HOSTIA_BOOT_SAI;
      IioWriteCpuCsr32 (IioIndex, StackIndex, ACRDPR5_N0_SB_PRVT_FBLP_REG, AcRdPr5.Data);
      IioWriteCpuCsr32 (IioIndex, StackIndex, ACWRPR5_N0_SB_PRVT_FBLP_REG, AcWrPr5.Data);

      if (EnableOverride) {
        //
        // Set RXTERMENCTL immediately after PPD0.ilinktrn0 is set.
        //
        RxTermEnCtl.Data = IioReadCpuCsr32 (IioIndex, PortIndex, RXTERMENCTL_SB_FBLP_REG);
        RxTermEnCtl.Bits.rxtermen = 0xFFFF;
        RxTermEnCtl.Bits.ovren = 1;
        IioWriteCpuCsr32 (IioIndex, PortIndex, RXTERMENCTL_SB_FBLP_REG, RxTermEnCtl.Data);
      } else {
        //
        // Clear RXTERMENCTL prior to programming PPBIFCTL_PRIV
        //
        RxTermEnCtl.Data = IioReadCpuCsr32 (IioIndex, PortIndex, RXTERMENCTL_SB_FBLP_REG);
        RxTermEnCtl.Bits.rxtermen = 0;
        RxTermEnCtl.Bits.ovren = 0;
        IioWriteCpuCsr32 (IioIndex, PortIndex, RXTERMENCTL_SB_FBLP_REG, RxTermEnCtl.Data);
      }
      //
      // Restore FBLP DFX1
      //
      IioWriteCpuCsr32 (IioIndex, StackIndex, ACRDPR5_N0_SB_PRVT_FBLP_REG, OrgAcRdPr5.Data);
      IioWriteCpuCsr32 (IioIndex, StackIndex, ACWRPR5_N0_SB_PRVT_FBLP_REG, OrgAcWrPr5.Data);
    }
  }
}

/**

   The purpose of this routine is to do the necessary work
   required to enable the PCIE Ports. In addition this routine could and should be be
   used to do any othe initialization required before PCI
   Express training.

    @param IioGlobalData - Pointer to IioGlobalData
    @param IioIndex      - Index to Iio

    @retval None

**/
VOID
PcieLinkTrainingInit (
  IIO_GLOBALS                         *IioGlobalData,
  UINT8                               IioIndex
  )
{
  UINT8                             PortIndex;
  UINT8                             NtbPortIndex;
  UINT8                             MaxPortNumberPerSocket;
  PPD0_IIO_PCIEGen4_STRUCT          Ppd0Gen4Reg;
  FBLP_PPD0_SB_FBLP_STRUCT          Ppd0FblpReg;
  PCI_ROOT_BRIDGE_PCI_ADDRESS       PciDev;
  UINTN                             Mbar0;
  EFI_STATUS                        Status;

  IIO_D_DBG ("%a at device scanning...\n", __FUNCTION__);

  if (IioGlobalData->SetupData.DelayBeforePCIeLinkTraining != 0) {
    //
    // apply delay before link training requested in setup menu
    //
    IioStall (IioGlobalData->SetupData.DelayBeforePCIeLinkTraining);
  }

  MaxPortNumberPerSocket = GetMaxPortNumPerSocket ();

  for (PortIndex = 1; PortIndex < MaxPortNumberPerSocket; PortIndex++) {
    if (!IioVData_ConfigurePciePort(IioGlobalData, IioIndex, PortIndex)) {
      continue;
    }

    if (!IioIsPortPresent (IioIndex, PortIndex)) {
      continue;
    }

    IIO_D_DBG ("%a PortIndex = 0x%x\n", __FUNCTION__, PortIndex);

    if (IioPortControllerGen (IioGlobalData, IioIndex, PortIndex) == IIO_GEN5) {
      //
      // Start link training for Gen5 RP
      //
      Ppd0FblpReg.Data = IioReadCpuCsr32 (IioIndex, PortIndex, FBLP_PPD0_SB_FBLP_REG);
      Ppd0FblpReg.Bits.ilinktrn0 = 1;
      IioWriteCpuCsr32 (IioIndex, PortIndex, FBLP_PPD0_SB_FBLP_REG, Ppd0FblpReg.Data);
    }

    if (GetPciePortOwnership (IioIndex, PortIndex) == NTB_PORT_DEF_NTB_NTB) {
      Status = GetNtbPortPerPortIndex (PortIndex, &NtbPortIndex);
      if (Status != EFI_SUCCESS) {
        continue;
      }
      NtbSetMPL (IioGlobalData, IioIndex, PortIndex);
      //
      //  Skip link training for NTB ports if NtbLinkBiosTrainEn setup knob is disabled
      //
      if (!IioGlobalData->SetupData.NtbLinkBiosTrainEn) {
        continue;
      }
      //
      // Start link training for NTB
      //
      GetPciePortBusDeviceFunction (IioIndex, PortIndex, &PciDev);
      //
      // Make sure its NTB due to possibility of NTB mix mode
      //
      PciDev.Device = IioGlobalData->IioVar.PreLinkData.PcieInfo.PortInfo[PortIndex].NtbDevice;
      PciDev.Function = IioGlobalData->IioVar.PreLinkData.PcieInfo.PortInfo[PortIndex].NtbFunction;
      Mbar0 = GetAddressFromMBar01 (IioIndex, PciDev);
      if (Mbar0 == 0) {
        IIO_D_PCIWARN ("No BAR assigned, skipping NTB link training\n");
        continue;
      }
      EnableMBarAccess (IioIndex, PciDev);
      Ppd0Gen4Reg.Data = IioMmioRead32 (Mbar0 + NTB_MBAR01_PPD0_OFFSET);
      Ppd0Gen4Reg.Bits.ilinktrn0 = 1;
      IioMmioWrite32 (Mbar0 + NTB_MBAR01_PPD0_OFFSET, Ppd0Gen4Reg.Data);
      DisableMBarAccess (IioIndex, PciDev);
    } else {
      //
      // Start link training for Gen4 RP
      //
      if (IioPortControllerGen (IioGlobalData, IioIndex, PortIndex) == IIO_GEN4) {
        Ppd0Gen4Reg.Data = IioReadCpuCsr32 (IioIndex, PortIndex, PPD0_IIO_PCIEGen4_REG);
        Ppd0Gen4Reg.Bits.ilinktrn0 = 1;
        IioWriteCpuCsr32 (IioIndex, PortIndex, PPD0_IIO_PCIEGen4_REG, Ppd0Gen4Reg.Data);
      }
    }
  }
  PcieRxControlOverride (IioGlobalData, IioIndex, TRUE);
}

/**
  The purpose of this routine is to do the necessary work
  required to configure the PCIe Port Bifurcation Control
  register in order to access PCIe Configuration Space.

 @param[in] IioGlobalData - Pointer to IioGlobalData
 @param[in] IioIndex      - Index to Iio

 @return Void.
**/
VOID
PcieDynamicPortSubdivision (
  IIO_GLOBALS             *IioGlobalData,
  UINT8                   IioIndex
  )
{
  PPBIFCTL_PRIV_SB_PRVT_PCIE_G4_STRUCT        PpbifctlGen4;
  PPBIFCTL_PRIV_SB_PRVT_FBLP_STRUCT           PpbifctlFblp;
  UINT32                                      Gen4Bkp;
  UINT32                                      FblpBkp;
  UINT8                                       MaxIioStack;
  UINT8                                       Stack;
  PI5_MODES                                   StackMode;
  UINT8                                       IouConfig;

  MaxIioStack = GetMaxIouNumPerSocket () + 1; // PCIE + DMI

  PcieRxControlOverride (IioGlobalData, IioIndex, FALSE);

  for (Stack = 0; Stack < MaxIioStack; Stack++) {
    if (!(IioGlobalData->IioVar.IioVData.StackPersonality[IioIndex][Stack] == TYPE_UBOX_IIO)) {
      continue;
    }
    StackMode = GetStackMode (IioGlobalData, IioIndex, Stack);
    IouConfig = GetIouConfig (IioGlobalData, IioIndex, Stack);
    if (IouConfig >= NELEMENTS (ModesConfigTable) ||
        StackMode >= NELEMENTS (ModesConfigTable[IouConfig]) ||
        !ModesConfigTable[IouConfig][StackMode].Valid) {

      IIO_D_ERR ("[%d.%d] Bifurcation %a (%d) is not valid for stack mode %a (%d) - ignoring\n",
                 IioIndex, Stack, IioDbgBifurcationName (IouConfig), IouConfig,
                 IioDbgStackModeName (StackMode), StackMode);
      continue;
    }
    SetPcsMux (IioGlobalData, IioIndex, Stack, IouConfig, StackMode);

    if (IIO_STACK0 != Stack) {

      //
      // Program Gen4 bifurcation to the controller
      //
      PpbifctlGen4.Data = Gen4Bkp = IioReadCpuCsr32 (IioIndex, Stack, PPBIFCTL_PRIV_SB_PRVT_PCIE_G4_REG);
      PpbifctlGen4.Bits.bifctl0 = ModesConfigTable[IouConfig][StackMode].Gen4;
      PpbifctlGen4.Bits.cl = 1;
      IioWriteCpuCsr32 (IioIndex, Stack, PPBIFCTL_PRIV_SB_PRVT_PCIE_G4_REG, PpbifctlGen4.Data);
      IIO_D_DBG ("[%d.%d] %a: PPBIFCTL_PRIV_SB_PRVT_PCIE_G4_REG 0x%04X -> 0x%04X -> 0x%04X\n",
                 IioIndex, Stack, __FUNCTION__, Gen4Bkp, PpbifctlGen4.Data,
                 IioReadCpuCsr32 (IioIndex, Stack, PPBIFCTL_PRIV_SB_PRVT_PCIE_G4_REG));

      //
      // Program Gen5/CXL bifurcation to FBLP if not in Gen4 only mode
      //
      if (ModesConfigTable[IouConfig][StackMode].Pcs != 0) {
        PpbifctlFblp.Data = FblpBkp = IioReadCpuCsr32 (IioIndex, Stack, PPBIFCTL_PRIV_SB_PRVT_FBLP_REG);
        PpbifctlFblp.Bits.bifctl0 = ModesConfigTable[IouConfig][StackMode].Gen5;
        PpbifctlFblp.Bits.cl = 1;
        IioWriteCpuCsr32 (IioIndex, Stack, PPBIFCTL_PRIV_SB_PRVT_FBLP_REG, PpbifctlFblp.Data);
        IIO_D_DBG ("[%d.%d] %a: PPBIFCTL_PRIV_SB_PRVT_FBLP_REG 0x%04X -> 0x%04X -> 0x%04X\n",
                   IioIndex, Stack, __FUNCTION__, FblpBkp, PpbifctlFblp.Data,
                   IioReadCpuCsr32 (IioIndex, Stack, PPBIFCTL_PRIV_SB_PRVT_FBLP_REG));
      }
    }
  }
}

