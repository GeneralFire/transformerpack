/** @file
  This file initialize the IIO Misc Feature for SPR

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

#include <Library/IioDataHubLib.h>
#include <Library/IioAccessLib.h>
#include <RcRegs.h>
#include <Library/IioSysInfoLib.h>
#include <IioSetupDefinitions.h>
#include <Library/KtiApi.h>
#include <Library/IioMiscFeatureLib.h>
#include <Library/SiliconWorkaroundLib.h>
#include <LibraryPrivate/IioDebug.h>
#include <IioRegs.h>


UINT8 mMs2iosfChannelMapping_Dmi = (UINT8) (BIT5|BIT6|BIT7); // IIO_BIFURCATE_xxx8xxx8

UINT8 mMs2iosfChannelMapping_Ial = (UINT8) (BIT0|BIT1); // IIO_BIFURCATE_xxxxxx16

UINT8 mMs2iosfChannelMapping_Ntb = (UINT8) (BIT7); // IIO_BIFURCATE_xxxxxx16

UINT8 mMs2iosfChannelMapping_PcieGen4[] = {
  (UINT8) (BIT1|BIT3|BIT5|BIT7),  // IIO_BIFURCATE_x4x4x4x4
  (UINT8) (BIT3|BIT5|BIT7),       // IIO_BIFURCATE_x4x4xxx8
  (UINT8) (BIT1|BIT3|BIT7),       // IIO_BIFURCATE_xxx8x4x4
  (UINT8) (BIT3|BIT7),            // IIO_BIFURCATE_xxx8xxx8
  (UINT8) (BIT7),                 // IIO_BIFURCATE_xxxxxx16
};

UINT8 mMs2iosfChannelMapping_PcieGen5Gen4[] = {
  (UINT8) (BIT0|BIT1|BIT2|BIT3|BIT4|BIT5|BIT6|BIT7),  // IIO_BIFURCATE_x4x4x4x4
  (UINT8) (BIT0|BIT1|BIT4|BIT5|BIT6|BIT7),            // IIO_BIFURCATE_x4x4xxx8
  (UINT8) (BIT0|BIT1|BIT2|BIT3|BIT4|BIT5),            // IIO_BIFURCATE_xxx8x4x4
  (UINT8) (BIT0|BIT1|BIT4|BIT5),                      // IIO_BIFURCATE_xxx8xxx8
  (UINT8) (BIT0|BIT1),                                // IIO_BIFURCATE_xxxxxx16
  (UINT8) (BIT0|BIT1|BIT4|BIT5|BIT6|BIT7),            // IIO_BIFURCATE_x2x2x4x8
  (UINT8) (BIT0|BIT1|BIT4|BIT5|BIT6|BIT7),            // IIO_BIFURCATE_x4x2x2x8
  (UINT8) (BIT0|BIT1|BIT2|BIT3|BIT4|BIT5),            // IIO_BIFURCATE_x8x2x2x4
  (UINT8) (BIT0|BIT1|BIT2|BIT3|BIT4|BIT5),            // IIO_BIFURCATE_x8x4x2x2
  (UINT8) (BIT0|BIT1|BIT2|BIT3|BIT4|BIT5|BIT6|BIT7),  // IIO_BIFURCATE_x2x2x4x4x4
  (UINT8) (BIT0|BIT1|BIT2|BIT3|BIT4|BIT5|BIT6|BIT7),  // IIO_BIFURCATE_x4x2x2x4x4
  (UINT8) (BIT0|BIT1|BIT2|BIT3|BIT4|BIT5|BIT6|BIT7),  // IIO_BIFURCATE_x4x4x2x2x4
  (UINT8) (BIT0|BIT1|BIT2|BIT3|BIT4|BIT5|BIT6|BIT7),  // IIO_BIFURCATE_x4x4x4x2x2
  (UINT8) (BIT0|BIT1|BIT2|BIT3|BIT4|BIT5|BIT6|BIT7),  // IIO_BIFURCATE_x2x2x2x2x8
  (UINT8) (BIT0|BIT1|BIT2|BIT3|BIT4|BIT5|BIT6|BIT7),  // IIO_BIFURCATE_x8x2x2x2x2
  (UINT8) (BIT0|BIT1|BIT2|BIT3|BIT4|BIT5|BIT6|BIT7),  // IIO_BIFURCATE_x2x2x2x2x4x4
  (UINT8) (BIT0|BIT1|BIT2|BIT3|BIT4|BIT5|BIT6|BIT7),  // IIO_BIFURCATE_x2x2x4x2x2x4
  (UINT8) (BIT0|BIT1|BIT2|BIT3|BIT4|BIT5|BIT6|BIT7),  // IIO_BIFURCATE_x2x2x4x4x2x2
  (UINT8) (BIT0|BIT1|BIT2|BIT3|BIT4|BIT5|BIT6|BIT7),  // IIO_BIFURCATE_x4x2x2x2x2x4
  (UINT8) (BIT0|BIT1|BIT2|BIT3|BIT4|BIT5|BIT6|BIT7),  // IIO_BIFURCATE_x4x2x2x4x2x2
  (UINT8) (BIT0|BIT1|BIT2|BIT3|BIT4|BIT5|BIT6|BIT7),  // IIO_BIFURCATE_x4x4x2x2x2x2
  (UINT8) (BIT0|BIT1|BIT2|BIT3|BIT4|BIT5|BIT6|BIT7),  // IIO_BIFURCATE_x2x2x2x2x2x2x4
  (UINT8) (BIT0|BIT1|BIT2|BIT3|BIT4|BIT5|BIT6|BIT7),  // IIO_BIFURCATE_x2x2x2x2x4x2x2
  (UINT8) (BIT0|BIT1|BIT2|BIT3|BIT4|BIT5|BIT6|BIT7),  // IIO_BIFURCATE_x2x2x4x2x2x2x2
  (UINT8) (BIT0|BIT1|BIT2|BIT3|BIT4|BIT5|BIT6|BIT7),  // IIO_BIFURCATE_x4x2x2x2x2x2x2
  (UINT8) (BIT0|BIT1|BIT2|BIT3|BIT4|BIT5|BIT6|BIT7)   // IIO_BIFURCATE_x2x2x2x2x2x2x2x2
};

/**
  Hide unused VMD device.

  @param IioIndex   - index of the socket

  @return VOID
 */
VOID
HideUnusedVmdDevice (
  IN  UINT8  IioIndex
)
{
  UINT8                       StackIndex;
  FNHIDE_IIO_VTD_STRUCT       Ms2iosfFnHide;

  for (StackIndex = IIO_STACK8; StackIndex < MAX_IIO_STACK_SPR; StackIndex++) {
    if (IfStackPresent (IioIndex, StackIndex)) {
      Ms2iosfFnHide.Data = IioReadCpuCsr8 (IioIndex, StackIndex, FNHIDE_IIO_VTD_REG);
      Ms2iosfFnHide.Bits.function5_hide = 1;
      IioWriteCpuCsr8 (IioIndex, StackIndex, FNHIDE_IIO_VTD_REG, Ms2iosfFnHide.Data);
    }
  }
}

/**
  Hide unused DINO devices.
  @param[in] IioGlobalData   - Global data for sockets
  @param[in] IioIndex        - index of the socket

  @return VOID
 */

VOID
HideUnusedDinoDevice (
  IN  IIO_GLOBALS *IioGlobalData,
  IN  UINT8  IioIndex
)
{
  PSF_2_AGNT_T1_SHDW_PCIEN_CPM0_RS0_D6_F0_OFFSET64_SB_PSF_HCX_STRUCT      Psf1CpmConfig;
  PSF_2_AGNT_T1_SHDW_PCIEN_HQM0_RS0_D9_F0_OFFSET65_SB_PSF_HCX_STRUCT      Psf1HqmConfig;

  UINT8    StackIndex;
  UINT8    BoxInst;

  for (StackIndex = IIO_STACK8; StackIndex < MAX_IIO_STACK_SPR; StackIndex++) {
    if (!IfStackPresent (IioIndex, StackIndex)) {
      continue;
    }

    BoxInst = StackIndex - IIO_STACK8;

    if (IioGlobalData->SetupData.DsaEn[IioIndex * NUM_DSA + BoxInst] == IIO_OPTION_DISABLE) {
      IioDsaPsfFunctionDisable (IioGlobalData, IioIndex, StackIndex);
    }

    if (IioGlobalData->SetupData.IaxEn[IioIndex * NUM_IAX + BoxInst] == IIO_OPTION_DISABLE) {
      IioIaxPsfFunctionDisable (IioGlobalData, IioIndex, StackIndex);
    }

    if (IioGlobalData->SetupData.CpmEn[IioIndex * NUM_CPM + BoxInst] == IIO_OPTION_DISABLE) {
      Psf1CpmConfig.Data = IioReadCpuCsr32 (IioIndex, BoxInst, PSF_2_AGNT_T1_SHDW_PCIEN_CPM0_RS0_D6_F0_OFFSET64_SB_PSF_HCX_REG);
      IIO_D_LOG ("Hiding CPM Psf1CpmConfig: %x \n", Psf1CpmConfig.Data);
      Psf1CpmConfig.Bits.fundis = 1;
      IioWriteCpuCsr32 (IioIndex, BoxInst, PSF_2_AGNT_T1_SHDW_PCIEN_CPM0_RS0_D6_F0_OFFSET64_SB_PSF_HCX_REG, Psf1CpmConfig.Data);
    }

    if (IioGlobalData->SetupData.HqmEn[IioIndex * NUM_IAX + BoxInst] == IIO_OPTION_DISABLE) {
      Psf1HqmConfig.Data = IioReadCpuCsr32 (IioIndex, BoxInst, PSF_2_AGNT_T1_SHDW_PCIEN_HQM0_RS0_D9_F0_OFFSET65_SB_PSF_HCX_REG);
      IIO_D_LOG ("Hiding HQM Psf1HqmConfig: %x \n", Psf1HqmConfig.Data);
      Psf1HqmConfig.Bits.fundis = 1;
      IioWriteCpuCsr32 (IioIndex, BoxInst, PSF_2_AGNT_T1_SHDW_PCIEN_HQM0_RS0_D9_F0_OFFSET65_SB_PSF_HCX_REG, Psf1HqmConfig.Data);
    }
  }
}

/**
  Disable DSA PSF function.
  Used when disabling Fabric decode for source decode or abort access when in Fabric decode.

  @param[in] IioGlobalData   - Global data for sockets
  @param[in] IioIndex        - index of the socket
  @param[in] StackIndex      - index of the stack

  @return VOID
 */
VOID
IioDsaPsfFunctionDisable (
  IN  IIO_GLOBALS *IioGlobalData,
  IN  UINT8  IioIndex,
  IN  UINT8  StackIndex
)
{
  PSF_1_AGNT_T0_SHDW_PCIEN_DSA_RS0_D1_F0_OFFSET2_SB_PSF_DINO_STRUCT       Psf1DsaConfig;

  UINT8    BoxInst;

  BoxInst = StackIndex - IIO_STACK8;
  if(IioGlobalData->IioVar.IioVData.StackPersonality[IioIndex][StackIndex] == TYPE_DINO) {
    Psf1DsaConfig.Data = IioReadCpuCsr32 (IioIndex, BoxInst, PSF_1_AGNT_T0_SHDW_PCIEN_DSA_RS0_D1_F0_OFFSET2_SB_PSF_DINO_REG);
    IIO_D_LOG ("Hiding DSA Psf1DsaConfig: %x \n", Psf1DsaConfig.Data);
    Psf1DsaConfig.Bits.fundis = 1;
    IioWriteCpuCsr32 (IioIndex, BoxInst, PSF_1_AGNT_T0_SHDW_PCIEN_DSA_RS0_D1_F0_OFFSET2_SB_PSF_DINO_REG, Psf1DsaConfig.Data);
  }
}

/**
  Disable IAX PSF function.
  Used when disabling Fabric decode for source decode or abort access when in Fabric decode.

  @param[in] IioGlobalData   - Global data for sockets
  @param[in] IioIndex        - index of the socket
  @param[in] StackIndex      - index of the stack

  @return VOID
 */
VOID
IioIaxPsfFunctionDisable (
  IN  IIO_GLOBALS *IioGlobalData,
  IN  UINT8  IioIndex,
  IN  UINT8  StackIndex
)
{
  PSF_1_AGNT_T0_SHDW_PCIEN_IAX_RS0_D2_F0_OFFSET3_SB_PSF_DINO_STRUCT       Psf1IaxConfig;

  UINT8    BoxInst;

  BoxInst = StackIndex - IIO_STACK8;
  if(IioGlobalData->IioVar.IioVData.StackPersonality[IioIndex][StackIndex] == TYPE_DINO) {
    Psf1IaxConfig.Data = IioReadCpuCsr32 (IioIndex, BoxInst, PSF_1_AGNT_T0_SHDW_PCIEN_IAX_RS0_D2_F0_OFFSET3_SB_PSF_DINO_REG);
    IIO_D_LOG ("Hiding IAX Psf1IaxConfig: %x \n", Psf1IaxConfig.Data);
    Psf1IaxConfig.Bits.fundis = 1;
    IioWriteCpuCsr32 (IioIndex, BoxInst, PSF_1_AGNT_T0_SHDW_PCIEN_IAX_RS0_D2_F0_OFFSET3_SB_PSF_DINO_REG, Psf1IaxConfig.Data);
  }
}

/**
  Disable MSM PSF function.
  Used when disabling Fabric decode for source decode or abort access when in Fabric decode.

  @param[in] IioGlobalData   - Global data for sockets
  @param[in] IioIndex        - index of the socket
  @param[in] StackIndex      - index of the stack

  @return VOID
 */
VOID
IioMsmPsfFunctionDisable (
  IN  IIO_GLOBALS *IioGlobalData,
  IN  UINT8  IioIndex,
  IN  UINT8  StackIndex
)
{
  PSF_1_AGNT_T0_SHDW_PCIEN_MSM_RS0_D3_F0_OFFSET4_SB_PSF_DINO_STRUCT       Psf1MsmConfig;
  PSF_1_AGNT_T0_SHDW_PCIEN_MSM_RS0_D3_F1_OFFSET6_SB_PSF_DINO_STRUCT       Psf1MsmPmonConfig;
  PSF_1_AGNT_T0_SHDW_PCIEN_MSM_RS0_D3_F2_OFFSET5_SB_PSF_DINO_STRUCT       Psf1MsmDfxConfig;

  UINT8    BoxInst;

  BoxInst = StackIndex - IIO_STACK8;
  if(IioGlobalData->IioVar.IioVData.StackPersonality[IioIndex][StackIndex] == TYPE_DINO) {
    Psf1MsmConfig.Data = IioReadCpuCsr32 (IioIndex, BoxInst, PSF_1_AGNT_T0_SHDW_PCIEN_MSM_RS0_D3_F0_OFFSET4_SB_PSF_DINO_REG);
    Psf1MsmConfig.Bits.fundis = 1;
    IioWriteCpuCsr32 (IioIndex, BoxInst, PSF_1_AGNT_T0_SHDW_PCIEN_MSM_RS0_D3_F0_OFFSET4_SB_PSF_DINO_REG, Psf1MsmConfig.Data);

    Psf1MsmPmonConfig.Data = IioReadCpuCsr32 (IioIndex, BoxInst, PSF_1_AGNT_T0_SHDW_PCIEN_MSM_RS0_D3_F1_OFFSET6_SB_PSF_DINO_REG);
    Psf1MsmPmonConfig.Bits.fundis = 1;
    IioWriteCpuCsr32 (IioIndex, BoxInst, PSF_1_AGNT_T0_SHDW_PCIEN_MSM_RS0_D3_F1_OFFSET6_SB_PSF_DINO_REG, Psf1MsmPmonConfig.Data);

    Psf1MsmDfxConfig.Data = IioReadCpuCsr32 (IioIndex, BoxInst, PSF_1_AGNT_T0_SHDW_PCIEN_MSM_RS0_D3_F2_OFFSET5_SB_PSF_DINO_REG);
    Psf1MsmDfxConfig.Bits.fundis = 1;
    IioWriteCpuCsr32 (IioIndex, BoxInst, PSF_1_AGNT_T0_SHDW_PCIEN_MSM_RS0_D3_F2_OFFSET5_SB_PSF_DINO_REG, Psf1MsmDfxConfig.Data);
  }
}

/**
  Force outband traffic class to zero.

  @param IN IioGlobalData  - Pointer to IIO_GLOBALS
  @param IN IioIndex       - index of the socket

  @return VOID
 */
VOID
ClearZeroObTc (
  IN  IIO_GLOBALS   *IioGlobalData,
  IN  UINT8         IioIndex
)
{
  UINT8                             MaxIouStack;
  UINT8                             StackIndex;
  UINT8                             ChannelIndex;
  UINT32                            MiscCtrlStsRegAddr;
  MISCCTRLSTS_CH0_IIO_VTD_STRUCT    MiscCtrlStsCh;
  PI5_MODES                         StackMode;

  MaxIouStack = GetMaxIouNumPerSocket ();
  for (StackIndex = 0; StackIndex < (MaxIouStack + 1); StackIndex++) {
    if (!IfStackPresent (IioIndex, StackIndex)) {
      continue;
    }
    StackMode = GetStackMode (IioGlobalData, IioIndex, StackIndex);
    switch (StackMode) {
    case PI5_MODE_GEN4_ONLY:
    case PI5_MODE_GEN5_AND_GEN4:
      for (ChannelIndex = 0; ChannelIndex < MAX_MS2IOSF_CHANNEL_PER_STACK; ChannelIndex++) {
        MiscCtrlStsRegAddr = MISCCTRLSTS_CH0_IIO_VTD_REG + ChannelIndex * 8;
        MiscCtrlStsCh.Data = IioReadCpuCsr32 (IioIndex, StackIndex, MiscCtrlStsRegAddr);
        MiscCtrlStsCh.Bits.zero_ob_tc = 0;
        IioWriteCpuCsr32 (IioIndex, StackIndex, MiscCtrlStsRegAddr, MiscCtrlStsCh.Data);
      }
      break;
    default:
      break;
    }
  }
}

VOID
EFIAPI
IioPsfInit (
  IN  IIO_GLOBALS         *IioGlobalData,
  IN  UINT8               IioIndex
  )
{
  UINT8                 MaxIouStack;
  UINT8                 StackIndex;
  UINT8                 PsfInstance;
  PI5_MODES             StackMode;
  PSF_1_RC_OWNER_RS0_SB_PSF_STRUCT           Psf1RcOwnerRs0;
  PSF_1_RC_OWNER_RS3_SB_PSF_STRUCT           Psf1RcOwnerRs3;

  IIO_D_LOG ("IIO_PSF_INIT_START\n");

  MaxIouStack = GetMaxIouNumPerSocket();
  for (StackIndex = IIO_STACK0; StackIndex < (MaxIouStack + 1); StackIndex++) {
    PsfInstance = StackIndex;

    if (!IfStackPresent (IioIndex, StackIndex)) {
      continue;
    }
    StackMode = GetStackMode (IioGlobalData, IioIndex, StackIndex);
    switch (StackMode) {
    case PI5_MODE_GEN5_AND_GEN4:
      Psf1RcOwnerRs0.Data = IioReadCpuCsr32 (IioIndex, PsfInstance, PSF_1_RC_OWNER_RS0_SB_PSF_REG);
      Psf1RcOwnerRs3.Data = IioReadCpuCsr32 (IioIndex, PsfInstance, PSF_1_RC_OWNER_RS3_SB_PSF_REG);
      //
      // Enable Command
      //
      Psf1RcOwnerRs0.Bits.psfid = 1;
      Psf1RcOwnerRs0.Bits.chanmap = 1;
      Psf1RcOwnerRs3.Bits.psfid = 1;
      Psf1RcOwnerRs3.Bits.chanmap = 1;

      IioWriteCpuCsr32 (IioIndex, PsfInstance, PSF_1_RC_OWNER_RS0_SB_PSF_REG, Psf1RcOwnerRs0.Data);
      IioWriteCpuCsr32 (IioIndex, PsfInstance, PSF_1_RC_OWNER_RS3_SB_PSF_REG, Psf1RcOwnerRs3.Data);

      break;

    default:
      break;
    }
  }
  IIO_D_LOG ("IIO_PSF_INIT_END\n");
  return;
}



/**

    Iio Misc Featues Init before link training.

    @param IioGlobalData  - Pointer to IIO_GLOBALS
    @param IioIndex       - Index to CPU/IIO

    @retval None

**/
VOID
IioMiscFeatureEarlyInit (
  IN  IIO_GLOBALS                                    *IioGlobalData,
  IN  UINT8                                          IioIndex
  )
{
  HideUnusedVmdDevice (IioIndex);
  HideUnusedDinoDevice (IioGlobalData, IioIndex);
  if (!IsSiliconWorkaroundEnabled ("S14010402958")) {
    ClearZeroObTc (IioGlobalData, IioIndex);
  }
  return;
}

/**
  Enable Misc Featues by writing to IIOMISCCTRL

  @param[in] IioGlobalData  Pointer to IIO_GLOBALS
  @param[in] IioIndex       Index to CPU/IIO
**/
VOID
IioMiscFeatureInit (
  IN IIO_GLOBALS *IioGlobalData,
  IN UINT8       IioIndex
  )
{
  UINT8                         Stack;
  TOCM_IIO_VTD_STRUCT           IioTocm;
  IIOMISCCTRL_N0_IIO_VTD_STRUCT IioMiscCtrl;
  UINT8                         MaxStackPerSocket;

  MaxStackPerSocket = GetMaxStackNumPerSocket ();
  for (Stack = 0; Stack < MaxStackPerSocket; Stack++) {
    if (!(IioGlobalData->IioVar.IioVData.StackPresentBitmap[IioIndex] & (1 << Stack))) {
      continue;
    }

    //
    // Setup the EN1K feature of IIOMISCCTRL (Register offset 0x1C0, bit 2 )
    //
    IioMiscCtrl.Data = IioReadCpuCsr32 (IioIndex, Stack, IIOMISCCTRL_N0_IIO_VTD_REG);
    IioMiscCtrl.Bits.en1k = 0;
    if (IioGlobalData->SetupData.EN1K) {
      IioMiscCtrl.Bits.en1k = 1;
    }

    //
    // Remote VGA enable : default for non-legacy sockets
    //
    if ((IioGlobalData->SetupData.LegacyVgaSoc == IioIndex) &&
        (IioGlobalData->SetupData.LegacyVgaStack == Stack)) {
      IioMiscCtrl.Bits.rvgaen = 1;
    }

    //
    // NOTE: We are asking user to explicitly indicate on which socket its VGA card
    // would be connected for the IIO apart from other configuration settings to
    // select onboard video device or detect the VGA device from the PCIe port.....
    //
    IioTocm.Data = IioReadCpuCsr8 (IioIndex, Stack, TOCM_IIO_VTD_REG);
    IioTocm.Bits.tocmvalid = 1;
    IioWriteCpuCsr8 (IioIndex, Stack, TOCM_IIO_VTD_REG, IioTocm.Data);
    IioWriteCpuCsr32 (IioIndex, Stack, IIOMISCCTRL_N0_IIO_VTD_REG, IioMiscCtrl.Data);
  } // End of for (Stack = 0; Stack < MAX_IIO_STACK; Stack++)

  //
  // Initialize HCx
  //
  if (!IsHbmSku ()) { // No HCx on HBM SKU
    IioHcxInit (IioGlobalData, IioIndex);
  }
  IioPsfInit (IioGlobalData, IioIndex);
}

/**

    Peer 2 peer memory config

    @param IioGlobalData - Pointer to IIO_GLOBALS
    @param IioIndex      - Current socket.
    @param PortIndex     - Port number being initialized.

    @retval None

**/
VOID
Peer2peerMemoryConfig (
  IN  IIO_GLOBALS    *IioGlobalData,
  IN  UINT8          IioIndex,
  IN  UINT8          PortIndex
  )
{
  UINT8                             StackIndex;
  UINT8                             ChannelIndex;
  UINT32                            MiscCtrlStsRegAddr;
  MISCCTRLSTS_CH0_IIO_VTD_STRUCT    MiscCtrlStsCh;

  StackIndex = IioGlobalData->IioVar.IioVData.StackPerPort[IioIndex][PortIndex];
  ChannelIndex = GetMs2iosfChannelIndex(PortIndex);
  MiscCtrlStsRegAddr = MISCCTRLSTS_CH0_IIO_VTD_REG + ChannelIndex*8;

  MiscCtrlStsCh.Data = IioReadCpuCsr32 (IioIndex, StackIndex, MiscCtrlStsRegAddr);

  MiscCtrlStsCh.Bits.problematic_channel_for_lock_flows =1;

  IioWriteCpuCsr32 (IioIndex, StackIndex, MiscCtrlStsRegAddr, MiscCtrlStsCh.Data);
}

/**

  MS2IOSF channel PERFCTRLSTS register initial.

  @param Phase         - Initialization phase (pre, init, post).
  @param IioGlobalData - Pointer to IIO global data/methods
  @param IioIndex      - Current socket.
  @param StackIndex    - Stack number being initialized.

**/
VOID
Ms2iosfChannelMiscInit (
  IN  IIO_INIT_PHASE                 Phase,
  IN  IIO_GLOBALS                    *IioGlobalData,
  IN  UINT8                          IioIndex,
  IN  UINT8                          StackIndex
  )
{
  UINT8     MaxPortNumberPerSocket;
  UINT8     PortIndex = 0;
  UINT8     ChannelIndex = 0;
  UINT8     ChannelBitMap = 0;
  PI5_MODES StackMode;
  UINT8     Bifurcation;
  UINT32    PerfCtrlStsRegAddr;
  UINT32    MiscCtrlStsRegAddr;
  PERFCTRLSTS_CH0_IIO_VTD_STRUCT    PerfCtrlStsCh;
  MISCCTRLSTS_CH0_IIO_VTD_STRUCT    MiscCtrlStsCh;

  if (IsPcieRootPortStack (IioGlobalData, IioIndex, StackIndex)) {
    StackMode = GetStackMode (IioGlobalData, IioIndex, StackIndex);
    if (StackMode == PI5_MODE_GEN4_ONLY) {
      PortIndex = GetPortIndexbyStack (StackIndex, IioPortH);
    } else {
      PortIndex = GetPortIndexbyStack (StackIndex, PORT_0_INDEX);
    }
    if (IioVData_ConfigurePciePort (IioGlobalData, IioIndex, PortIndex)) {  //check whether the PCIe port can be configured
      Bifurcation = CheckBifurcationMapPort (IioGlobalData, IioIndex, PortIndex);
      switch (StackMode) {
      case PI5_MODE_DMI:
        ChannelBitMap = mMs2iosfChannelMapping_Dmi;
        break;
      case PI5_MODE_IAL:
        ChannelBitMap = mMs2iosfChannelMapping_Ial;
        break;
      case PI5_MODE_NTB:
        ChannelBitMap = mMs2iosfChannelMapping_Ntb;
        break;
      case PI5_MODE_GEN4_ONLY:
        ChannelBitMap = mMs2iosfChannelMapping_PcieGen4[Bifurcation];
        break;
      case PI5_MODE_GEN5_AND_GEN4:
        ChannelBitMap = mMs2iosfChannelMapping_PcieGen5Gen4[Bifurcation];
        break;
      default:
        ChannelBitMap = 0;
        break;
      }
      // No channel mapping need to programming
      if (0 == ChannelBitMap) {
        return;
      }

      //
      // Program all channels connected in the stack
      //
      for (ChannelIndex = 0; ChannelIndex < LastChannel; ChannelIndex++) {
        if ((ChannelBitMap & (BIT0 << ChannelIndex)) == 0) {
          continue;
        }

        IIO_D_LOG ("\tIioIndex = %d\tPortIndex = %d\tStackIndex = %d\tChannel = %d\n", IioIndex, PortIndex, StackIndex, ChannelIndex);
        PerfCtrlStsRegAddr = PERFCTRLSTS_CH0_IIO_VTD_REG + ChannelIndex*8;
        PerfCtrlStsCh.Data = IioReadCpuCsr32 (IioIndex, StackIndex, PerfCtrlStsRegAddr);
        switch (Phase) {
        case IIOInitPhase1:
          if (IsVc1OrVcm (IioIndex, StackIndex, ChannelIndex, TRUE, TRUE)) {
            MiscCtrlStsRegAddr = MISCCTRLSTS_CH0_IIO_VTD_REG + ChannelIndex*8;
            MiscCtrlStsCh.Data = IioReadCpuCsr32 (IioIndex, StackIndex, MiscCtrlStsRegAddr);
            MiscCtrlStsCh.Bits.do_not_block_during_lock = 1;
            IioWriteCpuCsr32 (IioIndex, StackIndex, MiscCtrlStsRegAddr, MiscCtrlStsCh.Data);
          }
          break;

        case IIOInitPhase2:
          break;

        case IIOInitPhase3:
          //
          // Relax Ordering override programming
          //
          if (IioGlobalData->SetupData.PcieRelaxedOrdering) {
            PerfCtrlStsCh.Bits.disable_ro = 0;
          } else {
            PerfCtrlStsCh.Bits.disable_ro = 1;
          }

          if (IioDmiIsPortIdentity (IioIndex, PortIndex, IioDmiPort)) {
            if (IioGlobalData->SetupData.DmiAllocatingFlow) {
              PerfCtrlStsCh.Bits.use_allocating_flow_wr = 1;
            } else {
              PerfCtrlStsCh.Bits.use_allocating_flow_wr = 0;
            }
          } else {
            if (IioGlobalData->SetupData.PcieAllocatingFlow) {
              PerfCtrlStsCh.Bits.use_allocating_flow_wr = 1;
            } else {
              PerfCtrlStsCh.Bits.use_allocating_flow_wr = 0;
            }
          }

          PerfCtrlStsCh.Bits.nosnoopopwren = 1;
          if (IioGlobalData->SetupData.NoSnoopWrCfg == IIO_OPTION_DISABLE &&
              !IsVc1OrVcm (IioIndex, StackIndex, ChannelIndex, TRUE, TRUE)) {
            PerfCtrlStsCh.Bits.nosnoopopwren = 0;
          }

          PerfCtrlStsCh.Bits.partial_cohrd_op = IioGlobalData->SetupData.CoherentReadPart;

          //
          // enable/disable TPH
          //
          PerfCtrlStsCh.Bits.disable_tph = IioGlobalData->SetupData.DisableTPH;
          if (IioGlobalData->SetupData.DisableTPH == FALSE) {
            PerfCtrlStsCh.Bits.prioritize_tph = IioGlobalData->SetupData.PrioritizeTPH;
          }
          MaxPortNumberPerSocket = IioGlobalData->IioVar.IioOutData.MaxPciePortNumberPerSocket[IioIndex];
          //
          // Check IODC
          //
          if ((IioGlobalData->SetupData.IODC[((IioIndex) * MaxPortNumberPerSocket) + PortIndex]) == IODC_GLOBAL_KTI_OPTION) {
            //
            // Apply KTI global option
            //
            switch (IioGlobalData->IioVar.IioVData.IODC) {
            case IODC_AUTO:
            case IODC_EN_REM_INVITOM_AND_WCILF:
              PerfCtrlStsCh.Bits.implicit_tph = 1;
              PerfCtrlStsCh.Bits.implicit_nosnoop = 0;
              break;
            case IODC_EN_REM_INVITOM_PUSH:
              PerfCtrlStsCh.Bits.implicit_tph = 1;
              PerfCtrlStsCh.Bits.implicit_nosnoop = 0;
              break;
            case IODC_EN_REM_INVITOM_ALLOCFLOW:
              PerfCtrlStsCh.Bits.implicit_tph = 0;
              PerfCtrlStsCh.Bits.implicit_nosnoop = 0;
              break;
            case IODC_EN_REM_INVITOM_ALLOC_NONALLOC:
              PerfCtrlStsCh.Bits.implicit_tph = 0;
              PerfCtrlStsCh.Bits.implicit_nosnoop = 1;
              break;
            case IODC_DISABLE:
            default:
              break;
            }
          } else {
            switch (IioGlobalData->SetupData.IODC[((IioIndex) * MaxPortNumberPerSocket) + PortIndex]) {
            //
            // Apply particular port configuration
            //
            case IODC_AUTO:
            case IODC_EN_REM_INVITOM_AND_WCILF:
              PerfCtrlStsCh.Bits.implicit_tph = 1;
              PerfCtrlStsCh.Bits.implicit_nosnoop = 0;
              break;
            case IODC_EN_REM_INVITOM_PUSH:
              PerfCtrlStsCh.Bits.implicit_tph = 1;
              PerfCtrlStsCh.Bits.implicit_nosnoop = 0;
              break;
            case IODC_EN_REM_INVITOM_ALLOCFLOW:
              PerfCtrlStsCh.Bits.implicit_tph = 0;
              PerfCtrlStsCh.Bits.implicit_nosnoop = 0;
              break;
            case IODC_EN_REM_INVITOM_ALLOC_NONALLOC:
              PerfCtrlStsCh.Bits.implicit_tph = 0;
              PerfCtrlStsCh.Bits.implicit_nosnoop = 1;
              break;
            case IODC_DISABLE:
            default:
              break;
            }
          }

          break;

        default:
          break;
        }
        IioWriteCpuCsr32 (IioIndex, StackIndex, PerfCtrlStsRegAddr, PerfCtrlStsCh.Data);
      }
    }
  } else if (Phase == IIOInitPhase3) {
    PerfCtrlStsCh.Data = IioReadCpuCsr32 (IioIndex, StackIndex, PERFCTRLSTS_CH0_IIO_VTD_REG);
    if (IioGlobalData->SetupData.DfxNpkAllocatingFlow) {
      PerfCtrlStsCh.Bits.use_allocating_flow_wr = 1;
    } else {
      PerfCtrlStsCh.Bits.use_allocating_flow_wr = 0;
    }
    IioWriteCpuCsr32 (IioIndex, StackIndex, PERFCTRLSTS_CH0_IIO_VTD_REG, PerfCtrlStsCh.Data);
  }
}
