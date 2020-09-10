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
#include <Library/SiliconWorkaroundLib.h>
#include <Library/CpuAndRevisionLib.h>

#include <IioSetupDefinitions.h>
#include <Library/IioAccessLib.h>
#include <Library/IioSysInfoLib.h>
#include <Library/IioDataHubLib.h>

#include <LibraryPrivate/IioMiscWorkArounds.h>
#include "../common10nm/IioCommon10nmMiscWAs.h"

/**
  Routine to check if need clear Egstfwd bits for PSF SB Ports

  @param IioGlobalData - IIO Global data structure
  @param IioIndex      - IIO index

  @retval TRUE - Need clear Egstfwd bits for PSF SB Ports
          FALSE - Skip to clear Egstfwd bits for PSF SB Ports
 */
BOOLEAN
NeedEgstfwdCleared (
  IN  IIO_GLOBALS             *IioGlobalData,
  IN  UINT8                   IioIndex
  )
{
  if (IsCpuAndRevision (CPU_ICXSP, REV_ALL)) {
    return TRUE;
  }
  return FALSE;
}

/**
  Routine contains common cross processor workarounds

  @param IioGlobalData - IIO Global data structure
  @param IioIndex      - IIO index

  @retval None
 */
VOID
IioCommon10nmMiscWAs (
  IN  IIO_GLOBALS             *IioGlobalData,
  IN  UINT8                   IioIndex
  )
{
  UINT8                                 BusBase;
  UINT8                                 StackIndex;

  OTCPIPEHAZARD_IIO_DFX_VTD_STRUCT      Otcpipehazard;
  ITCPIPEHAZARD_IIO_DFX_VTD_STRUCT      Itcpipehazard;
  IRP_MISC_DFX0_IIO_DFX_VTD_STRUCT      IrpMiscDfx0;
  DFX_HVM_IIO_DFX_VTD_STRUCT            Dfx_Hvm_Iio_Dfx_Cnx_Knh;
  ITCERRDIS_IIO_DFX_VTD_STRUCT          ItcErrDis;
  ITCCTRL89_N0_IIO_VTD_STRUCT           ItcCtrl89N0;
  ITCCTRL89_N1_IIO_VTD_STRUCT           ItcCtrl89N1;
  SPARE_IOTC_IIO_DFX_VTD_STRUCT         SpareIotcReg;
  IRP_MISC_DFX2_IIO_DFX_VTD_STRUCT      IrpMiscDfx2;

  BusBase = IioGlobalData->IioVar.IioVData.SocketBaseBusNumber[IioIndex];

  // The following is common for IVT, HSX
  // s3612991: Workaround for IIO is supposed to handle unimplemented functions.
  // Chicken bit must be set for all steppings.
  for (StackIndex = 0; StackIndex < MAX_IIO_STACK ; StackIndex++) {
    // Check for a valid stack
    if (!(IioGlobalData->IioVar.IioVData.StackPresentBitmap[IioIndex] & (1 << StackIndex))) {
      continue;
    }

    Dfx_Hvm_Iio_Dfx_Cnx_Knh.Data = IioReadCpuCsr32 (IioIndex, StackIndex, DFX_HVM_IIO_DFX_VTD_REG);
    //
    // 4168945: PCIE:  L0 20-lane reduced part hides Device 1 function 0
    //
    Dfx_Hvm_Iio_Dfx_Cnx_Knh.Bits.cfg_invalid_dev_mmio_chk_dis = 1;
    Dfx_Hvm_Iio_Dfx_Cnx_Knh.Bits.spare_cfg = 2;
    IioWriteCpuCsr32 (IioIndex, StackIndex, DFX_HVM_IIO_DFX_VTD_REG, Dfx_Hvm_Iio_Dfx_Cnx_Knh.Data);

    if (IioGlobalData->SetupData.DfxOtcPipeHazardThresh == 0) {
      //
      // OOBMSM sub-agent can miss indication for data for Cfg CplD if followed by certain messages within 4 cycles
      //
      if (IsSiliconWorkaroundEnabled ("S1407304690")) {
        if (IsVMDEnabledForStack (IioIndex, StackIndex)) {
          Otcpipehazard.Data = IioReadCpuCsr32 (IioIndex, StackIndex, OTCPIPEHAZARD_IIO_DFX_VTD_REG);
          Otcpipehazard.Bits.enable = 1;
          Otcpipehazard.Bits.mode = 0x6;
          Otcpipehazard.Bits.lteq = 0;
          Otcpipehazard.Bits.thresh = 4;
          Otcpipehazard.Bits.trigger = 0x10;
          Otcpipehazard.Bits.block = 0x10;
          IioWriteCpuCsr32 (IioIndex, StackIndex, OTCPIPEHAZARD_IIO_DFX_VTD_REG, Otcpipehazard.Data);
        }
      }
    }

    if (IsSiliconWorkaroundEnabled ("S2207539610")) {
      ItcErrDis.Data = IioReadCpuCsr32 (IioIndex, StackIndex,  ITCERRDIS_IIO_DFX_VTD_REG);
      ItcErrDis.Bits.itc_par_addr_rf = 0x1;
      IioWriteCpuCsr32 (IioIndex, StackIndex, ITCERRDIS_IIO_DFX_VTD_REG, ItcErrDis.Data);
    }
    //
    // ITC does not handle canceling for confined p2p posted three clock cycles after being rejected - for SNR A0.
    // Program the pipe-hazard/serialization DFX hook to prevent bug exposure prior to enabling DMA requests from IO.
    // Below combined values are to handle the above case and when:
    //   Lock can hang in StopReq2 phase in M2IOSF when prefetch arb is incorrectly cleared
    //
    if (IsSiliconWorkaroundEnabled ("S2209267204") || IsSiliconWorkaroundEnabled ("S2007748157")) {
      Itcpipehazard.Data = IioReadCpuCsr32 (IioIndex, StackIndex, ITCPIPEHAZARD_IIO_DFX_VTD_REG);
      Itcpipehazard.Bits.enable = 1;
      Itcpipehazard.Bits.mode = 3;
      Itcpipehazard.Bits.lteq = 1;
      if (IsSiliconWorkaroundEnabled ("S2209267204")) {
        Itcpipehazard.Bits.thresh = 0x5;   // latest value for "S2209267204"
      } else {
        Itcpipehazard.Bits.thresh = 0xa;   // earlier version of the WA: "S2007748157"
      }

      Itcpipehazard.Bits.trigger = 4;
      Itcpipehazard.Bits.block = 0x50;
      IioWriteCpuCsr32 (IioIndex, StackIndex, ITCPIPEHAZARD_IIO_DFX_VTD_REG, Itcpipehazard.Data);
    }

    //
    // 4032862: HSX Clone: SC: Completion starved at head of switch queue, many MWr to loopback address with ACS bits cleared
    // 4168495: HSX Clone: SC: Completion starved at head of switch queue, many MWr to loopback address with ACS bits cleared
    //
    IrpMiscDfx0.Data = IioReadCpuCsr32 (IioIndex, StackIndex, IRP_MISC_DFX0_IIO_DFX_VTD_REG);
    if (IsSiliconWorkaroundEnabled ("S1707072739")) {
      if ((IioGlobalData->IioVar.IioVData.StackId[IioIndex][StackIndex] == IIO_CPK_NAC) ||
          (IioGlobalData->IioVar.IioVData.StackId[IioIndex][StackIndex] == IIO_CPM_NAC) ||
          (IioGlobalData->IioVar.IioVData.StackId[IioIndex][StackIndex] == IIO_HQM)){
        IrpMiscDfx0.Bits.p_pf_snp_rsp_holdoff_cnt = 0xa;
      }
    } else {
      IrpMiscDfx0.Bits.p_pf_snp_rsp_holdoff_cnt = IioGlobalData->SetupData.SnoopResponseHoldOff;
    }
    IioWriteCpuCsr32 (IioIndex, StackIndex, IRP_MISC_DFX0_IIO_DFX_VTD_REG, IrpMiscDfx0.Data);

    if (IsSiliconWorkaroundEnabled ("S2206780810")) {
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

    if (IsSiliconWorkaroundEnabled ("S2207862491")) {
      //
      // WA S2207862491: If Vt-d is enabled or VMD is enabled
      //              program spare_iotc_1_0_3_cfg.spare_defeature[16]=1 for ICX-R0/SNR-A0
      //
      if (IsVMDEnabledForStack (IioIndex, StackIndex) || IioGlobalData->SetupData.VTdSupport) {
        SpareIotcReg.Data = IioReadCpuCsr32 (IioIndex, StackIndex, SPARE_IOTC_IIO_DFX_VTD_REG);
        SpareIotcReg.Bits.spare_defeature |= BIT16;
        IioWriteCpuCsr32 (IioIndex, StackIndex, SPARE_IOTC_IIO_DFX_VTD_REG, SpareIotcReg.Data);
      }
    }

    if (IsSiliconWorkaroundEnabled ("S1707089812")) {
      //
      // Prevent deadlock case when testing set-conflicts and CLTT
      //
      IrpMiscDfx2.Data = IioReadCpuCsr32 (IioIndex, StackIndex, IRP_MISC_DFX2_IIO_DFX_VTD_REG);
      IrpMiscDfx2.Bits.p_pf_timeout_cnt = 0x11;
      IioWriteCpuCsr32 (IioIndex, StackIndex, IRP_MISC_DFX2_IIO_DFX_VTD_REG, IrpMiscDfx2.Data);
    }

  }
}

/**
  This routine is to address the post RWO settings of the IIOMISCCTRL global register

    @param IioGlobalData  - Pointer to IIO_GLOBALS
    @param IioIndex       - Index to CPU/IIO

    @retval None
*/
VOID
IioCommon10nmPortEnumCtrlWA (
  IN  IIO_GLOBALS             *IioGlobalData,
  IN  UINT8                   IioIndex
  )
{
  UINT8                         PortInStack;
  UINT8                         MaxPortNumber;
  UINT8                         MaxStackSocket;
  UINT8                         Stack;
  UINT8                         PortIndex;
  PCI_ROOT_BRIDGE_PCI_ADDRESS   PciAddress;
  UINT8                         IoBas;

  IIOMISCCTRL_N0_IIO_VTD_STRUCT IioMiscCtrlN010nm;
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
      if (IioDmiIsPortIdentity (IioIndex, PortIndex, IioDmiPort)) {
        IioMiscCtrlN1.Data = IioReadCpuCsr32 (IioIndex, Stack, IIOMISCCTRL_N1_IIO_VTD_REG);
        IioMiscCtrlN1.Bits.csr_wrcache_snapshot_drain_during_lock = 1;
        IioWriteCpuCsr32 (IioIndex, Stack, IIOMISCCTRL_N1_IIO_VTD_REG, IioMiscCtrlN1.Data);
      }

      //
      //Invalid 'I/O addressing Capability' implemented in II-O chipset (3614969, 3876211, 3247349)
      //
      if (IioDmiIsPortIdentity (IioIndex, PortIndex, IioDmiPort)) {  //Skip DMI port
        continue;
      }

      if (IsCpuAndRevision (CPU_SNR, REV_ALL)) {
        //
        // This is possible for Gen3 only
        //

        //
        // setting EN1K bit
        //
        IioMiscCtrlN010nm.Data = IioReadCpuCsr32 (IioIndex, Stack, IIOMISCCTRL_N0_IIO_VTD_REG);
        IioMiscCtrlN010nm.Bits.en1k = 1;
        IioWriteCpuCsr32 (IioIndex, Stack, IIOMISCCTRL_N0_IIO_VTD_REG,IioMiscCtrlN010nm.Data);

        //
        // Clear IoBas more_i_o_base_address
        // This bit is present only in Gen3 controller
        //
        GetPciePortBusDeviceFunction (IioIndex, PortIndex, &PciAddress);
        IoBas = IioPciExpressRead8 (IioIndex, PciAddress.Bus, PciAddress.Device, PciAddress.Function, R_PCIE_PORT_IOBAS);
        IoBas &= ~B_PCIE_PORT_MOREIO;
        IioPciExpressWrite8 (IioIndex, PciAddress.Bus, PciAddress.Device, PciAddress.Function, R_PCIE_PORT_IOBAS, IoBas);

        //
        // clear EN1K bit
        //
        IioMiscCtrlN010nm.Data = IioReadCpuCsr32 (IioIndex, Stack, IIOMISCCTRL_N0_IIO_VTD_REG);
        IioMiscCtrlN010nm.Bits.en1k = 0;
        IioWriteCpuCsr32 (IioIndex, Stack, IIOMISCCTRL_N0_IIO_VTD_REG, IioMiscCtrlN010nm.Data);
      } // !IsCpuAndRevision (CPU_TNR, REV_ALL)

    } // foreach PortInStack

    //
    // Allow outbound requests sent to the MS2IOSF during lock to PCIe.
    //
    IioMiscCtrlN1.Data = IioReadCpuCsr32 (IioIndex, Stack, IIOMISCCTRL_N1_IIO_VTD_REG);
    IioMiscCtrlN1.Bits.csr_en_dns_txn_during_lock = 1;
    IioWriteCpuCsr32 (IioIndex, Stack, IIOMISCCTRL_N1_IIO_VTD_REG, IioMiscCtrlN1.Data);
  }// End of for (Stack = 0; Stack < MAX_IIO_STACK; Stack++)
}
