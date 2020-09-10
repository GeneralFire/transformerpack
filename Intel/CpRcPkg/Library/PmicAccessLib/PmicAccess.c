/** @file
  Low level functions to access On-Dimm-Pmic device for DDR5.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2006 - 2020 Intel Corporation. <BR>

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

#include "SysHost.h"
#include "PmicRegs.h"
#include <Library/MemoryCoreLib.h>
#include <Library/ProcSmbIpLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/MemMcIpLib.h>
#include <Library/SysHostPointerLib.h>
#include <Library/IoAccessLib.h>
#include <Memory/JedecDefinitions.h>
#include <Library/SpdDecodeLib.h>
#include <Library/MemRcLib.h>

#include <ReferenceCodeFatalErrors.h>

//
// Possible values for PWR_GOOD_IO_TYPE in R32.
//
#define PWR_GOOD_IO_TYPE_OUTPUT_ONLY      0
#define PMIC_PowerOn_GuardBand            1

//
// Vout Definition
//
#define Vout_0900mV   0
#define Vout_1000mV   1
#define Vout_1100mV   2
#define Vout_1200mV   3

/**

  Apply WA (VOUT 1.2V LDO Voltage) for Rambus RCD D0/D1

  @param[in] Dev     - Device address on SMBUS
  @param[in] Socket  - Socket number
  @param[in] Channel - Channel number
  @param[in] Dimm    - DIMM number

  @retval N/A

**/
VOID
RambusD0D1Workaround (
  IN SMB_DEVICE_STRUCT  Dev,
  IN UINT8              Socket,
  IN UINT8              Channel,
  IN UINT8              Dimm
  );

/**

  Clear Global Status

  @param[in] Dev     - Device address on SMBUS
  @param[in] Socket  - Socket number
  @param[in] Channel - Channel number
  @param[in] Dimm    - DIMM number

  @retval EFI_SUCCESS

**/
EFI_STATUS
ClearGlobalStatus (
  IN SMB_DEVICE_STRUCT  Dev,
  IN UINT8              Socket,
  IN UINT8              Channel,
  IN UINT8              Dimm
  );

/**

  Enables the PMIC on the specified DIMM

  @param[in] Socket  - Socket number
  @param[in] Channel - Channel number
  @param[in] Dimm    - DIMM number

  @retval EFI_SUCCESS       - Pmic is enabled successfully
  @retval EFI_DEVICE_ERROR  - Pmic is not enabled

**/
EFI_STATUS
EFIAPI
PmicEnable (
  IN UINT8  Socket,
  IN UINT8  Channel,
  IN UINT8  Dimm
  )
{
  PSYSHOST            Host;
  SMB_DEVICE_STRUCT   Dev;
  UINT8               tPmic_Pwr_Good_Out;
  UINT8               R2c_swa_decode;
  UINT8               R2d_swc_decode;
  UINT8               R2d_swd_decode;
  UINT8               R40_decode;
  UINT8               R41_decode;
  PMIC_REG_2C_STRUCT  PmicR2c;
  PMIC_REG_2D_STRUCT  PmicR2d;
  PMIC_REG_32_STRUCT  PmicR32;
  PMIC_REG_40_STRUCT  PmicR40;
  PMIC_REG_41_STRUCT  PmicR41;
  struct dimmNvram    (*DimmNvList) [MAX_DIMM];

  Host = GetSysHostPointer ();
  DimmNvList = GetDimmNvList (Host, Socket, Channel);

  Dev.compId = SPD;
  Dev.address.controller = PLATFORM_SMBUS_CONTROLLER_PROCESSOR;
  Dev.address.deviceType = DTI_PMIC;

  if (EFI_ERROR (GetSmbAddress (Socket, Channel, Dimm, &Dev))) {
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_147);
    return EFI_DEVICE_ERROR;
  }

  if (ReadProcSmb (Dev, PMIC_REG_32, &PmicR32.Data) != EFI_SUCCESS) {
    RcDebugPrintWithDevice (
      SDBG_DEFAULT, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "PMIC Enable Failure: cannot read SMBus\n"
    );
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_97);
    return EFI_DEVICE_ERROR;
  }

  //
  // Apply WA (VOUT 1.2V LDO Voltage) for Rambus RCD D0/D1
  //
  RambusD0D1Workaround (Dev, Socket, Channel, Dimm);

  //
  // Clear Global Status
  //
  ClearGlobalStatus (Dev, Socket, Channel, Dimm);

  //
  // Compute tPmic_Pwr_Good_Out
  //
  if ((ReadProcSmb (Dev, PMIC_REG_2D, &PmicR2d.Data) != EFI_SUCCESS) ||
      (ReadProcSmb (Dev, PMIC_REG_40, &PmicR40.Data) != EFI_SUCCESS) ||
      (ReadProcSmb (Dev, PMIC_REG_41, &PmicR41.Data) != EFI_SUCCESS) ||
      (ReadProcSmb (Dev, PMIC_REG_2C, &PmicR2c.Data) != EFI_SUCCESS)) {
    RcDebugPrintWithDevice (
      SDBG_DEFAULT, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "PMIC Enable Failure: cannot read Power-on timing over SMBus\n"
    );
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_97);
    return EFI_DEVICE_ERROR;
  }

  //
  // Refer to PMIC_REG_2c and PMIC_REG_2d for decoding
  //
  R2c_swa_decode = (PmicR2c.Bits.swa_output_soft_start_time == 0) ? 1 : (2 * PmicR2c.Bits.swa_output_soft_start_time);
  R2d_swc_decode = (PmicR2d.Bits.swc_output_soft_start_time == 0) ? 1 : (2 * PmicR2d.Bits.swc_output_soft_start_time);
  R2d_swd_decode = (PmicR2d.Bits.swd_output_soft_start_time == 0) ? 1 : (2 * PmicR2d.Bits.swd_output_soft_start_time);

  //
  // Refer to PMIC_REG_40 for decoding
  //
  if (PmicR40.Bits.power_on_sequence_config0_idle < 7) {
    R40_decode = (PmicR40.Bits.power_on_sequence_config0_idle == 0) ? 0 : (2 * PmicR40.Bits.power_on_sequence_config0_idle);
  } else {
    R40_decode = 24;
  }

  //
  // Refer to PMIC_REG_41 for decoding
  //
  if (PmicR41.Bits.power_on_sequence_config1_idle < 7) {
    R41_decode = (PmicR41.Bits.power_on_sequence_config1_idle == 0) ? 0 : (2 * PmicR41.Bits.power_on_sequence_config1_idle);
  } else {
    R41_decode = 24;
  }

  tPmic_Pwr_Good_Out = R2c_swa_decode + R2d_swc_decode + R2d_swd_decode + R40_decode + R41_decode;

  PmicR32.Bits.vr_enable = 1;
  PmicR32.Bits.pwr_good_io_type = 1;

  if (IsBrsPresent (Host, Socket)) {
    PmicR32.Bits.pwr_good_io_type = PWR_GOOD_IO_TYPE_OUTPUT_ONLY;
  }

  if (WriteProcSmb (Dev, PMIC_REG_32, &PmicR32.Data) != EFI_SUCCESS) {
    RcDebugPrintWithDevice (
      SDBG_DEFAULT, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "PMIC Enable Failure: cannot write SMBus\n"
    );
    RC_FATAL_ERROR (FALSE, ERR_RC_INTERNAL, RC_FATAL_ERROR_MINOR_CODE_97);
    return EFI_DEVICE_ERROR;
  }

  //Wait for tPmic_Pwr_Good_Out
  FixedDelayMicroSecond ((tPmic_Pwr_Good_Out + PMIC_PowerOn_GuardBand) * 1000);

  return EFI_SUCCESS;
} // PmicEnable

/**

  Apply WA (VOUT 1.2V LDO Voltage) for Rambus RCD D0/D1

  @param[in] Dev     - Device address on SMBUS
  @param[in] Socket  - Socket number
  @param[in] Channel - Channel number
  @param[in] Dimm    - DIMM number

  @retval N/A

**/
VOID
RambusD0D1Workaround (
  IN SMB_DEVICE_STRUCT  Dev,
  IN UINT8              Socket,
  IN UINT8              Channel,
  IN UINT8              Dimm
  )
{
  PSYSHOST            Host;
  struct dimmNvram    (*DimmNvList)[MAX_DIMM];
  UINT8               RcdRevision;
  UINT16              RcdVendor;
  PMIC_REG_2B_STRUCT  PmicR2b;

  Host = (PSYSHOST) GetSysHostPointer ();
  DimmNvList = GetDimmNvList (Host, Socket, Channel);

  RcdVendor = (*DimmNvList)[Dimm].SPDRegVen;
  RcdRevision = (*DimmNvList)[Dimm].SPDRegRev;

  if ((RcdVendor == MFGID_RAMBUS) && ((RcdRevision == RAMBUS_SPD_REGREV_D0) || (RcdRevision == RAMBUS_SPD_REGREV_D1))) {
    if (ReadProcSmb (Dev, PMIC_REG_2B, &PmicR2b.Data) == EFI_SUCCESS) {
      //
      // The host must adjust this register one step at a time (0.1 V increment or decrement) to prevent false trigger of power good
      // status and PWR_GOOD pin assertion. In other words, host should not increment or decrement 0.2 V or 0.3 V from its current setting
      //
      while (PmicR2b.Bits.vout_1100mV_voltage_setting != Vout_1200mV) {
        PmicR2b.Bits.vout_1100mV_voltage_setting += 1;
        if (WriteProcSmb (Dev, PMIC_REG_2B, &PmicR2b.Data) == EFI_SUCCESS) {
          RcDebugPrintWithDevice (SDBG_DEFAULT, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
          "Applying 1.2V LDO Voltage for Rambus RCD\n");
        } else {
         RcDebugPrintWithDevice (SDBG_DEFAULT, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
         "RambusD0D1Workaround Failure: cannot write LDO Voltage over SMBus\n");
         break;
        }
      }
    } else {
      RcDebugPrintWithDevice (SDBG_DEFAULT, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "RambusD0D1Workaround Failure: cannot read LDO Voltage over SMBus\n");
    }
  }
} // RambusD0D1Workaround

/**

  Clear Global Status

  @param[in] Dev     - Device address on SMBUS
  @param[in] Socket  - Socket number
  @param[in] Channel - Channel number
  @param[in] Dimm    - DIMM number

  @retval EFI_SUCCESS

**/
EFI_STATUS
ClearGlobalStatus (
  IN SMB_DEVICE_STRUCT  Dev,
  IN UINT8              Socket,
  IN UINT8              Channel,
  IN UINT8              Dimm
  )
{
  PMIC_REG_08_STRUCT    PmicR08;
  PMIC_REG_14_STRUCT    PmicR14;

  //
  // Clear Global Status
  //
  PmicR14.Data = 0;
  PmicR14.Bits.global_clear_status = 1;
  if (WriteProcSmb (Dev, PMIC_REG_14, &PmicR14.Data) == EFI_SUCCESS) {
    ReadProcSmb (Dev, 0x08, &PmicR08.Data);
    RcDebugPrintWithDevice (SDBG_DEFAULT, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "Clear Global Status is applied successfully, PmicR08 = 0x%x\n", PmicR08.Data);
  } else {
    RcDebugPrintWithDevice (SDBG_DEFAULT, Socket, Channel, Dimm, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
      "Clear Global Status is not able to apply since smbus write failed!\n");
  }

  return EFI_SUCCESS;
} // ClearGlobalStatus