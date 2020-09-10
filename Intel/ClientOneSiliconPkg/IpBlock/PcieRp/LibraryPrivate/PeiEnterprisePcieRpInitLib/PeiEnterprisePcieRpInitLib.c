/** @file
  Implementation of the library for enterprise PCIe root ports initialization

@copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation.

  The source code contained or described herein and all documents related to the
  source code ("Material") are owned by Intel Corporation or its suppliers or
  licensors. Title to the Material remains with Intel Corporation or its suppliers
  and licensors. The Material may contain trade secrets and proprietary and
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

#include <Library/IoLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/PchCycleDecodingLib.h>
#include <Ppi/SiPolicy.h>
#include <Library/PchFiaLib.h>
#include <Library/PchInfoLib.h>
#include <Library/PeiItssLib.h>
#include <PchLimits.h>
#include <Uefi/UefiBaseType.h>
#include <Library/PchDmiLib.h>
#include <Library/PchSbiAccessLib.h>
#include <Library/PeiHsioLib.h>
#include <Library/PeiPcieRpLib.h>
#include <Library/PcieRpLib.h>
#include <Library/DebugLib.h>
#include <Register/PchRegs.h>
#include <Library/PchPcieRpLib.h>
#include <PchPcieRpInfo.h>
#include <Register/PchPcrRegs.h>
#include <PcieRegs.h>
#include <Library/HobLib.h>
#include <Library/PsfLib.h>
#include <Library/PeiPchPcieClocksLib.h>
#include <Library/TimerLib.h>
#include <Library/PmcPrivateLib.h>
#include <Library/PmcSocLib.h>
#include <Library/PciExpressHelpersLib.h>
#include <Library/PcieHelperLib.h>
#include <Library/PchPciBdfLib.h>
#include <Register/EnterprisePcieRpRegs.h>
#include <Library/PeiEnterprisePcieRpInitLib.h>
#include <Library/BaseMemoryLib.h>

#include "PeiEnterprisePcieRpInitInternal.h"

#define LINK_ACTIVE_POLL_INTERVAL 100     // in microseconds
#define LINK_ACTIVE_POLL_TIMEOUT  1000000 // in microseconds

#define L1_EXIT_LATENCY           4 //  8us to less than 16us

#define PCH_MAX_PCIE_PTM_DELAY_REGISTER             6

typedef struct {
  UINT32 AndValue;
  UINT32 OrValue;
} PTM_PIPE_STAGE_DELAY_VALUE;

typedef struct {
  PTM_PIPE_STAGE_DELAY_VALUE Register[PCH_MAX_PCIE_PTM_DELAY_REGISTER];
} PTM_PIPE_STAGE_DELAY_CONTROLLER;

GLOBAL_REMOVE_IF_UNREFERENCED CONST UINT32 mPtmRegisters[PCH_MAX_PCIE_PTM_DELAY_REGISTER] =
  {
  R_PCH_PCIE_CFG_PTMG1TXPSD,
  R_PCH_PCIE_CFG_PTMG1RXPSD,
  R_PCH_PCIE_CFG_PTMG2TXPSD,
  R_PCH_PCIE_CFG_PTMG2RXPSD,
  R_PCH_PCIE_CFG_PTMG3TXPSD,
  R_PCH_PCIE_CFG_PTMG3RXPSD
  };

GLOBAL_REMOVE_IF_UNREFERENCED CONST PTM_PIPE_STAGE_DELAY_CONTROLLER mPtmPipeStageDelay [PCH_MAX_PCIE_CONTROLLERS] = {
  {{{0x0, 0x0},{0x0, 0x0},{0x0, 0x0},{0x0, 0x0},{0x0, 0x0},{0x0, 0x0}}},
  {{{0x0, 0x0},{0x0, 0x0},{0x0, 0x0},{0x0, 0x0},{0x0, 0x0},{0x0, 0x0}}},
  {{{0x0, 0x0},{0x0, 0x0},{0x0, 0x0},{0x0, 0x0},{0x0, 0x0},{0x0, 0x0}}}
};

typedef enum {
  PchPcieL1EL1us,  // 0: Less than 1us
  PchPcieL1EL2us,  // 1: 1us to less than 2us
  PchPcieL1EL4us,  // 2: 2us to less than 4us
  PchPcieL1EL8us,  // 3: 4us to less than 8us
  PchPcieL1EL16us, // 4: 8us to less than 16us
  PchPcieL1EL32us, // 5: 16us to less than 32us
  PchPcieL1EL64us, // 6: 32us to less than 64us
  PchPcieL1ELMax   // 7: More than 64us
} PCH_PCIE_L1_EXIT_LATENCY;

/**
  Device information structure
**/
typedef struct {
  UINT16  Vid;
  UINT16  Did;
  UINT8   MaxLinkSpeed;
} PCIE_DEVICE_INFO;

/**
  This function reads Pci Config register via SBI Access

  @param[in]  RpHandle            Pointer to PCIE RP handle
  @param[in]  Offset              Offset of Config register
  @param[out] *Data32             Value of Config register

  @retval EFI_SUCCESS             SBI Read successful.
**/
EFI_STATUS
PchSbiRpPciRead32 (
  IN    PCH_RP_HANDLE *RpHandle,
  IN    UINT32  Offset,
  OUT   UINT32  *Data32
  )
{
  EFI_STATUS    Status;
  UINT8         Response;
  UINT16        Fid;
  UINT32        RpIndex;
  PCH_RP_DEVICE *RpDevice;

  RpDevice = RpHandle->Device;
  RpIndex = RpDevice->RootPortIndex;

  Fid = (UINT16) ((RpDevice->Device << 3) | (RpIndex % 4));
  if (RpHandle->Callback->PchRpSbiExecutionEx) {
    Status = RpHandle->Callback->PchRpSbiExecutionEx (
               RpDevice,
               GetRpSbiPid (RpIndex),
               Offset,
               PciConfigRead,
               FALSE,
               0xF,
               0,
               Fid,
               Data32,
               &Response
               );
    if (Status != EFI_SUCCESS) {
      DEBUG ((DEBUG_ERROR,"Sideband Read Failed of RpIndex %d Offset 0x%x. Device = %d Fid = 0x%x\n", RpIndex, Offset, RpDevice->Device, Fid));
    }
  } else {
    DEBUG ((DEBUG_ERROR, "PchSbiRpPciRead32() PchRpSbiExecutionEx not implemented\n"));
    Status = EFI_UNSUPPORTED;
  }
  return Status;
}

/**
  This function And then Or Pci Config register via SBI Access

  @param[in]  RpHandle            Pointer to PCIE RP handle
  @param[in]  Offset              Offset of Config register
  @param[in]  Data32And           Value of Config register to be And-ed
  @param[in]  Data32AOr           Value of Config register to be Or-ed

  @retval EFI_SUCCESS             SBI Read and Write successful.
**/
EFI_STATUS
PchSbiRpPciAndThenOr32 (
  IN  PCH_RP_HANDLE *RpHandle,
  IN  UINT32  Offset,
  IN  UINT32  Data32And,
  IN  UINT32  Data32Or
  )
{
  EFI_STATUS  Status;
  UINT32      Data32;
  UINT8       Response;
  UINT16      Fid;
  UINT32      RpIndex;
  PCH_RP_DEVICE *RpDevice;

  RpDevice = RpHandle->Device;
  RpIndex = RpDevice->RootPortIndex;

  Status = PchSbiRpPciRead32 (RpHandle, Offset, &Data32);
  if (Status == EFI_SUCCESS) {
    Data32 &= Data32And;
    Data32 |= Data32Or;
    Fid = (UINT16) ((RpDevice->Device << 3) | (RpIndex % 4));
    if (RpHandle->Callback->PchRpSbiExecutionEx) {
      Status = RpHandle->Callback->PchRpSbiExecutionEx (
                 RpDevice,
                 GetRpSbiPid (RpIndex),
                 Offset,
                 PciConfigWrite,
                 FALSE,
                 0xF,
                 0,
                 Fid,
                 &Data32,
                 &Response
                 );
      if (Status != EFI_SUCCESS) {
        DEBUG ((DEBUG_ERROR,"Sideband Write Failed of RpIndex %d Offset 0x%x. Device = %d Fid = 0x%x\n", RpIndex, Offset, RpDevice->Device, Fid));
      }
    } else {
      DEBUG ((DEBUG_ERROR, "PchSbiRpPciAndThenOr32() PchRpSbiExecutionEx not implemented\n"));
      Status = EFI_UNSUPPORTED;
    }
  }
  return Status;
}

/**
  This function And then Or Pci Memory register via SBI Access

  @param[in]  RpHandle            Pointer to PCIE RP handle
  @param[in]  Offset              Offset of Config register
  @param[in]  Data32And           Value of Config register to be And-ed
  @param[in]  Data32AOr           Value of Config register to be Or-ed

  @retval EFI_SUCCESS             SBI Read and Write successful.
**/
EFI_STATUS
PchSbiRpPciMemAndThenOr32 (
  IN  PCH_RP_HANDLE *RpHandle,
  IN  UINT64  Offset,
  IN  UINT32  Data32And,
  IN  UINT32  Data32Or
  )
{
  EFI_STATUS  Status;
  UINT32      Data32;
  UINT16      Fid;
  UINT8       Response;
  UINT32      RpIndex;
  PCH_RP_DEVICE *RpDevice;

  RpDevice = RpHandle->Device;
  RpIndex  = RpDevice->RootPortIndex;

  Data32   = 0;
  Fid      = 0;
  Response = 0;
  Fid = (UINT16) (RpDevice->Device << 3);

  if (RpHandle->Callback->PchRpSbiExecutionEx) {
    Status = RpHandle->Callback->PchRpSbiExecutionEx (
               RpDevice,
               GetRpSbiPid (RpIndex),
               Offset,
               MemoryRead,
               FALSE,
               0xF,
               0,
               Fid,
               &Data32,
               &Response
               );
    if (Status != EFI_SUCCESS) {
      DEBUG ((DEBUG_ERROR,
        "PchSbiRpPciMemAndThenOr32: Read Failed of RpIndex %d Offset 0x%x Device = %d Fid = 0x%x Status = %r \n",
        RpIndex, Offset, RpDevice->Device, Fid, Status
        ));
      return Status;
    }
  } else {
    DEBUG ((DEBUG_ERROR, "PchSbiRpPciMemAndThenOr32() PchRpSbiExecutionEx not implemented\n"));
    Status = EFI_UNSUPPORTED;
  }

  Data32 &= Data32And;
  Data32 |= Data32Or;

  if (RpHandle->Callback->PchRpSbiExecutionEx) {
    Status = RpHandle->Callback->PchRpSbiExecutionEx (
               RpDevice,
               GetRpSbiPid (RpIndex),
               Offset,
               MemoryWrite,
               FALSE,
               0xF,
               0,
               Fid,
               &Data32,
               &Response
               );
    if (Status != EFI_SUCCESS) {
      DEBUG ((DEBUG_ERROR,
        "PchSbiRpPciMemAndThenOr32: Write Failed of RpIndex %d Offset 0x%x Device = %d Fid = 0x%x Status = %r \n",
        RpIndex, Offset, RpDevice->Device, Fid,Status
        ));
    }
  } else {
    DEBUG ((DEBUG_ERROR, "PchSbiRpPciMemAndThenOr32() PchRpSbiExecutionEx not implemented\n"));
    Status = EFI_UNSUPPORTED;
  }

  return Status;
}

/**
  Checks if lane reversal is enabled on a given Root Port

  @param[in] RpHandle     Pointer to PCIE RP handle
  @param[in] RpCtrlHandle Pointer to handle for first PCIE RP in controller

  @retval TRUE if lane reversal is enabled, FALSE otherwise
**/
BOOLEAN
IsPcieLaneReversalEnabled (
  IN  PCH_RP_HANDLE *RpHandle,
  IN  PCH_RP_HANDLE *RpCtrlHandle
  )
{
  UINT32  Data32;

  PchSbiRpPciRead32 (RpCtrlHandle, R_PCH_PCIE_CFG_LTSSMSMSTS, &Data32);
  return !! (Data32 & B_PCH_PCIE_CFG_LTSSMSMSTS_LNKREVERSED);
}

/**
  Translate PCIe Port/Lane pair to 0-based PCIe lane number.

  @param[in] RpHandle     Pointer to PCIE RP handle
  @param[in] RpCtrlHandle Pointer to handle for first PCIE RP in controller
  @param[in] RpLane       Root Port Lane

  @retval PCIe lane number (0-based)
**/
UINT32
PchPciePhysicalLane (
  IN  PCH_RP_HANDLE *RpHandle,
  IN  PCH_RP_HANDLE *RpCtrlHandle,
  IN  UINT32        RpLane
  )
{
  UINT32  ControllerIndex;
  UINT32  ControllerLane;

  ControllerIndex = RpHandle->Device->CtrlIndex;
  ControllerLane  = (RpHandle->Device->RootPortIndex % PCH_PCIE_CONTROLLER_PORTS) * PCH_PCIE_LANES_PER_PORT + RpLane;
  ASSERT (ControllerLane < PCH_PCIE_LANES_PER_CONTROLLER);
  if (IsPcieLaneReversalEnabled (RpHandle, RpCtrlHandle)) {
    ControllerLane = (PCH_PCIE_LANES_PER_CONTROLLER - 1) - ControllerLane;
  }

  return ControllerIndex * PCH_PCIE_LANES_PER_CONTROLLER + ControllerLane;
}

/**
  Determines whether PCIe link is active

  @param[in] RpBase    Root Port base address
  @retval BOOLEAN      Link Active state
**/
BOOLEAN
PchPcieIsLinkActive (
  IN UINT64 RpBase
  )
{
  return !! (PciSegmentRead16 (RpBase + R_PCH_PCIE_CFG_LINKSTS) & B_PCIE_LSTS_LA);
}

/*
  Masks the elastic buffer error for given root port base.

  @param[in] RpBase       Root Port base address
*/
VOID
PchPcieMaskElasticBufferError (
  IN  UINT64 RpBase
  )
{
  PciSegmentOr32(RpBase + R_PCH_PCIE_CFG_EBERRMSK, B_PCH_PCIE_CFG_EBERRMSK_MASK);
}

/**
  Get Pch Maximum Pcie of PTM Pipe

  @retval Pch Maximum Pcie of PTM Pipe
**/
UINT8
GetPchMaxPciePtmNum (
  VOID
  )
{
  return 6;
}

/**
  Set RC default link equalization settings for a PCIe root port

  @param[out] PcieRpOneLinkPreMemConfig         Pointer to PCIe root port link equalization settings
**/
STATIC
VOID
PcieRpLinkEqPreMemSetConfigDefault (
  OUT ENTERPRISE_PCIE_LINK_EQ_CONFIG  *PcieRpOneLinkPreMemConfig
  )
{
  CONST ENTERPRISE_PCIE_LINK_EQ_FOMHINTTBL_ENTRY  PcieLinkEqEntries[] = {
    {EnterprisePcieEqFormatCoefficient,  6,  8},
    {EnterprisePcieEqFormatCoefficient,  8,  2},
    {EnterprisePcieEqFormatCoefficient, 10,  6},
    {EnterprisePcieEqFormatCoefficient, 12, 12},
    {EnterprisePcieEqFormatCoefficient, 14,  2},
    {EnterprisePcieEqFormatDisabled,     0,  0},
    {EnterprisePcieEqFormatDisabled,     0,  0},
    {EnterprisePcieEqFormatDisabled,     0,  0},
    {EnterprisePcieEqFormatDisabled,     0,  0},
    {EnterprisePcieEqFormatDisabled,     0,  0},
    {EnterprisePcieEqFormatDisabled,     0,  0},
    {EnterprisePcieEqFormatDisabled,     0,  0}
  };

  PcieRpOneLinkPreMemConfig->Ph1Settings.DownstreamPortTxPreset = 7;
  PcieRpOneLinkPreMemConfig->Ph1Settings.UpstreamPortTxPreset   = 5;
  PcieRpOneLinkPreMemConfig->Ph2Settings.LocalTxOverrideEnable  = FALSE;
  PcieRpOneLinkPreMemConfig->Ph2Settings.TxCoeffPreCursor       = 0;
  PcieRpOneLinkPreMemConfig->Ph2Settings.TxCoeffCursor          = 0;
  PcieRpOneLinkPreMemConfig->Ph2Settings.TxCoeffPostCursor      = 0;
  if (ARRAY_SIZE (PcieLinkEqEntries) <= ARRAY_SIZE (PcieRpOneLinkPreMemConfig->Ph3Settings.Ph3Entries)) {
      CopyMem (PcieRpOneLinkPreMemConfig->Ph3Settings.Ph3Entries, PcieLinkEqEntries, sizeof (PcieLinkEqEntries));
  }
}

/**
  Sets Retrain Link register of root port device.

  @param[in] RpBase     Root port base address
  @param[in] Set        Sets or clears bit
**/
VOID
PchPcieSetRetrainLink (
  IN UINT64  RpBase,
  IN BOOLEAN Set
  )
{
  if (Set) {
    PciSegmentOr16 (RpBase + R_PCH_PCIE_CFG_LINKCTL, B_PCIE_LCTL_RL);
  } else {
    PciSegmentAnd16 (RpBase + R_PCH_PCIE_CFG_LINKCTL, (UINT16) ~(B_PCIE_LCTL_RL));
  }
}

/**
  Sets Target Link Speed of root port device.

  @param[in] RpBase     Root port base address
  @param[in] PciSpeed   Target link speed: 1 - gen1, 2 - gen2, 3 - gen3
**/
VOID
PchPcieSetTargetLinkSpeed (
  IN UINT64  RpBase,
  IN UINT32  PciSpeed
  )
{
  switch (PciSpeed) {
    case 1:
      PciSpeed = V_PCH_PCIE_CFG_LINKCTL2_TLS_GEN1;
      break;
    case 2:
      PciSpeed = V_PCH_PCIE_CFG_LINKCTL2_TLS_GEN2;
      break;
    case 3:
      /* fallthrough */
    default:
      PciSpeed = V_PCH_PCIE_CFG_LINKCTL2_TLS_GEN3;
      break;
  }

  PciSegmentAndThenOr16 (
        RpBase + R_PCH_PCIE_CFG_LINKCTL2,
        (UINT16) ~B_PCIE_LCTL2_TLS,
        (UINT16) PciSpeed
        );
}

/**
  Sets ASPM capability of root port device.

  @param[in] RpDevice       Pointer to PCIE RP device
  @param[in] Aspm           Aspm modes supported
  @param[in] L1ExitLatency  Exit latency of L1
**/
VOID
PchPcieSetAspmCap (
  IN PCH_RP_DEVICE                *RpDevice,
  IN PCH_PCIE_ASPM_CONTROL        Aspm,
  IN PCH_PCIE_L1_EXIT_LATENCY     L1ExitLatency
  )
{
  UINT32 Data32And;
  UINT32 Data32Or;

  Data32And  = (UINT32) (~B_PCIE_LCAP_EL1);
  Data32And &= (UINT32) (~B_PCIE_LCAP_APMS);

  Data32Or = ((UINT32) L1ExitLatency << N_PCIE_LCAP_EL1) & B_PCIE_LCAP_EL1;

  ///
  /// Set LCAP APMS according to platform policy.
  ///
  if (Aspm < PchPcieAspmAutoConfig) {
    Data32Or |= Aspm << N_PCIE_LCAP_APMS;
  } else {
    Data32Or |= B_PCIE_LCAP_APMS_L0S | B_PCIE_LCAP_APMS_L1;
  }

  PciSegmentAndThenOr32 (
    RpDevice->PciCfgBaseAddr + R_PCH_PCIE_CFG_LINKCAP,
    Data32And,
    Data32Or
    );
}

/**
  Sets ASPM control of root port device.

  @param[in] RpBase         Root port base address
  @param[in] Aspm           Aspm modes enabled
**/
VOID
PchPcieSetAspmCtrl (
  IN UINT64                 RpBase,
  IN PCH_PCIE_ASPM_CONTROL  Aspm
  )
{
  UINT32 Data32Or;

  ///
  /// Set LCTL APMS according to platform policy.
  ///
  switch (Aspm) {
    case PchPcieAspmDisabled:
      Data32Or = 0;
      break;
    case PchPcieAspmL0s:
      Data32Or = V_PCIE_LCTL_ASPM_L0S;
      break;
    case PchPcieAspmL1:
      Data32Or = V_PCIE_LCTL_ASPM_L1;
      break;
    case PchPcieAspmL0sL1:
      /* fallthrough */
    case PchPcieAspmAutoConfig:
      /* fallthrough */
    case PchPcieAspmMax:
      /* fallthrough */
    default:
      Data32Or = V_PCIE_LCTL_ASPM_L0S_L1;
      break;
  }

  PciSegmentAndThenOr32 (
    RpBase + R_PCH_PCIE_CFG_LINKCTL,
    (UINT32) (~B_PCIE_LCTL_ASPM),
    Data32Or
    );
}

/**
  Sets ASPM Optionality Compliance bit of root port device.

  @param[in] RpBase         Root port base address
  @param[in] AspmOpComp     Set ASPM Optionality Compliance bit
**/
VOID
PchPcieSetAspmOpCmp (
  IN UINT64   RpBase,
  IN BOOLEAN  AspmOpComp
  )
{
  PciSegmentAndThenOr32 (
    RpBase + R_PCH_PCIE_CFG_LINKCAP,
    (UINT32) (~B_PCIE_LCAP_ASPMOC),
    AspmOpComp ? (UINT32) B_PCIE_LCAP_ASPMOC : 0
    );
}

/**
  This function assigns sub bus number to PCIe bus.

  @param[in] RpDevice    Pointer to PCIE RP device
  @param[in] TempPciBus  Temporary PCI bus number
  @retval UINT64         Assigned sub bus base address
**/
UINT64
AssignTemporaryBus (
  IN PCH_RP_DEVICE *RpDevice,
  IN UINT8  TempPciBus
  )
{
  UINT64 EpBase;
  //
  // Assign bus numbers to the root port
  //
  PciSegmentAndThenOr32 (
    RpDevice->PciCfgBaseAddr + PCI_BRIDGE_PRIMARY_BUS_REGISTER_OFFSET,
    (UINT32) ~B_PCI_BRIDGE_BNUM_SBBN_SCBN,
    ((UINT32) (TempPciBus << 8)) | ((UINT32) (TempPciBus << 16))
    );
  //
  // A config write is required in order for the device to re-capture the Bus number,
  // according to PCI Express Base Specification, 2.2.6.2
  // Write to a read-only register VendorID to not cause any side effects.
  //
  EpBase  = PCI_SEGMENT_LIB_ADDRESS (RpDevice->Segment, TempPciBus, 0, 0, 0);
  PciSegmentWrite16 (EpBase + PCI_VENDOR_ID_OFFSET, 0);

  return EpBase;
}

/**
  Clear temp bus usage.

  @param[in] RpBase     Root Port PCI base address
**/
VOID
ClearBus (
  IN UINT64 RpBase
  )
{
  PciSegmentAnd32 (
    RpBase + PCI_BRIDGE_PRIMARY_BUS_REGISTER_OFFSET,
    (UINT32) ~B_PCI_BRIDGE_BNUM_SBBN_SCBN
    );
}

/**
  Sets Relax Ordering control of endpoint device.

  @param[in] RpDevice            Pointer to PCIE RP device
  @param[in] TempPciBus          Temporary PCI bus number
  @param[in] RelaxedOrderEnable  Enable Relaxed Order
**/
VOID
PchPcieSetRelaxOrderingEndpoint (
  IN PCH_RP_DEVICE          *RpDevice,
  IN UINT8                  TempPciBus,
  IN BOOLEAN                RelaxedOrderEnable
  )
{
  UINT8 Data8;
  UINT64 EpBase;
  UINT8  EpPcieCapOffset;
  UINT64 RpBase;

  RpBase = RpDevice->PciCfgBaseAddr;

  AssignTemporaryBus (RpDevice, TempPciBus);
  EpBase = PCI_SEGMENT_LIB_ADDRESS (RpDevice->Segment, TempPciBus, 0, 0, 0);
  EpPcieCapOffset = PcieBaseFindCapId (EpBase, EFI_PCI_CAPABILITY_ID_PCIEXP);
  Data8 = PciSegmentRead8 (EpBase + EpPcieCapOffset + R_PCIE_DCTL_OFFSET);

  if (Data8 & B_PCIE_DCTL_RO) {
    if (RelaxedOrderEnable == TRUE) {
      //
      // According to PCIe base spec section 7.8.4 the default of Relaxed Order is 1 in endpoints
      // There is no need to set this value
      //
    } else {
      PciSegmentAnd8 (
        EpBase + EpPcieCapOffset + R_PCIE_DCTL_OFFSET,
        (UINT8) ~B_PCIE_DCTL_RO
      );
    }
  } else {
    DEBUG((DEBUG_WARN, "Relax Order is not supported on device connected to root port %d", RpBase));
  }

  ClearBus (RpBase);
}

/**
  Sets No Snoop control of endpoint device.

  @param[in] RpDevice            Pointer to PCIE RP device
  @param[in] TempPciBus          Temporary PCI bus number
  @param[in] NoSnoop             Enable No Snoop
**/
VOID
PchPcieSetNoSnoopEndpoint (
  IN PCH_RP_DEVICE          *RpDevice,
  IN UINT8                  TempPciBus,
  IN BOOLEAN                NoSnoop
  )
{
  UINT16 Data16;
  UINT64 EpBase;
  UINT8  EpPcieCapOffset;
  UINT64 RpBase;

  RpBase = RpDevice->PciCfgBaseAddr;

  AssignTemporaryBus (RpDevice, TempPciBus);
  EpBase = PCI_SEGMENT_LIB_ADDRESS (RpDevice->Segment, TempPciBus, 0, 0, 0);
  EpPcieCapOffset = PcieBaseFindCapId (EpBase, EFI_PCI_CAPABILITY_ID_PCIEXP);
  Data16 = PciSegmentRead16 (EpBase + EpPcieCapOffset + R_PCIE_DCTL_OFFSET);

  if (Data16 & B_PCIE_DCTL_NS) {
    if (NoSnoop == TRUE) {
      //
      // According to PCIe base spec section 7.8.4 the default of No Snoop is 1 in endpoints
      // There is no need to set this value
      //
    } else {
      PciSegmentAnd16 (
        EpBase + EpPcieCapOffset + R_PCIE_DCTL_OFFSET,
        (UINT16) ~B_PCIE_DCTL_NS
      );
    }
  } else {
    DEBUG((DEBUG_WARN, "No Snoop is not supported on device connected to root port %d", RpBase));
  }

  ClearBus (RpBase);
}

/**
  Sets ASPM control of endpoint device.

  @param[in] RpDevice       Pointer to PCIE RP device
  @param[in] TempPciBus     Temporary PCI bus number
  @param[in] Aspm           Aspm modes enabled
**/
VOID
PchPcieSetAspmEndpoint (
  IN PCH_RP_DEVICE          *RpDevice,
  IN UINT8                  TempPciBus,
  IN PCH_PCIE_ASPM_CONTROL  Aspm
  )
{
  UINT32 Data32;
  UINT64 EpBase;
  UINT8  EpPcieCapOffset;

  AssignTemporaryBus (RpDevice, TempPciBus);
  EpBase = PCI_SEGMENT_LIB_ADDRESS (RpDevice->Segment, TempPciBus, 0, 0, 0);
  EpPcieCapOffset = PcieBaseFindCapId (EpBase, EFI_PCI_CAPABILITY_ID_PCIEXP);

  Data32 = PciSegmentRead32 (EpBase + EpPcieCapOffset + R_PCIE_LCAP_OFFSET);

  if ((Data32 & B_PCIE_LCAP_APMS_L1) && Aspm == PchPcieAspmL1) {
    PciSegmentAndThenOr32 (
    EpBase + EpPcieCapOffset + R_PCIE_LCTL_OFFSET,
    (UINT32) (~B_PCIE_LCTL_ASPM),
    V_PCIE_LCTL_ASPM_L1
    );
  }
  ClearBus (RpDevice->PciCfgBaseAddr);
}

/**
  This function sets Common Clock Mode bit in rootport and endpoint connected to it, if both sides support it.
  This bit influences rootport's Gen3 training and should be set before Gen3 software equalization is attempted.
  It does not attempt to set CCC in further links behind rootport

  @param[in] RpDevice    Pointer to PCIE RP device
**/
VOID
EnableCommonClock (
  IN PCH_RP_DEVICE *RpDevice,
  IN UINT8  TempPciBus
  )
{
  UINT64 RpBase;
  UINT64 EpBase;
  UINT8  Func;
  UINT8  EpPcieCapOffset;
  RpBase = RpDevice->PciCfgBaseAddr;
  EpBase = AssignTemporaryBus (RpDevice, TempPciBus);
  EpPcieCapOffset = PcieBaseFindCapId (EpBase, EFI_PCI_CAPABILITY_ID_PCIEXP);
  if (GetScc (RpBase, R_PCH_PCIE_CFG_EXPCAPLST) && (EpPcieCapOffset != 0) && GetScc (EpBase, EpPcieCapOffset)) {
    EnableCcc (RpBase, R_PCH_PCIE_CFG_EXPCAPLST);
    EnableCcc (EpBase, EpPcieCapOffset);
    if (IsMultifunctionDevice (EpBase)) {
      for (Func = 1; Func <= PCI_MAX_FUNC; Func++) {
        EpBase = PCI_SEGMENT_LIB_ADDRESS (RpDevice->Segment, TempPciBus, 0, Func, 0);
        EnableCcc (EpBase, PcieBaseFindCapId (EpBase, EFI_PCI_CAPABILITY_ID_PCIEXP));
      }
    }
    RetrainLink (RpBase, R_PCH_PCIE_CFG_EXPCAPLST, TRUE);
  }
  ClearBus (RpBase);
}

/**
  This function determines whether root port is configured in non-common clock mode.
  Result is based on the NCC soft-strap setting.

  @param[in] RpBase      Root Port pci segment base address

  @retval TRUE           Port in NCC SSC mode.
  @retval FALSE          Port not in NCC SSC mode.
**/
BOOLEAN
IsPcieNcc (
  IN     UINT64  RpBase
  )
{
  if (PciSegmentRead16 (RpBase + R_PCH_PCIE_CFG_LINKSTS) & B_PCIE_LSTS_SCC) {
    return FALSE;
  } else {
    return TRUE;
  }
}

/**
  This function checks if de-emphasis needs to be changed from default for a given rootport

  @param[in] DevInfo      Information on device that is connected to rootport

  @retval TRUE            De-emphasis needs to be changed
  @retval FALSE           No need to change de-emphasis
**/
BOOLEAN
NeedDecreasedDeEmphasis (
  IN PCIE_DEVICE_INFO      DevInfo
  )
{
  //
  // Intel WiGig devices
  //
  if (DevInfo.Vid == V_PCH_INTEL_VENDOR_ID && DevInfo.Did == 0x093C) {
    return TRUE;
  }
  return FALSE;
}

/**
  Detect whether CLKREQ# is supported by the platform and device.

  Assumes device presence. Device will pull CLKREQ# low until CPM is enabled.
  Test pad state to verify that the signal is correctly connected.
  This function will change pad mode to GPIO!

  @param[in] RootPortConfig      Root port configuration
  @param[in] DevicePresent       Determines whether there is a device on the port

  @retval TRUE if supported, FALSE otherwise.
**/
BOOLEAN
PcieDetectClkreq (
  IN       UINT32          RpIndex,
  IN CONST PCH_PCIE_CONFIG *PcieConfig
  )
{
  if (!IsClkReqAssigned (PchClockUsagePchPcie0 + RpIndex)) {
    return FALSE;
  }
  if (PcieConfig->RootPort[RpIndex].PcieRpCommonConfig.ClkReqDetect &&
      EFI_ERROR (CheckClkReq (PchClockUsagePchPcie0 + RpIndex))) {
    DEBUG ((DEBUG_INFO, "CLKREQ is not Low, disabling power management for RP %d.\n", RpIndex));
    return FALSE;
  }
  return TRUE;
}

/**
  Get max link width.

  @param[in] RpBase    Root Port base address
  @retval Max link width
**/
UINT8
GetMaxLinkWidth (
  UINT64  RpBase
  )
{
  UINT8  LinkWidth;
  LinkWidth = (UINT8) ((PciSegmentRead32 (RpBase + R_PCH_PCIE_CFG_LINKCAP) & B_PCIE_LCAP_MLW) >> N_PCIE_LCAP_MLW);
  ASSERT (LinkWidth <= PCH_PCIE_LANES_PER_CONTROLLER);
  if (LinkWidth > PCH_PCIE_LANES_PER_CONTROLLER) {
    LinkWidth = PCH_PCIE_LANES_PER_CONTROLLER;
  }
  return LinkWidth;
}

/**
  Verify whether the PCIe port does own all lanes according to the port width.

  @param[in] RpHandle           Pointer to PCIE RP handle
  @param[in] RpCtrlHandle       Pointer to handle for first PCIE RP in controller

  @retval TRUE   All PCIe lanes are connected to PHY
  @retval FALSE  At least one lane is not connected to PHY
**/
BOOLEAN
IsPcieRootPortWired (
  IN  PCH_RP_HANDLE *RpHandle,
  IN  PCH_RP_HANDLE *RpCtrlHandle
  )
{
  UINT32     MaxLinkWidth;
  UINT32     RpLaneIndex;
  UINT32     PhysLane;

  MaxLinkWidth = GetMaxLinkWidth (RpHandle->Device->PciCfgBaseAddr);
  for (RpLaneIndex = 0; RpLaneIndex < MaxLinkWidth; ++RpLaneIndex) {
    if (RpHandle->Callback->PchRpIsPcieRootPortLaneConnected) {
      PhysLane = PchPciePhysicalLane (RpHandle, RpCtrlHandle, RpLaneIndex);
      if (!RpHandle->Callback->PchRpIsPcieRootPortLaneConnected (RpHandle->Device, PhysLane)) {
        return FALSE;
      }
    } else {
      DEBUG ((DEBUG_ERROR, "%a Not implemented PchRpIsPcieRootPortLaneConnected\n", __FUNCTION__));
      return FALSE;
    }
  }
  return TRUE;
}

/**
  Check if Root Port Link is in active state

  @param[in] RpHandle           Pointer to PCIE RP handle

  @retval    TRUE               Link is in Active state
  @retval    FALSE              Link is not in Active state
**/
STATIC
BOOLEAN
IsRpLinkActive (
  IN PCH_RP_HANDLE *RpHandle
  )
{
  UINT32      Data32;
  EFI_STATUS  Status;

  Data32 = 0;

  Status = PchSbiRpPciRead32 (RpHandle, (R_PCH_PCIE_CFG_LINKSTS-2), &Data32);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "RootPort %d is not present %d\n", RpHandle->Device->RootPortIndex));
    return FALSE;
  }
  return (BOOLEAN) (Data32 >> 16) & B_PCIE_LSTS_LA;
}

/**
  Disables the root port. Use sideband access unless the port is still available.

  @param[in] RpHandle           Pointer to PCIE RP handle
**/
VOID
DisablePcieRootPort (
  IN PCH_RP_HANDLE *RpHandle
  )
{
  PCH_SBI_PID Pid;
  PCH_RP_DEVICE *RpDevice;
  UINT32        RpIndex;

  DEBUG ((DEBUG_INFO, "%a (%d) Start\n", __FUNCTION__, RpHandle->Device->RootPortIndex));

  RpDevice = RpHandle->Device;
  RpIndex = RpDevice->RootPortIndex;

  if (IsRpLinkActive (RpHandle)) {
    DEBUG ((DEBUG_INFO, "Disabling the link.\n"));
    PchSbiRpPciAndThenOr32 (RpHandle, R_PCH_PCIE_CFG_LINKCTL, ~0u, B_PCIE_LCTL_LD);
  }

  ///
  /// Make port disappear from PCI bus
  ///
  Pid = PchGetPcieControllerSbiPid (RpIndex / PCH_PCIE_CONTROLLER_PORTS);
  DEBUG ((DEBUG_INFO, "Hiding the port: %d (0x%x)\n", RpIndex, Pid));
  if (RpHandle->Callback->PchRpPcrAndThenOr16) {
    RpHandle->Callback->PchRpPcrAndThenOr16 (
      RpDevice,
      Pid,
      R_PCH_PCIE_MSG_DEVHIDE,
      0x1FF,
      (UINT16) (((B_PCH_PCIE_MSG_DEVHIDE_P0CFGH |
        B_PCH_PCIE_MSG_DEVHIDE_P0GBE) <<
        (RpIndex % PCH_PCIE_CONTROLLER_PORTS)) |
        B_PCH_PCIE_MSG_DEVHIDE_PCFGHP)
    );
  } else {
    DEBUG ((DEBUG_ERROR, "%a Not implemented PchRpPcrAndThenOr16\n", __FUNCTION__));
  }

  if (RpHandle->Callback->PchRpPsfDisablePcieRootPort) {
    RpHandle->Callback->PchRpPsfDisablePcieRootPort (RpDevice);
  } else {
    DEBUG ((DEBUG_ERROR, "%a Not implemented PchRpPsfDisablePcieRootPort\n", __FUNCTION__));
  }
  if (RpHandle->Callback->PchRpPmcDisablePcieRootPort) {
    RpHandle->Callback->PchRpPmcDisablePcieRootPort (RpDevice);
  } else {
    DEBUG ((DEBUG_ERROR, "%a Not implemented PchRpPmcDisablePcieRootPort\n", __FUNCTION__));
  }

  DEBUG ((DEBUG_INFO, "%a () End\n", __FUNCTION__));
}


/**
  The function to change the root port speed based on policy

  @param[in] PcieConfig           Root port configuration

  @retval EFI_SUCCESS             Succeeds.
**/
EFI_STATUS
PcieRpSpeedChange (
  IN CONST PCH_PCIE_CONFIG *PcieConfig
  )
{
  UINTN                 PortIndex;
  UINTN                 PchMaxPciePortNum;
  UINT64                PciRootPortRegBase[PCH_MAX_PCIE_ROOT_PORTS];
  UINTN                 LinkRetrainedBitmap;
  UINTN                 TimeoutCount;
  UINT32                MaxLinkSpeed;
  UINT32                TargetLinkSpeed = 1;

  PchMaxPciePortNum = GetPchMaxPciePortNum ();

  //
  // Since we are using the root port base many times, it is best to cache them.
  //
  for (PortIndex = 0; PortIndex < PchMaxPciePortNum; PortIndex++) {
    PciRootPortRegBase[PortIndex] = PchPcieRpPciCfgBase (PortIndex);
  }

  LinkRetrainedBitmap = 0;

  for (PortIndex = 0; PortIndex < PchMaxPciePortNum; PortIndex++) {

    if ((PciRootPortRegBase[PortIndex] == 0) |
        (PciSegmentRead16 (PciRootPortRegBase[PortIndex] + PCI_VENDOR_ID_OFFSET) == 0xFFFF)) {
      continue;
    }

    MaxLinkSpeed = GetMaxLinkSpeed (PciRootPortRegBase[PortIndex]);

    switch (PcieConfig->RootPort[PortIndex].PcieRpCommonConfig.PcieSpeed) {
    case PcieGen1:
      TargetLinkSpeed = 1;
      break;
    case PcieGen2:
      TargetLinkSpeed = 2;
      break;
    case PcieGen3:
    case PcieAuto:
      TargetLinkSpeed = 3;
      break;
    default:
      DEBUG ((DEBUG_ERROR, "Unsupported PCIe Speed!\n"));
      ASSERT (FALSE);
      break;
    }

    TargetLinkSpeed = TargetLinkSpeed > MaxLinkSpeed ? MaxLinkSpeed : TargetLinkSpeed;

    PchPcieSetTargetLinkSpeed (PciRootPortRegBase[PortIndex], TargetLinkSpeed);

    if (PchPcieIsLinkActive (PciRootPortRegBase[PortIndex])) {
      PchPcieSetRetrainLink (PciRootPortRegBase[PortIndex], TRUE);
      LinkRetrainedBitmap |= (1u << PortIndex);
    }
  }

  //
  // 15 ms timeout while checking for link active on retrained link
  //
  for (TimeoutCount = 0; ((LinkRetrainedBitmap != 0) && (TimeoutCount < 150)); TimeoutCount++) {
    MicroSecondDelay (100);
    //
    // Check for remaining root port which was link retrained
    //
    for (PortIndex = 0; PortIndex < PchMaxPciePortNum; PortIndex++) {
      if ((LinkRetrainedBitmap & (1u << PortIndex)) != 0) {
        if (PchPcieIsLinkActive (PciRootPortRegBase[PortIndex])) {
          LinkRetrainedBitmap &= ~(1u << PortIndex);
        }
      }
    }
  }

  //
  // If 15 ms has timeout, and some link are not active, train to gen1
  //
  if (LinkRetrainedBitmap != 0) {
    for (PortIndex = 0; PortIndex < PchMaxPciePortNum; PortIndex++) {
      if ((LinkRetrainedBitmap & (1u << PortIndex)) != 0) {
        PchPcieSetTargetLinkSpeed (PciRootPortRegBase[PortIndex], 1);
        PchPcieSetRetrainLink (PciRootPortRegBase[PortIndex], TRUE);
      }
    }

    //
    // Wait for retrain completion or timeout in 15ms. Do not expect failure as
    // port was detected and trained as Gen1 earlier
    //
    for (TimeoutCount = 0; ((LinkRetrainedBitmap != 0) && (TimeoutCount < 150)); TimeoutCount++) {
      MicroSecondDelay (100);
      //
      // Check for remaining root port which was link retrained
      //
      for (PortIndex = 0; PortIndex < PchMaxPciePortNum; PortIndex++) {
        if ((LinkRetrainedBitmap & (1u << PortIndex)) != 0) {
          if (PchPcieIsLinkActive (PciRootPortRegBase[PortIndex])) {
            LinkRetrainedBitmap &= ~(1u << PortIndex);
          }
        }
      }
    }
  }

  return EFI_SUCCESS;
}


/*
  Checks if given rootport has an endpoint connected

  @param[in] DeviceBase       PCI segment base address of root port

  @retval                     TRUE if endpoint is connected
  @retval                     FALSE if no endpoint was detected
*/
BOOLEAN
IsEndpointConnected (
  UINT64 DeviceBase
  )
{
  return !!(PciSegmentRead16 (DeviceBase + R_PCH_PCIE_CFG_SLOTSTS) & B_PCIE_SLSTS_PDS);
}


/*
  Some PCIe devices may take long time between they become detected and form a link.
  This function waits until all enabled, non-empty rootports form a link or until a timeout

  @param[in] MaxRootPorts      number of rootports
  @param[in] DisabledPortMask  mask of rootprots that don't need to be considered
*/
VOID
WaitForLinkActive (
  UINT32 MaxRootPorts,
  UINT32 DisabledPortMask
  )
{
  UINT32 PortMask;
  UINT32 Index;
  UINT32 Time;
  UINT64 RpBase;

  Time = 0;
  //
  // Set a bit in PortMask for each rootport that exists and isn't going to be disabled
  //
  PortMask = (0x1 << MaxRootPorts) - 1;
  PortMask &= ~DisabledPortMask;

  DEBUG ((DEBUG_INFO, "WaitForLinkActive, RP mask to wait for = 0x%08x\n", PortMask));
  while (Time < LINK_ACTIVE_POLL_TIMEOUT) {
    for (Index = 0; Index < MaxRootPorts; Index ++) {
      if (!(PortMask & (BIT0 << Index))) {
        continue;
      }
      RpBase = PchPcieRpPciCfgBase (Index);
      //
      // if PDS is not set or if LA is set then this rootport is done - clear it from mask
      //
      if (!IsEndpointConnected (RpBase) || PchPcieIsLinkActive (RpBase)) {
        PortMask &= ~ (BIT0 << Index);
      }
    }
    if (PortMask == 0x0) {
      DEBUG ((DEBUG_INFO, "WaitForLinkActive, all RPs done, lost %dms waiting\n", Time/1000));
      return;
    }
    MicroSecondDelay (LINK_ACTIVE_POLL_INTERVAL);
    Time += LINK_ACTIVE_POLL_INTERVAL;
  }

  DEBUG ((DEBUG_WARN, "WaitForLinkActive, timeout with the following RPs still not done: 0x%08x\n", PortMask));
}


/**
  Get information about the endpoint

  @param[in]  RpHandle    Pointer to PCIE RP handle
  @param[in]  TempPciBus  Temporary bus number
  @param[out] DeviceInfo  Device information structure

  @raturn TRUE if device was found, FALSE otherwise
**/
BOOLEAN
GetDeviceInfo (
  IN  PCH_RP_DEVICE     *RpDevice,
  IN  UINT8             TempPciBus,
  OUT PCIE_DEVICE_INFO  *DeviceInfo
  )
{
  UINT64                  EpBase;
  UINT32                  Data32;
  UINT8                   EpPcieCapPtr;
  UINT8                   EpLinkSpeed;

  DeviceInfo->Vid = 0xFFFF;
  DeviceInfo->Did = 0xFFFF;
  DeviceInfo->MaxLinkSpeed = 0;

  //
  // Check for device presence
  //
  if (!IsEndpointConnected (RpDevice->PciCfgBaseAddr)) {
    return FALSE;
  }

  EpBase = AssignTemporaryBus (RpDevice, TempPciBus);

  Data32 = PciSegmentRead32 (EpBase + PCI_VENDOR_ID_OFFSET);
  DeviceInfo->Vid = (UINT16) (Data32 & 0xFFFF);
  DeviceInfo->Did = (UINT16) (Data32 >> 16);

  EpLinkSpeed = 0;
  EpPcieCapPtr = PcieFindCapId (RpDevice->Segment, TempPciBus, 0, 0, EFI_PCI_CAPABILITY_ID_PCIEXP);
  if (EpPcieCapPtr != 0) {
    EpLinkSpeed = PciSegmentRead8 (EpBase + EpPcieCapPtr + R_PCIE_LCAP_OFFSET) & B_PCIE_LCAP_MLS;
  }
  DeviceInfo->MaxLinkSpeed = EpLinkSpeed;

  //
  // Clear bus numbers
  //
  ClearBus (RpDevice->PciCfgBaseAddr);

  DEBUG ((DEBUG_INFO, "VID: %04X DID: %04X  MLS: %d\n",
          DeviceInfo->Vid, DeviceInfo->Did, DeviceInfo->MaxLinkSpeed));

  return (Data32 != 0xFFFFFFFF);
}

/**
  Enable PCI Express Clock Gating for Root Ports and Virtual Root Ports.
  This settings are relevant to all ports including disabled ports.
  Use sideband access since primary may not be available.

  @param[in] RpHandle           Pointer to PCIE RP handle
**/
VOID
PchPcieEnableRootPortClockGating (
  IN  PCH_RP_HANDLE *RpHandle
  )
{
  DEBUG ((DEBUG_INFO, "PchPcieEnableRootPortClockGating(%d) Start\n", RpHandle->Device->RootPortIndex));

  PchSbiRpPciAndThenOr32 (RpHandle, R_PCH_PCIE_CFG_IOSFDEVCLKGCTL, ~0u, (UINT32) (B_PCH_PCIE_CFG_SBDEVCLKGCTL_ICGE << 16) | B_PCH_PCIE_CFG_IOSFDEVCLKGCTL_ICGE);
  PchSbiRpPciAndThenOr32 (RpHandle, R_PCH_PCIE_CFG_PT0TXNCLKGCTL , ~0u, (UINT32) (B_PCH_PCIE_CFG_PT1TXNCLKGCTL_ICGE << 16) | B_PCH_PCIE_CFG_PT0TXNCLKGCTL_ICGE);
  PchSbiRpPciAndThenOr32 (RpHandle, R_PCH_PCIE_CFG_PT2TXNCLKGCTL , ~0u, (UINT32) (B_PCH_PCIE_CFG_PT3TXNCLKGCTL_ICGE << 16) | B_PCH_PCIE_CFG_PT2TXNCLKGCTL_ICGE);
  PchSbiRpPciAndThenOr32 (RpHandle, R_PCH_PCIE_CFG_PT0LNKCLKGCTL , ~0u, (UINT32) (B_PCH_PCIE_CFG_PT1LNKCLKGCTL_ICGE << 16) | B_PCH_PCIE_CFG_PT0LNKCLKGCTL_ICGE);
  PchSbiRpPciAndThenOr32 (RpHandle, R_PCH_PCIE_CFG_PT2LNKCLKGCTL , ~0u, (UINT32) (B_PCH_PCIE_CFG_PT3LNKCLKGCTL_ICGE << 16) | B_PCH_PCIE_CFG_PT2LNKCLKGCTL_ICGE);
  PchSbiRpPciAndThenOr32 (RpHandle, R_PCH_PCIE_CFG_CFGPCIECLKGCTL, ~0u, B_PCH_PCIE_CFG_CFGPCIECLKGCTL_ICGE);
  PchSbiRpPciAndThenOr32 (RpHandle, R_PCH_PCIE_CFG_PT0PHYCLKGCTL , ~0u, (UINT32) (B_PCH_PCIE_CFG_PT1PHYCLKGCTL_ICGE << 16) | B_PCH_PCIE_CFG_PT0PHYCLKGCTL_ICGE);
  PchSbiRpPciAndThenOr32 (RpHandle, R_PCH_PCIE_CFG_PT2PHYCLKGCTL , ~0u, (UINT32) (B_PCH_PCIE_CFG_PT3PHYCLKGCTL_ICGE << 16) | B_PCH_PCIE_CFG_PT2PHYCLKGCTL_ICGE);
  PchSbiRpPciAndThenOr32 (RpHandle, R_PCH_PCIE_CFG_PLLSTBYCTL    , ~0u, (UINT32) (B_PCH_PCIE_CFG_PLLSTBYCTL_PCLK_TRUNK_CGE) | B_PCH_PCIE_CFG_PLLSTBYCTL_PSE);
}

/**
  Check for device presence with timeout.

  @param[in]     RpBase      Root Port base address
  @param[in]     TimeoutUs   Timeout in microseconds
  @param[in,out] Timer       Timer value, must be initialized to zero
                             before the first call of this function.
**/
BOOLEAN
IsPcieDevicePresent (
  IN     UINT64  RpBase,
  IN     UINT32  TimeoutUs,
  IN OUT UINT32  *Timer
  )
{
  while (TRUE) {
    if (IsEndpointConnected (RpBase)) {
      return TRUE;
    }
    if (*Timer < TimeoutUs) {
      MicroSecondDelay (10);
      *Timer += 10;
    } else {
      break;
    }
  }
  return FALSE;
}

/**
  Initializes hot plug support in enterprise root port.

  @param[in] RpHandle           Pointer to PCIE RP handle
**/
VOID
PcieInitHotPlugSupport (
  IN  PCH_RP_HANDLE *RpHandle
  )
{
  PCH_PCIE_ROOT_PORT_CONFIG     *RpConfig;
  PCIE_ROOT_PORT_COMMON_CONFIG  *RpCommonConfig;
  PCH_PCIE_CONFIG               *PchPcieConfig;
  PCH_RP_DEVICE                 *RpDevice;
  UINT32                        Data32Or;
  UINT8                         VppnField;
  UINT64                        RpBase;

  RpDevice = RpHandle->Device;
  PchPcieConfig = RpHandle->PciExpressConfig;
  RpConfig = &PchPcieConfig->RootPort[RpDevice->RootPortIndex];
  RpCommonConfig = &RpConfig->PcieRpCommonConfig;
  RpBase = RpDevice->PciCfgBaseAddr;

  if (!RpCommonConfig->HotPlug) {
    return;
  }

  if (!RpCommonConfig->SlotImplemented) {
    DEBUG ((DEBUG_ERROR, "Hot plug support requested but slot not implemented, hot plug init aborted RP=%d\n", RpDevice->RootPortIndex));
    return;
  }

  //
  // Configure the virtual pin port communication
  // for the root port. This will tell the root port
  // which virtual port pin will be used for hot plug
  // event detection
  //
  if (RpConfig->VppPort != 0xFF) {
    Data32Or = B_PCH_PCIE_CFG_VPPCSR_VPPE;
    VppnField = (RpConfig->VppAddress & (BIT0 | BIT1 | BIT2)) << 1 | (RpConfig->VppPort & BIT0);
    Data32Or |= ((UINT32) VppnField) << N_PCH_PCIE_CFG_VPPCSR_VPPN;

    //
    // For form factors that use EMILS we have to inform the root
    // port about different method of hot plug detection event.
    // We also have to set input inversion since SHPC will not
    // invert the ELS pin value to match the PCIe specification.
    //
    if (RpCommonConfig->FormFactor == PcieFormFactorExpressModule) {
      Data32Or |= (B_PCH_PCIE_CFG_VPPCSR_HPFF | B_PCH_PCIE_CFG_VPPCSR_IOE);
    }

    PciSegmentOr32(RpBase + R_PCH_PCIE_CFG_VPPCSR, Data32Or);
  }

  //
  // Set suprise hot plug support bit in SLOTCAP register.
  // In addition we also enable support for power and attention indicator,
  // power controller and attention button. It is assumed that platforms
  // that enable hot plug have support for those components.
  //
  Data32Or = (B_PCIE_SLCAP_HPC | B_PCIE_SLCAP_HPS | B_PCIE_SLCAP_PIP | B_PCIE_SLCAP_AIP | B_PCIE_SLCAP_PCP | B_PCIE_SLCAP_ABP);

  if (RpCommonConfig->MrlSensorPresent) {
    Data32Or |= B_PCIE_SLCAP_MRLSP;
  }

  PciSegmentOr32 (RpBase + R_PCH_PCIE_CFG_SLOTCAP, Data32Or);

  //
  // Clear the PDC status bit
  //
  PciSegmentAnd16 (RpBase + R_PCH_PCIE_CFG_SLOTSTS, B_PCIE_SLSTS_PDC);

  //
  // Disable hot plug interrupt generation and enable interrupt on presence detect change.
  //
  PciSegmentAndThenOr16 (RpBase + R_PCH_PCIE_CFG_SLOTCTL, (UINT16) (~B_PCIE_SLCTL_HPE), B_PCIE_SLCTL_PDE);

  //
  // Disable SMI on hot plug and enable SCI on hot plug
  //
  PciSegmentAnd8 (RpBase + R_PCH_PCIE_CFG_SMICSR, (UINT8)(~B_PCH_PCIE_CFG_SMICSR_HPSMIE) );
  PciSegmentOr8 (RpBase + R_PCH_PCIE_CFG_SCICSR, B_PCH_PCIE_CFG_SCICSR_HPSCIE);

  //
  // Mask the completion timeout errors
  //
  PciSegmentOr32 (RpBase + R_PCH_PCIE_CFG_ERRUNCMSK, B_PCIE_EX_UEM_CT);
}

/**
  Performs mandatory Root Port Initialization.
  This function is silicon-specific and configures proprietary registers.

  @param[in]  RpHandle                Pointer to PCIE RP handle
**/
STATIC
VOID
InitPcieSingleRootPort (
  IN  PCH_RP_HANDLE                             *RpHandle
  )
{
  UINT64                            RpBase;
  UINT32                            Data32Or;
  UINT32                            Data32And;
  UINT16                            Data16;
  UINT16                            Data16Or;
  UINT16                            Data16And;
  PCH_PCIE_CONFIG                   *PchPcieConfig;
  CONST PCIE_ROOT_PORT_COMMON_CONFIG *RootPortCommonConfig;
  PCIE_DEVICE_INFO                  DevInfo;
  UINT32                            PortIndex;

  PchPcieConfig = RpHandle->PciExpressConfig;
  PortIndex = RpHandle->Device->RootPortIndex;

  RootPortCommonConfig = &PchPcieConfig->RootPort[PortIndex].PcieRpCommonConfig;

  DEBUG ((DEBUG_INFO, "InitPcieSingleRootPort  (%d) Start \n", PortIndex));
  RpBase = RpHandle->Device->PciCfgBaseAddr;

  GetDeviceInfo (RpHandle->Device, RpHandle->Device->TempPciBusMin, &DevInfo);

  if (NeedDecreasedDeEmphasis (DevInfo)) {
    PciSegmentOr32 (RpBase + R_PCH_PCIE_CFG_LINKCTL2, B_PCIE_LCTL2_SD);
  }

  PchPcieMaskElasticBufferError (RpBase);

  ///
  /// PCH BIOS Spec Section 8.15.1 Power Optimizer Configuration
  /// If B0:Dxx:Fn + 400h is programmed, BIOS will also program B0:Dxx:Fn + 404h [1:0] = 11b,
  /// to enable these override values.
  /// - Fn refers to the function number of the root port that has a device attached to it.
  /// - Default override value for B0:Dxx:Fn + 400h should be 880F880Fh
  /// - Also set 404h[2] to lock down the configuration
  /// - Refer to table below for the 404h[3] policy bit behavior.
  /// Done in PcieSetPm()
  ///
  /// PCH BIOS Spec Section 8.15.1 Power Optimizer Configuration
  /// Program B0:Dxx:Fn + 64h [11] = 1b
  ///
  Data32Or = 0;
  Data32And = ~0u;
  if (RootPortCommonConfig->LtrEnable == TRUE) {
    Data32Or |= B_PCIE_DCAP2_LTRMS;
  } else {
    Data32And &= (UINT32) ~(B_PCIE_DCAP2_LTRMS);
  }
  ///
  /// PCH BIOS Spec Section 8.15.1 Power Optimizer Configuration
  /// Optimized Buffer Flush/Fill (OBFF) is not supported.
  /// Program B0:Dxx:Fn + 64h [19:18] = 0h
  ///
  Data32And &= (UINT32) ~B_PCIE_DCAP2_OBFFS;
  PciSegmentAndThenOr32 (RpBase + R_PCH_PCIE_CFG_DEVCAP2, Data32And, Data32Or);
  ///
  /// PCH BIOS Spec Section 8.15.1 Power Optimizer Configuration
  /// Program B0:Dxx:Fn + 68h [10] = 1b
  ///
  Data16 = PciSegmentRead16 (RpBase + R_PCH_PCIE_CFG_DEVCTL2);
  if (RootPortCommonConfig->LtrEnable == TRUE) {
    Data16 |= B_PCIE_DCTL2_LTREN;
  } else {
    Data16 &= (UINT16) ~(B_PCIE_DCTL2_LTREN);
  }
  PciSegmentWrite16 (RpBase + R_PCH_PCIE_CFG_DEVCTL2, Data16);

  //
  // Program Max Payload Size
  //
  PciSegmentAndThenOr8 (RpBase + R_PCH_PCIE_CFG_DEVCAP, (UINT8) ~B_PCH_PCIE_CFG_DEVCAP_MPSS, (UINT8) (B_PCH_PCIE_CFG_DEVCAP_MPSS & RootPortCommonConfig->MaxPayload));

  PchPcieSetAspmCap (RpHandle->Device, RootPortCommonConfig->Aspm, L1_EXIT_LATENCY);
  PchPcieSetAspmCtrl (RpBase, RootPortCommonConfig->Aspm);
  PchPcieSetAspmEndpoint (RpHandle->Device, RpHandle->Device->TempPciBusMin, RootPortCommonConfig->Aspm);
  PchPcieSetRelaxOrderingEndpoint (RpHandle->Device, RpHandle->Device->TempPciBusMin, (BOOLEAN) RootPortCommonConfig->RelaxedOrder);
  PchPcieSetNoSnoopEndpoint (RpHandle->Device, RpHandle->Device->TempPciBusMin, (BOOLEAN) RootPortCommonConfig->NoSnoop);


  // TODO: needed for CDF?
  ///
  /// PCH BIOS Spec Section 8.15 Additional PCI Express* Programming Steps
  /// Enable PME_TO Time-Out Policy, Dxx:Fn + E2h[6] =1b
  ///
  PciSegmentOr8 (RpBase + R_PCH_PCIE_CFG_RPPMCSR, B_PCH_PCIE_CFG_RPPMCSR_PME_TO_ACK_TOP);

  ///
  /// PCH BIOS Spec Section 8.15 Additional PCI Express* Programming Steps
  /// If there is no IOAPIC behind the root port, set EOI Forwarding Disable bit (PCIE RP PCI offset D4h[1]) to 1b.
  /// For Empty Hot Plug Slot, set is done in InitPcieSingleRootPort  ()
  ///

  ///
  /// System bios should initiate link retrain for all slots that has card populated after register restoration.
  /// Done in PchPciExpressHelpersLibrary.c PchPcieInitRootPortDownstreamDevices ()
  ///

  /// Disable command and data parity check for incoming data into the IOSF Target block, and
  /// disable the ability of downstream relaxed ordered completions from passing posted requests in the same direction.

  /// Set RECO[2] bit to ensure the big-endian byte order specified in the PTM Messages section (PCI Express Base Specification r4.0, r5.0 chapter 2.2.8.10)
  PciSegmentOr32 (
    RpBase + R_PCH_PCIE_CFG_XPTDEF2,
    B_PCH_PCIE_CFG_XPTDEF2_TCPAR_CHK_DIS |
    B_PCH_PCIE_CFG_XPTDEF2_TDPAR_CHK_DIS |
    B_PCH_PCIE_CFG_XPTDEF2_DNRODIS |
    (BIT2 << N_PCH_PCIE_CFG_XPTDEF2_RECO)
    );

  ///
  /// Configure Extended Synch
  ///
  if (PchPcieConfig->RootPort[PortIndex].ExtSync) {
    Data16And = (UINT16) ~0;
    Data16Or  = B_PCIE_LCTL_ES;
  } else {
    Data16And = (UINT16) (~B_PCIE_LCTL_ES);
    Data16Or  = 0;
  }
  PciSegmentAndThenOr16 (RpBase + R_PCH_PCIE_CFG_LINKCTL, Data16And, Data16Or);

  ///
  /// Configure Completion Timeout
  ///
  Data16And = (UINT16) ~(B_PCIE_DCTL2_CTD | B_PCIE_DCTL2_CTV);
  Data16Or  = 0;
  if (RootPortCommonConfig->CompletionTimeout == PcieCompletionTO_Disabled) {
    Data16Or = B_PCIE_DCTL2_CTD;
  } else {
    switch (RootPortCommonConfig->CompletionTimeout) {
      case PcieCompletionTO_Default:
        Data16Or = V_PCH_PCIE_CFG_DEVCTL2_CTV_DEFAULT;
        break;

      case PcieCompletionTO_50_100us:
        Data16Or = V_PCH_PCIE_CFG_DEVCTL2_CTV_50US_100US;
        break;

      case PcieCompletionTO_1_10ms:
        Data16Or = V_PCH_PCIE_CFG_DEVCTL2_CTV_1MS_10MS;
        break;

      case PcieCompletionTO_16_55ms:
        Data16Or = V_PCH_PCIE_CFG_DEVCTL2_CTV_16MS_55MS;
        break;

      case PcieCompletionTO_65_210ms:
        Data16Or = V_PCH_PCIE_CFG_DEVCTL2_CTV_65MS_210MS;
        break;

      case PcieCompletionTO_260_900ms:
        Data16Or = V_PCH_PCIE_CFG_DEVCTL2_CTV_260MS_900MS;
        break;

      case PcieCompletionTO_1_3P5s:
        Data16Or = V_PCH_PCIE_CFG_DEVCTL2_CTV_1S_3P5S;
        break;

      case PcieCompletionTO_4_13s:
        Data16Or = V_PCH_PCIE_CFG_DEVCTL2_CTV_4S_13S;
        break;

      case PcieCompletionTO_17_64s:
        Data16Or = V_PCH_PCIE_CFG_DEVCTL2_CTV_17S_64S;
        break;

      default:
        Data16Or = 0;
        break;
    }
  }

  PciSegmentAndThenOr16 (RpBase + R_PCH_PCIE_CFG_DEVCTL2, Data16And, Data16Or);

  ///
  /// For Root Port Slots Numbering on the CRBs.
  ///
  Data32Or  = 0;
  Data32And = (UINT32) (~(B_PCIE_SLCAP_SLV | B_PCIE_SLCAP_SLS | B_PCIE_SLCAP_PSN));

  ///
  /// Get the width from LCAP
  /// Slot Type  X1  X2/X4/X8/X16
  /// Default     10W   25W
  /// The slot power consumption and allocation is platform specific. Please refer to the
  /// "PCI Express* Card Electromechanical (CEM) Spec" for details.
  ///
  if (RootPortCommonConfig->SlotPowerLimitValue != 0) {
    Data32Or |= (UINT32) (RootPortCommonConfig->SlotPowerLimitValue << N_PCIE_SLCAP_SLV);
    Data32Or |= (UINT32) (RootPortCommonConfig->SlotPowerLimitScale << N_PCIE_SLCAP_SLS);
  } else {
    if (GetMaxLinkWidth (RpBase) == 1) {
      Data32Or |= (UINT32) (100 << N_PCIE_SLCAP_SLV);
      Data32Or |= (UINT32) (1 << N_PCIE_SLCAP_SLS);
    } else if (GetMaxLinkWidth (RpBase) >= 2) {
      Data32Or |= (UINT32) (250 << N_PCIE_SLCAP_SLV);
      Data32Or |= (UINT32) (1 << N_PCIE_SLCAP_SLS);
    }
  }

  ///
  /// PCH BIOS Spec section 8.2.4
  /// Initialize Physical Slot Number for Root Ports
  ///
  Data32Or |= (UINT32) (RootPortCommonConfig->PhysicalSlotNumber << N_PCIE_SLCAP_PSN);
  PciSegmentAndThenOr32 (RpBase + R_PCH_PCIE_CFG_SLOTCAP, Data32And, Data32Or);

  PcieInitHotPlugSupport (RpHandle);

  InitCapabilityList (RpHandle->Device, RootPortCommonConfig);

  //
  // All actions involving LinkDisable must finish before anything is programmed on endpoint side
  // because LinkDisable resets endpoint
  //

  ///
  /// Root PCI-E Powermanagement SCI Enable
  ///
  if (PchPcieConfig->RootPort[PortIndex].PcieRpCommonConfig.PmSci) {
    ///
    /// PCH BIOS Spec section 8.7.3 BIOS Enabling of Intel PCH PCI Express* PME SCI Generation
    /// Step 1
    /// Make sure that PME Interrupt Enable bit, Dxx:Fn:Reg 5Ch[3] is cleared
    ///
    PciSegmentAnd16 (RpBase + R_PCH_PCIE_CFG_ROOTCTL, (UINT16) (~B_PCIE_RCTL_PIE));

    ///
    /// Step 2
    /// Program Misc Port Config (MPC) register at PCI config space offset
    /// D8h as follows:
    /// Set Power Management SCI Enable bit, Dxx:Fn:Reg D8h[31]
    /// Clear Power Management SMI Enable bit, Dxx:Fn:Reg D8h[0]
    /// Use Byte Access to avoid RWO bit
    ///
    PciSegmentAnd8 (RpBase + R_PCH_PCIE_CFG_SMICSR, (UINT8) (~B_PCH_PCIE_CFG_SMICSR_PMSMIE));
    PciSegmentOr8 (RpBase + R_PCH_PCIE_CFG_SCICSR, (UINT8) (B_PCH_PCIE_CFG_SCICSR_PMSCIE));
  }


  ///
  /// PCH BIOS Spec Section 8.10 PCI Bus Emulation & Port80 Decode Support
  /// The I/O cycles within the 80h-8Fh range can be explicitly claimed
  /// by the PCIe RP by setting MPC.P8XDE, PCI offset D8h[26] = 1 (using byte access)
  /// BIOS must also configure the corresponding DMI registers GCS.RPR and GCS.RPRDID
  /// to enable DMI to forward the Port8x cycles to the corresponding PCIe RP
  ///
  if (PchPcieConfig->EnablePort8xhDecode && (PortIndex == (UINT8)PchPcieConfig->PchPciePort8xhDecodePortIndex)) {
    PciSegmentOr8 (RpBase + R_PCH_PCIE_CFG_PBTXNCTL, (UINT8) (B_PCH_PCIE_CFG_PBTXNCTL_P8XDE));
    if (RpHandle->Callback->PchRpIoPort80DecodeSet) {
      RpHandle->Callback->PchRpIoPort80DecodeSet (RpHandle->Device);
    } else {
      DEBUG ((DEBUG_ERROR, "PcieRpInit() PchRpIoPort80DecodeSet not implemented\n"));
    }
  }

 //
 // Enable MVC
 //

  if (PchPcieConfig->RootPort[PortIndex].MvcEnabled == TRUE) {
    PciSegmentAndThenOr8 (RpBase + R_PCH_PCIE_CFG_VC0RCTL, (UINT8) 0x0, V_PCH_PCIE_CFG_VC0RCTL_MVC);
    PciSegmentAndThenOr32 (RpBase + R_PCH_PCIE_CFG_VC1RCTL, (UINT32) 0x0, V_PCH_PCIE_CFG_VC1RCTL_MVC);
    DEBUG ((DEBUG_INFO, "PcieRpInit() MVC set for Rp %d\n", PortIndex));
  } else {
 //
 // Change TCVCM value to 7 to allow traffic targetting TC=7
 //
  PciSegmentOr8 (RpBase + R_PCH_PCIE_CFG_VC0RCTL, (UINT8) (V_PCH_PCIE_CFG_VC0RCTL_TCVCM << N_PCH_PCIE_CFG_VC0RCTL_TCVCM));
  }
}


/**
  Program Root Port PTM

  @param[in] RpHandle             Pointer to PCIE RP handle
**/
VOID
PcieRpProgramPtm (
  IN  PCH_RP_HANDLE           *RpHandle
  )
{
  UINT32    Index;
  UINT32    CtrlIndex;

  if ((RpHandle->Device->RootPortIndex % PCH_PCIE_CONTROLLER_PORTS) != 0) {
    return;
  }

  CtrlIndex = RpHandle->Device->CtrlIndex;

  //
  // PTM programming happens per controller
  // Lanes 0-5 have different mPhy type and require different programming than the rest
  // For 2nd controller which is composed of both types of mPhy lanes, the lower values must be used
  //
  for (Index = 0 ; Index < GetPchMaxPciePtmNum () ; Index++) {
    PchSbiRpPciAndThenOr32 (RpHandle,
                            mPtmRegisters[Index],
                            mPtmPipeStageDelay[CtrlIndex].Register[Index].AndValue,
                            mPtmPipeStageDelay[CtrlIndex].Register[Index].OrValue);
  }
  PchSbiRpPciAndThenOr32 (
    RpHandle,
    R_PCH_PCIE_CFG_PTMGTECSR,
    (UINT32) ~(B_PCH_PCIE_CFG_PTMGTECSR_PGTSCFF |
               B_PCH_PCIE_CFG_PTMGTECSR_PGTSCFE |
               B_PCH_PCIE_CFG_PTMGTECSR_IOSFMADP),
    V_PCH_PCIE_CFG_PTMGTECSR_IOSFMADP
    );
}


/**
  Perform Initialization of the Downstream Root Ports.

  @param[in] RpHandle             Pointer to PCIE RP handle
**/
VOID
PcieRpDownstreamRootPortInit (
  IN  PCH_RP_HANDLE           *RpHandle
  )
{
  PCH_RP_DEVICE              *RpDevice;

  RpDevice         = RpHandle->Device;

  InitPcieSingleRootPort (
    RpHandle
    );
  ///
  /// Initialize downstream devices
  ///
  RootportDownstreamConfiguration (
    RpDevice->Segment,
    RpDevice->Bus,
    RpDevice->Device,
    RpDevice->Function,
    RpDevice->TempPciBusMin,
    RpDevice->TempPciBusMax,
    EnumPchPcie
    );
}


/*
  Setup Root Port Link

  @param[in] RpHandle                   Pointer to PCIE RP handle
  @param[in] PcieRpLinkEqPreMemConfig   Pointer to PCIE RP pre mem config
  @param[in] RpEnabledMask              Root ports enabled mask
*/
VOID
PcieRpSetupLink (
  IN PCH_RP_HANDLE                       *RpHandle,
  IN ENTERPRISE_PCIE_LINK_PREMEM_CONFIG  *PcieRpLinkEqPreMemConfig,
  IN UINT32                              RpEnabledMask
  )
{
  UINT32                              RpIndex;
  ENTERPRISE_PCIE_LINK_EQ_CONFIG      PcieRpOneLinkPreMemConfig;
  UINT64                              RpBase;

  RpIndex = RpHandle->Device->RootPortIndex;

  //
  // Some of the root ports have already been disabled
  // in earlier parts of the BIOS flow. We have to avoid
  // executing EQ flow for such root ports.
  //
  RpBase = RpHandle->Device->PciCfgBaseAddr;
  if (PciSegmentRead16 (RpBase) == 0xFFFF) {
    return;
  }

  // Check if the RC default config for PCIe link equalization should be used
  // or the values from the BIOS setup
  if (!PcieRpLinkEqPreMemConfig->EqConfig[RpIndex].OverrideDefaultEqSettings) {
    PcieRpLinkEqPreMemSetConfigDefault (&PcieRpOneLinkPreMemConfig);
  } else {
    CopyMem (&PcieRpOneLinkPreMemConfig, &PcieRpLinkEqPreMemConfig->EqConfig[RpIndex], sizeof(PcieRpOneLinkPreMemConfig));
  }

  if ((RpEnabledMask & (BIT0 << RpIndex)) == 0) {
    if (RpHandle->Callback->PchRpPsfDisablePcieRootPort) {
      RpHandle->Callback->PchRpPsfDisablePcieRootPort (RpHandle->Device);
    } else {
      DEBUG ((DEBUG_ERROR, "PcieRpSetupLink() PchRpPsfDisablePcieRootPort not implemented\n"));
    }
  } else {
    PcieRpTrainToLinkActive (RpHandle, &PcieRpOneLinkPreMemConfig);
  }
}


/*
  Disable Root Port at PSF level

  @param[in] RpHandle             Pointer to PCIE RP handle
*/
VOID
PcieDisableUnusedPort (
  IN PCH_RP_HANDLE                   *RpHandle
  )
{
  PCH_RP_DEVICE   *RpDevice;
  PCH_RP_CALLBACK *RpCallback;
  UINT32 CtrlIndex;
  UINT32 RpIndex;
  UINT32 StrapInfo;
  UINT32 FuseInfo;
  UINT32 RootPortEnabled;

  RpDevice = RpHandle->Device;
  RpCallback = RpHandle->Callback;
  CtrlIndex = RpDevice->CtrlIndex;
  RpIndex = RpDevice->RootPortIndex;

  if (RpCallback->PchRpPcrRead32) {
    StrapInfo = RpCallback->PchRpPcrRead32 (RpDevice, PchGetPcieControllerSbiPid (CtrlIndex), R_PCH_PCIE_STRAPCSR1) & B_PCH_PCIE_STRAPCSR1_PORTEN;
    FuseInfo  = RpCallback->PchRpPcrRead32 (RpDevice, PchGetPcieControllerSbiPid (CtrlIndex), R_PCH_PCIE_FUSECSR1) & R_PCH_PCIE_FUSECSR1_PORTEN;

    RootPortEnabled = !!(StrapInfo & FuseInfo & (1 << (RpIndex % PCH_PCIE_CONTROLLER_PORTS)));

    if (!RootPortEnabled) {
      if (RpHandle->Callback->PchRpPsfDisablePcieRootPort) {
        RpHandle->Callback->PchRpPsfDisablePcieRootPort (RpDevice);
      } else {
        DEBUG ((DEBUG_ERROR, "PcieDisableUnusedPort() PchRpPsfDisablePcieRootPort not implemented\n"));
      }
    }
  } else {
    DEBUG ((DEBUG_ERROR, "PcieDisableUnusedPort() PchRpPcrRead32 not implemented\n"));
  }
}

