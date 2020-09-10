/** @file

  DefaultWarnings.c

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

#include <SysHost.h>

#include <Library/MemoryServicesLib.h>
#include <Library/EnhancedWarningLogLib.h>
#include <Library/SysHostPointerLib.h>

/**

  Used to add warnings to the promote warning exception list

  @param[in] Host  - pointer to sysHost

  @retval None

**/
UINT32
MemInitDefaultWarnings (
  IN PSYSHOST Host
  )
{
  SYS_SETUP *Setup;

  Setup = GetSysSetupPointer ();

  SetPromoteWarningException (WARN_USER_DIMM_DISABLE, WARN_MINOR_WILDCARD);
  SetPromoteWarningException (WARN_MEM_LIMIT, WARN_MINOR_WILDCARD);
  SetPromoteWarningException (WARN_CLTT_DISABLE, WARN_MINOR_WILDCARD);
  SetPromoteWarningException (WARN_CLTT_DIMM_UNKNOWN, WARN_MINOR_WILDCARD);
  SetPromoteWarningException (WARN_CLOSED_PAGE_OVERRIDE, WARN_MINOR_WILDCARD);
  SetPromoteWarningException (WARN_DIMM_VREF_NOT_PRESENT, WARN_MINOR_WILDCARD);
  SetPromoteWarningException (WARN_DIMM_COMPAT, WARN_CHANNEL_MIX_ECC_NONECC);
  SetPromoteWarningException (WARN_PPR_FAILED, WARN_MINOR_WILDCARD);
  SetPromoteWarningException (WARN_MIRROR_DISABLE, WARN_MIRROR_DISABLE_MINOR_RAS_DISABLED);
  SetPromoteWarningException (WARN_PMIRROR_DISABLE, WARN_PMIRROR_DISABLE_MINOR_RAS_DISABLED);
  SetPromoteWarningException (WARN_SPARE_DISABLE, WARN_SPARE_DISABLE_MINOR_RK_SPARE);
  SetPromoteWarningException (WARN_LOCKSTEP_DISABLE, WARN_LOCKSTEP_DISABLE_MINOR_RAS_MODE);
  SetPromoteWarningException (WARN_PTRLSCRB_DISABLE, WARN_PTRLSCRB_MINOR_DISABLE);
  SetPromoteWarningException (WARN_ADDDC_DISABLE, WARN_ADDDC_MINOR_DISABLE);
  SetPromoteWarningException (WARN_SDDC_DISABLE, WARN_SDDC_MINOR_DISABLE);
  SetPromoteWarningException (WARN_DIMM_COMPAT, WARN_DIMM_SKU_MISMATCH);
  SetPromoteWarningException (WARN_INTERLEAVE_FAILURE, WARN_MINOR_WILDCARD);
  if(Setup->mem.NvmMediaStatusException) {
    SetPromoteWarningException (WARN_NVMCTRL_MEDIA_STATUS, WARN_MINOR_WILDCARD);
  }
  SetPromoteWarningException (WARN_DIMM_COMPAT, WARN_DIMM_SPEED_NOT_SUP);
  SetPromoteWarningException (WARN_NVDIMM_SURPRISE_CLOCK_STOP, WARN_POWER_CYCLE_POLICY_APPLY);
  SetPromoteWarningException (WARN_THROT_INSUFFICIENT, WARN_2X_REFRESH_TEMPLO_DISABLED);
  SetPromoteWarningException (WARN_THROT_INSUFFICIENT, WARN_CUSTOM_REFRESH_RATE_REVERTED);
  SetPromoteWarningException (WARN_FNV_BSR, WARN_MEDIA_READY_ERROR);

  SetPromoteWarningException (WARN_REGISTER_OVERFLOW, WARN_MINOR_REGISTER_UNDERFLOW);  // MRC IP related (register underflow)
  SetPromoteWarningException (WARN_REGISTER_OVERFLOW, WARN_MINOR_REGISTER_OVERFLOW);   // MRC IP related (register overflow)
  SetPromoteWarningException (WARN_NO_MEMORY, WARN_NO_MEMORY_MINOR_NO_MEMORY);         // MRC IP related (no memory)
  SetPromoteWarningException (WARN_DIMM_POP_RUL, WARN_DIMM_POP_RUL_AEP_VDD_CHANGED);
  SetPromoteWarningException (ERR_SKU_LIMIT, ERR_SKU_LIMIT_VIOLATION);
  SetPromoteWarningException (WARN_MEMORY_BOOT_HEALTH_CHECK, WARN_MEMORY_BOOT_HEALTH_CHECK_MASK_FAIL);// MRC BOOT HEALTH CHECK. (Warning for margin check)
  SetPromoteWarningException (WARN_MEMORY_BOOT_HEALTH_CHECK, WARN_MEMORY_BOOT_HEALTH_CHECK_CHANNEL_MAP_OUT);// MRC BOOT HEALTH CHECK. (Exclude channel mapout)
  SetPromoteWarningException (WARN_MEMORY_POWER_MANAGEMENT, WARN_MEMORY_PM_WEIGHT_TABLE_NOT_FOUND);  // Memory Power Management - Dimm Entry not found in Weight Table, using Defaults
  SetPromoteWarningException (WARN_MEMORY_POWER_MANAGEMENT, WARN_MEMORY_PM_THERMAL_TABLE_NOT_FOUND); // Memory Power Management - Dimm Entry not found in Throttling Table, using Defaults
  SetPromoteWarningException (WARN_DIMM_COMPAT, WARN_NO_DDR4_ON_SOCKET);
  SetPromoteWarningException (WARN_DIMM_COMPAT, WARN_NO_DDR4_ON_S0C0D0);

  //
  // Add BIOS partition errors to exceptions list to allow user-triggered recovery
  //
  SetPromoteWarningException (WARN_BIOS_CONFIG_HEADER_STATUS, WARN_BIOS_CONFIG_HEADER_CHECKSUM_MISMATCH);
  SetPromoteWarningException (WARN_BIOS_CONFIG_HEADER_STATUS, WARN_BIOS_CONFIG_HEADER_REVISION_MISMATCH);
  SetPromoteWarningException (WARN_BIOS_CONFIG_HEADER_STATUS, WARN_BIOS_CONFIG_HEADER_SIGNATURE_MISMATCH);
  SetPromoteWarningException (WARN_CFGCUR_STATUS, WARN_CFGCUR_SIGNATURE_MISMATCH);
  SetPromoteWarningException (WARN_CFGCUR_STATUS, WARN_CFGCUR_CHECKSUM_MISMATCH);
  SetPromoteWarningException (WARN_CFGCUR_STATUS, WARN_CFGCUR_REVISION_MISMATCH);
  SetPromoteWarningException (WARN_CFGCUR_STATUS, WARN_CFGCUR_DATASIZE_EXCEEDED);
  SetPromoteWarningException (WARN_CFGCUR_STATUS, WARN_CFGCUR_DATASIZE_OFFSET_EXCEEDED);

  //
  // Add OS partition errors to exceptions list to allow user-triggered recovery
  // (system will boot up and BIOS will uses BIOS partition CCUR record)
  //
  SetPromoteWarningException (WARN_OS_CONFIG_HEADER_STATUS, WARN_OS_CONFIG_HEADER_CHECKSUM_MISMATCH);
  SetPromoteWarningException (WARN_OS_CONFIG_HEADER_STATUS, WARN_OS_CONFIG_HEADER_REVISION_MISMATCH);
  SetPromoteWarningException (WARN_OS_CONFIG_HEADER_STATUS, WARN_OS_CONFIG_HEADER_SIGNATURE_MISMATCH);
  SetPromoteWarningException (WARN_OS_CONFIG_HEADER_STATUS, WARN_OS_CONFIG_HEADER_LENGTH_MISMATCH);
  SetPromoteWarningException (WARN_CFGIN_STATUS, WARN_CFGIN_SIGNATURE_MISMATCH);
  SetPromoteWarningException (WARN_CFGIN_STATUS, WARN_CFGIN_CHECKSUM_MISMATCH);
  SetPromoteWarningException (WARN_CFGIN_STATUS, WARN_CFGIN_REVISION_MISMATCH);
  SetPromoteWarningException (WARN_CFGIN_STATUS, WARN_CFGIN_DATASIZE_EXCEEDED);
  SetPromoteWarningException (WARN_CFGOUT_STATUS, WARN_CFGOUT_SIGNATURE_MISMATCH);
  SetPromoteWarningException (WARN_CFGOUT_STATUS, WARN_CFGOUT_CHECKSUM_MISMATCH);
  SetPromoteWarningException (WARN_CFGOUT_STATUS, WARN_CFGOUT_REVISION_MISMATCH);
  SetPromoteWarningException (WARN_CFGOUT_STATUS, WARN_CFGOUT_DATASIZE_EXCEEDED);

  SetPromoteWarningException (WARN_DIMM_COMPAT, WARN_DIMM_NOT_IN_DDRT_POR_DDR_TABLE);
  SetPromoteWarningException (WARN_DIMM_COMPAT, WARN_DIMM_IN_DDRT_POR_DDR_TABLE_NOT_VALIDATED);
  SetPromoteWarningException (WARN_DIMM_POP_RUL, WARN_DIMM_POP_RUL_MINOR_NO_DDRT_ON_MC0);
  SetPromoteWarningException (WARN_DIMM_POP_RUL, WARN_DIMM_POP_RUL_MINOR_NO_DDRT_ON_SOCKET0);
  SetPromoteWarningException (WARN_DIMM_POP_RUL, WARN_DIMM_POP_RUL_DDR4_CAP_NOT_POR_FOR_MEM_POP);
  SetPromoteWarningException (WARN_DIMM_POP_RUL, WARN_DIMM_POP_RUL_DDR4_TYPE_NOT_POR_FOR_MM);
  SetPromoteWarningException (WARN_DIMM_POP_RUL, WARN_DIMM_POP_RUL_MEM_TOPOLOGY_NOT_SYMMETRICAL);
  SetPromoteWarningException (WARN_DIMM_POP_RUL, WARN_DIMM_POP_RUL_NM_FM_RATIO_VIOLATION);
  SetPromoteWarningException (WARN_DIMM_POP_RUL, WARN_DIMM_POP_RUL_2LM_IMC_MEM_MISMATCH);
  SetPromoteWarningException (WARN_DIMM_POP_RUL, WARN_DIMM_POP_RUL_DDRT_CAPACITY_MISMATCH);
  SetPromoteWarningException (WARN_DIMM_POP_RUL, WARN_DIMM_POP_RUL_DDRT_PARTITION_MISMATCH);
  SetPromoteWarningException (WARN_DIMM_POP_RUL, WARN_DIMM_POP_RUL_SOCKET_MODE_MISMATCH);
  SetPromoteWarningException (WARN_DIMM_POP_RUL, WARN_DIMM_POP_RUL_DDR_CAPACITY_MISMATCH);
  SetPromoteWarningException (WARN_DIMM_POP_RUL, WARN_DIMM_POP_RUL_POPULATION_POR_MISMATCH);
  SetPromoteWarningException (WARN_DIMM_POP_RUL, WARN_DIMM_POP_RUL_MEM_POP_POR_TBL_INVALID);
  SetPromoteWarningException (WARN_DIMM_POP_RUL, WARN_DIMM_POP_RUL_2LM_FM_CH_NOT_PWR_OF_TWO);
  SetPromoteWarningException (WARN_DIMM_POP_RUL, WARN_DIMM_POP_RUL_PMEM_X1_POPULATION_INVALID);
  SetPromoteWarningException (WARN_USER_DIMM_DISABLE, WARN_USER_DIMM_DISABLE_RANK_DISABLED);
  SetPromoteWarningException (WARN_USER_DIMM_DISABLE, WARN_USER_DIMM_DISABLE_MAPPED_OUT);
  SetPromoteWarningException (WARN_SILICON_CAPABILITY_LIMITATION, WARN_TME_ENABLED_CRYSTAL_RIDGE_NOT_SUPPORTED);
  SetPromoteWarningException (WARN_SILICON_CAPABILITY_LIMITATION, WARN_SGX_ENABLED_CRYSTAL_RIDGE_NOT_SUPPORTED);
  SetPromoteWarningException (WARN_MCA_UNCORRECTABLE_ERROR, WARN_PREVIOUS_BOOT_MCA_MINOR_CODE);
  SetPromoteWarningException (WARN_DIMM_POP_RUL, WARN_DIMM_POP_RUL_NO_DIMM_ON_SOCKET);

  SetPromoteWarningException (WARN_DIMM_MEM_MODE, WARN_DIMM_MEM_MODE_NEW_DIMM_2LM_NOT_SUPPORTED);
  SetPromoteWarningException (WARN_DIMM_MEM_MODE, WARN_DIMM_MEM_MODE_2LM_NOT_SUPPORTED);

  //
  // Check if not supported DCPMM warning should be added to exception list
  //
  if(Setup->mem.AepNotSupportedException) {
    SetPromoteWarningException (WARN_DIMM_COMPAT, WARN_DDRT_DIMM_NOT_SUPPORTED);
  }
  return SUCCESS;

} // MemInitDefaultWarnings
