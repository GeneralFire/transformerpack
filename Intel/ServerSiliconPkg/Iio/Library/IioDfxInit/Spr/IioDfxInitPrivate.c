/** @file
  This file contains the IIO DFX init functions specific for SPR

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1999 - 2020 Intel Corporation. <BR>

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

#include <Library/KtiApi.h>

#include <KtiSetupDefinitions.h>
#include <RcRegs.h>
#include <Memory/MemDefaults.h>

#include <Library/IioSysInfoLib.h>
#include <Library/IioDataHubLib.h>
#include <Library/IioAccessLib.h>

#include <LibraryPrivate/IioDfxInitPrivate.h>

/**
  Program CSIPSUBSAD register for a particular stack.

  @param IioIndex     - Index to CPU/IIO
  @param StackIndex   - Index of the stack
  @param ChannelIndex - Index of the MS2IOSF channel

  @return TRUE if given channel is used by ports on given stack
 */
BOOLEAN
IsChanelEnabledForStack (
  IN UINT8   StackIndex,
  IN UINT8   ChannelIndex
  )
{
  UINT8   PortIndexInStack;
  UINT8   PortIndexInSocket;

  for (PortIndexInStack = 0; PortIndexInStack < GetMaxPortNumPerStack (StackIndex); PortIndexInStack++) {
    PortIndexInSocket = GetPortIndexbyStack (StackIndex, PortIndexInStack);

    if (ChannelIndex == GetMs2iosfChannelIndex (PortIndexInSocket)){
      return TRUE;
    }
  }
  return FALSE;
}

/**
  Program CSIPSUBSAD register for a particular stack.

  @param IioIndex   - Index to CPU/IIO
  @param StackIndex - Index of the stack

 */
VOID
IioDfxSetCsipsubsad (
  IN UINT8   IioIndex,
  IN UINT8   StackIndex
  )
{
  CSIPSUBSAD_IIO_DFX_VTD_STRUCT   Csipsubsad;

  Csipsubsad.Data = IioReadCpuCsr32 (IioIndex, StackIndex, CSIPSUBSAD_IIO_DFX_VTD_REG);
  Csipsubsad.Bits.valid = 1;
  IioWriteCpuCsr32 (IioIndex, StackIndex, CSIPSUBSAD_IIO_DFX_VTD_REG, Csipsubsad.Data);
}



/**
  Program registers needs for Isoc support for a particular stack.

  @param IioIndex   - Index to CPU/IIO
  @param StackIndex - Index of the stack
  @param EnableIsoc - Signal if Isoc should be enabled
 */
VOID
IioDfxProgramIsoc (
  IN UINT8   IioIndex,
  IN UINT8   StackIndex,
  IN BOOLEAN EnableIsoc
  )
{
  UINT8                                 ChannelIndex;
  CSIPISOCRES_IIO_DFX_VTD_STRUCT        CsiPiSocres;
  UINT32                                MiscCtrlStsRegAddr;
  MISCCTRLSTS_CH0_IIO_VTD_STRUCT        MiscCtrlStsCh;


  CsiPiSocres.Data = IioReadCpuCsr32 (IioIndex, StackIndex, CSIPISOCRES_IIO_DFX_VTD_REG);
  if (EnableIsoc) {
    CsiPiSocres.Bits.isoc_enabled = 1;
  } else {
    CsiPiSocres.Bits.isoc_enabled = 0;
  }
  //
  // For non-legacy sockets only if Isoc is disabled set also merge_vc0_cnt
  //
  if ((IioIndex != 0) && (!EnableIsoc)) {
    CsiPiSocres.Bits.merge_vc0_cnt = 0;
  }
  IioWriteCpuCsr32 (IioIndex, StackIndex, CSIPISOCRES_IIO_DFX_VTD_REG, CsiPiSocres.Data);

  for (ChannelIndex = 0; ChannelIndex < MAX_MS2IOSF_CHANNEL_PER_STACK; ChannelIndex++) {
    MiscCtrlStsRegAddr = MISCCTRLSTS_CH0_IIO_VTD_REG + ChannelIndex * 8;
    MiscCtrlStsCh.Data = IioReadCpuCsr32 (IioIndex, StackIndex, MiscCtrlStsRegAddr);
    if (EnableIsoc) {
      MiscCtrlStsCh.Bits.enable_isoch = 1;
    } else {
      MiscCtrlStsCh.Bits.enable_isoch = 0;
    }
    IioWriteCpuCsr32 (IioIndex, StackIndex, MiscCtrlStsRegAddr, MiscCtrlStsCh.Data);
  }

}

/**
  Program registers in context of VC1 support for a particular stack.

  @param IioIndex   - Index to CPU/IIO
  @param StackIndex - Index of the stack
  @param EnableVc1  - Signal if VC1 should be enabled
 */
VOID
IioDfxProgramIrpMiscDfx2 (
  IN UINT8   IioIndex,
  IN UINT8   StackIndex,
  IN BOOLEAN EnableVc1
  )
{
  IRP_MISC_DFX2_IIO_DFX_VTD_STRUCT      IrpMiscDfx2Global;

  IrpMiscDfx2Global.Data = IioReadCpuCsr32 (IioIndex, StackIndex, IRP_MISC_DFX2_IIO_DFX_VTD_REG);
  if (EnableVc1) {
    IrpMiscDfx2Global.Bits.enable_vc1 = 1;
  } else {
    IrpMiscDfx2Global.Bits.enable_vc1 = 0;
  }
  IioWriteCpuCsr32 (IioIndex, StackIndex, IRP_MISC_DFX2_IIO_DFX_VTD_REG, IrpMiscDfx2Global.Data);
}

/**
  Program IRPEGCREDITS register for a particular stack.

  @param IioIndex   - Index to CPU/IIO
  @param StackIndex - Index of the stack
 */
VOID
IioDfxProgramEgCredits (
  IN UINT8   IioIndex,
  IN UINT8   StackIndex
  )
{
  IRPEGCREDITS_IIO_DFX_VTD_STRUCT       IrpEgCredits10nm;

  //
  // SPR doesn't need to program ad_shared_cdt_threshold in this phase, it will do it before.
  //
  IrpEgCredits10nm.Data = IioReadCpuCsr32 (IioIndex, StackIndex, IRPEGCREDITS_IIO_DFX_VTD_REG);

  //
  // Overriding ddrt_shared_cdt_threshold value default value 0x4 to recommended value 0x1
  //
  if (GetDdrtQosMode () == DDRT_QOS_MODE_2) {
    IrpEgCredits10nm.Bits.ddrt_shared_cdt_threshold = 0x01;
  } else {
    IrpEgCredits10nm.Bits.ddrt_shared_cdt_threshold = 0x00;
  }

  IioWriteCpuCsr32 (IioIndex, StackIndex, IRPEGCREDITS_IIO_DFX_VTD_REG, IrpEgCredits10nm.Data);
}

