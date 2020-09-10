/** @file
  This file contains VER3 specific FIA routines

@copyright
  INTEL CONFIDENTIAL
  Copyright 2019 - 2020 Intel Corporation.

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
#include "PchFiaLibInternal.h"
#include "PchFia15.h"
#include <Register/PchPcrRegs.h>
#include <Library/PchDmiLib.h>
#include <PchPcieRpInfo.h>
#include <Library/PchInfoLib.h>

/**
  Returns the FIA Instance
  @return FIA Instance
**/
FIA_INSTANCE
FiaGetInstance (
  VOID
  )
{
  FIA_INSTANCE FiaInst;
  FiaInst.SbPortId = PID_FIA;
  return FiaInst;
}

/**
  Returns a FIA lane number for a given SATA port.

  @param[in]  SataCtrlIndex  SATA controller index
  @param[in]  /PortIndex  SATA port index
  @param[out] LaneNum        Pointer to the variable that stores lane number.
                             The output value is only valid if this function returns TRUE.

  @return TRUE if given SATA port owns FIA lane, FALSE otherwise
**/
BOOLEAN
PchFiaGetSataLaneNum (
  IN  UINT32  SataCtrlIndex,
  IN  UINT32  SataPortIndex,
  OUT UINT8   *LaneNum
  )
{
  FIA_INSTANCE FiaInst;
  UINT8        SocSataPortNum;

  FiaInst = FiaGetInstance ();
  SocSataPortNum = (UINT8) ((SataCtrlIndex * PCH_MAX_SATA_PORTS) + SataPortIndex);

  if (SocSataPortNum < PchFiaGetMaxLaneNum ()) {
    *LaneNum = ((PchFiaGetMaxLaneNum () - 1) - SocSataPortNum);
    return (PchFiaGetLaneOwner (FiaInst, *LaneNum) == PchFiaOwnerSata);
  } else {
    return FALSE;
  }
}

/**
  Returns a FIA lane number for a GbE controller

  @param[out] LaneNum  FIA lane number that is owned by GbE

  @retval TRUE   Found FIA lane assigned to GbE
  @retval FALSE  No lanes assigned to GbE
**/
BOOLEAN
PchFiaGetGbeLaneNum (
  OUT UINT8  *LaneNum
  )
{
  FIA_INSTANCE FiaInst;
  UINT8  GbeLanes[] = {9, 11, 13};
  UINT8  LaneIndex;


  FiaInst = FiaGetInstance ();

  //
  // Since there is only one instance of GbE controller on the system
  // we return first lane that is owned by GbE.
  //
  for (LaneIndex = 0; LaneIndex < ARRAY_SIZE (GbeLanes); LaneIndex++) {
    if (PchFiaGetLaneOwner (FiaInst, GbeLanes[LaneIndex]) == PchFiaOwnerGbe) {
      *LaneNum = GbeLanes[LaneIndex];
      return TRUE;
    }
  }

  DEBUG ((DEBUG_INFO, "No FIA lanes assigned as GbE\n"));
  return FALSE;
}

/**
  Checks if given lane is DMI

  @param[in]  FiaLaneNum  Fia lane num

  @return TRUE if given lane is DMI, FALSE otherwise
**/
BOOLEAN
PchFiaIsLaneDmi (
  IN UINT8  FiaLaneNum
  )
{
  if (FiaLaneNum < PchDmiNegotiatedLinkWidth ()) {
    return TRUE;
  }
  return FALSE;
}

/**
  Returns a FIA lane number for a given PCIe lane.

  @param[in]  PciePhysicalLane  Index of the PCIe lane
  @param[out] LaneNum           Pointer to the variable that stores lane number.
                                The output value is only valid if this function returns TRUE.

  @return TRUE if given PciePhysicalLane owns FIA lane, FALSE otherwise
**/
BOOLEAN
PchFiaGetPcieLaneNum (
  IN  UINT32  PciePhysicalLane,
  OUT UINT8   *LaneNum
  )
{
  if (PciePhysicalLane < 8) {
    //
    // SPA controller lanes start at the last FIA lane.
    //
    *LaneNum = (PchFiaGetMaxLaneNum () - 1) - (UINT8) PciePhysicalLane;
  } else if (PciePhysicalLane < 24) {
    //
    // SPB and SPC have an unusual lane layout to be able to support 4x1 PCIe configuration on PCIe controller
    // which doesn't support x1 connections. In 4x1 mode controller is configured to 4x2 mode but FIA doesn't route
    // second lane of each of the root ports. In 2x2 or in 1x4 modes only first 4 root ports are used and all PCIe lanes
    // are routed in FIA. This results in a mapping where depending on the bifurcation settings FIA lane 14 can be routed
    // either to PCIe lane 9 in 2x2 or 1x4 modes or to lane 12 in 4x1 mode. We assume that caller of this function is aware
    // of the bifurcation settings and we only blindly check lane ownership without validating if they agree with bifurcation.
    //
    switch (PciePhysicalLane) {
      case 8:
        *LaneNum = 15;
        break;
      case 9:
      case 12:
        *LaneNum = 14;
        break;
      case 10:
        *LaneNum = 13;
        break;
      case 11:
      case 14:
        *LaneNum = 12;
        break;
      case 16:
        *LaneNum = 11;
        break;
      case 17:
      case 20:
        *LaneNum = 10;
        break;
      case 18:
        *LaneNum = 9;
        break;
      case 19:
      case 22:
        *LaneNum = 8;
        break;
      //
      // All other PCIe lanes are not connected to FIA.
      //
      default:
        return FALSE;
    }
  } else if (PciePhysicalLane < 32) {
    //
    // SPD controller starts at FIA lane 7 and continues down to FIA lane 0.
    //
    *LaneNum = 7 - (PciePhysicalLane % 8);
  } else {
    //
    // Invalid PCIe lane
    //
    return FALSE;
  }
  return (PchFiaGetLaneOwner (FiaGetInstance (), *LaneNum) == PchFiaOwnerPcie);
}

/**
  Returns a FIA lane number for a given USB3 port.
  This function assumes that USB3 port to FIA lane mapping is 1:1

  @param[in]  Usb3PortIndex  USB3 port index
  @param[out] LaneNum        Pointer to the variable that stores lane number.
                             The output value is only valid if this function returns TRUE.

  @return TRUE if given USB3 port owns FIA lane, FALSE otherwise
**/
UINT8
PchFiaGetUsb3LaneNum (
  IN  UINT32  Usb3PortIndex,
  OUT UINT8   *LaneNum
  )
{
  ASSERT (Usb3PortIndex < GetPchUsb3MaxPhysicalPortNum ());
  *LaneNum = (PchFiaGetMaxLaneNum () - 1) - (UINT8) Usb3PortIndex;
  return (PchFiaGetLaneOwner (FiaGetInstance (), *LaneNum) == PchFiaOwnerUsb3);
}

/**
  Returns a FIA lane number for a given UFS lane.

  @param[in]  UfsControllerIndex  Index of the UFS controller
  @param[in]  UfsLaneIndex        Index of the UFS lane on given controller
  @param[out] LaneNum             Optional. Pointer to the variable that stores lane number.
                                  The output value is only valid if this function returns TRUE.

 @return TRUE if given UFS lane owns FIA lane, FALSE otherwise
**/
BOOLEAN
PchFiaGetUfsLaneNum (
  IN  UINT32  UfsControllerIndex,
  IN  UINT32  UfsLaneIndex,
  OUT UINT8   *LaneNum
  )
{
  ASSERT (FALSE);
  return FALSE;
}

/**
  Returns a FIA lane number for a given TSN port.

  @param[in]  TsnPortIndex  TSN port index
  @param[out] LaneNum       Pointer to the variable that stores lane number.
                            The output value is only valid if this function returns TRUE.

  @return TRUE if given TSN port owns FIA lane, FALSE otherwise
**/
BOOLEAN
PchFiaGetTsnLaneNum (
  IN  UINT32  TsnPortIndex,
  OUT UINT8   *LaneNum
  )
{
  ASSERT (FALSE);
  return FALSE;
}

/**
  Returns number of FIA lanes

  @return Number of FIA lanes
**/
UINT8
PchFiaGetMaxLaneNum (
  VOID
  )
{
  return 24;
}

/**
  Return FIA lane owner.

  @param[in] LaneNum  FIA lane number

  @return Code of the FIA lane owner, PchFiaOwnerInvalid if lane number wasn't valid
**/
PCH_FIA_LANE_OWNER
PchFiaGetLaneOwner (
  IN  FIA_INSTANCE FiaInst,
  IN  UINT8        LaneNum
  )
{
  if (LaneNum >= PchFiaGetMaxLaneNum ()) {
    ASSERT (FALSE);
    return PchFiaOwnerInvalid;
  }
  return PchFia15GetLaneOwner (FiaInst, LaneNum);
}

/**
  Print FIA LOS registers.
**/
VOID
PchFiaPrintLosRegisters (
  VOID
  )
{
  FIA_INSTANCE FiaInst;
  FiaInst = FiaGetInstance ();
  PchFia15PrintLosRegisters (FiaInst);
}

/**
  Assigns CLKREQ# to PCH PCIe ports

  @param[in] ClkReqMap      Mapping between PCH PCIe ports and CLKREQ#
  @param[in] ClkReqMapSize  Size of the map
**/
VOID
PchFiaAssignPchPciePortsClkReq (
  IN UINT8  *ClkReqMap,
  IN UINT8  ClkReqMapSize
  )
{
  FIA_INSTANCE FiaInst;
  FiaInst = FiaGetInstance ();
  PchFia15AssignPchPciePortsClkReq (FiaInst, ClkReqMap, ClkReqMapSize);
}

/**
  Assigns CLKREQ# to CPU PCIe ports

  @param[in] RpIndex        CPU PCIe root port index
  @param[in] ClkReqNum      Number of the CLKREQ
**/
VOID
PchFiaAssignCpuPciePortClkReq (
  IN UINT32  RpIndex,
  IN UINT8   ClkReqNum
  )
{

}

/**
  Enable CLKREQ# to CPU PCIe ports

  @param[in] RpIndex        CPU PCIe root port index
  @param[in] ClkReqNum      Number of the CLKREQ
**/
VOID
PchFiaEnableCpuPciePortClkReq (
  IN UINT32  RpIndex,
  IN UINT8   ClkReqNum
)
{
  return;
}

/**
  Return the status of the CLKREQ state received with VW msg.

  @param[in] RpIndex  CPU PCIe index.

  @return Status of the CLKREQ.
**/
BOOLEAN
PchFiaGetCpuPcieClkReqStatus (
  IN UINT32  RpIndex
  )
{
  ASSERT (FALSE);
  return FALSE;
}

/**
  Assigns CLKREQ# to GbE

  @param[in]  ClkReqNum  CLKREQ# number
**/
VOID
PchFiaAssignGbeClkReq (
  IN UINT8  ClkReqNum
  )
{
  FIA_INSTANCE FiaInst;
  FiaInst = FiaGetInstance ();
  PchFia15AssignGbeClkReq (FiaInst, ClkReqNum);
}

/**
  Configures lower bound of delay between ClkReq assertion and driving RefClk.
**/
VOID
PchFiaSetClockOutputDelay (
  VOID
  )
{
  FIA_INSTANCE FiaInst;
  FiaInst = FiaGetInstance ();
  PchFia15SetClockOutputDelay (FiaInst);
}

/**
  Performs FIA programming required at the end of configuration and locks lockable FIA registers
**/
VOID
PchFiaFinalizeConfigurationAndLock (
  VOID
  )
{
  FIA_INSTANCE FiaInst;
  FiaInst = FiaGetInstance ();
  //
  // Program PCR[FIA] + 20h bit [13:11] to 001, bit [30:28] to [000]
  //
  PchPcrAndThenOr32 (
    FiaInst.SbPortId,
    R_PCH_FIA_PCR_PLLCTL,
    (UINT32) ~(B_PCH_FIA_PCR_PLLCTL_CL0PLLWAIT | B_PCH_FIA_PCR_PLLCTL_PLLCLKVADT),
    (UINT32) (1 << N_PCH_FIA_PCR_PLLCTL_CL0PLLWAIT)
    );

  //
  // Program PCR[FIA] + 18h bit [17:16, 1:0] to [00, 00]
  //
  PchPcrAndThenOr32 (
    FiaInst.SbPortId,
    R_PCH_FIA_PCR_CLSDM,
    (UINT32) ~(B_PCH_FIA_PCR_CLSDM_DMIIPSLSD | B_PCH_FIA_PCR_CLSDM_PCIEGBEIPSLSD),
    0
    );

  //
  // Set PCR[FIA] + 0h bit [31, 17, 16, 15] to [1, 1, 1, 1]
  //
  PchPcrAndThenOr32 (
    FiaInst.SbPortId,
    R_PCH_FIA_PCR_CC,
    ~0u,
    B_PCH_FIA_PCR_CC_SRL | B_PCH_FIA_PCR_CC_PTOCGE | B_PCH_FIA_PCR_CC_OSCDCGE | B_PCH_FIA_PCR_CC_SCPTCGE
    );

  //
  // Set PCR[FIA] + 48h bit [0] to [0]
  //
  PchPcrAndThenOr32 (
    FiaInst.SbPortId,
    R_PCH_FIA_PCR_PGCUC,
    (UINT32) ~(B_PCH_FIA_PCR_PGCUC_ACC_CLKGATE_DISABLED),
    0
    );
}

/**
  Returns number of FIA LOS registers used

  @param[in] FiaIndex        FIA index to specify FIA instance

  @return Number of FIA LOS registers used
**/
UINT32
PchFiaGetMaxLosRegister (
  FIA_INDEX FiaIndex
  )
{
  ASSERT (FiaIndex == FiaIndexFia);
  return (((PchFiaGetMaxLaneNum () - 1) / 8) + 1);
}

/**
  Get FIA LOS register value

  @param[in] FiaIndex        FIA index to specify FIA instance
  @param[in] LosIndex        LOS FIA register index

  @return  LOS FIA register value
**/
UINT32
PchFiaGetLos (
  FIA_INDEX FiaIndex,
  UINT32    LosIndex
  )
{
  UINT32  MaxLosRegister;
  FIA_INSTANCE FiaInst;

  ASSERT (FiaIndex == FiaIndexFia);
  FiaInst = FiaGetInstance ();

  MaxLosRegister = ((PchFiaGetMaxLaneNum () - 1) / 8) + 1;
  if (LosIndex > MaxLosRegister) {
    ASSERT (FALSE);
    LosIndex = MaxLosRegister;
  }

  return PchPcrRead32 (FiaInst.SbPortId, R_PCH_FIA_15_PCR_LOS1_REG_BASE + (4 * LosIndex));
}