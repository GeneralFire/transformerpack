/** @file
  This file contains internal PSF routines for PCH PSF SoC lib usage

@copyright
  INTEL CONFIDENTIAL
  Copyright 2019 - 2020 Intel Corporation.

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
#include <Library/SataSocLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/SataSocLib.h>
#include <Register/PchRegsPsf.h>
#include <Register/PsfRegsEbg.h>
#include <Register/PchPcrRegs.h>
#include <Library/PsfLib.h>
#include <Library/PsfSocLib.h>
#include <PchPcieRpInfo.h>
#include <PchLimits.h>

GLOBAL_REMOVE_IF_UNREFERENCED PSF_PORT mPchSerialIoUartPsfPort[] =
{
  {PID_PSF3, R_VER3_PCH_PSF3_PCR_T0_SHDW_UART0_REG_BASE},
  {PID_PSF3, R_VER3_PCH_PSF3_PCR_T0_SHDW_UART1_REG_BASE},
};

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
  ASSERT (FALSE);
  return PSF_PORT_NULL;
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
  ASSERT (FALSE);
  return PSF_PORT_NULL;
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
  if (UartNum < ARRAY_SIZE (mPchSerialIoUartPsfPort)) {
    return mPchSerialIoUartPsfPort[UartNum];
  }

  ASSERT (FALSE);
  return PSF_PORT_NULL;
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
  PsfPort.RegBase = R_VER3_PCH_PSF2_PCR_T0_SHDW_NPK_RS0_REG_BASE;

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
  PsfPort.RegBase = R_VER3_PCH_PSF2_PCR_T0_SHDW_NPK_ACPI_REG_BASE;

  return PsfPort;
}

GLOBAL_REMOVE_IF_UNREFERENCED UINT16 mPchHHeciPsfRegBase[] =
{
  R_VER3_PCH_PSF1_PCR_T0_SHDW_HECI1_REG_BASE,
  R_VER3_PCH_PSF1_PCR_T0_SHDW_HECI2_REG_BASE,
  R_VER3_PCH_PSF1_PCR_T0_SHDW_HECI3_REG_BASE,
  R_VER3_PCH_PSF1_PCR_T0_SHDW_HECI4_REG_BASE
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

  PsfPort.PsfPid = PID_PSF1;
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

  PsfPort.PsfPid = PID_PSF1;
  PsfPort.RegBase = R_VER3_PCH_PSF1_PCR_T0_SHDW_KT_BASE;

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

  PsfPort.PsfPid = PID_PSF1;
  PsfPort.RegBase = R_VER3_PCH_PSF1_PCR_T0_SHDW_IDER_REG_BASE;
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
  PSF_PORT PsfPort;

  PsfPort.PsfPid = PID_PSF3;
  PsfPort.RegBase = R_VER3_PCH_PSF3_PCR_T0_SHDW_HDA_REG_BASE;
  PsfDisableDevice (PsfPort);
}

/**
  Disable DSP Bar (HdAudio BAR4 and BAR5) at PSF level.
**/
VOID
PsfDisableDspBar (
  VOID
  )
{
  PSF_PORT PsfPort;

  PsfPort.PsfPid = PID_PSF3;
  PsfPort.RegBase = R_VER3_PCH_PSF3_PCR_T0_SHDW_HDA_REG_BASE;

  PsfDisableDeviceBar (PsfPort, (R_PCH_PSFX_PCR_T0_SHDW_BAR5 | R_PCH_PSFX_PCR_T0_SHDW_BAR4));
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
  PsfPort.RegBase = R_VER3_PCH_PSF2_PCR_T0_SHDW_XHCI_REG_BASE;
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

  ASSERT (SataCtrlIndex < MaxSataControllerNum ());

  PsfPort.PsfPid = PID_PSF2;
  if (SataCtrlIndex == SATA_1_CONTROLLER_INDEX) {
    PsfPort.RegBase = R_VER3_PCH_PSF2_PCR_T0_SHDW_SATA1_REG_BASE;
  } else if (SataCtrlIndex == SATA_2_CONTROLLER_INDEX) {
    PsfPort.RegBase = R_VER3_PCH_PSF2_PCR_T0_SHDW_SATA2_REG_BASE;
  } else if (SataCtrlIndex == SATA_3_CONTROLLER_INDEX) {
    PsfPort.RegBase = R_VER3_PCH_PSF2_PCR_T0_SHDW_SATA3_REG_BASE;
  } else {
    return;
  }
  PsfDisableDevice (PsfPort);
}

/**
  Disable GbE device at PSF level
**/
VOID
PsfDisableGbeDevice (
  VOID
  )
{
  PSF_PORT PsfPort;

  PsfPort.PsfPid = PID_PSF3;
  PsfPort.RegBase = R_VER3_PCH_PSF3_PCR_T0_SHDW_GBE_RS0_REG_BASE;
  PsfDisableDevice (PsfPort);
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
  PsfPort.RegBase = R_VER3_PCH_PSF3_PCR_T0_SHDW_DMA_SMBUS_REG_BASE;
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
  PsfPort.RegBase = R_VER3_PCH_PSF3_PCR_T0_SHDW_DMA_SMBUS_REG_BASE;
  PsfDisableDevice (PsfPort);
}

//
// PSF1_T1_SHDW_PCIExx_REG_BASE registers for VER3 PCH
//
GLOBAL_REMOVE_IF_UNREFERENCED UINT16 mPchPsfRs0PcieRegBase[] =
{
  R_VER3_PCH_PSF1_PCR_T1_SHDW_PCIE00_RS0_REG_BASE,
  R_VER3_PCH_PSF1_PCR_T1_SHDW_PCIE01_RS0_REG_BASE,
  R_VER3_PCH_PSF1_PCR_T1_SHDW_PCIE02_RS0_REG_BASE,
  R_VER3_PCH_PSF1_PCR_T1_SHDW_PCIE03_RS0_REG_BASE,
  R_VER3_PCH_PSF1_PCR_T1_SHDW_PCIE04_RS0_REG_BASE,
  R_VER3_PCH_PSF1_PCR_T1_SHDW_PCIE05_RS0_REG_BASE,
  R_VER3_PCH_PSF1_PCR_T1_SHDW_PCIE06_RS0_REG_BASE,
  R_VER3_PCH_PSF1_PCR_T1_SHDW_PCIE07_RS0_REG_BASE,
  R_VER3_PCH_PSF1_PCR_T1_SHDW_PCIE08_RS0_REG_BASE,
  R_VER3_PCH_PSF1_PCR_T1_SHDW_PCIE09_RS0_REG_BASE,
  R_VER3_PCH_PSF1_PCR_T1_SHDW_PCIE10_RS0_REG_BASE,
  R_VER3_PCH_PSF1_PCR_T1_SHDW_PCIE11_RS0_REG_BASE,
  R_VER3_PCH_PSF1_PCR_T1_SHDW_PCIE12_RS0_REG_BASE,
  R_VER3_PCH_PSF1_PCR_T1_SHDW_PCIE13_RS0_REG_BASE,
  R_VER3_PCH_PSF1_PCR_T1_SHDW_PCIE14_RS0_REG_BASE,
  R_VER3_PCH_PSF1_PCR_T1_SHDW_PCIE15_RS0_REG_BASE
};

GLOBAL_REMOVE_IF_UNREFERENCED PSF_PORT mPchPsfPcieSecondLevelPort[] =
{
  {0, 0},
  {0, 0},
  {0, 0},
  {0, 0},
  {PID_PSF8, R_VER3_PCH_PSF8_PCR_T1_SHDW_PCIE04_RS0_REG_BASE},
  {PID_PSF8, R_VER3_PCH_PSF8_PCR_T1_SHDW_PCIE05_RS0_REG_BASE},
  {PID_PSF8, R_VER3_PCH_PSF8_PCR_T1_SHDW_PCIE06_RS0_REG_BASE},
  {PID_PSF8, R_VER3_PCH_PSF8_PCR_T1_SHDW_PCIE07_RS0_REG_BASE},
  {PID_PSF8, R_VER3_PCH_PSF8_PCR_T1_SHDW_PCIE08_RS0_REG_BASE},
  {PID_PSF8, R_VER3_PCH_PSF8_PCR_T1_SHDW_PCIE09_RS0_REG_BASE},
  {PID_PSF8, R_VER3_PCH_PSF8_PCR_T1_SHDW_PCIE10_RS0_REG_BASE},
  {PID_PSF8, R_VER3_PCH_PSF8_PCR_T1_SHDW_PCIE11_RS0_REG_BASE},
  {0, 0},
  {0, 0},
  {0, 0},
  {0, 0}
};

/**
  Return second level PSF_PORT to which PCIE Root Port device is connected (directly)

  @param[in] RpIndex        PCIe Root Port Index (0 based)

  @retval    PsfPort        PSF PORT structure for PCIe
**/
PSF_PORT
PsfPcieSecondLevelPort (
  IN UINT32  RpIndex
  )
{
  if (RpIndex < ARRAY_SIZE (mPchPsfPcieSecondLevelPort)) {
    return mPchPsfPcieSecondLevelPort[RpIndex];
  }
  ASSERT (FALSE);
  return PSF_PORT_NULL;
}

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
  PSF_PORT PsfPort;
  if (RpIndex < ARRAY_SIZE (mPchPsfRs0PcieRegBase)) {
    PsfPort.PsfPid = PID_PSF1;
    PsfPort.RegBase = mPchPsfRs0PcieRegBase[RpIndex];
    return PsfPort;
  }
  ASSERT (FALSE);
  return PSF_PORT_NULL;
}

/**
  Return RS3 PSF_PORT at root PSF level to which PCIe Root Port device is connected

  @param[in] RpIndex        PCIe Root Port Index (0 based)

  @retval    PsfPort        PSF PORT structure for PCIe
**/
PSF_PORT
PsfRootRs3PciePort (
  IN UINT32  RpIndex
  )
{
  PSF_PORT PsfPort = {0, 0};
  return PsfPort;
}

GLOBAL_REMOVE_IF_UNREFERENCED CONST PSF_GRANT_COUNT_REG mPchGntCntRegs[PCH_MAX_PCIE_CONTROLLERS] = {
  {PID_PSF8, R_VER3_PCH_PSF1_PCR_DEV_GNTCNT_RELOAD_DGCR0, R_VER3_PCH_PSF8_PCR_TARGET_GNTCNT_RELOAD_PG1_TGT0},
  {PID_PSF8, R_VER3_PCH_PSF8_PCR_DEV_GNTCNT_RELOAD_DGCR0, R_VER3_PCH_PSF8_PCR_TARGET_GNTCNT_RELOAD_PG1_TGT0},
  {PID_PSF8, R_VER3_PCH_PSF1_PCR_DEV_GNTCNT_RELOAD_DGCR0, R_VER3_PCH_PSF8_PCR_TARGET_GNTCNT_RELOAD_PG1_TGT0},
  {PID_PSF8, R_VER3_PCH_PSF1_PCR_DEV_GNTCNT_RELOAD_DGCR0, R_VER3_PCH_PSF8_PCR_TARGET_GNTCNT_RELOAD_PG1_TGT0}
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
  ASSERT(Controller < ARRAY_SIZE (mPchGntCntRegs));
  *GrantCountReg = mPchGntCntRegs[Controller];
}


GLOBAL_REMOVE_IF_UNREFERENCED CONST UINT8 mDgcr [][PCH_PCIE_CONTROLLER_PORTS] = {
  { 16, 15, 13, 12 }, //SPA
  {  8,  7,  6,  5 }, //SPB
  {  4,  3,  2,  1 }, //SPC
  { 11, 10,  8,  7 }  //SPD
};

GLOBAL_REMOVE_IF_UNREFERENCED CONST UINT8 mPgTgt [][PCH_PCIE_CONTROLLER_PORTS] = {
  {  0,  1,  2,  3 }, //SPA
  {  0,  1,  2,  3 }, //SPB
  {  4,  5,  6,  7 }, //SPC
  {  4,  5,  6,  7 }  //SPD
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
  if ((Channel < ARRAY_SIZE (mDgcr[0])) && (Controller < ARRAY_SIZE (mDgcr))) {
    *DgcrNo = mDgcr[Controller][Channel];
    *PgTgtNo = mPgTgt[Controller][Channel];
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
  DEBUG ((DEBUG_INFO, "(PsfTopoPort.PsfId == %d) && (PsfTopoPort.PortId == %d)",PsfTopoPort.PsfId,PsfTopoPort.PortId));
  *GrantCountReg = mPchGntCntRegs[1];
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
  DEBUG ((DEBUG_INFO, "(PsfTopoPort.PsfId == %d) && (PsfTopoPort.PortId == %d)",PsfTopoPort.PsfId,PsfTopoPort.PortId));
  *DgcrNo = 2;
  *PgTgtNo = 8;
}

//
// VER3 PSF topology for PCIe
//

GLOBAL_REMOVE_IF_UNREFERENCED CONST PSF_TOPOLOGY mPchPsf8PcieTreeTopo[] = {
  //{PsfId, Port}, PortType, Child;
  {{8, 0}, PsfPcieCtrlPort, NULL, {1}},//SPB
  {{8, 1}, PsfPcieCtrlPort, NULL, {2}},//SPC
  PSF_TOPOLOGY_END
};

GLOBAL_REMOVE_IF_UNREFERENCED CONST PSF_TOPOLOGY mPchPsf1PcieTreeTopo[] = {
  //{PsfId, Port}, PortType, Child;
  {{1, 0}, PsfPcieCtrlPort, NULL, {0}}, //SPA
  {{1, 1}, PsfPcieCtrlPort, NULL, {3}}, //SPD
  {{1, 2}, PsfToPsfPort, mPchPsf8PcieTreeTopo, {0}},
  PSF_TOPOLOGY_END
};

GLOBAL_REMOVE_IF_UNREFERENCED CONST PSF_TOPOLOGY mPchPsfPcieTreeTopo[] = {
  {{0, 0}, PsfToPsfPort, mPchPsf1PcieTreeTopo},
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
  return mPchPsfPcieTreeTopo;
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
    case 1:
      *EoiTargetBase = R_VER3_PCH_PSF1_PCR_MC_AGENT_MCAST0_TGT0_EOI;
      *EoiControlBase = R_VER3_PCH_PSF1_PCR_MC_CONTROL_MCAST0_EOI;
      MaxTargets = 21;
      break;

    case 3:
      *EoiTargetBase = R_VER3_PCH_PSF3_PCR_MC_AGENT_MCAST0_TGT0_EOI;
      *EoiControlBase = R_VER3_PCH_PSF3_PCR_MC_CONTROL_MCAST0_EOI;
      MaxTargets = 1;
      break;
    case 8:
      *EoiTargetBase = R_VER3_PCH_PSF8_PCR_MC_AGENT_MCAST0_TGT0_EOI;
      *EoiControlBase = R_VER3_PCH_PSF8_PCR_MC_CONTROL_MCAST0_EOI;
      MaxTargets = 8;
      break;

    default:
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
    case 1:
      *MctpTargetBase = R_VER3_PCH_PSF1_PCR_MC_AGENT_MCAST1_TGT0_MCTP1;
      *MctpControlBase = R_VER3_PCH_PSF1_PCR_MC_CONTROL_MCAST1_MCTP1;
      MaxTargets = 10;
      break;

    case 4:
      *MctpTargetBase = R_VER3_PCH_PSF4_PCR_MC_AGENT_MCAST0_TGT0_MCTP0;
      *MctpControlBase = R_VER3_PCH_PSF4_PCR_MC_CONTROL_MCAST0_MCTP0;
      MaxTargets = 1;
      break;

    case 8:
      *MctpTargetBase = R_VER3_PCH_PSF8_PCR_MC_AGENT_MCAST1_TGT0_MCTP1;
      *MctpControlBase = R_VER3_PCH_PSF8_PCR_MC_CONTROL_MCAST1_MCTP1;
      MaxTargets = 8;
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

  P2sbTarget.RegVal = V_VER3_PCH_PSFX_PCR_PSF_MC_AGENT_MCAST_TGT_P2SB;
  return P2sbTarget;
}

/**
  DMI PSF port Destination ID (psf_id:port_group_id:port_id:channel_id)

  @retval DMI Destination ID
**/
PSF_PORT_DEST_ID
PsfDmiDestinationId (
  VOID
  )
{
  PSF_PORT_DEST_ID DmiTarget;

  DmiTarget.RegVal = V_VER3_PCH_PSFX_PSF_PCR_MC_AGENT_MCAST_TGT_DMI;
  return DmiTarget;
}

GLOBAL_REMOVE_IF_UNREFERENCED PSF_PORT_DEST_ID PchRpDestId[] =
{
  {0x18000}, {0x18001}, {0x18002}, {0x18003}, // SPA: PortID = 0
  {0x88000}, {0x88001}, {0x88002}, {0x88203}, // SPB: PortID = 0
  {0x88100}, {0x88101}, {0x88102}, {0x88103}, // SPC: PortID = 1
  {0x18100}, {0x18101}, {0x18102}, {0x18103}, // SPD: PortID = 1
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

  if (RpIndex < ARRAY_SIZE (PchRpDestId)) {
    return PchRpDestId[RpIndex];
  }
  ASSERT (FALSE);
  return PsfPortDestID;
}

//
// RS0 PSF1_T1_AGENT_FUNCTION_CONFIG_SPx_Dxx_Fx registers for PCH-LP
//
GLOBAL_REMOVE_IF_UNREFERENCED UINT16 mPchPsf1PcieFuncCfgBase[] =
{
  R_VER3_PCH_PSF1_PCR_T1_AGENT_FUNCTION_CONFIG_SPA_RS0_D8_F0,
  R_VER3_PCH_PSF1_PCR_T1_AGENT_FUNCTION_CONFIG_SPA_RS0_D9_F0,
  R_VER3_PCH_PSF1_PCR_T1_AGENT_FUNCTION_CONFIG_SPA_RS0_D10_F0,
  R_VER3_PCH_PSF1_PCR_T1_AGENT_FUNCTION_CONFIG_SPA_RS0_D11_F0,
  R_VER3_PCH_PSF1_PCR_T1_AGENT_FUNCTION_CONFIG_SPB_RS0_D12_F0,
  R_VER3_PCH_PSF1_PCR_T1_AGENT_FUNCTION_CONFIG_SPB_RS0_D13_F0,
  R_VER3_PCH_PSF1_PCR_T1_AGENT_FUNCTION_CONFIG_SPB_RS0_D14_F0,
  R_VER3_PCH_PSF1_PCR_T1_AGENT_FUNCTION_CONFIG_SPB_RS0_D15_F0,
  R_VER3_PCH_PSF1_PCR_T1_AGENT_FUNCTION_CONFIG_SPC_RS0_D16_F0,
  R_VER3_PCH_PSF1_PCR_T1_AGENT_FUNCTION_CONFIG_SPC_RS0_D17_F0,
  R_VER3_PCH_PSF1_PCR_T1_AGENT_FUNCTION_CONFIG_SPC_RS0_D18_F0,
  R_VER3_PCH_PSF1_PCR_T1_AGENT_FUNCTION_CONFIG_SPC_RS0_D19_F0,
  R_VER3_PCH_PSF1_PCR_T1_AGENT_FUNCTION_CONFIG_SPD_RS0_D26_F0,
  R_VER3_PCH_PSF1_PCR_T1_AGENT_FUNCTION_CONFIG_SPD_RS0_D27_F0,
  R_VER3_PCH_PSF1_PCR_T1_AGENT_FUNCTION_CONFIG_SPD_RS0_D28_F0,
  R_VER3_PCH_PSF1_PCR_T1_AGENT_FUNCTION_CONFIG_SPD_RS0_D29_F0
};

GLOBAL_REMOVE_IF_UNREFERENCED PSF_PORT mPchPsfPcieFuncCfgSecondLevelPort[] =
{
  {0, 0},
  {0, 0},
  {0, 0},
  {0, 0},
  {PID_PSF8, R_VER3_PCH_PSF8_PCR_T1_AGENT_FUNCTION_CONFIG_SPB_RS0_D12_F0},
  {PID_PSF8, R_VER3_PCH_PSF8_PCR_T1_AGENT_FUNCTION_CONFIG_SPB_RS0_D13_F0},
  {PID_PSF8, R_VER3_PCH_PSF8_PCR_T1_AGENT_FUNCTION_CONFIG_SPB_RS0_D14_F0},
  {PID_PSF8, R_VER3_PCH_PSF8_PCR_T1_AGENT_FUNCTION_CONFIG_SPB_RS0_D15_F0},
  {PID_PSF8, R_VER3_PCH_PSF8_PCR_T1_AGENT_FUNCTION_CONFIG_SPC_RS0_D16_F0},
  {PID_PSF8, R_VER3_PCH_PSF8_PCR_T1_AGENT_FUNCTION_CONFIG_SPC_RS0_D17_F0},
  {PID_PSF8, R_VER3_PCH_PSF8_PCR_T1_AGENT_FUNCTION_CONFIG_SPC_RS0_D18_F0},
  {PID_PSF8, R_VER3_PCH_PSF8_PCR_T1_AGENT_FUNCTION_CONFIG_SPC_RS0_D19_F0},
  {0, 0},
  {0, 0},
  {0, 0},
  {0, 0}
};
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
  PSF_PORT  PsfPort;

  PsfPort.PsfPid = PID_PSF1;

  if (RpIndex < ARRAY_SIZE (mPchPsf1PcieFuncCfgBase)) {
    PsfPort.RegBase = mPchPsf1PcieFuncCfgBase[RpIndex];
    return PsfPort;
  }
  ASSERT (FALSE);
  return PSF_PORT_NULL;
}

/**
  Return the PSF (Root level) RS3 Function Config PSF_PORT for PCIe Root Port

  @param[in] RpIndex        PCIe Root Port Index (0 based)

  @retval    PsfPort        PSF PORT structure for PCIe Function Config
**/
PSF_PORT
PsfRootRs3PcieFunctionConfigPort (
  IN UINT32  RpIndex
  )
{
  PSF_PORT  PsfPort = {0,0};

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
  if (RpIndex < ARRAY_SIZE (mPchPsfPcieFuncCfgSecondLevelPort)) {
    return mPchPsfPcieFuncCfgSecondLevelPort[RpIndex];
  }
  ASSERT (FALSE);
  return PSF_PORT_NULL;
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
  PsfPort.RegBase = R_VER3_PCH_PSF3_PCR_T0_SHDW_PMC_REG_BASE;

  return PsfPort;
}

GLOBAL_REMOVE_IF_UNREFERENCED PSF_SEGMENT mVer3PsfTable[]  =
{
  {1, PID_PSF1},
  {2, PID_PSF2},
  {3, PID_PSF3},
  {4, PID_CSME_PSF},
  {5, PID_PSF5},
  {6, PID_PSF6},
  {7, PID_PSF7},
  {8, PID_PSF8}
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
  *PsfTable = mVer3PsfTable;
  *PsfTableLength = ARRAY_SIZE (mVer3PsfTable);
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
  This function returns Psf Port Relaxed Ordering Configs

  @param[out] PsfPortRelaxedOrderingConfigRegs                          PCH Series specific table
  @param[out] PsfPortRelaxedOrderingConfigRegsTableSize                 PCH Series specific table size
  @param[out] PsfPortRelaxedOrderingConfigRegsPchTypeSpecific           PCH type specific table
  @param[out] PsfPortRelaxedOrderingConfigRegsPchTypeSpecificTableSize  PCH type specific table size
**/
VOID
GetPsfPortRelaxedOrderingTables (
  PSF_PORT_RELAXED_ORDERING_CONFIG_REG** PsfPortRelaxedOrderingConfigRegs,
  UINT32*                               PsfPortRelaxedOrderingConfigRegsTableSize,
  PSF_PORT_RELAXED_ORDERING_CONFIG_REG** PsfPortRelaxedOrderingConfigRegsPchTypeSpecific,
  UINT32*                               PsfPortRelaxedOrderingConfigRegsPchTypeSpecificTableSize
  )
{
  *PsfPortRelaxedOrderingConfigRegs                         = NULL;
  *PsfPortRelaxedOrderingConfigRegsTableSize                = 0;
  *PsfPortRelaxedOrderingConfigRegsPchTypeSpecific          = NULL;
  *PsfPortRelaxedOrderingConfigRegsPchTypeSpecificTableSize = 0;
}

/**
  Return RC_OWNER value to program

  @retval RC_OWNER
**/
UINT32
PsfGetRcOwner (
  )
{
  return V_VER3_PCH_PSFX_PCR_RC_OWNER_PMT;
}
