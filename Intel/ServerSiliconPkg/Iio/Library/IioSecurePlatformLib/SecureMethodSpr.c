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
#include <IndustryStandard/Pci22.h>
#include <Library/IioDataHubLib.h>
#include <Library/IioAccessLib.h>
#include <RcRegs.h>
#include <Library/IioDmiLib.h>
#include <Chip/Include/CpuPciAccess.h>
#include <Library/IioSysInfoLib.h>
#include <IioSetupDefinitions.h>
#include <Library/PciMmio32AssignLib.h>
#include <Library/IioDataHubLib.h>
#include <Library/IioPcieInit.h>
#include <Library/KtiApi.h>
#include <Library/CxlIpLib.h>

#include <LibraryPrivate/IioDmiConf.h>
#include <LibraryPrivate/IioDebug.h>



/**

   Function that writes all the CSRs and MSRs defined in the SeCoE BIOS
   recommendations document to allow the ACM to later do and SCHECK and
   find the platform to be secure.

// ********************************************************************
// ******************   WARNING  **************************************
// *******  Settings within this function must not be changed *********
// *******  without reviewing the changes with SeCoE or the   *********
// *******  TXT BIOS owner.  Changes to this function will    *********
// *******  produce corresponding failures to the ACM Lock    *********
// *******  Configuration Checks and will prevent Trusted     *********
// *******  Boot.                                             *********
// ********************************************************************
// ********************************************************************

    @param IioGlobalData - Pointer to IIO_GLOBALS

    @retval None

**/
IIO_PORT_HIDE_REGISTER_BIT PortHideCovertTable[] = {
  {DEVHIDE_SB_PRVT_PCIE_G5_REG, BIT0},  //PORT_1A_INDEX_SPR,
  {DEVHIDE_SB_PRVT_PCIE_G4_REG, BIT3},  //PORT_1B_INDEX_SPR,
  {DEVHIDE_SB_PRVT_PCIE_G5_REG, BIT1},  //PORT_1C_INDEX_SPR,
  {DEVHIDE_SB_PRVT_PCIE_G4_REG, BIT2},  //PORT_1D_INDEX_SPR,
  {DEVHIDE_SB_PRVT_PCIE_G5_REG, BIT2},  //PORT_1E_INDEX_SPR,
  {DEVHIDE_SB_PRVT_PCIE_G4_REG, BIT1},  //PORT_1F_INDEX_SPR,
  {DEVHIDE_SB_PRVT_PCIE_G5_REG, BIT3},  //PORT_1G_INDEX_SPR,
  {DEVHIDE_SB_PRVT_PCIE_G4_REG, BIT0}   //PORT_1H_INDEX_SPR,
};

VOID
TxtSecurePlatform (
  IN  IIO_GLOBALS    *IioGlobalData,
  IN  UINT8          IioIndex
  )
{
  TSWCTL0_IIO_DFX_VTD_STRUCT                TswCtl0;
  UINT8                                     Stack;

  IIO_D_DBG ("IioInit Secure the Platform (TXT)..\n");
  for (Stack = 0; Stack < MAX_IIO_STACK; Stack++) {
    if (!(IioGlobalData->IioVar.IioVData.StackPresentBitmap[IioIndex] & (1 << Stack))){
      continue;
    }
    //
    // HSD 4166541: Implement HSX/BDX Processor Security Locks as per SeCoE Recommendation
    //
    TswCtl0.Data = IioReadCpuCsr32 (IioIndex, Stack, TSWCTL0_IIO_DFX_VTD_REG);
    TswCtl0.Bits.me_disable = 1;
    TswCtl0.Bits.allow_ib_mmio_cfg = 0;
    TswCtl0.Bits.ignore_acs_p2p_ma_lpbk = 0;
    IioWriteCpuCsr32 (IioIndex, Stack, TSWCTL0_IIO_DFX_VTD_REG, TswCtl0.Data);
  } // End of for (Stack; Stack <MAX_IIO_STACK; Stack ++)


}

/**
   Function that writes the CSR that enables PECI as trusted interface.
   HSD 4166549: Implement Power Management Security Configuration as per SeCoE recommendation

    @param pIioGlobalData - Pointer to USD

    @retval None
**/
VOID
TxtTrustPeci (
  IN  IIO_GLOBALS *IioGlobalData,
  IN  UINT8       IioIndex
  )
{
}

/**
    This function will hide BDFs under M2IOSF by setting the disable bits under each IP

    @param IioGlobalData - Pointer to IIO_GLOBALS
    @param IioIndex      - Index to CPU/IIO
    @param IioDevFuncHide - Pointer to every BDF to be hidden

    @retval None
**/
VOID
IioDevFuncHideM2iosf (
  IN IIO_GLOBALS                *IioGlobalData,
  IN UINT8                      IioIndex,
  IN IIO_DEVFUNHIDE_TABLE       *IioDevFuncHide
  )
{
  FNHIDE_IIO_VTD_STRUCT       FnHideMs2iosf;
  DEVHIDE_SB_PRVT_PCIE_G4_STRUCT PciePrivPortDis;
  UINT32 ClassCode;
  UINT32 DevHideData;
  UINT8  MaxIioStack;
  UINT8  PciePortStack;
  UINT8  StackNum;
  UINT8  BusNum;
  UINT8  DevNum;
  UINT8  FuncNum;
  UINT32 RegisterOffset;

  MaxIioStack = GetMaxStackNumPerSocket ();
  for (StackNum = 0; StackNum < MaxIioStack; StackNum++) {
    if (!(IioGlobalData->IioVar.IioVData.StackPresentBitmap[IioIndex] & (1 << StackNum))) {
      continue;
    }
    for (FuncNum = 0; FuncNum < MAX_FUNC_NUM; FuncNum++) {
      DevHideData = IioDevFuncHide[IioIndex].IioStackDevHide[StackNum].DevToHide[FuncNum];
      for (DevNum = 0; DevNum < MAX_DEV_NUM; DevNum++) {
        if ((DevHideData >> DevNum) & BIT0) {
          BusNum = IioGlobalData->IioVar.IioVData.SocketStackBus[IioIndex][StackNum];
          ClassCode = (UINT32) IioPciExpressRead8 ( IioIndex, BusNum, DevNum, FuncNum, R_PCIE_PORT_CCR_N0);
          ClassCode |= (UINT32) (IioPciExpressRead16 ( IioIndex, BusNum, DevNum, FuncNum, R_PCIE_PORT_CCR_N1) << 8);

          //
          // Check if BDF to hide corresponds to DFX
          //
          if (ClassCode == (PCI_CLASS_SYSTEM_PERIPHERAL << 16 | PCI_SUBCLASS_PERIPHERAL_OTHER << 8 | 0)) {
            FnHideMs2iosf.Data = IioReadCpuCsr8 (IioIndex, StackNum, FNHIDE_IIO_VTD_REG);
            FnHideMs2iosf.Bits.function3_hide = 1;    // Hide DFX device
            IIO_D_DBG ("Hiding DFX on stack [%d] Fnhide [0x%x]\n", StackNum, FnHideMs2iosf.Data);
            IioWriteCpuCsr8 (IioIndex, StackNum, FNHIDE_IIO_VTD_REG, FnHideMs2iosf.Data);
          }

          //
          // Check if BDF to hide corresponds to PCIE root port
          //
          if (ClassCode == (PCI_CLASS_BRIDGE << 16 | PCI_CLASS_BRIDGE_P2P << 8 | PCI_IF_BRIDGE_P2P)) {
            if (DevNum > sizeof (PortHideCovertTable) / sizeof (IIO_PORT_HIDE_REGISTER_BIT)) {
              IIO_D_ERR (" It is out of bound of PortHideCovertTable with DevNum %x\n", DevNum);
              ASSERT (FALSE);
              continue;
            }
            PciePortStack = DevNum - PCIE_PORT_A_DEV_SPR;
            RegisterOffset = PortHideCovertTable[PciePortStack].DevHideRegister;
            PciePrivPortDis.Data = IioReadCpuCsr32 (IioIndex, StackNum, RegisterOffset);
            PciePrivPortDis.Data|= PortHideCovertTable[PciePortStack].DevHideBit;
            IIO_D_DBG ("  Hiding root port: [0x%x] [%d] [%d]\n", BusNum, DevNum, FuncNum);
            IIO_D_DBG ("  PciePrivPortDis.Data: 0x%x\n", PciePrivPortDis.Data);
            IioWriteCpuCsr32 (IioIndex, StackNum, RegisterOffset, PciePrivPortDis.Data);
          } // End if Pcie root port detection

          //
          // Check if BDF to hide corresponds to VMD
          //
          if (ClassCode == V_MASS_STORAGE_CCR_RAID_CTRL) {
            FnHideMs2iosf.Data = IioReadCpuCsr8 (IioIndex, StackNum, FNHIDE_IIO_VTD_REG);
            FnHideMs2iosf.Bits.function5_hide = 1;
            IioWriteCpuCsr8 (IioIndex, StackNum, FNHIDE_IIO_VTD_REG, FnHideMs2iosf.Data);
          } // End if Vmd detection

        } // End if ((DevHideData >> DevNum) & BIT0)
      } // End DevNum For
    } // End FuncNum For

  } // End StackNum For
}

IIO_DEVFUNHIDE DevHideTargetOffsetIio[] = {
   {DEVHIDE0_UBOX_MISC_REG, DEVHIDE1_UBOX_MISC_REG, DEVHIDE2_UBOX_MISC_REG, DEVHIDE3_UBOX_MISC_REG, DEVHIDE4_UBOX_MISC_REG, DEVHIDE5_UBOX_MISC_REG, DEVHIDE6_UBOX_MISC_REG, DEVHIDE7_UBOX_MISC_REG},
   {DEVHIDE0_1_UBOX_MISC_REG, DEVHIDE1_1_UBOX_MISC_REG, DEVHIDE2_1_UBOX_MISC_REG, DEVHIDE3_1_UBOX_MISC_REG, DEVHIDE4_1_UBOX_MISC_REG, DEVHIDE5_1_UBOX_MISC_REG, DEVHIDE6_1_UBOX_MISC_REG, DEVHIDE7_1_UBOX_MISC_REG},
   {0,0,0,0,0,0,0,0}
};

/**
  This function will hide devices on uncore stacks using DEVHIDEx_UBOX_MISC registers.

  @param[in] IioGlobalData  - Pointer to IIO_GLOBALS
  @param[in] IioIndex       - Index to CPU/IIO
  @param[in] IioDevFuncHide - Pointer to every BDF to be hidden
**/
VOID
IioDevHideUncore (
  IN IIO_GLOBALS              *IioGlobalData,
  IN UINT8                    IioIndex,
  IN IIO_DEVFUNHIDE_TABLE     *IioDevFuncHide
  )
{
  DEVHIDE1_UBOX_MISC_STRUCT   DevHideData;
  UINT32                      DevHideReg;
  UINT8                       Stack;
  UINT8                       FuncNum;
  UINT32                      DevsToHide;

  //
  // Last two stacks of every socket are the Uncore ones for 10nm's CPUs
  //
  for (Stack = 0; Stack < MAX_UNCORE_STACK; Stack++) {
    if (!(DevHideTargetOffsetIio[Stack].DevToHide[0])) {
      IIO_D_ERR ("[%d.%d] DEVHIDE registers not defined for this stack\n", IioIndex, Stack);
      break;
    }
    for (FuncNum = 0; FuncNum < NUM_DEVHIDE_REGS_PER_STACK; FuncNum++) {
      DevsToHide = IioDevFuncHide[IioIndex].UncoreStackDevHide[Stack].DevToHide[FuncNum];
      if (DevsToHide == 0) {
        //
        // There is nothing to hide
        //
        continue;
      }

      IIO_D_PCIDBG ("[%d] Hide Uncore-stack %d func: %d devs:0x%X\n", IioIndex, Stack, FuncNum, DevsToHide);

      DevHideReg = DevHideTargetOffsetIio[Stack].DevToHide[FuncNum];
      DevHideData.Data = IioReadCpuCsr32 (IioIndex, 0, DevHideReg);
      DevHideData.Data |= DevsToHide;
      IioWriteCpuCsr32 (IioIndex, 0, DevHideReg, DevHideData.Data);
    } // End FuncNum For
  } // End Stack For
}

/**
  This function will set PLKCTL.CL bit to set any RW capability bits to RO for NTB.

  @param[in] IioIndex       - Index to the current Socket
  @param[in] StackIndex     - Index to the current Stack
**/
VOID
NtbPersonalityLockEnable (
  IN  UINT8                 IioIndex,
  IN  UINT8                 StackIndex
  )
{
  UINT8                           NtbIndex;
  PLKCTL_IIO_PCIENTB_STRUCT       PlkCtl;

  NtbIndex = StackIndex - 1;
  PlkCtl.Data = IioReadCpuCsr16 (IioIndex, NtbIndex, PLKCTL_IIO_PCIENTB_REG);
  PlkCtl.Bits.cl = 1;
  IioWriteCpuCsr16 (IioIndex, NtbIndex, PLKCTL_IIO_PCIENTB_REG, PlkCtl.Data);
}

/**
  This function will set PLKCTL.CL bit to set any RW capability bits to RO for CXL.

  @param[in] IioGlobalData  - Pointer to IIO_GLOBALS
  @param[in] IioIndex       - Index to the current Socket
  @param[in] StackIndex     - Index to the current Stack
**/
VOID
CxlPersonalityLockEnable (
  IN  IIO_GLOBALS           *IioGlobalData,
  IN  UINT8                 IioIndex,
  IN  UINT8                 StackIndex
  )
{
  UINT8                     CxlIndex;
  PLKCTL_IIO_CXL_STRUCT     PlkCtl;

  CxlIndex = StackIndex - 1;
  PlkCtl.Data = IioReadCpuCsr16 (IioIndex, CxlIndex, PLKCTL_IIO_CXL_REG);
  PlkCtl.Bits.cl = 1;
  IioWriteCpuCsr16 (IioIndex, CxlIndex, PLKCTL_IIO_CXL_REG, PlkCtl.Data);
}

/**
  This function will set PLKCTL.CL bit to set any RW capability bits to RO for PCIe.

  @param[in] IioGlobalData  - Pointer to IIO_GLOBALS
  @param[in] IioIndex       - Index to the current Socket
  @param[in] StackIndex     - Index to the current Stack
**/
VOID
PciePersonalityLockEnable (
  IN  IIO_GLOBALS           *IioGlobalData,
  IN  UINT8                 IioIndex,
  IN  UINT8                 StackIndex
  )
{
  UINT8                           PortOffset;
  UINT8                           PortIndex;
  UINT8                           MaxPortNumPerIou;
  IIO_CONFIG                      *SetupData;
  PLKCTL_IIO_PCIE_G5_STRUCT       PlkCtl;


  SetupData = GetIioConfig ();
  MaxPortNumPerIou = GetMaxPortNumPerIou ();

  for (PortOffset = 0; PortOffset < MaxPortNumPerIou; PortOffset++) {
    PortIndex = GetPortIndexbyStack (StackIndex, PortOffset);
    if (!IioVData_ConfigurePciePort (IioGlobalData, IioIndex, PortIndex)) {
      continue;
    }
    PlkCtl.Data = IioReadCpuCsr16 (IioIndex, PortIndex, PLKCTL_IIO_PCIE_G5_REG);
    PlkCtl.Bits.cl = 1;
    IioWriteCpuCsr16 (IioIndex, PortIndex, PLKCTL_IIO_PCIE_G5_REG, PlkCtl.Data);
  }
}

/**
  This function will program the remain CXL settings before BIOS ends.

  @param[in] IioIndex       - Index to the current IIO (Socket)
**/
VOID
CxlEndOfDxeEvent (
  IN  UINT8                 IioIndex
  )
{
  UINT8                     StackIndex;

  for (StackIndex = 0; StackIndex < MAX_CXL_PER_SOCKET; StackIndex ++) {
    if (GetCxlStatus (IioIndex, StackIndex) == AlreadyInCxlMode) {
      //
      // Lock the CXL DP's CXL.Arb-Mux secured registers
      //
      SetCxlArbMuxSrl (IioIndex, StackIndex);
    }
  }
}

/**
  This function set PLKCTL.CL lock.
  It makes many PCIe port registers RO.

  @param[in] IioGlobalData  Pointer to IIO_GLOBALS
  @param[in] IioIndex       Index to the current IIO (Socket)
**/
VOID
IioLockPersonalityRegs (
  IN  IIO_GLOBALS           *IioGlobalData,
  IN  UINT8                 IioIndex
  )
{
  UINT8               MaxIioStack;
  UINT8               StackIndex;
  PI5_MODES           StackMode;
  UINT8               PortIndex;

  IIO_D_LOG ("Lock the CXL.Arb-Mux secured register if there is any CXL port.\n");
  CxlEndOfDxeEvent (IioIndex);

  MaxIioStack = GetMaxIouNumPerSocket () + 1; // PCIE + DMI
  for (StackIndex = 0; StackIndex < MaxIioStack; StackIndex++) {
    StackMode = GetStackMode (IioGlobalData, IioIndex, StackIndex);
    switch (StackMode) {
    case PI5_MODE_DMI:
      PortIndex = GetPortIndexbyStack (StackIndex, 0);
      IioDmiPersonalityLockEnable (IioIndex, PortIndex);
      break;
    case PI5_MODE_NTB:
      NtbPersonalityLockEnable (IioIndex, StackIndex);
      break;
    case PI5_MODE_IAL:
      if (GetCxlStatus (IioIndex, StackIndex) == AlreadyInCxlMode) {
        CxlPersonalityLockEnable (IioGlobalData, IioIndex, StackIndex);
        break;
      }
    case PI5_MODE_GEN4_ONLY:
    case PI5_MODE_GEN5_AND_GEN4:
      PciePersonalityLockEnable (IioGlobalData, IioIndex, StackIndex);
      break;
    default:
      IIO_D_LOG ("PI5 Mode is invalid!\n");
      break;
    }
  }
}

