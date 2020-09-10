/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018-2020 Intel Corporation. <BR>

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
#include <Library/DebugLib.h>
#include <Library/UsraCsrLib.h>
#include <Library/OobIpLib.h>
#include <Library/KtiApi.h>
#include <Library/CpuAndRevisionLib.h>
#include <KtiSetupDefinitions.h>


#if defined(SNR_HOST) || defined(TNR_HOST) || defined(ICXDE_HOST)

//
//SNR PMON definitions
//

//
// Offset is *PMONUNITCTRL* in header files.
//
STATIC UNIT_DISCOVERY_STRUCT UnitDiscoveryTableDefault[UNIT_TYPE_MAX] = {
  {UNIT_TYPE_CHA,    UNIT_DISC_0_LOW(0x8, 48, 0x1, 4), UNIT_DISC_0_HIGH(PMON_ACCESS_MSR,   0x7), 0x1C00,   2,   2}, // CHA
  {UNIT_TYPE_TC,     UNIT_DISC_0_LOW(0x1, 48, 0x8, 4), UNIT_DISC_0_HIGH(PMON_ACCESS_MSR,   0x7), 0x1E00,   8,   8}, // TC
  {UNIT_TYPE_IRP,    UNIT_DISC_0_LOW(0x1, 48, 0x8, 2), UNIT_DISC_0_HIGH(PMON_ACCESS_MSR,   0x7), 0x1EA0,  13,  13}, // IRP
  {UNIT_TYPE_M2PCIE, UNIT_DISC_0_LOW(0x1, 48, 0x8, 4), UNIT_DISC_0_HIGH(PMON_ACCESS_MSR,   0x7), 0x1E50,  18,  18}, // M2PCIE
  {UNIT_TYPE_PCU,    UNIT_DISC_0_LOW(0x8, 48, 0x1, 4), UNIT_DISC_0_HIGH(PMON_ACCESS_MSR,   0x7), 0x1EF0,  23,  23}, // PCU
  {UNIT_TYPE_UBOX,   UNIT_DISC_0_LOW(0x8, 48, 0x3, 2), UNIT_DISC_0_HIGH(PMON_ACCESS_MSR,   0x7), 0x0000,   1,   1}, // UBOX
  {UNIT_TYPE_MC,     UNIT_DISC_0_LOW(0x8, 48, 0x40,4), UNIT_DISC_0_HIGH(PMON_ACCESS_MMIO, 0x5C), 0x22800, 24,  24}, // MC
  {UNIT_TYPE_M2M,    UNIT_DISC_0_LOW(0x8, 48, 0x30,4), UNIT_DISC_0_HIGH(PMON_ACCESS_CFG,  0x70), 0x438,   27,  27}, // M2M
  {UNIT_TYPE_UPI_LL, UNIT_DISC_0_LOW(0x0,  0,  0x0,0), UNIT_DISC_0_HIGH(PMON_ACCESS_CFG,  0x00), 0x000,    0,   0}, // UPI invalid for SNR
  {UNIT_TYPE_M3UPI,  UNIT_DISC_0_LOW(0x0,  0,  0x0,0), UNIT_DISC_0_HIGH(PMON_ACCESS_CFG,  0x00), 0x000,    0,   0}, // M3UPI invalid for SNR
  {UNIT_TYPE_PCIE,   UNIT_DISC_0_LOW(0x8, 48, 0x28,4), UNIT_DISC_0_HIGH(PMON_ACCESS_CFG,   0x4), 0x4E0,   28,  28}  // PCIE
};
#endif


#if defined(ICX_HOST)

//
//ICX-SP PMON definitions
//

//
// Offset is *PMONUNITCTRL* in header files.
//
STATIC UNIT_DISCOVERY_STRUCT UnitDiscoveryTableDefault[UNIT_TYPE_MAX] = {
  {UNIT_TYPE_CHA,    UNIT_DISC_0_LOW(0x8, 48, 0x1, 4), UNIT_DISC_0_HIGH(PMON_ACCESS_MSR,   0x7), 0x0E00,   2,   2}, // CHA
  {UNIT_TYPE_TC,     UNIT_DISC_0_LOW(0x1, 48, 0x8, 4), UNIT_DISC_0_HIGH(PMON_ACCESS_MSR,   0x5), 0x0A50,  18,  18}, // TC
  {UNIT_TYPE_IRP,    UNIT_DISC_0_LOW(0x1, 48, 0x3, 2), UNIT_DISC_0_HIGH(PMON_ACCESS_MSR,   0x5), 0x0A4A,  24,  24}, // IRP
  {UNIT_TYPE_M2PCIE, UNIT_DISC_0_LOW(0x1, 48, 0x6, 4), UNIT_DISC_0_HIGH(PMON_ACCESS_MSR,   0x5), 0x0A40,  30,  30}, // M2PCIE
  {UNIT_TYPE_PCU,    UNIT_DISC_0_LOW(0x7, 48, 0x1, 4), UNIT_DISC_0_HIGH(PMON_ACCESS_MSR,   0x6), 0x0710,  36,  36}, // PCU
  {UNIT_TYPE_UBOX,   UNIT_DISC_0_LOW(0x8, 48, 0x3, 2), UNIT_DISC_0_HIGH(PMON_ACCESS_MSR,   0x7), 0x0000,   1,   1}, // UBOX
  {UNIT_TYPE_MC,     UNIT_DISC_0_LOW(0x8, 48, 0x40,4), UNIT_DISC_0_HIGH(PMON_ACCESS_MMIO, 0x5C), 0x22800, 64,  37}, // MC
  {UNIT_TYPE_M2M,    UNIT_DISC_0_LOW(0x8, 48, 0x30,4), UNIT_DISC_0_HIGH(PMON_ACCESS_CFG,  0x70), 0x438,   76,  76}, // M2M
  {UNIT_TYPE_UPI_LL, UNIT_DISC_0_LOW(0x8, 48, 0x38,4), UNIT_DISC_0_HIGH(PMON_ACCESS_CFG,  0x64), 0x318,   80,  80}, // UPI
  {UNIT_TYPE_M3UPI,  UNIT_DISC_0_LOW(0x8, 48, 0x38,4), UNIT_DISC_0_HIGH(PMON_ACCESS_CFG,  0x58), 0x0A0,   83,  83}, // M3UPI
  {UNIT_TYPE_PCIE,   UNIT_DISC_0_LOW(0x8, 48, 0x28,4), UNIT_DISC_0_HIGH(PMON_ACCESS_CFG,   0x4), 0x4E0,   86,  86}  // PCIE
};
#endif

#define PMON_MAX_STATE_SUPPORTED     (MAX_STATE_TC     +\
                                      MAX_STATE_IRP    +\
                                      MAX_STATE_M2PCIE +\
                                      MAX_STATE_PCU    +\
                                      MAX_STATE_UBOX   +\
                                      MAX_STATE_MC     +\
                                      MAX_STATE_M2M    +\
                                      MAX_STATE_PCIE    \
                                      )

/**
  OOB: Get CHA pmon count

  @param   ChopType   - The CPU chop type to be used

  @retval             - The number of CHA PMONs available or zero if not supported
**/
UINT8
OobMsmGetPmonChaCountPrivate (
  UINT8 ChopType
)
{
  switch (ChopType) {
    case TypeXcc:
      return MAX_STATE_CHA_XCC;
    case TypeHcc:
      return MAX_STATE_CHA_HCC;
    case TypeLcc:
      return MAX_STATE_CHA_LCC;
  }
  return 0;
}

/**
  OOB: Get the pmon offset table

  @param   UnitTable  - Table to fill in with discovery unit offsets
  @param   ChopType   - The CPU chop type to be used
  @param   PmonConfig - Indicates what PMONs will be configured

  @retval            - True if pmon table was supported

**/
BOOLEAN
EFIAPI
OobMsmGetPmonOffsetTable (
  PMON_DISC_INFO  UnitTable[UNIT_TYPE_MAX],
  UINT8           ChopType,
  UINT8           PmonConfig
  )
{
  UINT16 BaseOffset;
  UINT8  MaxStateCha;
  BOOLEAN IsIcxLcc;

  if (PmonConfig > KTI_PMON_CONFIG_FULL || PmonConfig == KTI_PMON_CONFIG_DISABLE ||
      IsCpuAndRevision (CPU_ICXD, REV_ALL)) {
    return FALSE;
  }

  MaxStateCha = OobMsmGetPmonChaCountPrivate (ChopType);
  if (MaxStateCha == 0) {
    return FALSE;
  }

  IsIcxLcc = (IsCpuAndRevision (CPU_ICXSP, REV_ALL) && ChopType == TypeLcc);

  BaseOffset = PMON_BASE_OFFSET;

  UnitTable[UNIT_TYPE_PCU].BaseOffset        = BaseOffset;
  UnitTable[UNIT_TYPE_PCU].Stride            = PMON_SINGLE_UNIT_STRIDE;
  UnitTable[UNIT_TYPE_PCU].MaxSupportedUnits = 0;
  BaseOffset += PMON_BLOCK_STRIDE_OFFSET;

  UnitTable[UNIT_TYPE_CHA].BaseOffset        = BaseOffset;
  UnitTable[UNIT_TYPE_CHA].Stride            = PMON_SINGLE_UNIT_STRIDE;
  UnitTable[UNIT_TYPE_CHA].MaxSupportedUnits = 0;
  BaseOffset += MaxStateCha * PMON_BLOCK_STRIDE_OFFSET;

  //
  // UPI and M3UPI units are always together and represent a link
  //
  if (IsCpuAndRevisionOrLater (CPU_ICXSP, REV_L0)) {
    UnitTable[UNIT_TYPE_UPI_LL].BaseOffset            = BaseOffset + PMON_BLOCK_STRIDE_OFFSET;
    UnitTable[UNIT_TYPE_UPI_LL].Stride                = PMON_PER_UPI_LINK_STRIDE;
    UnitTable[UNIT_TYPE_UPI_LL].MaxSupportedUnits     = 0;

    UnitTable[UNIT_TYPE_M3UPI].BaseOffset             = BaseOffset;
    UnitTable[UNIT_TYPE_M3UPI].Stride                 = PMON_PER_UPI_LINK_STRIDE;
    UnitTable[UNIT_TYPE_M3UPI].MaxSupportedUnits      = 0;

    BaseOffset += MAX_STATE_UPI * PMON_BLOCK_STRIDE_OFFSET * PMON_PER_UPI_LINK_STRIDE;
  } else {
    UnitTable[UNIT_TYPE_UPI_LL].BaseOffset            = 0;
    UnitTable[UNIT_TYPE_UPI_LL].Stride                = 0;
    UnitTable[UNIT_TYPE_UPI_LL].MaxSupportedUnits     = 0;

    UnitTable[UNIT_TYPE_M3UPI].BaseOffset             = 0;
    UnitTable[UNIT_TYPE_M3UPI].Stride                 = 0;
    UnitTable[UNIT_TYPE_M3UPI].MaxSupportedUnits      = 0;
  }

  UnitTable[UNIT_TYPE_MC].BaseOffset         = BaseOffset;
  UnitTable[UNIT_TYPE_MC].Stride             = PMON_SINGLE_UNIT_STRIDE;
  UnitTable[UNIT_TYPE_MC].MaxSupportedUnits  = 0;
  BaseOffset += MAX_STATE_MC * PMON_BLOCK_STRIDE_OFFSET;

  UnitTable[UNIT_TYPE_M2M].BaseOffset        = BaseOffset;
  UnitTable[UNIT_TYPE_M2M].Stride            = PMON_SINGLE_UNIT_STRIDE;
  UnitTable[UNIT_TYPE_M2M].MaxSupportedUnits = 0;
  BaseOffset += MAX_STATE_M2M * PMON_BLOCK_STRIDE_OFFSET;

  //
  // M2PCIE, TC, and IRP units are always together and represent a single stack
  //

  UnitTable[UNIT_TYPE_M2PCIE].BaseOffset        = BaseOffset;
  UnitTable[UNIT_TYPE_M2PCIE].Stride            = PMON_PER_STACK_STRIDE;
  UnitTable[UNIT_TYPE_M2PCIE].MaxSupportedUnits = 0;

  UnitTable[UNIT_TYPE_TC].BaseOffset            = BaseOffset + PMON_BLOCK_STRIDE_OFFSET;
  UnitTable[UNIT_TYPE_TC].Stride                = PMON_PER_STACK_STRIDE;
  UnitTable[UNIT_TYPE_TC].MaxSupportedUnits     = 0;

  UnitTable[UNIT_TYPE_IRP].BaseOffset           = BaseOffset + (2 * PMON_BLOCK_STRIDE_OFFSET);
  UnitTable[UNIT_TYPE_IRP].Stride               = PMON_PER_STACK_STRIDE;
  UnitTable[UNIT_TYPE_IRP].MaxSupportedUnits    = 0;

  BaseOffset += ((IsIcxLcc)? MAX_STATE_STACK_LCC : MAX_STATE_STACK) * PMON_BLOCK_STRIDE_OFFSET * PMON_PER_STACK_STRIDE;

  UnitTable[UNIT_TYPE_UBOX].BaseOffset        = BaseOffset;
  UnitTable[UNIT_TYPE_UBOX].Stride            = PMON_SINGLE_UNIT_STRIDE;
  UnitTable[UNIT_TYPE_UBOX].MaxSupportedUnits = 0;
  BaseOffset += PMON_BLOCK_STRIDE_OFFSET;

  UnitTable[UNIT_TYPE_PCIE].BaseOffset        = BaseOffset;
  UnitTable[UNIT_TYPE_PCIE].Stride            = PMON_SINGLE_UNIT_STRIDE;
  UnitTable[UNIT_TYPE_PCIE].MaxSupportedUnits = 0;

  if (PmonConfig >= KTI_PMON_CONFIG_REDUCED) {
    UnitTable[UNIT_TYPE_PCU].MaxSupportedUnits     = MAX_STATE_PCU;
    UnitTable[UNIT_TYPE_CHA].MaxSupportedUnits     = MaxStateCha;
    if (IsCpuAndRevisionOrLater (CPU_ICXSP, REV_L0)) {
      UnitTable[UNIT_TYPE_UPI_LL].MaxSupportedUnits  = MAX_STATE_UPI_LL;
      UnitTable[UNIT_TYPE_M3UPI].MaxSupportedUnits   = MAX_STATE_M3UPI;
    }
    UnitTable[UNIT_TYPE_MC].MaxSupportedUnits      = MAX_STATE_MC;
    UnitTable[UNIT_TYPE_M2M].MaxSupportedUnits     = MAX_STATE_M2M;
    UnitTable[UNIT_TYPE_UBOX].MaxSupportedUnits    = 0;// MAX_STATE_UBOX when supported
    UnitTable[UNIT_TYPE_PCIE].MaxSupportedUnits    = MAX_STATE_PCIE;
  }

  if (PmonConfig > KTI_PMON_CONFIG_REDUCED) {
    UnitTable[UNIT_TYPE_TC].MaxSupportedUnits      = ((IsIcxLcc)? MAX_STATE_STACK_LCC : MAX_STATE_TC);
    UnitTable[UNIT_TYPE_IRP].MaxSupportedUnits     = ((IsIcxLcc)? MAX_STATE_STACK_LCC : MAX_STATE_IRP);
    UnitTable[UNIT_TYPE_M2PCIE].MaxSupportedUnits  = ((IsIcxLcc)? MAX_STATE_STACK_LCC : MAX_STATE_TC);
  }

  return TRUE;
}

/**
  OOB: Get the available pmon count

  @param   ChopType   - The CPU chop type to be used

  @retval             - The number of PMONs available or zero if not supported

**/
UINT32
EFIAPI
OobMsmGetPmonAvailableCount (
  UINT8           ChopType
  )
{
  UINT32 MaxStateUpi, MaxStateM3Upi, MaxStateCha, PmonCount;

  if (IsCpuAndRevision (CPU_ICXD, REV_ALL)) {
    return 0;
  }

  MaxStateCha = OobMsmGetPmonChaCountPrivate (ChopType);
  if (MaxStateCha == 0) {
    return 0;
  }

  if (IsCpuAndRevisionOrLater (CPU_ICXSP, REV_L0)) {
    MaxStateUpi   = MAX_STATE_UPI_LL;
    MaxStateM3Upi = MAX_STATE_M3UPI;
  } else {
    MaxStateUpi   = 0;
    MaxStateM3Upi = 0;
  }

  PmonCount = PMON_MAX_STATE_SUPPORTED + MaxStateCha + MaxStateUpi + MaxStateM3Upi;

  return PmonCount;
}

/**
  OOB: Get the unit ID from the block ID and Unit type

  @param  ChopType    - The CPU chop type to be used
  @param  UnitType    - Type of Unit being accessed
  @param  PmonIndex   - Index of the unit being accessed within the pmon block type
  @param  NewUnit     - Place to store the unit default values

  @retval             - The Pmon index within the unit or PMON_BLOCK_ID_INV

**/
UINT8
EFIAPI
OobMsmNewUnitDefaultValue (
  UINT8     ChopType,
  UNIT_TYPE UnitType,
  UINT8     PmonIndex,
  UNIT_DISCOVERY_STRUCT *NewUnit
  )
{
  UINT32  MaxStateUpi, MaxStateM3Upi, MaxStateCha;

  MaxStateCha = OobMsmGetPmonChaCountPrivate (ChopType);
  if (MaxStateCha == 0) {
    return PMON_BLOCK_ID_INV;
  }

  if (IsCpuAndRevisionOrLater (CPU_ICXSP, REV_L0)) {
    MaxStateUpi   = MAX_STATE_UPI_LL;
    MaxStateM3Upi = MAX_STATE_M3UPI;
  } else {
    MaxStateUpi   = 0;
    MaxStateM3Upi = 0;
  }

  switch (UnitType) {
  case UNIT_TYPE_CHA:
    if (PmonIndex >= MaxStateCha) {
      return PMON_BLOCK_ID_INV;
    }
    *NewUnit   = UnitDiscoveryTableDefault[UNIT_TYPE_CHA];
    NewUnit->BaseOffset    += PmonIndex * PMON_CHA_STRIDE;
    NewUnit->GlobalStatPos += PmonIndex;
    NewUnit->BlockId       += PmonIndex;
    return PmonIndex;
  case UNIT_TYPE_TC:
    if (PmonIndex >= MAX_STATE_TC) {
      return PMON_BLOCK_ID_INV;
    }
    *NewUnit   = UnitDiscoveryTableDefault[UNIT_TYPE_TC];
    NewUnit->BaseOffset    += PmonIndex * PMON_TC_STRIDE + PMON_TC_STRIDE_UPPER(PmonIndex);
    NewUnit->GlobalStatPos += PmonIndex;
    NewUnit->BlockId       += PmonIndex;
    return PmonIndex;
  case UNIT_TYPE_IRP:
    if (PmonIndex >= MAX_STATE_IRP) {
      return PMON_BLOCK_ID_INV;
    }
    *NewUnit   = UnitDiscoveryTableDefault[UNIT_TYPE_IRP];
    NewUnit->BaseOffset    += PmonIndex * PMON_IRP_STRIDE + PMON_IRP_STRIDE_UPPER(PmonIndex);
    NewUnit->GlobalStatPos += PmonIndex;
    NewUnit->BlockId       += PmonIndex;
    return PmonIndex;
  case UNIT_TYPE_M2PCIE:
    if (PmonIndex >= MAX_STATE_M2PCIE) {
      return PMON_BLOCK_ID_INV;
    }
    *NewUnit   = UnitDiscoveryTableDefault[UNIT_TYPE_M2PCIE];
    NewUnit->BaseOffset    += PmonIndex * PMON_M2PCIE_STRIDE + PMON_M2PCIE_STRIDE_UPPER(PmonIndex);
    NewUnit->GlobalStatPos += PmonIndex;
    NewUnit->BlockId       += PmonIndex;
    return PmonIndex;
  case UNIT_TYPE_PCU:
    if (PmonIndex >= MAX_STATE_PCU) {
      return PMON_BLOCK_ID_INV;
    }
    *NewUnit   = UnitDiscoveryTableDefault[UNIT_TYPE_PCU];
    NewUnit->GlobalStatPos += PmonIndex;
    NewUnit->BlockId       += PmonIndex;
    return PmonIndex;
  case UNIT_TYPE_UBOX:
    if (PmonIndex >= MAX_STATE_UBOX) {
      return PMON_BLOCK_ID_INV;
    }
    *NewUnit   = UnitDiscoveryTableDefault[UNIT_TYPE_UBOX];
    NewUnit->GlobalStatPos += PmonIndex;
    NewUnit->BlockId       += PmonIndex;
    return PmonIndex;
  case UNIT_TYPE_MC:
    if (PmonIndex >= MAX_STATE_MC) {
      return PMON_BLOCK_ID_INV;
    }
    *NewUnit   = UnitDiscoveryTableDefault[UNIT_TYPE_MC];
    NewUnit->GlobalStatPos += PmonIndex;
    NewUnit->BlockId       += PmonIndex;
    return PmonIndex;
  case UNIT_TYPE_M2M:
    if (PmonIndex >= MAX_STATE_M2M) {
      return PMON_BLOCK_ID_INV;
    }
    *NewUnit   = UnitDiscoveryTableDefault[UNIT_TYPE_M2M];
    NewUnit->GlobalStatPos += PmonIndex;
    NewUnit->BlockId       += PmonIndex;
    return PmonIndex;
  case UNIT_TYPE_UPI_LL:
    if (PmonIndex >= MaxStateUpi) {
      return PMON_BLOCK_ID_INV;
    }
    *NewUnit   = UnitDiscoveryTableDefault[UNIT_TYPE_UPI_LL];
    NewUnit->GlobalStatPos += PmonIndex;
    NewUnit->BlockId       += PmonIndex;
    return PmonIndex;
  case UNIT_TYPE_M3UPI:
    if (PmonIndex >= MaxStateM3Upi) {
      return PMON_BLOCK_ID_INV;
    }
    *NewUnit   = UnitDiscoveryTableDefault[UNIT_TYPE_M3UPI];
    NewUnit->GlobalStatPos += PmonIndex;
    NewUnit->BlockId       += PmonIndex;
    return PmonIndex;
  case UNIT_TYPE_PCIE:
    if (PmonIndex >= MAX_STATE_PCIE) {
      return PMON_BLOCK_ID_INV;
    }
    *NewUnit   = UnitDiscoveryTableDefault[UNIT_TYPE_PCIE];
    NewUnit->GlobalStatPos += PmonIndex;
    NewUnit->BlockId       += PmonIndex;
    return PmonIndex;
  }

  return PMON_BLOCK_ID_INV;
}

/**
  OOB: Get the pmon global default register value based on CPU type

  @param   ChopType             - The CPU chop type to be used
  @param   GlobalDiscoveryState - Variable used to store the default value

  @retval            - True if pmon table was supported

**/
BOOLEAN
EFIAPI
OobMsmGetPmonGlobalDiscDefault (
  UINT8           ChopType,
  GLOBAL_DISCOVERY_STATE *GlobalDiscoveryState
  )
{
  if (GlobalDiscoveryState == NULL) {
    return FALSE;
  }

  GlobalDiscoveryState->Fields.Type        = UNIT_TYPE_UBOX;
  GlobalDiscoveryState->Fields.AccessType  = PMON_ACCESS_MSR;
  GlobalDiscoveryState->Fields.BlockStride = PMON_BLOCK_STRIDE;

  GlobalDiscoveryState->Fields.MaxBlocks          = OobMsmGetPmonAvailableCount (ChopType);
  GlobalDiscoveryState->Fields.GlobalControlAddr  = PMON_GLBL_CTR_ADDR;
  GlobalDiscoveryState->Fields.GblCtrStatAddr     = PMON_GLBL_STAT_CTR_ADDR;
  GlobalDiscoveryState->Fields.NumBlockStatusBits = PMON_NUM_BLOCK_STS_BITS;

  return TRUE;
}

/**
  OOB: Get the pmon unit offset

  @param  ChopType    - The CPU chop type to be used
  @param  PmonIndex   - Index of the unit being accessed within the pmon block type
  @param  UnitType    - The unit type being accessed
  @param  Unit        - Index of the unit being accessed within the pmon block type

  @retval             - The Pmon offset

**/
UINT32
EFIAPI
OobMsmGetPmonOffset (
  UINT8          ChopType,
  UINT8          PmonIndex,
  UNIT_TYPE      UnitType,
  PMON_DISC_INFO Unit
  )
{
  UINT32   Offset;
  BOOLEAN  IsIcxLcc;

  if (Unit.BaseOffset == 0 || Unit.MaxSupportedUnits == 0) {
    return 0;
  }

  if (UnitType == UNIT_TYPE_TC || UnitType == UNIT_TYPE_IRP || UnitType == UNIT_TYPE_M2PCIE) {
    //
    // The UBOX and the last stack are reversed, so the index is updated
    //
    IsIcxLcc = (IsCpuAndRevision (CPU_ICXSP, REV_ALL) && ChopType == TypeLcc);
    if (PmonIndex == 0) {
      PmonIndex = ((IsIcxLcc)? MAX_STATE_STACK_LCC : MAX_STATE_TC) - 1;
    } else if (PmonIndex == (((IsIcxLcc)? MAX_STATE_STACK_LCC : MAX_STATE_TC) - 1)) {
      PmonIndex = 0;
    }
  }

  Offset = Unit.BaseOffset + (PmonIndex * Unit.Stride * PMON_BLOCK_STRIDE_OFFSET);

  return Offset;
}