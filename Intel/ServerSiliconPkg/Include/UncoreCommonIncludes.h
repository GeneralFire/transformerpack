/** @file
  This file defines common equates.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2011 - 2020 Intel Corporation. <BR>

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

#ifndef _UNCORE_COMMON_INCLUDES_H_
#define _UNCORE_COMMON_INCLUDES_H_

#include <MaxSocket.h>
#include <MaxCore.h>
#include <MaxThread.h>
// APTIOV_SERVER_OVERRIDE_RC_START : SDL tokens in ACPI.sdl are used to update table ID's.
#if BUILD_FSP_BINARY == 0
#include <AcpiOemElinks.h>
#endif
// APTIOV_SERVER_OVERRIDE_RC_END : SDL tokens in ACPI.sdl are used to update table ID's.

#ifndef V_INTEL_VID
#define V_INTEL_VID               0x8086
#endif

#define MAX_TILE_BITMASK              0x1FFFFFFFFFFF    // Max Tile mask for KNH
#define MAX_CORE_BITMASK               0xFFFFFFFFFFF    // for ICX CPU

#define MAX_PROCESSOR_CORE_RATIO               100
#define DEFAULT_PROCESSOR_CORE_RATIO           23
#define MAX_SCRAMBLE_SEED_LOW                  65535
#define MAX_SCRAMBLE_SEED_HIGH                 65535
#define ITURBO_MODE_TRADITIONAL                0
#define ITURBO_MODE_POWER_OPTIMIZED            4
#define MAX_PROCESSOR_TSEG                     5

// Define the different SKX package Sku's
#define SKX_PHY_CHOP_HCC 0x2
#define SKX_PHY_CHOP_MCC 0x1
#define SKX_PHY_CHOP_LCC 0x0

//
// SKX CPU steppings
//
#ifndef SKX_A0_CPU_STEP
#define SKX_A0_CPU_STEP  0x00
#endif
#ifndef SKX_A1_CPU_STEP
#define SKX_A1_CPU_STEP  SKX_A0_CPU_STEP
#endif
#ifndef SKX_A2_CPU_STEP
#define SKX_A2_CPU_STEP  0x01
#endif
#ifndef SKX_B0_CPU_STEP
#define SKX_B0_CPU_STEP  0x02
#endif
#ifndef SKX_L0_CPU_STEP
#define SKX_L0_CPU_STEP  0x02
#endif
#ifndef SKX_B1_CPU_STEP
#define SKX_B1_CPU_STEP  0x03
#endif
#ifndef SKX_H0_CPU_STEP
#define SKX_H0_CPU_STEP  0x04
#endif
#ifndef SKX_M0_CPU_STEP
#define SKX_M0_CPU_STEP  0x04
#endif
#ifndef SKX_U0_CPU_STEP
#define SKX_U0_CPU_STEP  0x04
#endif

// CLX CPU steppings/revisions
#ifndef CLX_A0_CPU_STEP
#define CLX_A0_CPU_STEP  0x05
#endif //#ifdef CLX_A0_CPU_STEP

#ifndef CLX_B0_CPU_STEP
#define CLX_B0_CPU_STEP  0x06
#endif //#ifdef CLX_B0_CPU_STEP

#ifndef CLX_N0_CPU_STEP
#define CLX_N0_CPU_STEP  0x06
#endif //#ifdef CLX_N0_CPU_STEP

#ifndef CLX_V0_CPU_STEP
#define CLX_V0_CPU_STEP  0x06
#endif //#ifdef CLX_V0_CPU_STEP

#ifndef CLX_B1_CPU_STEP
#define CLX_B1_CPU_STEP  0x07
#endif //#ifdef CLX_B1_CPU_STEP

#ifndef CLX_N1_CPU_STEP
#define CLX_N1_CPU_STEP  0x07
#endif //#ifdef CLX_N1_CPU_STEP

#ifndef CLX_V1_CPU_STEP
#define CLX_V1_CPU_STEP  0x07
#endif //#ifdef CLX_V1_CPU_STEP

// CPX CPU steppings/revisions
#ifndef CPX_A0_CPU_STEP
#define CPX_A0_CPU_STEP  0x0A
#endif //#ifdef CPX_A0_CPU_STEP

#ifndef CPX_A1_CPU_STEP
#define CPX_A1_CPU_STEP  0x0B
#endif //#ifdef CPX_A1_CPU_STEP

//
// ICX SP CPU steppings
//
#ifndef ICXSP_R0_CPU_STEP
#define ICXSP_R0_CPU_STEP  0x00
#endif
#ifndef ICXSP_L0_CPU_STEP
#define ICXSP_L0_CPU_STEP  0x04
#endif
#ifndef ICXSP_C0_CPU_STEP
#define ICXSP_C0_CPU_STEP  0x05
#endif
#ifndef ICXSP_D0_CPU_STEP
#define ICXSP_D0_CPU_STEP  0x06
#endif

//
// ICX-D CPU steppings
//
#ifndef ICXD_A0_CPU_STEP
#define ICXD_A0_CPU_STEP  0x00
#endif
#ifndef ICXD_B0_CPU_STEP
#define ICXD_B0_CPU_STEP  0x01
#endif

//
// SPR SP CPU steppings
//
#ifndef SPRSP_A0_CPU_STEP
#define SPRSP_A0_CPU_STEP  0x00
#endif
#ifndef SPRSP_B0_CPU_STEP
#define SPRSP_B0_CPU_STEP  0x01
#endif
#ifndef SPRSP_C0_CPU_STEP
#define SPRSP_C0_CPU_STEP  0x02
#endif
#ifndef SPRSP_D0_CPU_STEP
#define SPRSP_D0_CPU_STEP  0x03
#endif

// SNR CPU steppings/revisions
#ifndef SNR_A0_CPU_STEP
#define SNR_A0_CPU_STEP  0x00
#endif
#ifndef SNR_A1_CPU_STEP
#define SNR_A1_CPU_STEP  0x01
#endif
#ifndef SNR_A2_CPU_STEP
#define SNR_A2_CPU_STEP  0x02
#endif

#ifndef SNR_A3_CPU_STEP
#define SNR_A3_CPU_STEP  0x03
#endif

#ifndef SNR_B0_CPU_STEP
#define SNR_B0_CPU_STEP  0x04
#endif

#ifndef SNR_B1_CPU_STEP
#define SNR_B1_CPU_STEP  0x05
#endif

#ifndef SNR_C0_CPU_STEP
#define SNR_C0_CPU_STEP  0x07
#endif

// TNR CPU steppings/revisions
#ifndef TNR_A0_CPU_STEP
#define TNR_A0_CPU_STEP  0x0A
#endif

#define MAX_DIE                         1
#define MAX_CPU_NUM                     (MAX_THREAD * MAX_CORE * MAX_DIE * MAX_SOCKET)

#ifndef MAX_HA
#define MAX_HA                          2
#endif

// If you change this, please also update MAX_IMC in Library\ProcMemInit\Include\MemHostChip.h
// If you change this, please also update MAX_IMC in MemCommon.h
#ifndef MAX_IMC
#define MAX_IMC                       2                     // Maximum memory controllers per socket
#endif

#ifndef MAX_IMC_HBM
#define MAX_IMC_HBM                   4                     // Maximum HBM memory controllers per socket
#endif

// If you change this, please also update MAX_MC_CH in Library\ProcMemInit\Include\MemHostChip.h
// If you change this, please also update MAX_IMC in MemCommon.h
#ifndef MAX_MC_CH
#define MAX_MC_CH                     3                     // Max number of channels per MC (3 for EP)
#endif

#define MAX_MC_SCH                    4                     // Max number of Sub channels per MC (4 for SPR-SP)

// If you change this, please also update MAX_CH in Library\ProcMemInit\Include\MemHostChip.h
// If you change this, please also update MAX_IMC in MemCommon.h
#ifndef MAX_CH
#define MAX_CH                        ((MAX_IMC)*(MAX_MC_CH))     // Max channels per socket (worst case EP * EX combination = 16)
#endif

// If you change this, please also update MAX_DIMM in Library\ProcMemInit\Include\MemHostChip.h
#ifndef MAX_DIMM
#define MAX_DIMM                      2                       // Max DIMM per channel
#endif

// If you change this, please also update MC_MAX_NODE in Library\ProcMemInit\Include\MemHostChip.h
#ifndef MC_MAX_NODE
#define MC_MAX_NODE                   (MAX_SOCKET * MAX_IMC)  // Max number of memory nodes
#endif

// If you change this, please also update MAX_SYS_CH in Library\ProcMemInit\Include\MemHostChip.h
// If you change this, please also update MAX_IMC in MemCommon.h
#ifndef MAX_SYS_CH
#define MAX_SYS_CH                    (MAX_CH * MAX_SOCKET)   // Max channels in the system
#endif
#define MAX_SYS_DIMM                  MAX_SYS_CH * MAX_DIMM

#define MAX_CRS_ENTRIES_PER_NODE      8                       // Max number of ranges allowed on a memory node

#ifndef CB3_DEVICES_PER_SOCKET
#define CB3_DEVICES_PER_SOCKET       8
#endif

#ifndef TOTAL_CB3_DEVICES
#define TOTAL_CB3_DEVICES             64   // IOAT_TOTAL_FUNCS * MAX_SOCKET. Note: this covers up to 8S.
#endif

#ifndef MaxIIO
#define MaxIIO                        MAX_SOCKET
#endif

#ifndef TOTAL_IIO_STACKS
#define TOTAL_IIO_STACKS              48   // MAX_SOCKET * MAX_IIO_STACK. Not reflect architecture but only sysHost structure!
#endif

#ifndef NUMBER_NTB_PORTS_PER_SOCKET
#define NUMBER_NTB_PORTS_PER_SOCKET     5
#endif  // #ifndef NUMBER_NTB_PORTS_PER_SOCKET

#ifndef NUMBER_NTB_PORTS_PER_SOCKET_ICXDE
#define NUMBER_NTB_PORTS_PER_SOCKET_ICXDE 2
#endif // #ifndef NUMBER_NTB_PORTS_PER_SOCKET_ICXDE

#ifndef NUMBER_NTB_PORTS_PER_SOCKET_SNR
#define NUMBER_NTB_PORTS_PER_SOCKET_SNR   0  // Doesn't support NTB
#endif // #ifndef NUMBER_NTB_PORTS_PER_SOCKET_SNR

#ifndef NUMBER_NTB_PORTS_PER_SOCKET_SKX
#define NUMBER_NTB_PORTS_PER_SOCKET_SKX   3
#endif //#ifndef NUMBER_NTB_PORTS_PER_SOCKET_SKX

#ifndef MAX_VMD_ROOTPORTS_PER_PCH
  #define MAX_VMD_ROOTPORTS_PER_PCH   20    // Max number of rootports in PCH
#endif // MAX_VMD_ROOTPORTS_PER_PCH

#ifndef MAX_DEVHIDE_REGS_PER_SYSTEM
  #if MaxIIO > 4
    #define MAX_DEVHIDE_REGS_PER_SYSTEM                  512 // MAX_DEVHIDE_REGS_PER_SOCKET * MaxIIO
  #else
    #define MAX_DEVHIDE_REGS_PER_SYSTEM                  256 // MAX_DEVHIDE_REGS_PER_SOCKET * MaxIIO
  #endif
#endif

#define MAX_IOU_PORT_DEVICES          4

//
// Resource Ratio units used by Uncore Init PEIM.
//
// Assumption: these values must be 2^N; Otherwise the algorithm in OemProcMemInit.c
// needs to be adjusted: the "if (Alignment > KTI_SOCKET_BUS_RATIO_UNIT) {" should be
// removed when computing adjusted "Length".
//
#define KTI_SOCKET_BUS_RATIO_UNIT              0x20
#define KTI_SOCKET_IO_RATIO_UNIT               0x2000
#define KTI_SOCKET_MMIOL_RATIO_UNIT            0x4000000
//
// Maximum alignment bit allowed for Socket PCI Resources.
//
#define KTI_SOCKET_MAX_BUS_ALIGNMENT           0x8
#define KTI_SOCKET_MAX_IO_ALIGNMENT            0x10
#define KTI_SOCKET_MAX_MMIOL_ALIGNMENT         0x20

#ifndef MAX_B2P_MAILBOX_GROUPS
#define MAX_B2P_MAILBOX_GROUPS    32
#endif // !MAX_B2P_MAILBOX_GROUPS

//
// ACPI table information used to initialize tables.
//
// APTIOV_SERVER_OVERRIDE_RC_START : SDL tokens in ACPI.sdl are used to update table ID's.
#define EFI_ACPI_OEM_ID           ACPI_OEM_ID_MAK //"INTEL "
#define EFI_ACPI_OEM_TABLE_ID     ACPI_OEM_TBL_ID_MAK //0x59454C525550  // "PURLEY"
// APTIOV_SERVER_OVERRIDE_RC_END : SDL tokens in ACPI.sdl are used to update table ID's.
#define EFI_ACPI_OEM_REVISION     0x00000002
#define EFI_ACPI_CREATOR_ID       0x4C544E49          // "INTL"
#define EFI_ACPI_CREATOR_REVISION 0x20091013          // Oct 13 2009

#endif
