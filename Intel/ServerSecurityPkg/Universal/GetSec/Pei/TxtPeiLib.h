/** @file
  This file contains function definitions that can determine
  the LT capabilities of a platform during PEI and perform
  certain specific platform tasks that are required for LT
  during PEI.

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
**/

#ifndef _TXT_PEI_LIB_H_
#define _TXT_PEI_LIB_H_

#include <Cpu/CpuCoreRegs.h>
#include <Register/ArchitecturalMsr.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/LocalApicLib.h>
#include <Library/PcdLib.h>
#include <Ppi/TxtSclean.h>
#include <Ppi/CpuIo.h>
#include <Ppi/Stall.h>
#include <Ppi/MasterBootMode.h>
#include <Ppi/MpServices.h>
#include <Guid/PlatformTxt.h>
#include <Register/Cpuid.h>
#include <Cpu/Msr/CommonMsr.h>
#include <Library/TimerLib.h>

#define   B_MSR_LTCTRLSTS_LOCK     BIT0
#define LT_SPAD_DONE	             0x0A0
#define LT_ESTS                    0x008
#define LT_E2STS                   0x8F0
#define TXT_PUBLIC_BASE            0xFED30000

// This is only defined in platform include files
//  so we need a definition at the ServerCommon level
//
// The PCD is defined in CpRcPkg.dec and can be overridden in
//  PlatformPkg.dsc
//
#define MAX_SOCKET_COUNT           FixedPcdGet32 (PcdMaxCpuSocketCount)

extern EFI_GUID   gPeiPostScriptTablePpiGuid;
extern EFI_GUID   gPeiTxtMemoryUnlockedPpiGuid;
extern EFI_GUID   gEfiPeiMemoryDiscoveredPpiGuid;

typedef struct _LT_PEI_LIB_CONTEXT_ {
  CONST EFI_PEI_SERVICES           **PeiServices;
  EFI_PEI_CPU_IO_PPI               *CpuIoPpi;
  EFI_PEI_PCI_CFG2_PPI             *PciCfgPpi;
  EFI_PEI_STALL_PPI                *PeiStall;
  EFI_PEI_MP_SERVICES_PPI          *PeiMpServices;
  EFI_PLATFORM_TXT_DEVICE_MEMORY   *PlatformTxtDeviceMemory;
  EFI_PLATFORM_TXT_POLICY_DATA     *PlatformTxtPolicyData;
  UINT8                            ApVector;
  UINT8                            ApCount;
} LT_PEI_LIB_CONTEXT;

//
// Structure used to hold Socket information for S3 resume
//
typedef struct {
  UINT8     SocketIndex;
  UINT8     SocketFoundId;
  UINT16    ApApicId;
} LT_PEI_SOCKET_INFO;

EFI_STATUS
InitializeLtPeiLib (
  IN      CONST EFI_PEI_SERVICES  **PeiServices,
  IN OUT  LT_PEI_LIB_CONTEXT      *LtPeiCtx
  );

BOOLEAN
IsLtProcessor (
  IN      LT_PEI_LIB_CONTEXT      *LtPeiCtx
  );

VOID
EnableTxt (
  IN      LT_PEI_LIB_CONTEXT      *LtPeiCtx
  );

VOID
LockLtMsr (
  IN      VOID                    *Buffer
  );

VOID
LockLtMsrOnAll(
  IN      LT_PEI_LIB_CONTEXT      *LtPeiCtx
  );

BOOLEAN
IsLtSxProcessor (
  IN      LT_PEI_LIB_CONTEXT      *LtPeiCtx
  );

BOOLEAN
IsTpmPresent (
  IN      LT_PEI_LIB_CONTEXT      *LtPeiCtx
  );

BOOLEAN
IsLtEstablished (
  IN      LT_PEI_LIB_CONTEXT      *LtPeiCtx
  );

BOOLEAN
IsLtWakeError (
  IN      LT_PEI_LIB_CONTEXT      *LtPeiCtx
  );

BOOLEAN
IsLtResetStatus (
  IN      LT_PEI_LIB_CONTEXT      *LtPeiCtx
  );

BOOLEAN
IsLtEnabled (
  IN      LT_PEI_LIB_CONTEXT      *LtPeiCtx
  );

VOID
PowerCycleReset (
  IN      LT_PEI_LIB_CONTEXT      *LtPeiCtx
  );

EFI_STATUS
DoClearSecrets (
  IN      LT_PEI_LIB_CONTEXT      *LtPeiCtx
  );

EFI_STATUS
DoSclean (
  IN      LT_PEI_LIB_CONTEXT      *LtPeiCtx
  );

EFI_STATUS
PeiTxtLockConfigForAll (
  IN      LT_PEI_LIB_CONTEXT      *LtPeiCtx
  );

EFI_STATUS
PeiDoLockConfigForAll (
  IN      LT_PEI_LIB_CONTEXT      *LtPeiCtx
  );

VOID
DoLockConfig (
  IN      VOID                    *Buffer
  );

VOID
EFIAPI
DoLockConfigOnAP (
  IN      VOID                    *Buffer
  );

EFI_STATUS
EFIAPI
TxtPeimEntry (
  IN          EFI_PEI_FILE_HANDLE FileHandle,
  IN    CONST EFI_PEI_SERVICES    **PeiServices
  );

// BEGIN_OVERRIDE_HSD_5331848
//
// LtPeiLibLaunchBiosAcm is an internal routine used to wrap the BIOS ACM initialization code for the various functions inside of it.
//
STATIC
EFI_STATUS
LtPeiLibLaunchBiosAcm(
IN LT_PEI_LIB_CONTEXT      *LtPeiCtx,
IN UINT32                  AcmFunction
);
// END_OVERRIDE_HSD_5331848

#endif
