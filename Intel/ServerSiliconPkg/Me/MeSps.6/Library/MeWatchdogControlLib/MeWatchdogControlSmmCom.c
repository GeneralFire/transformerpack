/** @file
  Implements ME Watchdog HECI Interface via SMM Library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 Intel Corporation.

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

#include <Base.h>
#include <Uefi.h>
#include <PiDxe.h>
#include <Sps.h>

#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/DxeMeLib.h>
#include <Library/HobLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/MeTypeLib.h>

#include <Protocol/SmmCommunication.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MeWatchdogControlLib.h>



/**
  Initialize the communicate buffer using DataSize and Function.

  The communicate size is: SMM_COMMUNICATE_HEADER_SIZE + DataSize.

  Caution: This function may receive untrusted input.
  The data size external input, so this function will validate it carefully to avoid buffer overflow.

  @param[out]      CommunicateHeader     The communicate header.
  @param[out]      DataPtr               Points to the data in the communicate buffer.
  @param[in]       DataSize              The data size to send to SMM.

  @retval EFI_OUT_OF_RESOURCES           Can't allocated memory.
  @retval EFI_INVALID_PARAMETER          Some parameter is null.
  @retval EFI_SUCCESS                    Find the specified variable.

**/
EFI_STATUS
InitCommunicationBuffer (
  OUT     VOID         **CommunicateHeader,
  OUT     VOID         **DataPtr,
  IN      UINTN        DataSize
  )
{
  EFI_SMM_COMMUNICATE_HEADER  *SmmCommunicateHeader = NULL;

  if (CommunicateHeader == NULL) {
    ASSERT (CommunicateHeader != NULL);
    return EFI_INVALID_PARAMETER;
  }
  if (DataPtr == NULL) {
    ASSERT (DataPtr != NULL);
    return EFI_INVALID_PARAMETER;
  }

  if (DataSize != sizeof (SPS_SMM_WATCHDOG_HEADER)) {
    ASSERT (DataSize == sizeof (SPS_SMM_WATCHDOG_HEADER));
    return EFI_INVALID_PARAMETER;
  }

  SmmCommunicateHeader = (EFI_SMM_COMMUNICATE_HEADER*)AllocateRuntimePool (DataSize + SMM_SPS_WATCHDOG_COMMUNICATE_HEADER_SIZE);
  if (SmmCommunicateHeader == NULL) {
    ASSERT (SmmCommunicateHeader != NULL);
    return EFI_OUT_OF_RESOURCES;
  }

  ZeroMem (SmmCommunicateHeader, DataSize + SMM_SPS_WATCHDOG_COMMUNICATE_HEADER_SIZE);
  CopyGuid (&SmmCommunicateHeader->HeaderGuid, &gSpsSmmWatchdogGuid);
  SmmCommunicateHeader->MessageLength = DataSize + SMM_SPS_WATCHDOG_COMMUNICATE_HEADER_SIZE;

  *CommunicateHeader = SmmCommunicateHeader;
  *DataPtr = SmmCommunicateHeader->Data;

  return EFI_SUCCESS;
}



/**
  Send the data in communicate buffer to SMI handler and get response.

  @param[in]       SmmCommunication      The communication protocol.
  @param[in, out]  SmmCommunicateHeader  The communicate buffer.
  @param[in]       DataSize              The payload size.

  @retval EFI_INVALID_PARAMETER          Some parameter is null.
  @retval EFI_UNSUPPORTED                Current ME mode doesn't support this function
  @retval EFI_SUCCESS                    Sent buffer with success.

**/
EFI_STATUS
SendCommunicationBuffer (
  IN      EFI_SMM_COMMUNICATION_PROTOCOL    *SmmCommunication,
  IN OUT  EFI_SMM_COMMUNICATE_HEADER        *SmmCommunicateHeader,
  IN      UINTN                             DataSize
  )
{
  EFI_STATUS                Status;
  UINTN                     CommSize;
  SPS_SMM_WATCHDOG_HEADER   *SpsSmmWatchdogHeader;

  if (SmmCommunication == NULL) {
    ASSERT (SmmCommunication != NULL);
    return EFI_INVALID_PARAMETER;
  }

  if (SmmCommunicateHeader == NULL) {
    ASSERT (SmmCommunicateHeader != NULL);
    return EFI_INVALID_PARAMETER;
  }

  if (DataSize != sizeof (SPS_SMM_WATCHDOG_HEADER)) {
    ASSERT (DataSize == sizeof (SPS_SMM_WATCHDOG_HEADER));
    return EFI_INVALID_PARAMETER;
  }

  CommSize = DataSize + SMM_SPS_WATCHDOG_COMMUNICATE_HEADER_SIZE;
  SpsSmmWatchdogHeader = (SPS_SMM_WATCHDOG_HEADER*)SmmCommunicateHeader;

  DEBUG ((DEBUG_INFO, "[SPS] %a WatchdogFunction = %d\n", __FUNCTION__, SpsSmmWatchdogHeader->ActionFlag));
  Status = SmmCommunication->Communicate (SmmCommunication, SmmCommunicateHeader, &CommSize);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "[SPS] %a WatchdogFunction returned = %d\n", __FUNCTION__, SpsSmmWatchdogHeader->ReturnStatus));

  return SpsSmmWatchdogHeader->Status;
}


/**
  Initialize the Communicate Buffer and send data to SMM

  @param[in, out]  pHeaderBuffer      The pointer to the header structure

  @retval EFI_OUT_OF_RESOURCES   Can't allocated memory
  @retval EFI_INVALID_PARAMETER  Some parameter is NULL
  @retval EFI_SUCCESS            Notified with success.

**/
EFI_STATUS
MeWatchdogControlNotifySmm (
  IN OUT VOID   *pHeaderBuffer
)
{
  EFI_STATUS                       Status;
  UINTN                            PayloadSize;
  EFI_SMM_COMMUNICATE_HEADER       *SmmCommunicateHeader;
  EFI_SMM_COMMUNICATION_PROTOCOL   *SmmCommunication;
  SPS_SMM_WATCHDOG_HEADER          *SpsSmmWatchdogHeader;

  DEBUG ((DEBUG_INFO, "[SPS] %a call\n", __FUNCTION__));

  SmmCommunicateHeader = NULL;

  if (pHeaderBuffer == NULL) {
    ASSERT (pHeaderBuffer != NULL);
    return EFI_INVALID_PARAMETER;
  }

  PayloadSize = sizeof (SPS_SMM_WATCHDOG_HEADER);

  //
  // Initialize the communicate buffer.
  //
  Status = InitCommunicationBuffer ((VOID**)&SmmCommunicateHeader, (VOID**)&SpsSmmWatchdogHeader, PayloadSize);
  DEBUG ((DEBUG_INFO, "[SPS] %a InitCommunicationBuffer : %r\n", __FUNCTION__, Status));
  if (EFI_ERROR (Status)) {
    FreePool (SmmCommunicateHeader);
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  gBS->CopyMem (SpsSmmWatchdogHeader, pHeaderBuffer, PayloadSize);

  Status = gBS->LocateProtocol (&gEfiSmmCommunicationProtocolGuid, NULL, (VOID**)&SmmCommunication);
  DEBUG ((DEBUG_INFO, "[SPS] %a Locate gEfiSmmCommunicationProtocolGuid : %r\n", __FUNCTION__, Status));
  ASSERT_EFI_ERROR (Status);
  if (SmmCommunication != NULL) {
    //
    // Send data to SMM.
    //
    Status = SendCommunicationBuffer (SmmCommunication, SmmCommunicateHeader, PayloadSize);
    DEBUG ((DEBUG_INFO, "[SPS] %a SendCommunicationBuffer : %r\n", __FUNCTION__, Status));
  }

  gBS->CopyMem (pHeaderBuffer, SpsSmmWatchdogHeader, PayloadSize);
  FreePool (SmmCommunicateHeader);

  DEBUG ((DEBUG_INFO, "[SPS] %a Exit : %r\n", __FUNCTION__, Status));

  return Status;
}
