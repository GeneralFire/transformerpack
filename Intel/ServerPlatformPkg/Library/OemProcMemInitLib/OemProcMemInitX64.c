/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2005 - 2019 Intel Corporation. <BR>

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

#include <Platform.h>
#include <Guid/MemoryOverwriteControl.h>
#include <Library/SetupLib.h>
#include <Library/OemProcMemInitLib.h>

#include <Library/ReportStatusCodeLib.h>
#include <Library/PlatformStatusCodes.h>
#include <Library/BaseMemoryLib.h>
#include <Library/SysHostPointerLib.h>

/**

  Platform hook to Initialize Memory data

  @param[in] Host = pointer to sysHost structure

**/
VOID
PlatformInitializeMemoryData (
  IN PSYSHOST  Host
  )
{
}

/**

  Platform hook pre memory init

  @param[in] Host  - Pointer to SysHost

  @retval EFI_SUCCESS

**/
EFI_STATUS
EFIAPI
PlatformPreMemoryInit (
  IN PSYSHOST Host
  )
{
  return EFI_SUCCESS;
}

/**

  Platform hook post memory init

  @param[in] Host  - Pointer to SysHost

  @retval EFI_SUCCESS

**/
EFI_STATUS
EFIAPI
PlatformPostMemoryInit (
  IN PSYSHOST Host
  )
{
  return EFI_SUCCESS;
}

/**

  Platform hook before checking and triggering the proper type of reset.
  Executed by System BSP only.

  @param Host = pointer to sysHost structure

  @retval None

**/
VOID
PlatformPreHandleResetRequests (
  IN PSYSHOST             HOST
  )
{
  //
  // Perform Platform tasks before a reset might be triggered after control returns to the caller
  //
  return;
}

/**

  Puts the Host structure variables for POST

  @param Host - pointer to sysHost structure on stack

@retval VOID

**/
VOID
PlatformPublishDataForPost (
  IN PSYSHOST  Host
  )
{

}

/**

  Platform hook for initializing Tcrit to 105 temp offset & the config register
  which is inside of initialization of memory throttling

  @param socket            - socket number
  @param smbDevice dev     - SMB Device
  @param byteOffset        - byte Offset
  @param data              - data

**/
VOID
PlatformHookMst (
  UINT8             socket,
  SMB_DEVICE_STRUCT dev,
  UINT8             byteOffset,
  UINT8             *data
  )
{
  return;
}

/**

  Platform hook meant to be used by customers where they can use it to write their own jumper detection code
  and have it return FALSE when no jumper is present, thereby, blocking calling of the BSSA loader; else have it
  return TRUE if the concerned jumper is physically present. This check ensures that someone will have to be
  physically present by the platform to enable the BSSA Loader.

  @param[in] None

  @retval TRUE - This function returns TRUE by default as OEMs are meant to implement their own jumper detection
                 code(DetectPhysicalPresence).

**/
BOOLEAN
PlatformDetectPhysicalPresenceSSA (
  VOID
  )
{
  return TRUE;
}

/**
  Publish any Platform specfic HOBs.

  @param  None

  @retval EFI_SUCCESS

**/

EFI_STATUS
EFIAPI
PlatformPublishHobs (
  VOID
  )
{

  return EFI_SUCCESS;

} // PlatformPublishHobs
