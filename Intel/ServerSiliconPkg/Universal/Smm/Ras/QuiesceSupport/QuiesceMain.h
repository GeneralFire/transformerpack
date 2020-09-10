/** @file QuiesceMain.h
  Main Quiesce service definition

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2009 - 2018 Intel Corporation. <BR>

  The source code contained or described herein and all documents related to the
  source code ("Material") are owned by Intel Corporation or its suppliers or
  licensors. Title to the Material remains with Intel Corporation or its suppliers
  and licensors. The Material may contain trade secrets and proprietary and
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

#ifndef _QUIESCE_MAIN_H_
#define _QUIESCE_MAIN_H_

#include "QuiesceSupport.h"
#include <Protocol/QuiesceProtocol.h>
#include <Library/BaseLib.h>
#include <Library/SynchronizationLib.h>
#include <Protocol/MpService.h>
#include <Protocol/SmmCpu.h>
#include <Protocol/SmmCpuService.h>
#include <Library/SmmServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Protocol/IioUds.h>
#include <Register/ArchitecturalMsr.h>
#include <Cpu/CpuCoreRegs.h>
#include <Protocol/IioSystem.h>
#include <Register/Cpuid.h>

#define QUIESCE_TABLE_SIZE            (64 * 1024)             //64K
#define QUIESCE_CACHE_CODE_SIZE       QUIESCE_TABLE_SIZE      //64K
#define QUIESCE_CACHE_DATA_SIZE       QUIESCE_TABLE_SIZE      //64K
#define PAGE_SIZE_4K                  0x1000
#define QUIESCE_PAGE_DIRECTORY_SIIZE  PAGE_SIZE_4K      //4K for paget directory entry
#define QUIESCE_PAGE_TABLE_SIZE       PAGE_SIZE_4K      //4K for paget table entry

#define PROGRAM_SELF                  0
#define PROGRAM_ALL                   1

#define B_QUIESCE_CTL1_QUIESCE      0x01  //bit0
#define B_QUIESCE_CTL1_UNQUIESCE    0x02  //bit1
#define B_QUIESCE_CTL1_EVIC_PENDING 0x04  //bit2
#define B_QUIESCE_CTL1_QUIESCE_CAP  0x80  //bit7

#define B_QUIESCE_CTL2_DISABLE_PSMI 0x01

#ifndef MSR_IA32_CR_PAT
#define MSR_IA32_CR_PAT                       0x00000277
#endif
#define PAT_MSR_DEFAULT   (0x0000040600000406ULL)



//BKL_PORTING - Needs porting for Platform
// CSRs
#define PCU_DESIRED_CORES_CSR      ((10 << 15) + (1 << 12) + 0xA4)
#define MT_DISABLE_FLAG            (1 << 30)

#define BIOSSCRATCHPAD2_CSR        ((11 << 15) + (3 << 12) + 0x48)


//BKL_PORTING

#define B_QUIESCE_ON            0x01    //bit0
#define B_RECONFIG_DONE         0x02    //bit1
#define B_UNQUIESCED            0x04    //bit2

// for Monarch AP Status
#define B_MONARCH_AP_READY      0x010    //bit8 for monarch AP ready for config
#define B_MONARCH_AP_DONE       0x020    //bit9 for monarch AP ready for join back to main SMI


extern QUIESCE_COMMUNICATION_DATA_STRUCT mQuiesceCommData;
extern UINT32                             mSocketLevelShift;

VOID
QuiesceApHandler(
  IN EFI_QUIESCE_SUPPORT_PROTOCOL *QuiesceSupport
  );


UINT64
GetNumberOfProcessorsinSmm (
  VOID
  );

  VOID
Save_Misc_Feature_Msr (
  UINT64 *Value
);

VOID
QuiesceMainInit(
  VOID
);

#endif
