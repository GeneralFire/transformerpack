/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2008 - 2020 Intel Corporation. <BR>

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

#ifndef _RAS_INIT_H
#define _RAS_INIT_H

#include <BackCompatible.h>
#include <Library/SmmServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/HpIoxAccess.h>
#include <Protocol/QuiesceProtocol.h>
#include <Protocol/MemRasProtocol.h>
#include <Protocol/CpuRasProtocol.h>
#include <Protocol/IioRasProtocol.h>
#include <Protocol/CpuHotAddData.h>
#include <Protocol/GlobalNvsArea.h>
#include <Protocol/IioUds.h>
#include <Guid/AcpiVariable.h>
#include <Library/SynchronizationLib.h>
#include <Protocol/SpsRasNotifyProtocol.h>

#include <Protocol/SmmGpiDispatch2.h>
#include <Protocol/SmmPeriodicTimerDispatch2.h>

#include <Library/BaseMemoryLib.h>
#include <Library/BaseLib.h>
#include <Protocol/MpService.h>
#include <Library/LocalApicLib.h>
#include <Library/PciLib.h>
#include <Library/PciExpressLib.h>
#include <Library/DebugLib.h>
#include <Library/PreSiliconEnvDetectLib.h>
#include <Protocol/SmmCpuService.h>
#include <Ras/RasStateMachine.h>
#include <Register/PchRegsLpc.h>
#include <PchAccess.h>
#include <Library/MmPciBaseLib.h>
#include <Library/IoLib.h>
#include <Register/PchRegsPmc.h>
#include <Library/UbaGpioPlatformConfig.h>
#include <Library/MemMapDataLib.h>

#include <Protocol/PciRootBridgeIo.h>
#include <Protocol/SmmPeriodicTimerDispatch2.h>
#include <Protocol/SmmSwDispatch2.h>
#include <Library/PreSiliconEnvDetectLib.h>
#include <Library/SystemInfoLib.h>
#include <Register/Cpuid.h>

#define   B_EFI_MSR_IA32_APIC_BASE_G_XAPIC      BIT11
#define   B_EFI_MSR_IA32_APIC_BASE_M_XAPIC      BIT10
#define   B_EFI_MSR_IA32_APIC_BASE_BSP          BIT8

#define EFI_EXT_XAPIC_LOGICAL_APIC_ID         0x00000802

#define APIC_REGISTER_LOCAL_ID_OFFSET         0x00000020

#define DEBUG_ACCESS(x)            *(volatile UINT8 *)(UINTN)(x)

#define PCI_DEVICE_END    0xFF

#ifndef EFI_DEADLOOP
 #define EFI_DEADLOOP()    { volatile int __iii; __iii = 1; while (__iii); }
#endif


//
//
//
VOID
InitHpGPI(
  );

EFI_STATUS
EnableGpiChildHandler (
  );

EFI_STATUS
EFIAPI
RasInitGpiChildCallback (
  IN EFI_HANDLE     DispatchHandle,
  IN CONST VOID    *mGpiContext,
  IN OUT VOID      *CommBuffer      OPTIONAL,
  IN OUT UINTN     *CommBufferSize  OPTIONAL
  );


VOID
HpGPIHandler(
  );

EFI_STATUS
EFIAPI
RasPeriodicChildCallback (
  IN  EFI_HANDLE                  PeriodicTimerHandle,
  IN  CONST VOID     *mPeriodicTimer,
  IN  OUT VOID      *CommBuffer   OPTIONAL,
  IN  OUT UINTN     *CommBufferSize OPTIONAL
  );

VOID
RegisterPeriodicTimer(
  VOID
  );

EFI_STATUS
RasPeriodicTimerSmiHandler(
  VOID
  );

EFI_STATUS
CpuRasHandler(
  VOID
  );

EFI_STATUS
MemRasInit(
  VOID
  );

EFI_STATUS
MemRasHandler(
  VOID
  );

EFI_STATUS
IioRasHandler(
  VOID
  );

EFI_STATUS
GetSourceOfGpi(
  VOID
  );

typedef
EFI_STATUS
(EFIAPI *GET_SOURCE_OF_GPI) (
  VOID
  );

typedef
EFI_STATUS
(EFIAPI *CPU_RAS_HANDLER) (
  VOID
  );

typedef
EFI_STATUS
(EFIAPI *MEM_RAS_HANDLER) (
  VOID
  );

typedef
EFI_STATUS
(EFIAPI *IIO_RAS_HANDLER) (
  VOID
  );

typedef struct _RAS_DISPATCHER_TABLE {
  GET_SOURCE_OF_GPI         GetSourceOfGpi;
  CPU_RAS_HANDLER           CpuRasHandler;
  MEM_RAS_HANDLER           MemRasHandler;
  IIO_RAS_HANDLER           IioRasHandler;
  UINTN                     EndOfEntry;
} RAS_DISPATCHER_TABLE;

typedef
EFI_STATUS
(EFIAPI *NEXT_ENTRY_POINTER) (
  VOID
  );

typedef struct _NEXT_STATE_ENTRY {
  NEXT_ENTRY_POINTER     NextEntryPointer;
} NEXT_STATE_ENTRY;

EFI_STATUS
GenerateRasSci (
  VOID
  );

VOID
InitMemoryRas (
  VOID
);

VOID
InitIioRas (
  VOID
);


EFI_STATUS
EFIAPI
RasInitSwSmiChildCallback (
  IN EFI_HANDLE  DispatchHandle,
  IN CONST VOID     *mGpiContext,
  IN OUT VOID    *CommBuffer      OPTIONAL,
  IN OUT UINTN   *CommBufferSize  OPTIONAL
);

EFI_STATUS
RasSmiHandler(
  VOID
 );

EFI_STATUS
UpdateAcpiMemoryParams (
  IN UINT8         SocketId,
  IN UINT8         BranchId,
  IN BOOLEAN       Ejection
);

VOID
ModifyCacheLines (
   UINTN  StartAddress,
   UINTN  Length,
   UINTN  CacheLineSize
   );

VOID
ModifyCacheLinesWithFlush (
   UINTN  StartAddress,
   UINTN  Length,
   UINTN  CacheLineSize
   );

#endif
