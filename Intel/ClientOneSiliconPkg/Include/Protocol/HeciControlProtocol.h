/** @file
  ME HECI Control Protocol - allows ME-UEFI FW communication over HECI in DXE and SMM.

 @copyright
  INTEL CONFIDENTIAL
  Copyright (c) 2019 Intel Corporation.

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

#ifndef _HECI_CONTROL_PROTOCOL_H_
#define _HECI_CONTROL_PROTOCOL_H_

///
/// Default number of retries to be used HeciControl API calls
///
#define HECI_DEFAULT_RETRY_NUMBER              3        ///< Retries number

/**
  Default HECI Control PCH ID,
  leave the decision to HeciControl on how to send the message
**/
#define HECI_DEFAULT_PCH_ID                    0xDEFA001D

/**
  ME HECI Protocol GUID
  This protocol provides an interface to communicate with Intel ME
  over HECI function on PCH ME Device.
**/
extern EFI_GUID gHeciControlProtocolGuid;
extern EFI_GUID gHeciControlSmmProtocolGuid;


typedef struct _HECI_CONTROL_ HECI_CONTROL;


/**
  Reset HECI queue.

  This function triggers HECI queue reset. If timeout is greater than zero it
  also waits until ME is ready to receive messages.
  When HeciControl is provided last used HECI transport will be reset
  When no HeciControl is not valid, reset target is derived from handle

  @param[in]     This          HeciControl instance
  @param[in]     HeciPchId     PchId we want to communicate with 0..N
                               HECI_DEFAULT_PCH_ID Id is default and
                               HeciControl decides where to route communication
  @param[in,out] Timeout       On input timeout in us, on exit time left

  @retval EFI_INVALID_PARAMETER Input parameters are invalid
  @retval EFI_WRITE_PROTECTED   Can't find suitable HECI Transport
  @retval EFI_DEVICE_ERROR      HECI MBar can't be initialized
  @retval EFI_TIMEOUT           Timeout occurs during operation
  @retval EFI_SUCCESS           Reset HECI queue succeed
**/
typedef EFI_STATUS (EFIAPI *HECI_CONTROL_RESET) (
  IN     HECI_CONTROL        *This,
  IN     UINT32              HeciPchId,
  IN OUT UINT32              *Timeout
  );

/**
  Send request message to HECI queue, wait for response if needed.

  This function writes one message to HECI queue and - if receive buffer
  was provided and timeout is greater than zero - waits for response message.
  The request can be retried if Retries > 0 (0 means one attempt, no retry),
  but if retries are requested, the pReqMsg and pRspMsg pointers must point
  to different buffers. This is required to make sure request is unchanged.
  Size of receive message buffer is given in bytes in (*RspLen) on input.
  On exit (*RspLen) provides the number of bytes written to the message
  buffer. If buffer is too short the message is truncated.

  @param[in]     This          HeciControl instance
  @param[in]     HeciPchId     PchId we want to communicate with 0..N
                               HECI_DEFAULT_PCH_ID Id is default and
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
typedef EFI_STATUS (EFIAPI *HECI_CONTROL_SEND_AND_RECEIVE) (
  IN     HECI_CONTROL        *This,
  IN     UINT32              HeciPchId,
  IN OUT UINT32              *Timeout,
  IN     UINT8               Retries,
  IN     UINT8               HostAddress,
  IN     UINT8               EngineAddress,
  IN     UINT32              *ReqMsg,
  IN     UINT32              ReqLen,
     OUT UINT32              *RspMsg,
  IN OUT UINT32              *RspLen,
  IN OUT EFI_STATUS          *SendStatus,
  IN OUT EFI_STATUS          *ReceiveStatus
  );

/**
  Write a message to HECI queue, if necessary perform fragmentation.

  This function puts one message to HECI queue. If the message is longer than
  HECI queue size and timeout is greater than zero the function will split
  the message into several packets and wait for space in the queue to send all
  packets/fragments. Each packet has HECI packet header added.

  @param[in,out] This          HeciControl instance
  @param[in]     HeciPchId     PchId we want to communicate with 0..N
                               HECI_DEFAULT_PCH_ID Id is default and
                               HeciControl decides where to route communication
  @param[in,out] Timeout       On input timeout in us, on exit time left
  @param[in]     HostAddress   Host address
  @param[in]     EngineAddress Engine (ME/IE) address
  @param[in]     MsgBody       Body of the message to send
  @param[in]     MsgLen        Length in bytes of the message to send

  @retval EFI_INVALID_PARAMETER Input parameters are invalid
  @retval EFI_WRITE_PROTECTED   Can't find suitable HECI Transport
                                Possible reason: HECI filter active
  @retval EFI_DEVICE_ERROR      HECI MBar can't be initialized
  @retval EFI_TIMEOUT           Timeout occurs during operation
  @retval EFI_BAD_BUFFER_SIZE   Packet is bigger than queue length
  @retval EFI_SUCCESS           Message has been sent successfully
**/
typedef EFI_STATUS (EFIAPI *HECI_CONTROL_SEND) (
  IN OUT HECI_CONTROL        *This,
  IN     UINT32              HeciPchId,
  IN OUT UINT32              *Timeout,
  IN     UINT8               HostAddress,
  IN     UINT8               EngineAddress,
  IN     UINT32              *MsgBody,
  IN     UINT32              MsgLen
  );

/**
  Read a message from HECI queue, if necessary perform reassembly.

  This function reads one message from HECI queue. If the message is
  fragmented and timeout is greater than zero the function will wait for
  subsequent packets (fragments) until full message is completed.

  @param[in,out] This          HeciControl instance
  @param[in]     HeciPchId     PchId we want to communicate with 0..N
                               HECI_DEFAULT_PCH_ID Id is default and
                               HeciControl decides where to route communication
  @param[in,out] Timeout       On input timeout in us, on exit time left
  @param[out]    HostAddress   Host address
  @param[out]    EngineAddress Engine (ME/IE) address
  @param[out]    MsgBody       Body of the message to send
  @param[in,out] MsgLen        On input buffer size, on exit message body size in bytes

  @retval EFI_INVALID_PARAMETER Input parameters are invalid
  @retval EFI_WRITE_PROTECTED   Can't find suitable HECI Transport
  @retval EFI_DEVICE_ERROR      HECI MBar can't be initialized
  @retval EFI_TIMEOUT           Timeout occurs during operation
  @retval EFI_BAD_BUFFER_SIZE   Message exceeds buffer size
  @retval EFI_SUCCESS           Message has been received successfully
**/
typedef EFI_STATUS (EFIAPI *HECI_CONTROL_RECEIVE) (
  IN OUT HECI_CONTROL        *This,
  IN     UINT32              HeciPchId,
  IN OUT UINT32              *Timeout,
     OUT UINT8               *HostAddress,
     OUT UINT8               *EngineAddress,
     OUT UINT32              *MsgBody,
  IN OUT UINT32              *MsgLen
  );

/**
  Read HECI Firmware status

  This function reads HECI firmware status from given PCH and HECI interface

  @param[in]     This           HeciControl instance
  @param[in]     HeciNumber     Heci number to firmware status read [1..4]
  @param[in]     FilterFunction Heci filtering function to be installed
  @param[in,out] FirmwareStatus Pointer to returned firmware status

  @retval EFI_SUCCESS           Returned valid firmware status
  @retval EFI_INVALID_PARAMETER Input parameters are invalid
  @retval EFI_DEVICE_ERROR      Error occurred
**/
typedef EFI_STATUS (EFIAPI *HECI_CONTROL_READ_FIRMWARE_STATUS) (
  IN     HECI_CONTROL        *This,
  IN     UINT32              HeciPchId,
  IN     UINT8               HeciNumber,
  IN OUT UINT32              *FirmwareStatus
  );

/**
  EFI protocol for ME-UEFI FW communication over HECI.

  This protocol defines operations for communication with ME using HECI
  (Host Embedded Controller Interface). There are more HECI functions in
  ME device on PCI. HECI is designated for ME-UEFI FW boot flow communication.
**/
typedef struct _HECI_CONTROL_ {
  HECI_CONTROL_SEND_AND_RECEIVE     HeciSendAndReceive;     /// < Send HECI message and receive Ack
  HECI_CONTROL_SEND                 HeciSend;               /// < Send HECI message
  HECI_CONTROL_RECEIVE              HeciReceive;            /// < Receive HECI message
  HECI_CONTROL_RESET                HeciReset;              /// < Reset HECI interface
  HECI_CONTROL_READ_FIRMWARE_STATUS HeciFirmwareStatus;     /// < Reads HECI Firmware Status
} HECI_CONTROL;

#endif // _HECI_CONTROL_PROTOCOL_H_

