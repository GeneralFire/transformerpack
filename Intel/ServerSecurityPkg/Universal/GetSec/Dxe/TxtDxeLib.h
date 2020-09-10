/** @file
  This file contains function definitions that can determine
  the LT capabilities of a platform during DXE and perform
  certain specific platform tasks that are required for LT
  during DXE.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2007 - 2018 Intel Corporation. <BR>

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

#ifndef _TXT_DXE_LIB_H_
#define _TXT_DXE_LIB_H_

#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Protocol/MpService.h>
#include <Guid/PlatformTxt.h>
#include <Protocol/CpuIo2.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Protocol/FirmwareVolume2.h>
#include <Protocol/LoadedImage.h>
#include <Protocol/PciRootBridgeIo.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/LegacyBios.h>
#include <Protocol/LegacyRegion2.h>
#include <Guid/HobList.h>
#include <Library/S3BootScriptLib.h>
#include <Library/BaseLib.h>
#include <Library/PcdLib.h>
// OVERRIDE_HSD_4757591_BEGIN
#include <Library/DxeServicesTableLib.h>
// OVERRIDE_HSD_4757591_END
#include <Protocol/ReadyForLock.h>
#include <Protocol/LegacyRegion2.h>
#include <Protocol/PciCallback.h>
#include <FlashMap.h>
#include <Register/Cpuid.h>
#include <Cpu/Msr/CommonMsr.h>

#define   B_MSR_LTCTRLSTS_LOCK              BIT0
#define LT_SPAD_DONE_HIGH                   0x0A4
#define LT_SPAD_DONE_LOW                    0x0A0
#define TXT_PUBLIC_BASE                     0xFED30000

// APTIOV_SERVER_OVERRIDE_RC_START
#ifndef SPI_OPCODE_READ_INDEX
#define SPI_OPCODE_READ_INDEX               4
#endif
// APTIOV_SERVER_OVERRIDE_RC_END
#define ACM_BASE_ADDRESS_MASK               0x00FFFFFF
#define ACM_TYPE_OFFSET                     0x0F
#define ACM_READ_SIZE                       0x10

// This is only defined in platform include files
//  so we need a definition at the ServerCommon level
//
// The PCD is defined in CpRcPkg.dec and can be overridden in
//  PlatformPkg.dsc
//
#define MAX_SOCKET_COUNT                    FixedPcdGet32 (PcdMaxCpuSocketCount)

//
// Chispet register
//
#define SA_DPR_REG                          0x290
#define   SA_DPR_LOCK                       0x1
#define   SA_DPR_PRM                        0x4

#define LEGACY_BIOS_MEMORY_SEGMENT          0x0F0000
#define LEGACY_BIOS_MEMORY_LENGTH           0x010000

//
// OVERRIDE_HSD_4757591_BEGIN
//
//
// Define the TPM Decode and TXT Public and Private spaces:
// - TPM Decode  0xFED40000 - 0xFED44FFF
// - TXT Public  0xFED30000 - 0xFED3FFFF
// - TXT Private 0xFED20000 - 0xFED2FFFF
//
#define TXT_PUBLIC_PRIVATE_BASE  0xFED20000   // TPM Decode and TXT Public/Private reg address
#define TXT_PUBLIC_PRIVATE_SIZE  0x25000      // TPM Decode and TXT Public/Private reg size

//
// OVERRIDE_HSD_4757591_END
//
extern  EFI_RUNTIME_SERVICES          *gRT;
extern  EFI_GUID                      gPlatformTokenSpaceGuid;

typedef struct _LT_DXE_LIB_CONTEXT_ {
  EFI_HANDLE                          ImageHandle;
  EFI_SYSTEM_TABLE                    *SystemTable;
  EFI_CPU_IO2_PROTOCOL                *CpuIo;
  EFI_MP_SERVICES_PROTOCOL            *MpService;
  EFI_PLATFORM_TXT_DEVICE_MEMORY      *PlatformTxtDeviceMemory;
  EFI_PLATFORM_TXT_POLICY_DATA        *PlatformTxtPolicyData;
  UINT8                               ApVector;
  UINT16                              ApCount;
  UINT32                              ApicId[512];
} LT_DXE_LIB_CONTEXT ;
// APTIOV_SERVER_OVERRIDE_RC_START : TXT Error and status Register message printing.
#define TXT_ACM_STATUS_LO  0xFED300A0
#define TXT_ACM_STATUS_HI  0xFED300A4
#define TXT_ACM_ERRORCODE  0xFED30030
#define TXT_BIOS_ACM_ERRORCODE  0xFED30328

VOID PrintLtsxErrors(
);

// APTIOV_SERVER_OVERRIDE_RC_END : TXT Error and status Register message printing.

EFI_STATUS
InitializeLtDxeLib (
  IN      EFI_HANDLE              ImageHandle,
  IN      EFI_SYSTEM_TABLE        *SystemTable,
  IN OUT  LT_DXE_LIB_CONTEXT      *LtDxeCtx
  );

EFI_STATUS
DisableLtsxAndReset (
  VOID
  );

BOOLEAN
IsLtEstablished (
  IN      LT_DXE_LIB_CONTEXT      *LtDxeCtx
  );

BOOLEAN
IsLtProcessor (
  IN      LT_DXE_LIB_CONTEXT      *LtDxeCtx
  );

BOOLEAN
IsLtEnabled (
  IN      LT_DXE_LIB_CONTEXT      *LtDxeCtx
  );

BOOLEAN
IsTpmPresent (
  IN      LT_DXE_LIB_CONTEXT      *LtDxeCtx
  );

BOOLEAN
IsLtResetStatus (
  IN      LT_DXE_LIB_CONTEXT      *LtDxeCtx
  );

#ifdef SKX_HOST
EFI_STATUS
DoScheck (
  IN      LT_DXE_LIB_CONTEXT      *LtDxeCtx
  );
#endif //SKX_HOST

EFI_STATUS
DoLockConfig (
  IN      LT_DXE_LIB_CONTEXT      *LtDxeCtx
  );

EFI_STATUS
ClearTpmAuxIndex (
  IN      LT_DXE_LIB_CONTEXT      *LtDxeCtx
  );

EFI_STATUS
ResetTpmEstBit (
  IN      LT_DXE_LIB_CONTEXT      *LtDxeCtx
  );

EFI_STATUS
TxtLockConfigForAll (
  IN      LT_DXE_LIB_CONTEXT      *LtDxeCtx
  );


#endif
