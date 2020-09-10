/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation. <BR>

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

#include <Uefi.h>
#include <RcRegs.h>

#include <IndustryStandard/PciExpress21.h>

#include <Library/DebugLib.h>
#include <Library/CpuAndRevisionLib.h>
#include <Library/SiliconWorkaroundLib.h>

#include <Library/IioAccessLib.h>
#include <Library/IioDataHubLib.h>

#include <LibraryPrivate/IioDebug.h>
#include <LibraryPrivate/IioDmiConf.h>


/**
  Lock DMI port capabilities

  @param[in]  IioIndex   Socket/IIO complex index
  @param[in]  PortIndex  Index of the port
 */
VOID
IioDmiPersonalityLockEnable (
  IN  UINT8                         IioIndex,
  IN  UINT8                        PortIndex
  )
{
  PLKCTL_IIO_PCIEDMI_STRUCT       PlkCtl;

  PlkCtl.Data = IioReadCpuCsr16 (IioIndex, PortIndex, PLKCTL_IIO_PCIEDMI_REG);
  PlkCtl.Bits.cl = 1;
  IioWriteCpuCsr16 (IioIndex, PortIndex, PLKCTL_IIO_PCIEDMI_REG, PlkCtl.Data);
}

/**
  Disable DMI on given socket

  @param[in]  IioIndex  Socket/IIO complex index
 */
VOID
DisableDmiOnSocket (
  IN UINT8 IioIndex
  )
{
  //
  // Not yet implemented
  //
}

/**
  Returns TRUE if multiPch is running on this socket

  @param[in]  IioIndex  Socket/IIO complex index
  @return TRUE if multiPCH is running on this socket
 */
BOOLEAN
IsMultiPchRunningOnThisSocket (
  IN UINT8 IioIndex
  )
{
  //
  // Not yet implemented
  //
  return FALSE;
}

/**
  This function locks the North Bridge (Dmi, Rlink or other IP) LogPHY space,
  setting Secured Register Lock field in LCFG register.
 */
VOID
LockLogicalPhyNorthBridge (
  VOID
  )
{
  //
  // There is no implementation yet - not needed?
  //
}

/**
  Disable gen. 2 override bit so that DMI will train to gen. 3
  Needed only for selected CPUs.

  @param[in]  IioIndex   Socket/IIO complex index
  @param[in]  PortIndex  Index of the port
*/
VOID
DisableGen2Override (
  IN UINT8 IioIndex,
  IN UINT8 PortIndex
  )
{
  //
  // Nothing is needed here
  //
}

/**
  Program DMI PCI command register:
    - set BME/MSE bits
    - disable interrupts

  @param[in]  IioIndex   Socket/IIO complex index
  @param[in]  PortIndex  Index of the port
*/
VOID
IioDmiProgramCmdRegister (
  IN UINT8 IioIndex,
  IN UINT8 PortIndex
  )
{
  PCICMD_IIO_PCIEDMI_STRUCT    PciCmd;

  PciCmd.Data = IioReadCpuCsr16 (IioIndex, PortIndex, PCICMD_IIO_PCIEDMI_REG);
  PciCmd.Bits.bme = 1;
  PciCmd.Bits.mse = 1;
  PciCmd.Bits.intxd = 1;
  IioWriteCpuCsr16 (IioIndex, PortIndex, PCICMD_IIO_PCIEDMI_REG, PciCmd.Data);
}

/**
  Write back INT-PIN register to lock its value

  @param[in]  IioIndex   Socket/IIO complex index
  @param[in]  PortIndex  Index of the port
*/
VOID
LockIntPinRegister (
  IN UINT8 IioIndex,
  IN UINT8 PortIndex
  )
{
  INTP_IIO_PCIEDMI_STRUCT      IntPin;

  IntPin.Data = IioReadCpuCsr8 (IioIndex, PortIndex, INTP_IIO_PCIEDMI_REG);
  IioWriteCpuCsr8 (IioIndex, PortIndex, INTP_IIO_PCIEDMI_REG, IntPin.Data);
}

/**
  Program DMI MaxPayloadSize value

  @param[in]  IioIndex                Socket/IIO complex index
  @param[in]  PortIndex               Index of the port
  @param[in]  MaxPayloadSizeSupValue  Value to set in maxPayloadSizeSupported field
*/
VOID
IioDmiSetMpss (
  IN UINT8 IioIndex,
  IN UINT8 PortIndex,
  IN UINT8 MaxPayloadSizeSupValue
  )
{
  DEVCAP_IIO_PCIEDMI_STRUCT    DevCap;
  UINT32                       BkpData;

  DevCap.Data = IioReadCpuCsr32 (IioIndex, PortIndex, DEVCAP_IIO_PCIEDMI_REG);

  BkpData = DevCap.Data;
  DevCap.Bits.mpss = MaxPayloadSizeSupValue;

  IioWriteCpuCsr32 (IioIndex, PortIndex, DEVCAP_IIO_PCIEDMI_REG, DevCap.Data);
  IIO_D_DMIDBG ("[%d p%d] DEVCAP_IIO_PCIEDMI %04X -> %04X -> %04X\n", IioIndex, PortIndex,
                BkpData, DevCap.Data, IioReadCpuCsr16 (IioIndex, PortIndex, DEVCAP_IIO_PCIEDMI_REG));
}

/**
  Cleanup ARI bit in DevCap2 register

  SA BSU 0.8.11
  devcap2.ari_en default is "1" for DMI port and it should be set to 0 by BIOS
  This is a permanent workaround

  @param[in]  IioIndex   Socket/IIO complex index
  @param[in]  PortIndex  Index of the port
*/
VOID
IioDmiCleanUpDevCap2AriBit (
  IN UINT8 IioIndex,
  IN UINT8 PortIndex
  )
{
  DEVCAP2_IIO_PCIEDMI_STRUCT   DevCap2;
  UINT32                       BkpData;

  DevCap2.Data = IioReadCpuCsr32 (IioIndex, PortIndex, DEVCAP2_IIO_PCIEDMI_REG);

  BkpData = DevCap2.Data;
  DevCap2.Bits.ari = 0;

  IioWriteCpuCsr32 (IioIndex, PortIndex, DEVCAP2_IIO_PCIEDMI_REG, DevCap2.Data);
  IIO_D_DMIDBG ("[%d p%d] DEVCAP2_IIO_PCIEDMI %04X -> %04X -> %04X\n", IioIndex, PortIndex,
                BkpData, DevCap2.Data, IioReadCpuCsr32 (IioIndex, PortIndex, DEVCAP2_IIO_PCIEDMI_REG));
}

/**
  Program MiscCtrlSts register

  @param[in] IioGlobalData - Pointer to IIO_GLOBALS
  @param[in]  IioIndex   Socket/IIO complex index
  @param[in]  PortIndex  Index of the port
*/
VOID
IioDmiMiscCtrlStsInit (
  IN  IIO_GLOBALS  *IioGlobalData,
  IN  UINT8        IioIndex,
  IN  UINT8        PortIndex
  )
{
  //
  // Nothing is needed here for now
  //
}


/**
  Program PCIe Device Control register in DMI controller.

  @param[in] IioIndex                    Socket/IIO complex index
  @param[in] PortIndex                   Index of the port
  @param[in] UnsupportedRequestsEnabled  Enable/Disable Unsupported Requests bit
  @param[in] MaxPayloadSize              Max payload encoded as in the Device Control register
*/
VOID
IioDmiProgramDevCtrlReg (
  IN UINT8    IioIndex,
  IN UINT8    PortIndex,
  IN BOOLEAN  UnsupportedRequestsEnabled,
  IN UINT8    MaxPayloadSize
  )
{
  DEVCTL_IIO_PCIEDMI_STRUCT DmiDevCtl;
  UINT32                    Bkp;

  DmiDevCtl.Data = IioReadCpuCsr16 (IioIndex, PortIndex, DEVCTL_IIO_PCIEDMI_REG);
  Bkp = DmiDevCtl.Data;
  DmiDevCtl.Bits.urre = (UnsupportedRequestsEnabled != 0);
  DmiDevCtl.Bits.mps = MaxPayloadSize;
  IioWriteCpuCsr16 (IioIndex, PortIndex, DEVCTL_IIO_PCIEDMI_REG, DmiDevCtl.Data);
  IIO_D_DMIDBG ("[%d p%d] DEVCTL_IIO_PCIEDMI %04X -> %04X -> %04X\n", IioIndex, PortIndex, Bkp, DmiDevCtl.Data,
                IioReadCpuCsr16 (IioIndex, PortIndex, DEVCTL_IIO_PCIEDMI_REG));
}


/**
  Program DMI PmCsr register

  @param[in]  IioIndex    Socket/IIO complex index
  @param[in]  PortIndex   Index of the port
  @param[in]  PowerState  Power state register value
*/
VOID
IioDmiProgramPmCsr (
  IN UINT8    IioIndex,
  IN UINT8    PortIndex,
  IN UINT8    PowerState
  )
{
  PMCSR_IIO_PCIEDMI_STRUCT    DmiPmCsr;

  DmiPmCsr.Data = IioReadCpuCsr16 (IioIndex, PortIndex, PMCSR_IIO_PCIEDMI_REG);
  DmiPmCsr.Bits.ps = PowerState;
  IioWriteCpuCsr32 (IioIndex, PortIndex, PMCSR_IIO_PCIEDMI_REG, DmiPmCsr.Data);
}

/**
  Program DMI DevCtrl2 register

  @param[in]  IioIndex                    Socket/IIO complex index
  @param[in]  PortIndex                   Index of the port
  @param[in]  ComplToDis                  CompletionTimeoutDisable register value
  @param[in]  ComplToVal                  CompletionTimeoutValue register value
*/
VOID
IioDmiProgramDevCtr2lReg (
  IN UINT8    IioIndex,
  IN UINT8    PortIndex,
  IN UINT8    ComplToDis,
  IN UINT8    ComplToVal
  )
{
  DEVCTL2_IIO_PCIEDMI_STRUCT  DmiDevCtrl2;

  DmiDevCtrl2.Data = IioReadCpuCsr16 (IioIndex, PortIndex, DEVCTL2_IIO_PCIEDMI_REG);
  //
  // Always clear ARI bit
  //
  DmiDevCtrl2.Bits.arie = 0;

  DmiDevCtrl2.Bits.ctd = ComplToDis;
  DmiDevCtrl2.Bits.ctv = ComplToVal;

  IioWriteCpuCsr16 (IioIndex, PortIndex, DEVCTL2_IIO_PCIEDMI_REG, DmiDevCtrl2.Data);
}

/**
  Enable MSI on DMI Port

  @param[in]  IioIndex    Socket/IIO complex index
  @param[in]  PortIndex   Index of the port
  @param[in]  MsiEnabled  If MSI should be enabled for the port
*/
VOID
IioDmiEnableMsi (
  IN UINT8      IioIndex,
  IN UINT8      PortIndex,
  IN BOOLEAN    MsiEnabled
  )
{
  MSICTL_IIO_PCIEDMI_STRUCT       MsiMsgCtl;

  MsiMsgCtl.Data = IioReadCpuCsr16 (IioIndex, PortIndex, MSICTL_IIO_PCIEDMI_REG);
  MsiMsgCtl.Bits.msie = MsiEnabled ? 1 : 0;
  IioWriteCpuCsr16 (IioIndex, PortIndex, MSICTL_IIO_PCIEDMI_REG, MsiMsgCtl.Data);
}

/**
  Clear vc1m_nosnoopopdis bit for DMI

  @param[in]  IioIndex   Socket/IIO complex index
  @param[in]  PortIndex  Index of the port
*/
VOID
IioDmiClearVc1mNoSnoopopis (
  IN UINT8 IioIndex,
  IN UINT8 PortIndex
  )
{
  TSWCTL5_IIO_DFX_VTD_STRUCT  TswCtl5;

  TswCtl5.Data = IioReadCpuCsr32 (IioIndex, PortIndex, TSWCTL5_IIO_DFX_VTD_REG);
  TswCtl5.Bits.vc1m_ns_disable = 0;
  IioWriteCpuCsr32 (IioIndex, PortIndex, TSWCTL5_IIO_DFX_VTD_REG, TswCtl5.Data);
}

/**
    Program DMI PerfCtrlSts register

    @param[in]  IioGlobalData  Pointer to global Data
    @param[in]  IioIndex       Socket number
    @param[in]  Port           Port number.
**/
VOID
IioDmiProgramPerfCtrlSts (
  IN  IIO_GLOBALS                    *IioGlobalData,
  IN  UINT8                          IioIndex,
  IN  UINT8                          PortIndex
  )
{
  //
  // no implementation for SPR DMI for now
  //
}

/**
  Program RootPortCmd register

  @param[in]  IioIndex       Socket number
  @param[in]  PortIndex      Index of the PCIE port
  @param[in]  CereEn         Enable correctable error reporting
  @param[in]  NfereEn        Enable non fatal error reporting
  @param[in]  FereEn         Enable fatal error reporting
**/
VOID
IioDmiProgramRootPortCmdReg (
  IN  UINT8    IioIndex,
  IN  UINT8    PortIndex,
  IN  BOOLEAN  CereEn,
  IN  BOOLEAN  NFereEn,
  IN  BOOLEAN  FereEn
  )
{
  ROOTERRCMD_IIO_PCIEDMI_STRUCT  RpErrCmd;

  RpErrCmd.Data = IioReadCpuCsr32 (IioIndex, PortIndex, ROOTERRCMD_IIO_PCIEDMI_REG);
  if (CereEn) {
    RpErrCmd.Bits.cere = 1;
  }
  if (NFereEn) {
    RpErrCmd.Bits.nfere = 1;
  }
  if (FereEn) {
    RpErrCmd.Bits.fere = 1 ;
  }
  IioWriteCpuCsr32 (IioIndex, PortIndex, ROOTERRCMD_IIO_PCIEDMI_REG, RpErrCmd.Data);
}

/**
  Set address and enable DmiRcBar

  @param[in]  IioIndex         Socket/IIO complex index
  @param[in]  PortIndex        Index of the port
  @param[in]  DmiRcBarAddress  Address to set in DmiRcBar
*/
VOID
IioDmiEnableDmiRcBar (
  IN UINT8      IioIndex,
  IN UINT8      PortIndex,
  IN UINT32     DmiRcBarAddress
  )
{
  //
  // DmiRcBar is not programming here for SPR
  //
}

/**
  This function lock registers which needs to stay RO later.

  @param[in]  IioIndex         Socket/IIO complex index
  @param[in]  PortIndex        Index of the port
*/
VOID
IioDmiLockRegisters (
  IN UINT8      IioIndex,
  IN UINT8      PortIndex
  )
{
  //
  // Lock Revision ID
  //
  IioWriteCpuCsr8 (IioIndex, PortIndex, RID_IIO_PCIEDMI_REG, IioReadCpuCsr8 (IioIndex, PortIndex, RID_IIO_PCIEDMI_REG));

  //
  // Lock MSINXTPTR register
  //
  IioWriteCpuCsr16 (IioIndex, PortIndex, MSICAPLST_IIO_PCIEDMI_REG,
      IioReadCpuCsr16 (IioIndex, PortIndex, MSICAPLST_IIO_PCIEDMI_REG));

  //
  // Lock PMCSR register
  //
  IioWriteCpuCsr16 (IioIndex, PortIndex, PMCSR_IIO_PCIEDMI_REG,
      IioReadCpuCsr16 (IioIndex, PortIndex, PMCSR_IIO_PCIEDMI_REG));
}

/**
  This function clear Dmi error masks

  @param[in]  IioIndex         Socket/IIO complex index
  @param[in]  PortIndex        Index of the port
*/
VOID
IioDmiClearErorMasks (
  IN UINT8      IioIndex,
  IN UINT8      PortIndex
  )
{
  ERRUNCMSK_IIO_PCIEDMI_STRUCT  UncedMask;
  ERRUNCSTS_IIO_PCIEDMI_STRUCT  UncErrSts;
  LINKSTS_IIO_PCIEDMI_STRUCT    LnkSts;

  UncedMask.Data = IioReadCpuCsr32 (IioIndex, PortIndex, ERRUNCMSK_IIO_PCIEDMI_REG);
  UncedMask.Bits.ptlpem = 1;
  IioWriteCpuCsr32 (IioIndex, PortIndex, ERRUNCMSK_IIO_PCIEDMI_REG, UncedMask.Data);


  //
  // Clear expected PCIe errors
  //
  UncErrSts.Data = IioReadCpuCsr32 (IioIndex, PortIndex, ERRUNCSTS_IIO_PCIEDMI_REG);
  UncErrSts.Bits.ure = 1;
  IioWriteCpuCsr32 (IioIndex, PortIndex, ERRUNCSTS_IIO_PCIEDMI_REG, UncErrSts.Data);

  LnkSts.Data = IioReadCpuCsr16 (IioIndex, PortIndex, LINKSTS_IIO_PCIEDMI_REG);
  LnkSts.Bits.labs = 1;
  LnkSts.Bits.lbms = 1;
  IioWriteCpuCsr16 (IioIndex, PortIndex, LINKSTS_IIO_PCIEDMI_REG, LnkSts.Data);

}

/**
  Enable System Error Reporting (SERRE)

  @param[in]  IioIndex         Socket/IIO complex index
  @param[in]  PortIndex        Index of the port
*/
VOID
IioDmiSerreInit (
  IN UINT8      IioIndex,
  IN UINT8      PortIndex
  )
{
  //
  // There is no impl. for Dmi Gen4
  //
}

/**
  Program DMI port LinkCap register

  @param[in]  IioGlobalData    Pointer to the IIO global data
  @param[in]  IioIndex         Socket/IIO complex index
  @param[in]  PortIndex        Index of the port
*/
VOID
IioDmiProgramLinkCap (
  IN IIO_GLOBALS  *IioGlobalData,
  IN UINT8        IioIndex,
  IN UINT8        PortIndex
  )
{
  LINKCAP_IIO_PCIEDMI_STRUCT  DmiLnkCap;
  UINT8                       Aspm;

  //
  // Setup the DMI port Link capability features (which have RW-O attribute)
  //
  DmiLnkCap.Data = IioReadCpuCsr32 (IioIndex, PortIndex, LINKCAP_IIO_PCIEDMI_REG);

  //
  // L1 exit Latency (17:15)
  //
  DmiLnkCap.Bits.l1el = IioGlobalData->SetupData.PcieL1Latency[PortIndex];

  //
  // Set ASPM on value requested in setup
  //
  Aspm =  GetAspmSettingForPort (IioGlobalData, IioIndex, PortIndex);
  if (Aspm != PCIE_ASPM_AUTO) {
    DmiLnkCap.Bits.aspmsup = Aspm;
  }

  //
  // MaxLnkWidth (9:4)
  //
  // If Override Max Link Width was not specified in MaxPXPMap, then copy CurrentPXPMap (bifurcated max width) to MaxPXPMap (override max width)
  if (IioGlobalData->IioVar.IioOutData.MaxPXPMap[PORT_0_INDEX] == 0) {
    IioGlobalData->IioVar.IioOutData.MaxPXPMap[PORT_0_INDEX] =
        IioGlobalData->IioVar.IioOutData.CurrentPXPMap[PORT_0_INDEX];
  }

  DmiLnkCap.Bits.mlw = IioGlobalData->IioVar.IioOutData.MaxPXPMap[PORT_0_INDEX];

  //
  // ivt_eco s4677507: PCIE - Mismatch in advertising link capability between
  //                   lnkcap and lnkcap2 can cause legacy hardware(prior to 3.0
  //                   base PCIE spec) to train only to Gen1.
  //  [cloned to s4031727] - applicable to IVX, HSX, BDX
  //
  // 4925605: BIOS to support DMI Gen3
  //
  DmiLnkCap.Bits.mls = V_PCIE_PORT_PXPLNKCAP_LS_GEN3;

  IioWriteCpuCsr32 (IioIndex, PortIndex, LINKCAP_IIO_PCIEDMI_REG, DmiLnkCap.Data);
}


/**
  Program DMI port LinkCap2 register

  @param[in]  IioGlobalData    Pointer to the IIO global data
  @param[in]  IioIndex         Socket/IIO complex index
  @param[in]  PortIndex        Index of the port
*/
VOID
IioDmiProgramLinkCap2 (
  IN IIO_GLOBALS             *IioGlobalData,
  IN UINT8                    IioIndex,
  IN UINT8                    PortIndex
  )
{
  //
  // Nothing is needed here, Supported Link Speed Vector is RO in gen4 controller.
  //
}


/**
  Program DMI port LinkCtls registers (LinkCtl, LnkCtl2, LinkCtl3)

  @param[in]  IioGlobalData    Pointer to the IIO global data
  @param[in]  IioIndex         Socket/IIO complex index
  @param[in]  PortIndex        Index of the port
  @param[in]  TargetLinkSpeed  Link speed to set in Link Control2 register
*/
VOID
IioDmiProgramLinkCtls (
  IN IIO_GLOBALS             *IioGlobalData,
  IN UINT8                    IioIndex,
  IN UINT8                    PortIndex,
  IN UINT8                    TargetLinkSpeed
  )
{
  LINKCTL_IIO_PCIEDMI_STRUCT  DmiLnkCtl;
  LINKCTL2_IIO_PCIEDMI_STRUCT DmiLnkCtl2;
  LINKCTL3_IIO_PCIEDMI_STRUCT DmiLnkCtl3;
  UINT8                       Aspm;

  //
  // 5332537: [LBG Val Critical] LBG DMI Tx co-efficient update
  //  Program Link Control 3 for DMI for PCH Equalization settings.
  //
  DmiLnkCtl3.Data = IioReadCpuCsr32 (IioIndex, PortIndex, LINKCTL3_IIO_PCIEDMI_REG);
  DmiLnkCtl3.Bits.pe = 1;
  IioWriteCpuCsr32 (IioIndex, PortIndex, LINKCTL3_IIO_PCIEDMI_REG, DmiLnkCtl3.Data);

  //
  // Program Link Control 2
  //
  DmiLnkCtl2.Data = IioReadCpuCsr16 (IioIndex, PortIndex, LINKCTL2_IIO_PCIEDMI_REG);
  DmiLnkCtl2.Bits.ec = IioGlobalData->SetupData.IioPcieConfig.ComplianceMode[PORT_0_INDEX];
  DmiLnkCtl2.Bits.sd = IioGlobalData->SetupData.DeEmphasis[PORT_0_INDEX];
  DmiLnkCtl2.Bits.tls = TargetLinkSpeed;
  IioWriteCpuCsr16 (IioIndex, PortIndex, LINKCTL2_IIO_PCIEDMI_REG, DmiLnkCtl2.Data);

  //
  // Program DMI other link control features
  //
  DmiLnkCtl.Data = IioReadCpuCsr16 (IioIndex, PortIndex, LINKCTL_IIO_PCIEDMI_REG);
  //
  // Set ASPM on value requested in setup
  //
  Aspm =  GetAspmSettingForPort (IioGlobalData, IioIndex, PortIndex);
  if (Aspm != PCIE_ASPM_AUTO) {
    DmiLnkCtl.Bits.aspmctl = Aspm;
  }
  //
  // Program Extended Sync
  //
  DmiLnkCtl.Bits.es = IioGlobalData->SetupData.IioPcieConfig.ExtendedSync[PORT_0_INDEX];
  IioWriteCpuCsr16 (IioIndex, PortIndex, LINKCTL_IIO_PCIEDMI_REG, DmiLnkCtl.Data);
}


/**
  Program DMI port XptDefs registers (XptDef / XptDef2)

  @param[in]  IioIndex         Socket/IIO complex index
  @param[in]  PortIndex        Index of the port
*/
VOID
IioDmiProgramXptDefs (
  IN UINT8        IioIndex,
  IN UINT8        PortIndex
  )
{
  XPTDEF_IIO_PCIEDMI_STRUCT      DmiXptDef;
  XPTDEF2_IIO_PCIEDMI_STRUCT     DmiXptDef2;

   DmiXptDef2.Data = IioReadCpuCsr32 (IioIndex, PortIndex, XPTDEF2_IIO_PCIEDMI_REG);
   DmiXptDef2.Bits.tcpar_chk_dis = 0;
   DmiXptDef2.Bits.tdpar_chk_dis = 0;
   IioWriteCpuCsr32 (IioIndex, PortIndex, XPTDEF2_IIO_PCIEDMI_REG, DmiXptDef2.Data);

   if (IsSiliconWorkaroundEnabled ("S14010950010")) {
     DmiXptDef.Data = IioReadCpuCsr32 (IioIndex, PortIndex, XPTDEF_IIO_PCIEDMI_REG);
     DmiXptDef.Bits.malchkd = 1;
     IioWriteCpuCsr32 (IioIndex, PortIndex, XPTDEF_IIO_PCIEDMI_REG, DmiXptDef.Data);
   }
}

/**
  Returns link status register value for DMI port

  @param[in]  IioIndex   Socket/IIO complex index
  @param[in]  PortIndex  Index of the port
  @return                Register value
*/
UINT16
IioDmiReadLinkStatus (
  IN UINT8 IioIndex,
  IN UINT8 PortIndex
  )
{
  return IioReadCpuCsr16 (IioIndex, PortIndex, LINKSTS_IIO_PCIEDMI_REG);
}

/**
  Set link status register value for DMI port

  @param[in]  IioIndex   Socket/IIO complex index
  @param[in]  PortIndex  Index of the port
  @param[in]  RegValue   Register value to set
*/
VOID
IioDmiWriteLinkStatus (
  IN UINT8 IioIndex,
  IN UINT8 PortIndex,
  IN UINT16 RegValue
  )
{
  IioWriteCpuCsr16 (IioIndex, PortIndex, LINKSTS_IIO_PCIEDMI_REG, RegValue);
}

/**
  Returns link status register value for DMI port

  @param[in]  IioIndex   Socket/IIO complex index
  @param[in]  PortIndex  Index of the port
  @return                Register value
*/
UINT16
IioDmiReadLinkControl (
  IN UINT8 IioIndex,
  IN UINT8 PortIndex
  )
{
  return IioReadCpuCsr16 (IioIndex, PortIndex, LINKCTL_IIO_PCIEDMI_REG);
}

/**
  Set link control register value for DMI port

  @param[in]  IioIndex   Socket/IIO complex index
  @param[in]  PortIndex  Index of the port
  @param[in]  RegValue   Register value to set
*/
VOID
IioDmiWriteLinkControl (
  IN UINT8 IioIndex,
  IN UINT8 PortIndex,
  IN UINT16 RegValue
  )
{
  IioWriteCpuCsr16 (IioIndex, PortIndex, LINKCTL_IIO_PCIEDMI_REG, RegValue);
}

/**
  Returns link capabilities2 link speed value for DMI port

  @param[in]  IioIndex   Socket/IIO complex index
  @param[in]  PortIndex  Index of the port
  @return                Register value
*/
UINT8
IioDmiGetLinkSpeedVector (
  IN UINT8 IioIndex,
  IN UINT8 PortIndex
  )
{
  LINKCAP2_IIO_PCIEDMI_STRUCT    DmiLnkCap2;
  DmiLnkCap2.Data = IioReadCpuCsr32 (IioIndex, PortIndex, LINKCAP2_IIO_PCIEDMI_REG);

  return (UINT8) DmiLnkCap2.Bits.slnkspv;
}

/**
  Clean errors which can stay in register after successful link training

  @param[in] IioIndex         Index to the current Socket
  @param[in] PortIndex        Index to the current PCIe Port
**/
VOID
IioDmiClearErrorsAfterLinkTraining (
  IN  UINT8                   IioIndex,
  IN  UINT8                   PortIndex
  )
{
  //
  // Nothing is needed here for now
  //
}

/**
  Return Max Payload Size set for DMI in Device Control register.
  Should be used after IioPostLateInitialize () completed.

  @retval  DMI Max Payload Size as defined in PCIe Device Control register
           (000b - 128B, 001b - 256B, 010b - 512B, 011b - 1024B, 100b - 2048B, 101b - 4096B)
**/
UINT8
IioDmiIpGetMaxPayloadSize (
  VOID
  )
{
  PCI_REG_PCIE_DEVICE_CONTROL  DevCtl;

  DevCtl.Uint16 = IioReadCpuCsr16 (0, 0, DEVCTL_IIO_PCIEDMI_REG);

  return (UINT8) DevCtl.Bits.MaxPayloadSize;
}

/**
  Return Max Payload Size Supported set for DMI in Device Capabilities register.

  @retval  DMI Max Payload Size Supported as defined in PCIe Device Capabilities register
           (000b - 128B, 001b - 256B, 010b - 512B, 011b - 1024B, 100b - 2048B, 101b - 4096B)
**/
UINT8
IioDmiIpGetMaxPayloadSizeSupported (
  VOID
  )
{
  PCI_REG_PCIE_DEVICE_CAPABILITY  DevCap;

  DevCap.Uint32 = IioReadCpuCsr32 (0, 0, DEVCAP_IIO_PCIEDMI_REG);

  return (UINT8) DevCap.Bits.MaxPayloadSize;
}
