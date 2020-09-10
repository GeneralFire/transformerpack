/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2013 - 2017 Intel Corporation. <BR>

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

#ifndef _PLATFORM_SMI_FLASH_LIB_H_
#define _PLATFORM_SMI_FLASH_LIB_H_

#define SMI_FLASH_SIGNED_DEBUG 1

EFI_STATUS SpiFlashProgram(  VOID* Protocol,UINT32 *BaseAddress,  UINT64 *BufferAddress, UINT8 *Region,UINT32 size);

EFI_STATUS SpiFlashLock (  VOID* Protocol,BOOLEAN Lock  );

EFI_STATUS ReadStatusRegister (  VOID* Protocol,UINT8   *SpiStatus  );

EFI_STATUS SpiFlashErase (VOID* Protocol,UINT32 *BaseAddress, UINT8 *Region,UINT32 size);


// Check Heci HFS Register: FPT bit & Recovery BUP Loader Fault bit.
BOOLEAN IsMeFptBad (VOID);

UINT32 EnableHeci(  VOID  );

VOID RestoreHeciFD2(  UINT32                                  PchRcrbFD2  );

//return a smm Spi protocol pointer
EFI_STATUS PlatformSmiFlashSignedInit(VOID** SmmSpiProtocol);

UINT32
GetMERegionBase (VOID);

UINT32
GetMERegionLimit (VOID);

UINT32
GetPTURegionBase (VOID);

UINT32
GetPTURegionLimit (VOID);


UINT32
GetSPIBase (VOID);

VOID
PlatformHookAfterMEUpdate (VOID);

BOOLEAN
IsWarmResetFromMe (VOID);

BOOLEAN
IsSupportMERESETCommand (VOID);

UINT8 GetPlatformID (VOID);

UINT32 GetFdTempBufferAddress (VOID);

VOID ClearPlatformVariableAfterBiosUpdate (
  VOID* Protocol
);

/*++

Routine Description:

  Read FFS file from FV using SPI protocol Read command

Arguments:
  Protocol - SPI protocol

  FvBaseAddress - FV Base Address

  FFSGuid       - FFS to find & Read

  FFSDataSize   - Data size to read.

  FFSData       - Pointer to buffer for read.

  FFSSize       - FFS file size FYI for caller.

  skipheader - TRUE: skip to read the ffs and first section header,read from data directly, for one data section only ffs
               FALSE:read from header

Returns:

  None

--*/

EFI_STATUS
ReadFFSFileSPI (
    IN  VOID* Protocol,
    IN UINT32   FvBaseAddress,
    IN EFI_GUID FFSGuid,
    IN UINT32   FFSDataSize,
    IN OUT VOID *FFSData,
    OUT UINT32  *FFSSize,
    IN BOOLEAN  skipheader
    );

/**
  PlatformHook for Smi Flash Hander
  @param

  @retval      : EFI_STATUS Status

**/

EFI_STATUS
HookForSmiHander(
VOID
);

/**
  PlatformHook for Smi Flash Update entry
  @param

  @retval      : EFI_STATUS Status

**/

EFI_STATUS
HookForSmiSignedEntry (
  VOID
  );

/**
  PlatformHook for Smi Flash Update Handler
  This Hook is called to setup Periodic Processing of the incoming capsule
  @param  CapsuleHeader               -- Input capsule header

  @retval      : EFI_ACCESS_DENIED    -- Firmware access is prohibited by platform specific mechanism
  @retval      : EFI_SUCCESS          -- Specific Periodic handler has been started
  @retval      : EFI_NOT_FOUND        -- Capsule is not supported
  @retval      : EFI_UNSUPPORTED      -- Hook did not provide platform specific support

**/
EFI_STATUS
HookForSmiUpdateHandler (
  IN  EFI_CAPSULE_HEADER          *CapsuleHeader
  );

/**
  PlatformHook for Smi Firmware Status Reporting
  @param

  @retval      : EFI_STATUS Status

**/

EFI_STATUS
HookForSmiStatusHandler (
  VOID
  );

/**
  PlatformHook for PeriodicTimer Event
  This Hook supports delays measured as periodic events or can be used to
  signal eminent UNREGISTERing of the Periodic timer.  The UNREGISTER event can be
  postponed by loading "mPeriodicDelay" with a non-zero count

  @retval      : EFI_UNSUPPORTED == Proceed as if Hook has no impact (Note: UNREGISTER == FALSE if delay != 0)
               : EFI_SUCCESS == UNREGISTER the Periodic Timer

**/
EFI_STATUS
HookForSMIPeriodicTimer (
  VOID
  );


/**
  PlatformHook for HW Security Lock associated with Capsule Image at designated buffer address
  @param  VOID* SecureCapsuleBuffer

  @retval      : EFI_STATUS Status

**/
EFI_STATUS
HookForHwSecurityLock (
  VOID* SecureCapsuleBuffer
  );

/**
  PlatformHook for SecurityRevision Check made during ChallengeCapsule phase

  @param  VOID* SecureCapsuleBuffer

  @retval      : EFI_UNSUPPORTED -- Not supported by hook
  @retval      : EFI_NOT_FOUND   -- Capsule recognized/HW mismatch
  @retval      : EFI_SUCCESS     -- Proceed with SHA Challenge

**/
EFI_STATUS
HookForSecurityRev (
  VOID* SecureCapsuleBuffer
  );



EFI_STATUS
HookForTotalFvsize(
  IN  EFI_CAPSULE_HEADER          *CapsuleHeader,
  IN OUT  UINT32                  *pTotalFvSize,
  IN UINT8 *                     pCapsuleSmmBuffer,
  IN BOOLEAN                     PasswordValid
  );

EFI_STATUS
HookAfterCapsuleUpdateComplete(
VOID
);


VOID
HookForNvramUpdateEvent(
VOID* pSpiProtocol
);

VOID
HookForParseBIOSCapsule(
  IN VOID  *pStateElement,
  IN UINTN *pTotalBlock,
  IN UINT32 TotalFvSize,
  IN UINT8 *pCapsuleSmmBuffer
);

VOID
 SetOemIDPcd (
  UINT64                TempOemId,
  UINT64                TempOemTableId
);


#endif //
