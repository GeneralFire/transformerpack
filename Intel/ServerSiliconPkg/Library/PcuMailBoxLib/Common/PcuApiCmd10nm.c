/** @file
  Implementation of the Bios to Pcode Mailbox library for build for 10nm

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2018 - 2020 Intel Corporation. <BR>

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

#include <Library/MemoryCoreLib.h>
#include <RcRegs.h>
#include <Library/TimerLib.h>
#include <Library/DebugLib.h>
#include <Library/EmulationConfigurationLib.h>
#include <Library/UpiHostDataLib.h>
#include <Library/PcuMailBoxLib.h>
#include <Library/ProcMemInitChipLib.h>
#include <Library/UsraCsrLib.h>
#include <Library/CsrAccessLib.h>
#include <Library/MemoryServicesLib.h>
#include "Chip10nm/Include/MemHostChip10nm.h"
#include <Library/SysHostPointerLib.h>
#include "VrSerialVid.h"

#define PCU_MISC_SURPRISE_RESET BIT11


#define DRAM_THERMAL_DATA_MC_ENABLE_MASK (MAILBOX_BIOS_CMD_ENABLE_DRAM_THERMAL_DATA_MC_ENABLE_MASK_MC_0_MASK | MAILBOX_BIOS_CMD_ENABLE_DRAM_THERMAL_DATA_MC_ENABLE_MASK_MC_1_MASK | \
  MAILBOX_BIOS_CMD_ENABLE_DRAM_THERMAL_DATA_MC_ENABLE_MASK_MC_2_MASK | MAILBOX_BIOS_CMD_ENABLE_DRAM_THERMAL_DATA_MC_ENABLE_MASK_MC_3_MASK)

#define DRAM_THERMAL_DATA_MC_DISABLE_MASK (MAILBOX_BIOS_CMD_DISABLE_DRAM_THERMAL_DATA_MC_DISABLE_MASK_MC_0_MASK | MAILBOX_BIOS_CMD_DISABLE_DRAM_THERMAL_DATA_MC_DISABLE_MASK_MC_1_MASK | \
  MAILBOX_BIOS_CMD_DISABLE_DRAM_THERMAL_DATA_MC_DISABLE_MASK_MC_2_MASK | MAILBOX_BIOS_CMD_DISABLE_DRAM_THERMAL_DATA_MC_DISABLE_MASK_MC_3_MASK)

#define MAILBOX_BIOS_CMD_WRITE_PCU_MISC_CONFIG_DATA_SMBUS_ACCESS_LSB  28
#define MAILBOX_BIOS_CMD_WRITE_PCU_MISC_CONFIG_DATA_SMBUS_UNLOCK_MASK (1 << MAILBOX_BIOS_CMD_WRITE_PCU_MISC_CONFIG_DATA_SMBUS_ACCESS_LSB)
#define MAILBOX_BIOS_CMD_WRITE_PCU_MISC_CONFIG_DATA_SMM_SAI_WA        0xd

STATIC CHAR8 PCU_MBOX_ERROR_MESSAGE[] = "PCU: API - Command->0x%02X, Error Code -> 0x%02X\n";
/**
  Set Core or Mesh Ratio in PCU

  @param[in]  Socket        0 based Socket number
  @param[in]  CoreRatio     Core Ratio value
  @param[in]  MeshRatio     Mesh Ratio Value

  @retval EFI_SUCCESS    Function execution successful
  @retval !EFI_SUCCESS   Error on command execution
**/
EFI_STATUS
EFIAPI
PcuSetCoreMeshRatio (
  IN UINT8 Socket,
  IN UINT8 CoreRatio,
  IN UINT8 MeshRatio
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  UINT32 PcodeMailBoxStatus;
  UINT32 PcuMailboxData;

  PcuMailboxData = (MeshRatio << MAILBOX_BIOS_CMD_SET_CORE_MESH_RATIO_DATA_MESH_RATIO_LSB)|CoreRatio;
  PcodeMailBoxStatus = ReadModifyWritePcuMailbox (
                         Socket,
                         MAILBOX_BIOS_CMD_ZERO,
                         MAILBOX_BIOS_CMD_SET_CORE_MESH_RATIO,
                         &PcuMailboxData,
                         0,
                         0
                         );
  if (PcodeMailBoxStatus != MAILBOX_BIOS_CC_NO_ERROR) {
    RcDebugPrint (SDBG_DEFAULT, PCU_MBOX_ERROR_MESSAGE, MAILBOX_BIOS_CMD_SET_CORE_MESH_RATIO, PcodeMailBoxStatus);
    Status = EFI_DEVICE_ERROR;
  }
  return Status;
}

/*
  Function will do a Bios 2 Pcu mailbox write for adding SMM into WAC of BIOS_W policy group

  @param  [in]        Socket,

  @retval EFI_STATUS  Status  The status of the Bios 2 Pcode call
*/
EFI_STATUS
EFIAPI
PcuSetSmmSaiPolicyWa (
  UINT8  Socket
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  UINT32 PcodeMailBoxStatus;
  UINT32 PcuMailboxData;

  PcuMailboxData = (1 << MAILBOX_BIOS_CMD_WRITE_PCU_MISC_CONFIG_DATA_SMM_SAI_WA);
  PcodeMailBoxStatus = ReadModifyWritePcuMailbox (
                         Socket,
                         MAILBOX_BIOS_CMD_ZERO,
                         MAILBOX_BIOS_CMD_WRITE_PCU_MISC_CONFIG,
                         &PcuMailboxData,
                         0,
                         0
                         );
  if (PcodeMailBoxStatus != MAILBOX_BIOS_CC_NO_ERROR) {
    RcDebugPrint (SDBG_DEFAULT, PCU_MBOX_ERROR_MESSAGE, MAILBOX_BIOS_CMD_WRITE_PCU_MISC_CONFIG, PcodeMailBoxStatus);
    Status = EFI_DEVICE_ERROR;
  }
  return Status;
}

/**
  Set Pmax disable in PCU

  @param[in]  Socket        0 based Socket number

  @retval EFI_SUCCESS    Function execution successful
  @retval !EFI_SUCCESS   Error on command execution
**/
EFI_STATUS
EFIAPI
PcuSetPmaxDisable (
  IN UINT8 Socket
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  UINT32 TempData = 0;
  UINT32 PcodeMailboxStatus;

  PcodeMailboxStatus = ReadModifyWritePcuMailbox (
                         Socket,
                         MAILBOX_BIOS_CMD_READ_PCU_MISC_CONFIG,
                         MAILBOX_BIOS_CMD_WRITE_PCU_MISC_CONFIG,
                         &TempData,
                         ~(UINT32)MAILBOX_BIOS_CMD_WRITE_PCU_MISC_CONFIG_DATA_PMAX_DISABLE_MASK,
                         (1 << MAILBOX_BIOS_CMD_WRITE_PCU_MISC_CONFIG_DATA_PMAX_DISABLE_LSB)
                         );
  if (PcodeMailboxStatus != MAILBOX_BIOS_CC_NO_ERROR) {
    RcDebugPrint (SDBG_DEFAULT, "PCU: API -> PcuSetPmaxDisable, Error Code -> %x\n", PcodeMailboxStatus);
    Status = EFI_DEVICE_ERROR;
  }

  return Status;
}

/**
  Enable DRAM Power Management in PCU

  @param[in]  Socket        0 based Socket number
  @param[in]  ImcMask       ImcMask to enable DRAM Power Management
                            Bit 0 => McId0, Bit 1 => McId1 and so on
                            If bit is set, corresponding power management is enabled

  @retval EFI_SUCCESS    Function execution successful
  @retval !EFI_SUCCESS   Error on command execution
**/
EFI_STATUS
EFIAPI
PcuEnableDramPm (
  IN UINT8 Socket,
  IN UINT8 ImcMask
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  UINT32 PcodeMailboxStatus = 0;
  UINT32 TempData = 0;

  //
  // Check ImcMask is covered by the SOC allowed Enable mask
  //
  if ((ImcMask & DRAM_THERMAL_DATA_MC_ENABLE_MASK) != ImcMask) {
    return EFI_INVALID_PARAMETER;
  }

  TempData = ImcMask;
  PcodeMailboxStatus = ReadModifyWritePcuMailbox (
                         Socket,
                         MAILBOX_BIOS_CMD_ZERO,
                         MAILBOX_BIOS_CMD_ENABLE_DRAM_PM,
                         &TempData,
                         0,
                         0
                         );
  if (PcodeMailboxStatus != MAILBOX_BIOS_CC_NO_ERROR) {
    RcDebugPrint (SDBG_DEFAULT, PCU_MBOX_ERROR_MESSAGE, MAILBOX_BIOS_CMD_ENABLE_DRAM_PM, PcodeMailboxStatus);
    Status = EFI_DEVICE_ERROR;
  }
  if (!(UbiosGenerationOrHsleEnabled())) {
    TempData = 0;
    PcodeMailboxStatus = ReadModifyWritePcuMailbox (
                           Socket,
                           MAILBOX_BIOS_CMD_READ_DRAM_PM_ENABLED,
                           MAILBOX_BIOS_CMD_ZERO,
                           &TempData,
                           0,
                           0
                           );
    if (PcodeMailboxStatus != MAILBOX_BIOS_CC_NO_ERROR) {
      RcDebugPrint (SDBG_DEFAULT, PCU_MBOX_ERROR_MESSAGE, MAILBOX_BIOS_CMD_READ_DRAM_PM_ENABLED, PcodeMailboxStatus);
      Status = EFI_DEVICE_ERROR;
    }
    if ((TempData & (UINT32)(ImcMask)) != (UINT32)(ImcMask)) {
      RcDebugPrint (SDBG_DEFAULT, "PCU: API -> PcuEnableDramPm BIOS failed to issue B2P ENABLE_DRAM_PM, return data = 0x%x\n", TempData);
      Status = EFI_DEVICE_ERROR;
    }
  }
  return Status;
}

/**
  Disable DRAM Power Management in PCU

  @param[in]  Socket        0 based Socket number
  @param[in]  ImcMask       ImcMask to enable DRAM Power Management
                            Bit 0 => McId0, Bit 1 => McId1 and so on
                            If bit is set, corresponding power management is disabled

  @retval EFI_SUCCESS    Function execution successful
  @retval !EFI_SUCCESS   Error on command execution
**/
EFI_STATUS
EFIAPI
PcuDisableDramPm (
  IN UINT8 Socket,
  IN UINT8 ImcMask
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  UINT32 PcodeMailboxStatus = 0;
  UINT32 TempData = 0;

  //
  // Check ImcMask is covered by the SOC allowed Enable mask
  //
  if ((ImcMask & DRAM_THERMAL_DATA_MC_ENABLE_MASK) != ImcMask) {
    return EFI_INVALID_PARAMETER;
  }

  TempData = ImcMask;
  PcodeMailboxStatus = ReadModifyWritePcuMailbox (
                         Socket,
                         MAILBOX_BIOS_CMD_ZERO,
                         MAILBOX_BIOS_CMD_DISABLE_DRAM_PM,
                         &TempData,
                         0,
                         0
                         );
  if (PcodeMailboxStatus != MAILBOX_BIOS_CC_NO_ERROR) {
    RcDebugPrint (SDBG_DEFAULT, PCU_MBOX_ERROR_MESSAGE, MAILBOX_BIOS_CMD_DISABLE_DRAM_PM, PcodeMailboxStatus);
    Status = EFI_DEVICE_ERROR;
  }

  TempData = 0;
  PcodeMailboxStatus = ReadModifyWritePcuMailbox (
                         Socket,
                         MAILBOX_BIOS_CMD_READ_DRAM_PM_ENABLED,
                         MAILBOX_BIOS_CMD_ZERO,
                         &TempData,
                         0,
                         0
                         );
  if (PcodeMailboxStatus != MAILBOX_BIOS_CC_NO_ERROR) {
    RcDebugPrint (SDBG_DEFAULT, PCU_MBOX_ERROR_MESSAGE, MAILBOX_BIOS_CMD_READ_DRAM_PM_ENABLED, PcodeMailboxStatus);
    Status = EFI_DEVICE_ERROR;
  }

  //
  // Check returned data DRAM_PM_ENABLED_MASK of MAILBOX_BIOS_CMD_READ_DRAM_PM_ENABLED command.
  // If a bit is 0, corresponding iMC DRAM PM is sucessfully disabled.
  //
  if ((TempData & (UINT32)(ImcMask)) != 0) {
    Status = EFI_DEVICE_ERROR;
  }

  return Status;
}

/**
  Disable DRAM Thermal Management

  @param[in]  Socket        0 based Socket number
  @param[in]  ImcMask       ImcMask to enable DRAM Power Management
                            Bit 0 => McId0, Bit 1 => McId1 and so on
                            If bit is set, corresponding thermal management is disabled

  @retval EFI_SUCCESS    Function execution successful
  @retval !EFI_SUCCESS   Error on command execution
**/
EFI_STATUS
EFIAPI
PcuDisableDramThermal (
  IN UINT8 Socket,
  IN UINT8 ImcMask
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  UINT32 TempData = 0;
  UINT32 PcodeMailboxStatus = 0;

  //
  // Check ImcMask is covered by the SOC allowed Enable mask
  //
  if ((ImcMask & DRAM_THERMAL_DATA_MC_ENABLE_MASK) != ImcMask) {
    return EFI_INVALID_PARAMETER;
  }

  TempData = ImcMask;
  PcodeMailboxStatus = ReadModifyWritePcuMailbox (
                         Socket,
                         MAILBOX_BIOS_CMD_ZERO,
                         MAILBOX_BIOS_CMD_DISABLE_DRAM_THERMAL,
                         &TempData,
                         0,
                         0
                         );
  if (PcodeMailboxStatus != MAILBOX_BIOS_CC_NO_ERROR) {
    RcDebugPrint (SDBG_DEFAULT, PCU_MBOX_ERROR_MESSAGE, MAILBOX_BIOS_CMD_DISABLE_DRAM_THERMAL, PcodeMailboxStatus);
    Status = EFI_DEVICE_ERROR;
  }

  TempData = 0;
  PcodeMailboxStatus = ReadModifyWritePcuMailbox (
                         Socket,
                         MAILBOX_BIOS_CMD_READ_DRAM_THERMAL_ENABLED,
                         MAILBOX_BIOS_CMD_ZERO,
                         &TempData,
                         0,
                         0
                         );
  if (PcodeMailboxStatus != MAILBOX_BIOS_CC_NO_ERROR) {
    RcDebugPrint (SDBG_DEFAULT, PCU_MBOX_ERROR_MESSAGE, MAILBOX_BIOS_CMD_READ_DRAM_THERMAL_ENABLED, PcodeMailboxStatus);
    Status = EFI_DEVICE_ERROR;
  }
  if ((TempData & (UINT32)(ImcMask)) != 0) {
    Status = EFI_DEVICE_ERROR;
  }
  return Status;
}

/**
  Program VR Sensor Mapping in PCU

  @param[in]  Socket        0 based Socket number
  @param[in]  McId          Memory Controller number
  @param[in]  VrSvid        SVID of the Voltage Regulator

  @retval EFI_SUCCESS    Function execution successful
  @retval !EFI_SUCCESS   Error on command execution
**/
EFI_STATUS
EFIAPI
PcuProgramVrSensorMapping (
  IN UINT8 Socket,
  IN UINT8 McId,
  IN UINT8 VrSvid
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  UINT32 TempData = 0;
  UINT32 PcodeMailboxStatus = 0;

  TempData = ((VrSvid << MAILBOX_BIOS_CMD_DRAM_VR_SENSOR_MAPPING_DATA_SENSOR_VR_ADDR_LSB) | McId);
  PcodeMailboxStatus = ReadModifyWritePcuMailbox (
                         Socket,
                         MAILBOX_BIOS_CMD_DRAM_VR_SENSOR_MAPPING,
                         MAILBOX_BIOS_CMD_ZERO,
                         &TempData,
                         0,
                         0
                         );
  if (PcodeMailboxStatus != MAILBOX_BIOS_CC_NO_ERROR) {
    RcDebugPrint (SDBG_DEFAULT, PCU_MBOX_ERROR_MESSAGE, MAILBOX_BIOS_CMD_DRAM_VR_SENSOR_MAPPING, PcodeMailboxStatus);
    Status = EFI_DEVICE_ERROR;
  }
  if (TempData != 0) {
    RcDebugPrint (SDBG_DEFAULT, PCU_MBOX_ERROR_MESSAGE, MAILBOX_BIOS_CMD_DRAM_VR_SENSOR_MAPPING, PcodeMailboxStatus);
    Status = EFI_DEVICE_ERROR;
  }
  return Status;
}

/**
  Program VR memory controller Mapping in PCU

  @param[in]  Socket        0 based Socket number
  @param[in]  McId          Memory Controller number
  @param[in]  VrSvid        SVID of the Voltage Regulator

  @retval EFI_SUCCESS    Function execution successful
  @retval !EFI_SUCCESS   Error on command execution
**/
EFI_STATUS
EFIAPI
PcuProgramVrMcMapping (
  IN UINT8 Socket,
  IN UINT8 McId,
  IN UINT8 VrSvid
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  UINT32 TempData = 0;
  UINT32 PcodeMailboxStatus = 0;

  TempData = ((VrSvid << MAILBOX_BIOS_CMD_DRAM_VR_MC_MAPPING_DATA_VR_ADDR_LSB) | McId);
  PcodeMailboxStatus = ReadModifyWritePcuMailbox (
                         Socket,
                         MAILBOX_BIOS_CMD_DRAM_VR_MC_MAPPING,
                         MAILBOX_BIOS_CMD_ZERO,
                         &TempData,
                         0,
                         0
                         );
  if (PcodeMailboxStatus != MAILBOX_BIOS_CC_NO_ERROR) {
    RcDebugPrint (SDBG_DEFAULT, PCU_MBOX_ERROR_MESSAGE, MAILBOX_BIOS_CMD_DRAM_VR_MC_MAPPING, PcodeMailboxStatus);
    Status = EFI_DEVICE_ERROR;
  }
  if (TempData != 0) {
    RcDebugPrint (SDBG_DEFAULT, PCU_MBOX_ERROR_MESSAGE, MAILBOX_BIOS_CMD_DRAM_VR_MC_MAPPING, PcodeMailboxStatus);
    Status = EFI_DEVICE_ERROR;
  }

  return Status;
}

/**
  Program VR DIMM Mapping in PCU

  @param[in]  Socket            0 based Socket number
  @param[in]  DimmMask12V       Mask of Dimms attached to 12V. BIT0=>Dimm0...etc
  @param[in]  DimmMaskVr        Mask of Dimms attached to VR. BIT0=>Dimm0...etc
  @param[in]  McId              Memory Controller number
  @param[in]  ChId              Channel number

  @retval EFI_SUCCESS    Function execution successful
  @retval !EFI_SUCCESS   Error on command execution
**/
EFI_STATUS
EFIAPI
PcuProgramVrDimmMapping (
  IN UINT8 Socket,
  IN UINT8 DimmMask12V,
  IN UINT8 DimmMaskVr,
  IN UINT8 McId,
  IN UINT8 ChId
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  UINT32 TempData = 0;
  UINT32 PcodeMailboxStatus = 0;

  TempData = (DimmMaskVr << MAILBOX_BIOS_CMD_DRAM_DIMM_VR_MAPPING_DATA_VR_DIMM_MASK_LSB);
  TempData |= (DimmMask12V << MAILBOX_BIOS_CMD_DRAM_DIMM_VR_MAPPING_DATA_SENSOR_DIMM_MASK_LSB);

  TempData = TempData | (ChId << 8) | McId;
  PcodeMailboxStatus = ReadModifyWritePcuMailbox (
                         Socket,
                         MAILBOX_BIOS_CMD_DRAM_DIMM_VR_MAPPING,
                         MAILBOX_BIOS_CMD_ZERO,
                         &TempData,
                         0,
                         0
                         );
  if (PcodeMailboxStatus != MAILBOX_BIOS_CC_NO_ERROR) {
    RcDebugPrint (SDBG_DEFAULT, PCU_MBOX_ERROR_MESSAGE, MAILBOX_BIOS_CMD_DRAM_DIMM_VR_MAPPING, PcodeMailboxStatus);
    Status = EFI_DEVICE_ERROR;
  }
  if (TempData != 0) {
    RcDebugPrint (SDBG_DEFAULT, PCU_MBOX_ERROR_MESSAGE, MAILBOX_BIOS_CMD_DRAM_DIMM_VR_MAPPING, PcodeMailboxStatus);
    Status = EFI_DEVICE_ERROR;
  }

  return Status;
}


/**
  Enable DRAM Thermal Management

  @param[in]  Socket          0 based Socket number
  @param[in]  ImcMask         ImcMask to enable DRAM Power Management
                              Bit 0 => McId0, Bit 1 => McId1 and so on
                              If bit is set, corresponding thermal management is enabled.
  @param[in]  ThrottlingType  Throttling type to enable

  @retval EFI_SUCCESS    Function execution successful
  @retval !EFI_SUCCESS   Error on command execution
**/
EFI_STATUS
EFIAPI
PcuEnableDramThermal (
  IN UINT8 Socket,
  IN UINT8 ImcMask,
  IN PCU_THROTTLE_TYPE ThrottlingType
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  UINT32    PcodeMailboxStatus = 0;
  UINT32    TempData = 0;

  TempData = ImcMask;
  if (ThrottlingType == PCU_THROTTLE_CLTT_PECI) {
    TempData = TempData << 8;
  }

  PcodeMailboxStatus = ReadModifyWritePcuMailbox (
                         Socket,
                         MAILBOX_BIOS_CMD_ZERO,
                         MAILBOX_BIOS_CMD_ENABLE_DRAM_THERMAL,
                         &TempData,
                         0,
                         0
                         );
  if (PcodeMailboxStatus != MAILBOX_BIOS_CC_NO_ERROR) {
    RcDebugPrint (SDBG_DEFAULT, PCU_MBOX_ERROR_MESSAGE, MAILBOX_BIOS_CMD_ENABLE_DRAM_THERMAL, PcodeMailboxStatus);
    Status = EFI_DEVICE_ERROR;
  }

  if (!(UbiosGenerationOrHsleEnabled())) {
    TempData = 0;
    PcodeMailboxStatus = ReadModifyWritePcuMailbox (
                           Socket,
                           MAILBOX_BIOS_CMD_READ_DRAM_THERMAL_ENABLED,
                           MAILBOX_BIOS_CMD_ZERO,
                           &TempData,
                           0,
                           0
                           );
    if (PcodeMailboxStatus != MAILBOX_BIOS_CC_NO_ERROR) {
      RcDebugPrint (SDBG_DEFAULT, PCU_MBOX_ERROR_MESSAGE, MAILBOX_BIOS_CMD_READ_DRAM_THERMAL_ENABLED, PcodeMailboxStatus);
      Status = EFI_DEVICE_ERROR;
    }
    if (ThrottlingType == CLTT_PECI_ENABLE) {
      if (((TempData >> 8) & (UINT32)(DRAM_THERMAL_DATA_MC_ENABLE_MASK)) != (UINT32)(DRAM_THERMAL_DATA_MC_ENABLE_MASK)) {
        RcDebugPrint (SDBG_DEFAULT, "BIOS failed to issue B2P DRAM_THERMAL_ENABLE for CLTT w/ PECI, return data = 0x%x\n", TempData);
        Status = EFI_DEVICE_ERROR;
      }
    } else {
      if ((TempData & (UINT32)(DRAM_THERMAL_DATA_MC_ENABLE_MASK)) != (UINT32)(DRAM_THERMAL_DATA_MC_ENABLE_MASK)) {
        RcDebugPrint (SDBG_DEFAULT, "BIOS failed to issue B2P DRAM_THERMAL_ENABLE for CLTT, return data = 0x%x\n", TempData);
        Status = EFI_DEVICE_ERROR;
      }
    }
  }
  return Status;
}


/**
  Get CPU SKU Limit

  @param[in]   Socket              0 based Socket number
  @param[out]  *CpuSkuLimit        Return value of CPU SKU Limit (Memory capacity in 64GB Units)

  @retval EFI_SUCCESS    Function execution successful
  @retval !EFI_SUCCESS   Error on command execution
**/
EFI_STATUS
EFIAPI
PcuGetCpuSkuLimit (
  IN UINT8 Socket,
  OUT UINT32 *CpuSkuLimit
  )
{
  UINT32 PcodeMailboxStatus;
  UINT32 MailboxData;
  EFI_STATUS Status = EFI_SUCCESS;

  MailboxData = 0;
  PcodeMailboxStatus = ReadModifyWritePcuMailbox (
                         Socket,
                         MAILBOX_BIOS_CMD_READ_MAX_MEMORY_CAPACITY,
                         MAILBOX_BIOS_CMD_ZERO,
                         &MailboxData,
                         0,
                         0
                         );
  if (PcodeMailboxStatus == MAILBOX_BIOS_CC_NO_ERROR) {  // success
    //
    // The MAILBOX_BIOS_CMD_READ_MAX_MEMORY_CAPACITY response data from the PCU differs
    // between SKX\CLX and 10nm products.
    //
    // On SKX\CLX, a response of 0x0 indicates a fuse error which is not allowed; a
    // response of 0xFFFF indicates the "Unlimited" SKU.
    //
    // On 10nm, Memory Capacity SKU is not POR and is not implemented by the PCU. On
    // 10nm, BIOS can expect a return value of 0x0 which can be treated as the
    // "Unlimited" SKU. An error value is not defined on 10nm.
    //
    *CpuSkuLimit = (MailboxData & 0xFFFF); // LSB 16 bits are valid; 64GB units;
  } else {
    RcDebugPrint (SDBG_DEFAULT, PCU_MBOX_ERROR_MESSAGE, MAILBOX_BIOS_CMD_READ_MAX_MEMORY_CAPACITY, PcodeMailboxStatus);
    Status = EFI_DEVICE_ERROR;
  }
  return Status;
}



/**
  Get Vccp Value

  @param[in]   Socket              0 based Socket number
  @param[out]  *VccpValue           Return value of Vccp Voltage

  @retval EFI_SUCCESS    Function execution successful
  @retval !EFI_SUCCESS   Error on command execution
**/
EFI_STATUS
EFIAPI
PcuGetVccpValue (
  IN UINT8 Socket,
  IN UINT8 McId,
  OUT UINT16 *VccpValue
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  UINT32 PcodeMailboxStatus;
  UINT32 MailboxData;

  MailboxData = McId;
  PcodeMailboxStatus = ReadModifyWritePcuMailbox (
                         Socket,
                         MAILBOX_BIOS_CMD_READ_MC_VOLTAGE,
                         MAILBOX_BIOS_CMD_ZERO,
                         &MailboxData,
                         0,
                         0
                         );
  if (PcodeMailboxStatus == MAILBOX_BIOS_CC_NO_ERROR) {  // success
    MailboxData = UsraCsrRead (Socket, 0, BIOS_MAILBOX_DATA);
    *VccpValue = (MailboxData & 0xFFFF); // LSB 16 bits are valid;
    // convert vccp to actual voltage value
    *VccpValue = ((UINT32)(*VccpValue) * 1000)/VCCP_CONVERSION_FACTOR;
  } else {
    RcDebugPrint (SDBG_DEFAULT, PCU_MBOX_ERROR_MESSAGE, MAILBOX_BIOS_CMD_READ_MC_VOLTAGE, PcodeMailboxStatus);
    Status = EFI_DEVICE_ERROR;
  }
  return Status;
}

/**
  Read active VR info from PCU

  @param[in]   Socket              0 based Socket number
  @param[out]  *ActiveVrMask        Return value of Active VR

  @retval EFI_SUCCESS    Function execution successful
  @retval !EFI_SUCCESS   Error on command execution
**/
EFI_STATUS
EFIAPI
PcuReadActiveVr (
  IN UINT8 Socket,
  OUT UINT32 *ActiveVrMask
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  UINT32 PcodeMailboxStatus;

  if (FeaturePcdGet (PcdCteBuild) == TRUE) {
    //
    // There is no PCU register instance in CTE.
    //
    *ActiveVrMask = 0;
    return EFI_SUCCESS;
  }
  PcodeMailboxStatus = ReadModifyWritePcuMailbox (
                         Socket,
                         MAILBOX_BIOS_CMD_ACTIVE_VR_MASK,
                         MAILBOX_BIOS_CMD_ZERO,
                         ActiveVrMask,
                         0,
                         0
                         );
  if (PcodeMailboxStatus != MAILBOX_BIOS_CC_NO_ERROR) {
    RcDebugPrint (SDBG_DEFAULT, PCU_MBOX_ERROR_MESSAGE, MAILBOX_BIOS_CMD_ACTIVE_VR_MASK, PcodeMailboxStatus);
    Status = EFI_DEVICE_ERROR;
  }
  return Status;
}

/**
  Read VR mode for given VrAddress from PCU

  @param[in]   Socket             0 based Socket number
  @param[in]   VrAddress          Address to read VR Mode
  @param[out]  *VrMode            VR Mode information for given VR address

  @retval EFI_SUCCESS    Function execution successful
  @retval !EFI_SUCCESS   Error on command execution
**/
EFI_STATUS
EFIAPI
PcodeReadVrMode (
  IN UINT8  Socket,
  IN UINT32 VrAddress,
  OUT UINT32 *VrMode
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  UINT32 PcuMailboxCommand;
  UINT32 VrData;
  UINT32 PcodeMailboxStatus;

  PcuMailboxCommand = (UINT32) (MAILBOX_BIOS_CMD_VR_INTERFACE | (VrAddress << MAILBOX_BIOS_CMD_VR_INTERFACE_INTERFACE_VR_ID_LSB));
  VrData = (MAILBOX_BIOS_DATA_VR_REG_PROTOCOL_ID << MAILBOX_BIOS_CMD_VR_INTERFACE_DATA_REGISTER_ADDRESS_LSB) | MAILBOX_BIOS_VR_CMD_GET_REG;
  PcodeMailboxStatus = ReadModifyWritePcuMailbox (
                         Socket,
                         PcuMailboxCommand,
                         MAILBOX_BIOS_CMD_ZERO,
                         &VrData,
                         0,
                         0
                         );
  if (PcodeMailboxStatus == MAILBOX_BIOS_CC_NO_ERROR) {
    *VrMode = VrData;
  } else if (PcodeMailboxStatus == MAILBOX_BIOS_CC_BAD_VR_ADDR) {
    RcDebugPrint (SDBG_DEFAULT, "\n:: B2P responds MAILBOX_BIOS_CC_BAD_VR_ADDR for Socket = %d, VrAddress = 0x%x\n", Socket, VrAddress);
    Status = EFI_INVALID_PARAMETER;
  } else {
    RcDebugPrint (SDBG_DEFAULT, PCU_MBOX_ERROR_MESSAGE, MAILBOX_BIOS_CMD_VR_INTERFACE, PcodeMailboxStatus);
    Status = EFI_DEVICE_ERROR;
  }

  return Status;
}

/**
  Set Vdd voltage through PCU

  @param[in]   Socket             0 based Socket number
  @param[in]   VrAddress          Address to read VR Mode
  @param[in]   VrMode             Voltage Regulator mode
  @param[in]   Uservdd            UserVdd value from policy\setup

  @retval EFI_SUCCESS    Function execution successful
  @retval !EFI_SUCCESS   Error on command execution
**/
EFI_STATUS
EFIAPI
PcuSetVdd (
  IN UINT8  Socket,
  IN UINT32 VrAddress,
  IN UINT32 VrMode,
  IN UINT16 UserVdd
  )
{
  EFI_STATUS  Status;
  PSYSHOST    Host;
  UINT32      PcodeMailboxStatus;
  VddValues   *VddTable;
  UINT32      PcuMailboxCommand;
  UINT32      SvidEncoding;
  UINT8       VddTableIndex;
  UINT8       VddValuesTableSize;
  VddValues   VddValuesTable[] = {
  { SPD_VDD_110, SVID_DATA_110V_RAMP_ENCODING_VR12, SVID_DATA_110V_RAMP_ENCODING_VR12_5, SVID_DATA_110V_RAMP_ENCODING_VR13_5, SVID_DATA_110V_RAMP_ENCODING_VR13_10, CALL_TABLE_STRING("1.10V\n") },
  { SPD_VDD_120, SVID_DATA_120V_RAMP_ENCODING_VR12, SVID_DATA_120V_RAMP_ENCODING_VR12_5, SVID_DATA_120V_RAMP_ENCODING_VR13_5, SVID_DATA_120V_RAMP_ENCODING_VR13_10, CALL_TABLE_STRING("1.20V\n") },
  { 0xFF,        0xFF,                                                             0xFF,                                                               0xFF,                                                               0xFF,                                                                CALL_TABLE_STRING("Unknown VDD\n") },
  };

  Status = EFI_SUCCESS;
  Host = GetSysHostPointer ();
  PcodeMailboxStatus = 0;
  SvidEncoding = 0;
  VddTableIndex = 0;
  VddValuesTableSize = ARRAY_SIZE (VddValuesTable);

  if (UserVdd != 0) {
    //
    // Use the user provided Vdd if it is not 0
    //
    if ((VrMode == MAILBOX_BIOS_CMD_VR_INTERFACE_DATA_REGISTER_ADDRESS_VR_MODE_12) || (VrMode == MAILBOX_BIOS_CMD_VR_INTERFACE_DATA_REGISTER_ADDRESS_VR_MODE_13_5)) {
      //
      // Use 5mV VID table
      //
      SvidEncoding = VDD_SVID_ENCODE_5MV (UserVdd);
    } else {
      //
      // Use 10mV VID table
      //
      SvidEncoding = VDD_SVID_ENCODE_10MV (UserVdd);
    }
  } else {

    //
    // Set SVID encoding based on Selected Voltage and VrMode
    //
    for (VddTableIndex = 0; VddTableIndex < VddValuesTableSize; VddTableIndex++) {
      VddTable = &VddValuesTable[VddTableIndex];
      if (VddTable->ddrVoltageValue == Host->nvram.mem.socket[Socket].ddrVoltage) {
        break;
      }
    }

    if (VddTable->ddrVoltageValue == 0xFF) {
      Status = EFI_UNSUPPORTED;
    } else if (VrMode == MAILBOX_BIOS_CMD_VR_INTERFACE_DATA_REGISTER_ADDRESS_VR_MODE_12) {
      SvidEncoding = VddTable->Vr12Encoding;
    } else if (VrMode == MAILBOX_BIOS_CMD_VR_INTERFACE_DATA_REGISTER_ADDRESS_VR_MODE_12_5) {
      SvidEncoding = VddTable->Vr125Encoding;
    } else if (VrMode == MAILBOX_BIOS_CMD_VR_INTERFACE_DATA_REGISTER_ADDRESS_VR_MODE_13_5) {
      SvidEncoding = VddTable->Vr135Encoding;
    } else if (VrMode == MAILBOX_BIOS_CMD_VR_INTERFACE_DATA_REGISTER_ADDRESS_VR_MODE_13_10) {
      SvidEncoding = VddTable->Vr1310Encoding;
    }
  }

  if (!EFI_ERROR (Status)) {
    PcuMailboxCommand = (UINT32) (MAILBOX_BIOS_CMD_VDD_RAMP | (VrAddress << MAILBOX_BIOS_CMD_VDD_RAMP_INTERFACE_ADDR_LSB));
    PcodeMailboxStatus = ReadModifyWritePcuMailbox (
                           Socket,
                           MAILBOX_BIOS_CMD_ZERO,
                           PcuMailboxCommand,
                           &SvidEncoding,
                           0,
                           0
                           );
    if (PcodeMailboxStatus != MAILBOX_BIOS_CC_NO_ERROR) {
      RcDebugPrint (SDBG_DEFAULT, "PCU: API -> PcuSetVdd, Command->MAILBOX_BIOS_CMD_VDD_RAMP, Error Code -> %x", PcodeMailboxStatus);
      Status = EFI_DEVICE_ERROR;
    }
  }
  return Status;

}

/**
  Set warm reset data to PCU

  @param[in]   Socket                      0 based Socket number
  @param[in]   RstMode                     Surprize reset mode requested
  @param[in]   SurprizeRstCounter          Surprize reset counter.

  @retval EFI_SUCCESS    Function execution successful
  @retval !EFI_SUCCESS   Error on command execution
**/
EFI_STATUS
EFIAPI
PcuSetWarmResetCtrl (
  IN UINT8 Socket,
  IN PCU_RST_TYPE RstMode,
  IN UINT32 SurprizeRstCounter
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  UINT32 PcodeMailboxStatus;
  UINT32 PcuMailboxData;
  UINT8 RestMode = 0;

  switch (RstMode) {
    case PCU_LEGACY_RST:
      RestMode = WARM_RESET_SURPRIZRST_MODE_LEGACY;
      break;
    case PCU_ADR_RST:
      RestMode = WARM_RESET_SURPRIZRST_MODE_ADR;
      break;
    case PCU_VIRAL_RST:
      RestMode = WARM_RESET_SURPRIZRST_MODE_VIRAL;
      break;
    case PCU_BOTH_RST:
      RestMode = WARM_RESET_SURPRIZRST_MODE_BOTH;
      break;
    default:
      Status = EFI_INVALID_PARAMETER;
  }

  PcuMailboxData = SurprizeRstCounter | (RestMode << MAILBOX_BIOS_CMD_WARM_RESET_CTRL_DATA_SURPRIZRST_MODE_LSB);
  PcodeMailboxStatus = ReadModifyWritePcuMailbox (
                         Socket,
                         MAILBOX_BIOS_CMD_ZERO,
                         MAILBOX_BIOS_CMD_WARM_RESET_CTRL,
                         &PcuMailboxData,
                         0,
                         0
                         );
  if (PcodeMailboxStatus != MAILBOX_BIOS_CC_NO_ERROR) {
    RcDebugPrint (SDBG_DEFAULT, PCU_MBOX_ERROR_MESSAGE, MAILBOX_BIOS_CMD_WARM_RESET_CTRL, PcodeMailboxStatus);
    Status = EFI_DEVICE_ERROR;
  }
  return Status;
}

/**
  Set Scrub interval in PCU

  @param[in]   Socket                 0 based Socket number
  @param[in]   McId                   Reference clock type
  @param[in]   ScrubInterval          Value of the Scrub Interval

  @retval EFI_SUCCESS    Function execution successful
  @retval !EFI_SUCCESS   Error on command execution
**/
EFI_STATUS
EFIAPI
PcuSetScrubInterval (
  IN UINT8 Socket,
  IN UINT8 McId,
  IN UINT32 ScrubInterval
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  UINT32 PcodeMailboxStatus;
  UINT32 PcuMailboxCommand;
  UINT32 PcuMailboxData;

  PcuMailboxCommand = ((UINT32)MAILBOX_BIOS_CMD_MC_PATROL_SCRUB_INTERVAL_CONFIG) | (1 << (MAILBOX_BIOS_CMD_MC_PATROL_SCRUB_INTERVAL_CONFIG_INTERFACE_MC_INSTANCE_MASK_LSB + McId));
  PcuMailboxData = ScrubInterval;
  PcodeMailboxStatus = ReadModifyWritePcuMailbox (
                         Socket,
                         MAILBOX_BIOS_CMD_ZERO,
                         PcuMailboxCommand,
                         &PcuMailboxData,
                         0,
                         0
                         );
  if (PcodeMailboxStatus != MAILBOX_BIOS_CC_NO_ERROR) {
    RcDebugPrint (SDBG_DEFAULT, PCU_MBOX_ERROR_MESSAGE, MAILBOX_BIOS_CMD_MC_PATROL_SCRUB_INTERVAL_CONFIG, PcodeMailboxStatus);
    Status = EFI_DEVICE_ERROR;
  }
  return Status;
}

/**
  Get Scrub interval in PCU

  @param[in]   Socket                 0 based Socket number
  @param[in]   McId                   Reference clock type
  @param[in]   ScrubInterval          Value of the Scrub Interval

  @retval EFI_SUCCESS    Function execution successful
  @retval !EFI_SUCCESS   Error on command execution
**/
EFI_STATUS
EFIAPI
PcuGetScrubInterval (
  IN UINT8 Socket,
  IN UINT8 McId,
  OUT UINT32 *ScrubInterval
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  UINT32 PcodeMailboxStatus;
  UINT32 PcuMailboxCommand;
  UINT32 PcuMailboxData;

  PcuMailboxCommand = ((UINT32)MAILBOX_BIOS_CMD_MC_PATROL_SCRUB_INTERVAL_CONFIG) | (1 << (MAILBOX_BIOS_CMD_MC_PATROL_SCRUB_INTERVAL_CONFIG_INTERFACE_MC_INSTANCE_MASK_LSB + McId));
  PcodeMailboxStatus = ReadModifyWritePcuMailbox (
                         Socket,
                         PcuMailboxCommand,
                         MAILBOX_BIOS_CMD_ZERO,
                         &PcuMailboxData,
                         0,
                         0
                         );
  if (PcodeMailboxStatus == MAILBOX_BIOS_CC_NO_ERROR) {
    *ScrubInterval = PcuMailboxData;
  } else {
    RcDebugPrint (SDBG_DEFAULT, PCU_MBOX_ERROR_MESSAGE, MAILBOX_BIOS_CMD_MC_PATROL_SCRUB_INTERVAL_CONFIG, PcodeMailboxStatus);
    Status = EFI_DEVICE_ERROR;
  }
  return Status;
}


/**
  Set Missed Threshold for Scrub in PCU

  @param[in]   Socket                 0 based Socket number
  @param[in]   McId                   Reference clock type
  @param[in]   MissedThreshold        Value of the threshold for Scrub miss

  @retval EFI_SUCCESS    Function execution successful
  @retval !EFI_SUCCESS   Error on command execution
**/
EFI_STATUS
EFIAPI
PcuSetMissedThreshold (
  IN UINT8 Socket,
  IN UINT8 McId,
  IN UINT32 MissedThreshold
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  UINT32 MailboxCommand;
  UINT32 MailboxData;
  UINT32 PcodeMailboxStatus;

  //
  // Mailbox Command
  // 7:0   : Mailbox Command  (0xBE)
  // 15:8  : Mc Bitmap 0 = Mc0, 1 = Mc1, 2 = Mc2, 3 = Mc3
  // 17:16 : Mem Type 0 = DDR4, 1 = DDR5, 2 = HBM
  // 19:18 : Sub-Command
  // Sub-Command 0: Data value = wake_delay
  // Sub-Command 1: Data value = Drain Rate
  // Sub-Command 2: Data value = Missed_Threshold

  // Mailbox Data :
  // 15:0 : Data value
  //
  MailboxCommand = ((UINT32)MAILBOX_BIOS_CMD_MC_PATROL_SCRUB_PKGC_CONFIG) |
                   (0x1 << (MAILBOX_BIOS_CMD_MC_PATROL_SCRUB_INTERVAL_CONFIG_INTERFACE_MC_INSTANCE_MASK_LSB + McId)) |
                   (0x2 << MAILBOX_BIOS_CMD_MC_PATROL_SCRUB_PKGC_CONFIG_INTERFACE_SUB_COMMAND_LSB);

  MailboxData = MissedThreshold;

  PcodeMailboxStatus = ReadModifyWritePcuMailbox (
                         Socket,
                         MAILBOX_BIOS_CMD_ZERO,
                         MailboxCommand,
                         &MailboxData,
                         0,
                         0
                         );
  if (PcodeMailboxStatus != MAILBOX_BIOS_CC_NO_ERROR) {
    RcDebugPrint (SDBG_DEFAULT, PCU_MBOX_ERROR_MESSAGE, MAILBOX_BIOS_CMD_MC_PATROL_SCRUB_PKGC_CONFIG, PcodeMailboxStatus);
    Status = EFI_DEVICE_ERROR;
  }
  return Status;
}



/**
  Set Advanced Threshold for Scrub in PCU

  @param[in]   Socket                 0 based Socket number
  @param[in]   McId                   Reference clock type
  @param[in]   AdvancedThreshold      Value of Advanced threshold for Scrub

  @retval EFI_SUCCESS    Function execution successful
  @retval !EFI_SUCCESS   Error on command execution
**/
EFI_STATUS
EFIAPI
PcuSetAdvancedThreshold (
  IN UINT8 Socket,
  IN UINT8 McId,
  IN UINT32 AdvancedThreshold
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  UINT32 PcodeMailboxStatus;
  UINT32 MailboxCommand;
  UINT32 MailboxData;

  MailboxCommand = ((UINT32) MAILBOX_BIOS_CMD_MC_PATROL_SCRUB_ADV_SCRUB_CONFIG) |
                    (1 << (MAILBOX_BIOS_CMD_MC_PATROL_SCRUB_INTERVAL_CONFIG_INTERFACE_MC_INSTANCE_MASK_LSB + McId));
  MailboxData = (UINT32) (AdvancedThreshold << 16);
  PcodeMailboxStatus = ReadModifyWritePcuMailbox (
                         Socket,
                         MAILBOX_BIOS_CMD_ZERO,
                         MailboxCommand,
                         &MailboxData,
                         0,
                         0
                         );
  if (PcodeMailboxStatus != MAILBOX_BIOS_CC_NO_ERROR) {
    RcDebugPrint (SDBG_DEFAULT, PCU_MBOX_ERROR_MESSAGE, MAILBOX_BIOS_CMD_MC_PATROL_SCRUB_ADV_SCRUB_CONFIG, PcodeMailboxStatus);
    Status = EFI_DEVICE_ERROR;
  }
  return Status;
}


/**
  Check Fuse override enabled

  @param[in]    Socket                 0 based Socket number

  @retval EFI_SUCCESS    Function execution successful
  @retval !EFI_SUCCESS   Error on command execution
**/
EFI_STATUS
PcuFuseOverride (
  UINT8 Socket
  )
{
  EFI_STATUS Status = EFI_UNSUPPORTED;

  return Status;
}

/**
  Set miscellanous workaround enable

  @param[in]    Socket                  0 based Socket number
  @param[in]    CmdData                 Workaround Data

  @retval EFI_SUCCESS    Function execution successful
  @retval !EFI_SUCCESS   Error on command execution
**/
EFI_STATUS
PcuSetMiscWorkaroundEnable (
  UINT8 Socket,
  UINT32 CmdData
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  UINT32 PcodeMailboxStatus;
  UINT32 PcuMailboxData;

  PcuMailboxData = CmdData;
  PcodeMailboxStatus = ReadModifyWritePcuMailbox (
                         Socket,
                         MAILBOX_BIOS_CMD_ZERO,
                         MAILBOX_BIOS_CMD_MISC_WORKAROUND_ENABLE,
                         &PcuMailboxData,
                         0,
                         0
                         );
  if (PcodeMailboxStatus != MAILBOX_BIOS_CC_NO_ERROR) {
    Status = EFI_DEVICE_ERROR;
  }
  return Status;
}

/**
  Disable rcomp for given MCID mask

  @param[in]    Socket                 0 based Socket number
  @param[in]    McIdMask               McId mask value, Bit0=>Mc0..etc

  @retval EFI_SUCCESS    Function execution successful
  @retval !EFI_SUCCESS   Error on command execution
**/
EFI_STATUS
PcuDisableRcomp (
  UINT8 Socket,
  UINT8 McIdMask
)
{
  EFI_STATUS Status = EFI_SUCCESS;
  UINT32 PcodeMailboxStatus;
  UINT32 PCUData = 0;
  UINT32 DwordOrValue;

  //
  // Disable Pcode RCOMP Bit[20:19]
  //
  DwordOrValue = ((McIdMask & 0x3) << MAILBOX_BIOS_CMD_WRITE_PCU_MISC_CONFIG_DATA_DISABLE_PCODE_RCOMP_LSB);
  PcodeMailboxStatus = ReadModifyWritePcuMailbox (
                         Socket,
                         MAILBOX_BIOS_CMD_READ_PCU_MISC_CONFIG,
                         MAILBOX_BIOS_CMD_WRITE_PCU_MISC_CONFIG,
                         &PCUData,
                         (UINT32)-1,
                         DwordOrValue
                         );
  if (PcodeMailboxStatus != MAILBOX_BIOS_CC_NO_ERROR) {
    RcDebugPrint (SDBG_DEFAULT, "PCU: API -> PcuDisableRcomp, Error Code -> %x\n", PcodeMailboxStatus);
    Status = EFI_DEVICE_ERROR;
  }

  return Status;
}

/**
  Enable Surprise reset in PCU

  @param[in]    Socket                 0 based Socket number

  @retval EFI_SUCCESS    Function execution successful
  @retval !EFI_SUCCESS   Error on command execution
**/
EFI_STATUS
PcuEnableSurpriseReset (
  UINT8 Socket
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  UINT32 PcodeMailboxStatus;
  UINT32 PcuData = 0;

  //
  // Enable Surprise reset bit 11
  //
  PcodeMailboxStatus = ReadModifyWritePcuMailbox (
                         Socket,
                         MAILBOX_BIOS_CMD_READ_PCU_MISC_CONFIG,
                         MAILBOX_BIOS_CMD_WRITE_PCU_MISC_CONFIG,
                         &PcuData,
                         (UINT32)-1,
                         PCU_MISC_SURPRISE_RESET
                         );
  if (PcodeMailboxStatus != MAILBOX_BIOS_CC_NO_ERROR) {
    RcDebugPrint (SDBG_DEFAULT, "PCU: API -> PcuEnableSurpriseReset, Error Code -> %x\n", PcodeMailboxStatus);
    Status = EFI_DEVICE_ERROR;
  }

  return Status;
}

/**
  Program SREF PKGC register in PCU

  @param[in]    Socket                 0 based Socket number
  @param[in]    McId                    Memory Controller number
  @param[in]    ChId                    Channel Number
  @param[in]    SrefData                SREF Data

  @retval EFI_SUCCESS    Function execution successful
  @retval !EFI_SUCCESS   Error on command execution
**/
EFI_STATUS
PcuSetSrefPkgc (
  UINT8 Socket,
  UINT8 McId,
  UINT8 ChId,
  UINT32 SrefData
)
{
  return EFI_UNSUPPORTED;
}

/**
  Set E2E parity bit in PCU Misc

  @param[in]    Socket                  0 based Socket number

  @retval EFI_SUCCESS    Function execution successful
  @retval !EFI_SUCCESS   Error on command execution
**/
EFI_STATUS
PcuSetE2EParity (
  UINT8 Socket
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  UINT32 PcodeMailboxStatus = 0;
  UINT32 Data32 = 0;

  PcodeMailboxStatus = ReadModifyWritePcuMailbox (Socket, MAILBOX_BIOS_CMD_READ_PCU_MISC_CONFIG, MAILBOX_BIOS_CMD_WRITE_PCU_MISC_CONFIG, &Data32, ~(UINT32)MAILBOX_BIOS_CMD_WRITE_PCU_MISC_CONFIG_DATA_E2E_PARITY_ENABLE_MASK, 1 << MAILBOX_BIOS_CMD_WRITE_PCU_MISC_CONFIG_DATA_E2E_PARITY_ENABLE_LSB);
  if (PcodeMailboxStatus != MAILBOX_BIOS_CC_NO_ERROR) {
    RcDebugPrint (SDBG_DEFAULT, PCU_MBOX_ERROR_MESSAGE, MAILBOX_BIOS_CMD_WRITE_PCU_MISC_CONFIG, PcodeMailboxStatus);
    Status = EFI_DEVICE_ERROR;
  }
  return Status;
}

/**
  Program DRAM RAPL Mode 0=>Estimated,01=>VR Measurement

  @param[in]    Socket                  0 based Socket number
  @param[in]    RaplMode                Set Rapl Mode

  @retval EFI_SUCCESS       Function execution successful
  @retval EFI_UNSUPPORTED   Command not supported
  @retval !EFI_SUCCESS      Error on command execution
**/
EFI_STATUS
EFIAPI
PcuSetDramRaplMode (
  IN UINT8 Socket,
  IN UINT8 RaplMode
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Program Bandwidth power scaling factor

  @param[in]    Socket                  0 based Socket number
  @param[in]    Channel                 Channel Number
  @param[in]    SelectorType            Selector type, 0 => Scale Factor, 1=> Minimum Power
  @param[in]    Value                   Value to program

  @retval EFI_SUCCESS    Function execution successful
  @retval !EFI_SUCCESS   Error on command execution
**/
EFI_STATUS
EFIAPI
PcuSetBWPowerScale (
  IN UINT8 Socket,
  IN UINT8 Channel,
  IN UINT8 SelectorType,
  IN UINT16 Value
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Program DDR power scaling factor

  @param[in]    Socket                  0 based Socket number
  @param[in]    Channel                 Channel Number
  @param[in]    Dimm                    Dimm Number
  @param[in]    SelectorType            Selector type, 0 => Scale Factor, 1=> Minimum Power
  @param[in]    Value                   Value to program

  @retval EFI_SUCCESS    Function execution successful
  @retval !EFI_SUCCESS   Error on command execution
**/
EFI_STATUS
EFIAPI
PcuProgramDDRPowerScale (
  IN UINT8 Socket,
  IN UINT8 Channel,
  IN UINT8 Dimm,
  IN UINT8 SelectorType,
  IN UINT16 Value
  )
{
  return EFI_UNSUPPORTED;
}

/*
  Program PBM Throttle Adjust factor

  @param[in]    Socket                  0 based Socket number
  @param[in]    Channel                 Channel Number
  @param[in]    Value                   Value to program

  @retval EFI_SUCCESS    Function execution successful
  @retval !EFI_SUCCESS   Error on command execution
**/
EFI_STATUS
EFIAPI
PcuProgramPbmThrotAdj (
  IN UINT8 Socket,
  IN UINT8 Channel,
  IN UINT8 Value
  )
{
  return EFI_UNSUPPORTED;
}

/**
  Lock\UnLock Spd Smbus access

  @param[in]    Socket              0 based Socket number
  @param[in]    SpdSmbusAccessType  Lock\Open the access

  @retval EFI_SUCCESS    Function execution successful
  @retval !EFI_SUCCESS   Error on command execution
**/
EFI_STATUS
EFIAPI
PcuSpdSmbusAccess (
  IN UINT8 Socket,
  IN SPD_SMBUS_ACCESS_TYPE SpdSmbusAccessType
  )

{
  EFI_STATUS Status = EFI_SUCCESS;
  UINT32 PcodeMailboxStatus = 0;

  UINT32 Data32 = 0;

  if (SpdSmbusAccessType == SPD_SMBUS_ACCESS_UNLOCK) {
    RcDebugPrint (SDBG_DEFAULT, "PcuSpdSmbusAccess - Unlocking SMBUS Access\n");
    PcodeMailboxStatus = ReadModifyWritePcuMailbox (Socket,
                                                    MAILBOX_BIOS_CMD_READ_PCU_MISC_CONFIG,
                                                    MAILBOX_BIOS_CMD_WRITE_PCU_MISC_CONFIG,
                                                    &Data32,
                                                    ~(UINT32)(1 << MAILBOX_BIOS_CMD_WRITE_PCU_MISC_CONFIG_DATA_SMBUS_ACCESS_LSB),
                                                    MAILBOX_BIOS_CMD_WRITE_PCU_MISC_CONFIG_DATA_SMBUS_UNLOCK_MASK);

    if (PcodeMailboxStatus != MAILBOX_BIOS_CC_NO_ERROR) {
      RcDebugPrint (SDBG_DEFAULT, PCU_MBOX_ERROR_MESSAGE, MAILBOX_BIOS_CMD_WRITE_PCU_MISC_CONFIG, PcodeMailboxStatus);
      Status = EFI_DEVICE_ERROR;
    }
  } else if (SpdSmbusAccessType == SPD_SMBUS_ACCESS_LOCK) {
    // Do nothing for lock
    RcDebugPrint (SDBG_DEFAULT, "PcuSpdSmbusAccess - SMBUS locked by default\n");
    return Status;
  } else {
    Status = EFI_INVALID_PARAMETER;
  }

  return Status;
}




/**
  Check Fuse override enabled

  @param[in]    Socket                 0 based Socket number
  @param[in]    AvoidRatio             Send avoid mesh range to Pcode

  @retval EFI_SUCCESS    Function execution successful
  @retval !EFI_SUCCESS   Error on command execution
**/
EFI_STATUS
EFIAPI
PcuSetAvoidMeshRatio (
  IN UINT8  Socket,
  IN UINT32 AvoidRatio
)
{
  return EFI_UNSUPPORTED;
}

/**
  Trigger MDFIS advanced Training

  @param[in]    Socket                 0 based Socket number
  @param[in]    MinRatio               Min ratio
  @param[in]    MaxRatio               Max ratio

  @retval EFI_SUCCESS    Function execution successful
  @retval !EFI_SUCCESS   Error on command execution
**/
EFI_STATUS
EFIAPI PcuTriggerMdfisTraining (
  IN UINT8 Socket,
  IN UINT8 MinRatio,
  IN UINT8 MaxRatio
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  UINT32 PcodeMailBoxStatus;
  UINT32 PcodeMailboxCommand;
  UINT32 PcodeMailboxData;

  PcodeMailboxCommand = MAILBOX_BIOS_CMD_MULTI_SKT_UPD | (MAILBOX_BIOS_CMD_MULTI_SKT_UPD_INTERFACE_SUB_COMMAND_MDFIS_TRAINING << MAILBOX_BIOS_CMD_MULTI_SKT_UPD_INTERFACE_SUB_COMMAND_LSB);
  PcodeMailboxData = (MinRatio << MAILBOX_BIOS_CMD_MULTI_SKT_UPD_DATA_BIOS_MIN_RATIO_LSB) | \
                     (MaxRatio << MAILBOX_BIOS_CMD_MULTI_SKT_UPD_DATA_BIOS_MAX_RATIO_LSB);
  PcodeMailBoxStatus = ReadModifyWritePcuMailbox (
                         Socket,
                         MAILBOX_BIOS_CMD_ZERO,
                         PcodeMailboxCommand,
                         &PcodeMailboxData,
                         0,
                         0
                         );
  if (PcodeMailBoxStatus != MAILBOX_BIOS_CC_NO_ERROR) {
    RcDebugPrint (SDBG_DEFAULT, PCU_MBOX_ERROR_MESSAGE, MAILBOX_BIOS_CMD_MULTI_SKT_UPD, PcodeMailBoxStatus);
    Status = EFI_DEVICE_ERROR;
  }

  return Status;
}

/**
  This command is used to override the default pointer separation values for BGFs in the GV
  domain

  @param[in]    Socket                 0 based Socket number
  @param[in]    BGFPtrOverrideMask     Mask to indicate which BGFs pointer separation need to be programmed

  @retval EFI_SUCCESS    Function execution successful
  @retval !EFI_SUCCESS   Error on command execution
**/
EFI_STATUS
EFIAPI
PcuBGFPointerOverride (
  IN UINT8  Socket,
  IN UINT8  BGFPtrOverrideMask
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  UINT32 PcodeMailboxStatus = MAILBOX_BIOS_CC_NO_ERROR;
  UINT32 PcuMailboxCommand;
  UINT32 PcuMailboxData;

  // BIOS_MAILBOX_DATA: pointer separation data:
  // [2:0]: GV side pointer separation (pointer separation for M3KTI)
  // [6:4]: Non-GV side pointer separation (pointer separation for KTI)
  // Other bits reserved
  PcuMailboxCommand = MAILBOX_BIOS_CMD_GV_PTRSEP_OVERRIDE |
                      (MAILBOX_BIOS_CMD_GV_PTRSEP_OVERRIDE_INTERFACE_BGF_ID_M3KTI_KTI << MAILBOX_BIOS_CMD_GV_PTRSEP_OVERRIDE_INTERFACE_BGF_ID_LSB);
  PcuMailboxData = (BGF_POINTER_OVERRIDE_KTI_NON_GV << MAILBOX_BIOS_CMD_GV_PTRSEP_OVERRIDE_DATA_NON_GV_SEP_LSB) |
                   BGF_POINTER_OVERRIDE_KTI_GV;
  PcodeMailboxStatus = ReadModifyWritePcuMailbox (Socket, MAILBOX_BIOS_CMD_ZERO, PcuMailboxCommand, &PcuMailboxData, 0, 0);

  if (PcodeMailboxStatus != MAILBOX_BIOS_CC_NO_ERROR) {
    RcDebugPrint (SDBG_DEFAULT, PCU_MBOX_ERROR_MESSAGE, MAILBOX_BIOS_CMD_GV_PTRSEP_OVERRIDE, PcodeMailboxStatus);
    Status = EFI_DEVICE_ERROR;
  }

  return Status;
}

/**
  This command is used to trigger mechanism for TSC synchronization of hot
  added/onlined sockets

  @param[in]    Socket                 0 based Socket number
  @param[in]    Value                  1: assert the CPU only reset pin on the local socket.
                                       0: de-assert the CPU only reset pin on the local socket.

  @retval EFI_SUCCESS    Function execution successful
  @retval !EFI_SUCCESS   Error on command execution
**/
EFI_STATUS
EFIAPI
PcuSynchronizeTsc (
  IN UINT8  Socket,
  IN UINT8  Value
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  UINT32 PcodeMailboxStatus;
  UINT32 PcuMailboxData;

  PcuMailboxData = Value;
  PcodeMailboxStatus = ReadModifyWritePcuMailbox (
                         Socket,
                         MAILBOX_BIOS_CMD_ZERO,
                         MAILBOX_BIOS_CMD_SYNCHRONIZE_TSC_FOR_HOT_ADD,
                         &PcuMailboxData,
                         0,
                         0
                         );
  if (PcodeMailboxStatus != MAILBOX_BIOS_CC_NO_ERROR) {
    RcDebugPrint (SDBG_DEFAULT, PCU_MBOX_ERROR_MESSAGE, MAILBOX_BIOS_CMD_SYNCHRONIZE_TSC_FOR_HOT_ADD, PcodeMailboxStatus);
    Status = EFI_DEVICE_ERROR;
  }
  return Status;
}



/**
  BIOS uses this command to clear the FW agent link layer attribute in the UPI (f.k.a. KTI,
  f.k.a. QPI) transmit parameters on the neighboring socket, corresponding to the UPI port that
  will be connected to the socket being added.

  @param[in]    Socket                 0 based Socket number
  @param[in]    UpiPortId              UPI Port ID

  @retval EFI_SUCCESS    Function execution successful
  @retval !EFI_SUCCESS   Error on command execution
**/
EFI_STATUS
EFIAPI
PcuMaskFwAgent (
  IN UINT8  Socket,
  IN UINT8  UpiPortId
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  UINT32 PcodeMailboxStatus;
  UINT32 PcuMailboxCommand;
  UINT32 PcuMailboxData;

  PcuMailboxCommand = MAILBOX_BIOS_CMD_MASK_FW_AGENT | (UpiPortId << MAILBOX_BIOS_CMD_MASK_FW_AGENT_INTERFACE_LINK_ID_LSB);
  PcuMailboxData = 0;
  PcodeMailboxStatus = ReadModifyWritePcuMailbox (
                         Socket,
                         MAILBOX_BIOS_CMD_ZERO,
                         PcuMailboxCommand,
                         &PcuMailboxData,
                         0,
                         0
                         );
  if (PcodeMailboxStatus != MAILBOX_BIOS_CC_NO_ERROR) {
    RcDebugPrint (SDBG_DEFAULT, PCU_MBOX_ERROR_MESSAGE, MAILBOX_BIOS_CMD_MASK_FW_AGENT, PcodeMailboxStatus);
    Status = EFI_DEVICE_ERROR;
  }
  return Status;
}

/**
  Used to configure SNC

  @param[in]    Socket                 0 based Socket number
  @Param[in]    SncCfgInterfaceIndex   Snc configure interface Index
  @param[in]    Value                  SNC configuration

  @retval EFI_SUCCESS    Function execution successful
  @retval !EFI_SUCCESS   Error on command execution
**/
EFI_STATUS
EFIAPI
PcuCfgSnc (
  IN UINT8  Socket,
  IN UINT8  SncCfgInterfaceIndex,
  IN UINT32 Value
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  UINT32 PcodeMailboxStatus;
  UINT32 MailboxCmd;
  UINT32 MailboxData;

  MailboxData = Value;
  MailboxCmd = MAILBOX_BIOS_CMD_SNC_CONFIG | MAILBOX_BIOS_CMD_SNC_CONFIG_INTERFACE_READ_WRITE_MASK;
  PcodeMailboxStatus = ReadModifyWritePcuMailbox (
                         Socket,
                         MAILBOX_BIOS_CMD_ZERO,
                         MailboxCmd | (SncCfgInterfaceIndex << MAILBOX_BIOS_CMD_SNC_CONFIG_INTERFACE_SNC_INDEX_LSB),
                         &MailboxData,
                         0,
                         0
                         );

  RcDebugPrint (SDBG_DEFAULT, "Socket:%d  %a 0x%08x \n", Socket,
                ((SncCfgInterfaceIndex == SNC_INDEX_CONFIG) ? "SncConfig":
                 (SncCfgInterfaceIndex == SNC_INDEX_BASE_1) ? "Base1":
                 (SncCfgInterfaceIndex == SNC_INDEX_BASE_2) ? "Base2":
                 (SncCfgInterfaceIndex == SNC_INDEX_BASE_3) ? "Base3":
                 (SncCfgInterfaceIndex == SNC_INDEX_BASE_4) ? "Base4":
                 (SncCfgInterfaceIndex == SNC_INDEX_BASE_5) ? "Base5":
                 (SncCfgInterfaceIndex == SNC_INDEX_UPPER_BASE) ? "UpperBase": "Invalid Index"),
                Value);

  if (PcodeMailboxStatus != MAILBOX_BIOS_CC_NO_ERROR) {
    RcDebugPrint (SDBG_DEFAULT, PCU_MBOX_ERROR_MESSAGE, MAILBOX_BIOS_CMD_SNC_CONFIG, PcodeMailboxStatus);
    Status = EFI_DEVICE_ERROR;
  }

  return Status;
}

/**
  Check whether PCU command is MAILBOX_BIOS_CMD_SET_UPI_FREQ

  @param[in]    Socket                 0 based Socket number
  @param[in]    Value                  Mailbox Interface register value
  @param[out]   Flag                   Indicate whether it's SET_UPI_FREQ command or not

  @retval EFI_SUCCESS    Function execution successful
  @retval !EFI_SUCCESS   Error on command execution
**/
EFI_STATUS
EFIAPI
PcuCheckCmdIsSetUpiFreq (
  IN UINT8  Socket,
  IN UINT32 Value,
  OUT BOOLEAN *Flag
  )
{
  if((Value & 0xFF) == MAILBOX_BIOS_CMD_SET_UPI_FREQ) {
    *Flag = TRUE;
  } else {
    *Flag = FALSE;
  }

  return EFI_SUCCESS;
}

/**
  Initial Dimm_temp to 0x54 for DCPMM

  @param[in]  Socket        0 based Socket number
  @param[in]  McId          Memory Controller number
  @param[in]  Ch            0 based Channel number
  @param[in]  Dimm          0 based Dimm number

  @retval EFI_SUCCESS    Function execution successful
  @retval !EFI_SUCCESS   Error on command execution
**/
EFI_STATUS
EFIAPI
PcuInitialDimmTemp (
  IN UINT8  Socket,
  IN UINT8  McId,
  IN UINT8  Ch,
  IN UINT8  Dimm
)
{
  return EFI_UNSUPPORTED;
}

/**
  Enable IMON polling on given SVID by sending B2P Command
  @param[in]  Socket        0 based Socket number
  @param[in]  VrSvid        SVID of the Voltage Regulator

  @retval EFI_SUCCESS    Function execution successful
  @retval !EFI_SUCCESS   Error on command execution
**/
EFI_STATUS
EFIAPI
PcuEnableImon (
  IN UINT8 Socket,
  IN UINT8 VrSvid
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  UINT32 TempData = 0;
  UINT32 PcodeMailboxStatus = 0;

  PcodeMailboxStatus = ReadModifyWritePcuMailbox (Socket,  ((UINT32)MAILBOX_BIOS_CMD_SVID_VR_HANDLER | (MAILBOX_BIOS_CMD_SVID_VR_HANDLER_INTERFACE_SUB_COMMAND_SET_IMON_ENABLE << MAILBOX_BIOS_CMD_SVID_VR_HANDLER_INTERFACE_SUB_COMMAND_LSB) |
                                                  (VrSvid << MAILBOX_BIOS_CMD_SVID_VR_HANDLER_INTERFACE_PARAM2_LSB)),
                                                  MAILBOX_BIOS_CMD_ZERO,
                                                  &TempData, 0, 0);
  if (PcodeMailboxStatus != MAILBOX_BIOS_CC_NO_ERROR) {
    RcDebugPrint (SDBG_DEFAULT, PCU_MBOX_ERROR_MESSAGE, MAILBOX_BIOS_CMD_SVID_VR_HANDLER, PcodeMailboxStatus);
    Status = EFI_DEVICE_ERROR;
  }
  if (TempData != 0) {
    RcDebugPrint (SDBG_DEFAULT, PCU_MBOX_ERROR_MESSAGE, MAILBOX_BIOS_CMD_SVID_VR_HANDLER, PcodeMailboxStatus);
    Status = EFI_DEVICE_ERROR;
  }

  return Status;
}

/**
  Trigger late action credit update.

  In response to the mailbox command, pcode (on CPU-L) will inform the S3M to
  initiate a multi-socket update sequence where pcode sets the trigger control bit
  to cause a late-action update of the credits.

  @param[in]    Socket                 0 based Socket number

  @retval EFI_SUCCESS    Function execution successful
  @retval !EFI_SUCCESS   Error on command execution

**/
EFI_STATUS
EFIAPI
PcuTriggerLateActionRegUpdate (
  IN UINT8  Socket
  )
{
  EFI_STATUS Status;
  UINT32     PcodeMailBoxStatus;
  UINT32     PcodeMailboxCommand;
  UINT32     PcodeMailboxData;

  Status = EFI_SUCCESS;
  PcodeMailboxCommand = MAILBOX_BIOS_CMD_MULTI_SKT_UPD | (MAILBOX_BIOS_CMD_MULTI_SKT_UPD_INTERFACE_SUB_COMMAND_LATE_ACTION_REG_UPD << MAILBOX_BIOS_CMD_MULTI_SKT_UPD_INTERFACE_SUB_COMMAND_LSB);
  PcodeMailboxData = 0;
  PcodeMailBoxStatus = ReadModifyWritePcuMailbox (
                         Socket,
                         MAILBOX_BIOS_CMD_ZERO,
                         PcodeMailboxCommand,
                         &PcodeMailboxData,
                         0,
                         0
                         );
  if (PcodeMailBoxStatus != MAILBOX_BIOS_CC_NO_ERROR) {
    RcDebugPrint (SDBG_DEFAULT, PCU_MBOX_ERROR_MESSAGE, MAILBOX_BIOS_CMD_MULTI_SKT_UPD, PcodeMailBoxStatus);
    Status = EFI_DEVICE_ERROR;
  }

  return Status;
}

/**
  Read MC frequency information

  @param[in]    Socket                 0 based Socket number.
  @param[in]    MemType                PCU Memory type.
  @param[in]    McIdMask               McID mask to read,
                                       Bit 0 => McId0, Bit 1 => McId1 and so on.
                                       If bit is set, corresponding MC info is read.
  @param[out]   *RefClkType            Reference clock type.
  @param[out]   *McQclkRatio           MC QCLK Ratio Encoding.
  @param[out]   *MaxFreq               Max Frequency Supported. 0xFF for this API returning invalid value.

  @retval EFI_SUCCESS    Function execution successful.
  @retval !EFI_SUCCESS   Error on command execution.
**/
EFI_STATUS
EFIAPI
PcuReadMcFreq (
  IN UINT8 Socket,
  IN PCU_MEM_TYPE MemType,
  IN UINT8 McIdMask,
  OUT UINT8 *RefClkType,
  OUT UINT8 *McQclkRatio,
  OUT UINT8 *MaxFreq
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  UINT32 PcodeMailboxStatus;
  UINT32 MailboxData = 0;

  switch (MemType) {
    case PCU_MEM_HBM:
      MailboxData |= MAILBOX_BIOS_CMD_READ_MC_FREQ_DATA_MEMORY_TYPE_HBM << MAILBOX_BIOS_CMD_READ_MC_FREQ_DATA_MEMORY_TYPE_LSB;
      break;
    case PCU_MEM_DDR4:
      MailboxData |= MAILBOX_BIOS_CMD_READ_MC_FREQ_DATA_MEMORY_TYPE_DDR4 << MAILBOX_BIOS_CMD_READ_MC_FREQ_DATA_MEMORY_TYPE_LSB;
      break;
    case PCU_MEM_DDR5:
      MailboxData |= MAILBOX_BIOS_CMD_READ_MC_FREQ_DATA_MEMORY_TYPE_DDR5 << MAILBOX_BIOS_CMD_READ_MC_FREQ_DATA_MEMORY_TYPE_LSB;
      break;
    default:
      Status = EFI_INVALID_PARAMETER;
  }
  if (EFI_ERROR (Status)) {
    return Status;
  }
  MailboxData |= McIdMask << MAILBOX_BIOS_CMD_READ_MC_FREQ_DATA_MC_INSTANCE_LSB;
  PcodeMailboxStatus = ReadModifyWritePcuMailbox (
                         Socket,
                         MAILBOX_BIOS_CMD_READ_MC_FREQ,
                         MAILBOX_BIOS_CMD_ZERO,
                         &MailboxData,
                         0,
                         0
                         );
  if (PcodeMailboxStatus == MAILBOX_BIOS_CC_NO_ERROR) {
    //
    // The ratio got from PCU is QCLK ratio.
    //
    *McQclkRatio = (UINT8) MailboxData & 0x3F;

    *RefClkType = (UINT8) ((MailboxData & 0x40) >> 6);
  } else if (PcodeMailboxStatus == MAILBOX_BIOS_CC_ILLEGAL_DATA) {
    //
    // The READ_MC_FREQ mailbox command will return CC_ILLEGAL_DATA if SET_MC_FREQ has not been called
    // so this error code is not indicative of a real error.  Instead the assumption is that the current
    // ratio is 0.
    //
    *McQclkRatio = 0;
    *RefClkType = 0;
  } else {
    RcDebugPrint (SDBG_DEFAULT, "PCU: API -> PcuReadMcFreq, Command->MAILBOX_BIOS_CMD_READ_MC_FREQ, Error Code -> %x", PcodeMailboxStatus);
    Status = EFI_DEVICE_ERROR;
  }
  //
  // Note that the MaxFreq is not available in the READ_MC_FREQ command data for 10nm products but the interface is
  // here for historical reasons. We will return invalid frequency value, the caller has to verify if the value is valid.
  *MaxFreq = PCU_INVALID_FREQUENCY;

  return Status;
}
