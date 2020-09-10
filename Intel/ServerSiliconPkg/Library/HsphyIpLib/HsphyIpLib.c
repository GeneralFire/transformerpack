/** @file
  Hsphy IP library

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

#include <Library/BaseLib.h>
#include <Library/IoLib.h>
#include <Library/UpiHostDataLib.h>
#include <Library/HsphyIpLib.h>
#include <Library/UncoreLib.h>
#include <RcRegs.h>
#include <Library/UsraCsrLib.h>

UINT32 mHsphy_Upi_PortId_SPR[] = {
  837, // HSPhy1.SCF_UPI_1x.0 (UPI0 Lane 0~7)
  838, // HSPhy2.SCF_UPI_1x.0 (UPI0 Lane 8~15)
  839, // HSPhy3.SCF_UPI_1x.0 (UPI0 Lane 16~23)

  849, // HSPhy1.SCF_UPI_1x.1 (UPI1 Lane 0~7)
  850, // HSPhy2.SCF_UPI_1x.1 (UPI1 Lane 8~15)
  851, // HSPhy3.SCF_UPI_1x.1 (UPI1 Lane 16~23)

  843, // HSPhy1.SCF_UPI_1x.2 (UPI2 Lane 0~7)
  844, // HSPhy2.SCF_UPI_1x.2 (UPI2 Lane 8~15)
  845, // HSPhy3.SCF_UPI_1x.2 (UPI2 Lane 16~23)

  855, // HSPhy1.SCF_UPI_1x.3 (UPI3 Lane 0~7)
  856, // HSPhy2.SCF_UPI_1x.3 (UPI3 Lane 8~15)
  857, // HSPhy3.SCF_UPI_1x.3 (UPI3 Lane 16~23)
};

UINT8 mHsphy_Upi_PortId_SPR_length = sizeof (mHsphy_Upi_PortId_SPR) / sizeof (mHsphy_Upi_PortId_SPR[0]);

UINT32 mHsphy_Pcie_PortId_SPR[] = {
  564, // HSPhy_reg0.PI5.0 --- Port 0 Lane 0~7
  565, // HSPhy_reg1.PI5.0 --- Port 0 Lane 8~15

  584, // HSPhy_reg0.PI5.1 --- Port 1 Lane 0~7
  585, // HSPhy_reg1.PI5.1 --- Port 1 Lane 8~15

  604, // HSPhy_reg0.PI5.3 --- Port 2 Lane 0~7
  605, // HSPhy_reg1.PI5.3 --- Port 2 Lane 8~15

  644, // HSPhy_reg0.PI5.4 --- Port 3 Lane 0~7
  645, // HSPhy_reg1.PI5.4 --- Port 3 Lane 8~15

  624, // HSPhy_reg0.PI5.5 --- Port 4 Lane 0~7
  625, // HSPhy_reg1.PI5.5 --- Port 4 Lane 8~15

  664, // HSPhy_reg0.PI5.7 --- Port 5 Lane 0~7
  665, // HSPhy_reg1.PI5.7 --- Port 5 Lane 8~15

  555, // HSPhy_reg0.PI5.2 --- Port 6 Lane 0~7
  556, // HSPhy_reg1.PI5.2 --- Port 6 Lane 8~15

  771, // HSPhy_reg0.PI5.6 --- Port 7 Lane 0~7
  772, // HSPhy_reg1.PI5.6 --- Port 7 Lane 8~15
};

UINT8 mHsphy_Pcie_PortId_SPR_length = sizeof (mHsphy_Pcie_PortId_SPR) / sizeof (mHsphy_Pcie_PortId_SPR[0]);

STATIC CHAR8 HSPHY_RECIPE_MESSAGE[] = "\n\n Port %x is unsupported for Recipe Type %a!\n";

//
// cfg_vpcspipe_rate [6:4] of SCF_UPI_1X_x_HSPHY_PCS_0_CR_LANE_reg_17: used to indicate the UPI final speed
//
#define UPI_HSPHY_PCS_PIPE_RATE_OFFSET   0x7044
#define UPI_HSPHY_PCS_PIPE_RATE_MASK     (BIT6 | BIT5 | BIT4)

/**
  This function will get the Sb Port ID according to the recipe type, port and Lane.

  @param[in]     RecipeType            UPI recipe or PCIE recipe
  @param[in]     Port                  UPI link or PCIE Port
  @param[in]     HsphyRegIndex         Hsphy reg Index

  @retval        SbPortId              SB_REG_PORT_ID_INVALID --- means the return SB Port ID is invalid

**/
UINT32
EFIAPI
GetHsphySbPortId (
  IN       UINT8              RecipeType,
  IN       UINT8              Port,
  IN       UINT8              HsphyRegIndex
)
{
  UINT32  SbPortId;

  SbPortId = SB_REG_PORT_ID_INVALID;

  if (RecipeType == RECIPE_UPI_TYPE) {
    if (Port >= (mHsphy_Upi_PortId_SPR_length / MAX_HSPHY_REG_INDEX_PER_UPI)) {
      DEBUG ((EFI_D_ERROR, HSPHY_RECIPE_MESSAGE, Port, "UPI"));
      return SbPortId;
    }
    SbPortId = mHsphy_Upi_PortId_SPR[Port * MAX_HSPHY_REG_INDEX_PER_UPI + HsphyRegIndex];
  } else if (RecipeType == RECIPE_PCIE_TYPE) {
    if (Port >= (mHsphy_Pcie_PortId_SPR_length / MAX_HSPHY_REG_INDEX_PER_PCIE)) {
      DEBUG ((EFI_D_ERROR, HSPHY_RECIPE_MESSAGE, Port, "PCIE"));
      return SbPortId;
    }
    SbPortId = mHsphy_Pcie_PortId_SPR[Port * MAX_HSPHY_REG_INDEX_PER_PCIE + HsphyRegIndex];
  } else {
    DEBUG ((EFI_D_ERROR, "\n\nUnsupported Recipe Type %x\n", RecipeType));
  }

  return SbPortId;
}

/**
  This function will set the target's sb port id to get the target recipe SB MMIO BAR address

  @param[in]     Socket                Socket Number
  @param[in]     SbPortId              The Hsphy Reg's sb portid

  @retval        PhysicalAddress       The target recipe SB MMIO BAR address

**/
UINTN
EFIAPI
GetHsphyRecipeTargetPhyAddr (
  IN       UINT8              Socket,
  IN       UINT32             SbPortId
)
{
  CPU_CSR_ACCESS_VAR          *CpuCsrAccessVarPtr;
  UINTN                       PhysicalAddress;

  CpuCsrAccessVarPtr = GetSysCpuCsrAccessVar ();
  if (SbPortId != CpuCsrAccessVarPtr->LastSBPortId[Socket]) {
    UBoxConfigureSbRegPteReg (Socket, 0, SbPortId);
    CpuCsrAccessVarPtr->LastSBPortId[Socket] = SbPortId;
  }
  PhysicalAddress = (UINTN)(CpuCsrAccessVarPtr->MmioBar[Socket][TYPE_SBREG_BAR]);
  return PhysicalAddress;
}

/**
  This function will do the hsphy recipe programming for UPI & PCIE.

  @param[in]     RecipeType            UPI recipe or PCIE recipe
  @param[in]     Socket                Ssocket number.
  @param[in]     Port                  UPI link or PCIE Port (IIO Stack)
  @param[in]     X8LaneIndex           The Index of x8 lanes (SPR: UPI has 3 set of x8 lanes; PCIE has 2 sets of x8 lanes)
  @param[in]     RegOffset             Register Offset.
  @param[in]     BitMask               The value to AND with the read value from register.
  @param[in]     OrData                The value to OR with the result of the AND operation.

  @retval        EFI_SUCCESS           The programming is successful.
  @retval        EFI_INVALID_PARAMETER One of the input parameter is invalid or out of range.
  @retval        EFI_NOT_FOUND         Cant't find the access way.

**/
EFI_STATUS
EFIAPI
HsphyRecipeProgramming (
  IN       UINT8              RecipeType,
  IN       UINT8              Socket,
  IN       UINT8              Port,
  IN       UINT8              X8LaneIndex,
  IN       UINT32             RegOffset,
  IN       UINT32             BitMask,
  IN       UINT32             OrData
)
{
  UINT32     SbPortId;
  UINTN      PhysicalAddress;
  UINT32     Data32;
  UINT8      MaxHsphyRegIndex;

  //
  // Get the max hsphy reg number according to the Port type, loop all the hsphy reg.
  //
  if (RecipeType == RECIPE_UPI_TYPE) {
    MaxHsphyRegIndex = MAX_HSPHY_REG_INDEX_PER_UPI;
  } else if (RecipeType == RECIPE_PCIE_TYPE) {
    MaxHsphyRegIndex = MAX_HSPHY_REG_INDEX_PER_PCIE;
  } else {
    DEBUG ((EFI_D_ERROR, "\n\nUnsupported Recipe Type %x\n", RecipeType));
    return EFI_INVALID_PARAMETER;
  }

  if (X8LaneIndex >= MaxHsphyRegIndex) {
    DEBUG ((EFI_D_ERROR, "\n\nUnsupported Index of x8 Lanes %x\n", X8LaneIndex));
    return EFI_INVALID_PARAMETER;
  }

  //
  // Step1: Get the Sb Port ID of the target (Port, X8LaneIndex)
  //
  SbPortId = GetHsphySbPortId (RecipeType, Port, X8LaneIndex);
  if (SbPortId == SB_REG_PORT_ID_INVALID) {
    return EFI_NOT_FOUND;
  }

  //
  // Step2: Get the target recipe SB MMIO BAR address (Socket) via the target SB port id
  //
  PhysicalAddress = GetHsphyRecipeTargetPhyAddr (Socket, SbPortId);
  PhysicalAddress |= (RegOffset & 0xffff);
  DEBUG ((EFI_D_VERBOSE, "\nx8 Lane Index %x, SB PortId: %x, PhysicalAddress: %x", X8LaneIndex, SbPortId, PhysicalAddress));

  //
  // Step3: Programming the HSPHY recipe to the target address
  //
  Data32 = MmioRead32 (PhysicalAddress);
  Data32 &= BitMask;
  Data32 |= OrData;
  MmioWrite32 (PhysicalAddress, Data32);

  return EFI_SUCCESS;
}

/**
  This function will return the final upi hsphy speed.

  @param[in]     Socket                Ssocket number.
  @param[in]     Port                  UPI link or PCIE Port (IIO Stack)

  @retval        The final upi hsphy speed of the port.

**/
UINT8
EFIAPI
GetUpiHsphyPipeRate (
  IN       UINT8              Socket,
  IN       UINT8              Port
)
{
  UINT32     SbPortId;
  UINTN      PhysicalAddress;
  UINT8      Data8;
  UINT8      UpiHsphyRegIndex;
  UINT8      OriX8LanePipeRate;

  OriX8LanePipeRate = 0xff;

  for (UpiHsphyRegIndex = 0; UpiHsphyRegIndex < MAX_HSPHY_REG_INDEX_PER_UPI; UpiHsphyRegIndex++) {
    SbPortId = GetHsphySbPortId (RECIPE_UPI_TYPE, Port, UpiHsphyRegIndex);
    if (SbPortId == SB_REG_PORT_ID_INVALID) {
      return 0;
    }

    PhysicalAddress = GetHsphyRecipeTargetPhyAddr (Socket, SbPortId);
    PhysicalAddress |= UPI_HSPHY_PCS_PIPE_RATE_OFFSET;

    Data8 = MmioRead8 (PhysicalAddress);
    Data8 &= UPI_HSPHY_PCS_PIPE_RATE_MASK;
    if (OriX8LanePipeRate == 0xff) {
      OriX8LanePipeRate = Data8;
    } else {
      if (OriX8LanePipeRate != Data8) {
        return 0;
      }
    }
  }

  return (OriX8LanePipeRate >> 4);
}
