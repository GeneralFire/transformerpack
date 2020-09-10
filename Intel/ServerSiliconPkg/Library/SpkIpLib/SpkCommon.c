/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019-2020 Intel Corporation. <BR>

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
#include <Guid/SpkIpInterface.h>
#include <Library/EmulationConfigurationLib.h>
#include <Library/IoLib.h>
#include <Library/CsrPseudoOffsetConvertLib.h>

#define SPK_TOTAL_CLUSTERS_REGS  5

UINT32 SpkSncBaseRegisterOffset[SPK_TOTAL_CLUSTERS_REGS] = {SPK_SNC_BASE_1_SPK_MAIN_REG,
                                                            SPK_SNC_BASE_2_SPK_MAIN_REG,
                                                            SPK_SNC_BASE_3_SPK_MAIN_REG,
                                                            SPK_SNC_BASE_4_SPK_MAIN_REG,
                                                            SPK_SNC_BASE_5_SPK_MAIN_REG};


/**
  MMIO Write wrapper to print assembly code for emulation

  @param MmioBar         - The MemBar0 of SPK device.
  @param PciBarOffset    - The BAR offset for each SPK Instance.
  @param Reg             - The register to be accessed.
  @param Value           - The value to be programmed into the register.

  @return Value.
**/
UINT32
EFIAPI
SpkMmioWrite32 (
  IN UINT32 MmioBar,
  IN UINT32 PciBarOffset,
  IN UINT32 Reg,
  IN UINT32 Value
  )
{
  UINTN       Address;
  CSR_OFFSET  RegOffset;

  RegOffset.Data = Reg;

  if(RegOffset.Bits.pseudo) {
    RegOffset.Data = GetPseudoRegisterOffset (RegOffset);
  }

  //
  // Each SPK instance has 4KB MMIO range, so the register offset should be 12bits width.
  // That means it cannot directly take RegOffset.Bits.offset(16bits width) as register offset.
  //
  Address = (MmioBar + PciBarOffset + (RegOffset.Bits.offset & 0xFFF));

  if (UbiosGenerationEnabled ()) {
    DEBUG ((EFI_D_ERROR, "\n  mov DWORD PTR ds:[0%08xh], 0%08xh\n", Address, Value));
  }
  return MmioWrite32 (Address, Value);
}

/**
  MMIO Write wrapper to print assembly code for emulation

  @param MmioBar         - The MemBar0 of SPK device.
  @param PciBarOffset    - The BAR offset for each SPK Instance.
  @param Reg             - The register to be accessed.
  @param Value           - The value to be programmed into the register.

  @return Value.
**/
UINT16
EFIAPI
SpkMmioWrite16 (
  IN UINT32 MmioBar,
  IN UINT32 PciBarOffset,
  IN UINT32 Reg,
  IN UINT16 Value
  )
{
  UINTN       Address;
  CSR_OFFSET  RegOffset;

  RegOffset.Data = Reg;

  if(RegOffset.Bits.pseudo) {
    RegOffset.Data = GetPseudoRegisterOffset (RegOffset);
  }

  //
  // Each SPK instance has 4KB MMIO range, so the register offset should be 12bits width.
  // That means it cannot directly take RegOffset.Bits.offset(16bits width) as register offset.
  //
  Address = (MmioBar + PciBarOffset + (RegOffset.Bits.offset & 0xFFF));

  if (UbiosGenerationEnabled ()) {
    DEBUG ((EFI_D_ERROR, "\n  mov WORD PTR ds:[0%08xh], 0%04xh\n", Address, Value));
  }

  return MmioWrite16(Address, Value);
}

/**
  SPK:  Set the starting address using and index

  @param MmioBar      - MMIO BAR
  @param PciBarOffset - PCI BAR offset
  @param ClusterIdx   - The cluster index
  @param ClMemBase    - The cluster memory base address

  @retval             - EFI_UNSUPPORTED: the function not implemented
                        EFI_INVALID_PARAMETER   A required parameter was NULL or is out of range.
                        EFI_SUCCESS: the function is excuted successfully

**/

EFI_STATUS
EFIAPI
SpkSetStartingAddr (
  UINT32 MmioBar,
  UINT32 PciBarOffset,
  UINT8  ClusterIdx,
  UINT32 ClMemBase
  )
{

  if (ClusterIdx >= SPK_TOTAL_CLUSTERS_REGS) {
    DEBUG ((DEBUG_ERROR, "SPK: ERROR!! out of bounds access detected\n"));
    ASSERT (ClusterIdx < SPK_TOTAL_CLUSTERS_REGS);
    return EFI_INVALID_PARAMETER;
  }

  SpkMmioWrite32 (MmioBar, PciBarOffset, SpkSncBaseRegisterOffset[ClusterIdx], ClMemBase);

  return EFI_SUCCESS;
}

/**
  SPK:  Set SNC upper base configuration

  @param MmioBar       - MMIO BAR
  @param PciBarOffset  - PCI BAR offset
  @param SncUpperBase  - SNC upper base configuration

  @retval              - EFI_UNSUPPORTED: the function not implemented
                         EFI_SUCCESS: the function is excuted successfully

**/

EFI_STATUS
EFIAPI
SpkSetSncUpperBase (
  UINT32 MmioBar,
  UINT32 PciBarOffset,
  UINT32 SncUpperBase
  )
{
  SpkMmioWrite32 (MmioBar, PciBarOffset, SPK_SNC_UPPER_BASE_SPK_MAIN_REG, SncUpperBase);

  return EFI_SUCCESS;
}

/**
  SPK:  Set Snc

  @param MmioBar      - MMIO BAR
  @param PciBarOffset - PCI BAR offset
  @param SncConfig    - SNC configuration value

  @retval             - EFI_UNSUPPORTED: the function not implemented
                        EFI_SUCCESS: the function is excuted successfully

**/

EFI_STATUS
EFIAPI
SpkSetSncCfg (
  UINT32 MmioBar,
  UINT32 PciBarOffset,
  UINT32 SncConfig
  )
{
  SpkMmioWrite32 (MmioBar, PciBarOffset, SPK_SNC_CONFIG_SPK_MAIN_REG, SncConfig);
  return EFI_SUCCESS;
}

/**
  SPK:  Set Snc uncore configuration

  @param MmioBar          - MMIO BAR
  @param PciBarOffset     - PCI BAR offset
  @param NumChaPerCluster - Number of CHAs per SNC Cluster
  @param BaseChaCluster1  - Base of CHA Cluster 1
  @param BaseChaCluster2  - Base of CHA Cluster 2
  @param BaseChaCluster3  - Base of CHA Cluster 3

  @retval                 - EFI_UNSUPPORTED: the function not implemented
                            EFI_SUCCESS: the function is excuted successfully

**/
EFI_STATUS
EFIAPI
SpkSetSncUncoreCfg (
  UINT32 MmioBar,
  UINT32 PciBarOffset,
  UINT8  NumChaPerCluster,
  UINT8  BaseChaCluster1,
  UINT8  BaseChaCluster2,
  UINT8  BaseChaCluster3
  )
{
  SPK_UNCORE_SNC_CONFIG_SPK_MAIN_STRUCT    SpkSncUncore;

  SpkSncUncore.Data = 0;
  SpkSncUncore.Bits.num_cha_per_cluster = NumChaPerCluster;
  SpkSncUncore.Bits.base_cha_cluster_1  = BaseChaCluster1;
  SpkSncUncore.Bits.base_cha_cluster_2  = BaseChaCluster2;
  SpkSncUncore.Bits.base_cha_cluster_3  = BaseChaCluster3;

  SpkMmioWrite32 (MmioBar, PciBarOffset, SPK_UNCORE_SNC_CONFIG_SPK_MAIN_REG, SpkSncUncore.Data);
  return EFI_SUCCESS;
}