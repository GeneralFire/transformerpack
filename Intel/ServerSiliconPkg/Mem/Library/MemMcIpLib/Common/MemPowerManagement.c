/** @file
  Mem Power Managment.

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

#include <SysHostChip.h>
#include <Chip/Include/MemHostChip.h>
#include "MemHostChipCommon.h"
#include <Library/UsraCsrLib.h>

/**
  Turn off DDRIO data DLL when MemSS in CKE Power down (Auto Power Down or
  Pre-Charge Power Down) or Opp Self Refresh mode.

  @param[in] Host    - Pointer to sysHost
  @param[in] Socket  - Socket number
  @param[in] Channel - Memory channel number

  @retval N/A

**/
VOID
PmLpModeDataDllOff (
  IN PSYSHOST Host,
  IN UINT8    Socket,
  IN UINT8    Channel
  )
{

  return;
}

/**
  Program odt_value to HIGH in certain configurations to save power.

  @param[in] Socket     - Socket number
  @param[in] Ch         - Channel number

  @retval    EFI_SUCCESS If odt_value is successfully set.
**/
EFI_STATUS
EFIAPI
SetPowerSaveOdt (
  IN    UINT8     Socket,
  IN    UINT8     Ch
  )
{
  return EFI_SUCCESS;
}

/**

  Program DRAM RAPL Values such as TDP, Min power and Max Power

  @param[in] Socket        - Socket Number, 0 based
  @param[in] DramTpd       - The Spec power allowed for DRAM. The TDP setting
                             is typical (not guaranteed). The units for this
                             value are defined in Watt.
  @param[in] DramMinPwr    - The minimal power setting allowed for DRAM.
                             Lower values will be clamped to this value. The
                             minimum setting is typical (not guaranteed). The
                             units for this value are defined in Watt.
  @param[in] DramMaxPwr    - The maximal power setting allowed for DRAM.
                             Higher values will be clamped to this value. The
                             maximum setting is typical (not guaranteed). The
                             units for this value are defined in Watt.

  @retval N/A

**/
VOID
EFIAPI
ProgramDramRaplValues (
  IN UINT8  Socket,
  IN UINT16 DramTdp,
  IN UINT16 DramMinPwr,
  IN UINT16 DramMaxPwr
  )
{
  DRAM_POWER_INFO_N0_PCU_FUN2_STRUCT  DramPowerInfoLow;
  DRAM_POWER_INFO_N1_PCU_FUN2_STRUCT  DramPowerInfoHigh;

  DramPowerInfoLow.Data = UsraCsrRead (Socket, 0, DRAM_POWER_INFO_N0_PCU_FUN2_REG);
  DramPowerInfoLow.Bits.dram_tdp = DramTdp;
  DramPowerInfoLow.Bits.dram_min_pwr = DramMinPwr;
  UsraCsrWrite (Socket, 0, DRAM_POWER_INFO_N0_PCU_FUN2_REG, DramPowerInfoLow.Data);

  DramPowerInfoHigh.Data = UsraCsrRead (Socket, 0, DRAM_POWER_INFO_N1_PCU_FUN2_REG);
  DramPowerInfoHigh.Bits.dram_max_pwr = DramMaxPwr;
  UsraCsrWrite (Socket, 0, DRAM_POWER_INFO_N1_PCU_FUN2_REG, DramPowerInfoHigh.Data);
}
