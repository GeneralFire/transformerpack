/** @file
  Implement NTB port initialization

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

#include <Base.h>
#include <Uefi.h>
#include <Library/DebugLib.h>

#include <Library/IioDataHubLib.h>
#include <Library/IioAccessLib.h>
#include <RcRegs.h>
#include <Chip/Include/SysHostChip.h>
#ifndef IA32
#include <Library/IoLib.h>
#include <Library/S3BootScriptLib.h>
#endif
#include <Library/CpuAndRevisionLib.h>
#include <Library/IioSysInfoLib.h>
#include <PcieRegs.h>
#include <IioSetupDefinitions.h>
#include <Library/IioPcieInit.h>
#include <Library/IioNtbInit.h>
#include <Library/SiliconWorkaroundLib.h>

#include <LibraryPrivate/IioDebug.h>


/**
  Indicate if NTB Sideband access is supported on the given port index.
  Only valid when NTB is not sharing BDF with PCIe port.

  @param[in] IioIndex       - Current socket index
  @param[in] PortIndex      - Port number being initialized

  @retval TRUE              - NTB sideband registers access supported on given port
  @retval FALSE             - NTB sideband registers access is not supported on given port
**/
BOOLEAN
IsNtbSbActive (
  IN  UINT8                            IioIndex,
  IN  UINT8                            PortIndex
  )
{
  PPD1_SB_PRVT_PCIE_G4_STRUCT          Ppd1SbReg;
  UINT8                                StackIndex = 0;
  UINT8                                NtbPortIndex;
  UINT8                                PciePortOwnership;
  EFI_STATUS                           Status;

  Status = GetPcieStackPerPort (IioIndex, PortIndex, &StackIndex);
  if (Status != EFI_SUCCESS) {
    return FALSE;
  }

  Status = GetNtbPortPerPortIndex (PortIndex, &NtbPortIndex);
  if (Status != EFI_SUCCESS) {
    return FALSE;
  }

  Ppd1SbReg.Data = IioReadCpuCsr32 (IioIndex, StackIndex, PPD1_SB_PRVT_PCIE_G4_REG);
  PciePortOwnership = GetPciePortOwnership (IioIndex, PortIndex);

  if ((Ppd1SbReg.Data != PPD_REG_SB_INVALID_VALUE) &&
      ((PciePortOwnership == NTB_PORT_DEF_NTB_NTB) || (PciePortOwnership == NTB_PORT_DEF_NTB_RP))) {
    return TRUE;
  }
  return FALSE;
}

/**
  Sets the NTB port mode and enables cross link.

  @param IioIndex           Current socket index
  @param PortIndex          Port number being initialized
  @param Ppd                NTB port definition
  @param CrossLinkOverride  Cross link type

**/
VOID
NtbSetPpd (
  IN  UINT8                    IioIndex,
  IN  UINT8                    PortIndex,
  IN  UINT8                    Ppd,
  IN  UINT8                    CrossLinkOverride
)
{
  PPD1_SB_PRVT_PCIE_G4_STRUCT  Ppd1SbReg;
  UINT8                        StackIndex = 0;
  EFI_STATUS                   Status;

  //
  // Program NPD bitfield
  //
  Status = GetPcieStackPerPort (IioIndex, PortIndex, &StackIndex);
  if (Status != EFI_SUCCESS) {
   return;
  }
  Ppd1SbReg.Data = IioReadCpuCsr32(IioIndex, StackIndex, PPD1_SB_PRVT_PCIE_G4_REG);

  if (Ppd == NTB_PORT_DEF_NTB_NTB) {
    Ppd1SbReg.Bits.npd = BIT1;
  }
  if (CrossLinkOverride <= (BIT1 | BIT0)) {
    Ppd1SbReg.Bits.pcfgor = CrossLinkOverride;
  }
  IioWriteCpuCsr32(IioIndex, StackIndex, PPD1_SB_PRVT_PCIE_G4_REG, Ppd1SbReg.Data);
  IIO_D_PCIDBG ("[%d p%d] Ntb is active\n", IioIndex, PortIndex);
}

/**
  Sets the NTB split bar mode.

  @param IioIndex       Current socket index
  @param PortIndex      Port number being initialized
  @param SplitBar       Set split bar mode

**/
VOID
NtbSetSplitBar (
  IN  UINT8  IioIndex,
  IN  UINT8  PortIndex,
  IN  UINT8  SplitBar
  )
{
  // not implemented for SPR
}

/**
    Set NTB ImBar

    @param IioIndex          - Equals to Socket Index
    @param PortIndex         - Equals to Port Index
    @param ImBarSize         - Imbar1 size

    @retval None
**/
VOID
NtbSetImBar (
  IN  UINT8         IioIndex,
  IN  UINT8         NtbIndex,
  IN  UINT8         ImBarSize
  )
{
  IMBAR23SZ_IIO_PCIENTB_STRUCT      ImBar1Sz;
  MBAR23_IP_N0_IIO_PCIENTB_STRUCT   ImBar1BaseN0;
  MBAR23_IP_N1_IIO_PCIENTB_STRUCT   ImBar1BaseN1;

  //
  // Set IMBAR1.
  //
  ImBar1Sz.Data = 0;
  ImBar1Sz.Bits.size = ImBarSize;
  IioWriteCpuCsr8 (IioIndex, NtbIndex, IMBAR23SZ_IIO_PCIENTB_REG, ImBar1Sz.Data);

  ImBar1BaseN0.Data = 0;
  IioWriteCpuCsr32 (IioIndex, NtbIndex, MBAR23_IP_N0_IIO_PCIENTB_REG, ImBar1BaseN0.Data);

  ImBar1BaseN1.Data = 0xff00000;
  IioWriteCpuCsr32 (IioIndex, NtbIndex, MBAR23_IP_N1_IIO_PCIENTB_REG, ImBar1BaseN1.Data);
}

/**
    Set NTB EmBar

    @param IioIndex          - Equals to Socket Index
    @param PortIndex         - Equals to Port Index
    @param NtbPortIndex      - Equals to NTB Port Index
    @param ImBarSize         - Embar1 size

    @retval None
**/
VOID
NtbSetEmBar (
  IN  UINT8         IioIndex,
  IN  UINT8         NtbIndex,
  IN  UINT8         EmBarSize
  )
{
  EMBAR23SZ_IIO_PCIENTB_STRUCT    EmBar1Sz;

  //
  // Set EMBAR1.
  //
  EmBar1Sz.Data = 0;
  EmBar1Sz.Bits.size = EmBarSize;
  IioWriteCpuCsr8 (IioIndex, NtbIndex, EMBAR23SZ_IIO_PCIENTB_REG, EmBar1Sz.Data);
}

/**
    Set NTB Split Imbar
    @param IioIndex          - Equals to Socket Index
    @param PortIndex         - Equals to Port Index
    @param SplitEnable       - If NTB split bar enabled or not
    @param Imbar23Size       - Imbar23 size
    @param Imbar45Size       - Imbar45 size

    @retval None

**/
VOID
NtbSetSplitImBar (
  IN  UINT8         IioIndex,
  IN  UINT8         NtbIndex,
  IN  BOOLEAN       SplitEnable,
  IN  UINT8         Imbar23Size,
  IN  UINT8         Imbar45Size
  )
{
  UINT32                ImBar45BaseN0;
  UINT32                ImBar45BaseN1;


  if (SplitEnable) {
    IioWriteCpuCsr8 (IioIndex, NtbIndex, IMBAR23SZ_IIO_PCIENTB_REG, Imbar23Size);
    IioWriteCpuCsr8 (IioIndex, NtbIndex, IMBAR45SZ_IIO_PCIENTB_REG, Imbar45Size);
  } else {
    ImBar45BaseN0 = 0;
    ImBar45BaseN1 = 0xff00;
    IioWriteCpuCsr8 (IioIndex, NtbIndex, IMBAR45SZ_IIO_PCIENTB_REG, Imbar45Size);
    IioWriteCpuCsr32 (IioIndex, NtbIndex, MBAR45_IP_N0_IIO_PCIENTB_REG, ImBar45BaseN0);
    IioWriteCpuCsr32 (IioIndex, NtbIndex, MBAR45_IP_N1_IIO_PCIENTB_REG, ImBar45BaseN1);
  }
}

/**
    Set NTB Split Embar
    @param IioIndex          - Equals to Socket Index
    @param PortIndex         - Equals to Port Index
    @param SplitEnable       - If NTB split bar enabled or not
    @param Embar23Size       - Embar23 size
    @param Embar45Size       - Embar45 size

    @retval None

**/
VOID
NtbSetSplitEmBar (
  IN  UINT8         IioIndex,
  IN  UINT8         NtbIndex,
  IN  BOOLEAN       SplitEnable,
  IN  UINT8         Embar23Size,
  IN  UINT8         Embar45Size
  )
{

  if (SplitEnable) {
    IioWriteCpuCsr8 (IioIndex, NtbIndex, EMBAR23SZ_IIO_PCIENTB_REG, Embar23Size);
    IioWriteCpuCsr8 (IioIndex, NtbIndex, EMBAR45SZ_IIO_PCIENTB_REG, Embar45Size);
  } else {
    IioWriteCpuCsr8 (IioIndex, NtbIndex, EMBAR45SZ_IIO_PCIENTB_REG, Embar45Size);
  }
}

/**
    NTB MiscCtrlSts register Initailization

    @param IioIndex      - Socket number
    @param PortIndex     - Port number

    @retval None

**/
VOID
NtbMiscCtrlStsInit (
  IN  UINT8                                          IioIndex,
  IN  UINT8                                          PortIndex
  )
{
  return;
}


/**


    NTB Problematic Port configruation

    @param IioIndex      - Socket number
    @param PortIndex     - Port number

    @retval None

**/
VOID
NtbProblematicPortConfig (
  IN  UINT8                             IioIndex,
  IN  UINT8                             PortIndex
  )
{
  UINT8                                 StackIndex = 0;
  UINT8                                 ChannelIndex;
  UINT32                                MiscCtrlStsRegAddr;
  MISCCTRLSTS_CH0_IIO_VTD_STRUCT        MiscCtrlStsCh;
  EFI_STATUS                            Status;

  Status = GetPcieStackPerPort (IioIndex, PortIndex, &StackIndex);
  if (Status != EFI_SUCCESS) {
    return;
  }
  ChannelIndex = Ch7; // Always channel 7 for SPR NTB port.
  MiscCtrlStsRegAddr = MISCCTRLSTS_CH0_IIO_VTD_REG + ChannelIndex*8;
  MiscCtrlStsCh.Data = IioReadCpuCsr32 (IioIndex, StackIndex, MiscCtrlStsRegAddr);

  MiscCtrlStsCh.Bits.problematic_channel_for_lock_flows =1;
  IioWriteCpuCsr32 (IioIndex, StackIndex, MiscCtrlStsRegAddr, MiscCtrlStsCh.Data);
}

UINT64
NtbGetMbar01BaseAddress (
  UINT8         IioIndex,
  UINT8         NtbIndex
)
{
  UINT64 Addr;
  UINT64 AddrH;

    //if NTB is configured as NTB-RP...
  Addr = IioReadCpuCsr32 (IioIndex, NtbIndex, MBAR01_IP_N0_IIO_PCIENTB_REG);
  AddrH = IioReadCpuCsr32 (IioIndex, NtbIndex, MBAR01_IP_N1_IIO_PCIENTB_REG);
  Addr &= (UINT64) (~0x1fff);
  Addr = (AddrH << 32) | Addr;

  return Addr;
}

VOID
NtbSetPowerState (
  UINT8   IioIndex,
  UINT8   NtbIndex,
  UINT8   PowerState
)
{
  PMCSR_IIO_PCIENTB_STRUCT  PmcsrNtb;

  PmcsrNtb.Data = IioReadCpuCsr16 (IioIndex, NtbIndex, PMCSR_IIO_PCIENTB_REG);
  PmcsrNtb.Bits.ps = PowerState;
  IioWriteCpuCsr16 (IioIndex, NtbIndex, PMCSR_IIO_PCIENTB_REG, PmcsrNtb.Data);
}

/**
    NTB Late Workarounds called from DXE IioInit

    @param IioGlobalData IIO global data structure IIO_GLOBALS
**/

VOID
NtbLateWorkarounds (
  IN  IIO_GLOBALS                   *IioGlobalData
)
{
  UINT8                             IioIndex;
  UINT8                             PortIndex;
  UINT8                             MaxPortNumberPerSocket;

  MaxPortNumberPerSocket = GetMaxPortNumPerSocket();

  for (IioIndex = 0; IioIndex < MaxIIO; IioIndex++) {
    for (PortIndex = 1; PortIndex < MaxPortNumberPerSocket; PortIndex++) {
      if (!CheckNtbPortConfSetting (IioIndex, PortIndex)) {
        continue;
      }
      if (IsSiliconWorkaroundEnabled ("S1506869315")) {
        ApplyWAS1506869315 (IioGlobalData, IioIndex, PortIndex);
      }
    }
  }
}

#ifndef IA32
/**
    WAS1506869315 implementation

    @param IioGlobalData IIO global data structure IIO_GLOBALS
    @param IioIndex      - Socket number
    @param PortIndex     - Port number

**/

VOID
ApplyWAS1506869315 (
IN  IIO_GLOBALS                    *IioGlobalData,
IN  UINT8                          IioIndex,
IN  UINT8                          PortIndex
)
{
  SLOTCTL_IIO_PCIEGen4_STRUCT     SltCon;
  SLOTCAP_IIO_PCIEGen4_STRUCT     SltCap;
  LINKCAP_IIO_PCIEGen4_STRUCT     LnkCap;
  UINT8                           NtbPortIndex;
  UINT64                          Addr;
  UINT64                          AddrTemp;
  PCICMD_IIO_PCIENTB_STRUCT       PciCmd;
  PCICMD_IIO_PCIENTB_STRUCT       OrgPciCmd;
  EFI_STATUS                      Status;

  if (!IsSiliconWorkaroundEnabled ("S1506869315")) {
    return;
  }
  if (!CheckNtbPortConfSetting (IioIndex, PortIndex)) {
    return;
  }
  Status = GetNtbPortPerPortIndex (PortIndex, &NtbPortIndex);
  if (Status != EFI_SUCCESS) {
    return;
  }
  Addr = NtbGetMbar01BaseAddress(IioIndex, NtbPortIndex);
  if (Addr == 0) {
    IIO_D_PCIERR ("[%d p%d] %a No BAR assigned!\n", IioIndex, PortIndex, __FUNCTION__);
    return;
  }
  //
  //Enable the address access and transactions forwarding on the NTB port
  //
  PciCmd.Data = IioReadCpuCsr16 (IioIndex, NtbPortIndex, PCICMD_IIO_PCIENTB_REG);
  OrgPciCmd.Data = PciCmd.Data;
  PciCmd.Bits.mse = 1;  //Memory Space Enable
  IioWriteCpuCsr16 (IioIndex, NtbPortIndex, PCICMD_IIO_PCIENTB_REG, PciCmd.Data);


  AddrTemp = Addr + NTB_MBAR01_LINKCAP_OFFSET;
  LnkCap.Data = MmioRead32 (AddrTemp);
  LnkCap.Bits.dllerc = 1;
  MmioWrite32 (AddrTemp, LnkCap.Data);

  AddrTemp = Addr + NTB_MBAR01_SLOTCTL_OFFSET;
  SltCon.Data = MmioRead16 (AddrTemp);
  SltCon.Bits.hpie = 1;
  SltCon.Bits.dllsce = 1;
  MmioWrite16 (AddrTemp, SltCon.Data);

  AddrTemp = Addr + NTB_MBAR01_SLOTCAP_OFFSET;
  SltCap.Data = MmioRead32 (AddrTemp);
  SltCap.Bits.hpc = 1;
  MmioWrite32 (AddrTemp, SltCap.Data);

  //Restore NTB device state
  IioWriteCpuCsr16 (IioIndex, NtbPortIndex, PCICMD_IIO_PCIENTB_REG, OrgPciCmd.Data);
}
#else // IA32
VOID
ApplyWAS1506869315 (
IN  IIO_GLOBALS                    *IioGlobalData,
IN  UINT8                          IioIndex,
IN  UINT8                          PortIndex
)
{
}
#endif


/**
    NTB linkspeed setting

    @param[in]     IioGlobalData IIO global data structure IIO_GLOBALS
    @param[in]     IioIndex      - Socket number
    @param[in]     PortIndex     - Port number

**/

VOID
IioNtbSetLinkSpeed (
  IN  IIO_GLOBALS                    *IioGlobalData,
  IN  UINT8                          IioIndex,
  IN  UINT8                          PortIndex
)
{
  LINKCTL2_IIO_PCIEGen4_STRUCT    LnkCtl2;
  LINKCAP2_IIO_PCIEGen4_STRUCT    LnkCap2;
  UINT8                           NtbPortIndex;
  UINT32                          Mbar01;
  UINT32                          LnkCtl2Addr;
  UINT32                          LnkCap2Addr;
  PCICMD_IIO_PCIENTB_STRUCT       PciCmd;
  PCICMD_IIO_PCIENTB_STRUCT       OrgPciCmd;
  EFI_STATUS                      Status;

  if (!CheckNtbPortConfSetting (IioIndex, PortIndex)) {
    return;
  }
  Status = GetNtbPortPerPortIndex (PortIndex, &NtbPortIndex);
  if (Status != EFI_SUCCESS) {
    return;
  }
  Mbar01 = IioReadCpuCsr32 (IioIndex, NtbPortIndex, MBAR01_IP_N0_IIO_PCIENTB_REG);
  if (Mbar01 == 0) {
    IIO_D_PCIERR ("[%d p%d] %a No BAR assigned!\n", IioIndex, PortIndex, __FUNCTION__);
    return;
  }
  //
  //Enable the address access and transactions forwarding on the NTB port
  //
  PciCmd.Data = IioReadCpuCsr16 (IioIndex, NtbPortIndex, PCICMD_IIO_PCIENTB_REG);
  OrgPciCmd.Data = PciCmd.Data;
  PciCmd.Bits.mse = 1;  //Memory Space Enable
  IioWriteCpuCsr16 (IioIndex, NtbPortIndex, PCICMD_IIO_PCIENTB_REG, PciCmd.Data);

  LnkCtl2Addr = (Mbar01 & 0xFFFE0000) + NTB_MBAR01_LINKCTL2_OFFSET;
  LnkCap2Addr = (Mbar01 & 0xFFFE0000) + NTB_MBAR01_LINKCAP2_OFFSET;
  LnkCtl2.Data = IioMmioRead16 (LnkCtl2Addr);
  LnkCap2.Data = IioMmioRead32 (LnkCap2Addr);
  LnkCtl2.Bits.tls = GetPciePortLinkSpeed (IioGlobalData, IioIndex, PortIndex,
                                                       (UINT8)LnkCap2.Bits.slnkspv);
  IioMmioWrite16 (LnkCtl2Addr, LnkCtl2.Data);
}


/**

Routine Description:
 Performs any port configuration that is required before switching to NTB mode.

Arguments:
  IioGlobalData  - Pointer to IIO_GLOBALS
  IioIndex     - index to CPU/IIO
**/
VOID
PreNtbInitPortConfiguration(
  IIO_GLOBALS   *IioGlobalData,
  UINT8         IioIndex,
  UINT8         PortIndex
)
{
  //
  //5002495:Clone from BDX Sighting:[BDX-DE/EP] Max Payload Size = 256B does not set bit correctly
  //
  NtbSetMPL (IioGlobalData, IioIndex, PortIndex);
}

/**
  Used to set what lanes within the Ntb port should be masked during link training
  according to setup knob Override Max Link Width.

  @param[in] IioGlobalData            Pointer to IioGlobalData
  @param[in] IioIndex                 IIO index (socket number)
  @param[in] PortIndex                PCIe port index
  @param[in] DefaultMaxLinkWidth      Platform Default Max link width
  @param[in] OverrideMaxLinkWidth     Max link width set by BIOS knob to override default
  @param[in] LaneReversedLink         Port configured as lane reversed
**/
VOID
IioNtbMaxLinkWidth (
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
  UINT8                                   NtbPortIndex;
  UINT32                                  Mbar01;
  UINT32                                  LtssmTrnCtlAddr;
  PCICMD_IIO_PCIENTB_STRUCT               PciCmd;
  PCICMD_IIO_PCIENTB_STRUCT               OrgPciCmd;
  EFI_STATUS                              Status;

  if (!CheckNtbPortConfSetting (IioIndex, PortIndex)) {
    return;
  }
  Status = GetNtbPortPerPortIndex (PortIndex, &NtbPortIndex);
  if (Status != EFI_SUCCESS) {
    return;
  }
  Mbar01 = IioReadCpuCsr32 (IioIndex, NtbPortIndex, MBAR01_IP_N0_IIO_PCIENTB_REG);
  if (Mbar01 == 0) {
    IIO_D_PCIERR ("[%d p%d] %a No BAR assigned!\n", IioIndex, PortIndex, __FUNCTION__);
    return;
  }
  //
  //Enable the address access and transactions forwarding on the NTB port
  //
  PciCmd.Data = IioReadCpuCsr16 (IioIndex, NtbPortIndex, PCICMD_IIO_PCIENTB_REG);
  OrgPciCmd.Data = PciCmd.Data;
  PciCmd.Bits.mse = 1;  //Memory Space Enable
  IioWriteCpuCsr16 (IioIndex, NtbPortIndex, PCICMD_IIO_PCIENTB_REG, PciCmd.Data);

  LtssmTrnCtlAddr = (Mbar01 & 0xFFFE0000) + NTB_MBAR01_LTSSMTRNCTL_OFFSET;
  LtssmTrnCtl.Data = IioMmioRead32 (LtssmTrnCtlAddr);
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
  IioMmioWrite32 (LtssmTrnCtlAddr, LtssmTrnCtl.Data);

  LtssmTrnCtl.Data = IioMmioRead32 (LtssmTrnCtlAddr);
  IIO_D_PCIDBG ("Link Width (IIO%d, port%s(%d)) forced to x%d  widthmask 0x%x -> 0x%x -> 0x%x\n",
    IioIndex, PortIndex, OverrideMaxLinkWidth,
    LtssmTrnCtlBckup.Bits.widthmask, LtssmTrnCtlBckup2.Bits.widthmask, LtssmTrnCtl.Bits.widthmask);
}

/**

This function will program the earliest NTB settings that need to be done prior to PCIe link training initiation.

@param IioGlobalData	- IIO global data structure IIO_GLOBALS
@param IioIndex         - IIO socket index

@retval None

**/
VOID
NtbEarlyInit(
IN  IIO_GLOBALS  *IioGlobalData,
IN  UINT8         IioIndex
)
{
  UINT8                             NtbPortIndex;
  UINT8                             NtbPortPerPortIndex;
  UINT8                             MaxPortNumberPerSocket;
  UINT8                             PortIndex;
  UINT8                             MaxNtbPortPerSocket;
  UINT8                             PPD;
  UINT8                             CrossLinkOverride;
  IIO_CONFIG                        *SetupData;
  EFI_STATUS                        Status;

  IIO_D_PCIDBG ("%a: Start\n", __FUNCTION__);

  SetupData = GetIioConfig();

  //Enable NTB
  MaxPortNumberPerSocket = GetMaxPortNumPerSocket();
  MaxNtbPortPerSocket = GetMaxNtbNumPerSocket();

  for (PortIndex = 1; PortIndex < MaxPortNumberPerSocket; PortIndex++) {
    if (!IsNtbSbActive (IioIndex, PortIndex)) {
      continue;
    }

    if (GetPciePortOwnership (IioIndex, PortIndex) == NTB_PORT_DEF_NTB_NTB) {

      Status = GetNtbPortPerPortIndex (PortIndex, &NtbPortPerPortIndex);
      if (Status != EFI_SUCCESS) {
        continue;
      }
      NtbPortIndex = MaxNtbPortPerSocket * IioIndex + NtbPortPerPortIndex;

      //Retrieve NTB PPD setup question to determine if port should be configured as Transparent Bridge, NTB to NTB, or
      //NTB to Root Port
      PPD = SetupData->NtbPpd[NtbPortIndex];
      CrossLinkOverride = SetupData->NtbXlinkCtlOverride[NtbPortIndex];
      IIO_D_PCIDBG ("%a: PortIndex = 0x%x, PPD = 0x%x CrossLinkOverride = 0x%x\n",
        __FUNCTION__,
        PortIndex,
        PPD,
        CrossLinkOverride
        );
      NtbSetPpd (IioIndex, PortIndex, PPD, CrossLinkOverride);
    }
  }
}


/**
    Initializes the non-transparent bridge IMBAR/EMBAR for dev 1 / fun 0
    @param IioGlobalData     - Pointer to IIO_GLOBALS
    @param IioIndex          - Equals to Socket Index

    @retval None

**/
VOID
NtbInit (
  IN  IIO_GLOBALS                           *IioGlobalData,
  IN  UINT8                                 IioIndex
  )
{
  UINT8                                     NtbPortIndexGlobal;
  UINT8                                     NtbPortIndexLocal;
  UINT8                                     MaxPortNumberPerSocket;
  UINT8                                     MaxNtbPortPerSocket;
  UINT8                                     PortIndex;
  UINT8                                     StackIndex = 0;
  UINT8                                     PortOwnership;
  UINT8                                     Ppd;
  BOOLEAN                                   NtbSplitBarEnable;
  UINT8                                     NtbImbar1Size;
  UINT8                                     NtbImbar23Size;
  UINT8                                     NtbImbar45Size;
  UINT8                                     NtbEmbar1Size;
  UINT8                                     NtbEmbar23Size;
  UINT8                                     NtbEmbar45Size;
  IIO_CONFIG                                *SetupData;
  EFI_STATUS                                Status;

  IIO_D_PCILOG ("%a: IioIndex = 0x%x\n", __FUNCTION__, IioIndex);

  SetupData = GetIioConfig ();
  MaxPortNumberPerSocket = GetMaxPortNumPerSocket ();
  MaxNtbPortPerSocket = GetMaxNtbNumPerSocket ();

  for (PortIndex = 0; PortIndex < MaxPortNumberPerSocket; PortIndex++) {
    if (!IioVData_ConfigurePciePort(IioGlobalData, IioIndex, PortIndex)) {
      continue;
    }

    PortOwnership = GetPciePortOwnership (IioIndex, PortIndex);

    if ((PortOwnership != NTB_PORT_DEF_NTB_NTB) &&
        (!IsPCIeGen4 () && (PortOwnership != NTB_PORT_DEF_NTB_RP))) {
      continue;
    }

    Status = GetNtbPortPerPortIndex (PortIndex, &NtbPortIndexLocal);
    if (Status != EFI_SUCCESS) {
      continue;
    }

    IIO_D_PCILOG ("Initializing NTB for SKT%x Port%x\n", IioIndex, PortIndex);

    NtbMiscCtrlStsInit(IioIndex, PortIndex);
    IioNtbSetLinkSpeed (IioGlobalData,IioIndex, PortIndex);

    if ((PortOwnership != NTB_PORT_DEF_TRANSPARENT) && !IsPCIeGen4()) {
      //
      // 5370470: When NTB mode is enabled, max pay load size bit is locked
      //
      PreNtbInitPortConfiguration (IioGlobalData, IioIndex, PortIndex);
    }

    NtbPortIndexGlobal = MaxNtbPortPerSocket * IioIndex + NtbPortIndexLocal;

    Status = GetPcieStackPerPort (IioIndex, PortIndex, &StackIndex);
    if (Status != EFI_SUCCESS) {
      continue;
    }
    Ppd = SetupData->NtbPpd[NtbPortIndexGlobal];
    NtbSplitBarEnable = SetupData->NtbSplitBar[NtbPortIndexGlobal];

    IIO_D_PCILOG ("%a: StackIndex = 0x%x, NtbPortIndex = 0x%x, PPD = 0x%x, NtbSplitBarEnable = 0x%x.\n",
      __FUNCTION__,
      StackIndex,
      NtbPortIndexLocal,
      Ppd,
      NtbSplitBarEnable
      );

    if (!IsPCIeGen4 ()) {
      // For PCIE gen4 it's done in NtbEarlyInit
      NtbSetPpd (IioIndex, PortIndex, Ppd, NTB_CROSSLINK_NOT_SET);
      NtbSetSplitBar (IioIndex, NtbPortIndexLocal, NtbSplitBarEnable);
    }

    if (Ppd != NTB_PORT_DEF_TRANSPARENT) { //if NTB is not configured as "TB"
      //djm_xxx: we wish to only set the NTB BAR Size registers if the setup option for BAR register enable indicates so.  The reason
      //is that in some usage models, we leave it up to the drivers to write these registers.  If BIOS writes these registers, this may
      //cause driver implementations issues as these registers are write once.

      if (SetupData->NtbBarSizeOverride[NtbPortIndexGlobal] == 1) {
        NtbImbar1Size = SetupData->NtbBarSizeImBar1[NtbPortIndexGlobal];
        NtbEmbar1Size = SetupData->NtbBarSizeEmBarSZ1[NtbPortIndexGlobal];

        if (NtbSplitBarEnable) {
          NtbImbar23Size = SetupData->NtbBarSizeImBar2_0[NtbPortIndexGlobal];
          NtbImbar45Size = SetupData->NtbBarSizeImBar2_1[NtbPortIndexGlobal];

          NtbEmbar23Size = SetupData->NtbBarSizeEmBarSZ2_0[NtbPortIndexGlobal];
          NtbEmbar45Size = SetupData->NtbBarSizeEmBarSZ2_1[NtbPortIndexGlobal];
        } else {
          NtbImbar23Size = 0;
          NtbImbar45Size = SetupData->NtbBarSizeImBar2[NtbPortIndexGlobal];

          NtbEmbar23Size = 0;
          NtbEmbar45Size = SetupData->NtbBarSizeEmBarSZ2[NtbPortIndexGlobal];
        }

        NtbSetImBar (IioIndex, NtbPortIndexLocal, NtbImbar1Size);
        NtbSetSplitImBar (IioIndex, NtbPortIndexLocal, NtbSplitBarEnable, NtbImbar23Size, NtbImbar45Size);
        NtbSetEmBar (IioIndex, NtbPortIndexLocal, NtbEmbar1Size);
        NtbSetSplitEmBar (IioIndex, NtbPortIndexLocal, NtbSplitBarEnable, NtbEmbar23Size, NtbEmbar45Size);
      }
    }

    //if NTB is configured as NTB-NTB...
    if (!IsPCIeGen4 () && (Ppd == NTB_PORT_DEF_NTB_NTB)) {
      // done in NtbEarlyInit in PCIE gen4

      //only in the NTB-NTB configuration does BIOS need to configure the crosslink control override field of the
      //PCIE Port Definition register.  According to EAS, one platform BIOS should configure the register field as
      // "USD/DSP" while the other platform BIOS should configure the register field as "DSD/USP".

      //a setup option has been created for specifying this value (see IIO.sd).  we will read the value of that
      //setup option into XLINK below.  The value of the setup option matches the value that should be written
      //into bits 2:3 of the PPD register.

      //Port bifurcation is always "wait-on-BIOS".  However a potential issue is that the crosslink control
      //option is programmed after link training is initiated.  Also has no Crosslink-control HW strap like JSP does,
      //so this is always prorammed from setup.

      //now read in the PPD register so that we can set bits 2:3
      NtbSetPpd (IioIndex, PortIndex, NTB_PORT_DEF_NTB_NOT_SET, SetupData->NtbXlinkCtlOverride[NtbPortIndexGlobal]);
    }

    // SERVER_BIOS_FLAG note: skip power management programming, should be taken care of standard port config.
    // SERVER_BIOS_FLAG note: skip gen1/gen2/gen3 programming, should be taken care of in standard port config.
    // SERVER_BIOS_FLAG note: skip no_forw_prog programming, appears to not be available in this architcture.


    if (Ppd != NTB_PORT_DEF_TRANSPARENT) {
      //
      // To avoid potential deadlock issues arising from two Processors connected together which create certain
      // dependencies, disable remote p2p in the system, disable ACS on the NTB port, and mark the NTB port
      // as problematic.  Reference IIO EAS $6.4 "In/Out Dependency".
      //
      //

      NtbProblematicPortConfig (IioIndex, PortIndex);
    } // if Ppd != NTB_PORT_DEF_TRANSPARENT
  }
}

#ifndef IA32

/**

    This function will program the remain NTB settings required after PCI enumeration.

    @param IioGlobalData - Pointer to IIO_GLOBALS
    @param IioIndex      - Index to the current IIO (Socket)
    @param Phase         - Current phase for initialization

    @retval None

**/
VOID
NtbLateInit (
  IN  IIO_GLOBALS           *IioGlobalData,
  IN  UINT8                 IioIndex,
  IN  IIO_INIT_ENUMERATION  Phase
  )
{

  UINT64                      Addr;
  UINT64                      Addr1;
  UINT64                      AddrPcmd1;
  UINT64                      AddrPcmd2;
  PCICMD_IIO_PCIENTB_STRUCT   PciCmd;
  PCICMD_IIO_PCIENTB_STRUCT   OrgPciCmd;
  UINT8                       PortIndex;
  UINT8                       NtbIndex;
  UINT8                       MaxPortNumberPerSocket;
  UINT64                      Data64;
  EFI_STATUS                  Status;

  // NTB valid for P-STACK0 to P-STACK2
  MaxPortNumberPerSocket = GetMaxPciePortNumberPerSocket (IioIndex);
  for (PortIndex = 0; PortIndex < MaxPortNumberPerSocket; PortIndex++) {
    if (!IioVData_ConfigurePciePort(IioGlobalData, IioIndex, PortIndex)){
       continue;
    }

    if ((GetPciePortOwnership (IioIndex, PortIndex) != NTB_PORT_DEF_NTB_NTB) &&
        (GetPciePortOwnership (IioIndex, PortIndex) != NTB_PORT_DEF_NTB_RP)) {
      continue;
    }
    Status = GetNtbPortPerPortIndex (PortIndex, &NtbIndex);
    if (Status != EFI_SUCCESS) {
      continue;
    }

    Addr = NtbGetMbar01BaseAddress (IioIndex, NtbIndex);
    if (Addr == 0) {
      IIO_D_PCIERR ("[%d p%d] %a No BAR assigned!\n", IioIndex, PortIndex, __FUNCTION__);
      return;
    }

    Addr1 = Addr + R_EMBAR2XBASE;
    AddrPcmd1 = Addr + R_EMBAR1XBASE_IMBARPCIPCMD;
    AddrPcmd2 = Addr + R_EMBAR2XBASE_EMBARPCIPCMD;
    Addr += R_EMBAR1XBASE;

    switch (Phase) {
      case IioAfterResources:

        //Enable the address access and transactions forwarding on the NTB port
        PciCmd.Data = IioReadCpuCsr16 (IioIndex, NtbIndex, PCICMD_IIO_PCIENTB_REG);
        OrgPciCmd.Data = PciCmd.Data;
        PciCmd.Bits.mse = 1;  //Memory Space Enable
        PciCmd.Bits.bme = 1;  //Bus Master Enable
        IioWriteCpuCsr16 (IioIndex, NtbIndex, PCICMD_IIO_PCIENTB_REG, PciCmd.Data);

        //
        // 5372217: NTB pcicmd setup on secondary side
        //

        //Enable the address access and transactions forwarding primary side of the NTB port
        PciCmd.Data = *(volatile UINT16 *) AddrPcmd1;
        PciCmd.Bits.mse = 1;  //Memory Space Enable
        PciCmd.Bits.bme = 1;  //Bus Master Enable
        *(volatile UINT16 *) AddrPcmd1 = PciCmd.Data;
        S3BootScriptSaveMemWrite (S3BootScriptWidthUint16, AddrPcmd1, 1, &PciCmd.Data);

        //Enable the address access and transactions forwarding secondary side of the NTB port
        PciCmd.Data = *(volatile UINT16 *) AddrPcmd2;
        PciCmd.Bits.mse = 1;  //Memory Space Enable
        PciCmd.Bits.bme = 1;  //Bus Master Enable
        *(volatile UINT16 *) AddrPcmd2 = PciCmd.Data;
        S3BootScriptSaveMemWrite (S3BootScriptWidthUint16, AddrPcmd2, 1, &PciCmd.Data);
        // end 5372217

        //Bit 3 is the prefetchable bit, with 1 = enabled
        //Memory Access Type must be 64-bit addressable
        //Programming PBAR2XLAT
        Data64 = *(volatile UINT64 *) Addr;
        Data64 |= B_EMBAR0_PREFETCHABLE;
        Data64 |= (UINT64) B_IMBAR1BASET_Type_Mem64;

        *(volatile UINT64 *) Addr =  Data64;
        S3BootScriptSaveMemWrite (S3BootScriptWidthUint64, Addr, 1, &Data64);
        //Programming IMBAR2_0XLAT
        Data64 = *(volatile UINT64 *) Addr1;
        Data64 |= B_EMBAR0_PREFETCHABLE;
        Data64 |= (UINT64) B_IMBAR1BASET_Type_Mem64;

        *(volatile UINT64 *) Addr1 = Data64;
        S3BootScriptSaveMemWrite (S3BootScriptWidthUint64, Addr1, 1, &Data64);

        //Restore NTB device state
        *(volatile UINT16 *) AddrPcmd2 = OrgPciCmd.Data;
        *(volatile UINT16 *) AddrPcmd1 = OrgPciCmd.Data;
        IioWriteCpuCsr16 (IioIndex, NtbIndex, PCICMD_IIO_PCIENTB_REG, OrgPciCmd.Data);

        break;
      case IioReadyToBoot:

        //Enable the address access and transactions forwarding on the NTB port
        PciCmd.Data = IioReadCpuCsr16 (IioIndex, NtbIndex, PCICMD_IIO_PCIENTB_REG);
        PciCmd.Bits.mse = 1;  //Memory Space Enable
        PciCmd.Bits.bme = 1;  //Bus Master Enable
        IioNonS3WriteCpuCsr16 (IioIndex, NtbIndex, PCICMD_IIO_PCIENTB_REG, PciCmd.Data);

        //
        // 5372217: NTB pcicmd setup on secondary side
        //

        //Enable the address access and transactions forwarding primary side of the NTB port
        PciCmd.Data = *(volatile UINT16 *) AddrPcmd1;
        PciCmd.Bits.mse = 1;  //Memory Space Enable
        PciCmd.Bits.bme = 1;  //Bus Master Enable
        *(volatile UINT16 *) AddrPcmd1 = PciCmd.Data;

        //Enable the address access and transactions forwarding secondary side of the NTB port
        PciCmd.Data = *(volatile UINT16 *) AddrPcmd2;
        PciCmd.Bits.mse = 1;  //Memory Space Enable
        PciCmd.Bits.bme = 1;  //Bus Master Enable
        *(volatile UINT16 *) AddrPcmd2 = PciCmd.Data;
        // end 5372217

        break;
      default:
        break;

    } // Switch for IIO boot phase

  } // per-NtbPortIndex loop
} //end NtbInitAfterResources

#else // IA32

/**

    This function will program the remain NTB settings required after PCI enumeration.

    @param RootBridgeHandle - Handle to RootBridge
    @param Address          - Pcie Address of the RootBridge
    @param PciPhase         - Pci Phase
    @param Context          - Contex of the function

    @retval None

**/
VOID
NtbLateInit (
  IN  IIO_GLOBALS           *IioGlobalData,
  IN  UINT8                 IioIndex,
  IN  IIO_INIT_ENUMERATION  Phase
  )
{
}

#endif //IA32
