/** @file
  Header file for SMM S3 Handler Driver.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2009 - 2020 Intel Corporation. <BR>

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

#ifndef _ACPI_SMM_DRIVER_H
#define _ACPI_SMM_DRIVER_H
//
// Include files
//
#include "SmmVariable.h"
//
// Driver Consumed Protocol Prototypes
//
#include <Protocol/SmmPowerButtonDispatch2.h>
#include <Protocol/SmmSxDispatch2.h>
#include <Protocol/SmmSwDispatch2.h>
#include <Protocol/IioUds.h>
#include <Protocol/GlobalNvsArea.h>
#include <Protocol/PciRootBridgeIo.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Acpi/Fadt.h>
#include <Protocol/Spi.h>
#include <Library/SmmScriptLib.h>
#include <Library/IoLib.h>
#include <Library/PciLib.h>
#include <Library/PcdLib.h>
#include <Library/LockBoxLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/S3IoLib.h>
#include <Library/S3BootScriptLib.h>
#include <Guid/Acpi.h>
#include <Guid/GlobalVariable.h>
#include <Guid/MemoryOverwriteControl.h>
//APTIOV_SERVER_OVERRIDE_START: Remove Missing Headers
//#include <Guid/PhysicalPresenceData.h>
//APTIOV_SERVER_OVERRIDE_END: Remove Missing Headers
#include <Library/SmmServicesTableLib.h>
#include <Guid/SetupVariable.h>
#include <PchSetupVariable.h>
#include <Guid/SmramMemoryReserve.h>
#include <Library/ReportStatusCodeLib.h>
#include <Library/HobLib.h>
#include <Library/SetupLib.h>
#include "Platform.h"
#include <Library/MmPciBaseLib.h>

#include <Library/PchInfoLib.h>
#include <Library/UsraAccessApi.h>


#define CMOS_ADDR_PORT          0x70
#define CMOS_DATA_PORT          0x71

#define RTC_ADDRESS_REGISTER_D    13

#define PCI_DEVICE(Bus, Dev, Func)  \
          Bus, Dev, Func

#define PCI_REG_MASK(Byte0, Byte1, Byte2, Byte3, Byte4, Byte5, Byte6, Byte7) \
          Byte0, Byte1, Byte2, Byte3, Byte4, Byte5, Byte6, Byte7

#define PCI_DEVICE_END    0xFF

//
// Related data structures definition
//
typedef struct _EFI_ACPI_SMM_DEV {

  //
  // Parent dispatch driver returned sleep handle
  //
  EFI_HANDLE  PowerButtonHandle;

  EFI_HANDLE  S3SleepEntryHandle;

  EFI_HANDLE  S4SleepEntryHandle;

  EFI_HANDLE  S1SleepEntryHandle;

  EFI_HANDLE  S5SoftOffEntryHandle;

  EFI_HANDLE  EnableAcpiHandle;

  EFI_HANDLE  DisableAcpiHandle;

  EFI_HANDLE  PpCallbackHandle;

  EFI_HANDLE  MorCallbackHandle;

  //
  // PCH Power Management I/O base (aka ACPI Base)
  //
  UINT16      PchPmBase;

  UINT32      BootScriptSaved;

} EFI_ACPI_SMM_DEV;

//
// Prototypes
//
EFI_STATUS
InitPlatformAcpiSmm (
  IN EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE     *SystemTable
  );

EFI_STATUS
EFIAPI
SxSleepEntryCallBack (
  IN  EFI_HANDLE                    DispatchHandle,
  IN CONST EFI_SMM_SX_REGISTER_CONTEXT  *DispatchContext,
  IN OUT   VOID                         *CommBuffer,     OPTIONAL
  IN OUT   UINTN                        *CommBufferSize  OPTIONAL
  );

EFI_STATUS
RegisterToDispatchDriver (
  VOID
  );

EFI_STATUS
SaveRuntimeScriptTable (
  IN EFI_SMM_SYSTEM_TABLE2       *Smst
  );

EFI_STATUS
EFIAPI
BmcAcpiSwChildCallbackFunction (
  IN CONST EFI_GUID  *Protocol,
  IN VOID            *Interface,
  IN EFI_HANDLE      Handle
  );

EFI_STATUS
EFIAPI
SetBmcAcpiPowerStateCallback (
  IN  EFI_HANDLE                    DispatchHandle,
  IN  CONST VOID                    *DispatchContext,
  IN  OUT VOID                      *CommBuffer,
  IN  OUT UINTN                     *CommBufferSize
  );

EFI_STATUS
EFIAPI
PowerButtonCallback (
  IN  EFI_HANDLE                              DispatchHandle,
  IN  CONST VOID                              *DispatchContext,
  IN  OUT VOID                                *CommBuffer  OPTIONAL,
  IN  UINTN                                   *CommBufferSize  OPTIONAL
  );

EFI_STATUS
RestorePchS3SwCallback (
  IN  EFI_HANDLE                    DispatchHandle,
  IN  CONST VOID                    *DispatchContext,
  IN  OUT VOID                      *CommBuffer,
  IN  OUT UINTN                     *CommBufferSize
  );

extern EFI_GUID gPchS3CodeInLockBoxGuid;
extern EFI_GUID gPchS3ContextInLockBoxGuid;

#endif
