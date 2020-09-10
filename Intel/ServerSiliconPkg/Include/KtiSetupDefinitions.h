/** @file

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

#ifndef  _kti_setup_definitions_h
#define  _kti_setup_definitions_h

//
// Values for enable/disable options
//
#define KTI_DISABLE 0
#define KTI_ENABLE  1
#define KTI_AUTO    2

#define KTI_SNC2      2
#define KTI_SNC4      4

#define KTI_D2K_LOW      1
#define KTI_D2K_MED      2
#define KTI_D2K_MAX      3

#define KTI_STC_DIS      0
#define KTI_STC_LOW      1
#define KTI_STC_MED      2
#define KTI_STC_MAX      3
#define KTI_STC_AUTO     4

//
// Link speed
//
//
// ICX KTI Speed options
//
#define SPEED_REC_96GT      0
#define SPEED_REC_104GT     1
#define SPEED_REC_112GT     2
//
// SPR KTI Speed options
//
#define SPEED_REC_128GT     0
#define SPEED_REC_144GT     1
#define SPEED_REC_160GT     2
#define MAX_KTI_LINK_SPEED  3
#define FREQ_PER_LINK       4

//
// WHR KTI Speed options
//
#define WHR_HW_MODE_96GT  5
#define WHR_HW_MODE_104GT 4
#define WHR_HW_MODE_112GT 3

//
// Link Speed Modes
//
#define KTI_LINK_SLOW_SPEED_MODE 0
#define KTI_LINK_FULL_SPEED_MODE 1

//
// Degrade Precedence
//
#define TOPOLOGY_PRECEDENCE 0
#define FEATURE_PRECEDENCE  1

//
// Degrade 4S topology preference
//
#define TOPOLOGY_4S_FULLY_CONNECT 0
#define TOPOLOGY_4S_DUALLINK_RING 1

//
// VNA Credit override global setup variable values
//
#define  KTI_LL_VNA_SETUP_PER_LINK 0xff
#define  KTI_LL_VNA_SETUP_MAX      0x7f
#define  KTI_LL_VNA_SETUP_MIN      0x0

//
// Resource allocation settings
//
#define BUS_RATIO_MIN  0
#define BUS_RATIO_MAX  8
#define BUS_RATIO_DFLT 1

//
// IIO Uniphy disable options
//
#define IIO_UNIPHY_DIS_DO_NOT_DISABLE      0
#define IIO_UNIPHY_DIS_DISABLE_AND_CLKGATE 1
#define IIO_UNIPHY_DIS_DISABLE_NO_CLKGATE  2

//
// Legacy VGA allocation defaults
//
#define LEGACY_VGA_SOCKET_DFLT 0
#define LEGACY_VGA_SOCKET_MIN  0
#define LEGACY_VGA_SOCKET_MAX  3
#define LEGACY_VGA_STACK_DFLT  0
#define LEGACY_VGA_STACK_MIN   0
#define LEGACY_VGA_STACK_MAX   6

//
// KTI debug print Levels
//
#define KTI_DEBUG_PRINT_FATAL_ONLY         0x1
#define KTI_DEBUG_PRINT_WARNING_ONLY       0x2
#define KTI_DEBUG_PRINT_INFO_SUMMARY_ONLY  0x4
#define KTI_DEBUG_PRINT_INFO_DETAILED_ONLY 0x8
#define KTI_DEBUG_PRINT_ALL                0xf

//
// PMON discovery configurations
//
#define KTI_PMON_CONFIG_DISABLE  0x0
#define KTI_PMON_CONFIG_REDUCED  0x1
#define KTI_PMON_CONFIG_FULL     0x2
#define KTI_PMON_CONFIG_AUTO     0x3

//
// PMON access control configurations
//
#define KTI_PMON_ACC_CTRL_DISABLE  0x0
#define KTI_PMON_ACC_CTRL_ENABLE   0x1
#define KTI_PMON_ACC_CTRL_AUTO     0x2

//
// CRC modes
//
#define CRC_MODE_16BIT         0
#define CRC_MODE_ROLLING_32BIT 1
#define CRC_MODE_AUTO          2

//
// Hot plug topology settings
//
#define HOT_PLUG_TOPOLOGY_4S 0
#define HOT_PLUG_TOPOLOGY_8S 1

//
// IODC setup options
//
#define IODC_DISABLE                  0
#define IODC_AUTO                          1
#define IODC_EN_REM_INVITOM_PUSH           2
#define IODC_EN_REM_INVITOM_ALLOCFLOW      3
#define IODC_EN_REM_INVITOM_ALLOC_NONALLOC 4
#define IODC_EN_REM_INVITOM_AND_WCILF      5

//
// Thresholds setup options (IrqThreshold & TorThresLoctoremNorm & TorThresLoctoremEmpty)
//
#define THRESHOLDS_DISABLE                  0
#define THRESHOLDS_AUTO                     1
#define THRESHOLDS_LOW                      2
#define THRESHOLDS_MEDIUM                   3
#define THRESHOLDS_HIGH                     4

//
// HA OSB settings
//
#define OSB_AD_VNA_THR_SI_MIN  0
#define OSB_AD_VNA_THR_SI_DFLT 0
#define OSB_AD_VNA_THR_SI_MAX  31
#define OSB_SNP_THR_SI_MIN     0
#define OSB_SNP_THR_SI_DFLT    1
#define OSB_SNP_THR_SI_MAX     7

//
// Bias FWD modes
//
#define BIAS_FWD_MODE0 0
#define BIAS_FWD_MODE1 1
#define BIAS_FWD_MODE2 2
#define BIAS_FWD_MODE3 3
#define BIAS_FWD_MODE4 4
#define BIAS_FWD_AUTO  5

//
// System Degrade Modes
//
#define DEGRADE_TOPOLOGY_1S        0 // Reduce the system to 1S
#define DEGRADE_SUPPORTED_TOPOLOGY 1 // Reduce the system to supported topology
#define DEGRADE_TOPOLOGY_AS_IS     2 // Leave the topology as is

//
// Values for DfxDualLinksInterleavingMode
//
#define DFX_DUAL_LINK_INTLV_MODE_CHA_INTLV   1
#define DFX_DUAL_LINK_INTLV_MODE_DISABLE_D2C 0

//
// Lock Master
//
#define LOCK_MASTER_MIN  0
#define LOCK_MASTER_MAX  MAX_SOCKET - 1
#define LOCK_MASTER_AUTO MAX_SOCKET

#define DFX_IIO_STACK_EN_MIN 0
#define DFX_IIO_STACK_EN_MAX (DFX_IIO_STACK_EN_AUTO-1)
#define DFX_IIO_STACK_EN_AUTO 0xFFFFFFFF


#define MMCFG_BASE_1G    0
#define MMCFG_BASE_1_5G  1
#define MMCFG_BASE_1_75G 2
#define MMCFG_BASE_2G    3
#define MMCFG_BASE_2_25G 4
#define MMCFG_BASE_3G    5
#define MMCFG_BASE_AUTO  6

#define MMCFG_SIZE_64M   0
#define MMCFG_SIZE_128M  1
#define MMCFG_SIZE_256M  2
#define MMCFG_SIZE_512M  3
#define MMCFG_SIZE_1G    4
#define MMCFG_SIZE_2G    5
#define MMCFG_SIZE_AUTO  6

#define CXL_SECURITY_FULLY_TRUSTED      0
#define CXL_SECURITY_PARTIALLY_TRUSTED  1
#define CXL_SECURITY_UNTRUSTED          2
#define CXL_SECURITY_AUTO               3

#define XPT_FIFO_CREDIT_1     1
#define XPT_FIFO_CREDIT_2     2
#define XPT_FIFO_CREDIT_3     3
#define XPT_FIFO_CREDIT_4     4
#define XPT_FIFO_CREDIT_AUTO  5

#endif // _kti_setup_definitions_h
