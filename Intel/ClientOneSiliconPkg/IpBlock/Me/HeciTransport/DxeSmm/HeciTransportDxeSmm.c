 /** @file
  HECI Transport Protocol DXE SMM driver

 @copyright
  INTEL CONFIDENTIAL
  Copyright (c) 2019 - 2020 Intel Corporation.

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

  This file contains an 'Intel Peripheral Driver' and is uniquely identified as
  "Intel Reference Module" and is licensed for Intel CPUs and chipsets under
  the terms of your license agreement with Intel or your vendor. This file may
  be modified by the user, subject to additional terms of the license agreement.

**/

#include <PiDxe.h>
#include <Base.h>
#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/SmmServicesTableLib.h>
#include <Library/HeciTransportCoreLib.h>
#include <Library/HeciCommonLib.h>
#include <Library/MeUtilsLib.h>
#include <Protocol/HeciTransportProtocol.h>
#include <Protocol/SmmBase2.h>
#include <Protocol/HeciAccessProtocol.h>


#define HECI_TRANSPORT_DEBUG                    "[HECI Transport %s]"
//
// Global driver data
//
GLOBAL_REMOVE_IF_UNREFERENCED CHAR16            *mHeciModuleDebugPrefix;

/**
  HECI driver entry point used to initialize support for the HECI device
  for DXE/SMM

  @param[in] ImageHandle          Standard entry point parameter.
  @param[in] SystemTable          Standard entry point parameter.

  @retval EFI_SUCCESS             Returned for succeed execution
  @retval EFI_OUT_OF_RESOURCES    Can't allocate protocol data
  @retval EFI_UNSUPPORTED         Not supported when driver is called in non SMM context
  @retval EFI_DEVICE_ERROR        No HeciAccess protocol has been found
**/
EFI_STATUS
EFIAPI
HeciTransportEntryPoint (
  IN EFI_HANDLE                   ImageHandle,
  IN EFI_SYSTEM_TABLE             *SystemTable
  )
{
  EFI_STATUS                Status;
  HECI_TRANSPORT_PRIVATE    *HeciTransport;
  HECI_ACCESS               *HeciAccess;
  UINTN                     HeciTransportCounter;
  HECI_TRANSPORT_PRIVATE    *HeciTransportPrevious;
  HECI_TRANSPORT_PRIVATE    *HeciTransportRoot;
  EFI_GUID                  *SelectedHeciGuid;

  HeciTransportRoot = NULL;
  HeciTransportPrevious = NULL;
  mHeciModuleDebugPrefix = GetModuleIdentificationString ();
  if (mHeciModuleDebugPrefix == NULL) {
    ASSERT (mHeciModuleDebugPrefix != NULL);
    return EFI_OUT_OF_RESOURCES;
  }

  Status = EFI_SUCCESS;

  DEBUG ((DEBUG_INFO, HECI_TRANSPORT_DEBUG, mHeciModuleDebugPrefix));
  DEBUG ((DEBUG_INFO, " %a\n", __FUNCTION__));

  if (IsInSmm (NULL)) {
    SelectedHeciGuid = &gHeciTransportSmmProtocolGuid;
  } else {
    SelectedHeciGuid = &gHeciTransportProtocolGuid;
  }
  HeciTransportCounter = 0;
  HeciAccess = (HECI_ACCESS*)GetFirstHeciAccess ();
  while (HeciAccess != NULL) {
    HeciTransport = HeciAllocateMemory (sizeof (HECI_TRANSPORT_PRIVATE));

    if (HeciTransport == NULL) {
      ASSERT (HeciTransport != NULL);
      Status = EFI_OUT_OF_RESOURCES;
      break;
    }
    Status = HeciInitializeNewTransportInstance (
               HeciTransport,
               HeciAccess
               );
    ASSERT_EFI_ERROR (Status);
    if (!EFI_ERROR (Status)) {
      Status = InstallHeciProtocol (
                 SelectedHeciGuid,
                 (VOID*)HeciTransport
                 );
     }
    if (!EFI_ERROR (Status)) {
      // for first element get a root
      if (HeciTransportRoot == NULL) {
        HeciTransportRoot = HeciTransport;
      }
      // Link to next
      HeciTransport->HeciTransportRoot = HeciTransportRoot;
      if (HeciTransportPrevious != NULL) {
        HeciTransportPrevious->HeciTransportNext = HeciTransport;
      }
      HeciTransportPrevious = HeciTransport;
    } else {
      ASSERT_EFI_ERROR (Status);
      HeciFreeMemory (HeciTransport);
    }

    HeciTransportCounter++;
    HeciAccess = HeciAccess->GetNextHeciAccess (HeciAccess, HeciAccess);
  }

  DEBUG ((DEBUG_INFO, HECI_TRANSPORT_DEBUG, mHeciModuleDebugPrefix));
  DEBUG ((
    DEBUG_INFO,
    " HECI Transport Protocol Exit : %r. (installed %d transport(s))\n",
    Status,
    HeciTransportCounter
    ));

  return Status;
}
