/** @file
  Platform type driver to identify different platform.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 1999 - 2016 Intel Corporation. <BR>

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

#include <PiDxe.h>
#include <Library/DebugLib.h>
#include <Library/UefiLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/HobLib.h>
#include <Guid/HobList.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Guid/SetupVariable.h>
#include <Protocol/VariableWrite.h>
// APTIOV_SERVER_OVERRIDE_RC_START : Removing IPMI feature code
//#include <Protocol/IpmiTransportProtocol.h>
//#include <Library/IpmiBaseLib.h>
// APTIOV_SERVER_OVERRIDE_RC_END : Removing IPMI feature code

VOID                                   *mEfiVariableProtocolNotifyReg;
EFI_EVENT                              mEfiVariableProtocolEvent;
// APTIOV_SERVER_OVERRIDE_RC_START : Removing IPMI feature code
//EFI_IPMI_TRANSPORT                     *mIpmiTransport;
// APTIOV_SERVER_OVERRIDE_RC_END : Removing IPMI feature code

#define VarMfgUuid                     L"MfgSystemUuid"

#define EFI_SM_NETFN_INTEL_OEM         0x30
#define EFI_INTEL_OEM_SET_SYSTEM_GUID  0x41

extern EFI_GUID                        gEfiMfgUUIDHobGuid;

/**

  Get the system UUID from the UuidInfo Hob and send it to the BMC.

  @retval EFI_UNSUPPORTED       - Platform Info Protocol was not found.  UUID not sent to BMC
  @retval EFI_SUCCESS           - UUID was sent successfully to the BMC.

**/
// APTIOV_SERVER_OVERRIDE_RC_START : Removing IPMI feature code
#if 0
EFI_STATUS
SendUuidToBmc (
  VOID
  )
{

  EFI_STATUS              Status = EFI_SUCCESS;
  UINT8                   *SystemUuid=NULL;
  EFI_HOB_GUID_TYPE       *GuidHob;
  UINT8                   ResponseData;
  UINT8                   ResponseSize;

  GuidHob       = GetFirstGuidHob (&gEfiMfgUUIDHobGuid);
  ASSERT (GuidHob != NULL);
  if (GuidHob == NULL) {
    return EFI_NOT_FOUND;
  }
  SystemUuid  = GET_GUID_HOB_DATA (GuidHob);

  Status = gBS->LocateProtocol (
             &gEfiIpmiTransportProtocolGuid,
             NULL,
             &mIpmiTransport
           );

  if ((Status == EFI_SUCCESS) && (mIpmiTransport != NULL)) {
    ResponseSize  = sizeof (ResponseData);
    Status = mIpmiTransport->SendIpmiCommand(
               mIpmiTransport,
               EFI_SM_NETFN_INTEL_OEM,
               0,
               EFI_INTEL_OEM_SET_SYSTEM_GUID,
               SystemUuid,
               16,
               (UINT8*) (&ResponseData),
               (UINT8*) &ResponseSize
             );

    if (Status != EFI_SUCCESS) {
      DEBUG((DEBUG_ERROR, "[IPMI] Set System GUID (Intel OEM Command 0x41) to BMC Failed.\n"));
    }
  }
  return Status;
}
#endif
// APTIOV_SERVER_OVERRIDE_RC_END : Removing IPMI feature code

VOID
EFIAPI
MFGUuidVariableDelCallback (
  IN  EFI_EVENT Event,
  IN  VOID      *Context
  )
{
  EFI_STATUS        Status;
  UINT8             Buffer[sizeof (EFI_GUID)];
  UINTN             SizeUUID;

  SizeUUID = sizeof (EFI_GUID);
  Status = gRT->GetVariable (
                  VarMfgUuid,
                  &gEfiSetupVariableGuid,
                  NULL,
                  &SizeUUID,
                  Buffer
                  );

  if(!EFI_ERROR (Status)){
  Status = gRT->SetVariable (
                      VarMfgUuid,
                      &gEfiSetupVariableGuid,
                      EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_NON_VOLATILE,
                      0,
                      NULL
                      );
  }
}

/**

  Entry point for the driver.

  This routine check if MfgSystemUuid exists, if found, delete it to avoid duplicate write operation.

  @param ImageHandle  -  Image Handle.
  @param SystemTable  -  EFI System Table.

  @retval EFI_SUCCESS  -  Function has completed successfully.

**/
EFI_STATUS
EFIAPI
UuidDxeEntry (
  IN EFI_HANDLE                         ImageHandle,
  IN EFI_SYSTEM_TABLE                   *SystemTable
  )
{

  EFI_STATUS                      Status;
  UINT8                           Buffer[sizeof (EFI_GUID)];
  VOID                            *VarArch;
  UINTN                           SizeUUID;

// APTIOV_SERVER_OVERRIDE_RC_START : Removing IPMI feature code
//  Status = SendUuidToBmc(); 
// APTIOV_SERVER_OVERRIDE_RC_END : Removing IPMI feature code

  Status = gBS->LocateProtocol (
                   &gEfiVariableWriteArchProtocolGuid,
                   NULL,
                   &VarArch
                   );
  if ( !EFI_ERROR (Status) ){
    SizeUUID = sizeof (EFI_GUID);
    Status = gRT->GetVariable (
                    VarMfgUuid,
                    &gEfiSetupVariableGuid,
                    NULL,
                    &SizeUUID,
                    Buffer
                    );

    if(!EFI_ERROR (Status)){
     Status = gRT->SetVariable (
                        VarMfgUuid,
                        &gEfiSetupVariableGuid,
                        EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS | EFI_VARIABLE_NON_VOLATILE,
                        0,
                        NULL
                        );
     }
  }else{

  //
  // Register to be notified when the variable write protocol has been produced.
  // Check for UUID in NV area.  If found or error, then delete it.
  //
    Status = gBS->CreateEvent (
                   EVT_NOTIFY_SIGNAL,
                   TPL_CALLBACK,
                   MFGUuidVariableDelCallback,
                   NULL,
                   &mEfiVariableProtocolEvent
                   );
    ASSERT_EFI_ERROR (Status);

    Status = gBS->RegisterProtocolNotify (
                  &gEfiVariableWriteArchProtocolGuid,
                  mEfiVariableProtocolEvent,
                  &mEfiVariableProtocolNotifyReg
                  );
    ASSERT_EFI_ERROR (Status);
  }

  return EFI_SUCCESS;
}

