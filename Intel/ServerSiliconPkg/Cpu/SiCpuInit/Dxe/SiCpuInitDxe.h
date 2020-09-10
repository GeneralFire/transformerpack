/** @file
  Include file for Silicon CPU Initialize DXE Module.

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

#ifndef _SI_CPU_INIT_DXE_H_
#define _SI_CPU_INIT_DXE_H_


#include <Protocol/CpuPolicyProtocol.h>
#include <Protocol/MpService.h>
#include <Protocol/SmmConfiguration.h>
#include <Protocol/DxeSmmReadyToLock.h>
#include <Protocol/CpuCsrAccess.h>

#include <Register/ArchitecturalMsr.h>
#include <Register/Cpuid.h>
#include <Register/Microcode.h>
#include <Cpu/CpuCoreRegs.h>
#include <CpuDataStruct.h>
#include <RcRegs.h>

#include <Library/BaseLib.h>
#include <Library/SynchronizationLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiLib.h>
#include <Library/HobLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/PcdLib.h>
#include <Library/CpuConfigLib.h>
#include <Library/TimerLib.h>
#include <Library/LocalApicLib.h>
#include <Library/RegisterCpuFeaturesLib.h>
#include <Library/SystemInfoLib.h>
#include <Library/CpuSmbiosLib.h>
#include <Library/MicrocodeLib.h>
#include <Library/CpuEarlyDataLib.h>

#include <Guid/MicrocodePatchHob.h>

//
// Define MP data block which consumes individual processor block.
//
typedef struct {
  SPIN_LOCK                   APSerializeLock;
} MP_SYSTEM_DATA;

extern CPU_POLICY_CONFIGURATION     *mCpuPolicyConfiguration;
extern EFI_CPU_CSR_ACCESS_PROTOCOL  *mCpuCsrAccess;
extern MP_SYSTEM_DATA               mMPSystemData;
extern EFI_MP_SERVICES_PROTOCOL     *mMpServices;
extern CPU_VAR_DATA                 *mCpuVarDataPtr;

/**
  Detect whether specified processor can find matching mirocode patch.

  This function detects whether specified processor can find matching mirocode patch.

  @param  ProcessorNumber   The handle number of specified processor.

**/
VOID
MicrocodeDetect (
  IN UINTN   ProcessorNumber
  );

/**
  Collects basic processor data for calling processor.

  This function collects basic processor data for calling processor.

  @param  ProcessorNumber    Handle number of specified logical processor.

**/
VOID
CollectBasicProcessorData (
  IN UINTN  ProcessorNumber
  );

/**
  Collects data from all logical processors.

  This function collects data from all logical processors

**/
VOID
DataCollectionPhase (
  VOID
  );

/**
  Select least-feature processor as BSP.

  This function selects least-feature processor as BSP.

**/
VOID
SelectLfpAsBsp (
  VOID
  );

/**
  Collects processor microcode revision of the processor installed in the system.

  This function collects processor microcode revision of the processor installed in the system.

  @param  ProcessorNumber    Handle number of specified logical processor

**/
VOID
CollectMicrocodeRevision (
  UINTN    ProcessorNumber
  );

/**
  Get the cached microcode patch base address and size.

  @retval  EFI_SUCCESS             The microcode base address and size are
                                   updated.
  @retval  EFI_OUT_OF_RESOURCES    Not enough memory to complete the request.

**/
EFI_STATUS
GetMicrocodePatchInfo (
  VOID
  );

#endif
