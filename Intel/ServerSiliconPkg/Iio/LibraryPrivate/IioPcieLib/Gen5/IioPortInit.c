/** @file
  Implement north bridge PCI express port initialization

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


#include <RcRegs.h>
#include <PcieRegs.h>

#include <IioPlatformData.h>
#include <Library/IioAccessLib.h>
#include <Library/IioDataHubLib.h>

#include <IioSetupDefinitions.h>
#include <Library/SiliconWorkaroundLib.h>
#include <LibraryPrivate/IioPcieLib.h>
#include <Chip/Include/CpuPciAccess.h>

#define IIO_PCIE_GEN4_SOSCTL_OFFSET_IN_EXPPTMBAR       0x2C38

/**
  Lock MSINXTPTR reg

  @param[in] IioIndex      - Socket number
  @param[in] PortIndex     - Port number
**/
VOID
LockMsiCapPtr (
  IN  UINT8                     IioIndex,
  IN  UINT8                     PortIndex
  )
{
  MSICAPLST_IIO_PCIE_G5_STRUCT    Msinxtptr;

  Msinxtptr.Data = IioReadCpuCsr16 (IioIndex, PortIndex, MSICAPLST_IIO_PCIE_G5_REG);
  IioWriteCpuCsr16 (IioIndex, PortIndex, MSICAPLST_IIO_PCIE_G5_REG, Msinxtptr.Data);
}

/**
  Program Root port escalation status

  @param[in] IioIndex             - Socket number
  @param[in] PortIndex            - Port number
  @param[in] EnableCorrectableErr - Enable correctable errors
  @param[in] EnableNonFatalErr    - Enable non-fatal errors
  @param[in] EnableFatalErr       - Enable fatal errors
**/
VOID
RpErrEscalationProg (
  IN  UINT8                     IioIndex,
  IN  UINT8                     PortIndex,
  IN  BOOLEAN                   EnableCorrectableErr,
  IN  BOOLEAN                   EnableNonFatalErr,
  IN  BOOLEAN                   EnableFatalErr
  )
{
  ROOTERRCMD_IIO_PCIE_G5_STRUCT            RpErrCmd;

  //
  // Program Root Port Error Escalation enable/disable
  //
  RpErrCmd.Data = IioReadCpuCsr32 (IioIndex, PortIndex, ROOTERRCMD_IIO_PCIE_G5_REG);
  if (EnableCorrectableErr) {
    RpErrCmd.Bits.cere = 1;
  }
  if (EnableNonFatalErr) {
    RpErrCmd.Bits.nfere = 1;
  }
  if (EnableFatalErr) {
    RpErrCmd.Bits.fere = 1;
  }
  IioWriteCpuCsr32 (IioIndex, PortIndex, ROOTERRCMD_IIO_PCIE_G5_REG, RpErrCmd.Data);
}

/**
  Lock PXPNXTPTR reg

  @param[in] IioIndex      - Socket number
  @param[in] PortIndex     - Port number
**/
VOID
LockExpCapPtr (
  IN  UINT8                     IioIndex,
  IN  UINT8                     PortIndex
  )
{
  EXPCAPLST_IIO_PCIE_G5_STRUCT  Pxpnxtptr;

  Pxpnxtptr.Data = IioReadCpuCsr16 (IioIndex, PortIndex, EXPCAPLST_IIO_PCIE_G5_REG);
  IioWriteCpuCsr16 (IioIndex, PortIndex, EXPCAPLST_IIO_PCIE_G5_REG, Pxpnxtptr.Data);
}

/**
  Program necessary errors settings

  @param[in] IioIndex      - Socket number
  @param[in] PortIndex     - Port number
**/
VOID
PcieErrProg (
  IN  UINT8                     IioIndex,
  IN  UINT8                     PortIndex
  )
{
  ERRUNCSTS_IIO_PCIE_G5_STRUCT    UncErrSts;

  //
  // Clear Expected PCIe Errors (3612591, 3875328)
  // [permanent programming consideration for all steppings]
  UncErrSts.Data = IioReadCpuCsr32 (IioIndex, PortIndex, ERRUNCSTS_IIO_PCIE_G5_REG);
  UncErrSts.Bits.ure = 1;
  IioWriteCpuCsr32 (IioIndex, PortIndex, ERRUNCSTS_IIO_PCIE_G5_REG, UncErrSts.Data);
}

/**
  Program MSIMsgCtrl register

  @param[in] IioIndex      - Index to CPU/IIO
  @param[in] PortIndex     - Index to PCIe port
  @param[in] MsiEnabled    - MSI interrupts should be enabled
 */
VOID
ProgramMsiMsgCtrl (
  IN  UINT8    IioIndex,
  IN  UINT8    PortIndex,
  IN  BOOLEAN  MsiEnabled
  )
{
  MSICTL_IIO_PCIE_G5_STRUCT  MsiMsgCtl;

  MsiMsgCtl.Data = IioReadCpuCsr16 (IioIndex, PortIndex, MSICTL_IIO_PCIE_G5_REG);
  if (MsiEnabled) {
    MsiMsgCtl.Bits.msie = 1;
  } else {
    MsiMsgCtl.Bits.msie = 0;
  }
  IioWriteCpuCsr16 (IioIndex, PortIndex, MSICTL_IIO_PCIE_G5_REG, MsiMsgCtl.Data);
}

/**
    Program power state in PMCSR register

    @param[in] IioIndex      - Index to CPU/IIO
    @param[in] PortIndex     - Index to PCIe port
    @param[in] PowerState    - Register value to set
 */
VOID
SetPowerState (
  IN  UINT8  IioIndex,
  IN  UINT8  PortIndex,
  IN  UINT8  PowerState
  )
{
  PMCSR_IIO_PCIE_G5_STRUCT        Pmcsr;

  Pmcsr.Data = IioReadCpuCsr16 (IioIndex, PortIndex, PMCSR_IIO_PCIE_G5_REG);
  Pmcsr.Bits.ps = PowerState;
  IioWriteCpuCsr16 (IioIndex, PortIndex, PMCSR_IIO_PCIE_G5_REG, Pmcsr.Data);
}

/**
  This function locks Subsystem Vendor ID

  @param[in] IioIndex      - Socket number of the port being configured.
  @param[in] PortIndex     - Index to Iio Root Port.
**/
VOID
IioLockSvid (
  IN  UINT8                         IioIndex,
  IN  UINT8                         PortIndex
  )
{
  SSVID_IIO_PCIE_G5_STRUCT          Svid;

  Svid.Data = IioReadCpuCsr16 (IioIndex, PortIndex, SSVID_IIO_PCIE_G5_REG);
  IioWriteCpuCsr16 (IioIndex, PortIndex, SSVID_IIO_PCIE_G5_REG, Svid.Data);
}

/**
  This function locks Subsystem Device ID

  @param[in] IioIndex      - Socket number of the port being configured.
  @param[in] PortIndex     - Index to Iio Root Port.
**/
VOID
IioLockSdid (
  IN  UINT8                         IioIndex,
  IN  UINT8                         PortIndex
  )
{
  SSID_IIO_PCIE_G5_STRUCT           Sdid;

  Sdid.Data = IioReadCpuCsr16 (IioIndex, PortIndex, SSID_IIO_PCIE_G5_REG);
  IioWriteCpuCsr16 (IioIndex, PortIndex, SSID_IIO_PCIE_G5_REG, Sdid.Data);
}

/**
  Program SOSCTL.SRISEN to enable SRIS support.

  @param[in] IioGlobalData - Pointer to IioGlobalData
  @param[in] IioIndex      - Socket number
  @param[in] PortIndex     - Port number
**/
VOID
IioSrisEnable (
  IN  IIO_GLOBALS               *IioGlobalData,
  IN  UINT8                     IioIndex,
  IN  UINT8                     PortIndex
  )
{
  UINT8                         MaxPortNumberPerSocket;
  UINT32                        Data32;
  CSR_OFFSET                    CsrOffset;
  SOSCTL_SB_FBLP_STRUCT         SosCtl;

  MaxPortNumberPerSocket = GetMaxPciePortNumberPerSocket (IioIndex);

  //
  // Override SRIS - Bit 22 of SOSCTL
  //
  if (IioGlobalData->SetupData.SRIS[IioIndex * MaxPortNumberPerSocket + PortIndex]) {
    if (PortIndex & BIT0) {
      SosCtl.Data = IioReadCpuCsr32 (IioIndex, PortIndex, SOSCTL_SB_FBLP_REG);
      SosCtl.Bits.srisen = 1;
      IioWriteCpuCsr32 (IioIndex, PortIndex, SOSCTL_SB_FBLP_REG, SosCtl.Data);
    } else {
      CsrOffset.Bits.boxtype = 0x0E;
      CsrOffset.Bits.funcblk = 0x02;
      CsrOffset.Bits.size = 2;
      CsrOffset.Bits.offset = (IIO_PCIE_GEN4_SOSCTL_OFFSET_IN_EXPPTMBAR & 0xFFFF);
      Data32 = IioReadCpuCsr32 (IioIndex, PortIndex, CsrOffset.Data);
      Data32 |= BIT22;
      IioWriteCpuCsr32 (IioIndex, PortIndex, CsrOffset.Data, Data32);
    }
  }
}

/**
  Set no compliance bit for the given port.

  @param[in] IioIndex              - Socket number
  @param[in] PortIndex             - Port number
  @param[in] EnterNoCompliance     - TRUE or FALSE if given port should block compliance entry

**/
VOID
IioSetNoCompliance (
  IN  UINT8                         IioIndex,
  IN  UINT8                         PortIndex,
  IN  BOOLEAN                       EnterNoCompliance
  )
{
  return;
}

/**
  Program UPCFGCSR to enable support for PCIE DLW (Dynamic Link Width)

  @param[in] IioIndex      - Socket number of the port being configured.
  @param[in] PortIndex     - Index to Iio Root Port.
**/
VOID
IioPcieEnableDlw (
  IN  UINT8                         IioIndex,
  IN  UINT8                         PortIndex
  )
{
  UPCFGCSR_IIO_PCIEGen4_STRUCT      PcieGen4UpCfgCsr;
  UPCFGCSR_SB_FBLP_STRUCT           PcieGen5SbUpCfgCsr;

  if (PortIndex % 2 == 1) {
    PcieGen5SbUpCfgCsr.Data = IioReadCpuCsr32 (IioIndex, PortIndex, UPCFGCSR_SB_FBLP_REG);
    PcieGen5SbUpCfgCsr.Bits.upcfgcapen = 1;
    IioWriteCpuCsr32 (IioIndex, PortIndex, UPCFGCSR_SB_FBLP_REG, PcieGen5SbUpCfgCsr.Data);
  } else {
    PcieGen4UpCfgCsr.Data = IioReadCpuCsr32 (IioIndex, PortIndex, UPCFGCSR_IIO_PCIEGen4_REG);
    PcieGen4UpCfgCsr.Bits.upcfgcapen = 1;
    IioWriteCpuCsr32 (IioIndex, PortIndex, UPCFGCSR_IIO_PCIEGen4_REG, PcieGen4UpCfgCsr.Data);
  }
}


/**
 If HotPlug is enabled by policy, HotPlug should be enabled by setting enable bit
 in VPPCSR register and defining number of PCA9555 device and slot number of PCA9555

  @param[in] IioIndex[in]      - Socket number
  @param[in] PortIndex[in]     - Port number
**/
VOID
IioVppHotPlugInit (
  IN  UINT8                     IioIndex,
  IN  UINT8                     PortIndex
  )
{
  VPPCSR_IIO_PCIE_G5_STRUCT     VppCsr;
  IIO_CONFIG                    *SetupData;
  UINT16                        TotalPortsIndex;
  UINT8                         RegOffset;
  SEGMAP0_SB_CXPSMB_STRUCT      SegMapN;
  SEGROW0_SB_CXPSMB_STRUCT      SegRowN;
  SMTCTL_SB_CXPSMB_STRUCT       Smtctl;

  SetupData = GetIioConfig ();
  TotalPortsIndex = (IioIndex * NUMBER_PORTS_PER_SOCKET) + PortIndex;

  //
  // HSD 4166776: PCIE slot power loss on warm reset
  //              BIOS needs to ensure that SLOTCAP is zero if HP disabled
  //              Also MRLSP, the code below uses these two to decide which
  //              .. slot power control function to use
  //
  if (!IioIsHpEnabledOnPort (IioIndex, PortIndex)) {
    SetupData->SLOTHPCAP[TotalPortsIndex] = 0;
    SetupData->SLOTMRLSP[TotalPortsIndex] = 0;
    SetupData->SLOTPIP[TotalPortsIndex] = 0;
    SetupData->SLOTPCP[TotalPortsIndex] = 0;
    SetupData->SLOTABP[TotalPortsIndex] = 0;
    SetupData->SLOTAIP[TotalPortsIndex] = 0;
  }

  //
  // CXPSMB must be programmed before programming any PCIe RP's Hot-Plug Registers (VPPCSR in PCIe RP).
  //
    if (SetupData->VppEnabled[TotalPortsIndex] && (SetupData->VppAddress[TotalPortsIndex] != 0)) {
    //
    // According to CXPSMB_Strapping_XCC_1p0_0p3.xlsx,
    // Souce ID entry 0-7 is for DMI stack.
    // Souce ID entry 8-47 is for port 1 - 40
    //
    if (PortIndex == 0) {
      RegOffset = 0;
    } else {
      RegOffset = 7 + PortIndex;
    }
    RegOffset *= sizeof (SEGMAP0_SB_CXPSMB_STRUCT);
    SegMapN.Data = IioReadCpuCsr8 (IioIndex, 0, SEGMAP0_SB_CXPSMB_REG + RegOffset);
    SegMapN.Bits.smap = PortIndex;
    IioWriteCpuCsr8 (IioIndex, 0,  SEGMAP0_SB_CXPSMB_REG + RegOffset, SegMapN.Data);
    //
    // Take SegRow 0-40 for Port 0-40.
    //
    RegOffset = PortIndex * (sizeof (SEGROW0_SB_CXPSMB_STRUCT));
    SegRowN.Data = IioReadCpuCsr16 (IioIndex, 0, SEGROW0_SB_CXPSMB_REG + RegOffset);
    SegRowN.Bits.valid = 1;
    if (SetupData->MuxAddress[TotalPortsIndex] != 0) {
      SegRowN.Bits.mp = 1; // mux present
      SegRowN.Bits.maddr = SetupData->MuxAddress[TotalPortsIndex] >> 1;
      SegRowN.Bits.mchansel = SetupData->ChannelID[TotalPortsIndex];
    }
    IioWriteCpuCsr16 (IioIndex, 0, SEGROW0_SB_CXPSMB_REG + RegOffset, SegRowN.Data);

    Smtctl.Data = IioReadCpuCsr32 (IioIndex, 0, SMTCTL_SB_CXPSMB_REG);
    if (Smtctl.Bits.smben == 0) {
      Smtctl.Bits.smben = 1;
      IioWriteCpuCsr32 (IioIndex, 0, SMTCTL_SB_CXPSMB_REG, Smtctl.Data);
    }
  }
  //
  // Program VPP, enable VPP and misc
  //
  VppCsr.Data = IioReadCpuCsr32 (IioIndex, PortIndex, VPPCSR_IIO_PCIE_G5_REG);
  VppCsr.Bits.vppe = 0;

  //
  // HSD 4166776: PCIE slot power loss on warm reset
  //              BIOS needs to ensure that VPP address and enable are zero if HP disabled
  //
  if (IioIsHpEnabledOnPort (IioIndex, PortIndex)) {
    if (SetupData->VppEnabled[TotalPortsIndex] && (SetupData->VppAddress[TotalPortsIndex] != 0)) {
      //
      // VPP is enabled for this port/slot in UBA
      // VPP Address = Vpp Port (one bit only) | 3 bits [4-1] from VppAddress
      //
      VppCsr.Bits.vppe = BIT0;
      VppCsr.Bits.vppn  = SetupData->VppPort[TotalPortsIndex] & BIT0;
      VppCsr.Bits.vppn |= SetupData->VppAddress[TotalPortsIndex] & 0xE;
      VppCsr.Bits.vppmo = BIT1;
    }
  }
  IioWriteCpuCsr32 (IioIndex, PortIndex, VPPCSR_IIO_PCIE_G5_REG, VppCsr.Data);
}

/**
  Read FormFactor value from MiscCtrlStat1 register - this is supported only by Gen3 controllers
  @param[in]   IioIndex            Socket number
  @param[in]   PortIndex           Port number
  @param[out]  FormFactorPresent   Buffer will contains TRUE if FF value was read
  @param[out]  FormFactorValue     Buffer for Ff value (0xFF is value not read)
 */
VOID
GetMiscCtlStat1FormFactor (
  IN  UINT8    IioIndex,
  IN  UINT8    PortIndex,
  OUT BOOLEAN  *FormFactorPresent,
  OUT UINT8    *FormFactorValue
  )
{
  *FormFactorPresent = FALSE;
  *FormFactorValue = 0xFF;
}
