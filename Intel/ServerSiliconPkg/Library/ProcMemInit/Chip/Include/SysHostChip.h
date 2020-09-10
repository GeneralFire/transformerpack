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

#ifndef _SYS_HOST_CHIP_H_
#define _SYS_HOST_CHIP_H_

#include <SysHost.h>
#include <MemHost.h>
#include <RcRegs.h>
#include <Upi/KtiHost.h>
#include <ScratchPadReg.h>

#define CCMRC_REVISION_CHANGELIST  320717

//
//  ICX_TODO: need to correct it after they are fixed. Defined them as SKX's now for code preparation
//
#define ICX_LCC_CHOP 0x00
#define ICX_MCC_CHOP 0x01
#define ICX_HCC_CHOP 0x02
#define ICX_XCC_CHOP 0x03


//
// SKX Steppings
//
#define SKX_LCC_CHOP 0x00  //(U0)
#define SKX_HCC_CHOP 0x02  //(L0),(M0)
#define SKX_XCC_CHOP 0x03  //(A0/A1/A2/B0/B1/H0)
//
//CLX Chop types
//
#define CLX_LCC_CHOP 0x00  // (V0)
#define CLX_HCC_CHOP 0x02  // (N0)
#define CLX_XCC_CHOP 0x03  // (A0/B0)

//
// Move to a separate header file
// Scratch register usage
//
#define CSR_EMULATION_FLAG_1_11_0_CFG_REG 0x401580b0  // register at B:D:F: 1:11:0, offset 0xB0. with SoftSimulation flag
// -----------------------------------------------------------------------------
// Sticky Scratch Pad regsiter usage
//  B:D:F:  n:11:3: offset 0x40, 0x44, ...,  total 8 registers
//
#define SR_WARMBOOT_MMCFG_DATA_CSR  BIOSSCRATCHPAD7_UBOX_MISC_REG

//
// -----------------------------------------------------------------------------
// Non-Sticky Scratch Pad regsiter usage
//  B:D:F:  n:11:3: offset 0x60, 0x64, ...,  total 16 registers


//
// CSR for maintaining a pointer to the Compatible BIOS Data structure.
//

#define SR_BDAT_STRUCT_PTR_CSR                    (BIOSNONSTICKYSCRATCHPAD5_UBOX_MISC_REG)   // UBOX scratchpad CSR5
#define SR_BDAT_STRUCT_PTR_CSR_MMIO               0x4015b074  // BIOSNONSTICKYSCRATCHPAD5_1_11_3_CFG_REG

//
// CSRs for PIPE/SOCKET/MRC MILESTONES
//
#define SR_PBSP_CHECKIN_CSR                       (BIOSNONSTICKYSCRATCHPAD2_UBOX_MISC_REG)   // UBOX scratchpad CSR02
#define SR_BIOS_SERIAL_DEBUG_CSR                  (BIOSSCRATCHPAD6_UBOX_MISC_REG)            // UBOX scratchpad CSR6
#define SR_POST_CODE_CSR                          (BIOSNONSTICKYSCRATCHPAD7_UBOX_MISC_REG)   // UBOX scratchpad CSR7
#define SR_ERROR_CODE_CSR                         (BIOSNONSTICKYSCRATCHPAD8_UBOX_MISC_REG)   // UBOX scratchpad CSR8
#define SR_PRINTF_SYNC_CSR                        (BIOSNONSTICKYSCRATCHPAD9_UBOX_MISC_REG)   // UBOX scratchpad CSR9
#define SR_RC_VERSION_CSR                         (BIOSNONSTICKYSCRATCHPAD12_UBOX_MISC_REG)  // UBOX scratchpad CSR12
#define SR_RC_REVISION_CSR                        (BIOSNONSTICKYSCRATCHPAD1_UBOX_MISC_REG)   // UBOX scratchpad CSR1
#define PIPE_DISPATCH_SYNCH_PSYSHOST  4

#define SR_MEMORY_DATA_STORAGE_DISPATCH_PIPE_CSR  (BIOSNONSTICKYSCRATCHPAD13_UBOX_MISC_REG)  // UBOX scratchpad CSR13
#define SR_MEMORY_DATA_STORAGE_COMMAND_PIPE_CSR   (BIOSNONSTICKYSCRATCHPAD14_UBOX_MISC_REG)  // UBOX scratchpad CSR14
#define SR_MEMORY_DATA_STORAGE_DATA_PIPE_CSR      (BIOSNONSTICKYSCRATCHPAD15_UBOX_MISC_REG)  // UBOX scratchpad CSR15

//
// bit definitions for the PCU_BIOS_SPARE2 register
//
#define PCU_BIOS_SPARE2_UDIMM                 0
#define PCU_BIOS_SPARE2_RDIMM                 BIT12
#define PCU_BIOS_SPARE2_LRDIMM                BIT13
#define PCU_BIOS_SPARE2_125V_DIMM             0
#define PCU_BIOS_SPARE2_135V_DIMM             BIT14
#define PCU_BIOS_SPARE2_150V_DIMM             BIT15
//
// TxEq translate RegValue to EqCode
//
#define  REG_TO_TXEQ                   1
#define  TXEQ_TO_REG                   0



#endif // #ifndef _SYS_HOST_CHIP_H_
