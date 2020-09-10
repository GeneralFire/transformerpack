/** @file
  PMIC Register Definitions

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

#ifndef  _PMIC_REGS_H
#define  _PMIC_REGS_H

//
// PMIC Spec Rev 0.8, the header file only contains PMIC registers that MRC consume
//

#define PMIC_REG_04 0x04
typedef union {
  struct {
    UINT8 reserved : 4;
    /* R04 [3:0]: Reserved */

    UINT8 global_error_log_critical_temperature : 1;
    /* R04 [4]: GLOBAL_ERROR_LOG_CRITICAL_TEMPERATURE
       Global Error Log History for Critical Temperature3
       0 = No Error
       1 = Error
    */

    UINT8 global_error_log_vin_bulk_over_votlage : 1;
    /* R04 [5]: GLOBAL_ERROR_LOG_VIN_BULK_OVER_VOTLAGE
       Global Error Log History for VIN_BULK Over Voltage3
       0 = No Error
       1 = Error
    */

    UINT8 global_error_log_buck_ov_or_uv : 1;
    /* R04 [6]: GLOBAL_ERROR_LOG_BUCK_OV_OR_UV
       Global Error Log History for Buck Regulator Output Over Voltage or Under Voltage
       0 = No Error
       1 = Error
    */

    UINT8 global_error_count : 1;
    /* R04 [7]: GLOBAL_ERROR_COUNT
       Global Error Count Since Last Erase Operation
       0 = No Error or Only 1 Error Since Last Erase Operation
       1 = > 1 Error Count since last Erase Operation
    */
  } Bits;
  UINT8 Data;
} PMIC_REG_04_STRUCT;

#define PMIC_REG_05 0x05
typedef union {
  struct {
    UINT8 pmic_error_log : 3;
    /* R05 [2:0]: PMIC_ERROR_LOG
       PMIC Power On - High Level Status Bit to Indicate previous power down cycle
       000 = Normal Power Down
       001 = Reserved
       010 = Buck Regulator Output Over or Under Voltage Lockout
       011 = Critical Temperature
       100 = VIN_BULK Input Over Voltage
       101 = Reserved
       110 = Reserved
       111 = Reserved
    */

    UINT8 swd_power_good : 1;
    /* R05 [3]: SWD_POWER_GOOD
       0 = Normal Power On
       1 = SWD Power Not Good
    */

    UINT8 swc_power_good : 1;
    /* R05 [4]: SWC_POWER_GOOD
       0 = Normal Power On
       1 = SWC Power Not Good
    */

    UINT8 swb_power_good : 1;
    /* R05 [5]: SWB_POWER_GOOD
       0 = Normal Power On
       1 = SWB Power Not Good
    */

    UINT8 swa_power_good : 1;
    /* R05 [6]: SWA_POWER_GOOD
       0 = Normal Power On
       1 = SWA Power Not Good
    */

    UINT8 reserved : 1;
    /* R05 [7]: Reserved */
  } Bits;
  UINT8 Data;
} PMIC_REG_05_STRUCT;

#define PMIC_REG_08 0x08
typedef union {
  struct {
    UINT8 vin_bulk_input_over_voltage_status : 1;
    /* R08 [0]: VIN_BULK_INPUT_OVER_VOLTAGE_STATUS
       VIN_Bulk Input Supply Over Voltage Status
       0 = No Over Voltage
       1 = Over Voltage
    */

    UINT8 vin_mgmt_input_over_voltage_status : 1;
    /* R08 [1]: VIN_MGMT_INPUT_OVER_VOLTAGE_STATUS
       VIN_Mgmt Input Supply Over Voltage Status
       0 = No Over Voltage
       1 = Over Voltage
    */

    UINT8 swd_output_power_good_status : 1;
    /* R08 [2]: SWD_OUTPUT_POWER_GOOD_STATUS
       Switch Node D Output Power Good Status
       0 = Power Good
       1 = Power Not Good
    */

    UINT8 swc_output_power_good_status : 1;
    /* R08 [3]: SWC_OUTPUT_POWER_GOOD_STATUS
       Switch Node C Output Power Good Status
       0 = Power Good
       1 = Power Not Good
    */

    UINT8 swb_output_power_good_status : 1;
    /* R08 [4]: SWB_OUTPUT_POWER_GOOD_STATUS
       Switch Node B Output Power Good Status
       0 = Power Good
       1 = Power Not Good
    */

    UINT8 swa_output_power_good_status : 1;
    /* R08 [5]: SWA_OUTPUT_POWER_GOOD_STATUS
       Switch Node A Output Power Good Status
       0 = Power Good
       1 = Power Not Good
    */

    UINT8 critical_temp_shutdown_status : 1;
    /* R08 [6]: CRITICAL_TEMP_SHUTDOWN_STATUS
       Critical Temperature Shutdown Status
       0 = No Critical Temperature Shutdown
       1 = Critical Temperature Shutdown
    */

    UINT8 vin_bulk_input_power_good_status : 1;
    /* R08 [7]: VIN_BULK_INPUT_POWER_GOOD_STATUS
       VIN_Bulk Input Power Good Status
       0 = Power Good
       1 = Power Not Good
    */
  } Bits;
  UINT8 Data;
} PMIC_REG_08_STRUCT;

#define PMIC_REG_14 0x14
typedef union {
  struct {
    UINT8 global_clear_status : 1;
    /* R14 [0]: GLOBAL_CLEAR_STATUS
       Clear all status bits.
       1 = Clear all status bits
    */
    UINT8 reserved_1 : 1;
    UINT8 clear_vout_1v_output_power_good_status : 1;
    /* R14 [2]: CLEAR_VOUT_1.01V_OUTPUT_POWER_GOOD_STATUS
       Clear VOUT_1.01V Output Power Good Status.
       1 = Clear Register Table 143, “Register 0x33” [2]
    */
    UINT8 clear_vbias_output_or_vin_bulk_under_voltage_lockout_status : 1;
    /* R14 [3]:
       CLEAR_VBIAS_OUTPUT_OR_VIN_BULK_UNDER_VOLTAGE_LOCKOUT_STATUS
       Clear VBias Output or VIN_Bulk Input Under Voltage Lockout Status.
       1 = Clear Register Table 143, “Register 0x33” [3]
    */
    UINT8 clear_vin_mgmt_power_good_status_switchover_mode : 1;
    /* R14 [3]:
       CLEAR_VBIAS_OUTPUT_OR_VIN_BULK_UNDER_VOLTAGE_LOCKOUT_STATUS
       Clear VBias Output or VIN_Bulk Input Under Voltage Lockout Status.
       1 = Clear Register Table 143, “Register 0x33” [3]
    */
    UINT8 reserved_5 : 3;
  } Bits;
  UINT8 Data;
} PMIC_REG_14_STRUCT;

#define PMIC_REG_2B 0x2B
typedef union {
  struct {
    UINT8 swd_voltage_range : 1;
    /* R2B [0]: SWD_VOLTAGE_RANGE
       SWD Output Voltage Range Selection
       0 = Range 1500mV to 2135mv for SWD - 5mV step size
       1 = Range 2200mV to 2835mv for SWD - 5mV step size
    */

    UINT8 vout_1100mV_voltage_setting : 2;
    /* R2B [2:1]: VOUT_1.1V_VOLTAGE_SETTING
       VOUT 1.1V LDO Output Voltage Setting
       00 = 0.9V
       01 = 1.0V
       10 = 1.1V
       11 = 1.2V
    */

    UINT8 swc_voltage_range : 1;
    /* R2B [3]: SWC_VOLTAGE_RANGE
       SWC Output Voltage Range Selection
       0 = Range 800mV to 1435mv for SWC - 5mV step size
       1 = Range 600mV to 1235mv for SWC - 5mV step size
    */

    UINT8 swb_voltage_range : 1;
    /* R2B [4]: SWB_VOLTAGE_RANGE
       SWB Output Voltage Range Selection
       0 = Range 800mV to 1435mv for SWB - 5mV step size
       1 = Range 600mV to 1235mv for SWB - 5mV step size
    */

    UINT8 swa_voltage_range : 1;
    /* R2B [5]: SWA_VOLTAGE_RANGE
       SWA Output Voltage Range Selection
       0 = Range 800mV to 1435mv for SWA - 5mV step size
       1 = Range 600mV to 1235mv for SWA - 5mV step size
    */

    UINT8 vout_1800mV_voltage_setting : 2;
    /* R2B [7:6]: VOUT_1.8V_VOLTAGE_SETTING
       VOUT 1.8V LDO Output Voltage Setting
       00 = 1.7V
       01 = 1.8V
       10 = 1.9V
       11 = Reserved
    */
  } Bits;
  UINT8 Data;
} PMIC_REG_2B_STRUCT;

#define PMIC_REG_2C 0x2C
typedef union {
  struct {
    UINT8 reserved_0 : 1;
    /* R2C [0]: Reserved */

    UINT8 swb_output_soft_start_time : 3;
    /* R2C [3:1]: SWB_OUTPUT_SOFT_START_TIME
       SWB Output Regulator Soft Start Time After VR Enable3
       000 = 1 ms
       001 = 2 ms (Default)
       010 = 4 ms
       011 = 6 ms
       ...
       111 = 14 ms
    */

    UINT8 reserved_4 : 1;
    /* R2C [4]: Reserved */

    UINT8 swa_output_soft_start_time : 3;
    /* R2C [7:5]: SWC_OUTPUT_SOFT_START_TIME
       SWA Output Regulator Soft Start Time After VR Enable3
       000 = 1 ms
       001 = 2 ms (Default)
       010 = 4 ms
       011 = 6 ms
       ...
       111 = 14 ms
    */
  } Bits;
  UINT8 Data;
} PMIC_REG_2C_STRUCT;

#define PMIC_REG_2D 0x2D
typedef union {
  struct {
    UINT8 reserved_0 : 1;
    /* R2D [0]: Reserved */

    UINT8 swd_output_soft_start_time : 3;
    /* R2D [3:1]: SWD_OUTPUT_SOFT_START_TIME
       SWD Output Regulator Soft Start Time After VR Enable3
       000 = 1 ms
       001 = 2 ms (Default)
       010 = 4 ms
       011 = 6 ms
       ...
       111 = 14 ms
    */

    UINT8 reserved_4 : 1;
    /* R2D [4]: Reserved */

    UINT8 swc_output_soft_start_time : 3;
    /* R2D [7:5]: SWC_OUTPUT_SOFT_START_TIME
       SWC Output Regulator Soft Start Time After VR Enable3
       000 = 1 ms
       001 = 2 ms (Default)
       010 = 4 ms
       011 = 6 ms
       ...
       111 = 14 ms
    */
  } Bits;
  UINT8 Data;
} PMIC_REG_2D_STRUCT;

#define PMIC_REG_2F 0x2F
typedef union {
  struct {
    UINT8 mask_bits_register_control : 2;
    /* R2F [1:0]: MASK_BITS_REGISTER_CONTROL
       Mask Bits Register Control
       00 = Mask GSI_n Signal Only - PWR_GOOD Signal will de-assert
       01 = Mask PWR_GOOD Signal Only - GSI_n Signal will assert
       01 = Mask GSI_n and PWR_GOOD Signal Only - neither PWR_GOOD de-assert or GSI_n Signal will assert
       11 = Reserved
    */

    UINT8 secure_mode : 1;
    /* R2F [2]: SECURE_MODE
       PMIC Mode Operation
       0 = Secure Mode Operation
       1 = Programmable Mode Operation
    */

    UINT8 swd_regulator_control : 1;
    /* R2F [3]: SWD_REGULATOR_CONTROL
       Disable SWD Regulator Output
       0 = Disable Switch Node D Output Regulator
       1 = Enable Switch Node D Output Regulator
    */

    UINT8 swc_regulator_control : 1;
    /* R2F [4]: SWC_REGULATOR_CONTROL
       Disable SWC Regulator Output
       0 = Disable Switch Node C Output Regulator
       1 = Enable Switch Node C Output Regulator
    */

    UINT8 swb_regulator_control : 1;
    /* R2F [5]: SWB_REGULATOR_CONTROL
       Disable SWB Regulator Output
       0 = Disable Switch Node B Output Regulator
       1 = Enable Switch Node B Output Regulator
    */

    UINT8 swa_regulator_control : 1;
    /* R2F [6]: SWA_REGULATOR_CONTROL
       Disable SWA Regulator Output
       0 = Disable Switch Node A Output Regulator
       1 = Enable Switch Node A Output Regulator
    */

    UINT8 vin_mgmt_input_supply_switchover_threshold : 1;
    /* R2F [7]: VIN_MGNT_INPUT_SUPPLY_SWITCHOVER_THRESHOLD
       VIN_Mgmt Input Supply Switchover Voltage Threshold to VIN_Buik Input Supply
       0 = Vendor Specific
       1 = Reserved
    */
  } Bits;
  UINT8 Data;
} PMIC_REG_2F_STRUCT;

#define PMIC_REG_32 0x32
typedef union {
  struct {
    UINT8 reserved : 3;
    /* Reserved
       R32 [2:0]: Reserved
    */

    UINT8 pmic_pwr_good_0utput_signal_control : 2;
    /* R32 [4:3]: PWR_GOOD_OUTPUT_SIGNAL_CONTROL
       0x = pwr_good_io_type
       10 = PWR_GOOD Output Low
       11 = PWR_GOOD Output Float
    */

    UINT8 pwr_good_io_type : 1;
    /* R32 [5]: PWR_GOOD_IO_TYPE
       0 = Output only
       1 = Input and Output
    */

    UINT8 management_interface_selection : 1;
    /* R32 [6]: MANAGEMENT_INTERFACE_SELECTION
       PMIC Management Bus Interface Protocol Selection
       0 = I2C Protocol (Max speed 1 MHz)
       1 = I3C Basic Protocol
    */

    UINT8 vr_enable : 1;
    /* R32 [7]: VR_ENABLE
       PMIC Enable
       0 = PMIC Disable
       1 = PMIC Enable
    */
  } Bits;
  UINT8 Data;
} PMIC_REG_32_STRUCT;

#define PMIC_REG_3B 0x3B
typedef union {
  struct {
    UINT8 pmic_part_capability : 1;
    /* R3B [0]: PMIC_PART_CAPABILITY
       PMIC Current Capability. See also bits [7:6] definition.
       0 = Small PMIC (Low Current)
       1 = Big PMIC (High Current)
    */

    UINT8 revision_id_minor_stepping : 3;
    /* R3B [3:1]: REVISION_ID_MINOR_STEPPING
       Minor Revision Stepping
       000 = Revision 0
       001 = Revision 1
       010 = Revision 2
       011 = Revision 3
       All other encodings are reserved
    */

    UINT8 revision_id_major_stepping : 2;
    /* R3B [5:4]: REVISION_ID_MAJOR_STEPPING
       Major Revision Stepping
       00 = Revision 1
       01 = Revision 2
       10 = Revision 3
       11 = Revision 4
    */

    UINT8 pmic_part_capability_ext : 2;
    /* R3B [7:6]: PMIC_PART_CAPABILITY_EXT
       PMIC Current Capability Extension. This register bits [7:6] and bit [0]
       provides 3 bit encoding as following:
       000 = Small PMIC (Low Current)
       001 = Big PMIC (High Current)
       01x = Extreme PMIC (Highest Current)
       All other encodings are reserved
    */
  } Bits;
  UINT8 Data;
} PMIC_REG_3B_STRUCT;

#define PMIC_REG_3C 0x3C
typedef union {
  struct {
    UINT8 vendor_id_byte0 : 8;
    /* R3C [7:0]: VENDOR_ID_BYTE0
       Vendor Identification Register Byte 0
    */
  } Bits;
  UINT8 Data;
} PMIC_REG_3C_STRUCT;

#define PMIC_REG_3D 0x3D
typedef union {
  struct {
    UINT8 vendor_id_byte1 : 8;
    /* R3D [7:0]: VENDOR_ID_BYTE1
       Vendor Identification Register Byte 1
    */
  } Bits;
  UINT8 Data;
} PMIC_REG_3D_STRUCT;

#define PMIC_REG_40 0x40
typedef union {
  struct {
    UINT8 power_on_sequence_config0_idle : 3;
    /* R40 [2:0]: POWER_ON_SEQUENCE_CONFIG0_IDLE
       Idle time after Power On Sequence Config 0
       000 = 0 ms
       001 = 2 ms (Default)
       010 = 4 ms
       011 = 6 ms
       100 = 8 ms
       101 = 10 ms
       110 = 12 ms
       111 = 24 ms
    */

    UINT8 power_on_sequence_config0_swd_enable : 1;
    /* R40 [3]: POWER_ON_SEQUENCE_CONFIG0_SWD_ENABLE
       Enable SWD Output Regulator.
       0 = Disable SWD Output Regulator
       1 = Enable SWD Output Regulator
    */

    UINT8 power_on_sequence_config0_swc_enable : 1;
    /* R40 [4]: POWER_ON_SEQUENCE_CONFIG0_SWC_ENABLE
       Enable SWC Output Regulator.
       0 = Disable SWC Output Regulator
       1 = Enable SWC Output Regulator
     */

    UINT8 power_on_sequence_config0_swb_enable : 1;
    /* R40 [5]: POWER_ON_SEQUENCE_CONFIG0_SWB_ENABLE
       Enable SWB Output Regulator.
       0 = Disable SWB Output Regulator
       1 = Enable SWB Output Regulator
    */

    UINT8 power_on_sequence_config0_swa_enable : 1;
    /* R40 [6]: POWER_ON_SEQUENCE_CONFIG0_SWA_ENABLE
       Enable SWA Output Regulator.
       0 = Disable SWA Output Regulator
       1 = Enable SWA Output Regulator
     */

    UINT8 power_on_sequence_config0: 1;
    /* R40 [7]: POWER_ON_SEQUENCE_CONFIG0
       PMIC Power On Sequence Config 0
       0 = Do Not Execute Config 0
       1 = Execute Config 0
    */
  } Bits;
  UINT8 Data;
} PMIC_REG_40_STRUCT;

#define PMIC_REG_41 0x41
typedef union {
  struct {
    UINT8 power_on_sequence_config1_idle : 3;
    /* R41 [2:0]: POWER_ON_SEQUENCE_CONFIG1_IDLE
       Idle time after Power On Sequence Config 1
       000 = 0 ms
       001 = 2 ms (Default)
       010 = 4 ms
       011 = 6 ms
       100 = 8 ms
       101 = 10 ms
       110 = 12 ms
       111 = 24 ms
    */

    UINT8 power_on_sequence_config1_swd_enable : 1;
    /* R41 [3]: POWER_ON_SEQUENCE_CONFIG1_SWD_ENABLE
       Enable SWD Output Regulator.
       0 = Disable SWD Output Regulator
       1 = Enable SWD Output Regulator
    */

    UINT8 power_on_sequence_config1_swc_enable : 1;
    /* R41 [4]: POWER_ON_SEQUENCE_CONFIG1_SWC_ENABLE
       Enable SWC Output Regulator.
       0 = Disable SWC Output Regulator
       1 = Enable SWC Output Regulator
     */

    UINT8 power_on_sequence_config1_swb_enable : 1;
    /* R41 [5]: POWER_ON_SEQUENCE_CONFIG1_SWB_ENABLE
       Enable SWB Output Regulator.
       0 = Disable SWB Output Regulator
       1 = Enable SWB Output Regulator
    */

    UINT8 power_on_sequence_config1_swa_enable : 1;
    /* R41 [6]: POWER_ON_SEQUENCE_CONFIG1_SWA_ENABLE
       Enable SWA Output Regulator.
       0 = Disable SWA Output Regulator
       1 = Enable SWA Output Regulator
     */

    UINT8 power_on_sequence_config1: 1;
    /* R41 [7]: POWER_ON_SEQUENCE_CONFIG1
       PMIC Power On Sequence Config 1
       0 = Do Not Execute Config 1
       1 = Execute Config 1
    */
  } Bits;
  UINT8 Data;
} PMIC_REG_41_STRUCT;

#endif   // _PMIC_REGS_H
