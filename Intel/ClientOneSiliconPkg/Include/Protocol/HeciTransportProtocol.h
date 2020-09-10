/** @file
  ME HECI Transport Protocol - allows ME-UEFI FW communication over HECI in DXE and SMM.

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

#ifndef _HECI_TRANSPORT_PROTOCOL_H_
#define _HECI_TRANSPORT_PROTOCOL_H_

typedef struct _HECI_TRANSPORT_ HECI_TRANSPORT;

/**
  Maximum possible length of HECI packet not including header.

  Actually it is maximum length of HECI queue that can be configured by ME
  minus packet header size. Single packet must not exceed HECI queue size.
  Actual HECI queue size is configured by ME, so this macro does not define
  the current maximum packet length, but the maximum possible length.
  The current maximum packet length for the to host and from host directions.
**/
#define MAX_CIRCULAR_BUFFER_SIZE  (128 * sizeof (UNIT32))
#define HECI_PKT_MAXLEN           (MAX_CIRCULAR_BUFFER_SIZE - sizeof (HECI_PACKET_HEADER))

/**
  Default HECI communication timeouts for HECI stack
**/
#define HECI_TIMEOUT_TICK             100  ///< Timeout grain in us for the HECI functions
#define HECI_SEND_WAITTIME          10000  ///< 10 ms timeout in microseconds,
#define HECI_RESET_TIMEOUT        2000000  ///< 2 s timeout in microseconds
#define HECI_INIT_TIMEOUT        15000000  ///< 15 s timeout in microseconds
#define HECI_READ_TIMEOUT          500000  ///< 0.5 s timeout in microseconds
#define HECI_SEND_TIMEOUT          500000  ///< 0.5 s timeout in microseconds

/**
  ME HECI Protocol GUID
  This protocol provides an interface to communicate with Intel ME
  over HECI function on PCH ME Device.
**/
extern EFI_GUID gHeciTransportProtocolGuid;
extern EFI_GUID gHeciTransportSmmProtocolGuid;

///
/// HECI device information information
///
typedef struct {
  UINTN            HeciPch;
  UINTN            HeciSegment;
  UINTN            HeciBus;
  UINTN            HeciDevice;
  UINTN            HeciFunction;
  UINTN            HeciNo;
} HECI_DEVICE_INFO;

/**
  Get HECI information record

  @param[in]     This          Pointer to HECI Transport device context structure

  @retval HECI device information record pointer
**/
typedef HECI_DEVICE_INFO* (EFIAPI *HECI_TRANSPORT_GET_HECI_DEVICE_INFO) (
  IN      HECI_TRANSPORT   *This,
  IN      HECI_DEVICE_INFO *HeciDeviceInfo
  );

/**
  Get HECI Transport initialization state

  @param[in]     This          Pointer to HECI Transport device context structure

  @retval TRUE  HECI device is initialized
  @retval FALSE HECI device is not initialized
**/
typedef BOOLEAN (EFIAPI *HECI_TRANSPORT_GET_INIT_STATE) (
  IN      HECI_TRANSPORT   *This
  );

/**
  Get HECI Firmware Status

  @param[in]     This           Pointer to HECI Transport device context structure
  @param[in,out] FirmwareStatus Pointer to returned firmware status

  @retval EFI_SUCCESS           Returned valid firmware status
  @retval EFI_INVALID_PARAMETER Input parameters are invalid
  @retval EFI_DEVICE_ERROR      Error occurred
**/
typedef EFI_STATUS (EFIAPI *HECI_TRANSPORT_GET_FIRMWARE_STATUS) (
  IN     HECI_TRANSPORT    *This,
  IN OUT UINT32            *FirmwareStatus
  );

/**
  Initialize HECI interface.

  This function initializes host side of HECI interface. If timeout is
  greater than zero it also waits until ME is ready to receive messages.

  @param[in]     HeciTransport HECI access pointer the message has to be sent via
  @param[in,out] Timeout       On input timeout in us, on exit time left

  @retval EFI_INVALID_PARAMETER Input parameters are invalid
  @retval EFI_DEVICE_ERROR      Device not present or HECI MBar can't be initialized
  @retval EFI_SUCCESS           Initialization succeed
**/
typedef EFI_STATUS (EFIAPI *HECI_TRANSPORT_INIT) (
  IN     HECI_TRANSPORT      *HeciTransport,
  IN OUT UINT32              *Timeout
  );

/**
  Reset HECI queue.

  This function triggers HECI queue reset. If timeout is greater than zero it
  also waits until ME is ready to receive messages.

  @param[in]     HeciTransport HECI access pointer the message has to be sent via
  @param[in,out] Timeout       On input timeout in us, on exit time left

  @retval EFI_INVALID_PARAMETER Input parameters are invalid
  @retval EFI_DEVICE_ERROR      HECI MBar can't be initialized
  @retval EFI_TIMEOUT           Timeout occurs during operation
  @retval EFI_SUCCESS           Reset HECI queue succeed
**/
typedef EFI_STATUS (EFIAPI *HECI_TRANSPORT_RESET) (
  IN     HECI_TRANSPORT      *HeciTransport,
  IN OUT UINT32              *Timeout
  );

/**
  Send request message to HECI queue, wait for response if needed.

  This function writes one message to HECI queue and - if receive buffer
  was provided and timeout is greater than zero - waits for response message.
  The request can be retried if Retries > 0 (0 means one attempt, no retry),
  but if retries are requested, the ReqMsg and pRspMsg pointers must point
  to different buffers. This is required to make sure request is unchanged.
  Size of receive message buffer is given in bytes in (*RspLen) on input.
  On exit (*RspLen) provides the number of bytes written to the message
  buffer. If buffer is too short the message is truncated.

  @param[in]     HeciTransport HECI access pointer the message has to be sent via
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
  @retval EFI_DEVICE_ERROR      HECI MBar can't be initialized
  @retval EFI_TIMEOUT           Timeout occurs during operation
  @retval EFI_BAD_BUFFER_SIZE   Message is too big
  @retval EFI_SUCCESS           Message request has been completed successfully
**/
typedef EFI_STATUS (EFIAPI *HECI_TRANSPORT_SEND_AND_RECEIVE) (
  IN     HECI_TRANSPORT      *HeciTransport,
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

  @param[in]     HeciTransport HECI access pointer the message has to be sent via
  @param[in,out] Timeout       On input timeout in us, on exit time left
  @param[in]     HostAddress   Host address
  @param[in]     EngineAddress Engine (ME/IE) address
  @param[in]     MsgBody       Body of the message to send
  @param[in]     MsgLen        Length in bytes of the message to send

  @retval EFI_INVALID_PARAMETER Input parameters are invalid
  @retval EFI_DEVICE_ERROR      HECI MBar can't be initialized
  @retval EFI_TIMEOUT           Timeout occurs during operation
  @retval EFI_BAD_BUFFER_SIZE   Packet is bigger than queue length
  @retval EFI_SUCCESS           Message has been sent successfully
**/
typedef EFI_STATUS (EFIAPI *HECI_TRANSPORT_SEND) (
  IN     HECI_TRANSPORT      *HeciTransport,
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

  @param[in]     HeciTransport HECI Transport pointer the message has to be sent via
  @param[in,out] Timeout       On input timeout in us, on exit time left
  @param[out]    HostAddress   Host address
  @param[out]    EngineAddress Engine (ME/IE) address
  @param[out]    MsgBody       Body of the message to send
  @param[in,out] MsgLen        On input buffer size, on exit message body size in bytes

  @retval EFI_INVALID_PARAMETER Input parameters are invalid
  @retval EFI_DEVICE_ERROR      HECI MBar can't be initialized
  @retval EFI_TIMEOUT           Timeout occurs during operation
  @retval EFI_BAD_BUFFER_SIZE   Message exceeds buffer size
  @retval EFI_SUCCESS           Message has been received successfully
**/
typedef EFI_STATUS (EFIAPI *HECI_TRANSPORT_RECEIVE) (
  IN     HECI_TRANSPORT      *HeciTransport,
  IN OUT UINT32              *Timeout,
     OUT UINT8               *HostAddress,
     OUT UINT8               *EngineAddress,
     OUT UINT32              *MsgBody,
  IN OUT UINT32              *MsgLen
  );

/**
  Get Next HECI Transport

  @param[in]     This          Pointer to HECI Transport device context structure
  @param[in]     HeciTransport Pointer to HECI Transport device context structure
                               the next element should be relative to
                               Passing NULL requests first element

  @retval HECI_TRANSPORT Next HeciTransport object
**/
typedef HECI_TRANSPORT* (EFIAPI *HECI_TRANSPORT_GET_NEXT_HECI) (
  IN      HECI_TRANSPORT   *This,
  IN      HECI_TRANSPORT   *HeciTransport
  );

/**
  EFI protocol for ME-UEFI FW communication over HECI.

  This protocol defines operations for communication with ME using HECI
  (Host Embedded Controller Interface). There are more HECI functions in
  ME device on PCI. HECI is designated for ME-UEFI FW boot flow communication.
**/
typedef struct _HECI_TRANSPORT_ {
  HECI_TRANSPORT_GET_INIT_STATE       GetInitializationState;  /// < Get HECI Transport initialization state
  HECI_TRANSPORT_GET_HECI_DEVICE_INFO GetHeciDeviceInfo;       /// < Get HECI information record
  HECI_TRANSPORT_GET_FIRMWARE_STATUS  ReadHeciFirmwareStatus;  /// < Get HECI Firmware Status
  HECI_TRANSPORT_GET_NEXT_HECI        GetNextHeciTransport;    /// < HECI Transport protocol walker
  HECI_TRANSPORT_INIT                 HeciInit;                /// < Initialize HECI interface
  HECI_TRANSPORT_SEND_AND_RECEIVE     HeciSendAndReceive;      /// < Send HECI message and receive Ack
  HECI_TRANSPORT_SEND                 HeciSend;                /// < Send HECI message
  HECI_TRANSPORT_RECEIVE              HeciReceive;             /// < Receive HECI message
  HECI_TRANSPORT_RESET                HeciReset;               /// < Reset HECI interface
} HECI_TRANSPORT;

#endif // _HECI_TRANSPORT_PROTOCOL_H_

