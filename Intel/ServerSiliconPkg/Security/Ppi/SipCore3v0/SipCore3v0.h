/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation. <BR>

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

#ifndef _SIP_CORE_3V0_H_
#define _SIP_CORE_3V0_H_

#include <Ppi/MpServices.h>              // # CONSUMES
#include <Ppi/SipCore3v0Ppi.h>           // # PRODUCES

#include <Library/BaseMemoryLib.h>      // ZeroMem
#include <Cpu/CpuCoreRegs.h>            // SprMsr
#include <Library/MemoryAllocationLib.h> //AllocatePages

//#include <Library/PeiServicesLib.h>
#include <Register/Cpuid.h>
#include <Register/ArchitecturalMsr.h>
#include <Base.h>
#include <PiPei.h>
#include <Uefi.h>
#include <Uefi/UefiBaseType.h>
#include <Guid/SgxInitDataSpr.h>
#include <Library/SgxDebugLib.h>        // SGX Debug Queue

SIP_CORE_3V0_PROGRAM_PRMRR _ProgramPrmrr;
SIP_CORE_3V0_LOCK_PRMRR    _LockPrmrr;

EFI_STATUS
SgxMpServicesData (
  IN CONST EFI_PEI_SERVICES **PeiServices
  );

VOID
CoreScopeProgramCorePrmrr (
  VOID
  );

#endif // _SIP_CORE_3V0_H_
