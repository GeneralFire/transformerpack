/** @file
  Definitions shared with HFR/VFR files.

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

#ifndef _IIOSETUPDEFINITIONS_H_
#define _IIOSETUPDEFINITIONS_H_

//-----------------------------------------------------------------------------------
// Number of ports per socket for CPUs
//------------------------------------------------------------------------------------
#define NUMBER_PORTS_PER_SOCKET_ICX       21
#define NUMBER_PORTS_PER_SOCKET_ICXG      13
#define NUMBER_PORTS_PER_SOCKET_ICXDE     9
#define NUMBER_PORTS_PER_SOCKET_ICXDE_LCC 5
#define NUMBER_PORTS_PER_SOCKET_SNR       5
#define NUMBER_PORTS_PER_SOCKET_SPR       41
#define NUMBER_PORTS_PER_SOCKET_SKX       21
#define NUMBER_PORTS_PER_SOCKET_CPX       13

//-----------------------------------------------------------------------------------
// General NUMBER_PORTS_PER_SOCKET definition
//------------------------------------------------------------------------------------
#ifdef SPR_HOST
#define NUMBER_PORTS_PER_SOCKET       NUMBER_PORTS_PER_SOCKET_SPR
#else
#define NUMBER_PORTS_PER_SOCKET       NUMBER_PORTS_PER_SOCKET_SKX
#endif

/**
==================================================================================================
================= Equates common for Setup options (.vfr/.hfr) and source files (.c/.h) ==========
==================================================================================================
**/

#define IIO_BIFURCATE_xxxxxxxx          0xFE
#define IIO_BIFURCATE_x4x4x4x4          0x0
#define IIO_BIFURCATE_x4x4xxx8          0x1
#define IIO_BIFURCATE_xxx8x4x4          0x2
#define IIO_BIFURCATE_xxx8xxx8          0x3
#define IIO_BIFURCATE_xxxxxx16          0x4
#define IIO_BIFURCATE_x2x2x4x8          0x5
#define IIO_BIFURCATE_x4x2x2x8          0x6
#define IIO_BIFURCATE_x8x2x2x4          0x7
#define IIO_BIFURCATE_x8x4x2x2          0x8
#define IIO_BIFURCATE_x2x2x4x4x4        0x9
#define IIO_BIFURCATE_x4x2x2x4x4        0xA
#define IIO_BIFURCATE_x4x4x2x2x4        0xB
#define IIO_BIFURCATE_x4x4x4x2x2        0xC
#define IIO_BIFURCATE_x2x2x2x2x8        0xD
#define IIO_BIFURCATE_x8x2x2x2x2        0xE
#define IIO_BIFURCATE_x2x2x2x2x4x4      0xF
#define IIO_BIFURCATE_x2x2x4x2x2x4      0x10
#define IIO_BIFURCATE_x2x2x4x4x2x2      0x11
#define IIO_BIFURCATE_x4x2x2x2x2x4      0x12
#define IIO_BIFURCATE_x4x2x2x4x2x2      0x13
#define IIO_BIFURCATE_x4x4x2x2x2x2      0x14
#define IIO_BIFURCATE_x2x2x2x2x2x2x4    0x15
#define IIO_BIFURCATE_x2x2x2x2x4x2x2    0x16
#define IIO_BIFURCATE_x2x2x4x2x2x2x2    0x17
#define IIO_BIFURCATE_x4x2x2x2x2x2x2    0x18
#define IIO_BIFURCATE_x2x2x2x2x2x2x2x2  0x19
#define IIO_BIFURCATE_AUTO              0xFF

#define PCIE_ASPM_DISABLE                  0
#define PCIE_ASPM_L0S_ONLY                 1
#define PCIE_ASPM_L1_ONLY                  2
#define PCIE_ASPM_L0S_L1_BOTH              3
#define PCIE_ASPM_AUTO                     4

#define PCIE_GLOBAL_ASPM_DISABLE           0
#define PCIE_GLOBAL_ASPM_PER_PORT          1
#define PCIE_GLOBAL_ASPM_L1_ONLY           2

#define PCIE_LTR_ENABLE_NO                 0
#define PCIE_LTR_ENABLE_YES                1
#define PCIE_LTR_ENABLE_AUTO               2

#define PCIE_MAX_PAYLOAD_AUTO              7
#define PCIE_MAX_PAYLOAD_128B              0
#define PCIE_MAX_PAYLOAD_256B              1
#define PCIE_MAX_PAYLOAD_512B              2

#define PCIE_LIB_MRRS_VALUE_AUTO           7
#define PCIE_LIB_MRRS_128B                 0
#define PCIE_LIB_MRRS_256B                 1
#define PCIE_LIB_MRRS_512B                 2
#define PCIE_LIB_MRRS_1024B                3
#define PCIE_LIB_MRRS_2048B                4
#define PCIE_LIB_MRRS_4096B                5

#define PCIE_LINK_SPEED_AUTO               0
#define PCIE_LINK_SPEED_GEN1               1
#define PCIE_LINK_SPEED_GEN2               2
#define PCIE_LINK_SPEED_GEN3               3
#define PCIE_LINK_SPEED_GEN4               4
#define PCIE_LINK_SPEED_GEN5               5

#define PCIE_L1_8US_16US                   4

#define IIO_OPTION_AUTO    2
#define IIO_OPTION_ENABLE  1
#define IIO_OPTION_DISABLE 0

#define TXEQ_PH2_PRECURSOR_MIN        0
#define TXEQ_PH2_CURSOR_MIN           0
#define TXEQ_PH2_POSTCURSOR_MIN       0

#define TXEQ_PH2_PRECURSOR_MAX        63
#define TXEQ_PH2_CURSOR_MAX           63
#define TXEQ_PH2_POSTCURSOR_MAX       63

#if defined(EAGLESTREAM_FLAG) && defined(CLX_HOST)
//
// EBG+SKX PO settings, to be removed after EBG+SPR PO.
//
#define TXEQ_PH2_G3_PRECURSOR_DEFAULT  7
#define TXEQ_PH2_G3_CURSOR_DEFAULT     49
#define TXEQ_PH2_G3_POSTCURSOR_DEFAULT 7
#else
#define TXEQ_PH2_G3_PRECURSOR_DEFAULT  11
#define TXEQ_PH2_G3_CURSOR_DEFAULT     41
#define TXEQ_PH2_G3_POSTCURSOR_DEFAULT 11
#endif
#define TXEQ_PH2_G4_PRECURSOR_DEFAULT   0
#define TXEQ_PH2_G4_CURSOR_DEFAULT     24
#define TXEQ_PH2_G4_POSTCURSOR_DEFAULT  0

#define TXEQ_PH2_G5_PRECURSOR_DEFAULT   0
#define TXEQ_PH2_G5_CURSOR_DEFAULT     52
#define TXEQ_PH2_G5_POSTCURSOR_DEFAULT  0

#define TXEQ_PH3_PRECURSOR_MIN        0
#define TXEQ_PH3_CURSOR_MIN           0
#define TXEQ_PH3_POSTCURSOR_MIN       0

#define TXEQ_PH3_PRECURSOR_MAX        63
#define TXEQ_PH3_CURSOR_MAX           63
#define TXEQ_PH3_POSTCURSOR_MAX       63

#if defined(EAGLESTREAM_FLAG) && defined(CLX_HOST)
//
// EBG+SKX PO settings, to be removed after EBG+SPR PO.
//
#define TXEQ_PH3_PRECURSOR_DEFAULT    0
#define TXEQ_PH3_CURSOR_DEFAULT       55
#define TXEQ_PH3_POSTCURSOR_DEFAULT   8
#else
#define TXEQ_PH3_PRECURSOR_DEFAULT    11
#define TXEQ_PH3_CURSOR_DEFAULT       41
#define TXEQ_PH3_POSTCURSOR_DEFAULT   11
#endif

#define DFX_TX_PRESET_AUTO                0xFF
#define DFX_RX_PRESET_AUTO                0xFF

/**
  DFX Gen3/Gen4 Tx Coefficient override mode definitions for IIO_CONFIG :: DfxGen3OverrideMode and
  IIO_CONFIG :: DfxGen4OverrideMode.
**/
#define DFX_TXEQ_OVERRIDE_MODE_PHY            0 ///< 0: UniPHY
#define DFX_TXEQ_OVERRIDE_MODE_MANUAL         1 ///< 1: Manual mode for both Phase 2 and Phase 3
#define DFX_TXEQ_OVERRIDE_MODE_MANUAL_PH2     2 ///< 2: Manual mode for Phase 2 only
#define DFX_TXEQ_OVERRIDE_MODE_MANUAL_PH3     3 ///< 3: Manual mode for Phase 3 only
#define DFX_TXEQ_OVERRIDE_MODE_ALTERNATE_TXEQ 4 ///< 4: LAGUNA patch mode
#define DFX_TXEQ_OVERRIDE_MODE_TEST_CARD      5 ///< 5: Test Card mode
#define DFX_TXEQ_OVERRIDE_MODE_MGPHYSHORTCH   6 ///< 6: MGPhy with short patch

#define DFX_TXEQ_TEST_CARD_LAGUNA           0
#define DFX_TXEQ_TEST_CARD_NTB              1

#define DFX_TXEQ_EQ_MODE_TESTCARD          1
#define DFX_TXEQ_EQ_MODE_NTB_TESTCARD      2


#define COMPLETION_TIMEOUT_260MS_900MS     9

#define SNOOP_RESP_DEF_VALUE               6

#define MC_INDEX_POS_12                    0xC

#define MC_NUM_GROUP_8                     8

#define CONFIG_IOU_AUTO                    0xFF

#define NTB_BARSIZE_IMBAR1_DEFAULT         0xC
#define NTB_BARSIZE_IMBAR2_DEFAULT         0xC
#define NTB_BARSIZE_IMBAR2_0_DEFAULT       0xC
#define NTB_BARSIZE_IMBAR2_1_DEFAULT       0xC
#define NTB_BARSIZE_EMBAR1_DEFAULT         0xC
#define NTB_BARSIZE_EMBAR2_DEFAULT         0xC
#define NTB_BARSIZE_EMBAR2_0_DEFAULT       0xC
#define NTB_BARSIZE_EMBAR2_0_DEFAULT       0xC
#define NTB_IIO_XLINK_CTL_DSD_USP          2

#define VMD_CFG_BAR_SIZE_DEFAULT           25
#ifdef SPR_HOST
  #define VMD_MEM_BAR_SIZE1_DEFAULT        27
  #define VMD_MEM_BAR_SIZE2_DEFAULT        21
#else
  #define VMD_MEM_BAR_SIZE1_DEFAULT        25
  #define VMD_MEM_BAR_SIZE2_DEFAULT        20
#endif

#define VMD_32BIT_NONPREFETCH              0
#define VMD_64BIT_NONPREFETCH              1
#define VMD_64BIT_PREFETCH                 2

#define IODC_DISABLE                       0
#define IODC_AUTO                          1
#define IODC_EN_REM_INVITOM_PUSH           2
#define IODC_EN_REM_INVITOM_ALLOCFLOW      3
#define IODC_EN_REM_INVITOM_ALLOC_NONALLOC 4
#define IODC_EN_REM_INVITOM_AND_WCILF      5
#define IODC_GLOBAL_KTI_OPTION             6

#define PCIE_PORT_DISABLE                   0
#define PCIE_PORT_ENABLE                    1
#define PCIE_PORT_AUTO                      2

#define NTB_PORT_DEF_TRANSPARENT            0
#define NTB_PORT_DEF_NTB_NTB                1
#define NTB_PORT_DEF_NTB_RP                 2
#define NTB_PORT_DEF_NTB_NOT_SET            0xFF

#define NTB_CROSSLINK_NOT_SET               0xFF

#define PERFORMANCE_SAFE_MODE               0
#define PERFORMANCE_ENABLE_MODE             1

#define IIO_MODE_GEN4_ONLY                  0
#define IIO_MODE_GEN5                       1
#define IIO_MODE_CXL                        2
#define IIO_MODE_FORCE_CXL                  3

#define PCIE_ACPI_HOTPLUG_DISABLE           0
#define PCIE_ACPI_HOTPLUG_ENABLE            1
#define PCIE_ACPI_HOTPLUG_PER_PORT          2

#endif /* _IIOSETUPDEFINITIONS_H_ */
