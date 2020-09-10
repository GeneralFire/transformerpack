/** @file

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

#include <Base.h>
#include <RcRegs.h>

#include <IioPlatformData.h>

#include <LibraryPrivate/IioMiscWorkArounds.h>
#include <Library/IioAccessLib.h>
#include <Library/KtiApi.h>
#include <Library/SiliconWorkaroundLib.h>

#include <Library/IioSysInfoLib.h>
#include <Library/CpuAndRevisionLib.h>
#include <Library/IioDataHubLib.h>

/**
  Routine name: IioOtherMiscellaneousWorkarounds
  Description: This routine may hold other miscellaneous workarounds which may
  be temporary or require only for testing purpose

    @param IioGlobalData - IIO global data structure
    @param IioIndex      - IIO index

    @retval None

**/
VOID
IioMiscellaneousWorkarounds (
  IN  IIO_GLOBALS              *IioGlobalData,
  IN  UINT8                    IioIndex
  )
{
  UINT8                                                 StackIndex;
  IRP_MISC_DFX2_IIO_DFX_VTD_STRUCT                      IrpMiscDfx2;
  ENABLE_B2B_OTC_VTDPIPE_0_IIO_DFX_VTD_STRUCT           EnableB2BOtcVtdpipe0;
  ENABLE_B2B_OTC_VTDPIPE_1_IIO_DFX_VTD_STRUCT           EnableB2BOtcVtdpipe1;
  ITCPIPEHAZARD_IIO_DFX_VTD_STRUCT                      Itcpipehazard;
  ITCPIPEHAZARD_NEW_IIO_DFX_VTD_STRUCT                  ItcpipehazardNew;
  ARBMUXCTL0_SB_CXL_AM_STRUCT                           CxlArbMuxCtl0;
  ITCCTRL89_N0_IIO_VTD_STRUCT                           ItcCtrl89N0;
  ITCCTRL89_N1_IIO_VTD_STRUCT                           ItcCtrl89N1;
  PI5_MODES                                             StackMode;
  IRP_MISC_DFX0_IIO_DFX_VTD_STRUCT                      IrpMiscDfx0;
  for (StackIndex = 0; StackIndex < MAX_IIO_STACK; StackIndex++) {
    if (!IfStackPresent (IioIndex, StackIndex)) {
        continue;
    }

    if (IsSiliconWorkaroundEnabled ("S1707089812")) {
      //
      // Prevent deadlock case when testing set-conflicts and CLTT
      //
      IrpMiscDfx2.Data = IioReadCpuCsr32 (IioIndex, StackIndex, IRP_MISC_DFX2_IIO_DFX_VTD_REG);
      IrpMiscDfx2.Bits.p_pf_timeout_cnt = 0x11;
      IioWriteCpuCsr32 (IioIndex, StackIndex, IRP_MISC_DFX2_IIO_DFX_VTD_REG, IrpMiscDfx2.Data);
    }

    if (IsSiliconWorkaroundEnabled ("S14010996350")) {
      EnableB2BOtcVtdpipe0.Data = IioReadCpuCsr32 (IioIndex, StackIndex, ENABLE_B2B_OTC_VTDPIPE_0_IIO_DFX_VTD_REG);
      EnableB2BOtcVtdpipe1.Data = IioReadCpuCsr32 (IioIndex, StackIndex, ENABLE_B2B_OTC_VTDPIPE_1_IIO_DFX_VTD_REG);

      EnableB2BOtcVtdpipe0.Bits.csrenablevtdpipeb2bsnnnh_chn0_p = 0;
      EnableB2BOtcVtdpipe0.Bits.csrenablevtdpipeb2bsnnnh_chn1_p = 0;
      EnableB2BOtcVtdpipe0.Bits.csrenablevtdpipeb2bsnnnh_chn2_p = 0;
      EnableB2BOtcVtdpipe0.Bits.csrenablevtdpipeb2bsnnnh_chn3_p = 0;
      EnableB2BOtcVtdpipe0.Bits.csrenablevtdpipeb2bsnnnh_chn4_p = 0;
      EnableB2BOtcVtdpipe0.Bits.csrenablevtdpipeb2bsnnnh_chn5_p = 0;
      EnableB2BOtcVtdpipe0.Bits.csrenablevtdpipeb2bsnnnh_chn6_p = 0;
      EnableB2BOtcVtdpipe0.Bits.csrenablevtdpipeb2bsnnnh_chn7_p = 0;

      EnableB2BOtcVtdpipe1.Bits.csrenablevtdpipeb2bsnnnh_chn8_p = 0;
      EnableB2BOtcVtdpipe1.Bits.csrenablevtdpipeb2bsnnnh_chn9_p = 0;
      EnableB2BOtcVtdpipe1.Bits.csrenablevtdpipeb2bsnnnh_chn10_p = 0;
      EnableB2BOtcVtdpipe1.Bits.csrenablevtdpipeb2bsnnnh_chn11_p = 0;

      IioWriteCpuCsr32 (IioIndex, StackIndex, ENABLE_B2B_OTC_VTDPIPE_0_IIO_DFX_VTD_REG, EnableB2BOtcVtdpipe0.Data);
      IioWriteCpuCsr32 (IioIndex, StackIndex, ENABLE_B2B_OTC_VTDPIPE_1_IIO_DFX_VTD_REG, EnableB2BOtcVtdpipe1.Data);
    }

    if (IsSiliconWorkaroundEnabled ("S14010021323")) {
      //
      // Prevent deadlock case when testing set-conflicts and CLTT
      //
      Itcpipehazard.Data = IioReadCpuCsr32 (IioIndex, StackIndex, ITCPIPEHAZARD_IIO_DFX_VTD_REG);
      Itcpipehazard.Bits.enable = 1;
      Itcpipehazard.Bits.mode = 3;
      Itcpipehazard.Bits.lteq = 0;
      Itcpipehazard.Bits.thresh = 0x6;
      Itcpipehazard.Bits.trigger = 4;
      Itcpipehazard.Bits.block = 0x100;
      IioWriteCpuCsr32 (IioIndex, StackIndex, ITCPIPEHAZARD_IIO_DFX_VTD_REG, Itcpipehazard.Data);
    }

    if (IsSiliconWorkaroundEnabled ("S14011367585")) {
      Itcpipehazard.Data = IioReadCpuCsr32 (IioIndex, StackIndex, ITCPIPEHAZARD_IIO_DFX_VTD_REG);
      ItcpipehazardNew.Data = IioReadCpuCsr32 (IioIndex, StackIndex, ITCPIPEHAZARD_NEW_IIO_DFX_VTD_REG);
      Itcpipehazard.Bits.enable = 0x1;
      Itcpipehazard.Bits.mode = 0x0;
      Itcpipehazard.Bits.lteq = 0x1;
      Itcpipehazard.Bits.thresh = 0x1;
      Itcpipehazard.Bits.trigger = 0x1F0;
      Itcpipehazard.Bits.block = 0x1F0;
      ItcpipehazardNew.Bits.enable = 0x1;
      ItcpipehazardNew.Bits.mode = 0x0;
      ItcpipehazardNew.Bits.lteq = 0x1;
      ItcpipehazardNew.Bits.thresh = 0x8;
      ItcpipehazardNew.Bits.trigger = 0x80;
      ItcpipehazardNew.Bits.block = 0x100;
      IioWriteCpuCsr32 (IioIndex, StackIndex, ITCPIPEHAZARD_IIO_DFX_VTD_REG, Itcpipehazard.Data);
      IioWriteCpuCsr32 (IioIndex, StackIndex, ITCPIPEHAZARD_NEW_IIO_DFX_VTD_REG, ItcpipehazardNew.Data);
    }

    if (IsSiliconWorkaroundEnabled ("S14011103009") &&
      IioGlobalData->IioVar.IioVData.StackPersonality[IioIndex][StackIndex] == TYPE_UBOX_IIO) {
      //
      //  the L1/L2 entry hysteresis bit is only applicable for DMI and PCIE stacks
      //  Default value of L1/L2 Entry Hysteresis should be increased to:
      //  0x100 - 256 clocks for A0.
      //  0x101 - 1024 clocks for B0.
      //  Programming shall be implemented in order to overcome the Silicon issue.
      //
      StackMode = GetStackMode (IioGlobalData, IioIndex, StackIndex);
      switch (StackMode) {
      case PI5_MODE_DMI:
      case PI5_MODE_NTB:
      case PI5_MODE_GEN4_ONLY:
        break;
      case PI5_MODE_IAL:
      case PI5_MODE_GEN5_AND_GEN4:
        //
        //  the arbmux register is only applicable to IAL and Gen5_and_Gen4 mode
        //
        CxlArbMuxCtl0.Data = IioReadCpuCsr32 (IioIndex, StackIndex, ARBMUXCTL0_SB_CXL_AM_REG);

        if (IsCpuAndRevision (CPU_SPRSP, REV_A0)) {
          CxlArbMuxCtl0.Bits.l12entryhys = 4; //'100' - 256 clocks for A0 stepping
        } else {
          CxlArbMuxCtl0.Bits.l12entryhys = 5; //'101' - 1024 clocks for B0 stepping and further.
        }

        IioWriteCpuCsr32 (IioIndex, StackIndex, ARBMUXCTL0_SB_CXL_AM_REG, CxlArbMuxCtl0.Data);
        break;
      default:
        break;
      }
    }

    IrpMiscDfx0.Data = IioReadCpuCsr32 (IioIndex, StackIndex, IRP_MISC_DFX0_IIO_DFX_VTD_REG);
    IrpMiscDfx0.Bits.p_pf_snp_rsp_holdoff_cnt = IioGlobalData->SetupData.SnoopResponseHoldOff;
    if (IioGlobalData->IioVar.IioVData.StackPersonality[IioIndex][StackIndex] == TYPE_DINO) {
      if (IsSiliconWorkaroundEnabled ("S1707072739")) {
        IrpMiscDfx0.Bits.p_pf_snp_rsp_holdoff_cnt = 0xa;
      }
    }
    IioWriteCpuCsr32 (IioIndex, StackIndex, IRP_MISC_DFX0_IIO_DFX_VTD_REG, IrpMiscDfx0.Data);

    if (IsSiliconWorkaroundEnabled ("S2207523533")) {
      ItcCtrl89N0.Data = IioReadCpuCsr32 (IioIndex, StackIndex, ITCCTRL89_N0_IIO_VTD_REG);
      ItcCtrl89N1.Data = IioReadCpuCsr32 (IioIndex, StackIndex, ITCCTRL89_N1_IIO_VTD_REG);
      ItcCtrl89N0.Bits.en_gbl_serialize_irp_p2p_np = 0;
      ItcCtrl89N0.Bits.en_gbl_serialize_irp_p2p_p = 0;
      ItcCtrl89N0.Bits.en_chnl_serialize_irp_p2p_p = 0;
      ItcCtrl89N1.Bits.en_chnl_serialize_irp_p2p_p = 0;
      ItcCtrl89N1.Bits.en_chnl_serialize_irp_p2p_np = 0;
      IioWriteCpuCsr32 (IioIndex, StackIndex, ITCCTRL89_N0_IIO_VTD_REG, ItcCtrl89N0.Data);
      IioWriteCpuCsr32 (IioIndex, StackIndex, ITCCTRL89_N1_IIO_VTD_REG, ItcCtrl89N1.Data);
    }
  }
}

/**
  This function is part of IioMiscWorkarounds interface (this is called from common IioWorkAround)

  @param IioGlobalData - IIO global data structure
  @param IioIndex      - IIO index

  @retval None

 */
VOID
IioWABeforeBif (
  IN  IIO_GLOBALS             *IioGlobalData,
  IN  UINT8                   IioIndex
  )
{
  IioMiscellaneousWorkarounds (IioGlobalData, IioIndex);
}

/**
  This routine is to address the post RWO settings of the IIOMISCCTRL global register

  @param[in] IioGlobalData    - Pointer to IIO_GLOBALS
  @param[in] IioIndex         - Index to CPU/IIO
**/
VOID
IioPortEnumCtrl (
  IN  IIO_GLOBALS               *IioGlobalData,
  IN  UINT8                     IioIndex
  )
{
  UINT8                         PortInStack;
  UINT8                         MaxPortNumber;
  UINT8                         MaxStackSocket;
  UINT8                         Stack;
  UINT8                         PortIndex;
  IIOMISCCTRL_N1_IIO_VTD_STRUCT IioMiscCtrlN1;

  MaxStackSocket = GetMaxStackNumPerSocket ();
  for (Stack = 0; Stack < MaxStackSocket; Stack++) {
    if (!(IioGlobalData->IioVar.IioVData.StackPresentBitmap[IioIndex] & (1 << Stack))) {
      continue;
    }

    MaxPortNumber = GetMaxPortNumPerStack (Stack);
    for (PortInStack = 0; PortInStack < MaxPortNumber; PortInStack++) {

      PortIndex = GetPortIndexbyStack (Stack, PortInStack);
      if (!IioVData_ConfigurePciePort (IioGlobalData, IioIndex, PortIndex)) {  //check whether the PCIe port can be configured
        continue;
      }
      if (IioDmiIsPortIdentity (IioIndex, PortIndex, IioDmiPortActive)) {
        IioMiscCtrlN1.Data = IioReadCpuCsr32 (IioIndex, Stack, IIOMISCCTRL_N1_IIO_VTD_REG);
        IioMiscCtrlN1.Bits.csr_wrcache_snapshot_drain_during_lock = 1;
        IioWriteCpuCsr32 (IioIndex, Stack, IIOMISCCTRL_N1_IIO_VTD_REG, IioMiscCtrlN1.Data);
      }
    } // foreach PortInStack
  }// End of for (Stack = 0; Stack < MAX_IIO_STACK; Stack++)
}

/**
  This routine is to address the post RWO settings of the IIOMISCCTRL global register

    @param IioGlobalData  - Pointer to IIO_GLOBALS
    @param IioIndex       - Index to CPU/IIO

    @retval None
*/
VOID
IioWAPortEnumCtrl (
  IN  IIO_GLOBALS             *IioGlobalData,
  IN  UINT8                   IioIndex
  )
{
  IioPortEnumCtrl (IioGlobalData, IioIndex);
}

