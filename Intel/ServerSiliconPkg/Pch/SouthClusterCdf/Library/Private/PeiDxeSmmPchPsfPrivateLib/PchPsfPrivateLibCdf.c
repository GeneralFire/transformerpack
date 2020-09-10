/** @file
  This file contains internal PSF routines for PCH PSF CNL lib usage

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2020 Intel Corporation.

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

  This file contains an 'Intel Peripheral Driver' and is uniquely identified as
  "Intel Reference Module" and is licensed for Intel CPUs and chipsets under
  the terms of your license agreement with Intel or your vendor. This file may
  be modified by the user, subject to additional terms of the license agreement.

@par Specification Reference:
**/
#include <Uefi/UefiBaseType.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/PchPcrLib.h>
#include <Library/PchInfoLib.h>
#include <Library/PchPcieRpInfo.h>
#include <Library/SataLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/SataLib.h>
#include <Library/SataSocLib.h>
#include <Register/PchRegsPsf.h>
#include <Register/PchRegsPsfCdf.h>
#include "PchPsfPrivateLibInternal.h"
#include <Private/Library/PchPsfPrivateLib.h>
#include <Private/Library/P2sbPrivateLibCdf.h>

/**
  Return PSF_PORT for SerialIO I2C device

  @param[in] I2cNum  Serial IO I2C device (I2C0, I2C1, ....)

  @retval  PsfPort   PSF PORT structure for SerialIO I2C device
**/
PSF_PORT
PsfSerialIoI2cPort (
  IN UINT32  I2cNum
  )
{
  PSF_PORT PsfPort = {0,0};
  return PsfPort;
}

/**
  Return PSF_PORT for SerialIO SPI device

  @param[in] SpiNum  Serial IO SPI device (SPI0, SPI1, ....)

  @retval  PsfPort   PSF PORT structure for SerialIO SPI device
**/
PSF_PORT
PsfSerialIoSpiPort (
  IN UINT32  SpiNum
  )
{
  PSF_PORT PsfPort = {0,0};
  return PsfPort;
}

/**
  Return PSF_PORT for SerialIO UART device

  @param[in] UartNum  Serial IO UART device (UART0, UART1, ....)

  @retval  PsfPort    PSF PORT structure for SerialIO UART device
**/
PSF_PORT
PsfSerialIoUartPort (
  IN UINT32  UartNum
  )
{
  PSF_PORT PsfPort = {0,0};
  return PsfPort;
}

/**
  Return PSF_PORT for TraceHub device

  @retval    PsfPort         PSF PORT structure for TraceHub device
**/
PSF_PORT
PsfTraceHubPort (
  VOID
  )
{
  PSF_PORT PsfPort;

  PsfPort.PsfPid = PID_PSF2;

  PsfPort.RegBase = R_CDF_PCH_PSF2_PCR_T0_SHDW_NPK_REG_BASE;

  return PsfPort;
}

/**
  Return PSF_PORT for TraceHub ACPI device

  @retval    PsfPort         PSF PORT structure for TraceHub ACPI device
**/
PSF_PORT
PsfTraceHubAcpiDevPort (
  VOID
  )
{
  PSF_PORT PsfPort;

  PsfPort.PsfPid = PID_PSF2;

  PsfPort.RegBase = R_CDF_PCH_PSF2_PCR_T0_SHDW_NPK_ACPI_REG_BASE;

  return PsfPort;
}

GLOBAL_REMOVE_IF_UNREFERENCED UINT16 mPchHHeciPsfRegBase[] =
{
  R_CDF_PCH_PSF2_PCR_T0_SHDW_ME_HECI1_REG_BASE,
  R_CDF_PCH_PSF2_PCR_T0_SHDW_ME_HECI2_REG_BASE,
  R_CDF_PCH_PSF2_PCR_T0_SHDW_ME_HECI3_REG_BASE,
  R_CDF_PCH_PSF2_PCR_T0_SHDW_ME_HECI4_REG_BASE
};

/**
  Return PSF_PORT for HECI device

  @param[in] HeciDevice      HECIx Device (HECI1-4)

  @retval    PsfPort         PSF PORT structure for HECI device
**/
PSF_PORT
PsfHeciPort (
  IN UINT8      HeciDevice
  )
{
  PSF_PORT PsfPort = {0,0};

  if ((HeciDevice < 1) || (HeciDevice > 4)) {
    ASSERT (FALSE);
  return PsfPort;
  }

  PsfPort.PsfPid = PID_PSF2;

  PsfPort.RegBase = mPchHHeciPsfRegBase[HeciDevice - 1];

  return PsfPort;
}

/**
  Return PSF_PORT for SOL device

  @retval    PsfPort         PSF PORT structure for SOL device
**/
PSF_PORT
PsfSolPort (
  VOID
  )
{
  PSF_PORT PsfPort;

  PsfPort.PsfPid = PID_PSF2;

  PsfPort.RegBase = R_CDF_PCH_PSF2_PCR_T0_SHDW_ME_KT_REG_BASE;

  return PsfPort;
}

/**
  Disable IDER device at PSF level
**/
VOID
PsfDisableIderDevice (
  VOID
  )
{
  PSF_PORT PsfPort;

  PsfPort.PsfPid = PID_PSF2;

  PsfPort.RegBase = R_CDF_PCH_PSF2_PCR_T0_SHDW_ME_IDER_REG_BASE;

  PsfDisableDevice (PsfPort);
}

/**
  Disable HDAudio device at PSF level
**/
VOID
PsfDisableHdaDevice (
  VOID
  )
{
  return;
}

/**
  Disable xDCI device at PSF level
**/
VOID
PsfDisableXdciDevice (
  VOID
  )
{
  return;
}

/**
  Disable xHCI device at PSF level
**/
VOID
PsfDisableXhciDevice (
  VOID
  )
{
  PSF_PORT PsfPort;

  PsfPort.PsfPid = PID_PSF2;

  PsfPort.RegBase = R_CDF_PCH_PSF2_PCR_T0_SHDW_XHCI_REG_BASE;

  PsfDisableDevice (PsfPort);
}

/**
  Disable xHCI VTIO Phantom device at PSF level
**/
VOID
PsfDisableXhciVtioDevice (
  VOID
  )
{
  return;
}

/**
  Disable SATA device at PSF level

  @param[in]  SataCtrlIndex     SATA controller index
**/
VOID
PsfDisableSataDevice (
  IN UINT32     SataCtrlIndex
  )
{
  PSF_PORT PsfPort;

  ASSERT (SataCtrlIndex < MaxSataControllerNum());

  PsfPort.PsfPid = PID_PSF1;

  if (SataCtrlIndex == SATA_1_CONTROLLER_INDEX) {
    PsfPort.RegBase = R_CDF_PCH_PSF1_PCR_T0_SHDW_SATA_1_REG_BASE;
  } else if (SataCtrlIndex == SATA_2_CONTROLLER_INDEX) {
    PsfPort.RegBase = R_CDF_PCH_PSF1_PCR_T0_SHDW_SATA_2_REG_BASE;
  } else if (SataCtrlIndex == SATA_3_CONTROLLER_INDEX) {
    PsfPort.RegBase = R_CDF_PCH_PSF1_PCR_T0_SHDW_SATA_3_REG_BASE;
  } else {
    ASSERT (FALSE);
    PsfPort.RegBase = 0;
  }

  PsfDisableDevice (PsfPort);
}

/**
  Program PSF grant counts for SATA
  Call this before SATA ports are accessed for enumeration
**/
VOID
PsfConfigureSataGrantCounts (
  VOID
  )
{
  //
  // PCH-LP:
  //   Set PSF_4_DEV_GNTCNT_RELOAD_DGCR2-5.GNTCNTRELOAD to 0x1
  //     VR    -> PSF_4_DEV_GNTCNT_RELOAD_DGCR2
  //     VS0-2 -> PSF_4_DEV_GNTCNT_RELOAD_DGCR3-5
  // PCH-H:
  //   Set PSF_4_DEV_GNTCNT_RELOAD_DGCR2-10.GNTCNTRELOAD to 0x1
  //     VR    -> PSF_4_DEV_GNTCNT_RELOAD_DGCR2
  //     VS0-7 -> PSF_4_DEV_GNTCNT_RELOAD_DGCR3-10
  //
  UINT16  Dgcr0Addr;
  UINT8   DgcrMinIndex;
  UINT8   DgcrMaxIndex;
  UINT8   DgcrIndex;

  Dgcr0Addr = R_CDF_PCH_PSF9_PCR_DEV_GNTCNT_RELOAD_DGCR0;
  DgcrMinIndex = 2;
  DgcrMaxIndex = 10;

  for (DgcrIndex = DgcrMinIndex; DgcrIndex <= DgcrMaxIndex; DgcrIndex++) {
    PchPcrAndThenOr32 (
      PID_PSFS,
      Dgcr0Addr  + (DgcrIndex * S_PCH_PSFX_PCR_DEV_GNTCNT_RELOAD_DGCR),
      (UINT32) ~B_PCH_PSFX_PCR_DEV_GNTCNT_RELOAD_DGCR_GNT_CNT_RELOAD ,
      0x1
      );
  }
}

/**
  Return PSF_PORT for ISH device

  @retval    PsfPort         PSF PORT structure for ISH device
**/
PSF_PORT
PsfIshPort (
  VOID
  )
{
  PSF_PORT PsfPort = {0,0};
  return PsfPort;
}

/**
  Disable GbE device at PSF level
**/
VOID
PsfDisableGbeDevice (
  VOID
  )
{
  return;
}

/**
  Disable SMBUS device at PSF level
**/
VOID
PsfDisableSmbusDevice (
  VOID
  )
{
  PSF_PORT PsfPort;

  PsfPort.PsfPid = PID_PSF3;

  PsfPort.RegBase = R_CDF_PCH_PSF3_PCR_T0_SHDW_SMBUS_REG_BASE;

  PsfDisableDevice (PsfPort);
}

/**
  Disable DMA SMBUS device at PSF level
**/
VOID
PsfDisableSmbusDmaDevice (
  VOID
  )
{
  PSF_PORT PsfPort;

  PsfPort.PsfPid = PID_PSF3;

  PsfPort.RegBase = R_CDF_PCH_PSF3_PCR_T0_SHDW_DMA_SMBUS_REG_BASE;

  PsfDisableDevice (PsfPort);
}


GLOBAL_REMOVE_IF_UNREFERENCED PSF_PORT mPchEmmcDevicePsfPort = {PID_PSF3, R_CDF_PCH_PSF3_PCR_T0_SHDW_EMMC_REG_BASE};

/**
  Return PSF_PORT for SCS eMMC device

  @retval    PsfPort      PSF PORT structure for SCS eMMC device
**/
PSF_PORT
PsfScsEmmcPort (
  VOID
  )
{
  return mPchEmmcDevicePsfPort;
}

/**
  Return PSF_PORT for SCS SD Card device

  @retval    PsfPort      PSF PORT structure for SCS SD Card device
**/
PSF_PORT
PsfScsSdCardPort (
  VOID
  )
{
  PSF_PORT PsfPort = {0,0};
  return PsfPort;
}

/**
  Return PSF_PORT for SCS UFS device

  @param[in] UfsNum       UFS Device

  @retval    PsfPort      PSF PORT structure for SCS UFS device
**/
PSF_PORT
PsfScsUfsPort (
  IN UINT32  UfsNum
  )
{
  PSF_PORT PsfPort = {0,0};
  return PsfPort;
}

//
// PSF1_T1_SHDW_PCIExx_REG_BASE registers for CDF PCH
//
GLOBAL_REMOVE_IF_UNREFERENCED PSF_PORT mPchPsfRs0PcieRegBase[] =
{
  {PID_PSF1, R_CDF_PCH_PSF1_PCR_T1_SHDW_PCIE01_RS0_REG_BASE},
  {PID_PSF1, R_CDF_PCH_PSF1_PCR_T1_SHDW_PCIE02_RS0_REG_BASE},
  {PID_PSF1, R_CDF_PCH_PSF1_PCR_T1_SHDW_PCIE03_RS0_REG_BASE},
  {PID_PSF1, R_CDF_PCH_PSF1_PCR_T1_SHDW_PCIE04_RS0_REG_BASE},
  {PID_PSF0, R_CDF_PCH_PSF0_PCR_T1_SHDW_PCIE05_RS0_REG_BASE},
  {PID_PSF0, R_CDF_PCH_PSF0_PCR_T1_SHDW_PCIE06_RS0_REG_BASE},
  {PID_PSF0, R_CDF_PCH_PSF0_PCR_T1_SHDW_PCIE07_RS0_REG_BASE},
  {PID_PSF0, R_CDF_PCH_PSF0_PCR_T1_SHDW_PCIE08_RS0_REG_BASE},
  {PID_PSF0, R_CDF_PCH_PSF0_PCR_T1_SHDW_PCIE09_RS0_REG_BASE},
  {PID_PSF0, R_CDF_PCH_PSF0_PCR_T1_SHDW_PCIE10_RS0_REG_BASE},
  {PID_PSF0, R_CDF_PCH_PSF0_PCR_T1_SHDW_PCIE11_RS0_REG_BASE},
  {PID_PSF0, R_CDF_PCH_PSF0_PCR_T1_SHDW_PCIE12_RS0_REG_BASE}
};


/**
  Return PSF_PORT at root PSF level to which PCIe Root Port device is connected

  @param[in] RpIndex        PCIe Root Port Index (0 based)

  @retval    PsfPort        PSF PORT structure for PCIe

**/
PSF_PORT
PsfRootPciePort (
  IN UINT32  RpIndex
  )
{
  PSF_PORT PsfPort = {0,0};
    if (RpIndex < ARRAY_SIZE(mPchPsfRs0PcieRegBase)) {
      return mPchPsfRs0PcieRegBase[RpIndex];
    }
  ASSERT (FALSE);
  return PsfPort;
}

GLOBAL_REMOVE_IF_UNREFERENCED CONST PSF_GRANT_COUNT_REG mPchHGntCntRegs[PCH_MAX_PCIE_CONTROLLERS] = {
    {PID_PSF1, R_CDF_PCH_PSF1_PCR_DEV_GNTCNT_RELOAD_DGCR0, R_CDF_PCH_PSF1_PCR_TARGET_GNTCNT_RELOAD_PG1_TGT0},
    {PID_PSF0, R_CDF_PCH_PSF0_PCR_DEV_GNTCNT_RELOAD_DGCR0, R_CDF_PCH_PSF0_PCR_TARGET_GNTCNT_RELOAD_PG1_TGT0},
    {PID_PSF0, R_CDF_PCH_PSF0_PCR_DEV_GNTCNT_RELOAD_DGCR0, R_CDF_PCH_PSF0_PCR_TARGET_GNTCNT_RELOAD_PG1_TGT0}
  };

/**
  Grant count regs data for PSF that is directly connected to PCIe Root Ports

  @param[in]  Controller     PCIe Root Port Controller index (0 based)
  @param[out] GrantCountReg  Structure with PSF Grant Count register data
**/
VOID
PsfPcieGrantCountBaseReg (
  IN  UINT8                Controller,
  OUT PSF_GRANT_COUNT_REG  *GrantCountReg
  )
{
    ASSERT(Controller < ARRAY_SIZE(mPchHGntCntRegs));
    *GrantCountReg = mPchHGntCntRegs[Controller];
}

GLOBAL_REMOVE_IF_UNREFERENCED CONST UINT8 mDgcrLp [][PCH_PCIE_CONTROLLER_PORTS] = {
    {  8,  9, 10, 11 }, //SPA
    { 12, 13, 14, 15 }, //SPB
    { 16, 17, 18, 19 }, //SPC
    { 20, 21, 22, 23 }  //SPD
  };

GLOBAL_REMOVE_IF_UNREFERENCED CONST UINT8 mPgTgtLp [][PCH_PCIE_CONTROLLER_PORTS] = {
    {  0,  1,  2,  3 }, //SPA
    {  8,  9, 10, 11 }, //SPB
    { 16, 17, 18, 19 }, //SPC
    { 24, 25, 26, 27 }  //SPD
  };


GLOBAL_REMOVE_IF_UNREFERENCED CONST UINT8 mDgcrH [PCH_MAX_PCIE_CONTROLLERS][PCH_PCIE_CONTROLLER_PORTS] = {
    {  1,  1,  1,  1 },
    {  1,  1,  1,  1 },
    {  0,  0,  0,  0 }
  };

GLOBAL_REMOVE_IF_UNREFERENCED CONST UINT8 mPgTgtH [PCH_MAX_PCIE_CONTROLLERS][PCH_PCIE_CONTROLLER_PORTS] = {
    {  1,  2,  3,  4 },
    {  4,  5,  6,  7 },
    {  0,  1,  2,  3 }
  };

/**
  Get Grant Count number (Device Grant Count and Target Grant Count)
  for PSF that is directly connected to PCIe Root Ports

  @param[in]  Controller    PCIe Root Port Controller index
  @param[in]  Channel       PCIe Root Port Channel index
  @param[out] DgcrNo        Device Grant Count number
  @param[out] PgTgtNo       Target Grant Count number
**/
VOID
PsfPcieGrantCountNumber (
  IN  UINT8 Controller,
  IN  UINT8 Channel,
  OUT UINT8 *DgcrNo,
  OUT UINT8 *PgTgtNo
  )
{
    if ((Channel < ARRAY_SIZE(mDgcrH[0])) && (Controller < ARRAY_SIZE(mDgcrH))) {
      *DgcrNo = mDgcrH[Controller][Channel];
      *PgTgtNo = mPgTgtH[Controller][Channel];
      return;
    }
  ASSERT (FALSE);
}

/**
  Grant count regs data for a given PSF-to-PSF port.

  @param[in] PsfTopoPort         PSF-to-PSF port

  @param[out] GrantCountReg      Structure with PSF Grant Count register data
**/
VOID
PsfSegmentGrantCountBaseReg (
  IN PSF_TOPO_PORT         PsfTopoPort,
  OUT PSF_GRANT_COUNT_REG  *GrantCountReg
  )
{
  ASSERT((PsfTopoPort.PsfId == 0) && (PsfTopoPort.PortId == 2));
  *GrantCountReg = mPchHGntCntRegs[1];
}

/**
  Grant Count number (Device Grant Count and Target Grant Count) for a given PSF-to-PSF port.

  @param[in] PsfTopoPort         PSF-to-PSF port
  @param[out] DgcrNo             Device Grant Count number
  @param[out] PgTgtNo            Target Grant Count number
**/
VOID
PsfSegmentGrantCountNumber (
  IN PSF_TOPO_PORT PsfTopoPort,
  OUT UINT8        *DgcrNo,
  OUT UINT8        *PgTgtNo
  )
{
  ASSERT((PsfTopoPort.PsfId == 0) && (PsfTopoPort.PortId == 2));
  *DgcrNo = 2;
  *PgTgtNo = 8;
  }

//
// CDF PSF topology for PCIe
//


GLOBAL_REMOVE_IF_UNREFERENCED CONST PSF_TOPOLOGY mPchHPsf1PcieTreeTopo[] = {
  //{PsfId, Port}, PortType, Child;
  {{1, 1}, PsfPcieCtrlPort, NULL, {0}},
  PSF_TOPOLOGY_END
};

GLOBAL_REMOVE_IF_UNREFERENCED CONST PSF_TOPOLOGY mPchHPsfPcieTreeTopo[] = {
  {{0, 0}, PsfPcieCtrlPort, NULL, {2}},
  {{0, 1}, PsfPcieCtrlPort, NULL, {1}},
  {{0, 2}, PsfToPsfPort, mPchHPsf1PcieTreeTopo},
  PSF_TOPOLOGY_END
};

/**
  Get PSF Pcie Tree topology

  @param[in] PsfTopology          PSF Port from PSF PCIe tree topology

  @retval PsfTopology             PSF PCIe tree topology
**/
CONST PSF_TOPOLOGY*
PsfGetRootPciePsfTopology (
  VOID
  )
{
    return mPchHPsfPcieTreeTopo;
}

/**
  Get EOI register data for given PSF ID

  @param[in]  PsfId           PSF ID (1 - PSF1, 2 - PSF2, ...)
  @param[out] EoiTargetBase   EOI Target register
  @param[out] EoiControlBase  EOI Control register

  @retval MaxTargets          Number of supported targets

**/
UINT8
PsfEoiRegData (
  UINT32        PsfId,
  UINT16        *EoiTargetBase,
  UINT16        *EoiControlBase
  )
{
  UINT8  MaxTargets;

  MaxTargets = 0;
  *EoiTargetBase = 0;
  *EoiControlBase = 0;

  switch (PsfId) {
    case 0:
      *EoiTargetBase = R_CDF_PCH_PSF0_PCR_PSF_MC_AGENT_MCAST0_TGT0_EOI;
      *EoiControlBase = R_CDF_PCH_PSF0_PCR_PSF_MC_CONTROL_MCAST0_EOI;
      MaxTargets = 3;
      break;

    case 1:
      *EoiTargetBase = R_CDF_PCH_PSF1_PCR_PSF_MC_AGENT_MCAST0_TGT0_MCTP0;
      *EoiControlBase = R_CDF_PCH_PSF1_PCR_PSF_MC_CONTROL_MCAST0_MCTP0;
      MaxTargets = 1;
      break;
  }
  return MaxTargets;
}

/**
  Get MCTP register data for given PSF ID

  @param[in]  PsfId            PSF ID (1 - PSF1, 2 - PSF2, ...)
  @param[out] MctpTargetBase   MCTP Target register
  @param[out] MctpControlBase  MCTP Control register

  @retval MaxTargets           Number of supported targets

**/
UINT8
PsfMctpRegData (
  UINT32        PsfId,
  UINT16        *MctpTargetBase,
  UINT16        *MctpControlBase
  )
{
  UINT8  MaxTargets;

  MaxTargets = 0;
  *MctpTargetBase = 0;
  *MctpControlBase = 0;

  switch (PsfId) {
    case 0:
      *MctpTargetBase = R_PCH_PSF_0_PSF_MC_AGENT_MCAST1_RS0_TGT0_MCTP1;
      *MctpControlBase = R_PCH_PSF_0_PSF_MC_CONTROL_MCAST1_RS0_MCTP1;
      MaxTargets = 6;
      break;

    case 1:
      *MctpTargetBase = R_PCH_PSF_1_PSF_MC_AGENT_MCAST0_RS0_TGT0_MCTP0;
      *MctpControlBase = R_PCH_PSF_1_PSF_MC_CONTROL_MCAST0_RS0_MCTP0;
      MaxTargets = 4;
      break;

    case 2:
      *MctpTargetBase = R_PCH_PSF_2_PSF_MC_AGENT_MCAST0_RS0_TGT0_MCTP0;
      *MctpControlBase = R_PCH_PSF_2_PSF_MC_CONTROL_MCAST0_RS0_MCTP0;
      MaxTargets = 3;
      break;

    case 4:
      *MctpTargetBase = R_PCH_PSF_4_PSF_MC_AGENT_MCAST0_RS0_TGT0_MCTP0;
      *MctpControlBase = R_PCH_PSF_4_PSF_MC_AGENT_MCAST0_RS0_TGT0_MCTP0;
      MaxTargets = 1;
      break;

    case 5:
      *MctpTargetBase = R_PCH_PSF_5_PSF_MC_AGENT_MCAST0_RS0_TGT0_MCTP0;
      *MctpControlBase = R_PCH_PSF_5_PSF_MC_CONTROL_MCAST0_RS0_MCTP0;
      MaxTargets = 1;
      break;
  }

  return MaxTargets;
}

/**
  P2SB PSF port Destination ID (psf_id:port_group_id:port_id:channel_id)

  @retval P2SB Destination ID
**/
PSF_PORT_DEST_ID
PsfP2sbDestinationId (
  VOID
  )
{
  PSF_PORT_DEST_ID P2sbTarget;

    P2sbTarget.RegVal = V_CDF_PCH_PSFX_PCR_PSF_MC_AGENT_MCAST_TGT_P2SB;
  return P2sbTarget;
}


GLOBAL_REMOVE_IF_UNREFERENCED PSF_PORT_DEST_ID   PchHRpDestId[] =
{
    0x00018100, 0x00018101, 0x00018102, 0x00018103, // PSF1, PortID = 1
    0x00008100, 0x00008101, 0x00008102, 0x00008103, // PSF0, PortID = 1
    0x00008000, 0x00008001, 0x00008002, 0x00008003  // PSF0, PortID = 0
};

/**
  PCIe PSF port destination ID (psf_id:port_group_id:port_id:channel_id)

  @param[in] RpIndex        PCIe Root Port Index (0 based)

  @retval Destination ID
**/
PSF_PORT_DEST_ID
PsfPcieDestinationId (
  IN UINT32  RpIndex
  )
{
  PSF_PORT_DEST_ID PsfPortDestID = {0};
    if (RpIndex < ARRAY_SIZE(PchHRpDestId)) {
      return PchHRpDestId[RpIndex];
    }
  ASSERT (FALSE);
  return PsfPortDestID;
}

/**
  Return the Function Config PSF_PORT for PCIe Root Port

  @param[in] RpIndex        PCIe Root Port Index (0 based)

  @retval    PsfPort        PSF PORT structure for PCIe Function Config
**/
PSF_PORT
PsfRootPcieFunctionConfigPort (
  IN UINT32  RpIndex
  )
{
  PSF_PORT PsfPort = {0,0};
  return PsfPort;
}

/**
  Return the PSF Function Config Second Level PSF_PORT for PCIe Root Port

  @param[in] RpIndex        PCIe Root Port Index (0 based)

  @retval    PsfPort        PSF PORT structure for PCIe Function Config
**/
PSF_PORT
PsfPcieFunctionConfigSecondLevelPort (
  IN UINT32  RpIndex
  )
{
  PSF_PORT PsfPort = {0,0};
  return PsfPort;
}

/**
  This function enables PCIe Relaxed Order in PSF
**/
VOID
PsfEnablePcieRelaxedOrder (
  VOID
  )
{
  //
  // PCH BIOS Spec Section 8.2.9
  //
  PchPcrAndThenOr32 (
    PID_PSF0,
    R_CDF_PCH_PSF_PCR_PORT_CONFIG_PG0_PORT0,
    ~0u,
    BIT1
    );
    PchPcrAndThenOr32 (
    PID_PSF0,
    R_CDF_PCH_PSF_PCR_PORT_CONFIG_PG1_PORT0,
      ~0u,
      BIT2
      );
    PchPcrAndThenOr32 (
    PID_PSF0,
    R_CDF_PCH_PSF_PCR_PORT_CONFIG_PG1_PORT1,
      ~0u,
      BIT2
      );
}

/**
  Return PSF_PORT for CNVi device

  @retval    PsfPort         PSF PORT structure for CNVi device
**/
PSF_PORT
PsfCnviPort (
  VOID
  )
{
  PSF_PORT PsfPort = {0,0};
  return PsfPort;
}

/**
  Return PSF_PORT for PMC device

  @retval    PsfPort         PSF PORT structure for PMC device
**/
PSF_PORT
PsfPmcPort (
  VOID
  )
{
  PSF_PORT PsfPort;

  PsfPort.PsfPid = PID_PSF3;

  PsfPort.RegBase = R_CDF_PCH_PSF3_PCR_T0_SHDW_PMC_REG_BASE;

  return PsfPort;
}

GLOBAL_REMOVE_IF_UNREFERENCED PSF_SEGMENT mCdfPsfTable[]  =
{
  {0, PID_PSF0},
  {1, PID_PSF1},
  {2, PID_PSF2},
  {3, PID_PSF3},
  {4, PID_PSF_CSME},
  {5, PID_PSF_IE},
  {6, PID_PSF_SATA0},
  {7, PID_PSF_SATA1},
  {8, PID_PSF_SATA2},
  {9, PID_PSFS}
};

/**
  Get list of supported PSF segments.

  @param[out] PsfTable        Array of supported PSF segments
  @param[out] PsfTableLength  Length of PsfTable
**/
VOID
PsfSegments (
  OUT PSF_SEGMENT  **PsfTable,
  OUT UINT32       *PsfTableLength
  )
{
    *PsfTable = mCdfPsfTable;
    *PsfTableLength = ARRAY_SIZE(mCdfPsfTable);
}

/**
  Disable D3:F0 device at PSF level for PCH-LP
**/
VOID
PsfDisableD3F0 (
  VOID
  )
{
  return;
}

/**
  Disable PSF upstream completion tracking for HDAudio on PCH-LP
**/
VOID
PsfDisableUpstreamCompletionTrackingForHda (
  VOID
  )
{
  return;
}

/**
  Check if MCTP is supported

  @retval TRUE              MCTP is supported
          FALSE             MCTP is not supported
**/
BOOLEAN
PsfIsMctpSupported (
  VOID
  )
{
  return TRUE;
}

/**
  PSF PCIe channel grant counts

  @param[in]  PsfPcieCtrlConfig        PCIe controller configuration
  @param[out] ChannelGrant             4 item array with PCIe Channel Grant Count values
**/
VOID
PsfPcieChannelGrantCounts (
  IN  PCIE_BIFURCATION_CONFIG  PsfPcieCtrlConfig,
  OUT UINT32                   *ChannelGrant
  )
{
  switch (PsfPcieCtrlConfig) {
    case PcieBifurcation4x2: //PcieBifurcation4x2
      ChannelGrant[0] = 1;
      ChannelGrant[1] = 1;
      ChannelGrant[2] = 1;
      ChannelGrant[3] = 1;
      break;
    case PcieBifurcation1x4_2x2: //PcieBifurcation1x4_2x2
      ChannelGrant[0] = 2;
      ChannelGrant[1] = DEFAULT_PCIE_GRANT_COUNT;
      ChannelGrant[2] = 1;
      ChannelGrant[3] = 1;
      break;
    case PcieBifurcation2x2_1x4: //PcieBifurcation2x2_1x4
      ChannelGrant[0] = 1;
      ChannelGrant[1] = 1;
      ChannelGrant[2] = 2;
      ChannelGrant[3] = DEFAULT_PCIE_GRANT_COUNT;
      break;
    case PcieBifurcation2x4: //PcieBifurcation2x4
      ChannelGrant[0] = 2;
      ChannelGrant[1] = DEFAULT_PCIE_GRANT_COUNT;
      ChannelGrant[2] = 2;
      ChannelGrant[3] = DEFAULT_PCIE_GRANT_COUNT;
      break;
    case PcieBifurcation1x8: //PcieBifurcation1x8
      ChannelGrant[0] = 4;
      ChannelGrant[1] = DEFAULT_PCIE_GRANT_COUNT;
      ChannelGrant[2] = DEFAULT_PCIE_GRANT_COUNT;
      ChannelGrant[3] = DEFAULT_PCIE_GRANT_COUNT;
      break;
    default:
      ASSERT (FALSE);
      break;
  }
}

/**
  Disable PCIe Root Port at PSF level

  @param[in] RpIndex        PCIe Root Port Index (0 based)
**/
VOID
PsfDisablePcieRootPort (
  IN UINT32  RpIndex
  )
{
  PsfDisableBridge (PsfRootPciePort (RpIndex));
}

/**
  CDF PCIe Root Ports are single function devices
  They do not require function swapping
  This function is a stub
**/
VOID
PsfReloadDefaultPcieRpFunc (
  VOID
  )
{
  return;
}

/**
  CDF PCIe Root Ports are single function devices
  They do not require function swapping
  This function is a stub

  @param[in] RpIndex        PCIe Root Port Index (0 based)
  @param[in] NewFunction    New Function number
**/
VOID
PsfSetPcieFunction (
  IN UINT32  RpIndex,
  IN UINT32  NewFunction
  )
{
  return;
}

/**
  Set nCPM Secondary Bus for Root Space 3 same value as Root Space 0
**/
VOID
PsfSetImrSettings (
  VOID
  )
{
  UINT32 Value;

  //
  // Unmask PSF1 before trying to access it
  //
  P2sbEpUnmask (PID_PSF1);

  Value = PchPcrRead32 (
  PID_PSF1,
  R_CDF_PCH_PSF0_PCR_T1_SHDW_BUS_NCPM_RS0_REG_BASE
  );

  PchPcrWrite32 (
  PID_PSF1,
  R_CDF_PCH_PSF0_PCR_T1_SHDW_BUS_NCPM_RS3_REG_BASE,
  Value
  );

  Value = PchPcrRead32 (
  PID_PSF1,
  R_CDF_PCH_PSF0_PCR_T1_SHDW_PCIEN_NCPM_RS0_REG_BASE
  );

  PchPcrWrite32 (
  PID_PSF1,
  R_CDF_PCH_PSF0_PCR_T1_SHDW_PCIEN_NCPM_RS3_REG_BASE,
  Value
  );

  P2sbEpMask (PID_PSF1);
}


VOID
PsfConfigureParityDetection (
  VOID
  )
{
  UINT32          PsfTableIndex;
  PSF_SEGMENT     *PsfTable;
  UINT32          PsfTableSize;

  PsfSegments (&PsfTable, &PsfTableSize);

  for (PsfTableIndex = 0; PsfTableIndex < PsfTableSize; PsfTableIndex++) {
    P2sbEpUnmask (PsfTable[PsfTableIndex].SbPid);
    PchPcrAndThenOr32 (PsfTable[PsfTableIndex].SbPid, R_PCH_PSF_PCR_GLOBAL_CONFIG, ~0u, B_PCH_PSF_PCR_GLOBAL_CONFIG_ENCMDPARDETINGRESS);
    P2sbEpMask (PsfTable[PsfTableIndex].SbPid);
  }
}
