/** @file
  Iio Gen4 initialization.

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

#include <Base.h>
#include <Uefi.h>
#include <Library/DebugLib.h>

#include <Library/IioAccessLib.h>
#include <IioPlatformData.h>
#include <IioSetupDefinitions.h>
#include <Library/IioInitLib.h>
#include <RcRegs.h>
#include <Library/IioDataHubLib.h>

static UINT8 MaxGen4EqRegs[] = {0,
                                16, 4, 8, 4,
                                16, 4, 8, 4,
                                16, 4, 8, 4,
                                16, 4, 8, 4,
                                16, 4, 8, 4,
                                16, 4, 8, 4,
                                16, 4, 8, 4,
                                16, 4, 8, 4,
                                16, 4, 8, 4,
                                16, 4, 8, 4};

static UINTN BifurcationMap[5][4] = {{ 4, 4, 4, 4},
                                     { 8, 0, 4, 4},
                                     { 4, 4, 8, 0},
                                     { 8, 0, 8, 0},
                                     {16, 0, 0, 0} };


/**
  Overrides the G4LANEEQCTL registers for PCIE

  @param IioGlobalData      Pointer to IIO_GLOBLAS structure
  @param IioIndex           Socket Index
  @param PortIndex          Port Index
**/
VOID
PresetOverrideGen4 (
  IN  IIO_GLOBALS  *IioGlobalData,
  IN  UINT8       IioIndex,
  IN  UINT8       PortIndex
  )
{
  G4LANEEQCTL_0_IIO_PCIEGen4_STRUCT  LnEq;
  UINT8                              Ctrl;
  UINT8                              MaxPortNumberPerSocket;
  IIO_CONFIG                         *SetupData;
  UINT8                              SetupIndex;

  SetupData = GetIioConfig();
  MaxPortNumberPerSocket = GetMaxPciePortNumberPerSocket (IioIndex);
  SetupIndex = ((IioIndex) * MaxPortNumberPerSocket) + PortIndex;

  DEBUG ((DEBUG_ERROR, "%a: Preset: DnTx: %d\nUpTxPreset: %d\n",
    __FUNCTION__,
    SetupData->IioPcieConfig.DfxDnTxPreset[SetupIndex],
    SetupData->IioPcieConfig.DfxUpTxPreset[SetupIndex]
    ));

  ASSERT (MaxPortNumberPerSocket <= ARRAY_SIZE (MaxGen4EqRegs));
  for (Ctrl = 0; Ctrl < MaxGen4EqRegs[PortIndex]; Ctrl++) {
    LnEq.Data = IioReadCpuCsr8 (
      IioIndex,
      PortIndex,
      G4LANEEQCTL_0_IIO_PCIEGen4_REG + (Ctrl * sizeof (G4LANEEQCTL_0_IIO_PCIEGen4_STRUCT))
      );

    if (SetupData->IioPcieConfig.DfxDnTxPreset[SetupIndex] != DFX_TX_PRESET_AUTO) {
      LnEq.Bits.dptp = SetupData->IioPcieConfig.DfxDnTxPreset[SetupIndex];
    } else {
      LnEq.Bits.dptp = 5;
    }

    if (SetupData->IioPcieConfig.DfxUpTxPreset[SetupIndex] != DFX_TX_PRESET_AUTO) {
      LnEq.Bits.uptp = SetupData->IioPcieConfig.DfxUpTxPreset[SetupIndex];
    }

    IioWriteCpuCsr8 (
      IioIndex,
      PortIndex,
      G4LANEEQCTL_0_IIO_PCIEGen4_REG + (Ctrl * sizeof (G4LANEEQCTL_0_IIO_PCIEGen4_STRUCT)),
      LnEq.Data
      );
  }
}

/**
  This function will program GEN4TXCOEFF[0-15] registers
  with the required value.

  @param IioIndex      Socket Index
  @param PortIndex     Port Index
  @param Precursor     Precursor value to be program
  @param Cursor        Cursor value to be program
  @param Postcursor    Postcursor value to be program
**/
VOID
Gen4ProgPrivTxEqCoefficients (
  IN  UINT8        IioIndex,
  IN  UINT8        PortIndex,
  IN  UINT32       Precursor,
  IN  UINT32       Cursor,
  IN  UINT32       Postcursor
  )
{
  //
  // To be defined for EGS
  //
}

/**
  This routine will program GEN4FARTXCOEFF[0-15]
  registers with the required values.

  @param IioIndex      Socket Index
  @param PortIndex     Port Index
  @param Precursor     Precursor value to be program
  @param Cursor        Cursor value to be program
  @param Postcursor    Postcursor value to be program
**/
VOID
Gen4ProgPrivRmtTxEqCoefficients (
  IN  UINT8        IioIndex,
  IN  UINT8        PortIndex,
  IN  UINT32       Precursor,
  IN  UINT32       Cursor,
  IN  UINT32       Postcursor
  )
{
  //
  // To be defined for EGS
  //
}

/**
    This routine will program G4EQEVALCTL and G4EQEVALCTL2 (gen3)
    registers with the required values.

    @param IioIndex        Socket Index
    @param PortIndex       Port Index
**/
VOID
Gen4ProgEqEval (
  IN  UINT8        IioIndex,
  IN  UINT8        PortIndex
  )
{
  //
  // To be defined for EGS
  //
}

/**
    The purpose of this function is to apply all the
    Gen3 override required in the link

    @param IioGlobalData  Pointer to IIO_GLOBALS structure
    @param IioIndex       Socket index
    @param PortIndex      Port index
**/
VOID
Gen3PrelinkOverride (
  IN  IIO_GLOBALS                       *IioGlobalData,
  IN  UINT8                             IioIndex,
  IN  UINT8                             PortIndex
  )
{
  return;
}

/**
    The purpose of this function is to apply all the
    Gen4 override required in the link

    @param IioGlobalData  Pointer to IIO_GLOBALS structure
    @param IioIndex       Socket index
    @param PortIndex      Port index
**/
VOID
Gen4PrelinkOverride (
  IN  IIO_GLOBALS                       *IioGlobalData,
  IN  UINT8                             IioIndex,
  IN  UINT8                             PortIndex
  )
{
  UINT8                             DfxGen4Mode;
  UINT8                             MaxPortNumberPerSocket;
  IIO_CONFIG                        *SetupData;
  UINT8                             SetupIndex;
  UINT8                             ClusterPort;
  UINT8                             Bifurcation;

  SetupData = GetIioConfig ();
  MaxPortNumberPerSocket = GetMaxPciePortNumberPerSocket (IioIndex);
  SetupIndex = ((IioIndex) * MaxPortNumberPerSocket) + PortIndex;
  DfxGen4Mode = SetupData->IioPcieConfig.DfxGen4OverrideMode[SetupIndex];
  ClusterPort = GetPciePortSuperClusterPort (PortIndex);
  Bifurcation = CheckBifurcationMapPort(IioGlobalData, IioIndex, PortIndex);

  if((SetupData->TXEQ[SetupIndex]) && (DfxGen4Mode == DFX_TXEQ_OVERRIDE_MODE_PHY)) {
    DfxGen4Mode = DFX_TXEQ_OVERRIDE_MODE_ALTERNATE_TXEQ;
  }

  // EnableBarAccess (IioIndex, PortIndex);

  if (BifurcationMap[Bifurcation][PortIndex - ClusterPort] > 0) {
    Gen4ProgEqEval (IioIndex, PortIndex);
  }

  switch (DfxGen4Mode) {
  case DFX_TXEQ_OVERRIDE_MODE_MANUAL:
    DEBUG ((DEBUG_ERROR, "%a: [%d] Manual override mode for port %a (%d)\n",
      __FUNCTION__,
      IioIndex,
      IIO_PORT_LABEL (PortIndex),
      PortIndex
      ));

    DEBUG ((DEBUG_ERROR, "%a: Manual override mode TxCoeff:\n Precursor: %d\n Cursor: %d\n Postcursor: %d\n",
      __FUNCTION__,
      SetupData->IioPcieConfig.DfxGen4ManualPh2_Precursor[SetupIndex],
      SetupData->IioPcieConfig.DfxGen4ManualPh2_Cursor[SetupIndex],
      SetupData->IioPcieConfig.DfxGen4ManualPh2_Postcursor[SetupIndex]
      ));


    Gen4ProgPrivTxEqCoefficients (IioIndex, PortIndex,
      SetupData->IioPcieConfig.DfxGen4ManualPh2_Precursor[SetupIndex],
      SetupData->IioPcieConfig.DfxGen4ManualPh2_Cursor[SetupIndex],
      SetupData->IioPcieConfig.DfxGen4ManualPh2_Postcursor[SetupIndex]
      );

    DEBUG ((DEBUG_ERROR, "%a: Manual override mode FarTxCoeff:\n Precursor: %d\n Cursor: %d\n Postcursor: %d\n",
      __FUNCTION__,
      SetupData->IioPcieConfig.DfxGen4ManualPh3_Precursor[SetupIndex],
      SetupData->IioPcieConfig.DfxGen4ManualPh3_Cursor[SetupIndex],
      SetupData->IioPcieConfig.DfxGen4ManualPh3_Postcursor[SetupIndex]
      ));

    Gen4ProgPrivRmtTxEqCoefficients (IioIndex, PortIndex,
      SetupData->IioPcieConfig.DfxGen4ManualPh3_Precursor[SetupIndex],
      SetupData->IioPcieConfig.DfxGen4ManualPh3_Cursor[SetupIndex],
      SetupData->IioPcieConfig.DfxGen4ManualPh3_Postcursor[SetupIndex]
      );
    break;
  case DFX_TXEQ_OVERRIDE_MODE_TEST_CARD:
    if (SetupData->IioPcieConfig.DfxGen4TestCard[SetupIndex] == DFX_TXEQ_TEST_CARD_LAGUNA) {
      DEBUG ((DEBUG_ERROR,"%a: override for Gen4 Laguna Test Card(SKT=%d, PORT=%a (%d))\n",
        __FUNCTION__,
        IioIndex,
        IIO_PORT_LABEL (PortIndex),
        PortIndex
        ));

      Gen4ProgPrivTxEqCoefficients (IioIndex, PortIndex,
        0x4,    //precursor
        0x18,   //cursor
        0x4     //postcursor
        );

    } else if (SetupData->IioPcieConfig.DfxGen4TestCard[SetupIndex] == DFX_TXEQ_TEST_CARD_NTB) {
        DEBUG ((DEBUG_ERROR,"%a: override for Gen4 NTB Test Card(SKT=%d, PORT=%a (%d))\n",
          __FUNCTION__,
          IioIndex,
          IIO_PORT_LABEL (PortIndex),
          PortIndex
          ));
    }
    break;
  case DFX_TXEQ_OVERRIDE_MODE_ALTERNATE_TXEQ:
    // we don't have alternate TXEQ currently
    // fallthrough
    break;
  case DFX_TXEQ_OVERRIDE_MODE_PHY:
    // fallthrough
  default:
    DEBUG ((DEBUG_ERROR, "%a: Uniphy mode (SKT=%d, PORT=%a(%d))\n",
      __FUNCTION__,
      IioIndex,
      IIO_PORT_LABEL (PortIndex),
      PortIndex
      ));

    //
    //Do not Override original values  given by UNIPHY recipe
    //
    break;
  }

  //DisableBarAccess (IioIndex, PortIndex);

  PresetOverrideGen4 (IioGlobalData, IioIndex, PortIndex);
}
