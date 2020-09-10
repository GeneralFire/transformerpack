/** @file
  Header file for the Crystal Ridge Firmware Activation Protocol

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2020 Intel Corporation. <BR>

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

#ifndef _CR_FW_ACTIVATE_PROTOCOL_H_
#define _CR_FW_ACTIVATE_PROTOCOL_H_

#define CR_FW_ACTIVATE_PROTOCOL_GUID { 0x67efc0b3, 0x3806, 0x42cd, 0xbd, 0x4a, 0x6a, 0x2c, 0x02, 0xac, 0x63, 0x05 }

//
//  Crystal Ridge Firmware Activation Protocol versions
//
#define CR_FW_ACTIVATE_PROTOCOL_VER_1   1
#define CR_FW_ACTIVATE_PROTOCOL_VER_CUR CR_FW_ACTIVATE_PROTOCOL_VER_1

#define CR_INVALID_BSR_CONTENT                -1LL // That value is used by GET_DIMM_ACTIVATION_STATE when BSR data is not valid
#define CR_INVALID_MAILBOX_STATUS_CONTENT     -1LL // That value is used by GET_DIMM_ACTIVATION_STATE when Mailbox Status is not valid

//
// DIMM Arm State
//
typedef enum {
  CrArmStateDisarmed = 0,
  CrArmStateArmed     = 1
} CR_ARM_STATE;

//
// Type of activation requested
//
typedef enum {
  CrActivationTypeQuiesceOnly = 0, // Restricted
  CrActivationTypeNormal      = 1
} CR_ACTIVATION_TYPE;

//
// Common for both platform and DIMM
//
typedef enum {
  CrActivationStateIdle = 0,
  CrActivationStateArmed = 1,
  CrActivationStateBusy = 2
} CR_ACTIVATION_STATE;

//
// Platform activation status
//
typedef enum {
  CrPlatformActivationStatusSuccess = 0,
  CrPlatformActivationStatusNoArmedDevices = 1,
  CrPlatformActivationStatusQuiesceFailure = 2,
  CrPlatformActivationStatusTimeout = 3,
  CrPlatformActivationStatusFailed = 4
} CR_PLATFORM_ACTIVATION_STATUS;

//
// DIMM activation status
//
typedef enum {
  CrActivationStatusSuccess = 0,
  CrActivationStatusNotSentFailedToGetDoorbell = 1,
  CrActivationStatusNotSentDoorbellNotClear = 2,
  CrActivationStatusNotSentFailedToSendActivation = 3,
  CrActivationStatusSentFailedToGetDoorbell = 4,
  CrActivationStatusSentDoorbellNotClear = 5,
  CrActivationStatusSentFailedToGetMailboxStatus = 6,
  CrActivationStatusSentMailboxCompletionNotSet = 7,
  CrActivationStatusSentErrorInMailboxStatus = 8,
  CrActivationStatusSentFailedToGetBsr = 9,
  CrActivationStatusSentTimeout = 0xA,
  CrActivationStatusUnclassifiedError = 0xB
} CR_ACTIVATION_STATUS;

//
// Defining a handle to store information about protocol version supported by the protocol consumer
//
typedef UINT64 CR_HANDLE;

/**
  @brief Initialize handle value and return it to consumer. Needs to be called before any other function
         from the protocol. Handle stores information about protocol version supported by a consumer.
         It is used to provide backward compatibility - a function may work differently depending on consumer.

  @param[in]  ProtocolVersion   Version of protocol supported by the consumer
  @param[out] HandlerPtr        Pointer to CR_HANDLE; populated by the function upon successful return

  @retval     EFI_STATUS

 **/
typedef
EFI_STATUS
(EFIAPI *INITIALIZE) (
  IN     UINT8      ProtocolVersion,
     OUT CR_HANDLE  *HandlePtr
);

/**
  @brief Arm or disarm a DIMM - in other words this is to select (Arm) or unselect (Disarm) a DIMM to activate FW on it
         in the upcoming FW Activation.

  @param[in]  Handle       Protocol handle returned from protocol initialization
  @param[in]  Socket       Socket index (within a platform)
  @param[in]  Imc          Memory controller index (within a socket)
  @param[in]  Channel      Channel index (within a memory controller)
  @param[in]  Dimm         DIMM index (within a channel)
  @param[in]  ArmState     Arm state to set

  @retval     Status       EFI_STATUS; Status depends on result of communication with the agent (e.g. IPMI)
**/
typedef
EFI_STATUS
(EFIAPI *SET_ARM_STATE) (
  IN CR_HANDLE    Handle,
  IN UINT8        Socket,
  IN UINT8        Imc,
  IN UINT8        Channel,
  IN UINT8        Dimm,
  IN CR_ARM_STATE ArmState
  );

/**
  @brief Get DIMM arm state - check if DIMM is selected for activation (Armed) or not (Disarmed).

  @param[in]   Handle       Protocol handle returned from protocol initialization
  @param[in]   Socket       Socket index (within a platform)
  @param[in]   Imc          Memory controller index (within a socket)
  @param[in]   Channel      Channel index (within a memory controller)
  @param[in]   Dimm         DIMM index (within a channel)
  @param[out]  ArmStatePtr  Pointer to CR_ARM_STATE;
                            populated by the function upon successful return

  @retval      Status        EFI_STATUS; Status depends on result of communication with the agent (e.g. IPMI)

**/
typedef
EFI_STATUS
(EFIAPI *GET_ARM_STATE) (
  IN     CR_HANDLE    Handle,
  IN       UINT8        Socket,
  IN       UINT8        Imc,
  IN       UINT8        Channel,
  IN        UINT8        Dimm,
       OUT CR_ARM_STATE *ArmStatePtr
  );


/**
  @brief Activate firmware on armed DIMMs. New FW is should be staged on DIMMs prior to calling activation.

  @param[in]  Handle              Protocol handle returned from protocol initialization
  @param[in]  ActivationDeadline  Activation deadline hint to FW Activation agent expressed
                                  in milliseconds
  @param[in]  ActivationType      Requested activation type

  @retval     Status              EFI_STATUS; Status depends on result of communication with the agent (e.g. IPMI)
**/
typedef
EFI_STATUS
(EFIAPI *ACTIVATE_FW) (
  IN  CR_HANDLE           Handle,
  IN  UINT16              ActivationDeadline,
  IN  CR_ACTIVATION_TYPE  ActivationType
  );




/**
  @brief Get platform activation state

  @param[in]   Handle                Protocol handle returned from protocol initialization
  @param[out]  ActivationState       Pointer to CR_PLATFORM_ACTIVATION_STATE;
                                     populated by the function upon successful return
  @param[out]  LastActivationStatus  Pointer to CR_PLATFORM_ACTIVATION_STATUS;
                                     populated by the function upon successful return

  @retval      Status                EFI_STATUS; Status depends on result of communication with the agent (e.g. IPMI)
**/
typedef
EFI_STATUS
(EFIAPI *GET_PLATFORM_ACTIVATION_STATE) (
  IN      CR_HANDLE                      Handle,
     OUT  CR_ACTIVATION_STATE            *ActivationState,
     OUT  CR_PLATFORM_ACTIVATION_STATUS  *LastActivationStatus
  );


/**
  @brief Get DIMM activation state

  @param[in]   Handle                   Protocol handle returned from protocol initialization
  @param[in]   Socket                   Socket index (within a platform)
  @param[in]   Imc                      Memory controller index (within a socket)
  @param[in]   Channel                  Channel index (within a memory controller)
  @param[in]   Dimm                     DIMM index (within a channel)
  @param[out]  ActivationStatePtr       Pointer to CR_ACTIVATION_STATE;
                                        populated by the function upon successful return
  @param[out]  LastActivationStatusPtr  Pointer to CR_ACTIVATION_STATUS;
                                        populated by the function upon successful return
  @param[out]  BsrPtr                   BSR (Boot Status Register) value during last activation attempt.
                                        It is set to CR_INVALID_BSR_CONTENT when the value cannot be determined.
  @param[out]  MailboxStatusPtr         Mailbox Register value during last activation attempt.
                                        It is set to CR_INVALID_MAILBOX_STATUS_CONTENT when the value cannot be determined.

  @retval      Status                   EFI_STATUS; Status depends on result of communication with the agent (e.g. IPMI)
**/
typedef
EFI_STATUS
(EFIAPI *GET_DIMM_ACTIVATION_STATE) (
  IN      CR_HANDLE             Handle,
  IN      UINT8                 Socket,
  IN      UINT8                 Imc,
  IN      UINT8                 Channel,
  IN      UINT8                 Dimm,
     OUT  CR_ACTIVATION_STATE   *ActivationStatePtr,
     OUT  CR_ACTIVATION_STATUS  *LastActivationStatusPtr,
     OUT  UINT64                *BsrPtr,
     OUT  UINT64                *MailboxStatusPtr
  );


typedef struct {
  UINT16                                            ProtocolVersion;
  UINT8                                             Reserved[6];
  INITIALIZE                                        Initialize;
  SET_ARM_STATE                                     SetArmState;
  GET_ARM_STATE                                     GetArmState;
  ACTIVATE_FW                                       ActivateFw;
  GET_PLATFORM_ACTIVATION_STATE                     GetPlatformActivationState;
  GET_DIMM_ACTIVATION_STATE                         GetDimmActivationState;
} CR_FW_ACTIVATE_PROTOCOL;

extern EFI_GUID gCrFwActivateProtocolGuid;

#endif // _CR_FW_ACTIVATE_PROTOCOL_H_
