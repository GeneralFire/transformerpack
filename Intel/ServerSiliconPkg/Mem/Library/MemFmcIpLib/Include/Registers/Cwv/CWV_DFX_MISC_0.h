
/** @file
  CWV_DFX_MISC_0.h

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2007 - 2020 Intel Corporation. <BR>
  
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
  
  This file contains Silicon register definitions.
  
  This is a generated file; please do not modify it directly.
  
**/

/* The following security policy groups are used by registers in this file:     */

/* CWV Security Policy Groups:                                                  */


#ifndef _CWV_DFX_MISC_0_h
#define _CWV_DFX_MISC_0_h
#include <DataTypes.h>

/* TEMP_CSR_CWV_DFX_MISC_0_REG supported on:                                    */
/*      CWV (0x400c6000)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_msc/msc_csr                            */
/* Struct generated from CWV BDF: 0_24_6                                        */
/* CWV Security PolicyGroup:                                                    */
/* For debug purposes only, used for thermal tuning. Temperature value written by FW. Temperature is defined to match the encoding for RDIMM temperature sensors, and is expressed in two's complement format.
*/


#define TEMP_CSR_CWV_DFX_MISC_0_REG 0x1B740000

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 temperature : 12;

                            /* Bits[11:0], Access Type=RW, default=0x00000000*/

                            /*
                               12-bit data for the internal temperature
                               measurement with 1.0 C resolution (Can not
                               report 0.0625 C resolution due to the Thermal
                               circuit).[br] bits [3:0] - tied to '0' due to
                               the Thermal circuit provides 1.0 C resolution
                               [br]
                            */
    UINT32 sign : 1;

                            /* Bits[12:12], Access Type=RW, default=0x00000000*/

                            /* Sign bit for internal temperature measurement. */
    UINT32 overtemplow : 1;

                            /* Bits[13:13], Access Type=RW, default=0x00000000*/

                            /*
                               When set, the temperature is below the low limit
                               minus the hysteresis.[br] Once set, it will only
                               be cleared when the temperature meets or exceeds
                               the LOW limit.[br]
                            */
    UINT32 overtemphigh : 1;

                            /* Bits[14:14], Access Type=RW, default=0x00000000*/

                            /*
                               When set, the temperature is above the high
                               limit minus the hysteresis.[br] Once set, it
                               will only be cleared when the temperature drops
                               below below below or equal to the HIGHT limit
                               minus the hysteresis.[br]
                            */
    UINT32 overtempcrit : 1;

                            /* Bits[15:15], Access Type=RW, default=0x00000000*/

                            /*
                               When set, the temperature is above the critical
                               limit minus the hysteresis.[br] The bit will
                               remain set as long as the temperature is above
                               CRITICAL and will clear once the temperature
                               drops below the limit.[br]
                            */
    UINT32 id : 2;

                            /* Bits[17:16], Access Type=RW, default=0x00000000*/

                            /*
                               This indicates the temperature reported.[br]
                               2'b00: Normalized Temperature[br] 2'b01: Max SXP
                               Temperature[br] 2'b10: Valley Controller
                               Temperature
                            */
    UINT32 rsvd : 14;

                            /* Bits[31:18], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} TEMP_CSR_CWV_DFX_MISC_0_STRUCT;

/* SGX_A_CWV_DFX_MISC_0_REG supported on:                                       */
/*      CWV (0x400c6004)                                                        */
/* Register default value on CWV: 0xFFFFFFFF                                    */
/* CWV Register File:    fmc_top_csr/fnv_msc/msc_csr                            */
/* Struct generated from CWV BDF: 0_24_6                                        */
/* CWV Security PolicyGroup:                                                    */
/* Future SGX SVN usage
*/


#define SGX_A_CWV_DFX_MISC_0_REG 0x1B740004

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 svn_value : 32;

                            /* Bits[31:0], Access Type=RO/V/P, default=0xFFFFFFFF*/

                            /* contains the current SVN value */

  } Bits;
  UINT32 Data;

} SGX_A_CWV_DFX_MISC_0_STRUCT;

/* VENDOR_DEVICE_ID_CWV_DFX_MISC_0_REG supported on:                            */
/*      CWV (0x400c6008)                                                        */
/* Register default value on CWV: 0x8086097C                                    */
/* CWV Register File:    fmc_top_csr/fnv_msc/msc_csr                            */
/* Struct generated from CWV BDF: 0_24_6                                        */
/* CWV Security PolicyGroup:                                                    */
/* This register contains the Vendor/Device ID
*/


#define VENDOR_DEVICE_ID_CWV_DFX_MISC_0_REG 0x1B740008

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 device_id : 16;

                            /* Bits[15:0], Access Type=RO, default=0x0000097C*/

                            /*
                               Device ID.[br] FNV - 0x0979[br] EKV - 0x097A[br]
                               BWV - 0x097B[br] CWV - 0x097C
                            */
    UINT32 vendor_id : 16;

                            /* Bits[31:16], Access Type=RO, default=0x00008086*/

                            /* Vendor ID */

  } Bits;
  UINT32 Data;

} VENDOR_DEVICE_ID_CWV_DFX_MISC_0_STRUCT;

/* REVISION_MFG_ID_CWV_DFX_MISC_0_REG supported on:                             */
/*      CWV (0x400c600c)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_msc/msc_csr                            */
/* Struct generated from CWV BDF: 0_24_6                                        */
/* CWV Security PolicyGroup:                                                    */
/* MFG Stepping ID value.Revision ID control and values.
*/


#define REVISION_MFG_ID_CWV_DFX_MISC_0_REG 0x1B74000C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rev_id_sel : 2;

                            /* Bits[1:0], Access Type=RW/O/P, default=0x00000000*/

                            /*
                               Selects RevID[br] 00 : SRID (default)[br] 01 :
                               CRID[br] NOTE: In the current implementation,
                               CRID and SRID contain the same value. Meaning
                               RevID select programming will not change RevID
                               value.[br] USER WARNING: Host reads may
                               inadvertently lock this register in an undesired
                               mode as some reads are realized as R-Ws.[br]
                               Therefore, should CRID differ from SRID at some
                               point, FW needs to lock the appropriate mode
                               before host accesses this CSR, for example in FW
                               init.[br] The mode may be determined by a
                               revenue SKU or other similar function
                            */
    UINT32 revision_id : 8;

                            /* Bits[9:2], Access Type=RO/V, default=0x00000000*/

                            /*
                               OS accessible RevID selected by RevID Sel[br]
                               The 8-bit REVID is split into 2 sections:[br]
                               The following bits are Reserved, [7:6] and
                               [3:2].[br] Upper 2-bits [5:4], translates to
                               base layer stepping[br] 00 A-step[br] 01
                               S-step[br] 10 C-step[br] 11 D-step[br] Lower
                               2-bits [1:0], translates to metal layer
                               stepping[br] 00 Metal-step 0[br] 01 Metal-step
                               1[br] 10 Metal-step 2[br] 11 Metal-step 3[br]
                            */
    UINT32 rsvd : 22;

                            /* Bits[31:10], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} REVISION_MFG_ID_CWV_DFX_MISC_0_STRUCT;

/* FNV_ROM_CTRL_CWV_DFX_MISC_0_REG supported on:                                */
/*      CWV (0x400c6010)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_msc/msc_csr                            */
/* Struct generated from CWV BDF: 0_24_6                                        */
/* CWV Security PolicyGroup:                                                    */
/* Fuse controlled register to support FW.
*/


#define FNV_ROM_CTRL_CWV_DFX_MISC_0_REG 0x1B740010

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 fuse_val : 32;

                            /* Bits[31:0], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               Bit[0] - load_run_raw_bin_en : 0 = do not load
                               raw fw, 1 = ok to load raw fw[br] FUSE map -->
                               fw_dfx_fw_dbg_fw_fuses_rom_ctrl_0[br][br] Bit[1]
                               - dis_dbg_type_perpart_chk : 0 = enables per
                               part check, 1 = no per part check required[br]
                               FUSE map -->
                               fw_dfx_fw_dbg_fw_fuses_rom_ctrl_1[br][br] Bit[2]
                               - ROM_functional_flow : 0 = do provisioning
                               flow, 1 = do normal ROM flow[br] FUSE map -->
                               fw_dfx_fw_dbg_fw_fuses_rom_ctrl_2[br][br] Bit[3]
                               - ROM_code_use_release_sign_key : 1 = PRQ[br]
                               FUSE map -->
                               fw_dfx_fw_dbg_fw_fuses_rom_ctrl_3[br][br] Bit[4]
                               - ROM_code_use_debug_sign_key : 1 = Pre PRQ[br]
                               FUSE map -->
                               fw_dfx_fw_dbg_fw_fuses_rom_ctrl_4[br][br] Bit[5]
                               - fpf_mfg_enable : 0 = soft fuses/legacy, 1 =
                               use FPFs[br] FUSE map -->
                               fw_dfx_fw_dbg_fw_fuses_rom_ctrl_5[br][br] Bit[6]
                               - internal_fpf_powerswitch_enable : 0 = external
                               FET, 1 = internal IP[br] FUSE map -->
                               fw_dfx_fw_dbg_fw_fuses_rom_ctrl_6[br][br] Bit[7]
                               - dimm_mfg_done : 0 = ROM load signed mfg
                               FW(only if [5] = 1, else don't care)[br] 1 = 1 =
                               load signed PRQ or debug FW only (only if [5] =
                               1, else don't care)[br] FUSE map -->
                               fw_dfx_fw_dbg_fw_fuses_rom_ctrl_7 (OEM
                               Fuse)[br][br] Bit[8] - FIPS_disable : 0 = FIPS
                               ROM code enabled[br] 1 = FIPS ROM code
                               disabled[br] FUSE map -->
                               fw_dfx_fw_dbg_fw_fuses_rom_ctrl_8[br][br] Bit[9]
                               - Reserved for future use (HVM Spare Fuses)[br]
                               FUSE map -->
                               fw_dfx_fw_dbg_fw_fuses_rom_ctrl_9[br][br]
                               Bit[10] - Reserved for future use (HVM Spare
                               Fuses)[br] FUSE map -->
                               fw_dfx_fw_dbg_fw_fuses_rom_ctrl_10[br][br]
                               Bit[11] - Reserved for future use (HVM Spare
                               Fuses)[br] FUSE map -->
                               fw_dfx_fw_dbg_fw_fuses_rom_ctrl_11[br][br]
                               Bit[12] - Reserved for future use (HVM Spare
                               Fuses)[br] FUSE map -->
                               fw_dfx_fw_dbg_fw_fuses_rom_ctrl_12[br][br]
                               Bit[13] - Reserved for future use (HVM Spare
                               Fuses)[br] FUSE map -->
                               fw_dfx_fw_dbg_fw_fuses_rom_ctrl_13[br][br]
                               Bit[14] - Reserved for future use (HVM Spare
                               Fuses)[br] FUSE map -->
                               fw_dfx_fw_dbg_fw_fuses_rom_ctrl_14[br][br]
                               Bit[15] - Reserved for future use (HVM Spare
                               Fuses)[br] FUSE map -->
                               fw_dfx_fw_dbg_fw_fuses_rom_ctrl_15[br][br]
                               Bit[16] - Reserved for future use (HVM Spare
                               Fuses)[br] FUSE map -->
                               fw_dfx_fw_dbg_fw_fuses_rom_ctrl_16[br][br]
                               Bit[17] - Reserved for future use (HVM Spare
                               Fuses)[br] FUSE map -->
                               fw_dfx_fw_dbg_fw_fuses_rom_ctrl_17[br][br]
                               Bit[18] - Reserved for future use (HVM Spare
                               Fuses)[br] FUSE map -->
                               fw_dfx_fw_dbg_fw_fuses_rom_ctrl_18[br][br]
                               Bit[19] - Reserved for future use (HVM Spare
                               Fuses)[br] FUSE map -->
                               fw_dfx_fw_dbg_fw_fuses_rom_ctrl_19[br][br]
                               Bit[20] - Reserved for future use (OEM Spare
                               Fuses)[br] FUSE map -->
                               fw_dfx_fw_dbg_fw_fuses_rom_ctrl_20[br][br]
                               Bit[21] - Reserved for future use (OEM Spare
                               Fuses)[br] FUSE map -->
                               fw_dfx_fw_dbg_fw_fuses_rom_ctrl_21[br][br]
                               Bit[22] - Reserved for future use (OEM Spare
                               Fuses)[br] FUSE map -->
                               fw_dfx_fw_dbg_fw_fuses_rom_ctrl_22[br][br]
                               Bit[23] - Reserved for future use (OEM Spare
                               Fuses)[br] FUSE map -->
                               fw_dfx_fw_dbg_fw_fuses_rom_ctrl_23[br][br]
                               Bit[24] - Reserved for future use (OEM Spare
                               Fuses)[br] FUSE map -->
                               fw_dfx_fw_dbg_fw_fuses_rom_ctrl_24[br][br]
                               Bit[25] - Reserved for future use (OEM Spare
                               Fuses)[br] FUSE map -->
                               fw_dfx_fw_dbg_fw_fuses_rom_ctrl_25[br][br]
                               Bit[26] - Reserved for future use (OEM Spare
                               Fuses)[br] FUSE map -->
                               fw_dfx_fw_dbg_fw_fuses_rom_ctrl_26[br][br]
                               Bit[27] - Reserved for future use (OEM Spare
                               Fuses)[br] FUSE map -->
                               fw_dfx_fw_dbg_fw_fuses_rom_ctrl_27[br][br]
                               Bit[28] - Reserved for future use (OEM Spare
                               Fuses)[br] FUSE map -->
                               fw_dfx_fw_dbg_fw_fuses_rom_ctrl_28[br][br]
                               Bit[29] - Reserved for future use (OEM Spare
                               Fuses)[br] FUSE map -->
                               fw_dfx_fw_dbg_fw_fuses_rom_ctrl_29[br][br]
                               Bit[30] - Reserved for future use (OEM Spare
                               Fuses)[br] FUSE map -->
                               fw_dfx_fw_dbg_fw_fuses_rom_ctrl_30[br][br]
                               Bit[31] - Reserved for future use (OEM Spare
                               Fuses)[br] FUSE map -->
                               fw_dfx_fw_dbg_fw_fuses_rom_ctrl_31
                            */

  } Bits;
  UINT32 Data;

} FNV_ROM_CTRL_CWV_DFX_MISC_0_STRUCT;

/* FNV_PERPARTID_L_CWV_DFX_MISC_0_REG supported on:                             */
/*      CWV (0x400c6014)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_msc/msc_csr                            */
/* Struct generated from CWV BDF: 0_24_6                                        */
/* CWV Security PolicyGroup:                                                    */
/* Part number loaded from fuses (parsmisc_hvm:misc_rsrv_fuse_bits_6[31:0], and is same value as dfxagg:uniqueid_fuse[31:0]). Low register.
*/


#define FNV_PERPARTID_L_CWV_DFX_MISC_0_REG 0x1B740014

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 part_id : 32;

                            /* Bits[31:0], Access Type=RO/V/P, default=0x00000000*/

                            /* Part ID. Low register. */

  } Bits;
  UINT32 Data;

} FNV_PERPARTID_L_CWV_DFX_MISC_0_STRUCT;

/* FNV_PERPARTID_H_CWV_DFX_MISC_0_REG supported on:                             */
/*      CWV (0x400c6018)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_msc/msc_csr                            */
/* Struct generated from CWV BDF: 0_24_6                                        */
/* CWV Security PolicyGroup:                                                    */
/* Part number loaded from fuses (parsmisc_hvm:misc_rsrv_fuse_bits_7[31:0], and is same value as dfxagg:uniqueid_fuse[63:32]). High register.
*/


#define FNV_PERPARTID_H_CWV_DFX_MISC_0_REG 0x1B740018

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 part_id : 32;

                            /* Bits[31:0], Access Type=RO/V/P, default=0x00000000*/

                            /* Part ID. High register. */

  } Bits;
  UINT32 Data;

} FNV_PERPARTID_H_CWV_DFX_MISC_0_STRUCT;

/* FNV_SKU_NUM_CWV_DFX_MISC_0_REG supported on:                                 */
/*      CWV (0x400c601c)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_msc/msc_csr                            */
/* Struct generated from CWV BDF: 0_24_6                                        */
/* CWV Security PolicyGroup:                                                    */
/* SKU# loaded from fuses.
*/


#define FNV_SKU_NUM_CWV_DFX_MISC_0_REG 0x1B74001C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 val : 8;

                            /* Bits[7:0], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               SKU# loaded from fuses.[br] FUSE map -->
                               fw_dfx_fw_dbg_fw_fuses_SKU_number
                            */
    UINT32 rsvd : 24;

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} FNV_SKU_NUM_CWV_DFX_MISC_0_STRUCT;

/* FNV_FUSE_CTRL_CWV_DFX_MISC_0_REG supported on:                               */
/*      CWV (0x400c6020)                                                        */
/* Register default value on CWV: 0x00000200                                    */
/* CWV Register File:    fmc_top_csr/fnv_msc/msc_csr                            */
/* Struct generated from CWV BDF: 0_24_6                                        */
/* CWV Security PolicyGroup:                                                    */
/* Fuse controlled register to enable/disable features.
*/


#define FNV_FUSE_CTRL_CWV_DFX_MISC_0_REG 0x1B740020

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 jtag_tunneling_dbg_en : 1;

                            /* Bits[0:0], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               DFX Features Disabled.[br] FUSE map -->
                               fw_dfx_fw_dbg_fw_fuses_jtag_tunneling_dbg_en
                            */
    UINT32 cr_locked : 1;

                            /* Bits[1:1], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               Unused.[br] FUSE map -->
                               fw_dfx_fw_dbg_fw_fuses_CR_locked
                            */
    UINT32 dwe_false : 1;

                            /* Bits[2:2], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               Unused.[br] FUSE map -->
                               fw_dfx_fw_dbg_fw_fuses_DWE_False
                            */
    UINT32 jtag_port_dis_red_lk : 1;

                            /* Bits[3:3], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               Unused.[br] FUSE map -->
                               fw_dfx_fw_dbg_fw_fuses_JTAG_Port_Dis_Red_Lk
                            */
    UINT32 fnv_fuse_ctrl_spare : 1;

                            /* Bits[4:4], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               Unused.[br] FUSE map -->
                               fw_dfx_fw_dbg_fw_fuses_fnv_fuse_ctrl_spare_4
                            */
    UINT32 jtag_tunnel_dis : 1;

                            /* Bits[5:5], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               Repurposed for UART disable.[br] FUSE map -->
                               fw_dfx_fw_dbg_fw_fuses_jtag_tunnel_dis
                            */
    UINT32 spi_flash_update : 1;

                            /* Bits[6:6], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               Deprecated: (do not use) [br] SPI Flash update
                               enhancement for AEP HVM (ROM code Changes)[br]
                               FUSE map -->
                               fw_dfx_fw_dbg_fw_fuses_spi_flash_update
                            */
    UINT32 spi_speed : 2;

                            /* Bits[8:7], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               SPI_Speed - Default is 37.5 MHz speed [br] Bit
                               [8:7]: Specifies SPI frequency to run on [br] 00
                               - 37.5 MHz (calculated by ROM based on uclk freq
                               = 300 MHz) [br] 01 - 11.5 MHz (calculated by ROM
                               based on uclk freq = 300 MHz) [br] 10 - 50 MHz
                               (calculated by ROM based on uclk freq = 300 MHz)
                               [br] 11 - Reserved
                            */
    UINT32 spi_sampling : 2;

                            /* Bits[10:9], Access Type=RO/V/P, default=0x00000001*/

                            /*
                               Specifies SPI sampling delay to use[br] Bit
                               [10:9] - Sample_Delay - default is 1 uclk delay
                               [br] 0 = MISO sampling must be delayed by 0
                               uclks [br] 1 = MISO sampling must be delayed by
                               1 uclks [br] 2 = MISO sampling must be delayed
                               by 2 uclks [br] 3 = MISO sampling must be
                               delayed by 3 uclks
                            */
    UINT32 rsvd : 21;

                            /* Bits[31:11], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} FNV_FUSE_CTRL_CWV_DFX_MISC_0_STRUCT;

/* TSOD_MIRROR_CSR_CWV_DFX_MISC_0_REG supported on:                             */
/*      CWV (0x400c6024)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_msc/msc_csr                            */
/* Struct generated from CWV BDF: 0_24_6                                        */
/* CWV Security PolicyGroup:                                                    */
/* Register used for the internal SoC temperature measurement.  Copy of I3C TSOD value for DDRT access.
*/


#define TSOD_MIRROR_CSR_CWV_DFX_MISC_0_REG 0x1B740024

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 temperature : 32;

                            /* Bits[31:0], Access Type=RW, default=0x00000000*/

                            /*
                               Copy of temperature value written to the I3C
                               TSOD. JEDEC SPD5 format. [br] Temperature value
                               written by FW at same time as the I3C TSOD is
                               written. [br] This is required to allow DDRT
                               access to the temperature value since DDRT
                               cannot access I3C TSOD.
                            */

  } Bits;
  UINT32 Data;

} TSOD_MIRROR_CSR_CWV_DFX_MISC_0_STRUCT;

/* FNV_AEP_MFG_CWV_DFX_MISC_0_REG supported on:                                 */
/*      CWV (0x400c6028)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_msc/msc_csr                            */
/* Struct generated from CWV BDF: 0_24_6                                        */
/* CWV Security PolicyGroup:                                                    */
/* This register contains information for FNV in AEP MFG
*/


#define FNV_AEP_MFG_CWV_DFX_MISC_0_REG 0x1B740028

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 aep_in_mfg : 1;

                            /* Bits[0:0], Access Type=RW/O/P, default=0x00000000*/

                            /*
                               Indicates if FNV is currently in AEP MFG mode.
                               Red unlock override takes effect. [br] If AEP
                               disable Fuse
                               (fw_dfx_fw_dbg_fw_fuses_aep_mfg_ovrd_dis) is
                               burned to 1, programming this register will have
                               no affect. [br] 1=Red override[br] 0=Must shift
                               red password for unlock or use fuse
                            */
    UINT32 red_unlock : 1;

                            /* Bits[1:1], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               Indicates if FNV is currently in red
                               lock/unlock.[br] 0=Red LOCKED[br] 1=Red
                               UNLOCKED[br] Decoded Value for DFX AGG Policy
                               Bus; Policy is 2 or 4
                            */
    UINT32 red2_unlock : 1;

                            /* Bits[2:2], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               Indicates if FNV is currently in red
                               lock/unlock.[br] 0=Red2 LOCKED[br] 1=Red2
                               UNLOCKED[br] Decoded Value for DFX AGG Policy
                               Bus; Policy is 2
                            */
    UINT32 red4_unlock : 1;

                            /* Bits[3:3], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               Indicates if FNV is currently in red
                               lock/unlock.[br] 0=Red4 LOCKED[br] 1=Red4
                               UNLOCKED[br] Decoded Value for DFX AGG Policy
                               Bus; Policy is 4
                            */
    UINT32 dfx_policy_bus : 4;

                            /* Bits[7:4], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               Indicates if DFX Policy bus value without
                               decode.[br]
                            */
    UINT32 rsvd : 24;

                            /* Bits[31:8], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} FNV_AEP_MFG_CWV_DFX_MISC_0_STRUCT;

/* JTAG_PAYLOAD_0_CWV_DFX_MISC_0_REG supported on:                              */
/*      CWV (0x400c6030)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_msc/msc_csr                            */
/* Struct generated from CWV BDF: 0_24_6                                        */
/* CWV Security PolicyGroup:                                                    */
/* Payload register for JTAG tunneling through SMBus
*/


#define JTAG_PAYLOAD_0_CWV_DFX_MISC_0_REG 0x1B740030

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 jtag_data : 32;

                            /* Bits[31:0], Access Type=RW/V, default=0x00000000*/

                            /* Data */

  } Bits;
  UINT32 Data;

} JTAG_PAYLOAD_0_CWV_DFX_MISC_0_STRUCT;

/* JTAG_PAYLOAD_1_CWV_DFX_MISC_0_REG supported on:                              */
/*      CWV (0x400c6034)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_msc/msc_csr                            */
/* Struct generated from CWV BDF: 0_24_6                                        */
/* CWV Security PolicyGroup:                                                    */
/* Payload register for JTAG tunneling through SMBus
*/


#define JTAG_PAYLOAD_1_CWV_DFX_MISC_0_REG 0x1B740034

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 jtag_data : 32;

                            /* Bits[31:0], Access Type=RW/V, default=0x00000000*/

                            /* Data */

  } Bits;
  UINT32 Data;

} JTAG_PAYLOAD_1_CWV_DFX_MISC_0_STRUCT;

/* JTAG_PAYLOAD_2_CWV_DFX_MISC_0_REG supported on:                              */
/*      CWV (0x400c6038)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_msc/msc_csr                            */
/* Struct generated from CWV BDF: 0_24_6                                        */
/* CWV Security PolicyGroup:                                                    */
/* Payload register for JTAG tunneling through SMBus
*/


#define JTAG_PAYLOAD_2_CWV_DFX_MISC_0_REG 0x1B740038

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 jtag_data : 32;

                            /* Bits[31:0], Access Type=RW/V, default=0x00000000*/

                            /* Data */

  } Bits;
  UINT32 Data;

} JTAG_PAYLOAD_2_CWV_DFX_MISC_0_STRUCT;

/* JTAG_PAYLOAD_3_CWV_DFX_MISC_0_REG supported on:                              */
/*      CWV (0x400c603c)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_msc/msc_csr                            */
/* Struct generated from CWV BDF: 0_24_6                                        */
/* CWV Security PolicyGroup:                                                    */
/* Payload register for JTAG tunneling through SMBus
*/


#define JTAG_PAYLOAD_3_CWV_DFX_MISC_0_REG 0x1B74003C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 jtag_data : 32;

                            /* Bits[31:0], Access Type=RW/V, default=0x00000000*/

                            /* Data */

  } Bits;
  UINT32 Data;

} JTAG_PAYLOAD_3_CWV_DFX_MISC_0_STRUCT;

/* JTAG_TUNNEL_CTRL_CWV_DFX_MISC_0_REG supported on:                            */
/*      CWV (0x400c6040)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_msc/msc_csr                            */
/* Struct generated from CWV BDF: 0_24_6                                        */
/* CWV Security PolicyGroup:                                                    */
/* This register controls JTAG tunneling through SMBus or DDRT. JTAG_PAYLOAD registers are shifted into/out of the TAP port as indicated by the Shift Amt field.[br]
JTAG_PAYLOAD0[0] -> TDI[br]
TDO -> JTAG_PAYLOAD3[31][br]
For more details, refer to Valley Controller DFX UG Chapter 14.1.4.2[br]
*/


#define JTAG_TUNNEL_CTRL_CWV_DFX_MISC_0_REG 0x1B740040

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 start_jtag_cmd : 1;

                            /* Bits[0:0], Access Type=RW/V, default=0x00000000*/

                            /*
                               When set to 1, the command selected by the
                               jtag_tunnel_ctrl.cmd is initiated. Hardware
                               clears this bit when the operation is complete.
                            */
    UINT32 jtag_cmd : 3;

                            /* Bits[3:1], Access Type=RW, default=0x00000000*/

                            /*
                               000: IR Shift and Pause [br] 001: IR Shift and
                               Update [br] 010: DR Shift and Pause [br] 011: DR
                               Shift and Update [br] 100: Reset [br] Other
                               values are reserved
                            */
    UINT32 jtag_shft : 8;

                            /* Bits[11:4], Access Type=RW, default=0x00000000*/

                            /*
                               Shift Amount.[br] 0-128d (0-80h) are valid
                               values[br] Other values are reserved
                            */
    UINT32 rsvd : 20;

                            /* Bits[31:12], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} JTAG_TUNNEL_CTRL_CWV_DFX_MISC_0_STRUCT;

/* FNV_DEBUG_LOCK_CWV_DFX_MISC_0_REG supported on:                              */
/*      CWV (0x400c6048)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_msc/msc_csr                            */
/* Struct generated from CWV BDF: 0_24_6                                        */
/* CWV Security PolicyGroup:                                                    */
/* This register controls access to DFx functionality. Mainly JTAG tunneling.
*/


#define FNV_DEBUG_LOCK_CWV_DFX_MISC_0_REG 0x1B740048

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 dbg_en : 1;

                            /* Bits[0:0], Access Type=RW/V/P, default=0x00000000*/

                            /*
                               0=Debug Disabled[br] 1=Debug Enabled[br] Note:
                               When lock is set, this field will not change
                               until next assertion of RST_N.
                            */
    UINT32 rsvd : 29;

                            /* Bits[29:1], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 lock : 1;

                            /* Bits[30:30], Access Type=RW/V/P, default=0x00000000*/

                            /*
                               When set, locks this register from further
                               changes until next assertion of RST_N. The
                               default value is loaded from fuses[br]
                               0=unlocked[br] 1=locked[br]
                            */
    UINT32 dwe : 1;

                            /* Bits[31:31], Access Type=RO/V/P, default=0x00000000*/

                            /*
                               HW sets this bit to indicate that Debug was
                               Enabled.
                            */

  } Bits;
  UINT32 Data;

} FNV_DEBUG_LOCK_CWV_DFX_MISC_0_STRUCT;

/* MSC_FEATURE_STAT_0_CWV_DFX_MISC_0_REG supported on:                          */
/*      CWV (0x400c6050)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_msc/msc_csr                            */
/* Struct generated from CWV BDF: 0_24_6                                        */
/* CWV Security PolicyGroup:                                                    */
/* This register contains status bits.
*/


#define MSC_FEATURE_STAT_0_CWV_DFX_MISC_0_REG 0x1B740050

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 spi_done : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               This bit indicates whether SPI download is done
                               or not[br] 1'b1: Download Done[br] 1'b0: No
                               download.
                            */
    UINT32 rsvd : 31;

                            /* Bits[31:1], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MSC_FEATURE_STAT_0_CWV_DFX_MISC_0_STRUCT;

/* SMBUS_CONTROLS_CWV_DFX_MISC_0_REG supported on:                              */
/*      CWV (0x400c6058)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_msc/msc_csr                            */
/* Struct generated from CWV BDF: 0_24_6                                        */
/* CWV Security PolicyGroup:                                                    */
/* Obsolete (not used in HW): [br]
            This register contains controls for SMBUS slave
*/


#define SMBUS_CONTROLS_CWV_DFX_MISC_0_REG 0x1B740058

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 defeature : 1;

                            /* Bits[0:0], Access Type=RW/V/P, default=0x00000000*/

                            /* Obsolete: (do not use) */
    UINT32 rsvd : 1;

                            /* Bits[1:1], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 temp_byteswap : 1;

                            /* Bits[2:2], Access Type=RW/P, default=0x00000000*/

                            /* Obsolete: (do not use) */
    UINT32 tsod_defeature : 1;

                            /* Bits[3:3], Access Type=RW/P, default=0x00000000*/

                            /* Obsolete: (do not use) */
    UINT32 rsvd_4 : 1;

                            /* Bits[4:4], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 reserved_0 : 1;

                            /* Bits[5:5], Access Type=RW/P, default=0x00000000*/

                            /* Obsolete: (do not use) */
    UINT32 rsvd_6 : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} SMBUS_CONTROLS_CWV_DFX_MISC_0_STRUCT;

/* SMBUS_XTRIGGER_CWV_DFX_MISC_0_REG supported on:                              */
/*      CWV (0x400c605c)                                                        */
/* Register default value on CWV: 0x00000004                                    */
/* CWV Register File:    fmc_top_csr/fnv_msc/msc_csr                            */
/* Struct generated from CWV BDF: 0_24_6                                        */
/* CWV Security PolicyGroup:                                                    */
/* This register contains controls for Cross Triggering
*/


#define SMBUS_XTRIGGER_CWV_DFX_MISC_0_REG 0x1B74005C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 en : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000000*/

                            /*
                               One bit enable for cross triggering. Default is
                               0 (Disabled)
                            */
    UINT32 rsvd : 1;

                            /* Bits[1:1], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 active_high : 1;

                            /* Bits[2:2], Access Type=RW/P, default=0x00000001*/

                            /*
                               1'b1 = Active High (default) [br] 1'b0 = Active
                               Low
                            */
    UINT32 rsvd_3 : 29;

                            /* Bits[31:3], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} SMBUS_XTRIGGER_CWV_DFX_MISC_0_STRUCT;

/* DIE_TEMP_CSR_CWV_DFX_MISC_0_REG supported on:                                */
/*      CWV (0x400c6060)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_msc/msc_csr                            */
/* Struct generated from CWV BDF: 0_24_6                                        */
/* CWV Security PolicyGroup:                                                    */
/* Obsolete: (do not use) [br]
*/


#define DIE_TEMP_CSR_CWV_DFX_MISC_0_REG 0x1B740060

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 4;

                            /* Bits[3:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 temperature : 9;

                            /* Bits[12:4], Access Type=RO/V, default=0x00000000*/

                            /* Obsolete: (do not use) [br] */
    UINT32 rsvd_13 : 19;

                            /* Bits[31:13], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DIE_TEMP_CSR_CWV_DFX_MISC_0_STRUCT;

/* MSC_TEMP_OVERRIDE_CSR_CWV_DFX_MISC_0_REG supported on:                       */
/*      CWV (0x400c6064)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_msc/msc_csr                            */
/* Struct generated from CWV BDF: 0_24_6                                        */
/* CWV Security PolicyGroup:                                                    */
/* Obsolete: (do not use) [br]
*/


#define MSC_TEMP_OVERRIDE_CSR_CWV_DFX_MISC_0_REG 0x1B740064

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 temp_enable : 1;

                            /* Bits[0:0], Access Type=RW, default=0x00000000*/

                            /* Obsolete: (do not use) [br] */
    UINT32 temp : 9;

                            /* Bits[9:1], Access Type=RW, default=0x00000000*/

                            /* Obsolete: (do not use) [br] */
    UINT32 rsvd : 22;

                            /* Bits[31:10], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MSC_TEMP_OVERRIDE_CSR_CWV_DFX_MISC_0_STRUCT;

/* MSC_TEMP_CNTRL_CSR_CWV_DFX_MISC_0_REG supported on:                          */
/*      CWV (0x400c6068)                                                        */
/* Register default value on CWV: 0x00000200                                    */
/* CWV Register File:    fmc_top_csr/fnv_msc/msc_csr                            */
/* Struct generated from CWV BDF: 0_24_6                                        */
/* CWV Security PolicyGroup:                                                    */
/* Obsolete: (do not use) [br]
*/


#define MSC_TEMP_CNTRL_CSR_CWV_DFX_MISC_0_REG 0x1B740068

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 4;

                            /* Bits[3:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 timeren : 1;

                            /* Bits[4:4], Access Type=RW, default=0x00000000*/

                            /* Obsolete: (do not use) [br] */
    UINT32 calsel : 1;

                            /* Bits[5:5], Access Type=RW, default=0x00000000*/

                            /* Obsolete: (do not use) [br] */
    UINT32 tempstat_hien : 1;

                            /* Bits[6:6], Access Type=RW, default=0x00000000*/

                            /* Obsolete: (do not use) [br] */
    UINT32 cattrip_test_en : 1;

                            /* Bits[7:7], Access Type=RW, default=0x00000000*/

                            /* Obsolete: (do not use) [br] */
    UINT32 cattrip_test : 1;

                            /* Bits[8:8], Access Type=RW, default=0x00000000*/

                            /* Obsolete: (do not use) [br] */
    UINT32 enable_event : 1;

                            /* Bits[9:9], Access Type=RW, default=0x00000001*/

                            /* Obsolete: (do not use) [br] */
    UINT32 force_event : 1;

                            /* Bits[10:10], Access Type=RW, default=0x00000000*/

                            /* Obsolete: (do not use) [br] */
    UINT32 rsvd_11 : 21;

                            /* Bits[31:11], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MSC_TEMP_CNTRL_CSR_CWV_DFX_MISC_0_STRUCT;

/* MSC_TEMP_LOW_CSR_CWV_DFX_MISC_0_REG supported on:                            */
/*      CWV (0x400c606c)                                                        */
/* Register default value on CWV: 0x000000FF                                    */
/* CWV Register File:    fmc_top_csr/fnv_msc/msc_csr                            */
/* Struct generated from CWV BDF: 0_24_6                                        */
/* CWV Security PolicyGroup:                                                    */
/* Obsolete: (do not use) [br]
*/


#define MSC_TEMP_LOW_CSR_CWV_DFX_MISC_0_REG 0x1B74006C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 templow : 9;

                            /* Bits[8:0], Access Type=RW, default=0x000000FF*/

                            /* Obsolete: (do not use) [br] */
    UINT32 rsvd : 23;

                            /* Bits[31:9], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MSC_TEMP_LOW_CSR_CWV_DFX_MISC_0_STRUCT;

/* MSC_TEMP_HIGH_CSR_CWV_DFX_MISC_0_REG supported on:                           */
/*      CWV (0x400c6070)                                                        */
/* Register default value on CWV: 0x000000FF                                    */
/* CWV Register File:    fmc_top_csr/fnv_msc/msc_csr                            */
/* Struct generated from CWV BDF: 0_24_6                                        */
/* CWV Security PolicyGroup:                                                    */
/* Obsolete: (do not use) [br]
*/


#define MSC_TEMP_HIGH_CSR_CWV_DFX_MISC_0_REG 0x1B740070

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 temphigh : 9;

                            /* Bits[8:0], Access Type=RW, default=0x000000FF*/

                            /* Obsolete: (do not use) [br] */
    UINT32 rsvd : 23;

                            /* Bits[31:9], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MSC_TEMP_HIGH_CSR_CWV_DFX_MISC_0_STRUCT;

/* MSC_TEMP_CRIT_CSR_CWV_DFX_MISC_0_REG supported on:                           */
/*      CWV (0x400c6074)                                                        */
/* Register default value on CWV: 0x000000FF                                    */
/* CWV Register File:    fmc_top_csr/fnv_msc/msc_csr                            */
/* Struct generated from CWV BDF: 0_24_6                                        */
/* CWV Security PolicyGroup:                                                    */
/* Obsolete: (do not use) [br]
*/


#define MSC_TEMP_CRIT_CSR_CWV_DFX_MISC_0_REG 0x1B740074

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 tempcrit : 9;

                            /* Bits[8:0], Access Type=RW, default=0x000000FF*/

                            /* Obsolete: (do not use) [br] */
    UINT32 rsvd : 23;

                            /* Bits[31:9], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MSC_TEMP_CRIT_CSR_CWV_DFX_MISC_0_STRUCT;

/* MSC_TEMP_CAL_CSR_CWV_DFX_MISC_0_REG supported on:                            */
/*      CWV (0x400c6078)                                                        */
/* Register default value on CWV: 0x0000A820                                    */
/* CWV Register File:    fmc_top_csr/fnv_msc/msc_csr                            */
/* Struct generated from CWV BDF: 0_24_6                                        */
/* CWV Security PolicyGroup:                                                    */
/* Obsolete: (do not use) [br]
*/


#define MSC_TEMP_CAL_CSR_CWV_DFX_MISC_0_REG 0x1B740078

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 tempslope : 12;

                            /* Bits[11:0], Access Type=RW, default=0x00000820*/

                            /* Obsolete: (do not use) [br] */
    UINT32 tempoffset : 9;

                            /* Bits[20:12], Access Type=RW, default=0x0000000A*/

                            /* Obsolete: (do not use) [br] */
    UINT32 rsvd : 11;

                            /* Bits[31:21], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MSC_TEMP_CAL_CSR_CWV_DFX_MISC_0_STRUCT;

/* MSC_TEMP_HIPCNTRL_CSR_CWV_DFX_MISC_0_REG supported on:                       */
/*      CWV (0x400c607c)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_msc/msc_csr                            */
/* Struct generated from CWV BDF: 0_24_6                                        */
/* CWV Security PolicyGroup:                                                    */
/* Obsolete: (do not use) [br]
*/


#define MSC_TEMP_HIPCNTRL_CSR_CWV_DFX_MISC_0_REG 0x1B74007C

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 fuselink_offset : 13;

                            /* Bits[12:0], Access Type=RW, default=0x00000000*/

                            /* Obsolete: (do not use) [br] */
    UINT32 viewpin_addr : 6;

                            /* Bits[18:13], Access Type=RW, default=0x00000000*/

                            /* Obsolete: (do not use) [br] */
    UINT32 pushbus_in : 10;

                            /* Bits[28:19], Access Type=RW, default=0x00000000*/

                            /* Obsolete: (do not use) [br] */
    UINT32 prvs_dig : 2;

                            /* Bits[30:29], Access Type=RW, default=0x00000000*/

                            /* Obsolete: (do not use) [br] */
    UINT32 rsvd : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MSC_TEMP_HIPCNTRL_CSR_CWV_DFX_MISC_0_STRUCT;

/* MSC_TEMP_STATUS_CSR_CWV_DFX_MISC_0_REG supported on:                         */
/*      CWV (0x400c6080)                                                        */
/* Register default value on CWV: 0x00000000                                    */
/* CWV Register File:    fmc_top_csr/fnv_msc/msc_csr                            */
/* Struct generated from CWV BDF: 0_24_6                                        */
/* CWV Security PolicyGroup:                                                    */
/* Obsolete: (do not use) [br]
*/


#define MSC_TEMP_STATUS_CSR_CWV_DFX_MISC_0_REG 0x1B740080

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 cattrip_status : 1;

                            /* Bits[0:0], Access Type=RO/V, default=0x00000000*/

                            /* Obsolete: (do not use) [br] */
    UINT32 rsvd : 31;

                            /* Bits[31:1], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MSC_TEMP_STATUS_CSR_CWV_DFX_MISC_0_STRUCT;













/* SGX_B_CWV_DFX_MISC_0_REG supported on:                                       */
/*      CWV (0x400c6190)                                                        */
/* Register default value on CWV: 0xFFFFFFFF                                    */
/* CWV Register File:    fmc_top_csr/fnv_msc/msc_csr                            */
/* Struct generated from CWV BDF: 0_24_6                                        */
/* CWV Security PolicyGroup:                                                    */
/* Future SGX SVN usage[br]
*/


#define SGX_B_CWV_DFX_MISC_0_REG 0x1B740190

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 svn_value : 32;

                            /* Bits[31:0], Access Type=RW/P, default=0xFFFFFFFF*/

                            /* contains the proposed SVN value */

  } Bits;
  UINT32 Data;

} SGX_B_CWV_DFX_MISC_0_STRUCT;









/* MSC_FNVIO_DDRTDLL_CSR_CWV_DFX_MISC_0_REG supported on:                       */
/*      CWV (0x400c61b8)                                                        */
/* Register default value on CWV: 0x00000020                                    */
/* CWV Register File:    fmc_top_csr/fnv_msc/msc_csr                            */
/* Struct generated from CWV BDF: 0_24_6                                        */
/* CWV Security PolicyGroup:                                                    */
/* FNVIO interface Ddrtdll override signals register
*/


#define MSC_FNVIO_DDRTDLL_CSR_CWV_DFX_MISC_0_REG 0x1B7401B8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 en : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000000*/

                            /* Enable to override signals with CSR value */
    UINT32 ddrtdllpowerdown01qnnnh_val : 1;

                            /* Bits[1:1], Access Type=RW/P, default=0x00000000*/

                            /* DDRT DLL power down control */
    UINT32 ddrtdllpowerdown23qnnnh_val : 1;

                            /* Bits[2:2], Access Type=RW/P, default=0x00000000*/

                            /* DDRT DLL power down control */
    UINT32 ddrtdllpowerdown4qnnnh_val : 1;

                            /* Bits[3:3], Access Type=RW/P, default=0x00000000*/

                            /* DDRT DLL power down control */
    UINT32 rsvd_4 : 2;

                            /* Bits[5:4], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rsvd : 26;

                            /* Bits[31:6], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} MSC_FNVIO_DDRTDLL_CSR_CWV_DFX_MISC_0_STRUCT;

























































/* DDRT_PLL_INIT_CTRL_CWV_DFX_MISC_0_REG supported on:                          */
/*      CWV (0x400c6390)                                                        */
/* Register default value on CWV: 0x2E412A06                                    */
/* CWV Register File:    fmc_top_csr/fnv_msc/msc_csr                            */
/* Struct generated from CWV BDF: 0_24_6                                        */
/* CWV Security PolicyGroup:                                                    */
/* DDRT IO Interface Initialization
*/


#define DDRT_PLL_INIT_CTRL_CWV_DFX_MISC_0_REG 0x1B740390

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 dpll_clkstp_shtdwn_dis : 1;

                            /* Bits[0:0], Access Type=RW/P, default=0x00000000*/

                            /* Disable PLL shutdown in clockstop */
    UINT32 trainreset : 1;

                            /* Bits[1:1], Access Type=RW/P, default=0x00000001*/

                            /* IO Train Reset */
    UINT32 ringoscdis_en : 1;

                            /* Bits[2:2], Access Type=RW/P, default=0x00000001*/

                            /* Obsolete: (do not use) */
    UINT32 debounce_oneshot_width_val : 3;

                            /* Bits[5:3], Access Type=RW/P, default=0x00000000*/

                            /* Pll Lock Debounce Oneshot */
    UINT32 spare_rw_p : 2;

                            /* Bits[7:6], Access Type=RW/P, default=0x00000000*/

                            /* Obsolete: (do not use) */
    UINT32 bgf_init_dlshiftwidth : 6;

                            /* Bits[13:8], Access Type=RW/P, default=0x0000002A*/

                            /* bgf download shift width */
    UINT32 vsshilevelqnnnh : 5;

                            /* Bits[18:14], Access Type=RW/P, default=0x00000004*/

                            /* Obsolete: (do not use) */
    UINT32 nsticky_rst_extend : 4;

                            /* Bits[22:19], Access Type=RW/P, default=0x00000008*/

                            /* nsticky reset deassertion delay */
    UINT32 rsvd : 2;

                            /* Bits[24:23], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 dll_powerdown : 1;

                            /* Bits[25:25], Access Type=RW/P, default=0x00000001*/

                            /* Obsolete: (do not use) */
    UINT32 pllbypt_delay : 2;

                            /* Bits[27:26], Access Type=RW/P, default=0x00000003*/

                            /* Obsolete: (do not use) */
    UINT32 rsvd_28 : 1;

                            /* Bits[28:28], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 fast_plldisen : 1;

                            /* Bits[29:29], Access Type=RW/P, default=0x00000001*/

                            /* Obsolete: (do not use) */
    UINT32 rsvd_30 : 1;

                            /* Bits[30:30], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 bgf_reset : 1;

                            /* Bits[31:31], Access Type=RW/P, default=0x00000000*/

                            /*
                               FW initiated reset to the BGF of this domain
                               [br]. Must set and immediately clear
                            */

  } Bits;
  UINT32 Data;

} DDRT_PLL_INIT_CTRL_CWV_DFX_MISC_0_STRUCT;





/* DDRT_PLL_REF_CTRL_CWV_DFX_MISC_0_REG supported on:                           */
/*      CWV (0x400c63a4)                                                        */
/* Register default value on CWV: 0x5440C26C                                    */
/* CWV Register File:    fmc_top_csr/fnv_msc/msc_csr                            */
/* Struct generated from CWV BDF: 0_24_6                                        */
/* CWV Security PolicyGroup:                                                    */
/* DDRT PLL Initialization 
*/


#define DDRT_PLL_REF_CTRL_CWV_DFX_MISC_0_REG 0x1B7403A4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 2;

                            /* Bits[1:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 vrefgenenableqnnnh : 1;

                            /* Bits[2:2], Access Type=RW/P, default=0x00000001*/

                            /* Obsolete: (do not use) */
    UINT32 clkdetectdetlev : 5;

                            /* Bits[7:3], Access Type=RW/P, default=0x0000000D*/

                            /* Obsolete: (do not use) */
    UINT32 clkdetectfiltlev : 2;

                            /* Bits[9:8], Access Type=RW/P, default=0x00000002*/

                            /* Obsolete: (do not use) */
    UINT32 refclkbypass : 1;

                            /* Bits[10:10], Access Type=RW/P, default=0x00000000*/

                            /* Obsolete: (do not use) */
    UINT32 refclkdivratio : 6;

                            /* Bits[16:11], Access Type=RW/P, default=0x00000018*/

                            /*
                               PLL Bus Ratio value [br] 0x18 : PLL freq - 1200
                               Mhz (non-POR for CWV) [br] 0x20 : PLL freq -
                               1600 Mhz (non-POR for CWV) [br] 0x24 : PLL freq
                               - 1800 Mhz [br] 0x28 : PLL freq - 2000 Mhz [br]
                               0x2C : PLL freq - 2200 Mhz [br] 0x30 : PLL freq
                               - 2400 Mhz [br] others: reserved
                            */
    UINT32 rxvrefctlqnnnh : 7;

                            /* Bits[23:17], Access Type=RW/P, default=0x00000020*/

                            /* Obsolete: (do not use) */
    UINT32 vrefhalfupqnnnh : 1;

                            /* Bits[24:24], Access Type=RW/P, default=0x00000000*/

                            /* Obsolete: (do not use) */
    UINT32 clkdetectbiascon : 4;

                            /* Bits[28:25], Access Type=RW/P, default=0x0000000A*/

                            /* Obsolete: (do not use) */
    UINT32 clkdetecthyscon : 2;

                            /* Bits[30:29], Access Type=RW/P, default=0x00000002*/

                            /* Obsolete: (do not use) */
    UINT32 sfrkeepon : 1;

                            /* Bits[31:31], Access Type=RW/P, default=0x00000000*/

                            /* Obsolete: (do not use) */

  } Bits;
  UINT32 Data;

} DDRT_PLL_REF_CTRL_CWV_DFX_MISC_0_STRUCT;

/* IO_INIT_CTRL_CWV_DFX_MISC_0_REG supported on:                                */
/*      CWV (0x400c63a8)                                                        */
/* Register default value on CWV: 0xE0003000                                    */
/* CWV Register File:    fmc_top_csr/fnv_msc/msc_csr                            */
/* Struct generated from CWV BDF: 0_24_6                                        */
/* CWV Security PolicyGroup:                                                    */
/* Various controls to initialize DDRT IO
*/


#define IO_INIT_CTRL_CWV_DFX_MISC_0_REG 0x1B7403A8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd : 12;

                            /* Bits[11:0], Access Type=RO, default=None*/

                            /* Reserved */

    UINT32 daunit_pmreset_mask : 1;

                            /* Bits[12:12], Access Type=RW/P, default=0x00000001*/

                            /* Obsolete: (do not use) */
    UINT32 ddrtio_pmreset_mask : 1;

                            /* Bits[13:13], Access Type=RW/P, default=0x00000001*/

                            /* Obsolete: (do not use) */
    UINT32 rsvd_14 : 14;

                            /* Bits[27:14], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 nmode : 1;

                            /* Bits[28:28], Access Type=RW/P, default=0x00000000*/

                            /*
                               FW will set this bit when it enters into normal
                               ddrt command processing mode.[br] FW can set it
                               after it receives cfg_lock command from
                               BIOS.[br] FW clears bit on cold reset, warm
                               reset, surprise reset. [br] HW uses bit to
                               qualify surprise reset.
                            */
    UINT32 pfail_pwon_mask : 1;

                            /* Bits[29:29], Access Type=RW/P, default=0x00000001*/

                            /*
                               When set: Core does not decode/use xxPWR_FAIL
                               from PMIC .[br] Clear CSR after Power On has
                               properly initialized
                            */
    UINT32 gnt_n_mask : 1;

                            /* Bits[30:30], Access Type=RW/V/P, default=0x00000001*/

                            /* Obsolete: (do not use) */

    UINT32 ddrtio_ckidle_mask : 1;

                            /* Bits[31:31], Access Type=RW/P, default=0x00000001*/

                            /*
                               When this bit is set, clock idle from ddrio is
                               masked. Firmware should only clear this bit when
                               [br] correct DDRT Frequencies are programmed in
                               boot flow. Set this bit when entering DDRT
                               Frequency Change [br] 0 : No Mask [br] 1 : MASK
                               Clock Idle (Default)
                            */

  } Bits;
  UINT32 Data;

} IO_INIT_CTRL_CWV_DFX_MISC_0_STRUCT;

/* DDRT_RESET_STATUS_CWV_DFX_MISC_0_REG supported on:                           */
/*      CWV (0x400c63ac)                                                        */
/* Register default value on CWV: 0x00002000                                    */
/* CWV Register File:    fmc_top_csr/fnv_msc/msc_csr                            */
/* Struct generated from CWV BDF: 0_24_6                                        */
/* CWV Security PolicyGroup:                                                    */
/* DDRT Reset Status Register
*/


#define DDRT_RESET_STATUS_CWV_DFX_MISC_0_REG 0x1B7403AC

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd_0 : 5;

                            /* Bits[4:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rsvd : 3;

                            /* Bits[7:5], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 freqcur : 7;

                            /* Bits[14:8], Access Type=RW/V/P, default=0x00000020*/

                            /* Feedback divider ratio value */
    UINT32 rsvd_15 : 1;

                            /* Bits[15:15], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rsvd_16 : 16;

                            /* Bits[31:16], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DDRT_RESET_STATUS_CWV_DFX_MISC_0_STRUCT;

/* SXP_RESET_STATUS_CWV_DFX_MISC_0_REG supported on:                            */
/*      CWV (0x400c63b0)                                                        */
/* Register default value on CWV: 0x00002000                                    */
/* CWV Register File:    fmc_top_csr/fnv_msc/msc_csr                            */
/* Struct generated from CWV BDF: 0_24_6                                        */
/* CWV Security PolicyGroup:                                                    */
/* SXP Reset Status Register
*/


#define SXP_RESET_STATUS_CWV_DFX_MISC_0_REG 0x1B7403B0

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd_0 : 5;

                            /* Bits[4:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rsvd : 3;

                            /* Bits[7:5], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 freqcur : 7;

                            /* Bits[14:8], Access Type=RW/V/P, default=0x00000020*/

                            /* Feedback divider ratio value */
    UINT32 rsvd_15 : 1;

                            /* Bits[15:15], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rsvd_16 : 15;

                            /* Bits[30:16], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rsvd_31 : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} SXP_RESET_STATUS_CWV_DFX_MISC_0_STRUCT;

/* DRAM_RESET_STATUS_CWV_DFX_MISC_0_REG supported on:                           */
/*      CWV (0x400c63b4)                                                        */
/* Register default value on CWV: 0x00002000                                    */
/* CWV Register File:    fmc_top_csr/fnv_msc/msc_csr                            */
/* Struct generated from CWV BDF: 0_24_6                                        */
/* CWV Security PolicyGroup:                                                    */
/* DRAM Reset Status Register
*/


#define DRAM_RESET_STATUS_CWV_DFX_MISC_0_REG 0x1B7403B4

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd_0 : 5;

                            /* Bits[4:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rsvd : 3;

                            /* Bits[7:5], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 freqcur : 7;

                            /* Bits[14:8], Access Type=RW/V/P, default=0x00000020*/

                            /* Feedback divider ratio value */
    UINT32 rsvd_15 : 1;

                            /* Bits[15:15], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rsvd_16 : 15;

                            /* Bits[30:16], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rsvd_31 : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} DRAM_RESET_STATUS_CWV_DFX_MISC_0_STRUCT;

/* CORE_RESET_STATUS_CWV_DFX_MISC_0_REG supported on:                           */
/*      CWV (0x400c63b8)                                                        */
/* Register default value on CWV: 0x00000C00                                    */
/* CWV Register File:    fmc_top_csr/fnv_msc/msc_csr                            */
/* Struct generated from CWV BDF: 0_24_6                                        */
/* CWV Security PolicyGroup:                                                    */
/* CORE Reset Status Register
*/


#define CORE_RESET_STATUS_CWV_DFX_MISC_0_REG 0x1B7403B8

typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 rsvd_0 : 5;

                            /* Bits[4:0], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rsvd : 3;

                            /* Bits[7:5], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 freqcur : 7;

                            /* Bits[14:8], Access Type=RW/V/P, default=0x0000000C*/

                            /* Feedback divider ratio value */
    UINT32 rsvd_15 : 1;

                            /* Bits[15:15], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rsvd_16 : 15;

                            /* Bits[30:16], Access Type=RO, default=None*/

                            /* Reserved */
    UINT32 rsvd_31 : 1;

                            /* Bits[31:31], Access Type=RO, default=None*/

                            /* Reserved */

  } Bits;
  UINT32 Data;

} CORE_RESET_STATUS_CWV_DFX_MISC_0_STRUCT;





























































































































































































































































































































#endif /* _CWV_DFX_MISC_0_h */
