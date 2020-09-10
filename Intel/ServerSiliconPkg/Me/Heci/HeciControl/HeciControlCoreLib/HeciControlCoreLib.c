/** @file
  HECI Control driver core operations for use in PEI, DXE, and SMM.

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
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Protocol/HeciTransportProtocol.h>
#include <Protocol/HeciControlProtocol.h>
#include <Library/HeciControlCoreLib.h>
#include <Library/HeciControlFilterLib.h>
#include <Library/HeciCommonLib.h>
#include <Library/MeEwlLib.h>
#include <Library/MeUefiFwHealthStatusLib.h>
#include <Library/PchMultiPchBase.h>
#include <Library/MeTypeLib.h>  // For SPS Ignition only without messaging
#include <Library/MeUtilsLib.h>
#include <Register/HeciRegs.h>
#include <Register/MeRegs.h>
#include <MeState.h>

#define HECI_CONTROL_DEBUG      "[HECI Control-%d %s]"

/**
  Local variables prototypes.
**/
extern CHAR16  *mHeciModuleDebugPrefix;

/**
  Returns PCH ID to use

  @param[in]     HeciPchId       PchId we want to communicate with 0..N
                                 HECI_DEFAULT_PCH_ID Id is default

  @retval PCH ID to be used
**/
UINT32
GetPrimaryPchId (
  UINT32  HeciPchId
  )
{
  if (HeciPchId == HECI_DEFAULT_PCH_ID) {
    HeciPchId = HECI_DEFAULT_COMMUNICATION_PCH_ID_RESOLVED;
  }

  return HeciPchId;
}

/**
  Returns HECI_TRANSPORT pointer based on HECI PCH ID

  @param[in,out] This            HeciControl instance
  @param[in]     HeciPchId       PchId we want to communicate with 0..N
                                 HECI_DEFAULT_PCH_ID Id is default
  @param[in]     HeciNumber      HECI number we want to use for communication

  @retval HECI_TRANSPORT*        On success HECI_TRANSPORT pointer is returned
  @retval NULL                   failure is indicated by NULL
**/
HECI_TRANSPORT*
GetHeciTransportByHeciParam (
  IN OUT HECI_CONTROL_PRIVATE *This,
  IN     UINT32               HeciPchId,
  IN     UINTN                HeciNumber
  )
{
  HECI_TRANSPORT   *HeciTransport;
  HECI_DEVICE_INFO HeciDeviceInfo;

  HeciPchId = GetPrimaryPchId (HeciPchId);

  HeciTransport = GetFirstHeciTransport ();
  while (HeciTransport) {
    HeciTransport->GetHeciDeviceInfo (HeciTransport, &HeciDeviceInfo);
    if ((HeciDeviceInfo.HeciNo == HeciNumber) &&
        (HeciDeviceInfo.HeciPch == HeciPchId)) {
          // Found HECI_TRANSPORT for given PCH and HECI
          break;
        }
    HeciTransport = HeciTransport->GetNextHeciTransport (HeciTransport, HeciTransport);
  }

  return HeciTransport;
}

/**
  Read HECI Firmware status

  This function reads HECI firmware status from given PCH and HECI interface

  @param[in]     This           HeciControl instance
  @param[in]     HeciPchId      PchId we want to communicate with 0..N
                                HECI_DEFAULT_PCH_ID Id is default
  @param[in]     HeciNumber     Heci number to firmware status read [1..4]
  @param[in,out] FirmwareStatus Pointer to returned firmware status

  @retval EFI_SUCCESS           Returned valid firmware status
  @retval EFI_INVALID_PARAMETER Input parameters are invalid
  @retval EFI_DEVICE_ERROR      Error occurred
**/
EFI_STATUS
HeciFirmwareStatus (
  IN OUT HECI_CONTROL_PRIVATE *This,
  IN     UINT32               HeciPchId,
  IN     UINT8                HeciNumber,
  IN OUT UINT32               *FirmwareStatus
  )
{
  EFI_STATUS        ReturnStatus;
  HECI_TRANSPORT    *HeciTransport;

  if ((This == NULL) || (FirmwareStatus == NULL)) {
    ASSERT (This != NULL);
    ASSERT (FirmwareStatus != NULL);
    return EFI_INVALID_PARAMETER;
  }
  *FirmwareStatus = MAX_UINT32;
  HeciTransport = GetHeciTransportByHeciParam (
                    This,
                    HeciPchId,
                    HeciNumber
                    );
  if (HeciTransport != NULL) {
    ReturnStatus = HeciTransport->ReadHeciFirmwareStatus (HeciTransport, FirmwareStatus);
  } else {
    ReturnStatus = EFI_DEVICE_ERROR;
  }

  return ReturnStatus;
}

/**
  Returns HECI_TRANSPORT pointer based on HECI PCH ID

  @param[in,out] This            HeciControl instance
  @param[in]     HeciPchId       PchId we want to communicate with 0..N
                                 HECI_DEFAULT_PCH_ID Id is default
  @param[in]     MessageIsValid  Message data provided to function is valid
  @param[in]     HostAddress     Host address
  @param[in]     EngineAddress   Engined (ME/IE) address
  @param[in]     MessageBody     Body of the message to send
  @param[in]     MessageLength   Length of the message to send in bytes

  @retval HECI_TRANSPORT*        On success HECI_TRANSPORT pointer is returned
  @retval NULL                   failure is indicated by NULL
**/
HECI_TRANSPORT*
GetHeciTransportForPchId (
  IN OUT HECI_CONTROL_PRIVATE *This,
  IN     UINT32               HeciPchId,
  IN     BOOLEAN              MessageIsValid,
  IN     UINT8                HostAddress,
  IN     UINT8                EngineAddress,
  IN     UINT32               *MessageBody,
  IN     UINT32               MessageLength
  )
{
  HECI_TRANSPORT *HeciTransport;
  UINTN          HeciFn;
  UINTN          HeciNo;
  EFI_STATUS     Status;

  HeciPchId = GetPrimaryPchId (HeciPchId);
  HeciFn = HECI_DEFAULT_COMMUNICATION_FUNCTION;
  HeciTransport = NULL;
  if (MessageIsValid) {
    // Update HECI function information according message
    Status = FilterHeciMessage (
                     HeciPchId,
                     &HeciFn,
                     HostAddress,
                     EngineAddress,
                     MessageBody,
                     MessageLength
                     );
  } else {
    Status = EFI_SUCCESS;
  }
  if (!EFI_ERROR (Status)) {
    Status = GetHeciNumber (HeciFn, &HeciNo);
    ASSERT_EFI_ERROR (Status);
    if (!EFI_ERROR (Status)) {
      HeciTransport = GetHeciTransportByHeciParam (This, HeciPchId, HeciNo);
    }
  }

  return HeciTransport;
}

/**
  Verify if ME supports message exchange based on ME Firmware Status.

  @param[in]     This           HeciControl instance
  @param[in]     HeciTransport  HECI device handle identifies device function will work on

  @retval TRUE           Communication is allowed
  @retval FALSE          Communication is disallowed
**/
BOOLEAN
EFIAPI
HeciVerifyMeAvailability (
  IN     HECI_CONTROL_PRIVATE  *This,
  IN     HECI_TRANSPORT        *HeciTransport
  )
{
  BOOLEAN                ReturnHeciAvailability;
  EFI_STATUS             Status;
  HECI_FWS_REGISTER      MeFirmwareStatus;
  HECI_DEVICE_INFO       HeciDeviceInfo;

  ReturnHeciAvailability = FALSE;
  if (HeciTransport != NULL) {
    Status = MeBiosGetMeStatusRaw (&MeFirmwareStatus.ul);
    if (EFI_ERROR (Status)) {
      ASSERT_EFI_ERROR (Status);
      HeciTransport->GetHeciDeviceInfo (HeciTransport, &HeciDeviceInfo);
    } else if ((!MeTypeIsClient () && MeFirmwareStatus.r.CurrentState == ME_STATE_DISABLED) ||
        (MeFirmwareStatus.r.MeOperationMode == ME_OPERATION_MODE_DEBUG) ||
        (MeTypeIsSpsIgnition ()))  {
      HeciTransport->GetHeciDeviceInfo (HeciTransport, &HeciDeviceInfo);
    } else {
      ReturnHeciAvailability = TRUE;
    }
  }

  return ReturnHeciAvailability;
}

/**
  Initialize HECI interface.

  This function initializes host side of HECI interface. If timeout is
  greater than zero it also waits until ME is ready to receive messages.

  @param[in]     This           HeciControl instance
  @param[in]     HeciTransport  HECI device handle identifies device function will work on

  @retval EFI_INVALID_PARAMETER Input parameters are invalid
  @retval EFI_DEVICE_ERROR      Device not present or HECI MBar can't be initialized
  @retval EFI_SUCCESS           Initialization succeed
**/
EFI_STATUS
EFIAPI
HeciControlInit (
  IN OUT HECI_CONTROL_PRIVATE *This,
  IN     HECI_TRANSPORT       *HeciTransport
  )
{
  UINT32           DefaultTimeout;
  EFI_STATUS       Status;

  Status = EFI_UNSUPPORTED;
  if (HeciTransport != NULL) {
    if (HeciVerifyMeAvailability (This, HeciTransport)) {
      if (FilterHeciInitialization (HeciTransport)) {
        DefaultTimeout = HECI_INIT_TIMEOUT;
        Status = HeciTransport->HeciInit (
                                  HeciTransport,
                                  &DefaultTimeout
                                  );
      }
    }
  }

  return Status;
}

/**
  Initialize defined HECI communication interface (excluding ICH interface).

  @param[in]     This           HeciControl instance
  @param[in]     HeciTransport  HeciTransport instance

  @retval EFI_SUCCESS           Initialization succeed
  @retval EFI_INVALID_PARAMETER At least one of input parameter is NULL
**/
EFI_STATUS
EFIAPI
HeciControlInitalizeCommunicationDevice (
  IN      HECI_CONTROL_PRIVATE *This,
  IN      HECI_TRANSPORT       *HeciTransport
  )
{
  HECI_DEVICE_INFO                    HeciDeviceInfo;
  ME_UEFI_FW_HEALTH_STATUS_FIELD_ID   HealthDeviceId;
  EFI_STATUS                          Status;

  if (This == NULL) {
    ASSERT (This != NULL);
    return EFI_INVALID_PARAMETER;
  }

  if (HeciTransport == NULL) {
    ASSERT (HeciTransport != NULL);
    return EFI_INVALID_PARAMETER;
  }

  HeciTransport->GetHeciDeviceInfo (HeciTransport, &HeciDeviceInfo);
  if (!HeciTransport->GetInitializationState (HeciTransport)) {
    Status = HeciControlInit (This, HeciTransport);
    if (!EFI_ERROR (Status)) {
      DEBUG ((DEBUG_INFO, HECI_CONTROL_DEBUG, HeciDeviceInfo.HeciNo, mHeciModuleDebugPrefix));
    } else {
      MeEwlStateFailures (
        MeEwlSeverityFatal,
        ME_EWL_CHKP_MAJOR_PREMEM,
        ME_EWL_CHKP_PREMEM_HECI,
        ME_EWL_WARN_MAJOR_HECI,
        ME_EWL_WARN_HECI_PCI
        );
    }
    if (HeciDeviceInfo.HeciPch == PCH_LEGACY_ID) {
      if (sizeof (UINTN) == sizeof (UINT32)) {
        HealthDeviceId = Heci1PpiInstalledStatus;
      } else {
        if (IsInSmm (NULL)) {
          HealthDeviceId = Heci1SmmInstalledStatus;
        } else {
          HealthDeviceId = Heci1DxeInstalledStatus;
        }
      }
      if ((HeciDeviceInfo.HeciNo > 4) || (HeciDeviceInfo.HeciNo < 1)) {
        ASSERT (FALSE);
      } else {
        HealthDeviceId += HeciDeviceInfo.HeciNo - 1;
        SetMeUefiFwHealthEfiStatus (HealthDeviceId, Status);
      }
    }
  } else {
  }

  return EFI_SUCCESS;
}

/**
  Initialize all defined HECI communication interfaces (excluding ICH interfaces).

  @param[in]     This           HeciControl instance
**/
VOID
HeciControlInitalizeAllCommunicationDevices (
  IN      HECI_CONTROL_PRIVATE *This
  )
{
  HECI_TRANSPORT                      *HeciTransport;
  EFI_STATUS                          Status;

  DEBUG ((
    DEBUG_INFO,
    "[HECI Control] Initialize HECI Interfaces\n"
    ));

  HeciTransport = GetFirstHeciTransport ();
  while (HeciTransport != NULL) {
    Status = HeciControlInitalizeCommunicationDevice (This, HeciTransport);
      if (EFI_ERROR (Status)) {
      break;
    }
    HeciTransport = HeciTransport->GetNextHeciTransport (HeciTransport, HeciTransport);
  }
}

/**
  Reset HECI queue.

  This function triggers HECI queue reset. If timeout is greater than zero it
  also waits until ME is ready to receive messages.
  When HeciControl is provided last used HECI transport will be reset
  When no HeciControl is not valid, reset target is derived from handle

  @param[in,out] This          HeciControl instance
  @param[in]     HeciPchId     PchId we want to communicate with 0..N
                               HECI_DEFAULT_PCH_ID Id is default
                               HeciControl decides where to route communication
  @param[in,out] Timeout       On input timeout in us, on exit time left

  @retval EFI_INVALID_PARAMETER Input parameters are invalid
  @retval EFI_WRITE_PROTECTED   Can't find suitable HECI Transport
  @retval EFI_DEVICE_ERROR      HECI MBar can't be initialized
  @retval EFI_TIMEOUT           Timeout occurs during operation
  @retval EFI_SUCCESS           Reset HECI queue succeed
**/
EFI_STATUS
EFIAPI
HeciControlReset (
  IN OUT HECI_CONTROL_PRIVATE *This,
  IN     UINT32               HeciPchId,
  IN OUT UINT32               *Timeout
  )
{
  HECI_TRANSPORT     *HeciTransport;
  EFI_STATUS         Status;
  HECI_DEVICE_INFO   HeciDeviceInfo;

  if (This == NULL) {
    ASSERT (This != NULL);
    return EFI_INVALID_PARAMETER;
  }

  Status = EFI_UNSUPPORTED;
  if (This->LastUsedHeciTransport != NULL) {
    // Use last transport handle
    HeciTransport = This->LastUsedHeciTransport;
  } else {
    // When Last transport is not available,
    // get transport to reset from heci handle
    HeciTransport = GetHeciTransportForPchId (
                      This,
                      HeciPchId,
                      FALSE,
                      0,
                      0,
                      NULL,
                      0
                      );
  }
  if (HeciTransport == NULL) {
    Status = EFI_WRITE_PROTECTED;
  } else if (HeciVerifyMeAvailability (This, HeciTransport)) {
    if (HeciTransport->GetInitializationState (HeciTransport)) {
      Status = HeciTransport->HeciReset (
                                HeciTransport,
                                Timeout
                                );
    } else {
      HeciTransport->GetHeciDeviceInfo (HeciTransport, &HeciDeviceInfo);
      DEBUG ((DEBUG_ERROR, HECI_CONTROL_DEBUG, HeciDeviceInfo.HeciNo, mHeciModuleDebugPrefix));
      DEBUG ((
        DEBUG_ERROR,
        ": ERROR: %a() : Heci %d is not initialized\n",
        __FUNCTION__,
        HeciDeviceInfo.HeciNo
        ));
      Status = EFI_DEVICE_ERROR;
    }
  }

  return Status;
}

/**
  Write a message to HECI queue, if necessary perform fragmentation.

  This function puts one message to HECI queue. If the message is longer than
  HECI queue size and timeout is greater than zero the function will split
  the message into several packets and wait for space in the queue to send all
  packets/fragments. Each packet has HECI packet header added.

  For send from SMM check if HECI MBARs has been not changed.
  In such case use configured by UEFI FW and restore after transmission
  On the end wait for send completion on ME side
  and reset HECI Q in addition to clean data.

  @param[in,out]   This            HeciControl instance
  @param[in]       HeciPchId       PchId we want to communicate with 0..N
                                   HECI_DEFAULT_PCH_ID Id is default
                                   HeciControl decides where to route communication
  @param[in,out]   Timeout         On input timeout in us, on exit time left
  @param[in]       HostAddress     Host address
  @param[in]       EngineAddress   Engined (ME/IE) address
  @param[in]       MessageBody     Body of the message to send
  @param[in]       MessageLength   Length of the message to send in bytes

  @retval EFI_INVALID_PARAMETER Input parameters are invalid
  @retval EFI_WRITE_PROTECTED   Can't find suitable HECI Transport
                                Possible reason: HECI filter active
  @retval EFI_DEVICE_ERROR      HECI MBar can't be initialized
  @retval EFI_TIMEOUT           Timeout occurs during operation
  @retval EFI_BAD_BUFFER_SIZE   Packet is bigger than queue length
  @retval EFI_SUCCESS           Message has been sent successfully
**/
EFI_STATUS
EFIAPI
HeciControlSend (
  IN OUT HECI_CONTROL_PRIVATE *This,
  IN     UINT32               HeciPchId,
  IN OUT UINT32               *Timeout,
  IN     UINT8                HostAddress,
  IN     UINT8                EngineAddress,
  IN     UINT32               *MessageBody,
  IN     UINT32               MessageLength
  )
{
  HECI_TRANSPORT    *HeciTransport;
  UINT32            DefaultTimeout;
  EFI_STATUS        Status;
  HECI_DEVICE_INFO  HeciDeviceInfo;

  if (This == NULL) {
    ASSERT (This != NULL);
    return EFI_INVALID_PARAMETER;
  }

  HeciTransport = GetHeciTransportForPchId (
                    This,
                    HeciPchId,
                    TRUE,
                    HostAddress,
                    EngineAddress,
                    MessageBody,
                    MessageLength
                    );
  if (HeciTransport == NULL) {
    Status = EFI_WRITE_PROTECTED;
  } else if (HeciTransport->GetInitializationState (HeciTransport)) {
    if (Timeout == NULL) {
      DefaultTimeout = HECI_SEND_TIMEOUT;
      Timeout = &DefaultTimeout;
    }

    Status = HeciTransport->HeciSend (
                              HeciTransport,
                              Timeout,
                              HostAddress,
                              EngineAddress,
                              MessageBody,
                              MessageLength
                              );

    This->LastUsedHeciTransport = HeciTransport;
  } else {
    HeciTransport->GetHeciDeviceInfo (HeciTransport, &HeciDeviceInfo);
    DEBUG ((DEBUG_ERROR, HECI_CONTROL_DEBUG, HeciDeviceInfo.HeciNo, mHeciModuleDebugPrefix));
    DEBUG ((
      DEBUG_ERROR,
      ": ERROR: %a() : Heci %d is not initialized\n",
      __FUNCTION__,
      HeciDeviceInfo.HeciNo
      ));
    Status = EFI_DEVICE_ERROR;
  }

  return Status;
}

/**
  Read a message from HECI queue, if necessary perform reassembly.

  This function reads one message from HECI queue. If the message is
  fragmented and timeout is greater than zero the function will wait for
  subsequent packets (fragments) until full message is completed.

  For send from SMM check if HECI MBARs has been not changed.
  In such case use configured by UEFI FW and restore after transmission
  On the end reset HECI Q in addition to clean data.

  @param[in,out] This           HeciControl instance
  @param[in]     HeciPchId      PchId we want to communicate with 0..N
                                HECI_DEFAULT_PCH_ID Id is default
                                HeciControl decides where to route communication
  @param[in,out] Timeout        On input timeout in us, on exit time left
  @param[out]    HostAddress    Host address
  @param[out]    EngineAddress  Engine (ME/IE) address
  @param[out]    MessageBody    Body of the message to send
  @param[in,out] MessageLength  On input buffer size, on exit message body size in bytes

  @retval EFI_INVALID_PARAMETER Input parameters are invalid
  @retval EFI_WRITE_PROTECTED   Can't find suitable HECI Transport
  @retval EFI_DEVICE_ERROR      HECI MBar can't be initialized
  @retval EFI_TIMEOUT           Timeout occurs during operation
  @retval EFI_BAD_BUFFER_SIZE   Message exceeds buffer size
  @retval EFI_SUCCESS           Message has been received successfully
**/
EFI_STATUS
EFIAPI
HeciControlReveive (
  IN OUT HECI_CONTROL_PRIVATE *This,
  IN     UINT32               HeciPchId,
  IN OUT UINT32               *Timeout,
     OUT UINT8                *HostAddress,
     OUT UINT8                *EngineAddress,
     OUT UINT32               *MessageBody,
  IN OUT UINT32               *MessageLength
  )
{
  HECI_TRANSPORT    *HeciTransport;
  UINT32            DefaultTimeout;
  EFI_STATUS        Status;
  HECI_DEVICE_INFO  HeciDeviceInfo;

  if (This == NULL) {
    ASSERT (This != NULL);
    return EFI_INVALID_PARAMETER;
  }

  if (This->LastUsedHeciTransport != NULL) {
    // Use last transport handle
    HeciTransport = This->LastUsedHeciTransport;
  } else {
    // When Last transport is not available,
    // get transport to receive
    HeciTransport = GetHeciTransportForPchId (
                      This,
                      HeciPchId,
                      FALSE,
                      0,
                      0,
                      NULL,
                      0
                      );
  }
  Status = EFI_UNSUPPORTED;
  if (HeciTransport == NULL) {
    Status = EFI_WRITE_PROTECTED;
  } else if (HeciTransport->GetInitializationState (HeciTransport)) {
    if (Timeout == NULL) {
      DefaultTimeout = HECI_READ_TIMEOUT;
      Timeout = &DefaultTimeout;
    }

    Status = HeciTransport->HeciReceive (
                              HeciTransport,
                              Timeout,
                              HostAddress,
                              EngineAddress,
                              MessageBody,
                              MessageLength
                              );

    This->LastUsedHeciTransport = HeciTransport;
  } else {
    HeciTransport->GetHeciDeviceInfo (HeciTransport, &HeciDeviceInfo);
    DEBUG ((DEBUG_ERROR, HECI_CONTROL_DEBUG, HeciDeviceInfo.HeciNo, mHeciModuleDebugPrefix));
    DEBUG ((
      DEBUG_ERROR,
      ": ERROR: %a() : Heci %d is not initialized\n",
      __FUNCTION__,
      HeciDeviceInfo.HeciNo
      ));
    Status = EFI_DEVICE_ERROR;
  }

  return Status;
}

/**
  Send request message to HECI queue, wait for response if needed.

  This function writes one message to HECI queue and - if receive buffer
  was provided and timeout is greater than zero - waits for response message.
  The request can be retried if Retries > 0 (0 means one attempt, no retry),
  but if retries are requested, the ReqMsg and RspMsg pointers must point
  to different buffers. This is required to make sure request is unchanged.
  Size of receive message buffer is given in bytes in (*RspLen) on input.
  On exit (*RspLen) provides the number of bytes written to the message
  buffer. If buffer is too short the message is truncated.

  For send from SMM check if HECI MBARs has been not changed.
  In such case use configured by UEFI FW and restore after transmission
  On the end reset HECI Q in addition to clean data.

  @param[in,out] This          HeciControl instance
  @param[in]     HeciPchId     PchId we want to communicate with 0..N
                               HECI_DEFAULT_PCH_ID Id is default
                               HeciControl decides where to route communication
  @param[in,out] Timeout       On input timeout in us, on exit time left
  @param[in]     Retries       The number of retries requested (0 => no retry)
  @param[in]     HostAddress   Host address
  @param[in]     EngineAddress Engine (ME/IE) address
  @param[in]     ReqMsg        Request message
  @param[in]     ReqLen        Request message length in bytes
  @param[out]    RspMsg        Buffer for the response message
  @param[in,out] RspLen        On input buffer size, on exit message, in bytes
  @param[in,out] SendStatus    Send operation status
  @param[in,out] ReceiveStatus Receive operation status

  @retval EFI_INVALID_PARAMETER Input parameters are invalid
  @retval EFI_WRITE_PROTECTED   Can't find suitable HECI Transport
                                Possible reason: HECI filter active
  @retval EFI_DEVICE_ERROR      HECI MBar can't be initialized
  @retval EFI_TIMEOUT           Timeout occurs during operation
  @retval EFI_BAD_BUFFER_SIZE   Message is too big
  @retval EFI_SUCCESS           Message request has been completed successfully
**/
EFI_STATUS
EFIAPI
HeciControlSendAndReceive (
  IN OUT HECI_CONTROL_PRIVATE *This,
  IN     UINT32               HeciPchId,
  IN OUT UINT32               *Timeout,
  IN     UINT8                Retries,
  IN     UINT8                HostAddress,
  IN     UINT8                EngineAddress,
  IN     UINT32               *ReqMsg,
  IN     UINT32               ReqLen,
     OUT UINT32               *RspMsg,
  IN     UINT32               *RspLen,
  IN OUT EFI_STATUS           *SendStatus,
  IN OUT EFI_STATUS           *ReceiveStatus
  )
{
  HECI_TRANSPORT    *HeciTransport;
  UINT32            DefaultTimeout;
  EFI_STATUS        Status;
  UINT32            *ReqMsgInternal;
  UINT32            AllocationSize;
  HECI_DEVICE_INFO  HeciDeviceInfo;

  if (This == NULL) {
    ASSERT (This != NULL);
    return EFI_INVALID_PARAMETER;
  }

  Status = EFI_UNSUPPORTED;
  HeciTransport = GetHeciTransportForPchId (
                    This,
                    HeciPchId,
                    TRUE,
                    HostAddress,
                    EngineAddress,
                    ReqMsg,
                    ReqLen
                    );
  if (HeciTransport == NULL) {
    Status = EFI_WRITE_PROTECTED;
  } else if (HeciTransport->GetInitializationState (HeciTransport)) {
    if (Timeout == NULL) {
      DefaultTimeout = HECI_SEND_TIMEOUT + HECI_READ_TIMEOUT;
      Timeout = &DefaultTimeout;
    }

    ReqMsgInternal = NULL;
    AllocationSize = 0;

    if ((ReqMsg == RspMsg) && (ReqLen > 0)) {
      AllocationSize = ((ReqLen / sizeof (UINT32)) + 1) * sizeof (UINT32);
      ReqMsgInternal = HeciAllocateMemory (AllocationSize);
      if (ReqMsgInternal != NULL) {
        CopyMem (ReqMsgInternal, ReqMsg, ReqLen);
      } else {
        AllocationSize = 0;
        ReqMsgInternal = ReqMsg;
      }
    } else {
      ReqMsgInternal = ReqMsg;
    }

    Status = HeciTransport->HeciSendAndReceive (
                              HeciTransport,
                              Timeout,
                              Retries,
                              HostAddress,
                              EngineAddress,
                              ReqMsgInternal,
                              ReqLen,
                              RspMsg,
                              RspLen,
                              SendStatus,
                              ReceiveStatus
                              );

    This->LastUsedHeciTransport = HeciTransport;

    if (AllocationSize > 0) {
      // Free local send buffer
      HeciFreeMemory (ReqMsgInternal);
    }
  } else {
    HeciTransport->GetHeciDeviceInfo (HeciTransport, &HeciDeviceInfo);
    DEBUG ((DEBUG_ERROR, HECI_CONTROL_DEBUG, HeciDeviceInfo.HeciNo, mHeciModuleDebugPrefix));
    DEBUG ((
      DEBUG_ERROR,
      ": ERROR: %a() : Heci %d is not initialized\n",
      __FUNCTION__,
      HeciDeviceInfo.HeciNo
      ));
    Status = EFI_DEVICE_ERROR;
  }

  return Status;
}

/**
  Initialize HECI_CONTROL protocol functions

  @param[out]     HeciControl HECI control pointer the be initialized
**/
VOID
SetHeciControlFunctions (
  OUT HECI_CONTROL_PRIVATE *HeciControl
  )
{
  if (HeciControl == NULL) {
    ASSERT (HeciControl != NULL);
    return;
  }

  HeciControl->HeciControl.HeciReset          = (HECI_CONTROL_RESET) HeciControlReset;
  HeciControl->HeciControl.HeciSendAndReceive = (HECI_CONTROL_SEND_AND_RECEIVE) HeciControlSendAndReceive;
  HeciControl->HeciControl.HeciSend           = (HECI_CONTROL_SEND) HeciControlSend;
  HeciControl->HeciControl.HeciReceive        = (HECI_CONTROL_RECEIVE) HeciControlReveive;
  HeciControl->HeciControl.HeciFirmwareStatus = (HECI_CONTROL_READ_FIRMWARE_STATUS) HeciFirmwareStatus;
}

/**
  Initialize HECI_CONTROL protocol instance

  @param[in]     HeciControl HECI control pointer the be initialized

  @retval EFI_INVALID_PARAMETER Input parameters are invalid
  @retval EFI_SUCCESS           Heci control initialized
**/
EFI_STATUS
HeciInitializeNewControlInstance (
  IN OUT HECI_CONTROL_PRIVATE *HeciControl
  )
{
  if (HeciControl == NULL) {
    ASSERT (HeciControl != NULL);
    return EFI_INVALID_PARAMETER;
  }

  SetHeciControlFunctions (HeciControl);

  HeciControl->LastUsedHeciTransport = NULL;
  HeciControl->PeiReinitialize = TRUE;

  return EFI_SUCCESS;
}