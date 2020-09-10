/** @file
  Header File for CPU Power Management

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2006 - 2018 Intel Corporation. <BR>

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

#ifndef _PPM_INITIALIZE_H
#define _PPM_INITIALIZE_H

#include <Library/PcdLib.h>
#include <Protocol/MpService.h>
#include <IioUniversalData.h>
//Libraries
#include <Library/HobLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiLib.h>
#include <Library/SynchronizationLib.h>
#include <Library/UsraCsrLib.h>
#include <Guid/HobList.h>
#include <Protocol/PpmPolicyProtocol.h>
#include <Protocol/CpuPolicyProtocol.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/CpuPpmLib.h>
#include "SysHost.h"
#include "UncoreCommonIncludes.h"
#include <Cpu/CpuCoreRegs.h>

extern EFI_MP_SERVICES_PROTOCOL     *mMpService;
extern PPM_FROM_PPMINFO_HOB         *mPpmInfo;
extern EFI_CPU_PM_STRUCT            mPpmLib;
extern IIO_UDS                      *mUdsHobPtr;
extern PPM_POLICY_CONFIGURATION     *mPpmPolicy;

/* CPUPM POST code - Major */
#define STS_PPM_STRUCT_INIT        0xD0  // CPU PM Structure Init
#define STS_PPM_CSR_PROGRAMMING    0xD1  // CPU PM CSR programming
#define STS_PPM_MSR_PROGRAMMING    0xD2  // CPU PM MSR programming
#define STS_PPM_PSTATE_TRANSITION  0xD3  // CPU PM PSTATE transition
#define STS_PPM_EXIT               0xD4  // CPU PM driver exit
#define STS_PPM_ON_READY_TO_BOOT   0xD5  // CPU PM On ready to boot event

VOID
EFIAPI
InitializeCpuInfoStruct(
  VOID
  );


#endif
