/** @file
  This file contains CNL specific FIA routines

@copyright
  INTEL CONFIDENTIAL
  Copyright 2017 - 2020 Intel Corporation.

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
#include <Register/PchRegsPcr.h>
#include <Library/PchPcieRpInfo.h>

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
  @param[in]  SataPortIndex  SATA port index
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
  FiaInst = FiaGetInstance ();
 *LaneNum = (UINT8) (SataCtrlIndex*8 + SataPortIndex);

  return (PchFiaGetLaneOwner (FiaInst, *LaneNum) == PchFiaOwnerSata);
}

/**
  Returns GbE over PCIe port number when GbE is enabled

  @return PCIe root port number(1- based), 0 if GbE over PCIe is disabled
**/
UINT32
PchFiaGetGbePortNumber (
  VOID
  )
{
  DEBUG ((DEBUG_INFO, "No FIA lanes assigned as GbE\n"));
  return 0;
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
  //
  // For CDF lane mapping is 1:1
  //
  *LaneNum = (UINT8) PciePhysicalLane;
  return (PchFiaGetLaneOwner (FiaGetInstance (), *LaneNum) == PchFiaOwnerPcie);
}

/**
  Checks if a given PCIe root port is assigned any FIA lane(s)

  @param[in]  RpIndex      PCIe root port index

  @return  TRUE if given PCIe root port is assigned a FIA lane(s), FALSE otherwise
**/
BOOLEAN
PchFiaIsPcieRootPortWired (
  IN UINT32  RpIndex
  )
{
  UINT8   LaneNum;
  UINT32  Lane;
  UINT32  ControllerPortIndex;
  UINT32  FirstPortLane;

  ControllerPortIndex = RpIndex % PCH_PCIE_CONTROLLER_PORTS;
  //
  // If port has more than one lane PchPciePhysicalLane() returns first assigned lane number
  //
  FirstPortLane = PchPciePhysicalLane (RpIndex, ControllerPortIndex);

  for (Lane = FirstPortLane; Lane < (FirstPortLane + PCH_PCIE_LANES_PER_PORT); Lane++) {
    if (PchFiaGetPcieLaneNum (Lane, &LaneNum)) {
      return TRUE;
    }
  }

  return FALSE;
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
  *LaneNum = (UINT8) Usb3PortIndex;
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
  Returns actual number of FIA lanes

  @return atual number of FIA lanes
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

  IN SI_POLICY_PPI          *SiPolicy
  )
{
  FIA_INSTANCE          FiaInst;
  UINT32                Data32Or;

  FiaInst = FiaGetInstance ();

  //
  // Program PCR[FIA] + 20h bit [13:11] to 1
  //
  PchPcrAndThenOr32 (
    FiaInst.SbPortId,
    R_PCH_FIA_PCR_PLLCTL,
    ~0u,
    B_PCH_FIA_PCR_PLLCTL_CL0PLLWAIT & ((UINT32)1 << N_PCH_FIA_PCR_PLLCTL_CL0PLLWAIT)
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
  Data32Or = 0;
    Data32Or |= B_PCH_FIA_PCR_CC_SRL;
  Data32Or |=B_PCH_FIA_PCR_CC_PTOCGE | B_PCH_FIA_PCR_CC_OSCDCGE | B_PCH_FIA_PCR_CC_SCPTCGE;
  PchPcrAndThenOr32 (
    FiaInst.SbPortId,
    R_PCH_FIA_PCR_CC,
    ~0u,
    Data32Or
    );

}

