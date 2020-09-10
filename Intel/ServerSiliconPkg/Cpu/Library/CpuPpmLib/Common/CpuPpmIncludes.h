/** @file
  Cpu Power Management Includes that expose local Cpu PM Lib.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2005 - 2020 Intel Corporation. <BR>

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

#ifndef  _CPU_PPM_INCLUDES_H_
#define  _CPU_PPM_INCLUDES_H_

#include <Library/DebugLib.h>
#include <Library/CpuPpmLib.h>
#include <Library/BaseLib.h>
#include <Protocol/MpService.h>
#include <Library/CpuConfigLib.h>
#include <Library/S3BootScriptLib.h>
#include <Library/SynchronizationLib.h>
#include <Library/PcuMailBoxLib.h>
#include <Library/MemRcLib.h>
#include <Library/CpuIpLib.h>
#include <Library/UsraCsrLib.h>
#include <Library/CpuAndRevisionLib.h>
#include <Register/ArchitecturalMsr.h>
#include <Cpu/CpuCoreRegs.h>
#include <CpuDataStruct.h>
#include <Register/Cpuid.h>

#define MAILBOX_WAIT_TIMEOUT  1000  ///< 1 millisecond
#define MAILBOX_WAIT_STALL    1     ///< 1 microsecond

#define PPM_MAJOR           0
#define PPM_MINOR           1
#define PPM_REV             0
#define PPM_BUILD           0

// Note: There 3 separate DEF blocks: CSR related, MSR related and MISC
//       Please add your DEFs in proper block

//---------------------------------------------
//             defs related to CSR
//---------------------------------------------

// BIOS_RESET_CPL bit description
#define BIOS_RSTCPL0_DONE                  BIT0
#define BIOS_RSTCPL1_MCALDONE              BIT1
#define BIOS_RSTCPL2_RCOMPDONE             BIT2
#define BIOS_RSTCPL3_PMCFGDONE             BIT3
#define BIOS_RSTCPL4_MEMCFGDONE            BIT4

//---------------------------------------------
//                 MISC defs
//---------------------------------------------
#define END_OF_TABLE                                 0xFFFF

//PKGC_SA_PS_CRITERIA
#define KTI_ALLOWED_PS_MASK                 0xF     //Bits[7:0]
#define KTI_CRITERIA_PS_MASK                0x8     //Bits[15:8]

#define RLINK_ALLOWED_PS_MASK               0xF     //Bits[7:0]
#define RLINK_CRITERIA_PS_MASK              0x8     //Bits[15:8]

#define MCDDR_ICXSP_R0_WA_ALLOWED_PS_MASK   0xA     //Bits[7:0]
#define MCDDR_ICXSP_R0_WA_CRITERIA_PS_MASK  0xA     //Bits[15:8]
#define OPTION_1_ALLOWED_PS_MASK            0x3     //Bits[7:0]
#define OPTION_1_CRITERIA_PS_MASK           0x2     //Bits[15:8]
#define OPTION_2_ALLOWED_PS_MASK            0xF     //Bits[7:0]
#define OPTION_2_CRITERIA_PS_MASK           0x8     //Bits[15:8]
#define OPTION_3_ALLOWED_PS_MASK            0x1F    //Bits[7:0]
#define OPTION_3_CRITERIA_PS_MASK           0x10    //Bits[15:8]

#define IIO_ALLOWED_PS_MASK                 0xF     //Bits[7:0]
#define IIO_CRITERIA_PS_MASK                0x8     //Bits[15:8]

#define MDFS_ALLOWED_PS_MASK                0x3     //Bits[7:0]
#define MDFS_CRITERIA_PS_MASK               0x2     //Bits[15:8]

#define KTI_ICXSP_ALLOWED_PS_MASK           0x9     //Bits[7:0]
#define KTI_ICXSP_CRITERIA_PS_MASK          0x3     //Bits[15:8]
#define RLINK_ICXSP_ALLOWED_PS_MASK         0x9     //Bits[7:0]
#define RLINK_ICXSP_CRITERIA_PS_MASK        0x3     //Bits[15:8]
#define IIO_ICXSP_ALLOWED_PS_MASK           0x9     //Bits[7:0]
#define IIO_ICXSP_CRITERIA_PS_MASK          0x3     //Bits[15:8]
#define MDFS_ICXSP_ALLOWED_PS_MASK          0x3     //Bits[7:0]
#define MDFS_ICXSP_CRITERIA_PS_MASK         0x1     //Bits[15:8]

#define PCU_OUT_OF_BAND_ALTERNATE_EPB                (1 << 19)

//
// Integer division with rounding to the nearest rather than truncating.
// For example 8/3=2 but EFI_IDIV_ROUND(8,3)=3. 1/3=0 and EFI_IDIV_ROUND(1,3)=0.
// A half is rounded up e.g., EFI_IDIV_ROUND(1,2)=1 but 1/2=0.
//
#define EFI_IDIV_ROUND(r, s)  (DivU64x32((r),(s)) + ((((ModU64x32((r),(s))) << 1) < (s)) ? 0 : 1))

// SKX A0 WA only: set P1 for core and mesh as 700MHz
#define UNCORE_RATIO_HARDCODE_PN             0x3f
#define MESH_RATIO_HARDCODE_PN               7
#define UNCORE_RATIO_HARDCODE_P1             0x3f
#define MESH_RATIO_HARDCODE_P1               40

//
// 10nm PMAX Detector Platform Loadline (mohms)
//
#define PMAX_LOAD_LINE_0P50     1  // 0.50mohm platform LL
#define PMAX_LOAD_LINE_0P85     2  // 0.85mohm platform LL
#define PMAX_LOAD_LINE_0P85_VALUE     850  // 0.85mohm platform LL
#define PMAX_LOAD_LINE_0P50_VALUE     500  // 0.50mohm platform LL

// PCU_CR_PMAX_CONFIG bit definition
#define MCP_POWER_LIMIT_DEFAULT           0x3B0           // Watts in 12.3 format
#define PMAX_OFFSET_MAX_LIMIT_SKX         0xF
#define PMAX_OFFSET_MAX_LIMIT_10nm        0x3F
#define PMAX_OFFSET_MAX_LIMIT_10nm_SPR    0x1F



#ifdef IA32
#define WriteRegisterTable(a,b,c,d,e,f)       do {} while(0);
#endif

#define SVID_CMD_GET_REG                0x07

//
// Package TDP vs PMAX Offset structure
//
typedef struct {
  UINT16      PackageTDP;
  UINT16      PmaxOffsetValue;
} PACKAGE_TDP_PMAX_OFFSET;

// SKX H0/M0/U0, CPX A0 WA: Adjust the delayed C-state timer via BIOS-to-PCODE mailbox.
#define DCST_LUT0               0x36
#define DCST_LUT1               0x18
#define DCST_LUT2               0x18
#define DCST_LUT3               0x0C
#define SET_DCST_LUTS_DATA      ((DCST_LUT3 << 24) | (DCST_LUT2 << 16) | (DCST_LUT1 << 8) | DCST_LUT0)

VOID
SetPpmBiosResetCpl (
  UINT8             SocketNumber,
  EFI_CPU_PM_STRUCT *ppm
);

VOID
ReportB2PPkgcSaPsCriteriaResult (
  UINT8   SocketNumber,
  UINT32  PcodeResult
);

VOID
PpmS3BootScriptSaveB2PWrite (
  IN  UINT8   SocketNumber,
  IN  UINT32  B2pCommand,
  IN  UINT32  B2pData
);

UINT32
PpmGetSocketFromProcessNumber (
  UINTN ProcessorNumber
);

/**
  Lock Psys power limit MSR registers.

  @param PsysPowerLimit   - Pointer to PSYS_POWRER_LIMIT instance
  @param ProcessorNumber  - Processor index that is executing

  @retval None

**/
VOID
LockPlatformPowerLimitMsr (
  PSYS_POWRER_LIMIT     *PsysPowerLimit,
  UINTN                 ProcessorNumber
  );

/**
  Lock Psys power limit CSR register.

  @param SocketNumber  -    Socket index that is executing
  @param ppm              - Pointer to policy protocol instance
  @param PsysPowerLimit   - Pointer to PSYS_POWRER_LIMIT instance

  @retval None

**/
VOID
LockPlatformPowerLimitCsr (
  UINT8                 SocketNumber,
  EFI_CPU_PM_STRUCT     *ppm,
  PSYS_POWRER_LIMIT     *PsysPowerLimit
  );

#endif
